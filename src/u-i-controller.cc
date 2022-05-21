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
		if (dynamic_cast<Gtk::Buildable*>(widgets.at(i).get())->get_name() == (Glib::ustring) "start_timer"){
			//dynamic_cast<Gtk::Widget*>(widgets.at(i).get())->get_ancestor(GTK_TYPE_BOX)
			//dynamic_cast<Gtk::Button*>(widgets.at(i).get())->signal_clicked().connect(sigc::mem_fun(*this,&Controlador_UI::botao_acionado));
			dynamic_cast<Gtk::Button*>(widgets.at(i).get())->signal_clicked().connect(sigc::bind<Gtk::Widget*>(sigc::mem_fun(*this,&UI_Controller::start_timer),dynamic_cast<Gtk::Widget*>(widgets.at(i).get()),i ) );
		}
		if (dynamic_cast<Gtk::Buildable*>(widgets.at(i).get())->get_name() == (Glib::ustring) "stop_timer"){
			dynamic_cast<Gtk::Button*>(widgets.at(i).get())->signal_clicked().connect(sigc::bind<Gtk::Widget*>(sigc::mem_fun(*this,&UI_Controller::stop_timer),dynamic_cast<Gtk::Widget*>(widgets.at(i).get()) ) );
		}
		if (dynamic_cast<Gtk::Buildable*>(widgets.at(i).get())->get_name() == (Glib::ustring) "restart_timer"){
			dynamic_cast<Gtk::Button*>(widgets.at(i).get())->signal_clicked().connect(sigc::bind<Gtk::Widget*>(sigc::mem_fun(*this,&UI_Controller::restart_timer),dynamic_cast<Gtk::Widget*>(widgets.at(i).get()) ) );
		}
		if (dynamic_cast<Gtk::Buildable*>(widgets.at(i).get())->get_name() == (Glib::ustring) "add_timer"){
			dynamic_cast<Gtk::Button*>(widgets.at(i).get())->signal_clicked().connect(sigc::bind<Gtk::Widget*>(sigc::mem_fun(*this,&UI_Controller::add_timer),dynamic_cast<Gtk::Widget*>(widgets.at(i).get()) ) );
		}
	}
	//janela_mensagem (nomes_widgets);
	app->run();
	//Gtk::ApplicationWindow * janela = relacoes_conteudo->get_ancestor(GTK_TYPE_APPLICATION_WINDOW);
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

void UI_Controller::start_timer(Gtk::Widget * selected, int position){
	if (bind_time.find(position) == bind_time.end() ){
		Time_Keeper * viado = new Time_Keeper();
		bind_time [position] = *viado;
	}
	Gtk::Label * display;
	refference->get_widget("timer_display",display);
	(bind_time [position]).start_timer ();
	while(true) display->set_text((bind_time [position]).display_timer ());
}

