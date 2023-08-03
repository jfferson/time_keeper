/* time-keeper-application.cc
 * derived from gtkmm example Copyright (C) 2016 gtkmm development team
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */


#include "time-keeper-application.h"
#include "u-i-controller.h"
#include <iostream>
#include <exception>

//Gtk::ApplicationWindow * main_win = 0;

/*void activate_app(Gtk::Application * app)
{
	app->add_window(*main_win);
	//main_win->show();
	//main_win->show_all_children ();
	main_win->present();
}*/

TimeKeeperApplication::TimeKeeperApplication()
: Gtk::Application("org.gtkmm.examples.application", Gio::Application::Flags::DEFAULT_FLAGS)
{
}

Glib::RefPtr<TimeKeeperApplication> TimeKeeperApplication::create()
{
  return Glib::make_refptr_for_instance<TimeKeeperApplication>(new TimeKeeperApplication());
}

TimeKeeperWindow* TimeKeeperApplication::create_appwindow()
{
  appwindow = TimeKeeperWindow::create();

  // Make sure that the application runs for as long this window is still open.
  add_window(*appwindow);

  // A window can be added to an application with Gtk::Application::add_window()
  // or Gtk::Window::set_application(). When all added windows have been hidden
  // or removed, the application stops running (Gtk::Application::run() returns()),
  // unless Gio::Application::hold() has been called.

  // Delete the window when it is hidden.
  //appwindow->signal_hide().connect([appwindow](){ delete appwindow; });

  return appwindow;
}

void TimeKeeperApplication::on_activate()
{
  try
  {
    // The application has been started, so let's show a window.
    appwindow = create_appwindow();
    appwindow->present();
	UI_Controller * controller = new UI_Controller( ((appwindow->get_builder()).get()), this );
  }
  // If create_appwindow() throws an exception (perhaps from Gtk::Builder),
  // no window has been created, no window has been added to the application,
  // and therefore the application will stop running.
  catch (const Glib::Error& ex)
  {
    std::cerr << "ExampleApplication::on_activate(): " << ex.what() << std::endl;
  }
  catch (const std::exception& ex)
  {
    std::cerr << "ExampleApplication::on_activate(): " << ex.what() << std::endl;
  }
}
