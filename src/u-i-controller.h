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

#include <unistd.h>
#include <gtkmm.h>
#include <unordered_map>
#include <glibmm/datetime.h>
#include <time-keeper.h>
#include <iostream>

#define TIMER_GRID_UI "src/n_time.ui"
#define COUNTER_GRID_UI "src/n_counter.ui"

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
	int grid_counter;
	bool timer_started=false;
	Gtk::Builder * refference;
	Gtk::ApplicationWindow * content_relations;
	Gtk::Application * app;
	std::vector<Glib::RefPtr<Glib::Object>> widgets;
	std::unordered_map<int,Time_Keeper> bind_time;
	void show_window(Gtk::Window *window);
	void start_timer(Gtk::Label * selected, int position);
	void stop_timer(int i) { (bind_time[i]).stop_timer (); };
	void restart_timer(int i) { (bind_time[i]).reset_timer ();};
	void add_timer(Gtk::Widget * selected);
	bool timeout_timer(Gtk::Label * display,int position);
	bool timeout_counter(Gtk::Label * display,int position, Glib::DateTime when);
	int get_index(Glib::RefPtr<Glib::Object> target);
	void start_counter (Gtk::Widget * selected, int position, Gtk::Widget * set_when);
	void stop_counter (int i){ (bind_time[i]).stop_counter();};
	void restart_counter(int i){ return ;};
	void add_counter(Gtk::Widget * selected);
	/*sigc::connection Glib::SignalTimeout::connect(const sigc::slot<bool()>& slot,
                                    unsigned int interval, int priority = Glib::PRIORITY_DEFAULT);*/
};

#endif // _U_I_CONTROLLER_H_

