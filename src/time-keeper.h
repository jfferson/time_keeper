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
 
 #define STATS_STORE PACKAGE_DATA_DIR"/time_keeper.dat"

#include <glibmm/datetime.h>
#include <iostream>
#include<memory>
#include <glibmm/timer.h>
#include <map>
#include "zpp/zpp_bits.h"
#include <fstream>
#include <list>
#include<vector>
#include <sys/stat.h>

class Time_Keeper
{
public:
	enum event_type {cevent=0, fevent=1};

	typedef struct
	{
		event_type event;
		std::map<gint64, gint64> time_intervals;
		unsigned long int duration =0;
		//Glib::ustring event_name; // serializer issue
	} time_data;
	
	Time_Keeper() { 
		if (!started){
			record_data = new time_data(); 
			started = true;
		}
	}
	void start_timer();
	void set_duration(){ record_data->duration = ((int)timer.get()->elapsed()); };
	void stop_timer();
	void reset_timer();
	//void set_name(Glib::ustring name) { record_data->event_name = name; } //serializer issue
	//Glib::ustring get_name() { return record_data->event_name; } // serializer issue
	Glib::ustring display_timer();
	Glib::ustring display_counter(Glib::DateTime when);
	void stop_counter() { counter_active = false;};
	void restart_counter () { return ;};
	Glib::DateTime hasheable() { return Glib::DateTime::create_now_local(); }
	bool get_timer_active() { return timer_active; };
	bool get_counter_active() { return counter_active; };
	void set_dates_interval(Glib::DateTime when);
	void save(int position);
	
protected:

private:

	Glib::DateTime start_time;
	Glib::TimeSpan elapsed_time;
	std::shared_ptr<Glib::Timer> timer;
	bool timer_active = false; 
	bool timer_initiated = false;
	bool counter_active = false;
	time_data* record_data;
	gint64 start_time_key;
	gint64 const get_instant() { return (gint64 const) ((Glib::DateTime::create_now_local()).to_unix()); };
	void set_start(){ start_time_key = get_instant(); };
	void set_end(){ (record_data->time_intervals).insert({ ((gint64) start_time_key), ((gint64) get_instant()) }); };
	bool started = false;
};

// save and load cycle happens at very distinct moments, namely right on initialization and during execution of the program, \
though cursors used for alteration would each need their own instances for ensuring write at the correct locations with well defined behaviour. \
among the advantages also is the ensurance of a single view of the buffer, as all comes down to the Time_Serializer

static std::vector<Time_Keeper::time_data> save_data;

class Time_Serializer
{

public:
	static Time_Serializer& get_record(){
		static Time_Serializer ts;
		return ts;
    }
    void save(Time_Keeper::time_data data, int save_pos);
    static std::vector<Time_Keeper::time_data> load(){
		auto [data_ser, in] = zpp::bits::data_in();
		
		std::fstream save_file;
		save_file.open(STATS_STORE, std::ios::in | std::ios::binary);
		
		struct stat info_file;
		stat(STATS_STORE, &info_file);
		
		if (save_file){
			//data_ser.resize((info_file.st_size));
			data_ser.resize(50);
			save_file.read(reinterpret_cast<char*>(&data_ser),(info_file.st_size));
			save_file.close();
			in(save_data).or_throw();
		} else {
			std::cout << "could not load informations" << std::endl;
		}
		return save_data;
	}

private:
    Time_Serializer() = default;
};

 #endif // _TIME_KEEPER_H_
