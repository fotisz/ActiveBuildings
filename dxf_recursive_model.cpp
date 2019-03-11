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

#include "dxf_recursive_model.h"

#undef min
#undef max


dxf_parser::dxf_parser() {
    polyline_in=0;
    first_polyline=0;
    reading_block=0;
    layers.clear();
    layers_used.clear();
    
}

void dxf_parser::addArc(const DL_ArcData & data){
    
    std::cout<<"ARC"<<std::endl;
    printf("  Attributes: Layer: %s\n ", attributes.getLayer().c_str());
    
    
    
    arc temp;
    
    for (int i=0;i<layers.size();i++){
        if (attributes.getLayer().compare(layers[i])==0){
            
            temp.layer=i;
            layers_used[i]=1;
        }
    }
    
    
    temp.x=data.cx;
    temp.y=data.cy;
    temp.z=data.cz;
    temp.r=data.radius;
    temp.ang1=data.angle1;
    temp.ang2=data.angle2;
    
    if (!reading_block){
        
        arcs.push_back(temp);
    }
    else{
        
        
        
        int block = block_definitions.size();
        
        if (block){
            
            block_definitions[block-1].arcs.push_back(temp);
            
        }
        
    }
    polyline_in=0;
    
    
    
}

void dxf_parser::addCircle(const DL_CircleData & data){
        
    circle temp;
    
    for (int i=0;i<layers.size();i++){
        if (attributes.getLayer().compare(layers[i])==0){            
            temp.layer=i;
            layers_used[i]=1;
        }
    }
    
    
    temp.x=data.cx;
    temp.y=data.cy;
    temp.z=data.cz;
    temp.r=data.radius;

    
    if (!reading_block){
        
        circles.push_back(temp);
    }
    else{
        
        
        int block = block_definitions.size();
        
        if (block){
            
            block_definitions[block-1].circles.push_back(temp);
            
        }
        
    }
    polyline_in=0;
    
    
    
}


void dxf_parser::addInsert(const DL_InsertData & data){
    

    block_entry temp;
    
    for (int i=0;i<layers.size();i++){
        if (attributes.getLayer().compare(layers[i])==0){

            temp.layer=i;
            layers_used[i]=1;
        }
    }
    
    temp.name=data.name;
    temp.scale_x=data.sx;
    temp.scale_y=data.sy;
    temp.scale_z=data.sz;
    temp.insert_x=data.ipx;
    temp.insert_y=data.ipy;
    temp.insert_z=data.ipz;
    temp.rotation=data.angle;

    
    if (!reading_block){
     
        blocks.push_back(temp);
        
    }
    else{
        
        int block = block_definitions.size();
        
        block_definitions[block-1].nested_blocks.push_back(temp); //if reading block push inserted block onto nested blocks of the block definition
        
    }

}





void dxf_parser::addBlock(const DL_BlockData& data){
    
    reading_block=1;
    block_definition temp;
    temp.name=data.name;
    temp.start_x=data.bpx;
    temp.start_y=data.bpy;
    temp.start_z=data.bpz;
    block_definitions.push_back(temp);
    
}

void dxf_parser::endBlock(){
    reading_block=0;
}

void dxf_parser::setVariableInt(const char* key, int value, int code){
    
    
    std::stringstream s;
    std::string str1,str2;
    
    s<<key;
    str1=s.str();
    s.str("");
    s<<"$INSUNITS";
    str2=s.str();
    
    if (!strcmp(str1.c_str(),str2.c_str())){
        units=value;
    }
    
    
    s.str("");
    s<<"$DIMAUNIT";
    str2=s.str();
    
    
    
    s.str("");
    s<<"$AUNITS";
    str2=s.str();
        
    
}



void dxf_parser::addLine(const DL_LineData& data) {
   
    line_in temp;
    
    for (int i=0;i<layers.size();i++){
        if (attributes.getLayer().compare(layers[i])==0){   
            temp.layer=i;
            layers_used[i]=1;
        }
    }
    
    temp.start.x=data.x1;
    temp.end.x=data.x2;
    temp.start.y=data.y1;
    temp.end.y=data.y2;
    temp.start.z=data.z1;
    temp.end.z=data.z2;
    
    
    if (!reading_block){
    
        lines.push_back(temp);
    }
    else{
        
        
        int block = block_definitions.size(); 
        
        if (block){

            
            block_definitions[block-1].lines.push_back(temp);
            
        }
            
    }
    polyline_in=0;

}


void 	dxf_parser::addLayer (const DL_LayerData &data){

    layers.push_back(data.name);
    layers_used.push_back(0);
    polyline_in=0;
}

void dxf_parser::addPolyline(const DL_PolylineData& data) {
    
    polyline_in=1;
    polyline_num=data.number;
    polyline_count=0;
    first_polyline=0;
    

}



void dxf_parser::addVertex(const DL_VertexData& data) {
   
    
  
    
    if (polyline_in){
        
        if (first_polyline==0){
            
            last_vertex.x=data.x;
            last_vertex.y=data.y;
            last_vertex.z=data.z; 
            first_polyline=1;
        }
        else{
            line_in temp;
            temp.start.x=last_vertex.x;
            temp.end.x=data.x;
            temp.start.y=last_vertex.y;
            temp.end.y=data.y;
            temp.start.z=last_vertex.z;
            temp.end.z=data.z;
            
            for (int i=0;i<layers.size();i++){
                if (attributes.getLayer().compare(layers[i])==0){

                    temp.layer=i;
                    layers_used[i]=1;
                }
            }
            
            if (!reading_block){
                lines.push_back(temp);
            }
            else{
                int block = block_definitions.size();
                
                if (block){

                    
                    block_definitions[block-1].lines.push_back(temp);
                    
                }
                
            }
            
            last_vertex.x=data.x;
            last_vertex.y=data.y;
            last_vertex.z=data.z;
            
        }
        
    }    
    
}






//void dxf_parser::printAttributes() {
    
//}

///READ IN FUNCTION
void testReading(char* file) {
    
    
    dxf_parser* creationClass = new dxf_parser();
    DL_Dxf* dxf = new DL_Dxf();
    if (!dxf->in(file, creationClass)) { // if file open failed
        std::cerr << file << " could not be opened.\n";
        return;
    }
    
    
    delete dxf;
    delete creationClass;
}

