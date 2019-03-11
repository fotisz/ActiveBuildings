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
#ifndef DRAW_WINDOW_H
#define DRAW_WINDOW_H


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
#include <stdio.h>
#include <time.h>

#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
//#include <FL/glu.h>
#include <FL/glut.H>
#include <FL/Fl_Menu_Bar.H>
#include "graph_new.h"
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
#include "boost/date_time/posix_time/posix_time.hpp"
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_Check_Button.H>


#include "globals.h"
#include "container.h"


#undef min
#undef max



class MyWindow : public Fl_Gl_Window {
    
    //methods
    void draw();
    int handle(int);
    void drawcube(double,double,double,double);
    void drawcube(double,double,double,double,GLuint);
    void drawcubeedge(double,double,double,double,double,int);
    void showpoints();
    void showsegments();
    void showscale();
    void showcolorscale(double,std::vector<double>);
    void showrotatinglegend(double);
    void drawaxialmap(std::vector<double>,int);
    void drawroute();
    void drawpoints(std::vector<double>);
    void draw_background(double);
    void set_rotation();
    void setzoom();
    void draw_desks();
    void draw_desks_model(int);
    void draw_locations(std::vector<double>,int);//all the overloading....
    void draw_locations(std::vector<location>*,int,int,int,int);
    void draw_location_links(std::vector<location>*,int,int,int);
    void draw_locations(std::vector<location>*,std::vector<double>*,double,double,int);
    void draw_walls(std::vector<wall>,std::vector<wall>);
    void draw_visibility();
    void drawpyramid(double , double , double , double);
    void showcoords();
    void draw_floorplan(double);
    
    void draw_block_nested(plan*,block_entry,double,int);//nested only
    void DrawCircle(circle,double);
    void DrawArc(arc, double);
    void DrawLines(line_in,double); //nested only
    void draw_snapon(double,double);
    void test_snapon();
    void show_FOV();
    void draw_areas(double);
    void ear_clipping(std::vector<vec>,std::vector<triangle>*);
    int test_in_triangle(double, double,triangle);
    void every_frame_calc_alt();
    void move_alt();
    
    void translate_frustum();
    
    void draw_floor();
    void draw_ceiling();
    
    bool point_in_polygon(double, double,std::vector<vec>);
    int pnpoly(double, double,std::vector<vec>);
    
    void mouseless_zoom(int);
    
    

    int pressed_for;
    
    boost::posix_time::ptime move_time;
    boost::posix_time::ptime move_start_time;
    
    double xfr,yfr,zfr,axx,azz;
    
    
    //data
    double frac_x,frac_y;
    
    double minx_overall,maxx_overall,miny_overall,maxy_overall;
    int h_initial,w_initial;
    double zoom_level;
    int drag;
    int zoom_mag;
    double xdown,ydown,xdragdown,ydragdown,xdragdown_old,ydragdown_old;
    double minx_down,maxx_down,miny_down,maxy_down;
    
    double midx,midy,midz;
    double ang,ang_old,rot[3],old_rot[3],cv[3],ov[3];
    double min_zoom,max_zoom;
    double image_x,image_y;
    double image_width,image_height;
    int snapon,snap_point;
    double snapx,snapy;
    Fl_Cursor old_cursor;
    
    double xfp_move,yfp_move,zfp_move;
    double xh,yh,zh,xh_offset,yh_offset,zh_offset,xh_offset_cp,yh_offset_cp,zh_offset_cp;
    double z_dist;
    double step_length;
    

    int metric_type;
    int count;
    int shift_on;
    
    wall temp_wall;
    
    const char *pathPtr;
    std::string bundle_address;
    
    int load_wall,load_ceiling,load_floor;
    
    
    

    
    GLuint textureObject;
    
    
    GLuint wall_texture;
    GLuint floor_texture;
    GLuint ceiling_texture;
    GLuint muzzleflash_texture;
    GLuint explosion_texture;
  
    
    
    container* root;
    
    
    
    Fl_Choice*& mark_locations_choice;
    
    //END REFERENCES TO ACCESS MAIN DATA
    
public:
    
    building* current;
    
    double minx,maxx,miny,maxy,minz;
    double ang_x,ang_y,ang_z,ang_xfp,ang_zfp;
    double xfp,yfp,zfp;
    int first_draw;
    double z_gap;
    int flr_choice;
    int all_flrs;
    int selector;
    int selected;
    int selected_matrix[2];
    double new_zoom;
    int zoom;
    int start_link,end_link;
    int drag_point;
    int set_first;
    double ang_snap;
    
    std::vector<std::vector<triangle> > floorarea_triangles;
    
    void deselect();
    void calculate_areas(std::vector<std::vector<vec> >* polygons);
    void load_textures();
    int loaded_textures;
    
    GLubyte* pixels;  //for image export
    
    MyWindow(int , int , int , int , const char *, container*);
    
    static void mouseless_zoom_in_entry(Fl_Widget*, void*);
    static void mouseless_zoom_out_entry(Fl_Widget*, void*);
    
    void show_warning_visibility(){
        
        if (root->current->vis_valid){
            push_time(text);
            push_output(text,"WARNING: Visibility data out of date.\n");
        
        root->current->vis_valid=0;
        update_progress(progress_vis,1,2);
            
        warning_visibility->show();
        warning_visibility->redraw();
        warning_visibility->parent()->redraw();
        win->redraw();
        Fl::check();
        Fl::flush();
        }
    }
    
    void show_warning_map(){
        
        if ((root->current->map_valid)||(root->current->lines_valid)){
            push_time(text);
            push_output(text,"WARNING: Map structures out of date.\n");
            
            root->current->map_valid=0;
            update_progress(progress,1,2);
            root->current->lines_valid=0;
            update_progress(progress_lines,1,2);
            root->current->lines_valid=0;
            update_progress(progress_lines,1,2);
            warning_map->show();
            warning_map->redraw();
            warning_map->parent()->redraw();
            win->redraw();
            Fl::check();
            Fl::flush();
        }
    }
    
        
    int is_valid(){
        if (valid()){
            return 1;
        }
        else{
            return 0;
        }
    }
        
    GLuint TexID;
    const char *texturefilename;
    
    void TextureFilename(const char *val) {
        texturefilename = val;
    }
    const char *TextureFilename() const {
        return(texturefilename);
    }
    
    
    
};




#endif /*DRAW_WINDOW_H*/