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

#ifndef GRAPH_NEW_H
#define GRAPH_NEW_H

#define EPSILON 1e-6
#define ANG_EPSILON 45

#include <math.h>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <vector>
#include <map>
#include <ctime>

//FLTK
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
#include <FL/Fl_Menu_Bar.H>

#include <FL/Fl_Native_File_Chooser.H>
#include <vector>
#include <FL/Fl_Chart.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Progress.H>

#include <boost/serialization/map.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>

#include "locations.h"


#undef min
#undef max

void update_progress(Fl_Progress*,double,int);


struct graph{
    
    friend class boost::serialization::access;
    
    std::vector<std::vector<int> > neighbours;
    std::vector<std::map<int,double> > weights;
    
    void clear(){
      
      
        neighbours.clear();
        weights.clear();
        
        std::vector<std::vector<int> >().swap(neighbours);
        std::vector<std::map<int,double> >().swap(weights);
        
    }
    
    template<class Archive>
    void serialize(Archive & ar, const unsigned int ){
        ar & neighbours & weights;
    }
};

struct route_weight_number{
    double weight;
    int number;
};

struct graph_routes{
    
    int exist;
    
    friend class boost::serialization::access;
    std::vector<double> weight;
    std::vector<std::vector <int> > path;
    std::vector<int> mult_path;
    std::vector<int> end_node;
    int origin;
    int node_origin;
    int dual;
    
    template<class Archive>
    void serialize(Archive & ar, const unsigned int /*version*/)
    {// note, version is always the latest when saving
        ar &  weight;ar &  path;ar &  mult_path;ar &  end_node; ar &  origin; ar & node_origin; ar & dual;
        
    }
    /*template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {//if(version > 0) ar & driver_name;
               ar &  weight;ar &  path;ar &  mult_path;ar &  end_node; ar &  origin; ar & node_origin; ar & dual;
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()*/
        graph_routes(){};
};


//BOOST_CLASS_VERSION(graph_routes, 1)

struct return_iterate{
    std::vector<std::vector<int> > paths;
    int count;
    int error;
};


struct map_structures{
  
    friend class boost::serialization::access;
    
    std::vector<graph_routes> angular_array,turns_array,distance_array,segments_array; //graphs based on points in segemnt map
    std::vector<graph_routes> seg_angular_array,seg_turns_array,seg_distance_array,seg_segments_array;  //graphs based on segments within segment map
    std::vector<graph_routes> seg_distance_3d_array;
    std::vector<vertex> dual_points, links;
    int num_segments,num_points,num_links;
    
    std::vector<std::vector<int> > points2dual_1;
    
    std::map<vertex,int> points2dual;
    std::map<vertex,int> points_2_segs;
    
    std::vector<graph_routes> crow_array,crow_3d_array,distance_3d_array;
    
    std::vector<graph_routes> seg_crow_array,seg_crow_3d_array;
    
    std::vector<int> line_segments;
    
    graph G,G_dist,G_dist_3D,S,S_dist,S_dist_3D,G_dual_ang,G_dual_turn,S_dual_ang,S_dual_turn;
    
    
    std::vector<std::vector<route_weight_number> > choice_cache_G_dist;
    std::vector<std::vector<route_weight_number> > choice_cache_G_dist_3D;
    std::vector<std::vector<route_weight_number> > choice_cache_G;
    std::vector<std::vector<route_weight_number> > choice_cache_G_ang;
    std::vector<std::vector<route_weight_number> > choice_cache_G_turn;
    std::vector<std::vector<route_weight_number> > choice_cache_S_dist;
    std::vector<std::vector<route_weight_number> > choice_cache_S_dist_3D;
    std::vector<std::vector<route_weight_number> > choice_cache_S;
    std::vector<std::vector<route_weight_number> > choice_cache_S_ang;
    std::vector<std::vector<route_weight_number> > choice_cache_S_turn;
    
    
    
    template<class Archive>
    void serialize(Archive & ar, const unsigned int )
    {// note, version is always the latest when saving
        
     ar & dual_points;ar & links;
     
     ar & num_segments; ar & num_points; ar & num_links;
     
     ar & points2dual & points_2_segs;
     
     ar & G_dual_ang & G_dual_turn & S_dual_ang & S_dual_turn & S & S_dist & S_dist_3D & G & G_dist & G_dist_3D;
     
     ar & line_segments;
    }
   
   /*
     template<class Archive>
     void save(Archive & ar, const unsigned int ) const
     {
    
     
     ar & dual_points;ar & links;
     
     ar & num_segments; ar & num_points; ar & num_links;
     
     ar & points2dual & points_2_segs;
     
     ar & G_dual_ang & G_dual_turn & S_dual_ang & S_dual_turn & S & S_dist & S_dist_3D & G & G_dist & G_dist_3D;

     ar & line_segments;
     
     }
     template<class Archive>
     void load(Archive & ar, const unsigned int )
     {
     ar & angular_array;ar & turns_array;ar & distance_array;ar & segments_array;
     
     ar & seg_angular_array;ar & seg_turns_array;ar & seg_distance_array;ar & seg_segments_array;
     
     ar & dual_points;ar & links;
     
     ar & num_segments; ar & num_points; ar & num_links;
     
     ar & points2dual_1; ar & crow_array; ar & crow_3d_array;ar & distance_3d_array;
     
     ar & G_crow_1; ar & G_crow_3D_1;
     
     ar & G_dual_ang_1 & G_dual_turn_1 & S_dual_ang_1 & S_dual_turn_1 & S_1 & S_dist_1 & S_dist_3D_1 & G_1 & G_dist_1 & G_dist_3D_1;
     
     ar & S_crow_1;
     ar & S_crow_3D_1;
     ar & line_segments;
     }
     BOOST_SERIALIZATION_SPLIT_MEMBER()
     */
    
        map_structures(){};
    void clear();
    
};

//BOOST_CLASS_VERSION(map_structures, 1)

struct total{
    Fl_Progress* progress;
    map_structures* map;
    text_output* tex;
    std::vector<vec> points;
    std::vector<std::vector<int> > matrix;
    std::vector<std::vector<int> >* points_2_segs;
    int* flag;
    int* flag2;
    Fl_Box* warning;
};


void iterate_path_reference(return_iterate*, const std::vector< std::vector<int> > &,const std::vector< int>& , const int &, const int &,const int &, int ,const  int&,int,int);


void dijkstra_reference_list_heap(graph_routes*, graph& ,int,int , int,std::vector<double>&,std::vector<int>&,int,map_structures*);


void dijkstra_reference_list_heap_raw(graph_routes*, graph& ,int,int, int,std::vector<double>&,std::vector<int>&,int,map_structures*);
void populate_raw(std::vector<std::vector<route_weight_number> >*,graph& ,int, int ,map_structures*, int,int&,int&);


int readin_desks(std::vector<vec> &,std::vector<location>&,std::string,text_output* );
int readin_points(std::vector<vec> &,std::string ,text_output*);
int readin_matrix(std::vector<vec> &,std::vector<vertex> &,std::string ,text_output*);
int readin_adj_list(std::vector<vec> &,std::vector<vertex> &,std::string ,text_output*);


void set_cache_arrays(map_structures*);
void* calculate_distances_paths(void*);
void* calculate_lines(void*);


bool equal(double a, double b);


std::istream& tool_getline(std::istream& is, std::string& t);

void awake_map_warning_off(void*);
void awake_vis_warning_off(void*);
void awake_map_warning_on(void*);
void awake_vis_warning_on(void*);

#endif /*GRAPH_NEW_H*/
