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

#ifndef LOCATIONS_H
#define LOCATIONS_H

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


#include "globals.h"


#include <stdlib.h>


#undef min
#undef max

struct text_output{
    Fl_Text_Buffer* buff;
    Fl_Text_Display* display;
};


struct update{
    text_output* t;
    std::string s;
};

void push_output(text_output*,std::string);
void clear_output(text_output*);
void push_time(text_output*);


struct triangle{
    friend class boost::serialization::access;
    double x1,x2,x3,y1,y2,y3,z1,z2,z3; //three points
    
    
    template<class Archive>
    void serialize(Archive & ar, const unsigned int /*version*/)
    {
        ar & x1 & x2 & x3 & y1 & y2 & y3 & z1 & z2 & z3;
    }
    triangle(){};
};



struct vec{
    friend class boost::serialization::access;
    double x;
    double y;
    double z;
    int floor; //include here?
    
    template<class Archive>
    void serialize(Archive & ar, const unsigned int /*version*/)
    {// note, version is always the latest when saving
        ar &  x;ar &  y; ar & z; ar & floor;
        
    }
    
    vec(){};
    
};




struct vertex{
    friend class boost::serialization::access;
    int start;
    int end;
    
    
    bool operator<( const vertex & n ) const {
        
        if ((this->start<n.start)||(this->start>n.start)){
            return this->start < n.start;   // for example
        }
        else{
            return this->end < n.end;
        }
    }
    
    
    template<class Archive>
    void serialize(Archive & ar, const unsigned int /*version*/)
    {// note, version is always the latest when saving
        ar &  start;ar &  end;
        
    }
   
    vertex(){};
};



struct location{
    friend class boost::serialization::access;
    vec loc;
    std::string label;
    int point;
    int segment;
    std::vector<int> vis_sitting;
    std::vector<int> vis_standing;
    std::vector<int> vis_standing_trans;
    std::vector<int> vis_sitting_trans;
    
    std::vector<std::vector<int> > quick_vis;
    
    
    int is_desk;
    
    
    template<class Archive>
    void serialize(Archive & ar, const unsigned int){
        
        ar &  loc;
        ar &  label;
        ar &  point;
        ar &  segment;
        ar &  vis_sitting;
        ar &  vis_standing;
     ar &  quick_vis;
    }
    

    location(){is_desk=0;};
    
};


struct wall{
    friend class boost::serialization::access;
    vec start;
    vec end;
    int transparent;
    
    
    template<class Archive>
    void serialize(Archive & ar, const unsigned int /*version*/)
    {
        // note, version is always the latest when saving
        ar &  start;
        ar &  end;
        ar & transparent;
        
        
    }
   
    
    
    
        wall(){};
};



struct line_in{
    friend class boost::serialization::access;
    vec start;
    vec end;
    int layer;
    
    
    template<class Archive>
    void serialize(Archive & ar, const unsigned int /*version*/)
    {   
        ar &  start &  end & layer;
   
    }
    
  
};


struct arc{
    friend class boost::serialization::access;
    double x,y,z,r,ang1,ang2;
    int layer;
    
    
    template<class Archive>
    void serialize(Archive & ar, const unsigned int /*version*/)
    {
        ar &  x & y & z & r & ang1 & ang2 & layer;
        
    }

   
    
};


struct circle{
    friend class boost::serialization::access;
    double x,y,z,r;
    int layer;
    
    
    template<class Archive>
    void serialize(Archive & ar, const unsigned int /*version*/)
    {
        ar &  x & y & z & r & layer;
        
    }
    
   
};

struct block_entry{
    
    friend class boost::serialization::access;
    std::string name;
    double scale_x;
    double scale_y;
    double scale_z;
    double insert_x;
    double insert_y;
    double insert_z;
    double rotation;
    int list_no;
    int layer;
    
    
    template<class Archive>
    void serialize(Archive & ar, const unsigned int /*version*/)
    {
     ar &  name & scale_x & scale_y & scale_z & insert_x & insert_y & insert_z & rotation & list_no & layer;
        
    }
 
    
};

struct block_definition{
    friend class boost::serialization::access;
    std::string name;
    std::vector<line_in> lines;
    std::vector<arc> arcs;
    std::vector<circle> circles;
    double start_x,start_y,start_z;
    std::vector<block_entry> nested_blocks;
    
    template<class Archive>
    void serialize(Archive & ar, const unsigned int /*version*/)
    {
        ar &  name & lines & arcs & circles & start_x & start_y & start_z & nested_blocks;
        
    }
    
    
    
};

struct plan{
    friend class boost::serialization::access;
    std::vector<line_in> lines_original;
    std::vector<line_in> lines;
    double shift_x,shift_y,shift_z;
    int unit_type,user_unit_type;
    std::vector<int> layers;
    std::vector<std::string> layers_name;
    std::vector<int> layers_used;

    
    
    std::vector<arc> arcs_original;
    std::vector<arc> arcs;
    
    std::vector<circle> circles_original;
    std::vector<circle> circles;
    
    //nested
    
    std::vector<block_entry> blocks;
    std::vector<block_entry> blocks_original;
    std::vector<block_definition> block_definitions;
    std::vector<block_definition> block_definitions_original;
    
    //end nested
    
    
    template<class Archive>
    void serialize(Archive & ar, const unsigned int )
    {
     ar & shift_x & shift_y & unit_type & lines & circles & arcs & blocks & block_definitions & layers & layers_name & layers_used; //forgot circles! ARGH!
        
    }
  
    
    
    plan();
    
    void scale();
    void scale(double);
    
    
};



#endif //LOCATIONS_H