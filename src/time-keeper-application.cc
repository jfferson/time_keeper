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
  add_window(*appwindow);

  //appwindow->signal_hide().connect([appwindow](){ delete appwindow; });

  return appwindow;
}

void TimeKeeperApplication::on_activate()
{
  try
  {
    appwindow = create_appwindow();
    appwindow->present();
	UI_Controller * controller = new UI_Controller( ((appwindow->get_builder()).get()), this );
  }
  catch (const Glib::Error& ex)
  {
    std::cerr << "ExampleApplication::on_activate(): " << ex.what() << std::endl;
  }
  catch (const std::exception& ex)
  {
    std::cerr << "ExampleApplication::on_activate(): " << ex.what() << std::endl;
  }
}
