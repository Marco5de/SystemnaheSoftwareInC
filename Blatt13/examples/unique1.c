#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stralloc.h>
#include <string.h>
#include <unistd.h>

char* cmdname;
stralloc tmpfile = {0};
bool tmpfile_created = false;

/* print an out of memory message to standard error and exit */
void memerr() {
   static char memerrmsg[] = "out of memory error\n";
   write(2, memerrmsg, sizeof(memerrmsg) - 1);
   if (tmpfile_created) unlink(tmpfile.s);
   exit(1);
}

/* print a error message to standard error and exit;
   include "message" in the output message, if not 0,
   otherwise strerror(errno) is being used
*/
void die(char* filename, char* message) {
   stralloc msg = {0};
   if (stralloc_copys(&msg, cmdname) &&
         stralloc_cats(&msg, ": ") &&
         (
            message?
               stralloc_cats(&msg, message)
            :
               stralloc_cats(&msg, strerror(errno))
         ) &&
         stralloc_cats(&msg, ": ") &&
         stralloc_cats(&msg, filename) &&
         stralloc_cats(&msg, "\n")) {
      write(2, msg.s, msg.len);
   } else {
      memerr();
   }
   if (tmpfile_created) unlink(tmpfile.s);
   exit(1);
}

void randsleep() {
   static int invocations = 0;
   if (invocations == 0) {
      srand(getpid());
   }
   ++invocations;
   /* determine timeout value (in milliseconds) */
   int timeout = rand() % (10 * invocations + 100);
   if (poll(0, 0, timeout) < 0) die("poll", 0);
}

int main(int argc, char* argv[]) {
   /* process command line arguments */
   cmdname = argv[0];
   if (argc != 2) {
      stralloc usage = {0};
      if (stralloc_copys(&usage, "Usage: ") &&
            stralloc_cats(&usage, cmdname) &&
            stralloc_cats(&usage, " counter\n")) {
         write(2, usage.s, usage.len);
      } else {
         memerr();
      }
      exit(1);
   }
   char* counter_file = argv[1];

   /* try to open the temporary file which also serves as a lock */
   if (!stralloc_copys(&tmpfile, counter_file) ||
         !stralloc_cats(&tmpfile, ".tmp") ||
         !stralloc_0(&tmpfile)) {
      memerr();
   }
   int outfd;
   for (int tries = 0; tries < 100; ++tries) {
      outfd = open(tmpfile.s, O_WRONLY|O_CREAT|O_EXCL, 0666);
      if (outfd >= 0) break;
      if (errno != EEXIST) break;
      randsleep();
   }
   if (outfd < 0) die(tmpfile.s, 0);
   tmpfile_created = true;

   /* determine current value of the counter */
   int current_value;
   int infd = open(counter_file, O_RDONLY);
   if (infd >= 0) {
      char buf[512];
      ssize_t nbytes = read(infd, buf, sizeof buf);
      if (nbytes <= 0) die(counter_file, 0);
      current_value = 0;
      for (char* cp = buf; cp < buf + nbytes; ++cp) {
         if (!isdigit(*cp)) die(counter_file, "decimal digits expected");
         current_value = current_value * 10 + *cp - '0';
      }
   } else if (errno != ENOENT) {
      die(counter_file, 0);
   } else {
      /* start a new counter */
      current_value = 0;
   }
   
   /* increment the counter and write it to the tmpfile */
   ++current_value;
   stralloc outbuf = {0};
   if (!stralloc_copys(&outbuf, "") ||
         !stralloc_catint(&outbuf, current_value)) {
      memerr();
   }
   int nbytes = write(outfd, outbuf.s, outbuf.len);
   if (nbytes < outbuf.len) die(tmpfile.s, 0);
   if (fsync(outfd) < 0) die(tmpfile.s, 0);
   if (close(outfd) < 0) die(tmpfile.s, 0);

   /* update counter file atomically by a rename */
   if (rename(tmpfile.s, counter_file) < 0) die(counter_file, 0);
   tmpfile_created = false;

   /* write counter value to stdout */
   if (!stralloc_cats(&outbuf, "\n")) memerr();
   nbytes = write(1, outbuf.s, outbuf.len);
   if (nbytes < outbuf.len) die("stdout", 0);
}
