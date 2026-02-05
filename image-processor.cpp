#include "image-processor.h"
#include <iostream>

Glib::RefPtr<Gdk::Pixbuf> ImageProcessor::trim_and_make_transparent(
    const Glib::RefPtr<Gdk::Pixbuf>& src)
{
    // grab image info
    int width = src->get_width(); // image width in pixels
    int height = src->get_height(); // image height in pixels
    int rowstride = src->get_rowstride(); // number of bytes between rows
    int n_channels = src->get_n_channels(); // number of channels (3 for RGB, 4 for RGBA)
    guchar* pixels = src->get_pixels(); // pointer to pixel data

    // decide background color
    guchar bg_r = pixels[0];
    guchar bg_g = pixels[1];
    guchar bg_b = pixels[2];

    // find bounding box of non-background pixels
    int min_x = width, min_y = height;
    int max_x = -1, max_y = -1;
    for (int y = 0; y < height; ++y) {
        guchar* row = pixels + y * rowstride;
        for (int x = 0; x < width; ++x) {
            int idx = x * n_channels;
            guchar r = row[idx];
            guchar g = row[idx+1];
            guchar b = row[idx+2];

            bool is_bg = (r == bg_r && g == bg_g && b == bg_b);
            if (!is_bg) {
                if (x < min_x) min_x = x;
                if (x > max_x) max_x = x;
                if (y < min_y) min_y = y;
                if (y > max_y) max_y = y;
            }
        }
    }

    // if no non-background pixels found, return an empty image
    if (max_x < 0) return {};

    // define crop size
    int crop_w = max_x - min_x + 1;
    int crop_h = max_y - min_y + 1;

    // create new pixbuf for cropped image with alpha channel
    auto out = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB,
                                   true, // has alpha
                                   8, // bits per sample
                                   crop_w, crop_h);

    // get pointers for output image
    int out_stride = out->get_rowstride();
    int out_ch = out->get_n_channels();
    guchar* out_px = out->get_pixels();

    // copy pixels from source to output, making background transparent
    for (int y = 0; y < crop_h; ++y) {
        guchar* out_row = out_px + y * out_stride;
        guchar* src_row = pixels + (min_y + y) * rowstride;
        for (int x = 0; x < crop_w; ++x) {
            int s = (min_x + x) * n_channels;
            int d = x * out_ch;

            guchar r = src_row[s];
            guchar g = src_row[s + 1];
            guchar b = src_row[s + 2];

            bool is_bg = (r == bg_r && g == bg_g && b == bg_b);

            out_row[d] = r;
            out_row[d + 1] = g;
            out_row[d + 2] = b;
            out_row[d + 3] = is_bg ? 0 : 255; // set alpha
        }
}
    return out;
}

void ImageProcessor::save_png(
    const Glib::RefPtr<Gdk::Pixbuf>& pixbuf,
    const std::string& filename)
{
    if (!pixbuf) return;
    pixbuf->save(filename, "png");
}
