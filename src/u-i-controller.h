/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * u-i-controller.h
 * Copyright (C) 2022 Unknown <jeff.lemos.a@gmail.com>
 *
 * time_keeper is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * time_keeper is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _U_I_CONTROLLER_H_
#define _U_I_CONTROLLER_H_

#include <gtkmm.h>
#include <unordered_map>
#include <glibmm/datetime.h>
#include <time-keeper.h>

namespace std{
	template <>
	struct hash<Time_Keeper>
	{
	 	size_t operator()( Time_Keeper& t) const
		{
			return t.hasheable().hash();
		}
 	};
}

class UI_Controller
{
public:
	UI_Controller(Gtk::Builder* refference,Gtk::Application * app);
	void deffine_application(Gtk::Application * app);
	void add_window_to_application(Gtk::Window * window);
protected:

private:
	Gtk::Builder * refference;
	Gtk::ApplicationWindow * content_relations;
	Gtk::Application * app;
	std::vector<Glib::RefPtr<Glib::Object>> widgets;
	std::unordered_map<int,Time_Keeper> bind_time;
	void show_window(Gtk::Window *window);
	void start_timer(Gtk::Widget * selected, int position);
	void stop_timer(Gtk::Widget * selected) { return ;};
	void restart_timer(Gtk::Widget * selected) { return ;};
	void add_timer(Gtk::Widget * selected) { return ;};
	

};

#endif // _U_I_CONTROLLER_H_

