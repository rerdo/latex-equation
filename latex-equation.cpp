#include "latex-equation.h"
#include <iostream>
#include <gdkmm/pixbuf.h>
#include <gtkmm/clipboard.h>

LatexEquation::LatexEquation()
: main_vbox(Gtk::ORIENTATION_VERTICAL),       // initialize box orientation
  button_hbox(Gtk::ORIENTATION_HORIZONTAL),   // initialize box orientation
  render_button("Render"),                    // render button label
  copy_button("Copy"),                        // copy button label
  save_button("Save")                         // save button label
{
    set_title("ROOT LaTeX Equation");
    set_default_size(1200, 400);

    // Configure vertical box
    main_vbox.set_spacing(10);
    main_vbox.set_border_width(10);
    add(main_vbox);

    // entry
    entry.set_placeholder_text("Type ROOT LaTeX here, e.g, #frac{1}{2} E=mc^{2}");
    main_vbox.pack_start(entry, Gtk::PACK_SHRINK);

    // connect enter in entry
    entry.signal_activate().connect(
        sigc::mem_fun(*this, &LatexEquation::entry_activated)
    );

    // display label
    main_vbox.pack_start(display, Gtk::PACK_EXPAND_WIDGET);

    // horizontal box for buttons
    button_hbox.set_spacing(10);
    main_vbox.pack_start(button_hbox, Gtk::PACK_SHRINK);

    // add buttons
    button_hbox.pack_start(render_button, Gtk::PACK_EXPAND_WIDGET);
    button_hbox.pack_start(copy_button, Gtk::PACK_EXPAND_WIDGET);
    button_hbox.pack_start(save_button, Gtk::PACK_EXPAND_WIDGET);

    // connect button signals
    render_button.signal_clicked().connect(
        sigc::mem_fun(*this, &LatexEquation::render_button_clicked)
    );
    save_button.signal_clicked().connect(
        sigc::mem_fun(*this, &LatexEquation::save_button_clicked)
    );
    copy_button.signal_clicked().connect(
        sigc::mem_fun(*this, &LatexEquation::copy_button_clicked)
    );

    show_all_children(); // show all widgets
}

LatexEquation::~LatexEquation() = default;

void LatexEquation::entry_activated()
{
    render_button_clicked(); // same as clicking render button
}

void LatexEquation::render_button_clicked()
{
    const auto latex = entry.get_text();
    if (latex.empty()) return;

    try{
        std::string filename = renderer.renderLatex(latex);

        // load image into a pixbuf and set it to Gtk::Image
        auto pixbuf = Gdk::Pixbuf::create_from_file(filename);
        display.set(pixbuf);
        current_pixbuf = pixbuf; // store current pixbuf

        show_all_children(); // make sure the image is shown
    }
    catch (const std::exception& e){
        std::cerr << "Error rendering LaTeX with ROOT: " << e.what() << std::endl;
    }
}

void LatexEquation::save_button_clicked()
{
    if (!current_pixbuf){
        std::cerr << "No image to save. Please render an equation first.\n";
        return;
    }

    // trim and make transparent
    auto trimmed = img_proc.trim_and_make_transparent(current_pixbuf);

    if (!trimmed){
        std::cerr << "Image processing failed.\n";
        return;
    }

    // save PNG
    img_proc.save_png(trimmed, "equation.png");

    // save SVG
    auto latex = entry.get_text();
    if (!latex.empty()){
        std::string svg_name = "equation.svg";
        renderer.renderLatexSvg(latex, svg_name);
    }
}

void LatexEquation::copy_button_clicked()
{
    if (!current_pixbuf){
        std::cerr << "No image to copy. Please render an equation first.\n";
    }

    // trim and make transparent but don't need to save
    auto trimmed = img_proc.trim_and_make_transparent(current_pixbuf);
    if (!trimmed){
        std::cerr << "Image processing failed. Nothing copied.\n";
        return;
    }

    // copy to clipboard
    auto clipboard = Gtk::Clipboard::get();
    if (clipboard){
        clipboard->set_image(trimmed);
        std::cout << "Image copied to clipboard.\n";
    }
    else{
        std::cerr << "Could not access clipboard.\n";
    }
}
