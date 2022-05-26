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
	refference->get_widget("main_window",this->content_relations);
	widgets = refference->get_objects();
	Glib::ustring widget_names = "";
	for (int i=0; i < widgets.size(); i++){
		widget_names = widget_names+dynamic_cast<Gtk::Widget*>(widgets.at(i).get())->get_name()+"\n";
		if (dynamic_cast<Gtk::Buildable*>(widgets.at(i).get())->get_name() == (Glib::ustring) "timer_grid"){
			Gtk::Grid * button_access = dynamic_cast<Gtk::Grid*>(widgets.at(i).get());
			grid_counter = i;
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(0,2))->signal_clicked().connect(sigc::bind<Gtk::Label*>(sigc::mem_fun(*this,&UI_Controller::start_timer),dynamic_cast<Gtk::Label*>(button_access->get_child_at(0,1)),i ) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(1,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::stop_timer),i ) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(2,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::restart_timer),i ) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(3,0))->signal_clicked().connect(sigc::bind<Gtk::Widget*>(sigc::mem_fun(*this,&UI_Controller::add_timer),button_access ) );
		}
	}
	app->run();
}

void UI_Controller::deffine_application(Gtk::Application * app)
{
	this->app = app;
}

void UI_Controller::add_window_to_application (Gtk::Window * window)
{
	app->add_window(*window);
}

void UI_Controller::show_window(Gtk::Window * window)
{
	add_window_to_application(window);
	window->show();
	window->show_all_children();
}

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

bool UI_Controller::timeout_timer(Gtk::Label * display,int position){
	if ( ((bind_time [position]).get_active()) ) display->set_text((bind_time [position]).display_timer ());
	return (bind_time [position]).get_active();
}

int UI_Controller::get_index(Glib::RefPtr<Glib::Object> target)
{
	auto it = find(widgets.begin(), widgets.end(), target);
	return (it - widgets.begin());
}

void UI_Controller::add_timer(Gtk::Widget * selected){
	Glib::RefPtr<Gtk::Builder> refference_grid = (Gtk::Builder::create_from_file(GRID_UI));
	std::vector<Glib::RefPtr<Glib::Object>> control_widgets = refference_grid->get_objects();
	Glib::ustring widget_names="";
	Gtk::Grid * button_access;
	grid_counter++;
	for (int i=0; i < control_widgets.size(); i++){
		widget_names = widget_names+dynamic_cast<Gtk::Widget*>(control_widgets.at(i).get())->get_name()+"\n";
		if (dynamic_cast<Gtk::Buildable*>(control_widgets.at(i).get())->get_name() == (Glib::ustring) "timer_grid"){
			button_access = dynamic_cast<Gtk::Grid*>(control_widgets.at(i).get());
			dynamic_cast<Gtk::Box*>((selected->get_ancestor (GTK_TYPE_BOX)))->pack_end(*(dynamic_cast<Gtk::Widget*>(button_access)),true,true);
			//(dynamic_cast<Gtk::Grid*>(button_access))->show();
			//(dynamic_cast<Gtk::Grid*>(button_access))->show_all_children ();
			//(dynamic_cast<Gtk::Box*>(button_access))->show();
			//(dynamic_cast<Gtk::Box*>(button_access))->show_all_children ();
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(0,2))->signal_clicked().connect(sigc::bind<Gtk::Label*>(sigc::mem_fun(*this,&UI_Controller::start_timer),dynamic_cast<Gtk::Label*>(button_access->get_child_at(0,1)),grid_counter ) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(1,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::stop_timer),grid_counter ) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(2,2))->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&UI_Controller::restart_timer),grid_counter ) );
			dynamic_cast<Gtk::Button*>(button_access->get_child_at(3,0))->signal_clicked().connect(sigc::bind<Gtk::Widget*>(sigc::mem_fun(*this,&UI_Controller::add_timer),button_access ) );
		}
	}
}