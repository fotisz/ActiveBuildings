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

#ifndef GLOBALS_H
#define GLOBALS_H

#include "config.h"
#if defined(WIN32)
#include "stdafx.h"
#endif
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Value_Input.H>
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
#include <FL/Fl_Check_Button.H>


#include <math.h>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <iomanip>



#include <stdlib.h>



#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>


#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <boost/serialization/split_member.hpp>





struct text_output;
class button_ex;

extern Fl_Choice* yaxis_choice;
extern Fl_Group* hist_group;

extern std::vector<Fl_Choice*> hist_building;
extern std::vector<Fl_Choice*> hist_metric;
extern std::vector<Fl_Counter*> bin_count;

extern Fl_Window* license_win;
extern Fl_Window* route_display;
extern Fl_Choice* route_type_c;
extern Fl_Int_Input *start_route_input,*end_route_input;

extern Fl_Window* add_link_win;
extern Fl_Int_Input *start_add_link,*end_add_link;

extern text_output* text;
extern text_output* data_text;
extern Fl_Choice* choice;

extern Fl_Choice* desk_option;

extern Fl_Choice* building_choice;
extern Fl_Choice* building_data_choice;

extern Fl_Choice* show_visibility;

extern Fl_Box* warning_visibility;
extern Fl_Box* warning_map;


extern Fl_Choice* md_draw_choice;
extern Fl_Choice* floor_choice;
extern Fl_Button* add_floor;
extern Fl_Choice* three_d_choice;
extern Fl_Choice* data_choice;
extern Fl_Choice* show_desks_choice;
extern Fl_Choice* mark_locations_choice;



extern Fl_Choice* desk_metric_floor_choice;
extern Fl_Choice* desk_metric_visibility_choice;
extern Fl_Choice* desk_metric_stat_choice;
extern Fl_Choice* desk_metric_type_choice;
extern Fl_Choice* desk_metric_basis_choice;
extern Fl_Choice* desk_metric_destination_choice;


extern Fl_Choice* metric_slot_choice;

extern Fl_Choice *custom_metric_choice,*route_type_choice,*custom_metric_floor_choice,*custom_metric_visibility_choice,*custom_metric_stat_choice,*custom_metric_destination_choice,*destination_criteria_choice,*custom_metric_basis,*custom_metric_self_count,*route_stat_choice;

extern Fl_Check_Button* inverse_option;
extern Fl_Check_Button* cache_metric_data;

extern Fl_Float_Input* eig_tol;

extern Fl_Choice* custom_metric_range_choice;

extern Fl_Value_Input* custom_metric_range;
extern Fl_Int_Input* max_iterations;
extern Fl_Button* ask2,*ask3;

extern Fl_Choice* plot_choice;

extern Fl_Choice* hist_interp_choice;
extern Fl_Choice* show_regression;


extern std::vector<Fl_Check_Button*> hist_p;

extern Fl_Check_Button *show_desks,*show_kitchens,*show_maleWC,*show_femaleWC,*show_printers,*show_meeting_rooms,*show_exits,*show_stairs,*show_lifts,*show_walls_sitting,*show_walls_standing,*show_floor_areas,*show_map;


extern button_ex* point_in;
extern button_ex* location_label_in;

extern Fl_Button* clear_selected_location;
extern Fl_Button* clear_last_location;

extern Fl_Button* clear_locations;


extern Fl_Int_Input* overwrite_location_point;
extern Fl_Input* location_label;


extern Fl_Counter* zfloor;
extern Fl_Box* zfloor_label;

extern Fl_Counter*  point_size;
extern Fl_Counter*  line_thickness;
extern Fl_Progress* progress;
extern Fl_Progress* progress2;
extern Fl_Progress* progress_vis;
extern Fl_Progress* progress_lines;
extern Fl_Double_Window* win;
extern Fl_Window* win2;
extern Fl_Window* about_win;

extern Fl_Window* quit_win;



extern Fl_Counter* xshift;
extern Fl_Counter* yshift;

extern Fl_Counter* turn_ang;

extern Fl_Value_Input* scale_floorplan;
extern Fl_Button* scale_floorplan_ok;

extern Fl_Window *guide,*destination_guide,*route_guide;
extern Fl_Window* custom_metric_calc;



extern Fl_Thread thread1,thread2,thread3,thread_redraw,main_tab_thread;




extern clock_t start_time;


extern std::string desks_file;
extern std::string points_file;
extern std::string matrix_file;


#endif /*GLOBALS_H*/