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

#include "minheap.h"


MinHeap::MinHeap(const std::vector<weight_index>& vector) : _vector(vector)
{
    
    for (int i=0;i<vector.size();i++){
        index_back.push_back(i);
    }
    
    Heapify();
}

MinHeap::MinHeap()
{
}

void MinHeap::Heapify()
{
    int length = _vector.size();
    for(int i=length-1; i>=0; --i)
    {
        BubbleDown(i);
    }
}

void MinHeap::BubbleDown(int index)
{
    int length = _vector.size();
    int leftChildIndex = 2*index + 1;
    int rightChildIndex = 2*index + 2;
    
    if(leftChildIndex >= length)
        return; //index is a leaf
    
    int minIndex = index;
    
    
    if (_vector[index].weight_sort > _vector[leftChildIndex].weight_sort)
    {
        minIndex = leftChildIndex;
    }
    
    if ((rightChildIndex < length) && (_vector[minIndex].weight_sort > _vector[rightChildIndex].weight_sort))
    {
        minIndex = rightChildIndex;
    }
    
    
    
    
    
    if(minIndex != index)
    {
        //need to swap
        weight_index temp = _vector[index];
        _vector[index] = _vector[minIndex];
        _vector[minIndex] = temp;
        
        index_back[_vector[minIndex].index]=minIndex;
        index_back[_vector[index].index]=index;
        
        BubbleDown(minIndex);
    }
}

void MinHeap::BubbleUp(int index)
{
    if(index == 0)
        return;
    
    int parentIndex = (index-1)/2;
    
    if(_vector[parentIndex].weight_sort > _vector[index].weight_sort)
    {
        weight_index temp = _vector[parentIndex];
        _vector[parentIndex] = _vector[index];
        _vector[index] = temp;
        
        index_back[_vector[parentIndex].index]=parentIndex;
        index_back[_vector[index].index]=index;
        
        BubbleUp(parentIndex);


    }
}

void MinHeap::Insert(weight_index newValue)
{

    
    int length = _vector.size();
    _vector.push_back(newValue);
    
    BubbleUp(length);
}

int MinHeap::GetMin()
{
    return _vector[0].index;
}

void MinHeap::DeleteMin()
{
    int length = _vector.size();
    
    if(length == 0)
    {
        return;
    }
    
    _vector[0] = _vector[length-1];
    _vector.pop_back();
    
    BubbleDown(0);
}