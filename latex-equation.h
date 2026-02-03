#ifndef GTKMM_LATEX_EQUATION_H
#define GTKMM_LATEX_EQUATION_H

#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/image.h>
#include <gtkmm/window.h>
#include <gdkmm/pixbuf.h>

#include "root-renderer.h"
#include "image-processor.h"

class LatexEquation : public Gtk::Window
{
public:
     LatexEquation();
    ~LatexEquation() override;

protected:
    // signal handlers
    void save_button_clicked();   // save button
    void render_button_clicked(); // render button
    void copy_button_clicked();   // copy button
    void entry_activated();       // press Enter in entry

    // vertical Layout
    Gtk::Box   main_vbox;    // vertical layout
    Gtk::Entry entry;        // input line
    Gtk::Image display;      // To display rendered equation

    // horizontal Layout for buttons
    Gtk::Box    button_hbox;   // horizontal layout for buttons
    Gtk::Button save_button;   // save button
    Gtk::Button render_button; // render button
    Gtk::Button copy_button;   // copy button

    // render via Root
    RootRenderer renderer;     // renderer instance

    // image processor
    Glib::RefPtr<Gdk::Pixbuf> current_pixbuf;
    ImageProcessor img_proc; // image processor instance
};

#endif // GTKMM_LATEX_EQUATION_H
