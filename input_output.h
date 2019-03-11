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

#ifndef INPUT_OUTPUT_H
#define INPUT_OUPUT_H

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


#include <math.h>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <iomanip>


#include <stdlib.h>

#include "building.h"


#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>


#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>


#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>



//new



#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>



#undef min
#undef max

int save_binary(const file_in_out*);
int read_binary(file_in_out*);




#endif //INPUT_OUTPUT_H