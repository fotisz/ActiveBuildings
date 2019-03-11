/**********************************************************************/
/***************** Copyright (C) 2014 Richard Spinney *****************/
/**********************************************************************/

/*
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 */

#ifndef DESK_METRICS_H
#define DESK_METRICS_H


#include "config.h"
#if defined(WIN32)
#include "stdafx.h"
#endif

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Box.H>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <FL/glut.H>
#include <FL/Fl_Menu_Bar.H>

#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_File_Chooser.H>
#include <vector>
#include <FL/Fl_Chart.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Progress.H>
#include <FL/Fl_Tile.H>
#include "threads.h"
#include <FL/Fl_Double_Window.H>
#include <ctime>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_Table_Row.H>
#include <FL/Fl_Check_Button.H>


#include <math.h>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <cctype>
#include <algorithm>

#include <stdlib.h>

#include "graph_new.h"
#include "draw_window.h"
#include "locations.h"

#undef min
#undef max

void power_iteration (std::vector<double>&,graph&,container*,double,std::vector<double>&,double);

std::vector<std::vector<route_weight_number> >* desk_choice_new(std::vector<std::vector<route_weight_number> >* choice,map_structures* map, int route_choice_metric, int desk_or_point, int iterations,container* root,int& error,int do_cache);


std::vector<std::vector<int> > calculate_route(container* root,map_structures* map,int choice,int start, int end,int& dual_marker,int iterations, double& value);

double desk_custom(std::vector<vec>& points,std::vector<location>& desks, map_structures* map, int desk, int dest_choice_metric,int route_choice_metric,int metric, int same_floor, int visible,int stat,int range_option,std::vector<location>* destination, int &error,int is_desk,double range,int desk_or_point, int self_count_aware,int dest_is_points,int dest_is_lines,int iterations, std::vector<std::vector<std::vector<double> > >* choice,graph_routes&,graph_routes&,std::vector<double>&,std::vector<int>&,int,std::vector<std::vector<route_weight_number> >&,int route_stat, int do_cache);



double dist_2d(int& ,int& ,std::vector<vec>& );

double dist_3d(int& ,int& ,std::vector<vec>& );

double seg_dist_2d(int& ,int& ,map_structures* ,std::vector<vec>& );
double seg_dist_3d(int& ,int& ,map_structures*,std::vector<vec>& );


#endif /*DESK_METRICS_H*/














