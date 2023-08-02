/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.cc
 * Copyright (C) 2022 Jeferson de Lemos Pinheiro <jeff.lemos.a@gmail.com>
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

/*#include <gtkmm.h>
#include <glibmm.h>
#include <iostream>

#include "config.h"


#ifdef ENABLE_NLS
#  include <libintl.h>
#endif

#include "u-i-controller.h"

/* For testing propose use the local (not installed) ui file */
/* #define UI_FILE PACKAGE_DATA_DIR"/ui/time_keeper.ui" */
/*#define UI_FILE "src/time_keeper.ui"

Gtk::ApplicationWindow * main_win = 0;

void activate_app(Gtk::Application * app)
{
	app->add_window(*main_win);
	//main_win->show();
	//main_win->show_all_children ();
	main_win->present();
}

int
main (int argc, char *argv[])
{
	
	//auto app = Gtk::Application::create(argc,argv,"org.gtkmm.time_keeper");
	auto app = Gtk::Application::create("org.gtkmm.time_keeper", Gio::Application::Flags::NONE);

	//Load the Glade file and instiate its widgets:
	//Glib::RefPtr<Gtk::Builder> builder;
	auto window =nullptr;
	Glib::RefPtr<Gtk::Builder> builder;
	try
	{
		builder = Gtk::Builder::create_from_resource(UI_FILE);
	}
	catch (const Glib::FileError & ex)
	{
		std::cerr << ex.what() << std::endl;
		return 1;
	}
	
	// builder->get_widget("main_window", main_win);
	main_win = builder->get_widget<Gtk::ApplicationWindow>(builder,"main_window");

	if (main_win)
	{
		//app->signal_startup().connect(sigc::bind<&Gtk::Application>(sigc::ptr_fun(&activate_app), app.get() ) );
		app->signal_startup().connect(sigc::bind<0>(sigc::ptr_fun(&activate_app), app.get() ) );
		//UI_Controller * controller = new UI_Controller(builder.get(),app.get());
	}

	/*
	 * adiciona sinal -> executa adição de campo (-> busca origem do sinal)-> 
	 * adiciona sinal -> ...
	 * confirma controle -> executa funções de tempo
	 */

//}

#include "time-keeper-application.h"

int main(int argc, char* argv[])
{
	
	auto application = TimeKeeperApplication::create();
  

  // Start the application, showing the initial window,
  // and opening extra views for any files that it is asked to open,
  // for instance as a command-line parameter.
  // run() will return when the last window has been closed.
  return application->run(argc, argv);
}
