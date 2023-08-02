/* time-keeper-application.h
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
#include "time-keeper-window.h"

class TimeKeeperApplication : public Gtk::Application{
protected:
    TimeKeeperApplication();
    void on_activate() override;
    //void on_open(const Gio::Application::type_vec_files& files, const Glib::ustring& hint) override;
public:
    static Glib::RefPtr<TimeKeeperApplication> create();
    TimeKeeperWindow * appwindow;
private:
    TimeKeeperWindow* create_appwindow();
};
