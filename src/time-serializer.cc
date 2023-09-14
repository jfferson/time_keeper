/* * -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * time-serializer.cc
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

void Time_Serializer::save(Time_Keeper::time_data data, int save_pos){
    std::cout << "saved position: " << save_pos << std::endl;
    if (save_pos >= save_data.size() ) save_data.resize( save_pos + 1);
    save_data.at(save_pos) = data;
    
    std::cout << "the saving vector contains " << save_data.size() << "elements" << std::endl;
    
    auto [data_ser, out] = zpp::bits::data_out();
    out(save_data).or_throw();
    
    //for ( auto data_it = (data.time_intervals).begin(); data_it!= (data.time_intervals).end(); it++) // working as intended
	//std::cout << "relevant interval: " << data_it->first << ", " << data_it->second << std::endl << std::endl;
    
    std::fstream save_file;
    save_file.open(STATS_STORE, std::ios::out | std::ios::binary);
    if (save_file){
        save_file.write(reinterpret_cast<char*>(&data_ser),data_ser.size());
        save_file.close();
    } else {
        std::cout << "could not save informations" << std::endl;
    }
}
