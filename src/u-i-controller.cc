/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * u-i-controller.cc
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

#include "u-i-controller.h"

UI_Controller::UI_Controller(Gtk::Builder * refference, Gtk::Application * app)
{
	deffine_application (app);
	this->refference = refference;
	content_relations = refference->get_widget<Gtk::ApplicationWindow>("main_window");
	widgets = refference->get_objects();
	Glib::ustring widget_names = "";
	save_cycle = std::shared_ptr<Glib::Timer>(new Glib::Timer);
	(save_cycle.get())->start();
	for (int i=0; i < widgets.size(); i++){
		if (dynamic_cast<Gtk::Buildable*>(widgets.at(i).get())->get_buildable_id() == (Glib::ustring) "timer_grid"){
			
			Gtk::Grid * button_access = dynamic_cast<Gtk::Grid*>(widgets.at(i).get());
			grid_counter = i;
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(0,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::start_timer),dynamic_cast<Gtk::Label*>(button_access->get_child_at(0,1)),i ) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(1,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::stop_timer),i ) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(2,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::restart_timer),i ) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(3,0))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::add_timer),button_access ) );
		}
		if (dynamic_cast<Gtk::Buildable*>(widgets.at(i).get())->get_buildable_id() == (Glib::ustring) "counter_grid"){
			Gtk::Grid * button_access = dynamic_cast<Gtk::Grid*>(widgets.at(i).get());
			grid_counter = i;
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(0,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::start_counter),(button_access->get_child_at(2,1)),i,(button_access->get_child_at(0,1))) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(1,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::stop_counter),i ) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(2,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::restart_counter),i ) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(3,0))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::add_counter),button_access ) );
		}
	}
	//Time_Serializer::load(); does not work yet
}

void UI_Controller::deffine_application(Gtk::Application * app)
{
	this->app = app;
}

void UI_Controller::save(int caller_id){
	//auto ensure_unique = std::find(r_caller.begin(), r_caller.end(), caller_id);
	if (r_caller.find(caller_id) == r_caller.end()){
		save_pos++;
		r_caller[caller_id] = save_pos;
		//r_caller.push_back(caller_id);
	}
	bind_time[caller_id].save(r_caller[caller_id]);
}

void UI_Controller::start_timer(Gtk::Label * selected, int caller_id){
	if (bind_time.find(caller_id) == bind_time.end() ){
		bind_time [caller_id] = *(new Time_Keeper());
	}
	(bind_time [caller_id]).start_timer ();
		sigc::slot<bool()> my_slot = sigc::bind(sigc::mem_fun(*this,
		          &UI_Controller::timeout_timer), selected, caller_id);
		auto conn = Glib::signal_timeout().connect(my_slot, 100);
		timer_started = true;
}

void UI_Controller::start_counter(Gtk::Widget * selected, int caller_id, Gtk::Widget * set_when){
	guint selected_year, selected_month, selected_day;
	if (bind_time.find(caller_id) == bind_time.end() ){
		bind_time [caller_id] = *(new Time_Keeper());
	}
	//bind_time[caller_id].set_name((dynamic_cast<Gtk::Entry*>(dynamic_cast<Gtk::Grid*>( (selected->get_ancestor(GTK_TYPE_GRID)) )->get_child_at(0,0))->get_buffer())->get_text()); // serializer issue
	Glib::DateTime when = (dynamic_cast<Gtk::Calendar*>(set_when))->get_date();
	bind_time[caller_id].set_dates_interval(when);
		sigc::slot<bool()> my_slot = sigc::bind(sigc::mem_fun(*this,
		          &UI_Controller::timeout_counter), (dynamic_cast<Gtk::Label*>(selected)), caller_id,when);
		auto conn = Glib::signal_timeout().connect(my_slot, 1000);
		timer_started = true;
}

bool UI_Controller::timeout_timer(Gtk::Label * display, int caller_id){
	if ( ((bind_time [caller_id]).get_timer_active()) ) display->set_text((bind_time [caller_id]).display_timer ());
	std::cout << (save_cycle.get())->elapsed() << std::endl;
	if ( ((save_cycle.get())->elapsed()) > 10 ){
		bind_time[caller_id].set_duration();
		//bind_time[caller_id].set_name((dynamic_cast<Gtk::Entry*>(dynamic_cast<Gtk::Grid*>( (display->get_ancestor(GTK_TYPE_GRID)) )->get_child_at(0,0))->get_buffer())->get_text()); //serializer issue
		//bind_time[caller_id].set_name((dynamic_cast<Gtk::Entry*>(( dynamic_cast<Gtk::Grid*>(widgets.at(position).get()) )->get_child_at(0,0))->get_buffer())->get_text());
		(save_cycle.get())->reset();
		//std::cout << bind_time[caller_id].get_name() << std::endl; // serializer issue
		save(caller_id);
	}
	return (bind_time [caller_id]).get_timer_active();
}

bool UI_Controller::timeout_counter(Gtk::Label * display,int caller_id,Glib::DateTime when){
	display->set_text((bind_time [caller_id]).display_counter (when));
	if ( ((save_cycle.get())->elapsed()) > 10 ){
		//bind_time[caller_id].set_name((dynamic_cast<Gtk::Entry*>(dynamic_cast<Gtk::Grid*>( (display->get_ancestor(GTK_TYPE_GRID)) )->get_child_at(0,0))->get_buffer())->get_text()); //serializer issue
		(save_cycle.get())->reset();
		//std::cout << bind_time[caller_id].get_name() << std::endl; // serializer issue
		save(caller_id);
	}
	return true;
}

int UI_Controller::get_index(Glib::RefPtr<Glib::Object> target)
{
	auto it = find(widgets.begin(), widgets.end(), target);
	return (it - widgets.begin());
}

void UI_Controller::add_timer(Gtk::Widget * selected){
	Glib::RefPtr<Gtk::Builder> refference_grid = (Gtk::Builder::create_from_file(TIMER_GRID_UI));
	std::vector<Glib::RefPtr<Glib::Object>> control_widgets = refference_grid->get_objects();
	Glib::ustring widget_names="";
	Gtk::Grid * button_access;
	grid_counter++;
	for (int i=0; i < control_widgets.size(); i++){
		widget_names = widget_names+dynamic_cast<Gtk::Widget*>(control_widgets.at(i).get())->get_buildable_id()+"\n";
		if (dynamic_cast<Gtk::Buildable*>(control_widgets.at(i).get())->get_buildable_id() == (Glib::ustring) "timer_grid"){
			button_access = dynamic_cast<Gtk::Grid*>(control_widgets.at(i).get());
			dynamic_cast<Gtk::Box*>((selected->get_ancestor (GTK_TYPE_BOX)))->append(*(dynamic_cast<Gtk::Widget*>(button_access)) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(0,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::start_timer),dynamic_cast<Gtk::Label*>(button_access->get_child_at(0,1)),grid_counter ) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(1,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::stop_timer),grid_counter ) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(2,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::restart_timer),grid_counter ) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(3,0))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::add_timer),button_access ) );
		}
	}
}

void UI_Controller::add_counter(Gtk::Widget * selected){
	Glib::RefPtr<Gtk::Builder> refference_grid = (Gtk::Builder::create_from_file(COUNTER_GRID_UI));
	std::vector<Glib::RefPtr<Glib::Object>> control_widgets = refference_grid->get_objects();
	Glib::ustring widget_names="";
	Gtk::Grid * button_access;
	grid_counter++;
	
	for (int i=0; i < control_widgets.size(); i++){
		if (dynamic_cast<Gtk::Buildable*>(control_widgets.at(i).get())->get_buildable_id() == (Glib::ustring) "counter_grid"){
			button_access = dynamic_cast<Gtk::Grid*>(control_widgets.at(i).get());
			dynamic_cast<Gtk::Box*>((selected->get_ancestor (GTK_TYPE_BOX)))->append(*(dynamic_cast<Gtk::Widget*>(button_access)) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(0,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::start_counter),(button_access->get_child_at(2,1)),i,(button_access->get_child_at(0,1))) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(1,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::stop_counter),grid_counter ) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(2,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::restart_counter),grid_counter ) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(3,0))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::add_counter),button_access ) );
		}
	}
}
