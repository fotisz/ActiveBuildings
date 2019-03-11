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


#include "config.h"

#if defined(WIN32)
    #include "stdafx.h"
#endif

#include <FL/Fl_Tooltip.H>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Float_Input.H>
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
#include <FL/fl_ask.H>
#include <FL/Fl_Value_Input.H>


#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_File_Chooser.H>
#include <vector>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Simple_Counter.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Progress.H>
#include <FL/Fl_Tile.H>

#include <FL/Fl_Double_Window.H>
#include <ctime>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_Pixmap.H>
#include <FL/Fl_Table_Row.H>
#include <FL/Fl_Check_Button.H>

#include <FL/Fl_Wizard.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Tabs.H>

#include <math.h>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <cctype>
#include <algorithm>



#include <stdlib.h>

#include "globals.h"
#include "graph_new.h"
#include "building.h"
#include "draw_window.h"
#include "locations.h"

#include "input_output.h"
#include "plot.h"

#include "dxf_recursive_model.h"
#include "container.h"
#include "button_ex.h"
#include "callbacks.h"


#undef min
#undef max


#include "splash.h"
#include "destination_guide.h"
#include "route_guide.h"



std::string desks_file;
std::string points_file;
std::string matrix_file;


Fl_Window* route_display;
Fl_Choice* route_type_c;
Fl_Int_Input *start_route_input,*end_route_input;

Fl_Window* add_link_win;
Fl_Int_Input *start_add_link,*end_add_link;


text_output* text;
text_output* data_text;
Fl_Choice* choice;

Fl_Choice* desk_option;

Fl_Choice* building_choice;
Fl_Choice* building_data_choice;

Fl_Choice* show_visibility;

Fl_Box* warning_visibility;
Fl_Box* warning_map;


Fl_Choice* floor_choice;
Fl_Button* add_floor;
Fl_Choice* three_d_choice;
Fl_Choice* data_choice;
Fl_Choice* show_desks_choice;
Fl_Choice* mark_locations_choice;


Fl_Choice* desk_metric_floor_choice;
Fl_Choice* desk_metric_visibility_choice;
Fl_Choice* desk_metric_stat_choice;
Fl_Choice* desk_metric_type_choice;
Fl_Choice* desk_metric_basis_choice;
Fl_Choice* desk_metric_destination_choice;


Fl_Choice* metric_slot_choice;

Fl_Choice *custom_metric_choice,*route_type_choice,*custom_metric_floor_choice,*custom_metric_visibility_choice,*custom_metric_stat_choice,*custom_metric_destination_choice,*destination_criteria_choice,*custom_metric_basis,*custom_metric_self_count,*route_stat_choice;

Fl_Check_Button* inverse_option;
Fl_Check_Button* cache_metric_data;

Fl_Float_Input*  eig_tol;

Fl_Choice* custom_metric_range_choice;

Fl_Value_Input* custom_metric_range;
Fl_Int_Input* max_iterations;

Fl_Button* ask2,*ask3;

Fl_Group* hist_group;

Fl_Choice* plot_choice;
Fl_Choice* hist_interp_choice;
Fl_Choice* show_regression;
Fl_Choice* yaxis_choice;
Fl_Counter* num_bin_count;

std::vector<Fl_Check_Button*> hist_p;

std::vector<Fl_Box*> hist_box;
std::vector<Fl_Choice*> hist_building;
std::vector<Fl_Choice*> hist_metric;
std::vector<Fl_Counter*> bin_count;


Fl_Check_Button *show_desks,*show_kitchens,*show_maleWC,*show_femaleWC,*show_printers,*show_meeting_rooms,*show_exits,*show_stairs,*show_lifts,*show_walls_sitting,*show_walls_standing,*show_floor_areas,*show_map;
Fl_Check_Button *snap_to_point, *transparent_walls;


button_ex* point_in;
button_ex* location_label_in;

Fl_Button* clear_selected_location;
Fl_Button* clear_last_location;

Fl_Button* clear_locations;


Fl_Int_Input* overwrite_location_point;
Fl_Input* location_label;


Fl_Counter* zfloor;
Fl_Box* zfloor_label;

Fl_Counter*  point_size;
Fl_Counter*  line_thickness;
Fl_Progress* progress;
Fl_Progress* progress2;
Fl_Progress* progress_vis;
Fl_Progress* progress_lines;
Fl_Double_Window* win;
Fl_Window* win2;
Fl_Window* about_win;
Fl_Window* quit_win;




Fl_Counter* xshift;
Fl_Counter* yshift;

Fl_Counter* turn_ang;

Fl_Value_Input* scale_floorplan;
Fl_Button* scale_floorplan_ok;

Fl_Window *guide,*destination_guide,*route_guide,*license_win;
Fl_Window* custom_metric_calc;



Fl_Thread thread1,thread2,thread3,thread_redraw,main_tab_thread;



clock_t start_time;   //non-boost





//===================================================== main


int main() {


    Fl::scheme("plastic");
    
    Fl::visual(FL_DOUBLE|FL_INDEX);
    

    Fl_Text_Buffer* buff = new Fl_Text_Buffer();
    Fl_Text_Buffer* data_buff = new Fl_Text_Buffer();
    
    text = new text_output;
    data_text = new text_output;
    
    Fl_Text_Display* out_read;
    Fl_Text_Display* out_data;
    
    
    
    container root; // main access structure
    root.undo_test=0;
    

    Fl_Button* create_map;
    Fl_Button* mean_depth;
    
    Fl_Button* calc_visibility;
    
    Fl_Button* desk_metric_button;
    
    Fl_Button* save;
    Fl_Button* refresh_data;
    
 
    
    Fl_Box* splash;
    Fl_Group* ooo;
    
    MyWindow* draw;
    MyPlot* graph;
    
    const char *pathPtr;
    std::string bundle_address;
    
    
    Fl_Menu_Item items[] = {
        {"File",0,0,0,FL_SUBMENU,0,0,FL_NORMAL_SIZE,FL_BLACK},
        {"&Open Project",FL_COMMAND+'o', read_binary_cb,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},
        {"&Save Project",FL_COMMAND|'s', save_binary_cb,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},
        {"Read pre-existing structures",0,0,0,FL_SUBMENU|FL_MENU_DIVIDER,0,0,FL_NORMAL_SIZE,FL_BLACK},
        {"Open &points", FL_ALT|'p', open_points_cb,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},
        {"Open adjacency &matrix", FL_ALT|'m', open_matrix_cb,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},
        {"Open adjacency &list", FL_ALT|'l', open_adjlist_cb,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},
        {"Open d&xf floorplan",FL_COMMAND|'x', open_dxf_cb,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},
        
        {"Open location marker files",0, 0,0,FL_SUBMENU,0,0,FL_NORMAL_SIZE,FL_BLACK},
        {"Open desks",0, open_desks_cb,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},
                {"Open kitchens",0, open_kitchens_cb,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},
                {"Open printers",0, open_printers_cb,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},
                {"Open male toilets",0, open_maleWC_cb,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},
                {"Open female toilets",0, open_femaleWC_cb,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},
                {"Open meeting rooms",0, open_meeting_cb,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},
                {"Open exits",0, open_exit_cb,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},
                {"Open stairs",0, open_stair_cb,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},
                {"Open lifts",0, open_lift_cb,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},
                {0,0,0,0,0,0,0,0,0},
        
        {"Floorplans to walls",0, 0,0,FL_SUBMENU,0,0,FL_NORMAL_SIZE,FL_BLACK},
                {"Standing opaque",0, lines_to_walls_standing,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},
                {"Standing transparent",0, lines_to_walls_standing_transparent,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},
                {"Sitting opaque",0, lines_to_walls_sitting,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},
                {"Sitting transparent",0, lines_to_walls_sitting_transparent,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},
                {0,0,0,0,0,0,0,0,0},
        {"Floorplan to spatial map",0, floorplan_to_map,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},

        {0,0,0,0,0,0,0,0,0},
        {"&Quit", FL_COMMAND|'q', main_callback,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},

        {0,0,0,0,0,0,0,0,0},
        {"Edit",0,0,0,FL_SUBMENU,0,0,FL_NORMAL_SIZE,FL_BLACK},
        {"Empty entire building slot",FL_COMMAND|'e',clear_building,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},
        {"Add or remove link (mouse free)",FL_ALT+'a',link_win_cb,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},
        {"Undo",FL_COMMAND|'z',undo,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},
        {"&Cancel metric calculation",FL_ALT+'c', kill_thread,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},
                {"Check map consistency",FL_COMMAND+'y',map_consistency,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},
        {0,0,0,0,0,0,0,0,0},
        {"View",0,0,0,FL_SUBMENU,0,0,FL_NORMAL_SIZE,FL_BLACK},
        
         {"Zoom in",FL_COMMAND|'[',MyWindow::mouseless_zoom_in_entry,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},
        {"Zoom out",FL_COMMAND|']',MyWindow::mouseless_zoom_out_entry,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},

        
        {"Inverted Colour",FL_COMMAND|'i',colour_cb,&root,FL_MENU_TOGGLE,0,0,FL_NORMAL_SIZE,FL_BLACK},
        
        {"Fullscreen",FL_COMMAND|'k',go_fullscreen,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},
        {"Undo Fullscreen",FL_COMMAND|'l',undo_fullscreen,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},
        {"Re-centre/reset view",FL_ALT+'r',reset_view,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},
        {"Scheme",0,0,0,FL_SUBMENU,0,0,FL_NORMAL_SIZE,FL_BLACK},
        {"Scheme 1",0,scheme0,0,FL_MENU_RADIO,0,0,FL_NORMAL_SIZE,FL_BLACK},
        {"Scheme 2",0,scheme1,0,FL_MENU_RADIO,0,0,FL_NORMAL_SIZE,FL_BLACK},
        {"Scheme 3",0,scheme2,0,FL_MENU_RADIO|FL_MENU_VALUE,0,0,FL_NORMAL_SIZE,FL_BLACK},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {"Options",0,0,0,FL_SUBMENU,0,0,FL_NORMAL_SIZE,FL_BLACK},

        
        
        {"Floorplan",FL_COMMAND+'f',show_floorplan_cb,&root,FL_MENU_TOGGLE|FL_MENU_VALUE,0,0,FL_NORMAL_SIZE,FL_BLACK},
        {"Grid",FL_COMMAND+'g',show_grid_cb,&root,FL_MENU_TOGGLE|FL_MENU_VALUE,0,0,FL_NORMAL_SIZE,FL_BLACK},
        {"Scale",FL_ALT+'s',show_scale_cb,&root,FL_MENU_TOGGLE|FL_MENU_VALUE,0,0,FL_NORMAL_SIZE,FL_BLACK},
        {"Route",FL_COMMAND+'r',show_route_cb,&root,FL_MENU_TOGGLE|FL_MENU_VALUE,0,0,FL_NORMAL_SIZE,FL_BLACK},//29
        {"Visualise metric",FL_COMMAND+'m',show_metric_cb,&root,FL_MENU_TOGGLE,0,0,FL_NORMAL_SIZE,FL_BLACK},//30
        {"Point labels",FL_COMMAND+'p',points_label_cb,&root,FL_MENU_TOGGLE,0,0,FL_NORMAL_SIZE,FL_BLACK},
        {"Segment labels",FL_ALT+'g',segments_label_cb,&root,FL_MENU_TOGGLE,0,0,FL_NORMAL_SIZE,FL_BLACK},
        {"Colour scale",FL_COMMAND+'u',color_scale_cb,&root,FL_MENU_TOGGLE,0,0,FL_NORMAL_SIZE,FL_BLACK},
        {"Transparent Walls",FL_COMMAND+'t',transparent_walls_cb,&root,FL_MENU_TOGGLE,0,0,FL_NORMAL_SIZE,FL_BLACK},
        {"Snap to point priority",0,snap_to_point_cb,&root,FL_MENU_TOGGLE|FL_MENU_VALUE,0,0,FL_NORMAL_SIZE,FL_BLACK},
        {0,0,0,0,0,0,0,0,0},

        { "Help", 0, 0, 0, FL_SUBMENU ,0,0,FL_NORMAL_SIZE,FL_BLACK},
        {"About",FL_COMMAND|'a',about_cb,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},
                {"License",0,open_license,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},

        {"Quick Guide",0,guide_cb,&root,0,0,0,FL_NORMAL_SIZE,FL_BLACK},
        {0,0,0,0,0,0,0,0,0},

        {0,0,0,0,0,0,0,0,0}

    };
    
    
    Fl::lock(); //multi threading
    
    
    
  
    
    
    
    int w=1200,h=855;
    
    root.w=w;
    root.h=h;
    
   
    
    
    //MAIN WINDOW
    
    win= new Fl_Double_Window(0.5*(Fl::w()-w),0.5*(Fl::h()-h),w,h, "Spatial Metric Calculator");
    win->begin();
    
    win->callback(main_callback,&root);
    
    win->size_range(w,h,5*w,3*h,1,1,1);
    
    
    
    

    Fl_Sys_Menu_Bar* m = new Fl_Sys_Menu_Bar(0, 0, win->w(),20);

            m->menu(items);
    


    win->add(m);

    
    
#if __APPLE__
#define sysoffset 15//15//15
#else
#define sysoffset 0
#endif
    
    ooo = new Fl_Group(10,20-sysoffset,win->w()-20,win->h()-140+sysoffset);
    
    Fl_Tabs* oo = new Fl_Tabs(10,20-sysoffset,win->w()-20,win->h()-140+sysoffset);

    oo->callback(main_tab_callback,&root);
    
   
    {Fl_Group* o = new Fl_Group(10, 40-sysoffset,  oo->w(),oo->h()-30, "Main");
        
        
        
        draw = new MyWindow(10,42-sysoffset,o->w()-250,oo->h()-32,0,&root);
        
        
        
        draw->redraw();
        
        Fl_Group* p = new Fl_Group(draw->x()+draw->w(),42-sysoffset,250,o->h()-2,0);
        
        Fl_Box* divider = new Fl_Box(p->x(),p->y(),5,p->h(),0);
        divider->box(FL_FLAT_BOX);
        
        p->add(divider);
        
        
        Fl_Group* content = new Fl_Group(p->x()+5,p->y(),p->w()-5,p->h(),0);
        content->box(FL_FLAT_BOX);
       
        
        
        building_choice = new Fl_Choice(content->x()+content->w()-120,content->y()+10,120,20,"Building: ");
        building_choice->add("1");
        building_choice->add("2");
        building_choice->add("3");
        building_choice->add("4");
        building_choice->value(0);
        building_choice->callback(building_choice_cb,&root);
        building_choice->tooltip("Building choice. Up to 4 buildings can be worked on/compared concurrently.");
        
        
        three_d_choice = new Fl_Choice (content->x()+content->w()-120,content->y()+40,120,20,"View: ");
        three_d_choice->add("2D");
        three_d_choice->add("3D Orthographic");
        three_d_choice->add("3D Perspective");
        three_d_choice->add("Free view");
        three_d_choice->value(0);
        three_d_choice->callback((Fl_Callback*) three_d_cb,&root);//was just redraw
        three_d_choice->tooltip("Choose between 2D, 3D and first person views. Editing in 2D only.");
        
        floor_choice = new Fl_Choice (content->x()+content->w()-120,content->y()+70,120,20,"Floor: ");
        floor_choice->add("All");
        floor_choice->add("0");
        floor_choice->value(0);
        floor_choice->callback((Fl_Callback*) floor_choice_cb,&root); //ws just redraw
        floor_choice->tooltip("Choose which floor (or all) to display/edit");
        
        add_floor = new Fl_Button (content->x()+content->w()-210,content->y()+70,40,20,"Add");
        add_floor->callback((Fl_Callback*) add_floor_cb,&root);
        add_floor->tooltip("Add a distinct floor level.");
        
        zfloor_label = new Fl_Box(content->x()+content->w()-210,content->y()+100,80,20,"Floor height");
        
        zfloor = new Fl_Counter(content->x()+content->w()-120,content->y()+100,120,20);
        zfloor->step(0.01,1);
        zfloor->value(0);
        zfloor->maximum(10000);
        zfloor->minimum(0);
        zfloor->callback((Fl_Callback*) zfloor_cb,&root);
        zfloor->tooltip("Adjust height of current floor level.");
        zfloor->deactivate();
        zfloor_label->deactivate();
        
        Fl_Box* location_box = new Fl_Box(content->x()+5,content->y()+145,content->w()-5,110,"Display");
        location_box->box(FL_GTK_DOWN_BOX);
        location_box->color(FL_DARK1);
        location_box->labelsize(12);
        location_box->align(Fl_Align(FL_ALIGN_TOP_LEFT));
        
        show_desks = new Fl_Check_Button (content->x()+10,content->y()+150,70,20,"Desks");
        show_kitchens = new Fl_Check_Button (content->x()+90,content->y()+150,80,20,"Kitchens");
        show_maleWC = new Fl_Check_Button (content->x()+170,content->y()+150,70,20,"M-WC");
        show_femaleWC = new Fl_Check_Button (content->x()+10,content->y()+170,70,20,"F-WC");
        show_printers = new Fl_Check_Button (content->x()+90,content->y()+170,70,20,"Printers");
        show_meeting_rooms = new Fl_Check_Button (content->x()+170,content->y()+170,70,20,"MR");
        show_exits = new Fl_Check_Button (content->x()+10,content->y()+190,70,20,"Exits");
        show_stairs = new Fl_Check_Button (content->x()+90,content->y()+190,70,20,"Stairs");
        show_lifts = new Fl_Check_Button (content->x()+170,content->y()+190,70,20,"Lifts");
        show_walls_sitting = new Fl_Check_Button (content->x()+10,content->y()+210,110,20,"Walls (sitting)");
        show_walls_standing = new Fl_Check_Button (content->x()+120,content->y()+210,120,20,"Walls (standing)");
        show_floor_areas = new Fl_Check_Button (content->x()+10,content->y()+230,90,20,"Floor area");
        show_map = new Fl_Check_Button (content->x()+120,content->y()+230,110,20,"Spatial map");
        
        show_map->value(1);
        
        show_desks->callback((Fl_Callback*) redraw_show,&root);
        show_kitchens->callback((Fl_Callback*) redraw_show,&root);
        show_maleWC->callback((Fl_Callback*) redraw_show,&root);
        show_femaleWC->callback((Fl_Callback*) redraw_show,&root);
        show_printers->callback((Fl_Callback*) redraw_show,&root);
        show_meeting_rooms->callback((Fl_Callback*) redraw_show,&root);
        show_exits->callback((Fl_Callback*) redraw_show,&root);
        show_stairs->callback((Fl_Callback*) redraw_show,&root);
        show_lifts->callback((Fl_Callback*) redraw_show,&root);
        show_walls_sitting->callback((Fl_Callback*) redraw_show,&root);
        show_walls_standing->callback((Fl_Callback*) redraw_show,&root);
        show_floor_areas->callback((Fl_Callback*) redraw_show,&root);
        show_map->callback((Fl_Callback*) redraw_show,&root);
        
        
        Fl_Box* line_point_box = new Fl_Box(content->x()+5,content->y()+275,content->w()-5,50,"Spatial map display options");
        line_point_box->box(FL_GTK_DOWN_BOX);
        line_point_box->color(FL_DARK1);
        line_point_box->labelsize(12);
        line_point_box->align(Fl_Align(FL_ALIGN_TOP_LEFT));
        
        line_thickness = new Fl_Simple_Counter(content->x()+15,content->y()+285,0.5*(content->w()-30),20,"Line thickness");
        line_thickness->step(1.0);
        line_thickness->value(1.0);
        line_thickness->maximum(10);
        line_thickness->minimum(1);
        line_thickness->callback((Fl_Callback*) redraw,&root);
        line_thickness->tooltip("Thickness in pixels");
        
        point_size = new Fl_Counter(content->x()+0.5*(content->w()-30)+22,content->y()+285,0.5*(content->w()-30),20,"Point size");
        point_size->step(0.05,0.2);
        point_size->value(0.2);
        point_size->maximum(10.0);
        point_size->minimum(0.05);
        point_size->callback((Fl_Callback*) redraw,&root);
        point_size->tooltip("Size in metres");
        
        desk_option = new Fl_Choice(content->x()+content->w()-100,content->y()+330,100,20,"Desk representation");
        desk_option->add("Location");
        //desk_option->add("Metric");
        desk_option->add("Model");
        desk_option->value(0);
        desk_option->callback((Fl_Callback*)redraw,&root);
        desk_option->tooltip("Display option for desks.");
        
        
        
        show_visibility = new Fl_Choice (content->x()+content->w()-120,content->y()+360,120,20,"Show visibility:");
     
        show_visibility->add("None");
        show_visibility->add("Sitting (all barriers)");
        show_visibility->add("Sitting (opaque barriers)");
        show_visibility->add("Standing (all barriers)");
        show_visibility->add("Standing (opaque barriers)");
        show_visibility->add("Selected: sitting (all barriers)");
        show_visibility->add("Selected: sitting (opaque barriers)");
        show_visibility->add("Selected: standing (all barriers)");
        show_visibility->add("Selected: standing (opaque barriers)");
        show_visibility->value(0);
        show_visibility->callback((Fl_Callback*)redraw,&root);
        show_visibility->tooltip("Visibility illustration options.");
        
        
        Fl_Tabs* subtab = new Fl_Tabs(content->x(),content->y()+390,content->w(),210,"");
        
        {Fl_Group* subtab_a = new Fl_Group(subtab->x()+20, subtab->y()+20, subtab->w(),subtab->h(), "Main Edit");
        
        
        mark_locations_choice = new Fl_Choice (content->x()+content->w()-150,content->y()+420,140,20,"Edit:");
        mark_locations_choice->add("-----");
        mark_locations_choice->add("Desk markers");
        mark_locations_choice->add("Kitchen markers");
        mark_locations_choice->add("Male WC markers");
        mark_locations_choice->add("Female WC markers");
        mark_locations_choice->add("Printer markers");
        mark_locations_choice->add("Meeting Room markers");
        mark_locations_choice->add("Entrance and Exit markers");
        mark_locations_choice->add("Stair markers");
        mark_locations_choice->add("Lift markers");
        mark_locations_choice->add("Visbility barriers: sitting");
        mark_locations_choice->add("Visbility barriers: standing");
        mark_locations_choice->add("Spatial graph: points");
        mark_locations_choice->add("Spatial graph: links");
        mark_locations_choice->add("Floor area");
        mark_locations_choice->value(0);
        mark_locations_choice->callback((Fl_Callback*) redraw_mark,&root);
        mark_locations_choice->tooltip("Main edit option. Constructs can only be added/edited/removed when appropriately selected here.");
            
        
        location_label = new Fl_Input (content->x()+content->w()-130,content->y()+450,85,20,"Selected Label:");
       
        location_label_in = new button_ex (content->x()+content->w()-35,content->y()+450,25,20,"OK",&root);
        location_label_in->callback((Fl_Callback*) location_label_cb,&root);
        location_label->deactivate();
        location_label_in->deactivate();
        location_label->tooltip("Add label to construct (if relevant).");
        location_label_in->tooltip("Add label to construct (if relevant).");
            
        overwrite_location_point = new Fl_Int_Input  (content->x()+content->w()-130,content->y()+480,85,20,"Selected Point:");
        point_in = new button_ex (content->x()+content->w()-35,content->y()+480,25,20,"OK",&root);
        point_in->callback((Fl_Callback*) point_in_cb,&root);
        overwrite_location_point->deactivate();
        point_in->deactivate();
        overwrite_location_point->tooltip("Override default associated point (if relevant).");
        point_in->tooltip("Override default associated point (if relevant).");
            
        clear_selected_location = new Fl_Button(content->x()+content->w()-150,content->y()+510,140,20,"Clear selected object");
        clear_selected_location->callback((Fl_Callback*) clear_selected_location_cb,&root);
        clear_selected_location->deactivate();
                        clear_selected_location->tooltip("Clear selected construct.");
        
        clear_last_location = new Fl_Button (content->x()+content->w()-150,content->y()+540,140,20,"Clear last object");
        clear_last_location->callback((Fl_Callback*) clear_last_location_cb,&root);
        clear_last_location->deactivate();
            clear_last_location->tooltip("Clear most recently added construct.");
            
        clear_locations= new Fl_Button (content->x()+content->w()-150,content->y()+570,140,20,"Clear all");
        clear_locations->callback((Fl_Callback*) clear_locations_cb,&root);
        clear_locations->deactivate();
            clear_locations->tooltip("Clear all constructs on all floors.");
        
       
        
        
            subtab_a->end();
        }
        {Fl_Group* subtab_b = new Fl_Group(subtab->x()+20, subtab->y()+20, subtab->w(),subtab->h(), "Floorplan");
      
            xshift = new Fl_Counter(content->x()+content->w()-160,content->y()+420,150,20,"Shift x");
            xshift->step(0.01,2);
            xshift->value(0);
            xshift->maximum(1000000);
            xshift->minimum(-1000000);
            xshift->callback((Fl_Callback*) shiftx_cb,&root);
            xshift->tooltip("Shift current floor floorplan in x direction.");
            
            yshift = new Fl_Counter(content->x()+content->w()-160,content->y()+460,150,20,"Shift y");
            yshift->step(0.01,2);
            yshift->value(0);
            yshift->maximum(1000000);
            yshift->minimum(-1000000);
            yshift->callback((Fl_Callback*) shifty_cb,&root);
            yshift->tooltip("Shift current floor floorplan in y direction.");
            
            scale_floorplan = new Fl_Value_Input(content->x()+content->w()-140,content->y()+500,90,20,"Scale floorplan");
            scale_floorplan->value(1);
            scale_floorplan->tooltip("Resize floorplan by given factor.");
            scale_floorplan_ok = new Fl_Button(content->x()+content->w()-40,content->y()+500,30,20,"OK");
            scale_floorplan_ok->callback((Fl_Callback*) scale_floorplan_cb,&root);
            scale_floorplan_ok->tooltip("Resize floorplan by given factor.");
            
            Fl_Button* layers_button = new Fl_Button(content->x()+content->w()-90,content->y()+540,80,20,"Layers");
            layers_button->callback(LayerCB,&root);
            layers_button->tooltip("Open layer dialogue for current floorplan.");
            subtab_b->end();
        }
        {Fl_Group* subtab_c = new Fl_Group(subtab->x()+20, subtab->y()+20, subtab->w(),subtab->h(), "Metrics");
            
            turn_ang = new Fl_Counter(content->x()+content->w()-160,content->y()+420,150,20,"Turn definition (degrees)");
            turn_ang->minimum(0);
            turn_ang->maximum(180);
            turn_ang->step(0.1,1);
            turn_ang->value(45);
            turn_ang->callback((Fl_Callback*) warning_map_cb,&root);
            turn_ang->tooltip("Angular deviation above this value constitutes a turn.\nDetermines definition in all metrics/routes/lines until 'Create' is re-run.");
            
            create_map = new Fl_Button(content->x()+content->w()-235,content->y()+460,80,20,"Create");
            create_map->callback((Fl_Callback*) create_structures,&root);
            create_map->tooltip("(Re-)Calculate graph structures. Required after any change of spatial map or definition of turn.");
            progress = new Fl_Progress(content->x()+content->w()-145,content->y()+460,135,20);
            progress->minimum(0);                      // set progress range to be 0.0 ~ 1.0
            progress->maximum(1);
            progress->color(0x88888800);               // background color
            progress->selection_color(fl_rgb_color(255, 191, 8));
            progress->labelcolor(FL_WHITE);            // percent text color
            progress->tooltip("Current validity of graph structures.");

            
            calc_visibility=new Fl_Button(content->x()+content->w()-235,content->y()+485,80,20,"Visibility");
            calc_visibility->callback((Fl_Callback*) visibility_cb,&root);
            calc_visibility->tooltip("(Re-)Calculate visibility information. Required after any change to locations or visibility barriers.");
            progress_vis = new Fl_Progress(content->x()+content->w()-145,content->y()+485,135,20);
            progress_vis->minimum(0);                      // set progress range to be 0.0 ~ 1.0
            progress_vis->maximum(1);
            progress_vis->color(0x88888800);               // background color
            progress_vis->selection_color(fl_rgb_color(255, 191, 8));

            progress_vis->labelcolor(FL_WHITE);            // percent text color
            progress_vis->tooltip("Current validity of visibility information.");

            Fl_Button* calc_lines=new Fl_Button(content->x()+content->w()-235,content->y()+510,80,20,"Find lines");
            calc_lines->callback((Fl_Callback*) line_calc_cb,&root);
            calc_lines->tooltip("Generate list of segements that represent all distinct sequences of segments that can be traversed with 0 turns");
            progress_lines = new Fl_Progress(content->x()+content->w()-145,content->y()+510,135,20);
            progress_lines->minimum(0);                      // set progress range to be 0.0 ~ 1.0
            progress_lines->maximum(1);
            progress_lines->color(0x88888800);               // background color
            progress_lines->selection_color(fl_rgb_color(255, 191, 8));

            progress_lines->labelcolor(FL_WHITE);            // percent text color

            
            
            
            Fl_Button *clear_metrics = new Fl_Button(content->x()+content->w()-235,content->y()+535,105,20,"Clear");
            clear_metrics->callback((Fl_Callback*) clear_metrics_cb,&root);
            clear_metrics->tooltip("Clear graph structures including metrics. (Use to save space when saving project).");
            
            Fl_Button *show_route_button = new Fl_Button(content->x()+content->w()-115,content->y()+535,105,20,"Show path");
            show_route_button->callback((Fl_Callback*) show_route_menu_cb,&root);
            show_route_button->tooltip("Open route display dialogue.");
            
            
            desk_metric_button = new Fl_Button (content->x()+content->w()-235,content->y()+560,225,30,"Metric dialogue (calculate/select)");
            desk_metric_button->callback((Fl_Callback*) desk_metric_cb,&root);
            desk_metric_button->tooltip("Open metric display/calculation dialogue.");
            
            
            
            
            
            subtab_c->end();
        }
        subtab->end();
        subtab->callback(tab_callback,&root);
        
        warning_visibility = new Fl_Box(FL_ENGRAVED_FRAME,content->x(),content->y()+610,content->w(),30,"Warning: visibility out of date.");
        warning_visibility->hide();
        
        warning_map = new Fl_Box(FL_ENGRAVED_FRAME,content->x(),content->y()+650,content->w(),30,"Warning: spatial map out of date.");
        warning_map->hide();
        
       
        content->resizable(0);
       
        content->end();
        p->add(content);
        
        p->end();
       
       
        o->end();
        o->resizable(draw);
       
    }
    {Fl_Group* o = new Fl_Group(10, 40-sysoffset,  oo->w(),oo->h()-30, "Data");
       
        out_data = new Fl_Text_Display(10,42-sysoffset,o->w()-250,oo->h()-32,0);
        
        
        out_data->textfont(4);
        
        
        data_text->buff=data_buff;
        data_text->display=out_data;
        clear_output(data_text);
        push_output(data_text,"No data selected.\n");
        out_data->redraw();
        o->resizable(out_data);
        
        
        
        Fl_Group* p = new Fl_Group(out_data->x()+out_data->w(),42-sysoffset,250,o->h()-2+sysoffset,0);
        
        Fl_Box* divider = new Fl_Box(p->x(),p->y(),5,p->h(),0);
        divider->box(FL_FLAT_BOX);
        
        p->add(divider);
        
        
        Fl_Group* content = new Fl_Group(p->x()+5,p->y(),p->w()-5,p->h(),0);
        content->box(FL_FLAT_BOX);
        
        
        building_data_choice = new Fl_Choice (out_data->x()+out_data->w()+100,out_data->y()+20,120,20,"Building: ");
        building_data_choice->add("1");
        building_data_choice->add("2");
        building_data_choice->add("3");
        building_data_choice->add("4");
        building_data_choice->value(0);
        building_data_choice->callback(building_data_choice_cb,&root);
        building_data_choice->tooltip("Building data display choice");
        
        data_choice = new Fl_Choice (out_data->x()+out_data->w()+100,out_data->y()+60,120,20,"Display data: ");
        data_choice->add("------");
        data_choice->add("Points");
       
        data_choice->add("Segments (Adjacency list)");
        data_choice->add("Desk marker info");
        data_choice->add("Kitchen marker info");
        data_choice->add("Male WC marker info");
        data_choice->add("Female WC marker info");
        data_choice->add("Printer marker info");
        data_choice->add("Meeting Room marker info");
        data_choice->add("Entrance and Exit marker info");
        data_choice->add("Stair marker info");
        data_choice->add("Lift marker info");
        data_choice->add("Visibility marker info");
        data_choice->add("Metrics");
        data_choice->add("Floor and Global info");        
        data_choice->value(0);
        data_choice->callback((Fl_Callback*) redraw_data,&root);
        data_choice->tooltip("Data output choice for chosen building");
        
        
        
        save = new Fl_Button(out_data->x()+out_data->w()+100,out_data->y()+100,120,20,"Export");
        save->callback((Fl_Callback*) save_data,&root);
        save->tooltip("Save current data buffer to text file.\n Any file type (specify .csv .txt .dat etc. if desired)");
        
        
        content->end();
        content->resizable(0);
        p->end();
        
        o->end();
        o->resizable(out_data);
    }
    {Fl_Group* o = new Fl_Group(10, 40-sysoffset,  oo->w(),oo->h()-30, "Plot");
        
        graph = new MyPlot(10,42-sysoffset,o->w()-250,oo->h()-32,0,&root);
        graph->redraw();
        
        o->resizable(graph);
        Fl_Group* p = new Fl_Group(draw->x()+draw->w(),42-sysoffset,250,o->h()-2,0);
        
        Fl_Box* divider = new Fl_Box(p->x(),p->y(),5,p->h(),0);
        divider->box(FL_FLAT_BOX);
        
        p->add(divider);
        
        
        Fl_Group* content = new Fl_Group(p->x()+5,p->y(),p->w()-5,p->h(),0);
        content->box(FL_FLAT_BOX);
        
        

       
        plot_choice = new Fl_Choice (graph->x()+graph->w()+100,graph->y()+10,120,20,"Plot: ");
        plot_choice->add("Scatter Plot");
        plot_choice->add("Histogram");
        plot_choice->value(0);
        plot_choice->callback((Fl_Callback*) plot_choice_cb,&root);// redraw_and_hist);
        plot_choice->tooltip("Choose graph type");
        
        hist_group = new Fl_Group(content->x(),content->y()+40,content->w(),content->h()-50);
        
        
        hist_interp_choice = new Fl_Choice(graph->x()+graph->w()+100,graph->y()+40,120,20,"Interpolation");
        hist_interp_choice->add("Yes");
        hist_interp_choice->add("No");
        hist_interp_choice->add("Yes (only)");
        hist_interp_choice->value(0);
        hist_interp_choice->callback(plot_choice_cb,&root);
        hist_interp_choice->deactivate();
        hist_interp_choice->tooltip("Choose to display interpolation (histogram only)");
        
        yaxis_choice = new Fl_Choice(graph->x()+graph->w()+100,graph->y()+70,120,20,"Y-values");
        yaxis_choice->add("N");
        yaxis_choice->add("Probability density");
        yaxis_choice->value(0);
        yaxis_choice->callback(plot_choice_cb,&root);
        yaxis_choice->deactivate();
        yaxis_choice->tooltip("Choose units: number in bin or normalised probability density (histogram only)");
        
        show_regression = new Fl_Choice(graph->x()+graph->w()+100,graph->y()+100,120,20,"Regression");
        show_regression->add("No");
        show_regression->add("Yes");
        show_regression->value(0);
        show_regression->callback(plot_choice_cb,&root);
        show_regression->tooltip("Display simple linear regression fit (2 variables only)");
        
        for (unsigned int i=0;i<4;i++){
            Fl_Box* temp;
            hist_box.push_back(temp);
            switch (i){
                case 0:
                    hist_box[i] = new Fl_Box(graph->x()+graph->w()+10,graph->y()+(i*120)+140,o->w()-graph->w()-10,80,"Variable 1");
                    break;
                case 1:
                    hist_box[i] = new Fl_Box(graph->x()+graph->w()+10,graph->y()+(i*120)+140,o->w()-graph->w()-10,80,"Variable 2");
                    break;
                case 2:
                    hist_box[i] = new Fl_Box(graph->x()+graph->w()+10,graph->y()+(i*120)+140,o->w()-graph->w()-10,80,"Variable 3");
                    break;
                case 3:
                    hist_box[i] = new Fl_Box(graph->x()+graph->w()+10,graph->y()+(i*120)+140,o->w()-graph->w()-10,80,"Variable 4");
                    break;
            }
            
            hist_box[i]->box(FL_GTK_DOWN_BOX);
            hist_box[i]->color(FL_DARK1);
            hist_box[i]->labelsize(12);
            hist_box[i]->align(Fl_Align(FL_ALIGN_TOP_LEFT));
        }

      
        
        for (unsigned int i=0;i<4;i++){
            Fl_Choice* temp;
            hist_building.push_back(temp);
            hist_building[i] = new Fl_Choice(hist_box[i]->x()+130,hist_box[i]->y()+10,100,20,"Building");
            hist_building[i]->add("1");
            hist_building[i]->add("2");
            hist_building[i]->add("3");
            hist_building[i]->add("4");
            hist_building[i]->value(0);
            hist_building[i]->callback(rebuild_hist_input,&root);
            if (i==3){
                hist_building[i]->deactivate();
            }
            std::stringstream s;
            s<<"Choose building for metric selection for variable "<<i+1;
            hist_building[i]->tooltip("Choose building for metric selection");
        }
        
        for (unsigned int i=0;i<4;i++){
            Fl_Choice* temp;
            hist_metric.push_back(temp);
            hist_metric[i] = new Fl_Choice(hist_box[i]->x()+130,hist_box[i]->y()+30,100,20,"Metric");
            hist_metric[i]->add("-----");
            hist_metric[i]->value(0);
            hist_metric[i]->callback(plot_choice_cb,&root);
            if (i==3){
                hist_metric[i]->deactivate();
            }
            std::stringstream s;
            s<<"Choose metric variable";// "<<i+1;
            hist_metric[i]->tooltip("Choose metric variable");
        }
        
        for (unsigned int i=0;i<4;i++){
            Fl_Counter* temp;
            bin_count.push_back(temp);
            bin_count[i] = new Fl_Counter(hist_box[i]->x()+130,hist_box[i]->y()+50,100,20,"Number of bins");
            bin_count[i]->step(1,5);
            bin_count[i]->value(20);
            bin_count[i]->maximum(1000);
            bin_count[i]->minimum(4);
            bin_count[i]->align(Fl_Align(FL_ALIGN_LEFT));
            bin_count[i]->callback(plot_choice_cb,&root);
            bin_count[i]->deactivate();
            std::stringstream s;
            s<<"Specify number of bins for variable";// "<<i+1<<" (histogram only)";
            bin_count[i]->tooltip("Specify number of bins for varible");
        }
        
        
        
        hist_group->end();
        
        content->end();
        content->resizable(0);
        p->add(content);
        
        p->end();
        
        o->end();
        o->resizable(graph);
        oo->resizable(o);
    }
  
    
    

    oo->end();
    
    ooo->add(oo);
    Fl_Box* divider = new Fl_Box(10,win->h()-130,win->w()-20,10,0);
    divider->box(FL_FLAT_BOX);
    ooo->add(divider);
    
    
    ooo->end();

    out_read = new Fl_Text_Display(10,win->h()-120,win->w()-20,110,0);
    out_read->textfont(4);


    
    text->buff=buff;
    text->display=out_read;
    

    
    
    {

        root.calculating=FREE;
        root.buttons.progress=progress;
        root.buttons.text=text;
        root.buttons.warning_map=warning_map;
        root.buttons.mark_locations_choice=mark_locations_choice;
        root.buttons.turn_ang=turn_ang;
        root.draw=draw;
        root.graph=graph;
        root.win=win;
        root.draw_floorplan=1;
        root.snap_to_point=1;
        root.draw_grid=1;
        root.draw_route=1;
        root.transparent_walls=0;
        root.draw_points_label=0;
        root.draw_segments_label=0;
        root.color_scale=0;
        root.show_scale=1;
        root.draw_metric=0;
        root.items=items;
        root.menu=m;
    }
    
    
    
    push_time(text);
    push_output(text,"Program started.\n");
    push_time(text);
    push_output(text,"Loading resources... ");

    out_read->redraw();
  

    win->resizable(ooo);
    
    
     win->end();
    
    //END MAIN WINDOW
    
    
    //QUIT WINDOW
    
    quit_win = new Fl_Window(win->x()+0.5*win->w()-175,win->y()+0.5*win->h()-75,350,180,"Quit?");
    
    Fl_Box* quit_box = new Fl_Box(25,25,100,50,"Quit without saving?");
    quit_win->end();
    quit_win->hide();
    quit_win->set_modal();
    quit_win->callback((Fl_Callback*)hide);
    
    
    //END QUIT WINDOW
    
    //ABOUT WINDOW
    
    about_win = new Fl_Window(win->x()+0.5*win->w()-125,win->y()+0.5*win->h()-75,250,150,"About");
    Fl_Box* about_box = new Fl_Box(0,10,250,80,"Spatial Metric Calculator\nA tool for calculating graph metrics\non undirected graphs\nCopyright 2014, Richard Spinney\n GUI with FLTK 1.3.3\n http://www.fltk.org/");
    Fl_Button* about_ok = new Fl_Button(100,105,50,30,"OK");
    about_ok->callback((Fl_Callback*) about_ok_cb, about_win);
    about_win->add(about_box);
    about_win->add(about_ok);
    about_win->end();
    about_win->hide();
    about_win->set_modal();
    
    //END ABOUT WINDOW
    
    
    //ADD LINK WINDOW
    
    add_link_win = new Fl_Window(win->x()+0.5*win->w()-100,win->y()+0.5*win->h()-80,200,160,"Add/Remove link");
    start_add_link = new Fl_Int_Input(80,10,80,20,"Point 1");
    end_add_link = new Fl_Int_Input(80,40,80,20,"Point 2");
    Fl_Button *add_link_button = new Fl_Button(80,70,80,20,"Add");
    add_link_button->callback(add_link_cb,&root);
    Fl_Button *remove_link_button = new Fl_Button(80,100,80,20,"Remove");
    remove_link_button->callback(remove_link_cb,&root);
    Fl_Button *cancel_link_button = new Fl_Button(80,130,80,20,"Cancel");
    cancel_link_button->callback(close_link_win,&root);
    add_link_win->end();
    add_link_win->hide();
    add_link_win->set_modal();
    
    //END ADD LINK WINDOW
    
    //ROUTE DISPLAY WINDOW
    
    route_display = new Fl_Window(win->x()+0.5*win->w()-135,win->y()+0.5*win->h()-140,280,170,"Metric minimising routes");
    
    route_type_c=new Fl_Choice (100,30,150,20,"Route type");
    
    
    
    route_type_c->add("least (x,y) metric distance (graph distance)");
    route_type_c->add("least (x,y,z) metric distance (graph distance)");
    route_type_c->add("least angular distance");
    route_type_c->add("least number of segments");
    route_type_c->add("least number of turns");
    route_type_c->value(0);
    route_type_c->tooltip("Choice of metric which route minimises");
    
    start_route_input = new Fl_Int_Input(150,60,100,20,"Start point");
    end_route_input = new Fl_Int_Input(150,90,100,20,"End point");
    
    Fl_Button *cancel_route = new Fl_Button(40,120,100,25,"Cancel");
    cancel_route->callback(close_route_cb,&root);
    
    Fl_Button *ok_route = new Fl_Button(150,120,100,25,"OK");
    ok_route->callback(ok_route_cb,&root);
    
    route_display->end();
    route_display->hide();
    route_display->set_modal();
    
    //END ROUTE DISPLAY WINDOW
    
    //CUSTOM METRIC CALC WINDOW    
    
    custom_metric_calc = new Fl_Window(win->x()+0.5*win->w()-200,win->y()+0.5*win->h()-314,400,627,"Custom Metric");
    {
        
        
        metric_slot_choice = new Fl_Choice (180,20,155,20,"Metric #/display metric");
        metric_slot_choice->tooltip("Choice of metric to display/slot to populate with new metric");
        
        Fl_Box* box0 = new Fl_Box(20,60,360,30,"Step 1: Metric Type");
        box0->box(FL_GTK_DOWN_BOX);
        box0->color(FL_DARK1);
        box0->labelsize(12);
        box0->align(Fl_Align(FL_ALIGN_TOP_LEFT));
        
        custom_metric_basis=new Fl_Choice (180,65,155,20,"Metric basis");
        custom_metric_basis->add("Desk based");
        custom_metric_basis->add("Point based");
        custom_metric_basis->add("Segment based");
        custom_metric_basis->tooltip("Desk based: metric for each desk\nPoint based: metric for each point\nSegment based: metric for each segment");
        custom_metric_basis->value(0);
        custom_metric_basis->callback(metric_cb,&root);
       
        
        
        Fl_Box* box1 = new Fl_Box(20,110,360,77,"Step 2: Metric Measure");
        box1->box(FL_GTK_DOWN_BOX);
        box1->color(FL_DARK1);
        box1->labelsize(12);
        box1->align(Fl_Align(FL_ALIGN_TOP_LEFT));
        
        custom_metric_choice=new Fl_Choice (180,115,155,20,"Metric measure");
        
        
        custom_metric_choice->add("euclidian distance x,y (non-graph distance) to destinations");
        custom_metric_choice->add("euclidian distance x,y,z (non-graph distance) to destinations");
        custom_metric_choice->add("(x,y) metric distance (graph distance) to destinations");
        custom_metric_choice->add("(x,y,z) metric distance (graph distance) to destinations");
        custom_metric_choice->add("angular distance to destinations");
        custom_metric_choice->add("number of segments to destinations");
        custom_metric_choice->add("number of turns to destinations");
        custom_metric_choice->add("number of qualifying destinations");
        custom_metric_choice->add("number of equivalent weighted route metric minimising paths to qualifying destinations");
        custom_metric_choice->add("number of shortest paths through point or segment between qualifying destinations");
        custom_metric_choice->add("betweenness centrality (normalised) generalised to qualifying destinations");
        custom_metric_choice->add("length of subgraph of all minimum paths between qualifying destinations");
        custom_metric_choice->add("eigenvector centrality");
        custom_metric_choice->value(0);

        custom_metric_choice->callback(metric_cb,&root);
        custom_metric_choice->tooltip("Choice of what quantity to measure");
        

        
        
        
        inverse_option = new Fl_Check_Button(180,137,85,20,"Inverse");
        
        inverse_option->value(0);
        inverse_option->tooltip("Take the inverse of the metric (after statistic taken where relevant)");
        
        custom_metric_stat_choice=new Fl_Choice (180,160,155,20,"Use of destinations");
       
        custom_metric_stat_choice->add("Minimum");
        custom_metric_stat_choice->add("Maximum");
        custom_metric_stat_choice->add("Mean");
        custom_metric_stat_choice->add("Median");
        custom_metric_stat_choice->add("Geometric mean");
        custom_metric_stat_choice->add("Harmonic mean");
        custom_metric_stat_choice->add("Variance");
        custom_metric_stat_choice->value(0);
        custom_metric_stat_choice->tooltip("Statistic of metric for the qualifying locations (where relevant)");
        
        
        Fl_Box* box2 = new Fl_Box(20,207,360,210,"Step 3: Destination selection");
        box2->box(FL_GTK_DOWN_BOX);
        box2->color(FL_DARK1);
        box2->labelsize(12);
        box2->align(Fl_Align(FL_ALIGN_TOP_LEFT));
        
        
        
        custom_metric_destination_choice=new Fl_Choice (180,212,155,20,"Destination Type");
        custom_metric_destination_choice->add("Desks");
        custom_metric_destination_choice->add("Kitchens");
        custom_metric_destination_choice->add("Male WCs");
        custom_metric_destination_choice->add("Female WCs");
        custom_metric_destination_choice->add("Printers");
        custom_metric_destination_choice->add("Meeting rooms");
        custom_metric_destination_choice->add("Exits");
        custom_metric_destination_choice->add("Stairs");
        custom_metric_destination_choice->add("Lifts");
        custom_metric_destination_choice->add("Points");
        
        custom_metric_destination_choice->value(0);
        
        custom_metric_destination_choice->callback(metric_cb,&root);
        custom_metric_destination_choice->tooltip("Identify destinations (desk/point based metrics only)");
        
        
        
        destination_criteria_choice=new Fl_Choice (180,242,155,20,"Selection metric");
        destination_criteria_choice->add("euclidian distance x,y (non-graph distance) between basis and destination");
        destination_criteria_choice->add("euclidian distance x,y,z (non-graph distance) between basis and destination");
        destination_criteria_choice->add("minimum (x,y) metric distance (graph distance) between basis and destination");
        destination_criteria_choice->add("minimum (x,y,z) metric distance (graph distance) between basis and destination");
        destination_criteria_choice->add("minimum angular distance between basis and destination");
        destination_criteria_choice->add("minimum number of segments between basis and destination");
        destination_criteria_choice->add("minimum number of turns between basis and destination");
        destination_criteria_choice->value(0);
        destination_criteria_choice->deactivate();
        destination_criteria_choice->tooltip("Metric used to identify and select destinations");
        destination_criteria_choice->callback(metric_cb,&root);
        
        
        ask3 = new Fl_Button(345,242,20,20,"?");
        
        ask3->callback((Fl_Callback*) dest_guide_cb,&root);
        
        
        custom_metric_range_choice = new Fl_Choice (180,272,155,20,"Use destination(s) that");
        custom_metric_range_choice->add("All (don't exclude based on selection metric)");
        custom_metric_range_choice->add("Minimise selection metric");
        custom_metric_range_choice->add("Maximise selection metric");
        custom_metric_range_choice->add("Are less than or equal to 'range' from origin (based on selection metric)");
        custom_metric_range_choice->add("Are greater than or equal to 'range' from origin (based on selection metric)");
        custom_metric_range_choice->add("Maximise selection metric whilst less than or equal to 'range' from origin (based on selection metric)");
        custom_metric_range_choice->add("Minimise selection metric whilst greater than or equal to 'range' from origin (based on selection metric)");
        custom_metric_range_choice->add("Are equal to 'range' from origin (based on selection metric)");
        custom_metric_range_choice->value(0);
        custom_metric_range_choice->callback(metric_cb,&root);
        custom_metric_range_choice->tooltip("Usage of selection metric to choose subset of destination type for use in calculation");
        
        
        custom_metric_range =new Fl_Value_Input(180,302,155,20,"Range");
        custom_metric_range->value(0);
        custom_metric_range->deactivate();
        
        
        custom_metric_floor_choice=new Fl_Choice (180,332,155,20,"Floor based criteria");
        custom_metric_floor_choice->add("No constraint");
        custom_metric_floor_choice->add("Same floor");
        custom_metric_floor_choice->value(0);
        custom_metric_floor_choice->tooltip("Restrict destinations based on floor");
       
        
        custom_metric_visibility_choice=new Fl_Choice (180,362,155,20,"Visibility criteria");
        custom_metric_visibility_choice->add("No constraint");
        custom_metric_visibility_choice->add("Standing visible (all barriers)");
        custom_metric_visibility_choice->add("Sitting visible (all barriers)");
        custom_metric_visibility_choice->add("Standing visible (opaque barriers)");
        custom_metric_visibility_choice->add("Sitting visible (opaque barriers)");
        custom_metric_visibility_choice->value(0);
        custom_metric_visibility_choice->tooltip("Restrict destinations based on visibility (desk based only)");
        
        
        custom_metric_self_count = new Fl_Choice(180,392,155,20,"Self include");
        custom_metric_self_count->add("No");
        custom_metric_self_count->add("Yes (if qualifying)");
        custom_metric_self_count->add("Yes (force)");        
        custom_metric_self_count->value(0);
        custom_metric_self_count->tooltip("Exclude or Include the desk, desk's point, point, segment in the pool of possible/used destinations if it satifies selection criteria or not");
                
        
        Fl_Box* box3 = new Fl_Box(20,437,360,90,"Step 4: Route selection");
        box3->box(FL_GTK_DOWN_BOX);
        box3->color(FL_DARK1);
        box3->labelsize(12);
        box3->align(Fl_Align(FL_ALIGN_TOP_LEFT));

        
        route_type_choice=new Fl_Choice (180,442,155,20,"Route type");
        route_type_choice->add("(x,y) least metric distance (graph distance)");
        route_type_choice->add("(x,y,z) least metric distance (graph distance)");
        route_type_choice->add("least angular distance");
        route_type_choice->add("least number of segments");
        route_type_choice->add("least number of turns");
        route_type_choice->value(0);
        route_type_choice->deactivate();
        route_type_choice->tooltip("Metric to determine path/route from origin to destinaion");
        route_type_choice->callback(metric_cb,&root);
        ask2 = new Fl_Button(345,442,20,20,"?");
        
        ask2->callback((Fl_Callback*) route_guide_cb,&root);
        ask2->deactivate();
        
        max_iterations = new Fl_Int_Input (180,472,155,20,"Maxmimum iterations");
        {
            std::stringstream s;
            s<<20;
            max_iterations->value(s.str().c_str());
        }
        max_iterations->deactivate();
        max_iterations->tooltip("Maximum considered number of equally weighted paths.\nCyclical routes are ignored so a low value on a graph with zero weight cycles may not find a valid existing route.\nWarnings are given if this occurs.");
       
        route_stat_choice=new Fl_Choice (180,502,155,20,"Route ambiguity breaker");
        route_stat_choice->add("Minimum of metric measure");
        route_stat_choice->add("Maximum of metric measure");
        route_stat_choice->add("Mean of metric measures");
        route_stat_choice->value(0);
        route_stat_choice->deactivate();
        route_stat_choice->tooltip("If there are multiple shortest paths and a different metric\nis being counted along it (metric measure) than\nthe metric which defines the path\nuse which metric measure?");
        
        
        Fl_Box* box4 = new Fl_Box(20,547,360,30,"Advanced");
        box4->box(FL_GTK_DOWN_BOX);
        box4->color(FL_DARK1);
        box4->labelsize(12);
        box4->align(Fl_Align(FL_ALIGN_TOP_LEFT));
        
        cache_metric_data = new Fl_Check_Button(30,552,105,20,"Cache routes");
        cache_metric_data->value(0);
        cache_metric_data->tooltip("Save route information when calculating metrics to vastly speed up calculations that use them later.\nWarning: can dramatically increase memory usage (not recommended for large graphs)\nBecomes invalid/cleared when map or turn definition is changed");
        
        
        eig_tol = new Fl_Float_Input(275,552,60,20,"Eigenvector tolerance");
        {
            std::stringstream s;
            s<<"1e-3";
            eig_tol->value(s.str().c_str());
        }
        eig_tol->deactivate();
        eig_tol->tooltip("Convergence acceptance tolerance for eigenvector centrality computation as\nfractional difference beteen largest and smallest eigenvalue estimates");
        
        Fl_Button* desk_metric_button = new Fl_Button (210,592,135,20,"Calculate");
        desk_metric_button->callback((Fl_Callback*) desk_metric_cb1,&root);
        desk_metric_button->tooltip("Calculate metric");
        
        Fl_Button* desk_metric_display = new Fl_Button (55,592,135,20,"Display/Cancel");
        desk_metric_display->callback((Fl_Callback*) close_desk_metric,&root);
        desk_metric_display->tooltip("Display metric in chosen slot");

    }
    custom_metric_calc->end();
    custom_metric_calc->hide();
    custom_metric_calc->set_modal();
    
    //END CUSTOM METRIC CALC WINDOW
    
    //DESTINATION CHOICE WINDOW
    
    destination_guide= new Fl_Window(win->x()+0.5*win->w()-215,win->y()+0.5*win->h()-200,387,430);
    Fl_Box* im_dest_guide = new Fl_Box(0,0,387,189);
    
    Fl_Pixmap dest_pixmap(destination_guide_array);
   
    im_dest_guide->image(&dest_pixmap);
    
    
    im_dest_guide->box(FL_UP_BOX);
    
    
    
    {
    Fl_Text_Display *out_guide = new Fl_Text_Display(0,189,387,205);
    std::stringstream s;
    s<<"Destination selection criteria: \n\nOut of many possible destinations of a given type, this option selects the criteria for which of those is chosen as the destination (where applicable) independently of the route that will be used to calculate the metric or the type of metric calculated. \nEuclidian x-y distance will choose the closest destination regardless of the spatial map. \nLeast metric (graph) ditance will choose the destination which can be arrived at in the smallest amount of metres whilst following the spatial map. \nLeast angular distance will choose the destination which can be arrived at by the least angular deviation while following the spatial map.";
    Fl_Text_Buffer* buff=new Fl_Text_Buffer;
    buff->append(s.str().c_str());
    out_guide->buffer(buff);
    out_guide->wrap_mode(1,0);
    }
    Fl_Button *ok_dest_guide = new Fl_Button(277,400,100,25,"OK");
    ok_dest_guide->callback(ok_dest_guide_cb,&root);
    destination_guide->end();
    destination_guide->hide();
    destination_guide->set_modal();
    
    //END DESTINATION CHOICE WINDOW    
    
    
    //ROUTE CHOICE WINDOW
    
    route_guide= new Fl_Window(win->x()+0.5*win->w()-215,win->y()+0.5*win->h()-200,387,430);
    Fl_Box* im_route_guide = new Fl_Box(6,0,374,189);        

    Fl_Pixmap route_pixmap(route_guide_array);
    
    im_route_guide->image(&route_pixmap);
    
    im_route_guide->box(FL_UP_BOX);
    
    
    
    {
        Fl_Text_Display *out_guide = new Fl_Text_Display(0,189,387,205);
        std::stringstream s;
        s<<"Route selection criteria: \n\nOnce a destination is established, there are many possible routes to it from the orgin (desk).\nDifferent routes will posess different values for a given metric.\nThis option chooses the criteria by which such a route is chosen.\nShortest path will select the route which minimises the distance is metres travelled between the relevant points on the spatial map.\nLeast angular distance will select the route which minimises the angular deviation between the two points.";
        Fl_Text_Buffer* buff=new Fl_Text_Buffer;
        buff->append(s.str().c_str());
        out_guide->buffer(buff);
        out_guide->wrap_mode(1,0);
    }
    Fl_Button *ok_route_guide = new Fl_Button(277,400,100,25,"OK");
    ok_route_guide->callback(ok_route_guide_cb,&root);
    route_guide->end();
    route_guide->hide();
    route_guide->set_modal();
    
    //END ROUTE CHOICE WINDOW
    
    //BEGIN LICENSE
    {
   license_win = new Fl_Window(win->x()+0.5*win->w()-265,win->y()+0.5*win->h()-265,530,530,"License");
    Fl_Button *ok_license = new Fl_Button(420,495,100,25,"OK");
    ok_license->callback(close_license,&root);
    Fl_Text_Display *out_license = new Fl_Text_Display(10,10,510,530-60);
    std::stringstream s;
    #include "license.h"
        
        s<<license_string1<<license_string2<<license_string3;
        
    Fl_Text_Buffer* buff_license=new Fl_Text_Buffer;
    buff_license->append(s.str().c_str());
    out_license->buffer(buff_license);
    out_license->wrap_mode(0,0);
    //std::cout<<s.str()<<std::endl;
    out_license->labelsize(20);
    out_license->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
    license_win->end();
    license_win->set_modal();
    license_win->hide();
    }
    //END LICENSE
    
    //BEGIN GUIDE
    
    guide= new Fl_Window(win->x()+0.5*win->w()-250,win->y()+0.5*win->h()-265,500,530,"Quick Guide");
     Fl_Button *ok_guide = new Fl_Button(390,495,100,25,"OK");
    ok_guide->callback(ok_guide_cb,&root);
    {
    
    Fl_Text_Display *out_guide = new Fl_Text_Display(10,10,500-20,530-80);
    std::stringstream s;
    s<<"NOTE:\n\nFor more complete information refer to the manual that should have been provided with this software\n\nImportant things to remember:\n\nEvery construct on a given floor has the same z position in space controlled by the floor height counter. \nTo have many different heights you need many different floors.\n\nLocation markers (for desks, kitchens etc.) are first and foremost visibility markers. Whether they are identified as a separate location (desk, kitchen etc.) depends on the location type.\n All desks location markers count as separate desks regardless of the point they are associated with.\nFor all other location types (kitchens etc.) all location markers at a given spatial map point count as a single location with the markers providing many visibility points for that location.\n\nInterface:\n\nGeneral:\n\tMiddle mouse: Translate\n\tLeft mouse: Rotate in 3D\n\tScroll: Zoom/alter field of view\n\tCntrl:Override rotate in 3D (translate)\nEditing:\n\tLeft mouse: Add\n\tRight mouse: Select\n\tCntrl: Overide add (for trackpad users)\n\tAlt: Drag\n\tShift: Snap on\n\tShift + Cntrl: Rotate guide lines (spatial map points and desks only)\nMisc:\n\tSpatial Links:\n\t\tLeft click: Start link/end link\n\t\tAny other click: Cancel\n\tWalls:\n\t\tO (When selected): Make opaque\n\t\tT (When selected): Make transparent\n\t\tLeft Click: Start wall (first), end wall (second)\n\t\tAny other click: Cancel\n\tFloor area:\n\t\tLeft click: Start polygon/extend polygon\n\t\tRight click: Close geometry (end polygon)\n\tFree view:\n\t\tW: Move forward\n\t\tDrag mouse: change direction\n\t3D perspective:\n\t\tW: Reduce depth\n\t\tS: Increase depth\n\t\tCntrl + Mousewheel: Increase/Reduce depth.";
    Fl_Text_Buffer* buff=new Fl_Text_Buffer;
    buff->append(s.str().c_str());
    out_guide->buffer(buff);
    out_guide->wrap_mode(1,0);
    
    out_guide->labelsize(20);
    out_guide->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
    
    }
    /*
    Fl_Tabs* guide_tabs = new Fl_Tabs(10,10,500-20,530-60);
    
    
    {Fl_Group* o = new Fl_Group(guide_tabs->x(), guide_tabs->y()+20, guide->w(),guide->h(), "Basics");
    
        Fl_Text_Display *out_guide = new Fl_Text_Display(o->x(),o->y(),500-20,530-80);
        std::stringstream s;
        s<<"General:\n\tMiddle mouse: Translate\n\tLeft mouse: Rotate in 3D\n\tScroll: Zoom/alter field of view\n\tCntrl:Override rotate in 3D (translate)\nEditing:\n\tLeft mouse: Add\n\tRight mouse: Select\n\tCntrl: Overide add (for trackpad users)\n\tAlt: Drag\n\tShift: Snap on\n\tShift + Cntrl: Rotate guide lines (spatial map points and desks only)\nMisc:\n\tSpatial Links:\n\t\tLeft click: Start link/end link\n\t\tAny other click: Cancel\n\tWalls:\n\t\tO (When selected): Make opaque\n\t\tT (When selected): Make transparent\n\t\tLeft Click: Start wall (first), end wall (second)\n\t\tAny other click: Cancel\n\tFloor area:\n\t\tLeft click: Start polygon/extend polygon\n\t\tRight click: Close geometry (end polygon)\n\tFree view:\n\t\tW: Move forward\n\t\tDrag mouse: change direction\n\t3D perspective:\n\t\tW: Reduce depth\n\t\tS: Increase depth\n\t\tCntrl + Mousewheel: Increase/Reduce depth.";
        Fl_Text_Buffer* buff=new Fl_Text_Buffer;
        buff->append(s.str().c_str());
        out_guide->buffer(buff);
        out_guide->wrap_mode(0,0);
        
        out_guide->labelsize(20);
        out_guide->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
        o->end();
        
      
    }
    {Fl_Group* o = new Fl_Group(guide_tabs->x(), guide_tabs->y()+20, guide->w(),guide->h(), "Spatial map");
        
        Fl_Text_Display *out_guide = new Fl_Text_Display(o->x(),o->y(),500-20,530-80);
        std::stringstream s;
        s<<"The basic structure on which all uses of this tool rests is the spatial map.\n\nThe spatial map is a graph structure and consists of two constructs 1) points 2) links.\n\nPoints are singular positions in space whilst links connect them.\n\nIn this tool points posess an integer identifier (starting at 0), a position (x,y,z) alongside a floor designation: ALL POINTS WITH A UNIQUE Z POSITION CONSTITUTE A SEPERATE FLOOR (whether this is physical or not). Links are represented through a symmetric matrix whereby the two indices represent the point identifiers, a value 0 indicates no link and 1 indicates a link.\n\nBefore anything should or can be done the spatial map must be created (this can be read in through the loading of a suitable points and matrix file, see importing/saving).\n\nTo do this we first need to plan the structure and determine how many separate floors or z values are required to describe the map bearing in mind points in the spatial map that reside between more traditional, horizontal, floors also count (floors cannot currenty be wholesale shifted so planning is crucial).\n\nOnce this is determined press the 'Add' button until there are enough floors/z layers for the spatial map (The floor count starts at 0)\n\nSelect each floor and load a dxf floor plan into each floor where appropriate (File->Read pre-exisiting structure->Open dxf floorplan or cmd/cntrl-x). These can be moved around as appropriate with the tools under the 'Floorplan' sub tab.\n\nOnce the floorplans are in alignment we can start constructing the spatial map. To do so we enter the main edit sub tab and select 'Spatial graph: points'. We can mark points now with the left mouse button. NOTE: editing can only occur when a specific floor is selected and then only that floor is shown.\n\nThe position of points may be related to others as such pressing SHIFT in the vicinity of a given point will overlay guide lines at 90 degrees to that point for easier placement. Simultaneously pressing CNTRL and dragging the mouse will change the angle of these guide lines.\n\nPoints can be selected with the right mouse and removed with the appropriate buttons and can be dragged with the left mouse if holding the ALT key.\n\nOnce the points are set select 'Spatial Graph: Links' in the main edit sub tab. Click on a point to begin creating a link and click on another to finish the link. Right click to cancel a started link and to highlight existing links for deletion.\n\nTo connect points between floors click on the first point on one floor, change the floor, then click on the second point. Such links will only be viewable when viewing 'All floors'\n\nLinks are maintainted once defined: do not delete a point and replace it if the links are correct, simply drag the point to the new position. The guide lines can still be used in such an operation.\n\nImportant: Naturally, the specific definition of the spatial map leads to the specific spatial metrics. Most are not too sensitve to the exact specification. However, one which is subtle is the angular distance associated with a route. The algorithm utilsied detects angular changes in the x-y plane between three points connected by two links. This means that turns onto perfectly vertical lines are always associated with zero angular distance. As such heading in the positive x direction, then performing a perfectly vertical movement, then heading in the negative x direction will incur zero angular distance even though without the perfect vertical movement 180 degrees of angular change would be incurred. If the vertical movement is not absolutely vertical then angular distance will be detected. This should be understood and accounted for when designing the spatial map.\n\nThe basic rule to remember is: If you wish to count angular distance which occurs over different heights then connections over such different heights should incur some x-y movement too.\n\nAn example is best as an illustration. Consider a staircase which doubles back on itself. To count the angular distance associated with travsersing such a structure movement in the z direction should always be accompanied by x-y movement, for instance a diagonal line from one landing to another. If you do not wish to count such angular distance then all z movement needs to occur on its own such that x-y movement and z movement are exclusive.  ";
        Fl_Text_Buffer* buff=new Fl_Text_Buffer;
        buff->append(s.str().c_str());
        out_guide->buffer(buff);
        out_guide->wrap_mode(1,0);
        
        out_guide->labelsize(20);
        out_guide->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
        o->end();
    }
    {Fl_Group* o = new Fl_Group(guide_tabs->x(), guide_tabs->y()+20, guide->w(),guide->h(), "Editing");
        
        Fl_Text_Display *out_guide = new Fl_Text_Display(o->x(),o->y(),500-20,530-80);
        std::stringstream s;
        s<<"Locations\n\n\nTo generate metrics, desks, at the very least, must be marked. Desks are an example of a location all of which are added in the same way.\n\nTo do so we select a floor then select the location in the edit option of the 'Main edit' sub tab and click where to mark the location.\n\nLocations can only be added if there are points in the spatial map on that floor.\n\nWhen a location is marked it is simulateously associated with a point of the spatial map (the nearest one). If this automatic selection is incorrect this can be overridden in the input field in the 'Main edit' sub tab by first selecting the location with a right click which will highlight it. Be sure to click 'OK' to ensure the point is changed.\n\nA text label can be attached to locations using a similar input field. Again be sure to click 'OK' to ensure the label is entered/changed.\n\nIMPORTANT: All metrics are derived from the spatial map and the location's association with it by means of the point specification. The actual position of the location is used exclusively for visibility information and so should be marked accordingly.\n\nNon-desk locations only: Since the visibilty of a location (eg. a kitchen) is not always appropriately determined by a single point in space, several 'kitchens' (for example) can be marked to provide multiple visibility points in different positions. These multiple locations (kitchens) will be considered to be one location (eg. kitchen) if they are all associated with the same spatial map point. This does NOT apply to desks.\n\nDesks (only) can be easily marked in a line using SHIFT to mark guidelines. These guidelines can be rotated by simutaneously holding CNTRL and dragging the left mouse. All locations can be selected (to change labels/points) with a right click when the suitable edit option is selected in the 'main edit' sub tab. All locations can be dragged when holding ALT.\n\n\nWalls\n\n\nTo calculate visibility or to have recognisable 3D rendering walls must be marked. There are two categories of wall: standing and sitting, deemed to be ceiling height and 1m respectively. In this respect one might deem standing walls to be walls which block line of sight and sitting walks to be walls which block direct movement.\n\nTo mark these walls, select a floor and make two separate clicks to start the wall and end it. After the first click it can be cancelled with a right click. Existing walls can be selected with a right click and deleted if necessary.\n\nPressing SHIFT will enable snap on behaviour (on to lines in the floor plan) to ease wall placement. Default snap on behaviour is to the nearest point of the nearest line within a tolerance. This can be changed with the checkbox option to prioritise the end points of lines which may be suitable for wall placement.\n\nFor visualisation only: Walls can be rendered as opauque or translucent. When a wall is selected pressing O will turn it opaque, T will turn it translucent.\n\n\nFloor areas\n\n\nTo designate floor area to calculate net usable area, again a floor must be selected alongside the appropriate option in the 'Main edit' sub tab.";
        Fl_Text_Buffer* buff=new Fl_Text_Buffer;
        buff->append(s.str().c_str());
        out_guide->buffer(buff);
        out_guide->wrap_mode(1,0);
        
        out_guide->labelsize(20);
        out_guide->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
        o->end();
    }
    
    {Fl_Group* o = new Fl_Group(guide_tabs->x(), guide_tabs->y()+20, guide->w(),guide->h(), "Metrics/data.");
        
        Fl_Text_Display *out_guide = new Fl_Text_Display(o->x(),o->y(),500-20,530-80);
        std::stringstream s;
        s<<"Metrics\n\n\nSpatial metrics are produced from various different marked constructs, some require intermediary steps and many take some time to generate. As such these steps are performed manually when the user has deemed the marking of spatial maps etc. complete. As such, should any changes occur to them (marking new points/deleting links/adding walls etc.) one must re calculate the relevant metrics and the intermidiary steps.\n\nThe main construct is an intermidiary step which characterises the various weights between all of the points on the spatial map. This is not used directly as a metric, but is from what all weight based metrics are derived. A complete set of points and links are required to complete this step. To produce/update such a construct select the 'Metrics' sub tab and click the 'create' button. Progress will be displayed in the progress bar and the information window. This needs to be recalculated should any points be added/removed or links added/removed. This construct can become very large for very large spatial maps (>500 ish points) so we recommend always saving marked constructs in a project file before this step.\n\nThe main metrics (distance etc.) are produced using the weights construct, the markings of the locations and visibility information if visibility dependent. These are calculated by .... These need to be recalculated if a spatial point/link is added/removed or if a relevant location is added/removed or a wall added/removed if visibility dependent.\n\nVisibility is calculated in the same sub tab when the walls and desks are fully specified. Should a desk or wall be added/removed visibility will need to be recalculated.\n\n\nData\n\n\nMetrics are always calculated on a 'by desk' basis since they are intended to be asociated with individuals.\n\nData, in all forms, whether that be the points and links of the spatial map, location information etc. can be viewed in the data viewer tab where it can be exported to a file of the users choice. Data is in csv format with a header where appropriate. NOTE: it is advisable to refresh the data if any changes have been made to the data shown. ";
        Fl_Text_Buffer* buff=new Fl_Text_Buffer;
        buff->append(s.str().c_str());
        out_guide->buffer(buff);
        out_guide->wrap_mode(1,0);
        
        out_guide->labelsize(20);
        out_guide->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
        o->end();
    }
    {Fl_Group* o = new Fl_Group(guide_tabs->x(), guide_tabs->y()+20, guide->w(),guide->h(), "Importing/Saving");
        
        Fl_Text_Display *out_guide = new Fl_Text_Display(o->x(),o->y(),500-20,530-80);
        std::stringstream s;
        s<<"Importing\n\n\nCreating spatial maps can all be performed natively in this program. However, you may find it easier to generate them yourself in software of your choice. This program supports this and allows importing of the spatial map structures. To do so they must be in an appropriate format. This involves producing two distinct text files (non binary .txt .dat .csv .yourfile etc.).\n\nPoints file:\n\nThis is a file which specifies the positions of the points in space. These files must be comprised of 'rows'. These rows can be in one of three formats 'x y z', 'point# x y z', 'point# x y z floor#'. Each item within the row must be separated by spaces, commas or semi-colons. Each 'row' must appear on a separate line or all be on a single line and encased in a pair of brackets '()', '[]' or '{}'. Point numbers and floor numbers should be integers and points with them same floor number should have the same z-value. If floor number is not specified a floor is automatically assigned based on identical z values. Example formats could therefore take \n    (0, 1.23 1.23 1.23 1), (1, 2.23 2.23 2.23 1)...\n     or \n    1.23; 1.23; 1.23\n    2.23; 2.23; 2.23\n    ...\namongst others.\n\nMatrix file:\n\nThe matrix file has the same format constraints as the points file, but each row now represents a row of a symmetric matrix representing links between the points. '1' indicates a link. '0' indicates no link. The rows must be in the same order as the points in the points file. For instance if the 5th entry on the 7th line is '1' then point 5 and point 7 in the points file are connected. The 7th entry on the 5th line must therefore also be '1'. Diagonal values are arbitrary, but all must be integers. \n\nExported points/matrix data from the main data tab is in a suitable format to be imported when the preamble is removed.\n\nImport points shortcut: cmd/cntrl+p\n\nImport matrix shortcut: cmd/cntrl+m\n\nNote: It is possible to load only the points and connect them manually or to load both and to edit the points and/or connections between them. However, a matrix file cannot be loaded without the relevant number of points already having been loaded.\n\n\n\nSaving\n\n\nThis program supports saving of all constructs and metrics by means of serialisation. Metrics and spatial map information can be very large so we recommend clearing such constructs before saving and exporting them in a text format from the data tab if required. The suffix for the produced files needs to be '.proj' to be able to sucessfully read them back in.";
        Fl_Text_Buffer* buff=new Fl_Text_Buffer;
        buff->append(s.str().c_str());
        out_guide->buffer(buff);
        out_guide->wrap_mode(1,0);
        
        out_guide->labelsize(20);
        out_guide->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
        o->end();
    }
    guide_tabs->end();
     
     */
    guide->end();
    guide->hide();
    guide->set_modal();
    
    
    ///END GUIDE
    
   
    
    
    
    
    
    
    //BEGIN SPLASH SCREEN
    win2= new Fl_Window(0.5*(Fl::w()-679),0.5*(Fl::h()-266),679,266);
    win2->begin();
    splash = new Fl_Box(0,0,679,266);
    
    
    
    Fl_Pixmap splash_pixmap(splash_screen);

    splash->image(&splash_pixmap);
    
    win->hide();


    splash->box(FL_UP_BOX);
    win2->end();
    win2->clear_border();
    

    win2->set_modal();
    win2->show();
    //END SPLASH SCREEN
    
    
    
    
    start_time=clock();
   
    root.calculating=INITIALISING;
    Fl::add_idle(idle_load,&root);
    
 
    
    Fl::run(); // Turn control over to FLTK
    return 0;
}








