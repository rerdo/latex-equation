#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include <gdkmm/pixbuf.h>
#include <string>

class ImageProcessor
{
    public:
        // Trim transparent borders, make background fully transparent and return new Pixbuf
        Glib::RefPtr<Gdk::Pixbuf> trim_and_make_transparent(
            const Glib::RefPtr<Gdk::Pixbuf>& src_pixbuf
        );

        // save Pixbuf as PNG
        void save_png(
            const Glib::RefPtr<Gdk::Pixbuf>& pixbuf,
            const std::string& filename
        );
};

#endif
