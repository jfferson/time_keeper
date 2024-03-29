/* time-keeper-window.cc
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

#include "time-keeper-window.h"
#include <stdexcept>

TimeKeeperWindow::TimeKeeperWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder) : Gtk::ApplicationWindow(cobject), m_refBuilder(builder)
{
}

//static
TimeKeeperWindow* TimeKeeperWindow::create()
{
  // Load the Builder file and instantiate its widgets.
  auto builder = Gtk::Builder::create_from_file(UI_FILE);

  auto window = Gtk::Builder::get_widget_derived<TimeKeeperWindow>(builder, "main_window");
  if (!window)
    throw std::runtime_error("No \"app_window\" object in window.ui");

  return window;
}

/*void ExampleAppWindow::open_file_view(const Glib::RefPtr<Gio::File>& /* file *///)
//{
//}
