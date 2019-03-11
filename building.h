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

#ifndef BUILDING_H
#define BUILDING_H


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

#include "locations.h"
#include "graph_new.h"

#include <stdlib.h>

#undef min
#undef max


struct metric_description{
    
    int destination_crit,route,metric,visible,floor,stat,destination,range_option,basis,route_break,self_count,iterations,inverse,error;
    double eig_tolerance;
    double metric_range;
    double metric_turn;
};

struct building{
    
    friend class boost::serialization::access;
    
    std::vector<plan> floor_plans;
    plan floor_plan;
    std::vector<vec> points;
    std::vector<std::vector<int> > matrix; //remove
    map_structures map;
    
    std::vector<vertex> adj_list;
    
   
    std::vector<double> custom_metric;
    
    
    std::vector<std::vector<double> > metrics;
    std::vector<metric_description> metric_des;
    
    
    std::vector<double> floor_index;
    
    std::vector<location> desks;
    std::vector<location> kitchens;
    std::vector<location> male_toilets;
    std::vector<location> female_toilets;
    std::vector<location> printers;
    std::vector<location> meeting_rooms;
    std::vector<location> exits;
    std::vector<location> stairs;
    std::vector<location> lifts;
    
    std::vector<wall> walls_sitting,walls_standing;
    
   
     
    double mid_x,mid_y,mid_z,minimum_z,maximum_z,minimum_x,minimum_y,maximum_x,maximum_y;
    
    int vis_valid,map_valid,lines_valid;
    
    
    std::vector<std::vector<int> > points_2_segs; //SERIALIZE???
    
    std::vector<std::vector<vec> > floorarea_polygons; //SERIALIZE???
    std::vector<std::vector<triangle> > floorarea_triangles; //SERIALIZE????
    std::vector<double> floor_areas; //SERIALIZE????
    
    std::vector<std::vector<int> > route;
    int dual_route;
    
    std::string save_file_string;
    
    
    template<class Archive>
    void serialize(Archive & ar, const unsigned int)// version*)
    {// note, version is always the latest when saving
     
     
     
     
     ar &  floor_plans;
     ar &  points;
     ar &  adj_list;
     ar &  map;
     ar &  custom_metric;
     ar &  desks;
     ar &  kitchens;
     ar &  male_toilets;
     ar &  female_toilets;
     ar &  printers;
     ar &  meeting_rooms;
     ar &  exits;
     ar &  stairs;
     ar &  lifts;
     ar &  walls_sitting;
     ar &  walls_standing;
     ar & mid_x & mid_y & mid_z & minimum_x & minimum_y & minimum_z & maximum_x & maximum_y & maximum_z;
     ar & floor_index;
     ar & vis_valid & map_valid & lines_valid;
     ar & floorarea_polygons;
     ar & floorarea_triangles;
     ar & floor_areas;
     
        
   
    
    }
    

    /*
     template<class Archive>
     void save(Archive & ar, const unsigned int version) const
     {
     //if(version > 0) ar & driver_name;
         
         
     ar &  floor_plans;
     ar &  points;
     ar &  adj_list;
     ar &  map;
     ar &  custom_metric;
     ar &  desks;
     ar &  kitchens;
     ar &  male_toilets;
     ar &  female_toilets;
     ar &  printers;
     ar &  meeting_rooms;
     ar &  exits;
     ar &  stairs;
     ar &  lifts;
     ar &  walls_sitting;
     ar &  walls_standing;
     ar & mid_x & mid_y & mid_z & minimum_x & minimum_y & minimum_z & maximum_x & maximum_y & maximum_z;
     ar & floor_index;
     ar & vis_valid & map_valid & lines_valid;
     ar & floorarea_polygons;
     ar & floorarea_triangles;
     ar & floor_areas;
          
          
         
        
         
         
     }
     template<class Archive>
     void load(Archive & ar, const unsigned int version)
     {
     //if(version > 0) ar & driver_name;
     ar &  floor_plans;
     ar &  points;
     ar &  matrix;
     ar &  map;
     ar &  custom_metric;
     ar &  desks;
     ar &  kitchens;
     ar &  male_toilets;
     ar &  female_toilets;
     ar &  printers;
     ar &  meeting_rooms;
     ar &  exits;
     ar &  stairs;
     ar &  lifts;
     ar &  walls_sitting;
     ar &  walls_standing;
     ar & mid_x & mid_y & mid_z & minimum_x & minimum_y & minimum_z & maximum_x & maximum_y & maximum_z;
     ar & floor_index;
     ar & vis_valid & map_valid & lines_valid;
     ar & floorarea_polygons;
     ar & floorarea_triangles;
     ar & floor_areas;
     }
     BOOST_SERIALIZATION_SPLIT_MEMBER()
     */
    
    
    building(){
        vis_valid=0;
        map_valid=0;
        lines_valid=0;
        
        std::vector<vec> temp;
        plan temp1;
        
        floor_plans.push_back(temp1);
        floorarea_polygons.push_back(temp);
        
        floor_index.push_back(0);
        floor_areas.push_back(0);
        
        save_file_string = "projectname.proj";
    };
    
    void clear(){
        
        floor_plans.clear();
        std::vector<plan>().swap(floor_plans);
        points.clear();
        std::vector<vec>().swap(points);
        matrix.clear();
        std::vector<std::vector<int> >().swap(matrix);
        adj_list.clear();
        std::vector<vertex>().swap(adj_list);
        map.clear();
        custom_metric.clear();
        std::vector<double>().swap(custom_metric);
        metrics.clear();
        std::vector<std::vector<double> >().swap(metrics);
        metric_des.clear();
        std::vector<metric_description>().swap(metric_des);
        
        floor_index.clear();
        
        
        
        
        desks.clear();
        kitchens.clear();
        male_toilets.clear();
        female_toilets.clear();
        printers.clear();
        meeting_rooms.clear();
        exits.clear();
        stairs.clear();
        lifts.clear();
        
        
        std::vector<location>().swap(desks);
        std::vector<location>().swap(kitchens);
        std::vector<location>().swap(male_toilets);
        std::vector<location>().swap(female_toilets);
        std::vector<location>().swap(printers);
        std::vector<location>().swap(meeting_rooms);
        std::vector<location>().swap(exits);
        std::vector<location>().swap(stairs);
        std::vector<location>().swap(lifts);
        
        
        walls_sitting.clear();
        walls_standing.clear();
        
        std::vector<wall>().swap(walls_sitting);
        std::vector<wall>().swap(walls_standing);
        
        
        points_2_segs.clear(); //SERIALIZE???
        
        floorarea_polygons.clear(); //SERIALIZE???
        std::vector<std::vector<vec> >().swap(floorarea_polygons);
        floorarea_triangles.clear(); //SERIALIZE????
        std::vector<std::vector<triangle> >().swap(floorarea_triangles);
        floor_areas.clear(); //SERIALIZE????
        
        route.clear();
        
        
        vis_valid=1;
        map_valid=1;
        lines_valid=1;
        
        std::vector<vec> temp;
        plan temp1;
        
        floor_plans.push_back(temp1);
        floorarea_polygons.push_back(temp);
        
        floor_index.push_back(0);
        floor_areas.push_back(0);
        
    };
    
    
};
//BOOST_CLASS_VERSION(building, 1)

struct str_container{
    std::string str;
};



struct file_in_out{
    building content;
    std::string file;
};

void prepare_undo(void*);


#endif //BUILDING_H