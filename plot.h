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

#ifndef PLOT_H
#define PLOT_H

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
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_Check_Button.H>

#include "globals.h"
#include "container.h"

#undef min
#undef max


class MyPlot : public Fl_Gl_Window {
    
    //methods
    void draw();
    int handle(int);
    
    //data
    double frac_x,frac_y;
    double minx,maxx,miny,maxy,minz,maxz;
    double minx_overall,maxx_overall,miny_overall,maxy_overall;
    int h_initial,w_initial;
    double zoom_level;
    int zoom;
    int drag;
    int zoom_mag;
    double xdown,ydown,xdragdown,ydragdown,xdragdown_old,ydragdown_old;
    double minx_down,maxx_down,miny_down,maxy_down;

    double midx,midy,midz;
    double ang,ang_old,rot[3],old_rot[3],cv[3],ov[3];
    double min_zoom,max_zoom;

    double xaxis_min,xaxis_max,yaxis_min,yaxis_max,zaxis_min,zaxis_max;
    
    int hist_select;
    
    int got_formula;
    double plot_m,plot_c,plot_r2,plot_r2a;
    
    void stat_functions();
    
    void setzoom();
    void draw_axes();
    void draw_grid();
    void draw_points();
    void drawcuboid(double,double,double,double,double,double);
    void drawcuboidedge(double,double,double,double,double,double,double);
    void drawcross(double, double, double, double,double,double);
    void draw_hist();
    
    
    container* root;

public:
    
    int first_draw;
    double z_gap;
    int flr_choice;
    int all_flrs;
    int selector;
    int selected;
        double ang_x,ang_y,ang_z;
    int num_bins;
    
    std::vector<std::vector<double> >hist_x;
    std::vector<std::vector<double> > hist_n;
    std::vector<std::vector<double> > hist_nc;
    std::vector<std::vector<double> > bin_min,bin_max,bin_sum;
    std::vector<double> hist_gap,hist_min,hist_max;
    std::vector<int> sum_hist;
    std::vector<std::vector<double> > poly_hist_x;
    std::vector<std::vector<double> > poly_hist_y;
    
    double h_mean[2];
    double h_std1[2];
    double h_std2[2];
    
    void histogram(std::vector<double>*);
    
    MyPlot(int X, int Y, int W, int H, const char *L,container* root_): Fl_Gl_Window(X, Y, W, H, L),root(root_) {
        first_draw=1;
        zoom=0;
        drag=0;
        zoom_mag=0;
        zoom_level=1.0;
        ang_x=0;ang_y=0;ang_z=0;
        
        
        min_zoom=0.2;
        max_zoom=60;
        num_bins=20;

        
        
        for (int i=0;i<4;i++){
            std::vector<double> temp;

            poly_hist_y.push_back(temp);
            poly_hist_x.push_back(temp);
            bin_min.push_back(temp);
            bin_max.push_back(temp);
            bin_sum.push_back(temp);
            hist_n.push_back(temp);
            hist_nc.push_back(temp);
            hist_x.push_back(temp);
            sum_hist.push_back(0);
            hist_gap.push_back(0);
            hist_min.push_back(0);
            hist_max.push_back(0);
        }
        
    
        hist_select=-1;
    }
    
   
};


#endif /*PLOT_H*/