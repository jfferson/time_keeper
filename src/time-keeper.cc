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

void Time_Keeper::save(int position){
	Time_Serializer events_record = Time_Serializer::get_record();
	events_record.save(*record_data, position);
	//std::cout << "time interval at save point: " << record_data->time_intervals[start_time_key] << std::endl;
}

void Time_Keeper::start_timer(){
	set_start();
	if (!timer_initiated){
		timer = std::shared_ptr<Glib::Timer>(new Glib::Timer);
		timer_active = true;
		record_data->event = cevent;
	}
	if (timer_active) {
		timer.get()->start();
	} else {
		timer.get()->resume();
	}
	timer_initiated = true;
	timer_active = true;
};

void Time_Keeper::stop_timer(int save_pos){
	timer_active = false;
	timer.get()->stop();
	set_end();
	// for debugging time intervals (unix time) on the record data
	set_duration();
	std::cout << "event: " << record_data->event << std::endl;
	std::cout << "duration: " << record_data->duration << std::endl;
	for ( auto it = (record_data->time_intervals).begin(); it!= (record_data->time_intervals).end(); it++)
	std::cout << "intervals: " << it->first << ", " << it->second << std::endl << std::endl;
	Time_Serializer events_record = Time_Serializer::get_record();
	events_record.save(*record_data, save_pos);
}

Glib::ustring Time_Keeper::display_timer(){
	gint64 seconds;
	if (timer_initiated ) seconds = ( timer.get()->elapsed() + loaded_duration);
	else seconds = ( record_data->duration );
	gint64 hours = (seconds/3600);
	seconds%=3600;
	gint64 minutes=seconds/60;
	seconds%=60;
	return ((Glib::ustring) ( std::to_string(hours)+":" \
		+std::to_string(minutes)+":" \
		+std::to_string(seconds) ));
}

void Time_Keeper::reset_timer(){
	timer_active = false;
	timer.get()->reset();
}

void Time_Keeper::set_dates_interval(Glib::DateTime when){
	record_data->event = fevent;
	record_data->time_intervals.insert({ (Glib::DateTime::create_now_local()).to_unix(), when.to_unix() });
}

Glib::ustring Time_Keeper::display_counter(Glib::DateTime when){
	counter_active = true;
	Glib::DateTime now = Glib::DateTime::create_now_local();
	size_t seconds = ( ((size_t)(when.difference(now))) /1000000);

	size_t hours = (seconds/3600);
	seconds%=3600;
	size_t minutes=seconds/60;
	seconds%=60;
	return ((Glib::ustring) ( std::to_string(hours)+":" \
		+std::to_string(minutes)+":" \
		+std::to_string(seconds) ));
}
