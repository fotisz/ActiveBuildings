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
#ifndef MINHEAP_H
#define MINHEAP_H

#include <vector>;


struct weight_index{

    double weight;
    double weight_sort;
    int index;
    
    
};


class MinHeap
{
private:
    
    
    
    void Heapify();
    
public:
    
    std::vector<weight_index> _vector;
    std::vector<int> index_back;
    
  
    MinHeap(const std::vector<weight_index>& vector);
    MinHeap();
    
    void Insert(weight_index newValue);
    int GetMin();
    void DeleteMin();
    
    void BubbleDown(int index);
    void BubbleUp(int index);
    
};


#endif /*MINHEAP_H*/