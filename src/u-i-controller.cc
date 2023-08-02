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
	//Gtk::Calendar * view_limit_time = new Gtk::Calendar();
	//view_limit_time->set_hexpand(true);
	deffine_application (app);
	this->refference = refference;
	content_relations = refference->get_widget<Gtk::ApplicationWindow>("main_window");
	widgets = refference->get_objects();
	Glib::ustring widget_names = "";
	for (int i=0; i < widgets.size(); i++){
		widget_names = widget_names+dynamic_cast<Gtk::Widget*>(widgets.at(i).get())->get_buildable_id()+"\n";
		std::cout <<  widget_names << std::endl;
		if (dynamic_cast<Gtk::Buildable*>(widgets.at(i).get())->get_buildable_id() == (Glib::ustring) "timer_grid"){
			
			Gtk::Grid * button_access = dynamic_cast<Gtk::Grid*>(widgets.at(i).get());
			grid_counter = i;
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(0,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::start_timer),dynamic_cast<Gtk::Label*>(button_access->get_child_at(0,1)),i ) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(1,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::stop_timer),i ) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(2,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::restart_timer),i ) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(3,0))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::add_timer),button_access ) );
			button_access->set_size_request(700,100);
		}
		if (dynamic_cast<Gtk::Buildable*>(widgets.at(i).get())->get_buildable_id() == (Glib::ustring) "counter_grid"){
			Gtk::Grid * button_access = dynamic_cast<Gtk::Grid*>(widgets.at(i).get());
			std::cout << "contador" << std::endl;
			//button_access->attach(*(dynamic_cast<Gtk::Widget*>(view_limit_time)),0,1,2,1);
			grid_counter = i;
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(0,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::start_counter),(button_access->get_child_at(2,1)),i,(button_access->get_child_at(0,1))) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(1,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::stop_counter),i ) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(2,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::restart_counter),i ) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(3,0))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::add_counter),button_access ) );
			button_access->set_size_request(700,300);
		}
	}
	//app->run();
}

void UI_Controller::deffine_application(Gtk::Application * app)
{
	this->app = app;
}

/*void UI_Controller::add_window_to_application (Gtk::Window * window)
{
	app->add_window(*window);
}

void UI_Controller::show_window(Gtk::Window * window)
{
	add_window_to_application(window);
	window->show();
	window->show_all_children();
}*/

void UI_Controller::start_timer(Gtk::Label * selected, int position){
	if (bind_time.find(position) == bind_time.end() ){
		bind_time [position] = *(new Time_Keeper());
	}
	(bind_time [position]).start_timer ();
	//refference->get_widget("timer_display",display);
	//if (!timer_started){
		sigc::slot<bool()> my_slot = sigc::bind(sigc::mem_fun(*this,
		          &UI_Controller::timeout_timer), selected, position);
		auto conn = Glib::signal_timeout().connect(my_slot, 100);
		timer_started = true;
	//}
}

void UI_Controller::start_counter(Gtk::Widget * selected, int position, Gtk::Widget * set_when){
	guint selected_year, selected_month, selected_day;
	if (bind_time.find(position) == bind_time.end() ){
		bind_time [position] = *(new Time_Keeper());
	}
	//(dynamic_cast<Gtk::Calendar*>(set_when))->get_date(selected_year,selected_month,selected_day);
	// there was some bug on the function above setting the month to 1 unity less,
	// remove the line bellow if it has already been corrected
	selected_month+=1;
	Glib::DateTime when = (Glib::DateTime::create_local(selected_year,selected_month,selected_day,1,1,1));
	//refference->get_widget("timer_display",display);
	//if (!timer_started){
		sigc::slot<bool()> my_slot = sigc::bind(sigc::mem_fun(*this,
		          &UI_Controller::timeout_counter), (dynamic_cast<Gtk::Label*>(selected)), position,when);
		auto conn = Glib::signal_timeout().connect(my_slot, 1000);
		timer_started = true;
	//}
}

bool UI_Controller::timeout_timer(Gtk::Label * display,int position){
	if ( ((bind_time [position]).get_timer_active()) ) display->set_text((bind_time [position]).display_timer ());
	return (bind_time [position]).get_timer_active();
}

bool UI_Controller::timeout_counter(Gtk::Label * display,int position,Glib::DateTime when){
	/*if ( ((bind_time [position]).get_counter_active()) )*/ display->set_text((bind_time [position]).display_counter (when));
	return /*(bind_time [position]).get_counter_active()*/ true;
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
			dynamic_cast<Gtk::Box*>((selected->get_ancestor (GTK_TYPE_BOX)))->append(*(dynamic_cast<Gtk::Widget*>(button_access))/*,true,true*/);
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(0,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::start_timer),dynamic_cast<Gtk::Label*>(button_access->get_child_at(0,1)),grid_counter ) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(1,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::stop_timer),grid_counter ) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(2,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::restart_timer),grid_counter ) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(3,0))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::add_timer),button_access ) );
			button_access->set_size_request(700,100);
			//dynamic_cast<Gtk::ScrolledWindow*>((button_access->get_ancestor (GTK_TYPE_SCROLLED_WINDOW)))->set_min_content_height(300);
		}
	}
}

void UI_Controller::add_counter(Gtk::Widget * selected){
	Glib::RefPtr<Gtk::Builder> refference_grid = (Gtk::Builder::create_from_file(COUNTER_GRID_UI));
	std::vector<Glib::RefPtr<Glib::Object>> control_widgets = refference_grid->get_objects();
	Glib::ustring widget_names="";
	Gtk::Grid * button_access;
	grid_counter++;
	//Gtk::Calendar * view_limit_time = new Gtk::Calendar();
	//view_limit_time->set_hexpand (true);
	//view_limit_time->set_visible(true);
	
	for (int i=0; i < control_widgets.size(); i++){
		widget_names = widget_names+dynamic_cast<Gtk::Widget*>(control_widgets.at(i).get())->get_buildable_id()+"\n";
		if (dynamic_cast<Gtk::Buildable*>(control_widgets.at(i).get())->get_buildable_id() == (Glib::ustring) "counter_grid"){
			button_access = dynamic_cast<Gtk::Grid*>(control_widgets.at(i).get());
			dynamic_cast<Gtk::Box*>((selected->get_ancestor (GTK_TYPE_BOX)))->append(*(dynamic_cast<Gtk::Widget*>(button_access))/*,true,true*/);
			//button_access->attach(*(dynamic_cast<Gtk::Widget*>(view_limit_time)),0,1,2,1);
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(0,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::start_counter),(button_access->get_child_at(2,1)),i,(button_access->get_child_at(0,1))) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(1,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::stop_counter),grid_counter ) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(2,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::restart_counter),grid_counter ) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(3,0))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::add_counter),button_access ) );
			button_access->set_size_request(700,300);
			//dynamic_cast<Gtk::ScrolledWindow*>((button_access->get_ancestor (GTK_TYPE_SCROLLED_WINDOW)))->set_min_content_height(300);
		}
	}
}
