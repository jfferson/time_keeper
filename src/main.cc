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

#include <gtkmm.h>
#include <iostream>

#include "config.h"


#ifdef ENABLE_NLS
#  include <libintl.h>
#endif

#include "u-i-controller.h"

/* For testing propose use the local (not installed) ui file */
/* #define UI_FILE PACKAGE_DATA_DIR"/ui/time_keeper.ui" */
#define UI_FILE "src/time_keeper.ui"

Gtk::ApplicationWindow * main_win = nullptr;
Glib::RefPtr<Gtk::Builder> builder = nullptr;

void activate_app(Gtk::Application * app)
{
	app->add_window(*main_win);
	//Load the Glade file and instiate its widgets:
	try
	{
		builder = Gtk::Builder::create_from_file(UI_FILE);
		main_win = (Gtk::ApplicationWindow*) builder->get_widget<Gtk::ApplicationWindow>("main_window");
	}
	catch (const Glib::FileError & ex)
	{
		std::cerr << ex.what() << std::endl;
	}
	main_win->show();
	//return app->run(helloworld, argc, argv);
	//main_win->show_all_children ();
	//main_win->show_all_children ()
}

int
main (int argc, char *argv[])
{
	
	auto app = Gtk::Application::create("org.gtkmm.time_keeper");
	return app->make_window_and_run<Gtk::ApplicationWindow>(argc, argv);
	
	if (main_win)
	{
		app->signal_startup().connect(sigc::bind<0/*,Gtk::Application**/>(sigc::ptr_fun(&activate_app), app.get() ) );
		UI_Controller * controller = new UI_Controller(builder.get(),app.get());
	}
}

