/* time-keeper-window.h
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


#include <gtkmm.h>
#include <glibmm.h>

#include "config.h"

#ifdef ENABLE_NLS
#  include <libintl.h>
#endif

#include "u-i-controller.h"

// When testing use the local (not installed) UI_FILE, as in the comment
#define UI_FILE PACKAGE_DATA_DIR"/ui/time_keeper.ui"
//#define UI_FILE "src/time_keeper.ui"

#include <gtkmm.h>

class TimeKeeperWindow : public Gtk::ApplicationWindow
{
public:
  TimeKeeperWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);
  
  static TimeKeeperWindow* create();
  
  Glib::RefPtr<Gtk::Builder> get_builder() { return m_refBuilder; }

  //void open_file_view(const Glib::RefPtr<Gio::File>& file);

protected:
  Glib::RefPtr<Gtk::Builder> m_refBuilder;
};
