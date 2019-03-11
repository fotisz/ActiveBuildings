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

#ifndef BUTTON_EX_H
#define BUTTON_EX_H

#include <FL/Fl_Button.H>

class button_ex : public Fl_Button{
    
    container* root;
    int handle (int e){
        
        
        
        if (e==8){
            if (Fl::event_key(FL_Enter)){
                
                this->do_callback(this,root);
               
                return 1;
                
            }
        }
        
        return Fl_Button::handle(e);
    }
    
public:
    button_ex(int X, int Y, int W, int H, const char * L,container* root_):Fl_Button(X,Y,W, H, L),root(root_)
    {}
};


#endif /*BUTTON_EX_H*/