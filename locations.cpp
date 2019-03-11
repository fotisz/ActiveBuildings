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

#include "locations.h"

#undef min
#undef max

void push_output(text_output* tex,std::string str){
    
    Fl::lock();
    tex->buff->append(str.c_str());
    tex->display->buffer(tex->buff);
    tex->display->redraw();
    tex->display->insert_position(tex->buff->line_start(tex->buff->length()));//moves insert position to the character corresponding to the start of the line that the last character is on
    tex->display->show_insert_position(); //scrolls to that point
    
    Fl::unlock();
    Fl::awake();
    
}

void clear_output(text_output* tex){
    
    Fl::lock();
    
    tex->buff->remove(0,tex->buff->length());
    tex->display->buffer(tex->buff);
    tex->display->redraw();
    Fl::awake();
    Fl::unlock();
    
    
}

void push_time(text_output* tex){
    time_t t = time(NULL);
    tm* tt=localtime(&t);
    std::stringstream s;
    s<< std::setfill('0')<<std::setw(2)<<tt->tm_mday<<"/"<<std::setw(2)<<tt->tm_mon+1<<"/"<< std::setw(4)<<tt->tm_year+1900<<" "<< std::setw(2)<<tt->tm_hour<<":"<<std::setw(2)<<tt->tm_min<<":"<<std::setw(2)<<tt->tm_sec<<" >> ";
    push_output(tex,s.str());
}

plan::plan(){
    
    unit_type=0;
    user_unit_type=0;
    
    shift_x=0;
    shift_y=0;
    shift_z=0;
}


void plan::scale(double scale){
    
    for (int i=0;i<lines.size();i++){
        
        lines[i].start.x=scale*lines[i].start.x;// +shift_x;
        lines[i].end.x=scale*lines[i].end.x;//+shift_x;
        lines[i].start.y=scale*lines[i].start.y;//+shift_y;
        lines[i].end.y=scale*lines[i].end.y;//+shift_y;
        lines[i].start.z=scale*lines[i].start.z;//+shift_z;
        lines[i].end.z=scale*lines[i].end.z;//+shift_z;
    }
    
    
    for (int i=0;i<arcs.size();i++){
        
        arcs[i].x=scale*arcs[i].x;//+shift_x;
        arcs[i].y=scale*arcs[i].y;//+shift_y;
        arcs[i].z=scale*arcs[i].z;//+shift_z;
        arcs[i].r=scale*arcs[i].r;
        
    }
    
    for (int i=0;i<circles.size();i++){
        
        circles[i].x=scale*circles[i].x;//+shift_x;
        circles[i].y=scale*circles[i].y;//+shift_y;
        circles[i].z=scale*circles[i].z;//+shift_z;
        circles[i].r=scale*circles[i].r;
        
    }
    
    for (int i=0;i<block_definitions.size();i++){
        
        for (int j=0;j<block_definitions[i].lines.size();j++){
            
            block_definitions[i].lines[j].start.x=scale*block_definitions[i].lines[j].start.x;
            block_definitions[i].lines[j].end.x=scale*block_definitions[i].lines[j].end.x;
            block_definitions[i].lines[j].start.y=scale*block_definitions[i].lines[j].start.y;
            block_definitions[i].lines[j].end.y=scale*block_definitions[i].lines[j].end.y;
            block_definitions[i].lines[j].start.z=scale*block_definitions[i].lines[j].start.z;
            block_definitions[i].lines[j].end.z=scale*block_definitions[i].lines[j].end.z;
            
        }
        
        for (int j=0;j<block_definitions[i].arcs.size();j++){
            block_definitions[i].arcs[j].x=scale*block_definitions[i].arcs[j].x;
            block_definitions[i].arcs[j].y=scale*block_definitions[i].arcs[j].y;
            block_definitions[i].arcs[j].z=scale*block_definitions[i].arcs[j].z;
            block_definitions[i].arcs[j].r=scale*block_definitions[i].arcs[j].r;
            
        }
        
        for (int j=0;j<block_definitions[i].circles.size();j++){
            block_definitions[i].circles[j].x=scale*block_definitions[i].circles[j].x;
            block_definitions[i].circles[j].y=scale*block_definitions[i].circles[j].y;
            block_definitions[i].circles[j].z=scale*block_definitions[i].circles[j].z;
            block_definitions[i].circles[j].r=scale*block_definitions[i].circles[j].r;
            
        }
        
        for (int j=0;j<block_definitions[i].nested_blocks.size();j++){
            
            block_definitions[i].nested_blocks[j].insert_x=scale*block_definitions[i].nested_blocks[j].insert_x;
            block_definitions[i].nested_blocks[j].insert_y=scale*block_definitions[i].nested_blocks[j].insert_y;
            block_definitions[i].nested_blocks[j].insert_z=scale*block_definitions[i].nested_blocks[j].insert_z;
        }
        
    }
    
    for (int i=0;i<blocks.size();i++){
        blocks[i].insert_x=scale*blocks[i].insert_x;
        blocks[i].insert_y=scale*blocks[i].insert_y;
        blocks[i].insert_z=scale*blocks[i].insert_z;
    }
    
}

void plan::scale(){
    
    double scale;
    
    // 0 = unitless
    // 1 = inches
    // 2 = feet
    // 3 = miles
    // 4 = mm
    // 5 = cm
    // 6 = m
    // 7 = km
    // 8 = microinches
    // 9 = Mils
    // 10 = yards
    // 11 = angstroms
    // 12 = nanonmeters
    // 13 = microns
    // 14 = decimeters
    // 15 = decameters
    // 16 = hectometers
    // 17 = gigameters
    // 18 = AU
    // 19 = light years
    // 20 = parsecs
    
    switch (unit_type){
            
        case 0:
            scale =1;
            push_time(text);
            push_output(text,"WARNING! Floorplan provided without units! Units interpreted as metres.\n");
            push_time(text);
            push_output(text,"Set units in dxf file or scale floorplan in 'floorplan' tab of the main view tab. \n");
            break;
        case 1:
            scale=0.0254; break;
        case 2:
            scale=0.3048;break;
        case 3:
            scale=1609.344;break;
        case 4:
            scale=0.001;break;
        case 5:
            scale=0.01;break;
        case 6:
            scale=1.0;break;
        case 7:
            scale=1000.0;break;
        case 8:
            scale=2.5400e-8;break;
        case 9:
            scale=2.5400e-5;break;
        case 10:
            scale=0.9144;break;
        case 11:
            scale=1.0e-10;break;
        case 12:
            scale=1.0e-9;break;
        case 13:
            scale=1.0e-6;break;
        case 14:
            scale=0.1;break;
        case 15:
            scale=10.0;break;
        case 16:
            scale=100.0;break;
        case 17:
            scale=1.0e9;break;
        case 18:
            scale=149597870700.0;break;
        case 19:
            scale=9.4605284e15;break;
        case 20:
            scale=3.08567758e16;break; //getting a bit silly now...
        default:
            scale =1;
            push_time(text);
            push_output(text,"WARNING! Floorplan provided without units!\n");
            push_time(text);
            push_output(text,"Set units in source or adjust in 'floorplan' tab of the main 'floorplan' view tab\n");
            break;
            
    }
    
    std::cout<<"SCALING "<<scale<<std::endl;
    
    for (int i=0;i<lines.size();i++){
        
        lines[i].start.x=scale*lines_original[i].start.x;// +shift_x;
        lines[i].end.x=scale*lines_original[i].end.x;//+shift_x;
        lines[i].start.y=scale*lines_original[i].start.y;//+shift_y;
        lines[i].end.y=scale*lines_original[i].end.y;//+shift_y;
        lines[i].start.z=scale*lines_original[i].start.z;//+shift_z;
        lines[i].end.z=scale*lines_original[i].end.z;//+shift_z;
    }
    
    
    for (int i=0;i<arcs.size();i++){
        
        arcs[i].x=scale*arcs_original[i].x;//+shift_x;
        arcs[i].y=scale*arcs_original[i].y;//+shift_y;
        arcs[i].z=scale*arcs_original[i].z;//+shift_z;
        arcs[i].r=scale*arcs_original[i].r;
        
    }
    
    for (int i=0;i<circles.size();i++){
        
        circles[i].x=scale*circles_original[i].x;//+shift_x;
        circles[i].y=scale*circles_original[i].y;//+shift_y;
        circles[i].z=scale*circles_original[i].z;//+shift_z;
        circles[i].r=scale*circles_original[i].r;
        
    }
    
   
    
    for (int i=0;i<block_definitions.size();i++){
        
        for (int j=0;j<block_definitions[i].lines.size();j++){
            
            block_definitions[i].lines[j].start.x=scale*block_definitions_original[i].lines[j].start.x;
            block_definitions[i].lines[j].end.x=scale*block_definitions_original[i].lines[j].end.x;
            block_definitions[i].lines[j].start.y=scale*block_definitions_original[i].lines[j].start.y;
            block_definitions[i].lines[j].end.y=scale*block_definitions_original[i].lines[j].end.y;
            block_definitions[i].lines[j].start.z=scale*block_definitions_original[i].lines[j].start.z;
            block_definitions[i].lines[j].end.z=scale*block_definitions_original[i].lines[j].end.z;
            
        }
        
        for (int j=0;j<block_definitions[i].arcs.size();j++){
            block_definitions[i].arcs[j].x=scale*block_definitions_original[i].arcs[j].x;
            block_definitions[i].arcs[j].y=scale*block_definitions_original[i].arcs[j].y;
            block_definitions[i].arcs[j].z=scale*block_definitions_original[i].arcs[j].z;
            block_definitions[i].arcs[j].r=scale*block_definitions_original[i].arcs[j].r;
            
        }
        
        for (int j=0;j<block_definitions[i].circles.size();j++){
            block_definitions[i].circles[j].x=scale*block_definitions_original[i].circles[j].x;
            block_definitions[i].circles[j].y=scale*block_definitions_original[i].circles[j].y;
            block_definitions[i].circles[j].z=scale*block_definitions_original[i].circles[j].z;
            block_definitions[i].circles[j].r=scale*block_definitions_original[i].circles[j].r;
            
        }
        
        for (int j=0;j<block_definitions[i].nested_blocks.size();j++){
            
            block_definitions[i].nested_blocks[j].insert_x=scale*block_definitions_original[i].nested_blocks[j].insert_x;
            block_definitions[i].nested_blocks[j].insert_y=scale*block_definitions_original[i].nested_blocks[j].insert_y;
            block_definitions[i].nested_blocks[j].insert_z=scale*block_definitions_original[i].nested_blocks[j].insert_z;
        }
        
    }
    
    for (int i=0;i<blocks.size();i++){
        blocks[i].insert_x=scale*blocks_original[i].insert_x;
        blocks[i].insert_y=scale*blocks_original[i].insert_y;
        blocks[i].insert_z=scale*blocks_original[i].insert_z;
    }
    
    
    //end nested
    
}