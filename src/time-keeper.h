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
#include <iostream>
#include<memory>
#include <glibmm/timer.h>

class Time_Keeper
{
public:
	void start_timer();
	void stop_timer();
	void reset_timer();
	Glib::ustring display_timer();
	Glib::ustring display_counter(Glib::DateTime when);
	void stop_counter() { counter_active = false;};
	void restart_counter () { return ;};
	// I would rather use the constructor, but I need a default one in order \
	to use the unordered list
	void set_start(){ start_time = Glib::DateTime::create_now_local(); };
	Glib::DateTime hasheable() { return Glib::DateTime::create_now_local(); }
	bool get_timer_active() { return timer_active; };
	bool get_counter_active() { return counter_active; };
protected:

private:
	Glib::DateTime start_time;
	Glib::TimeSpan elapsed_time;
	std::shared_ptr<Glib::Timer> timer;
	bool timer_active = false;
	bool timer_initiated = false;
	bool counter_active = false;
};

#endif // _TIME_KEEPER_H_

