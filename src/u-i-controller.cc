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
	
	auto time_seed = Time_Serializer::load();
	
	for (int i=0; i < widgets.size(); i++){
		if (dynamic_cast<Gtk::Buildable*>(widgets.at(i).get())->get_buildable_id() == (Glib::ustring) "timer_grid"){
			initial_timer_grid = dynamic_cast<Gtk::Grid*>(widgets.at(i).get());
			//grid_counter++;
			//initial_timer = grid_counter;
		}
		if (dynamic_cast<Gtk::Buildable*>(widgets.at(i).get())->get_buildable_id() == (Glib::ustring) "scheduler_grid"){
			initial_scheduler_grid = dynamic_cast<Gtk::Grid*>(widgets.at(i).get());
			//grid_counter++;
			//initial_scheduler =grid_counter;
		}
	}
	
	bool initial_timer_loop = true;
	bool initial_scheduler_loop = true;
	
	if ( time_seed.size() == 0 ) std::cout << "vazio" << std::endl;
	//int time_grid_population =1;
	//if ( (time_seed.size() > grid_population) ) grid_population = time_seed.size();
	
	
	for (grid_counter=0; grid_counter < time_seed.size(); grid_counter++){
		if ( (time_seed.at(grid_counter)).event == Time_Keeper::event_type::cevent){
			if (initial_timer_loop) {
				complement_timer(initial_timer_grid, grid_counter, time_seed.size() ); //avoid collision for every add_timer function populated
				std::cout << "loop: " << grid_counter << std::endl;
				build_time_keeper(grid_counter);
				(bind_time [grid_counter]).regenerate( (time_seed.at(grid_counter)) );
				save(grid_counter);
				initial_timer_loop = false;
				// chamar funcao aqui
				//if ( (bind_time.contains(initial_timer)) )
				//	std::cout << "tudo legal" << std::endl;
				(dynamic_cast<Gtk::Label*>(initial_timer_grid->get_child_at(0,1)))->set_text((bind_time [grid_counter]).display_timer());
				//last_timer_loaded = dynamic_cast<Gtk::Widget*>(initial_timer_grid);
				//display->set_text((bind_time [initial_timer]).display_timer ());
			} else {
				grid_counter--; // due to self refferencing the function bellow also incremented, but that gives us problem in this loop
				add_timer(last_timer_loaded, time_seed.size() ); //avoid collision for every add_timer function populated
				std::cout << "loop: " << grid_counter << std::endl;
				build_time_keeper(grid_counter);
				(bind_time [grid_counter]).regenerate( (time_seed.at(grid_counter)) );
				save(grid_counter);
				(dynamic_cast<Gtk::Label*>( (dynamic_cast<Gtk::Grid*>(last_timer_loaded) )->get_child_at(0,1)) )->set_text((bind_time [grid_counter]).display_timer());
				//dynamic_cast<Gtk::Label*>(button_access->get_child_at(0,1))
				//display->set_text((bind_time [grid_counter]).display_timer ());
			}
		}
		if ( (time_seed.at(grid_counter)).event == Time_Keeper::event_type::fevent){
			if (initial_scheduler_loop) {
				complement_scheduler(initial_scheduler_grid, grid_counter, (time_seed.size() ) );
				build_time_keeper(grid_counter);
				(bind_time [initial_scheduler]).regenerate( (time_seed.at(grid_counter)) );
				save(grid_counter);
				initial_scheduler_loop = false;
				//(dynamic_cast<Gtk::Label*>(initial_timer_grid->get_child_at(0,1)))->set_text((bind_time [initial_timer]).display_timer());
				//last_scheduler_loaded = dynamic_cast<Gtk::Widget*>(initial_timer_grid);
			} else {
				
			}
		}
	}
	//if  (grid_counter > 0) grid_counter++;
	if (initial_timer_loop) complement_timer(initial_timer_grid, grid_counter, 0);
	if ( (initial_timer_loop && initial_scheduler_loop) ) grid_counter++;
	if (initial_scheduler_loop) complement_scheduler(initial_scheduler_grid, grid_counter,  0);
}
/*UI_Controller::~UI_Controller(void){
	for (std::unordered_map<int,Time_Keeper>::iterator i = bind_time.begin(); i != bind_time.end(); ++i){
		(i->second).stop();
	}

}*/

void UI_Controller::deffine_application(Gtk::Application * app)
{
	this->app = app;
}

bool UI_Controller::build_time_keeper(int caller_id){
	if (bind_time.find(caller_id) == bind_time.end() ){
		bind_time [caller_id] = *(new Time_Keeper());
		return true;
		//save_pos++;
		//r_caller[caller_id] = save_pos;
	} else {
		return false;
	}
}

void UI_Controller::start_timer(Gtk::Label * selected, int caller_id){
	build_time_keeper(caller_id);
	(bind_time [caller_id]).start_timer ();
		sigc::slot<bool()> my_slot = sigc::bind(sigc::mem_fun(*this,
		          &UI_Controller::timeout_timer), selected, caller_id);
		auto conn = Glib::signal_timeout().connect(my_slot, 100);
		timer_started = true;
}

void UI_Controller::set_event(Gtk::Widget * selected, int caller_id, Gtk::Widget * set_when){
	guint selected_year, selected_month, selected_day;
	build_time_keeper(caller_id); // ++
	scheduler_counter++;
	//bind_time[caller_id].set_name((dynamic_cast<Gtk::Entry*>(dynamic_cast<Gtk::Grid*>( (selected->get_ancestor(GTK_TYPE_GRID)) )->get_child_at(0,0))->get_buffer())->get_text()); // serializer issue
	events[scheduler_counter] = ((dynamic_cast<Gtk::Calendar*>(set_when))->get_date()); // adicionar data do campo de horarios
	(dynamic_cast<Gtk::Calendar*>(set_when))->mark_day( (events[scheduler_counter]).get_day_of_month() );
	bind_time[caller_id].set_dates_interval((events[scheduler_counter]));
	Gtk::Text * time_field = Gtk::manage( new Gtk::Text() );
	//time_field->set_max_length(8);
	//TimeBuffer * test_buffer = Gtk::manage( dynamic_cast<>(new TimeBuffer()) );
	//Glib::RefPtr<Gtk::EntryBuffer> test_buffer = (new TimeBuffer)->get_entry_buffer();
	//Glib::RefPtr<Gtk::EntryBuffer> test_buffer = test->get_buffer();
	Glib::RefPtr<TimeBuffer> time_buffer = TimeBuffer::create();
	//Gtk::TextBuffer::iterator test_pos = (dynamic_cast<Gtk::TextBuffer*>( (test_buffer.get()) ))->begin();
	//Gtk::TextBuffer::iterator test_pos = test_buffer->begin();
	//test->set_buffer(test_buffer);
	time_field->set_buffer(std::dynamic_pointer_cast<Gtk::EntryBuffer>(time_buffer));
	time_field->set_max_length(8);
	//dynamic_cast<Gtk::Editable>test->set_max_length(8);
	//test->set_max_width_chars(8);
	(dynamic_cast<Gtk::Box*>(selected))->append(*(dynamic_cast<Gtk::Widget*>(time_field)));
		//sigc::slot<bool()> my_slot = sigc::bind(sigc::mem_fun(*this,
		//          &UI_Controller::timeout_counter), (dynamic_cast<Gtk::Label*>(selected)), caller_id,when);
		//auto conn = Glib::signal_timeout().connect(my_slot, 1000);
		sigc::slot<bool()> my_slot = sigc::bind(sigc::mem_fun(*this, &UI_Controller::timeout_counter), time_field, caller_id,&(events[scheduler_counter]), ((const int) scheduler_counter));
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

bool UI_Controller::timeout_counter(Gtk::Text * time,int caller_id,Glib::DateTime * when, const int scheduler_aux){
	//display->set_text((bind_time [caller_id]).display_counter (when));
	if ( /*time != NULL*/ scheduler_aux == scheduler_counter ){
		if ( std::dynamic_pointer_cast<TimeBuffer>(time->get_buffer())->get_finished()){
			*when = when->add_hours(std::stoi(std::string((time->get_buffer()->get_text()).substr(0,2))));
			*when = when->add_minutes(std::stoi(std::string((time->get_buffer()->get_text()).substr(3,2))));
			*when = when->add_seconds(std::stoi(std::string((time->get_buffer()->get_text()).substr(6,2))));
			//std::cout << std::stoi(std::string((time->get_buffer()->get_text()).substr(0,2))) << std::endl;
			//std::cout << when.format_iso8601() << std::endl;
			Gtk::Box * time_box = dynamic_cast<Gtk::Box*>(time->get_ancestor(GTK_TYPE_BOX));
			scheduler_displays[ ((int) scheduler_aux) ] = new Gtk::Label;
			//time_box->insert_child_after(*(dynamic_cast<Gtk::Widget*>(scheduler_display)),*(dynamic_cast<Gtk::Widget*>(time)));
			time_box->prepend(*(dynamic_cast<Gtk::Widget*>( (scheduler_displays[ ((int) scheduler_aux) ]) )));
			time_box->remove(*(dynamic_cast<Gtk::Widget*>(time)));
			//if ( ((save_cycle.get())->elapsed()) > 10 ){
				//bind_time[caller_id].set_name((dynamic_cast<Gtk::Entry*>(dynamic_cast<Gtk::Grid*>( (display->get_ancestor(GTK_TYPE_GRID)) )->get_child_at(0,0))->get_buffer())->get_text()); //serializer issue
				//(save_cycle.get())->reset();
				//std::cout << bind_time[caller_id].get_name() << std::endl; // serializer issue
			scheduler_counter++;
			save(caller_id);
		}
	} else (scheduler_displays[ ((int) scheduler_aux) ])->set_text((bind_time [caller_id]).display_counter (*when));
	//}
	return true;
}

int UI_Controller::get_index(Glib::RefPtr<Glib::Object> target)
{
	auto it = find(widgets.begin(), widgets.end(), target);
	return (it - widgets.begin());
}

void UI_Controller::add_timer(Gtk::Widget * selected, int last_saved){
	Glib::RefPtr<Gtk::Builder> refference_grid = (Gtk::Builder::create_from_file(TIMER_GRID_UI));
	std::vector<Glib::RefPtr<Glib::Object>> control_widgets = refference_grid->get_objects();
	Glib::ustring widget_names="";
	Gtk::Grid * button_access;
	grid_counter++;
	//if ( (grid_counter == initial_scheduler) || (grid_counter == initial_timer) ) grid_counter++;
	
	for (int i=0; i < control_widgets.size(); i++){
		widget_names = widget_names+dynamic_cast<Gtk::Widget*>(control_widgets.at(i).get())->get_buildable_id()+"\n";
		if (dynamic_cast<Gtk::Buildable*>(control_widgets.at(i).get())->get_buildable_id() == (Glib::ustring) "timer_grid"){
			button_access = dynamic_cast<Gtk::Grid*>(control_widgets.at(i).get());
			dynamic_cast<Gtk::Box*>((selected->get_ancestor (GTK_TYPE_BOX)))->append(*(dynamic_cast<Gtk::Widget*>(button_access)) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(0,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::start_timer),dynamic_cast<Gtk::Label*>(button_access->get_child_at(0,1)),grid_counter ) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(1,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::stop_timer),grid_counter ) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(2,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::restart_timer),grid_counter ) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(3,0))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::add_timer), button_access, (grid_counter+last_saved) ) );
		}
	}
	last_timer_loaded = dynamic_cast<Gtk::Widget*>(button_access);
}

void UI_Controller::complement_timer(Gtk::Grid * button_access, int grid_counter, int last_saved){
	
	dynamic_cast<Gtk::Button*>(button_access->get_child_at(0,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::start_timer),dynamic_cast<Gtk::Label*>(button_access->get_child_at(0,1)),grid_counter ) );
	dynamic_cast<Gtk::Button*>(button_access->get_child_at(1,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::stop_timer),grid_counter ) );
	dynamic_cast<Gtk::Button*>(button_access->get_child_at(2,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::restart_timer),grid_counter ) );
	dynamic_cast<Gtk::Button*>(button_access->get_child_at(3,0))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::add_timer), button_access, (grid_counter + last_saved) ) );
	last_timer_loaded = dynamic_cast<Gtk::Widget*>(button_access);
}

void UI_Controller::complement_scheduler(Gtk::Grid * button_access, int grid_counter, int last_saved){
	dynamic_cast<Gtk::Button*>(button_access->get_child_at(0,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::set_event),(button_access->get_child_at(3,1)),grid_counter,(button_access->get_child_at(0,1))) );
	dynamic_cast<Gtk::Button*>(button_access->get_child_at(2,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::remove_event),grid_counter ) );
	dynamic_cast<Gtk::Button*>(button_access->get_child_at(4,0))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::add_scheduler),button_access, (grid_counter+last_saved) ) );
	last_scheduler_loaded = dynamic_cast<Gtk::Widget*>(button_access);
}


void UI_Controller::add_scheduler(Gtk::Widget * selected, int last_saved){
	Glib::RefPtr<Gtk::Builder> refference_grid = (Gtk::Builder::create_from_file(SCHEDULER_GRID_UI));
	std::vector<Glib::RefPtr<Glib::Object>> control_widgets = refference_grid->get_objects();
	Glib::ustring widget_names="";
	Gtk::Grid * button_access;
	grid_counter++;
	//if ( (grid_counter == initial_scheduler) || (grid_counter == initial_timer) ) grid_counter++;
	
	for (int i=0; i < control_widgets.size(); i++){
		if (dynamic_cast<Gtk::Buildable*>(control_widgets.at(i).get())->get_buildable_id() == (Glib::ustring) "scheduler_grid"){
			button_access = dynamic_cast<Gtk::Grid*>(control_widgets.at(i).get());
			dynamic_cast<Gtk::Box*>((selected->get_ancestor (GTK_TYPE_BOX)))->append(*(dynamic_cast<Gtk::Widget*>(button_access)) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(0,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::set_event),(button_access->get_child_at(3,1)),grid_counter,(button_access->get_child_at(0,1))) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(2,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::remove_event),grid_counter ) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(4,0))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::add_scheduler),button_access, (grid_counter+last_saved) ) );
		}
	}
	last_scheduler_loaded = dynamic_cast<Gtk::Widget*>(button_access);
}


