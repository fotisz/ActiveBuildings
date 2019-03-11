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

#ifndef CONTAINER_H
#define CONTAINER_H

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
#include <FL/Fl_Sys_Menu_Bar.H>

#include <FL/Fl_Native_File_Chooser.H>
#include <vector>
#include <FL/Fl_Chart.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Progress.H>
#include <FL/Fl_Tile.H>

#include <FL/Fl_Double_Window.H>
#include <ctime>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Shared_Image.H>



#include <math.h>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <iomanip>

#include <stdlib.h>

#include "building.h"

#undef min
#undef max

enum THREAD_STATE{
    
    FREE = 0,
    SAVING = 1,
    LOADING = 2,
    LOADING_POINTS = 3,
    LOADING_MATRIX = 4,
    LOADING_DESKS = 5,
    LOADING_DXF = 6,
    MAP_CHECK = 7,
    CALC_ROUTE = 8,
    CALC_METRIC = 9,
    CALC_STRUCTURES = 10,
    CALC_VIS = 11,
    CALC_LINES = 12,
    INITIALISING = 13,
    LOADING_ADJ = 14,
    CHECKING_MAP = 15
    
    
};


struct widgets{
    
    
    text_output* text;
    text_output* data_text;
    Fl_Choice* choice;
    
    Fl_Choice* show_visibility;
    
    Fl_Box* warning_visibility;
    Fl_Box* warning_map;
    
    Fl_Check_Button* show_points;
    Fl_Check_Button* show_scale;
    Fl_Check_Button* show_color_scale;
    Fl_Check_Button* show_segments;
    Fl_Choice* md_draw_choice;
    Fl_Choice* floor_choice;
    Fl_Choice* three_d_choice;
    Fl_Choice* show_desks_choice;
    Fl_Choice* mark_locations_choice;
    
    Fl_Choice* desk_metric_floor_choice;
    Fl_Choice* desk_metric_visibility_choice;
    Fl_Choice* desk_metric_stat_choice;
    Fl_Choice* desk_metric_type_choice;
    Fl_Choice* desk_metric_basis_choice;
    Fl_Choice* desk_metric_destination_choice;
    
    Fl_Input* overwrite_location_point;
    Fl_Input* location_label;
    
    Fl_Choice* plot_choice;
    
    Fl_Check_Button *show_desks,*show_kitchens,*show_maleWC,*show_femaleWC,*show_printers,*show_meeting_rooms,*show_exits,*show_stairs,*show_lifts,*show_walls_sitting,*show_walls_standing,*show_floorplan,*show_grid;
    Fl_Check_Button *snap_to_point, *transparent_walls;
    
    Fl_Counter*  point_size;
    Fl_Counter*  line_thickness;
    Fl_Counter* turn_ang;
    Fl_Progress* progress;
    Fl_Progress* progress2;
    Fl_Progress* progress_vis;
    Fl_Double_Window* win;
    Fl_Window* win2;
    
    
};

//struct map_structures;

class MyWindow;
class MyPlot;

struct container{
    int w,h;
    Fl_Double_Window* win;
    MyWindow* draw;
    MyPlot* graph;
    building* current;
    building* current_data;
    building old_building;
    widgets buttons;

    THREAD_STATE calculating;
    int undo_test;
    int draw_floorplan,draw_grid,draw_points_label,draw_segments_label,snap_to_point,transparent_walls,color_scale,show_scale,draw_route,draw_metric,draw_map;
    std::vector<building*> comp_buildings;
    Fl_Menu_Item* items;
    Fl_Sys_Menu_Bar* menu;

    int bow;
    int killthread;
    
    container(){
        
        killthread=0;
        bow=1;
        
        for (int i=0;i<4;i++){
            building* temp;
            comp_buildings.push_back(temp);
            comp_buildings[i] = new building;
        }
        
        current =comp_buildings[0];
        current_data =comp_buildings[0];
        
    }
    ~container(){
        
        for (std::size_t i=0;i<comp_buildings.size();i++){
            delete comp_buildings[i];
        }
        
        
        
    }
};

struct container_pass{
    container* content;
    
    std::string str;
};

struct building_pass{

    building* content;
    std::string str;
};

#endif //CONTAINER_H