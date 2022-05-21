/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * time-keeper.h
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

#ifndef _TIME_KEEPER_H_
#define _TIME_KEEPER_H_

#include <glibmm/datetime.h>
#include <glibmm/timer.h>

class Time_Keeper
{
public:
	void start_timer(){ /*timer.start();*/ };
	void stop_timer(){ /*timer.stop();*/ };
	void reset_timer(){ /*timer.reset();*/ };
	std::string display_timer() { return "" /*( std::to_string(((int) (timer.elapsed()/3600)))+\
	                                                         ":"+std::to_string(((int) (timer.elapsed()/60)))+\
	                                                         ":"+std::to_string(timer.elapsed() ))*/; };
	//const std::string hasheable (){ return (( const std::string) display_timer()); };
	//Time_Keeper(){ start_time = Glib::DateTime::create_now_local(); };
	//bool operator == ( Time_Keeper *t) { return t == this; };

protected:

private:
	Glib::DateTime start_time;
	Glib::TimeSpan elapsed_time; 
	//Glib::Timer timer;
};

#endif // _TIME_KEEPER_H_

