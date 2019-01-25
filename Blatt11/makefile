# the compiler: gcc for C program, define as g++ for C++
  CC = gcc

  # compiler flags:
  #  -g    adds debugging information to the executable file
  #  -Wall turns on most, but not all, compiler warnings
  CFLAGS  = -Wall 
  LINKF = -lm -lowfat

  # the build target executable:
  TARGET = game
  HASH = hashing

run:
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c $(HASH).c $(LINKF)

clean:
	$(RM) $(TARGET)
