#include "latex-equation.h"
#include <gtkmm/application.h>

int main(int argc, char* argv[])
{
    // gtkmm-3 version of creating an app:
  auto app = Gtk::Application::create(argc,
				      argv,
				      "org.gtkmm.rootlatexequation");

  LatexEquation window;

  // show the window and enter the GTK loop
  return app->run(window);
}
