#include <gdk-pixbuf/gdk-pixbuf.h>

int main(int argc, char** argv){
	unsigned int width = 100;
	unsigned int height = 100;

	GdkPixbuf* pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8,width,height);
	int n_channels = gdk_pixbuf_get_n_channels(pixbuf);
	int rowstride = gdk_pixbuf_get_rowstride(pixbuf);
	guchar* pixels = gdk_pixbuf_get_pixels(pixbuf);

	for(unsigned int i=0; i<height; ++i){
		for(unsigned int j=0; j<width; ++j){
			guchar* pp = pixels + i*rowstride + j * n_channels;

			pp[0] = 139.0;
			pp[1] = 155.0;
			pp[2] = 20.0;
		}
	}
	gdk_pixbuf_save(pixbuf,"image.jpg","jpeg",NULL,"quality","100",NULL);
}	
