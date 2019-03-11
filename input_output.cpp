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

#include "input_output.h"

int save_binary(const file_in_out* object){
    
    building obj = (*object).content;
    
    std::ofstream ofs((*object).file.c_str());
    
    //boost::archive::binary_oarchive oa(ofs);
    boost::archive::text_oarchive oa(ofs); //opt for text archive as although possible to meddle with and slightly larger, will work across all architectures
    oa & obj;

    
    
    /*assert(ofs.good());
    boost::archive::xml_oarchive oa(ofs);
    oa << BOOST_SERIALIZATION_NVP(obj);
    */
    
    
    
    std::cout<<"doing things here"<<std::endl;
    return 0;
}

int read_binary(file_in_out* object){
    
 

    
    std::ifstream ifs((*object).file.c_str());
    
    
    try
    {

        //boost::archive::binary_iarchive ia(ifs);
        boost::archive::text_iarchive ia(ifs);
        // read class state from archive
        
        ia >> (*object).content;//(*obj);
        
    }
    catch (boost::archive::archive_exception& ex) {
        push_time(text);
        push_output(text,"File not read! Incorrect or outdated file type.\n");
        return 1;
    }
    catch (...)
    {
        push_time(text);
        push_output(text,"File not read! Incorrect or outdated file type.\n");
        std::cout << "An exception occurred. Exception Nr. " << '\n';
        return 1;
    }
    
    

    
    return 0;
    
}

