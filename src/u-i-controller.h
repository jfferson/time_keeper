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
#include <glibmm/timer.h>
#include <time-keeper.h>
#include <iostream>
#include <list>

//use those deffinitions when debugging
//#define TIMER_GRID_UI "src/n_time.ui"
//#define COUNTER_GRID_UI "src/n_counter.ui"

#define TIMER_GRID_UI PACKAGE_DATA_DIR"/ui/n_time.ui"
#define SCHEDULER_GRID_UI PACKAGE_DATA_DIR"/ui/n_counter.ui"

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
	~UI_Controller(void) { for (std::unordered_map<int,Time_Keeper>::iterator i = bind_time.begin(); i != bind_time.end(); ++i) (i->second).stop(i->first); };
protected:

private:
	std::shared_ptr<Glib::Timer> save_cycle;
	int grid_counter =-1;
	bool timer_started=false;
	Gtk::Builder * refference;
	Gtk::ApplicationWindow * content_relations;
	Gtk::Application * app;
	Gtk::Widget * last_timer_loaded;
	Gtk::Widget * last_scheduler_loaded;
	Gtk::Grid * initial_timer_grid;
	Gtk::Grid * initial_scheduler_grid;
	int initial_timer;
	int initial_scheduler;
	std::vector<Glib::RefPtr<Glib::Object>> widgets;
	std::unordered_map<int,Time_Keeper> bind_time;
	int save_pos = -1;
	//std::unordered_map<int,int> r_caller;
	void show_window(Gtk::Window *window);
	void start_timer(Gtk::Label * selected, int caller_id);
	void stop_timer(int caller_id) { if ( (bind_time.find(caller_id) != bind_time.end()) ) (bind_time[caller_id]).stop_timer ( (caller_id) ); };
	void restart_timer(int caller_id) { if ( (bind_time.find(caller_id) != bind_time.end()) ) (bind_time[caller_id]).reset_timer ();};
	void add_timer(Gtk::Widget * selected, int last_saved);
	void save_names();
	//void load();
	void save (int caller_id) { /*bind_time[caller_id].save(r_caller[caller_id]);*/ bind_time[caller_id].save( (caller_id) ); }; 
	bool timeout_timer(Gtk::Label * display,int caller_id);
	bool timeout_counter(Gtk::Label * display,int caller_id, Glib::DateTime when);
	int get_index(Glib::RefPtr<Glib::Object> target);
	void set_event (Gtk::Widget * selected, int caller_id, Gtk::Widget * set_when);
	void remove_event (int i){ (bind_time[i]).stop_counter();};
	void add_scheduler(Gtk::Widget * selected, int last_saved);
	bool build_time_keeper(int caller_id);
	void complement_timer(Gtk::Grid * timer_grid, int grid_counter, int last_saved); //the different names also avoid confusion on compilation
	void complement_scheduler(Gtk::Grid * scheduler_grid, int grid_counter, int last_saved);
};

#endif // _U_I_CONTROLLER_H_

