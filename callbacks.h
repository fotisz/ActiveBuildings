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

#ifndef CALLBACKS_H
#define CALLBACKS_H

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


#include <math.h>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <cctype>
#include <algorithm>



#include <stdlib.h>


#include "threads.h"
#include "globals.h"
#include "graph_new.h"
#include "building.h"
#include "draw_window.h"
#include "locations.h"
#include "desk_metrics.h"
#include "input_output.h"
#include "plot.h"

#include "dxf_recursive_model.h"
#include "container.h"
#include "button_ex.h"
#include "callbacks.h"



#undef min
#undef max

void open_adjlist_cb(Fl_Widget* /*widget*/,void* data);
void* read_adj_list_pass(void*);

void close_route_cb(Fl_Widget*,void*);
void close_link_win(Fl_Widget*,void*);
void open_license(Fl_Widget*,void*);
void close_license(Fl_Widget*,void*);

void floorplan_to_map(Fl_Widget*,void*);
void link_win_cb(Fl_Widget*,void*);
void add_link_cb(Fl_Widget*,void*);
void remove_link_cb(Fl_Widget*,void*);

void open_desks_cb(Fl_Widget*,void*);
void open_kitchens_cb(Fl_Widget*,void*);
void open_printers_cb(Fl_Widget*,void*);
void open_maleWC_cb(Fl_Widget*,void*);
void open_femaleWC_cb(Fl_Widget*,void*);
void open_meeting_cb(Fl_Widget*,void*);
void open_exit_cb(Fl_Widget*,void*);
void open_stair_cb(Fl_Widget*,void*);
void open_lift_cb(Fl_Widget*,void*);

void open_points_cb(Fl_Widget*,void* data) ;
void open_matrix_cb(Fl_Widget*,void* data) ;

void create_structures(Fl_Widget* widget,void* data);


void test(Fl_Widget* widget,void* data);

void* read_desks_pass(void* data);
void* read_kitchens_pass(void* data);
void* read_printers_pass(void* data);
void* read_maleWC_pass(void* data);
void* read_femaleWC_pass(void* data);
void* read_meeting_pass(void* data);
void* read_exit_pass(void* data);
void* read_stair_pass(void* data);
void* read_lift_pass(void* data);

void* read_points_pass(void* data);
void* read_matrix_pass(void* data);
void visibility_cb(Fl_Widget* widget, void* data);
void* visibility_calc1(void* data);
int visible_test(double x1,double x2,double y1,double y2,double x_1,double x_2,double y_1,double y_2);
void desk_metric_cb(Fl_Widget* widget, void* data);
void* desk_metric_calc(void* data);
void save_binary_cb(Fl_Widget* widget, void* data);
void* save_binary_thread(void* data);
void read_binary_cb(Fl_Widget* widget, void* data);
void* read_binary_thread(void* data);


void open_dxf_cb(Fl_Widget* widget, void* data);
void* open_dxf_thread(void *);
void scale_floorplan_cb(Fl_Widget*,void*);
void redraw_mark(Fl_Widget* widget,void*);
void floor_choice_cb(Fl_Widget*,void*);
void three_d_cb(Fl_Widget*,void*);
void set_floor_choice(void*);
void reset_floor_choice(void*);
void undo(Fl_Widget*,void*);
void add_floor_cb(Fl_Widget*,void*);
void find_extrema_points(void*);

void redraw_data(Fl_Widget*, void*);

void save_data(Fl_Widget*, void*);

void clear_locations_cb(Fl_Widget*, void*);
void update_locations(std::vector<location>*,int);
void clear_selected_location_cb(Fl_Widget*, void*);
void clear_last_location_cb(Fl_Widget*, void*);
void location_label_cb(Fl_Widget*, void*);
void point_in_cb(Fl_Widget*, void*);
void zfloor_cb(Fl_Widget*, void*);

void shiftx_cb(Fl_Widget*, void*);
void shifty_cb(Fl_Widget*, void*);

void plot_choice_cb(Fl_Widget*, void*);

//temporary

void close_desk_metric(Fl_Widget* /*widget*/, void* data);
void desk_metric_cb1(Fl_Widget* widget, void* data);
void* custom_metric_thread(void* data);

void show_route_menu_cb(Fl_Widget*, void*);
void show_route_cb(Fl_Widget*, void*);



void warning_map_cb(Fl_Widget* /*widget*/, void* data);
void building_data_choice_cb(Fl_Widget* widget_, void* data);
void building_choice_cb(Fl_Widget* widget_, void* data);
void main_callback(Fl_Widget*, void* data);

void idle_load(void* data);
void clear_metrics_cb(Fl_Widget* /*widget*/, void* data);
void* showmain(void * /*data*/);
void* showsecond(void * /*data*/);
void redraw(Fl_Widget* /*widget*/,void* data);

void redraw_from_thread(void* data);
void check_from_thread(void * /*data*/);
void redraw_show(Fl_Widget* widget, void* data);
void scheme0(Fl_Widget* /*widget*/,void* /*data*/);
void scheme1(Fl_Widget* /*widget*/,void* /*data*/);

void scheme2(Fl_Widget* /*widget*/,void* /*data*/);

void main_tab_callback(Fl_Widget* widget,void* data);
void tab_callback(Fl_Widget* widget,void* data);
void LayerCB(Fl_Widget* /*widget*/, void* data);

void undo_fullscreen(Fl_Widget* /*widget*/, void* data);

void go_fullscreen(Fl_Widget* /*widget*/, void* /*data*/);
void about_cb(Fl_Widget* /*widget*/, void* data);
void about_ok_cb(Fl_Widget* /*widget*/, void* data);
void guide_cb(Fl_Widget* /*widget*/, void* data);
void dest_guide_cb(Fl_Widget* /*widget*/, void* data);
void route_guide_cb(Fl_Widget* /*widget*/, void* data);


void ok_guide_cb(Fl_Widget* /*widget*/, void* /*data*/);
void ok_dest_guide_cb(Fl_Widget* /*widget*/, void* /*data*/);
void ok_route_guide_cb(Fl_Widget* /*widget*/, void* /*data*/);
void show_route_cb(Fl_Widget*,void* data);
void show_floorplan_cb(Fl_Widget*,void* data);
void show_grid_cb(Fl_Widget*,void* data);
void snap_to_point_cb(Fl_Widget*,void* data);
void transparent_walls_cb(Fl_Widget*,void* data);
void points_label_cb(Fl_Widget*,void* data);
void segments_label_cb(Fl_Widget*,void* data);

void color_scale_cb(Fl_Widget*,void* data);
void show_scale_cb(Fl_Widget*,void* data);
void* route_thread(void* data);
void ok_route_cb(Fl_Widget* /*widget*/, void* data);

void quit_callback(Fl_Widget*, void*);
void hide(Fl_Widget*);

void colour_cb(Fl_Widget* /*widget*/, void* data);

void rebuild_hist_input(Fl_Widget*,void* data);

void metric_cb(Fl_Widget*,void*);


void show_metric_cb(Fl_Widget*, void*);

void custom_destination_cb(Fl_Widget* , void* );
void reset_view(Fl_Widget* widget, void* data);
void kill_thread(Fl_Widget*, void* data) ;

void show_warning_map_cb(void* data);
void show_warning_vis_cb(void* data);
    

void line_calc_cb(Fl_Widget*, void*);

void lines_to_walls(int, container*);
void lines_to_walls_standing(Fl_Widget*, void*);
void lines_to_walls_standing_transparent(Fl_Widget*, void*);
void lines_to_walls_sitting(Fl_Widget*, void*);
void lines_to_walls_sitting_transparent(Fl_Widget*, void*);
void addarc(arc,std::vector<wall>*,plan,int, int,container*);


void clear_building(Fl_Widget*,void*);

void map_consistency(Fl_Widget*, void*);
void* check_map(void*);


class LayerWin
{
private:
    struct pass{
        container* root;
        int floor;
        int layer;
    };
    struct close_pass{
        void* point;
        container* root;
    };
private:
    int m_layers;
    int floor;
    Fl_Window* m_dlg;
    //std::vector<Fl_Check_Button*> check_buttons;
    
    std::vector<pass> pass_data;
    close_pass pass_close;
    container* root; //very dodgy: look into?
    
public:
    LayerWin(void* data)
    :  floor(floor_choice->value()-1),m_dlg(0),root((container*) data)
    
    {//note these implicit initialisations have to be in the same order as initialisation within the class
        
        
    }
    ~LayerWin()
    {
        delete m_dlg;
    }
    
    
    static void close_layer_dialogue(Fl_Widget* /*widget*/, void* data){
        
       
        
        close_pass* pass_close =(close_pass*) data;
        LayerWin* self = (LayerWin*) (pass_close->point);
        container* root = pass_close->root;
        delete self;
        root->draw->first_draw=1;
        root->draw->redraw();
        
    }
    
    
    static void layer_check_cb(Fl_Widget* widget, void *data_){
        
        Fl_Check_Button*  button = (Fl_Check_Button*) widget;

        
        pass* data = (pass*) data_;
        container* root = data->root;
        int floor = data->floor;
        int layer = data->layer;
        
        root->current->floor_plans[floor].layers[layer]=(int)button->value();
        
        root->draw->redraw();
        
    }
    
    void Create()
    {
    
        
        std::vector<plan>& floor_plans = root->current->floor_plans;
        
        m_dlg  = new Fl_Window(win->x()+0.5*win->w()-250,win->y()+0.5*win->h()-265,500,530,"Layers");
        m_dlg->set_modal();
        Fl_Scroll* layer_scroll = new Fl_Scroll(5,5,490,475);
        int count=0;
        
        for (unsigned int i=0;i<floor_plans[floor].layers.size();i++){ //have to be SO careful: using push_back etc changes pointers!!!!!!!!
            
            pass temp;
            temp.floor=floor;
            temp.layer=i;
            temp.root=root;
            pass_data.push_back(temp);
            
          
        }
        
        for (unsigned int i=0;i<floor_plans[floor].layers.size();i++){
            
            if (floor_plans[floor].layers_used.size()){
                if (floor_plans[floor].layers_used[i]){
                    
                    Fl_Check_Button* temp = new Fl_Check_Button(10,10+count*20,20,20,floor_plans[floor].layers_name[i].c_str());
                    temp->value(floor_plans[floor].layers[i]);                    
                    
                    temp->callback(layer_check_cb,&pass_data.at(i));
                    count++;
                }
            }
            else{
                Fl_Check_Button* temp = new Fl_Check_Button(10,10+count*20,20,20,floor_plans[floor].layers_name[i].c_str());
                temp->value(floor_plans[floor].layers[i]);
                
                temp->callback(layer_check_cb,&pass_data.at(i));
                count++;
                
            }
            
        }
        
        
        layer_scroll->end();
        Fl_Button* layer_dialogue_close = new Fl_Button(400,490,80,30,"Close");
        pass_close.point=this;
        pass_close.root=root;

        layer_dialogue_close->callback(close_layer_dialogue,&pass_close);
        m_dlg->callback(close_layer_dialogue,&pass_close);
        m_dlg->end();
        m_dlg->show();
        
    }
    
    
    
    
};

#endif //CALLBACKS_H