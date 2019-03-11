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

#ifndef DXF_RECURSIVE_MODEL_H
#define DXF_RECURSIVE_MODEL_H

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

#include "locations.h"

#include <DXFLIB/dl_dxf.h>
#include <DXFLIB/dl_creationadapter.h>


#undef min
#undef max



class dxf_parser : public DL_CreationAdapter {
    
    int polyline_in,first_polyline,polyline_num,polyline_count;
    int reading_block;

    
public:
    
    int units;
    std::vector<line_in> lines;
    std::vector<arc> arcs;
    std::vector<circle> circles;
    vec last_vertex;
    std::vector<block_definition> block_definitions;
    std::vector<block_entry> blocks;
    std::vector<std::string> layers;
    std::vector<int> layers_used;
    dxf_parser();
    
   
    virtual void addLine(const DL_LineData& data);
   
    virtual void addPolyline(const DL_PolylineData& data);
    virtual void addVertex(const DL_VertexData& data);
    virtual void addLayer (const DL_LayerData& data);
   
    virtual void setVariableInt(const char* key, int value, int code);
    virtual void addBlock (const DL_BlockData& data);
    virtual void endBlock();
    virtual void addInsert(const DL_InsertData& data);
    virtual void addArc(const DL_ArcData & data);
    virtual void addCircle(const DL_CircleData & data);
   

    //void printAttributes();
};


#endif /*DXF_RECURSIVE_MODEL_H*/