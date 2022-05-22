/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * time-keeper.cc
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

#include "time-keeper.h"

Glib::ustring Time_Keeper::display_timer(){ 
	/*tain_stopwatch_read (time_interval, timer, (const tain_t*) CLOCK_MONOTONIC);
	unsigned int as_seconds;
	tain_uint (time_interval, as_seconds);
	unsigned int hours = as_seconds/3600;
	as_seconds%=3600;
	unsigned int minutes = as_seconds/60;
	as_seconds%=60;
	return ((Glib::ustring) ( std::to_string(hours)+":" \
		+std::to_string(minutes)+":" \
		+std::to_string(as_seconds) ));*/
}