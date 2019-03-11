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

#include <math.h>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <vector>

#include <ctime>
#include "graph_new.h"
#include "container.h"
#include "minheap.h"

#include <FL/Fl_Progress.H>

#undef min
#undef max

void awake_map_warning_off(void* data){
   if (warning_map) warning_map->hide();
}

void awake_vis_warning_off(void* data){
    warning_visibility->hide();
}

void awake_map_warning_on(void* data){
    warning_map->show();
}

void awake_vis_warning_on(void* data){
    warning_visibility->show();
}

std::istream& tool_getline(std::istream& is, std::string& t)
{
    t.clear();
    
    // The characters in the stream are read one-by-one using a std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.
    
    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();
    
    for(;;) {
        int c = sb->sbumpc();
        switch (c) {
            case '}':
                return is;
            case ']':
                return is;
            case ')':
                return is;
            case '\n':
                return is;
            case '\r':
                if(sb->sgetc() == '\n')
                    sb->sbumpc();
                return is;
            case EOF:
                // Also handle the case when the last line has no line ending
                if(t.empty())
                    is.setstate(std::ios::eofbit);
                return is;
            default:
                t += (char)c;
        }
    }
}

void update_progress(Fl_Progress* progress,double x,int i){
    int c1[3];
    int c2[3];
    
    switch(i){
        case 0: //color bar schemes
            c1[0]=255;c1[1]= 191;c1[2]= 8;
            c2[0]=255;c2[1]=21;c2[2]=93;
            
            c2[0]=255;c2[1]=0;c2[2]=0;
            
            break;
        case 1:
            c2[0]=255;c2[1]= 191;c2[2]= 8;
            c1[0]=255;c1[1]=21;c1[2]=93;
            break;
            
            
    }
    int d[3]={c2[0]-c1[0],c2[1]-c1[1],c2[2]-c1[2]};
    int f[3]={c1[0]+(int)((double)d[0]*x),c1[1]+(int)((double)d[1]*x),c1[2]+(int)((double)d[2]*x)};
    
    if (x==1){
        f[0]=0;f[1]=255;f[2]=0;
        if (i==2){
            f[0]=255;f[1]=0;f[2]=0;
        }
    }
    
    Fl::lock();
    progress->value(x);
    
    progress->selection_color(fl_rgb_color(f[0],f[1],f[2]));
    
    Fl::unlock();
    Fl::awake();
}


void map_structures::clear(){
    
    
    angular_array.clear();
    turns_array.clear();
    distance_array.clear();
    segments_array.clear();
    seg_angular_array.clear();
    seg_turns_array.clear();
    seg_distance_array.clear();
    seg_distance_3d_array.clear();
    seg_segments_array.clear();
    distance_3d_array.clear();
    dual_points.clear();
    points2dual.clear();
    points_2_segs.clear();
    links.clear();
    
    
    std::vector<graph_routes>().swap(angular_array);
    std::vector<graph_routes>().swap(turns_array);
    std::vector<graph_routes>().swap(distance_array);
    std::vector<graph_routes>().swap(distance_3d_array);
    std::vector<graph_routes>().swap(segments_array);
    
    std::vector<graph_routes>().swap(seg_angular_array);
    std::vector<graph_routes>().swap(seg_turns_array);
    std::vector<graph_routes>().swap(seg_distance_array);
    std::vector<graph_routes>().swap(seg_distance_3d_array);
    std::vector<graph_routes>().swap(seg_segments_array);
    
    std::vector<vertex>().swap(links);
    std::vector<vertex>().swap(dual_points);
    
    num_segments=0;
    num_points=0;
    num_links=0;
    
    
    G_dual_ang.clear();
    G_dual_turn.clear();
    S_dual_ang.clear();
    S_dual_turn.clear();
    S.clear();
    S_dist.clear();
    S_dist_3D.clear();
    G.clear();
    G_dist.clear();
    G_dist_3D.clear();
            
    
    choice_cache_G_dist.clear();
    choice_cache_G_dist_3D.clear();
    choice_cache_G.clear();
    choice_cache_G_ang.clear();
    choice_cache_G_turn.clear();
    choice_cache_S_dist.clear();
    choice_cache_S_dist_3D.clear();
    choice_cache_S.clear();
    choice_cache_S_ang.clear();
    choice_cache_S_turn.clear();
    
    std::vector<std::vector<route_weight_number> >().swap(choice_cache_G_dist);
    std::vector<std::vector<route_weight_number> >().swap(choice_cache_G_dist_3D);
    std::vector<std::vector<route_weight_number> >().swap(choice_cache_G);
    std::vector<std::vector<route_weight_number> >().swap(choice_cache_G_ang);
    std::vector<std::vector<route_weight_number> >().swap(choice_cache_G_turn);
    
    std::vector<std::vector<route_weight_number> >().swap(choice_cache_S_dist);
    std::vector<std::vector<route_weight_number> >().swap(choice_cache_S_dist_3D);
    std::vector<std::vector<route_weight_number> >().swap(choice_cache_S);
    std::vector<std::vector<route_weight_number> >().swap(choice_cache_S_ang);
    std::vector<std::vector<route_weight_number> >().swap(choice_cache_S_turn);
    
}




bool equal(double a, double b)
{
    return fabs(a - b) < EPSILON;
}





void iterate_path_reference(return_iterate* to_return,const std::vector< std::vector<int> > &path,const std::vector< int> &mult_path,const int &start_node,const int &node,const int &finish, int depth,const int &size,int N,int NN){
    
    

    to_return->error=0;
    to_return->paths.clear();

    
    
    std::vector<std::vector<int> > mult_choice,mult_choice_new;
    
    mult_choice.push_back(std::vector<int>(0));
    mult_choice.push_back(std::vector<int>(0));
    
    mult_choice_new=mult_choice;
    
    int iterating=1;
    int first=1;
    
    int count_attempt=0;
    
    while (iterating==1){
        
        count_attempt++;
        
        mult_choice_new[0].clear();
        mult_choice_new[1].clear();
        
        
        int split_node=-1;
        
        for (int i=mult_choice[0].size()-1;i>=0;i--){
            if (mult_choice[0][i]<(mult_choice[1][i]-1)){
                split_node=i;
                break;
            }
        }
        
        if ((!first)&&(split_node==-1)){
            iterating=0;
            break;
        }
        
        if (first){
            first=0;
        }
        
        std::vector<int> path_temp;
        path_temp.push_back(node);
        
        
        
        int n=node;
        int count=0;
        int error=0;
        

        while ((n!=start_node)&&(!error)){
            
            int mc;
            
            if (count==split_node){
                mc=mult_choice[0][count]+1;
            }
            else if (count<split_node){
                mc=mult_choice[0][count];
            }
            else{
                mc=0;
            }
            
                      
            
            
            mult_choice_new[0].push_back(mc);
            mult_choice_new[1].push_back(mult_path[n]);
            
            int new_node= path[n][mc];
            
            

            
            path_temp.push_back(new_node);
            
            
            
            n=new_node;
            
            for (int i=0;i<path_temp.size()-1;i++){
                if ((n!=start_node)&&(n==path_temp[i])){
                    error=1;
                }
            }
           
            
            if (count>size){//shouldn't happen
                error=1;
                to_return->error=4;
            }
            
            count++;
            
        }
       
        if (!error){
            to_return->paths.push_back(path_temp);
        }
       
        mult_choice=mult_choice_new;
        
        
        if ((count_attempt)>=N){
            if (to_return->paths.size()){
                to_return->error=1;
                break;
            }
            else{
                to_return->error=2;
                break;
            }
        }
         
         
        
        
    }
 
    
}



void populate_raw(std::vector<std::vector<route_weight_number> >* choice,graph& G,int seg, int dual,map_structures* map, int iterations,int& error,int& kill){
    
    int n;
    
    if (!dual){
        if (!seg){
            n=map->num_points;
        }
        else{
            n=map->num_links;
        }
    }
    else{
        if (!seg){
            n=2*map->num_links+map->num_points;
        }
        else{
            n=3*map->num_links;
        }
    }
    
    choice->resize(n,std::vector<route_weight_number>(n));
    std::vector<double> weight;
    std::vector<int> visited;
    graph_routes temp;
    return_iterate answer;
    
    push_time(text);
    push_output(text,"Starting weights and route numbers calculation...\n");
    
    int counter = 0;
    int start_buff=text->buff->length();
    std::stringstream output;
    
    //int done=0;
    
    for (int i=0;i<n;i++){
        
        if (kill){
            return;
        }
        
        
        counter++;
        if (((counter)>0.01*n)||((i+1)==n)){
            counter=0;
            Fl::lock();
            text->buff->remove(start_buff,text->buff->length());
            Fl::unlock();
            output.str("");
            output.precision(3);
            output.setf( std::ios::fixed, std:: ios::floatfield );
            output<<"------------------- >> "<<100*((double)(i+1)/(double)(n))<<"% calculated"<<std::endl;
            
            push_output(text,output.str());
        }
        
        
        
        dijkstra_reference_list_heap_raw(&temp,G,i,dual, map->num_links,weight, visited, seg,map);
        
        for (int j=0;j<n;j++){
            (*choice)[i][j].weight = temp.weight[j];
            
            
            
            iterate_path_reference(&answer,temp.path,temp.mult_path, temp.node_origin,temp.end_node[j],temp.end_node[j],0,map->num_points,iterations,iterations);
            
            if (answer.error){
                error=1;
            
            
            }
            
            
            /*
            if (answer.paths.size()>1000){
            std::cout<< " "<<map->dual_points[i].start<< " "<<map->dual_points[i].end<< " "<<map->dual_points[j].start<< " "<<map->dual_points[j].end<<" "<<answer.paths.size()<<" "<<temp.weight[j]<<std::endl;
               
                if (!done){
                    done=1;
                    for (int ii=0;ii<answer.paths[100].size();ii++){
                        
                        int i1 = answer.paths[100][ii];
                        std::cout<<map->dual_points[i1].start<<" "<<map->dual_points[i1].end<<std::endl;
                    }
                }
            }
             */
            
            if (temp.weight[j]>9e98){
                (*choice)[i][j].number = 0;
            }
            else{
                (*choice)[i][j].number = answer.paths.size();
            }
        }
    }
    push_time(text);
    push_output(text,"...done.\n");
    
}


void dijkstra_reference_list_heap_raw(graph_routes* return_struct, graph& G,int start_node,int dual, int num_links,std::vector<double>& weight,std::vector<int>& visited,int seg,map_structures* map){

    
    std::vector<int>& mult_path = return_struct->mult_path;
    std::vector<std::vector<int> >& path = return_struct->path;
    
    int num_nodes=G.neighbours.size();
    int num_points=num_nodes;
    
   
    
    
    if (return_struct->mult_path.size()!=num_nodes){
        
        return_struct->end_node.resize(num_nodes);
        return_struct->weight.resize(num_nodes);
        mult_path.resize(num_nodes);
        path.resize(num_nodes);
        
        
    }
    
    if (weight.size()!=num_nodes){
        weight.resize(num_nodes);
    }
    if (visited.size()!=num_nodes){
        visited.resize(num_nodes);
    }
    
    for (int i=0;i<num_nodes;i++){
        
        mult_path[i]=1;
        path[i]=std::vector<int>(1,start_node);
        weight[i]=INFINITY;
        visited[i]=0;
    }
    
    
    
    weight[start_node]=0;
    visited[start_node]=0;
    
    
    //HEAP
    
    
    std::vector<weight_index> vec_in;
    
    for (int i=0;i<num_nodes;i++){
        weight_index temp;
        temp.weight=INFINITY;
        temp.weight_sort=INFINITY;
        temp.index=i;
        vec_in.push_back(temp);
    }
    
    vec_in[start_node].weight=0;
    vec_in[start_node].weight_sort=0;
    
    
    MinHeap heap(vec_in);
    
    //END HEAP
    
    
    
   
    
    
    int count=1,current;
    double temp;
    
    
    double val,weight_current;
    
    while (count<=num_nodes+1){
        
        current=heap.GetMin(); //explicitly returns original matrix index
        weight_current = heap._vector[0].weight; // get node value
        heap._vector[0].weight_sort=INFINITY; //send visity nodes to the bottom of the heap'
        heap.BubbleDown(0); //actually send it down
        
        
        //for (int i=0;i<num_nodes;i++){
        for (int ii=0;ii<G.neighbours[current].size();ii++){
            
            int i=G.neighbours[current][ii];
            
            val=G.weights[current][i];  //from current to i
            
            if (dual){
                if (i==start_node){
                    val=-1;
                }
                
                if ((current>=2*num_links)&&(current!=start_node)){
                    val=-1; //<<-- absolutely necessary to stop routing through into/out of nodes (ie you can go into node > 2*num_links, but not back out (but you are allowed out of start)
                }
            }
            
                        
            if (val>=0){
                
                temp=weight_current+val;
                
                int heap_index = heap.index_back[i];  //we knwo node i but need to know where in the heap it is
                
                double weight_compare = heap._vector[heap_index].weight; //this is its actual weight (visited or not)
                
                
                
                if (equal(temp,weight_compare)){  //fine
                    mult_path[i]++;
                    path[i].push_back(current);
                }
                else if (temp<weight_compare){
                    
                    heap._vector[heap_index].weight=temp;
                    heap._vector[heap_index].weight_sort=temp;
                    heap.BubbleUp(heap_index); //send up as it might be the new minimum
                    
                    path[i][0]=current;
                    mult_path[i]=1;
                    path[i].resize(1);
                }
                
                
                
            }
        }
        
        
        count++;
    }
    
    
    for (int i=0;i<num_nodes;i++){
        
        int heap_index = heap.index_back[i];
        weight[i] = heap._vector[heap_index].weight;
        
    }
    

    return_struct->node_origin=start_node;
    
    
    for (int i=0;i<num_nodes;i++){
        
        
        return_struct->end_node[i]=i;
        
    }
    
    
    
        return_struct->origin=start_node;

        for (int i=0;i<num_nodes;i++){
            return_struct->weight[i]=weight[i];
        }
    
    
    
    
    
    
    
}





void dijkstra_reference_list_heap(graph_routes* return_struct, graph& G,int start_node,int dual, int num_links,std::vector<double>& weight,std::vector<int>& visited,int seg,map_structures* map){
    
    
    
    
    std::vector<int>& mult_path = return_struct->mult_path;
    std::vector<std::vector<int> >& path = return_struct->path;
    
    int num_nodes=G.neighbours.size();
    int num_points=num_nodes;
    
    
    if (dual){
        
        num_points=num_nodes-2*num_links;
        start_node+=2*num_links;
        
    }
    
    
    if (return_struct->mult_path.size()!=num_nodes){
        
        return_struct->end_node.resize(num_nodes);
        return_struct->weight.resize(num_points);
        mult_path.resize(num_nodes);
        path.resize(num_nodes);
        
        
    }
    
    if (weight.size()!=num_nodes){
        weight.resize(num_nodes);
    }
    if (visited.size()!=num_nodes){
        visited.resize(num_nodes);
    }
    
    for (int i=0;i<num_nodes;i++){
        
        mult_path[i]=1;
        path[i]=std::vector<int>(1,start_node);
        weight[i]=INFINITY;
        visited[i]=0;
    }
    
    
    
    weight[start_node]=0;
    visited[start_node]=0;
    
    
    //HEAP
    
    
    std::vector<weight_index> vec_in;
    
    for (int i=0;i<num_nodes;i++){
        weight_index temp;
        temp.weight=INFINITY;
        temp.weight_sort=INFINITY;
        temp.index=i;
        vec_in.push_back(temp);
    }
    
    vec_in[start_node].weight=0;
    vec_in[start_node].weight_sort=0;
    
    
    MinHeap heap(vec_in);
    
    //END HEAP
    
    
    
    
    
    
    
    
    int count=1,current;
    double temp;
    
    
    double val,weight_current;
    
    while (count<=num_nodes+1){                        
        
        current=heap.GetMin(); //explicitly returns original matrix index
        weight_current = heap._vector[0].weight; // get node value
        heap._vector[0].weight_sort=INFINITY; //send visity nodes to the bottom of the heap'
        heap.BubbleDown(0); //actually send it down
        
        
        //for (int i=0;i<num_nodes;i++){
        for (int ii=0;ii<G.neighbours[current].size();ii++){
            
            int i=G.neighbours[current][ii];
            
            val=G.weights[current][i];
            
            if (dual){
                if (i==start_node){
                    val=-1;
                }
                
                if ((current>=2*num_links)&&(current!=start_node)){
                    val=-1;
                }
            }
                                
            if (val>=0){
                
                temp=weight_current+val;
                
                int heap_index = heap.index_back[i];  //we knwo node i but need to know where in the heap it is
                
                double weight_compare = heap._vector[heap_index].weight; //this is its actual weight (visited or not)
                
                
                
                if (equal(temp,weight_compare)){  //fine
                    mult_path[i]++;
                    path[i].push_back(current);
                }
                else if (temp<weight_compare){
                    
                    heap._vector[heap_index].weight=temp;
                    heap._vector[heap_index].weight_sort=temp;
                    heap.BubbleUp(heap_index); //send up as it might be the new minimum
                    
                    path[i][0]=current;
                    mult_path[i]=1;
                    path[i].resize(1);
                }
                
                
                
            }
        }
        
        
        count++;
    }
    
    
    for (int i=0;i<num_nodes;i++){
        
        int heap_index = heap.index_back[i];
        weight[i] = heap._vector[heap_index].weight;
        
    }
    

    return_struct->node_origin=start_node;
    
    
    for (int i=0;i<num_points;i++){
        if (dual) {
            
            return_struct->end_node[i]=i+2*num_links;
        }
        else{
            
            return_struct->end_node[i]=i;
        }
    }
    
    
    if (dual){
        return_struct->origin=start_node-2*num_links;
        
        for (int i=0;i<num_points;i++){
            
            return_struct->weight[i]=weight[i+2*num_links];
        }
    }
    else{
        return_struct->origin=start_node;

        for (int i=0;i<num_points;i++){
            return_struct->weight[i]=weight[i];
        }
    }
    
    
    
    
    
    
}
/*******************START NEW READ IN********************/



int readin_desks(std::vector<vec> &points,std::vector<location>& desks,std::string str_desks,text_output* tex){
    
    std::stringstream output;
    
    std::ifstream in_data;
    std::string str;
    
    
    if (!points.size()){
        push_time(text);
        push_output(text,"No spatial map points! Cannot associate location markers with a spatial map. Failed.\n");
        return 1;
    }
    
    output.str("");output<<"OPENING: "<<str_desks<<std::endl;
    push_time(tex);
    push_output(tex,output.str());
    
    
    
    try {
        in_data.open(str_desks.c_str());
    }
    catch (std::ifstream::failure e) {
        std::cout << "Error opening "<<str_desks<<std::endl;
        output.str("");output<<"error opening "<<str_desks<<std::endl;
        push_time(tex);
        push_output(tex,output.str());
        
        return 1;
    }
    
    
    
    // determine format
    try{
        tool_getline(in_data, str);
    }
    catch(...){
        
        std::cout << "Error opening "<<str_desks<<std::endl;
        output.str("");output<<"error opening "<<str_desks<<std::endl;
        push_time(tex);
        push_output(tex,output.str());
        
        return 1;
    }
    
    
    
    
    for (int i=0;i<str.size();i++){
        if ((str[i]==',')||(str[i]==';')||(str[i]=='[')||(str[i]=='{')||(str[i]=='(')||(str[i]=='\t')){
            str[i]=' ';
        }
        
    }
    
    
    
    int format =-1;
    double x1,x2,x3;
    std::string test_string;
    int test_point;
    
    std::istringstream iss(str);
    if (!(iss >> test_string  >> x1 >> x2 >>x3>>test_point)){
        std::istringstream iss(str);
        if (!(iss >> test_string >> x1 >>x2 >>x3)){
            std::istringstream iss(str);
            if (!(iss >>  x1 >> x2 >> x3>>test_point)){
                
                std::istringstream iss(str);
                if (!(iss >> x1 >> x2 >>x3)){
                    std::cout << "Error opening "<<str_desks<<std::endl;
                    output.str("");output<<"error opening "<<str_desks<<std::endl;
                    push_time(tex);
                    push_output(tex,output.str());
                
                    return 1;
                }
                else{
                    format=0;
                }
            }
            else{
                std::cout<<test_point<<" "<<x1<<" "<<x2<<" "<<x3<<std::endl;
                format =1;
            }
        }
        else{
            format =2;
        }
    }
    else{
        format=3;
    }
    
    
    in_data.close();
    
    if ((format!=0)&&(format!=1)&&(format!=2)&&(format!=3)){
        std::cout << "Error opening "<<str_desks<<std::endl;
        output.str("");output<<"error opening "<<str_desks<<std::endl;
        push_time(tex);
        push_output(tex,output.str());
        
        return 1;
    }
    
    //end determine format
    
    
    
    std::cout<<"ARE WE GETTING TO HERE AT LEAST format: "<<format<<std::endl;
    
    
    
    std::vector<location> temp_desks;
    
    
    try {
        in_data.open(str_desks.c_str());
    }
    catch (std::ifstream::failure e) {
        std::cout << "Error opening "<<str_desks<<std::endl;
        output.str("");output<<"error opening "<<str_desks<<std::endl;
        push_time(tex);
        push_output(tex,output.str());
        
        return 1;
    }
    
    int c=1;
    while (c)
    {
        
        try{
            tool_getline(in_data, str);
        }
        catch(...){
            
            c=0;
            break;
        }
        
        for (int i=0;i<str.size();i++){
            if ((str[i]==',')||(str[i]==';')||(str[i]=='[')||(str[i]=='{')||(str[i]=='(')||(str[i]=='\t')){
                str[i]=' ';
            }
            
        }
        
        std::istringstream iss(str);
        std::string label;
        label.assign("");
        int point = -1;
        double x,y,z;
        
        int error=0;
        
        
        switch (format){
            case 3:
                if (!(iss >> label >> x >> y >> z >> point)) { error = 1;}
                break;
            case 2:
                if (!(iss  >> x >> y >> z >> point)) { error = 1;}
                break;
            case 1:
                if (!(iss >> label >> x >> y >> z)) { error = 1;}
                break;
            case 0:
                if (!(iss >> x >> y >> z)) { error = 1;}
                break;
            default:
                error=1;
        }
        
        


        if (error){break;}
        
    
        if ((point == -1)||(point<0)||(point>=points.size())){
            double min=9e99,xx,yy,zz;
            double temp;
            for (int i=0;i<points.size();i++){
                
                xx= points[i].x;
                yy= points[i].y;
                zz= points[i].z;
                
                temp = (xx-x)*(xx-x)+(yy-y)*(yy-y)+(zz-z)*(zz-z);
                
                if (temp<min){min=temp;point=i;}
                
            }
        }
        
        location temp;
    
        temp.point = point;
        temp.loc.x = x;
        temp.loc.y = y;
        temp.loc.z = z;
        temp.label = label;
        temp.is_desk = 1;
        
        if ((point<points.size())&&(point>=0)){
            int floor = points[point].floor;
            temp.loc.floor=floor;            
            temp_desks.push_back(temp);
        }
        
        
        
        
    }
    
    in_data.close();
    
    desks=temp_desks;
    
    output.str("");output<<"Read in "<<desks.size()<<" desks."<<std::endl;
    push_time(tex);
    push_output(tex,output.str());
    
    return 0;
    
}

int readin_points(std::vector<vec> &points_,std::string str_points,text_output* tex){
    
    
    std::stringstream output;
    
    std::ifstream in_data;
    std::string str;
    
    output.str("");output<<"OPENING: "<<str_points<<std::endl;
    push_time(tex);
    push_output(tex,output.str());

    
    in_data.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
    
    
    
    std::vector<vec> points;
    
    try {
        in_data.open(str_points.c_str());
       
    }
    catch (std::ifstream::failure e) {
        std::cout << "Error opening "<<str_points<<std::endl;
        output.str("");output<<"error opening "<<str_points<<std::endl;
        push_time(tex);
        push_output(tex,output.str());
        
        return 1;
    }
    
    
        
    
    
    
    
    // determine format
    try{
        tool_getline(in_data, str);
    }
    catch(...){
        
        std::cout << "Error opening "<<str_points<<std::endl;
        output.str("");output<<"error opening "<<str_points<<std::endl;
        push_time(tex);
        push_output(tex,output.str());
        
        return 1;
    }
    
    

    
    for (int i=0;i<str.size();i++){
        if ((str[i]==',')||(str[i]==';')||(str[i]=='[')||(str[i]=='{')||(str[i]=='(')||(str[i]=='\t')){
            str[i]=' ';
        }

    }
    
   
    
    int format =-1;
    double x1,x2,x3,x4,x5;
    std::istringstream iss(str);
    if (!(iss >> x1 >> x2 >> x3 >> x4 >>x5)){
        std::istringstream iss(str);
        if (!(iss >> x1 >> x2 >> x3 >> x4)){
            std::istringstream iss(str);
            if (!(iss >> x1 >> x2 >> x3)){
                std::cout << "Error opening "<<str_points<<std::endl;
                output.str("");output<<"error opening "<<str_points<<std::endl;
                push_time(tex);
                push_output(tex,output.str());
                
                return 1;
            }
            else{
                format =0;
            }
        }
        else{
            format =1;
        }
    }
    else{
        format=2;
    }
    
    
    in_data.close();
    
    if ((format!=0)&&(format!=1)&&(format!=2)){
        std::cout << "Error opening "<<str_points<<std::endl;
        output.str("");output<<"error opening "<<str_points<<std::endl;
        push_time(tex);
        push_output(tex,output.str());
        
        return 1;
    }
    
    //end determine format
    
    
    try {
        in_data.open(str_points.c_str());
    }
    catch (std::ifstream::failure e) {
        std::cout << "Error opening "<<str_points<<std::endl;
        output.str("");output<<"error opening "<<str_points<<std::endl;
        push_time(tex);
        push_output(tex,output.str());
        
        return 1;
    }
    
    std::string delim;
    delim = '\r';
    
    int c=1;
    while (c)
    {
        
        try{
            tool_getline(in_data, str);
        }
        catch(...){
            
            c=0;
            break;
        }
        
        for (int i=0;i<str.size();i++){
            if ((str[i]==',')||(str[i]==';')||(str[i]=='[')||(str[i]=='{')||(str[i]=='(')||(str[i]=='\t')){
                str[i]=' ';
            }

        }
        
        std::istringstream iss(str);
        int point,floor;
        double x,y,z;
        
        int error=0;
        
        if (format==0){
            
            if (!(iss >> x >> y >> z)) { error=1; }
            
        }
        else if (format ==1){
            
            if (!(iss >> point>> x >> y >> z )) { error=1; }
            
        }
        else if( format ==2){
            
            if (!(iss>> point >> x >> y >> z >> floor)) { error=1; }
            
        }
        
        
        
        if (error){break;}
        
        vec temp;
        temp.x=x;
        temp.y=y;
        temp.z=z;
        if (format==2){
            temp.floor=floor;
        }
        points.push_back(temp);
        
    }
    
    in_data.close();
    
    output.str("");output<<"size of points is "<<points.size()<<std::endl;
    push_time(tex);
    push_output(tex,output.str());
    
        
    
    if (format!=2){
        
        int floor=0;
        
        double maxz=-9e99;
        for (int i=0;i<points.size();i++){
            if (points[i].z>maxz) maxz=points[i].z;
        }
        
        double minz=9e99,old_minz=-9e99;
        
        
        do{
            minz=9e99;
            for (int i=0;i<points.size();i++){
                if ((points[i].z<minz)&&(points[i].z>old_minz)) minz=points[i].z;
            }
            
            for (int i=0;i<points.size();i++){
                if (points[i].z==minz) points[i].floor=floor;
            }
            old_minz=minz;
            floor++;
            std::cout<<old_minz<<" "<<maxz<<" "<<floor<<std::endl;
        }while(old_minz<maxz);
        //end floor asignment
        
    }
    
    points_=points;
    
    return 0;
    
    
}

int readin_adj_list(std::vector<vec> &points_,std::vector<vertex> &adj_list,std::string str_matrix,text_output* tex){

    std::stringstream output;
    
    std::ifstream in_data;
    std::string str;
    
    output.str("");output<<"OPENING: "<<str_matrix<<std::endl;
    push_time(tex);
    push_output(tex,output.str());
    
    in_data.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
    
    
    try {
        in_data.open (str_matrix.c_str());
        
    }
    catch (std::ifstream::failure e) {
        std::cout << "Error opening "<<str_matrix<<std::endl;
        
        output.str("");output<<"error opening "<<str_matrix<<std::endl;
        push_time(tex);
        push_output(tex,output.str());
        
        return 1;
    }
    
    std::vector<vertex> to_add;
    
    int line =0;
    
    int c=1;
    while (c)
    {
        
        try{
            tool_getline(in_data, str);
        }
        catch(...){
            
            c=0;
            break;
        }
        
        
        
        for (int i=0;i<str.size();i++){
            if ((str[i]==',')||(str[i]==';')||(str[i]=='[')||(str[i]=='{')||(str[i]=='(')||(str[i]=='\t')){
                str[i]=' ';
            }
        }
        
        
        
        std::vector<int> temp;
        
        std::istringstream iss(str);
        int point1=-1,point2=-1;
    
        int include =1;
        
        if (!(iss>>point1>>point2)){
            c=0;break;
        }
        
        if ((point1>-1)&&(point2>-1)){
        
        if ((point1>points_.size()-1)||(point1<0)){
            include=0;
        }
        if ((point2>points_.size()-1)||(point2<0)){
            include=0;
        }
        
        for (int i=0;i<adj_list.size();i++){
            if (((adj_list[i].start==point1)&&(adj_list[i].end==point2))||((adj_list[i].end==point1)&&(adj_list[i].start==point2))){
                include=0;
            }
            
            
        }
        for (int i=0;i<to_add.size();i++){
            if (((to_add[i].start==point1)&&(to_add[i].end==point2))||((to_add[i].end==point1)&&(to_add[i].start==point2))){
                include=0;
            }
        }
        if (include){
            vertex temp;
            temp.start=point1;
            temp.end=point2;
            to_add.push_back(temp);
        }
        }
        
    }
    
    in_data.close();
    
    
    for (int i=0;i<to_add.size();i++){
        adj_list.push_back(to_add[i]);
    }
    
    push_time(text);
    std::stringstream sss;
    sss<<"Added "<<to_add.size()<<" unique links."<<std::endl;
    push_output(text,sss.str());
    
    return 0;
}

    
int readin_matrix(std::vector<vec> &points_,std::vector<vertex> &adj_list,std::string str_matrix,text_output* tex){
    
    std::stringstream output;
    
    std::ifstream in_data;
    std::string str;
    
    output.str("");output<<"OPENING: "<<str_matrix<<std::endl;
    push_time(tex);
    push_output(tex,output.str());
    
    in_data.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
    
    
   
    
    try {
        in_data.open (str_matrix.c_str());
        
    }
    catch (std::ifstream::failure e) {
        std::cout << "Error opening "<<str_matrix<<std::endl;
        
        output.str("");output<<"error opening "<<str_matrix<<std::endl;
        push_time(tex);
        push_output(tex,output.str());
        
        return 1;
    }
    
   
      
    
    int line =0;
    int c=1;
    while (c)
    {
        
        try{
            tool_getline(in_data, str);
        }
        catch(...){
            
            c=0;
            break;
        }
        
        for (int i=0;i<str.size();i++){
            if ((str[i]==',')||(str[i]==';')||(str[i]=='[')||(str[i]=='{')||(str[i]=='(')||(str[i]=='\t')){
                str[i]=' ';
            }
        }
        
        

        std::vector<int> temp;
        
        std::istringstream iss(str);
        int ii;
        
        int column=0;
        while (iss>>ii){
           
            if ((column>line)&&(column<points_.size())&&(ii>0)){
                vertex temp;
                temp.start = line;
                temp.end = column;
                adj_list.push_back(temp);
            }
            column++;
        }
        
        
        line++;
        if (line>=points_.size()){
            break;
        }
    }
    
    in_data.close();
    
    std::stringstream s;
    s<<"Added "<<adj_list.size()<<" links"<<std::endl;
    push_time(text);
    push_output(text,s.str().c_str());
    
    return 0;
    
}


void* calculate_distances_paths(void* t){
    
   
    
    
    container* root = (container*) t;
    
   
    
    std::vector<vec> points = root->current->points;
    std::vector<vertex> adj_list = root->current->adj_list;
    text_output* tex = root->buttons.text; //check
    map_structures* map=&(root->current->map);
    Fl_Progress* progress=root->buttons.progress;
    
    
    
    
    std::stringstream output;

    double pi=3.141592653589793238;
    
    std::vector<vertex> links,dual_points,lines,links_temp;
    std::vector<int> lines_to_links,links_to_lines;
    int num_points,num_segments,num_dual_points,num_dual_points_seg;
    
    
    num_points=points.size();
    
    //CONSTRUCT SEGMENT AND DISTANCE MATRICES//
    
    push_time(tex);
    push_output(tex,"Starting construction of weight matrices for all metrics/starting configurations... \n");
    
    
    //populate points to segments matrix
 
    
    //CREATE LINKS/SEGEMENTS ARRAY IN TERMS OF POINTS///
   
    
    std::map<vertex,int> points_2_segs;
    
    for (int i=0;i<root->current->adj_list.size();i++){
        vertex temp,temp1;
        
        temp.start = adj_list[i].start;
        temp.end = adj_list[i].end;
        
        temp1.start = adj_list[i].end;
        temp1.end = adj_list[i].start;
        
        points_2_segs[temp]=i;
        points_2_segs[temp1]=i;
    }
    
    
    links = root->current->adj_list;
    
    
    num_segments=links.size();
    
    
    
    graph S,S_dist,S_dist_3D,G,G_dist,G_dist_3D;
    
    
    for (int i=0;i<num_points;i++){
        G.neighbours.push_back(std::vector<int>(0));  //vector of neighbours for each node
        G_dist.neighbours.push_back(std::vector<int>(0));
        G_dist_3D.neighbours.push_back(std::vector<int>(0));
        
        G.weights.push_back(std::map<int,double>());  //vector of hash tables which for neighbour node's values encode as keys for weights from the node to those neighbours
        G_dist.weights.push_back(std::map<int,double>());
        G_dist_3D.weights.push_back(std::map<int,double>());
    }
    
    
    for (int ii=0;ii<root->current->adj_list.size();ii++){
        
        int i = root->current->adj_list[ii].start;
        int j = root->current->adj_list[ii].end;
        
        double r2=sqrt((points[i].x-points[j].x)*(points[i].x-points[j].x)+(points[i].y-points[j].y)*(points[i].y-points[j].y));
        double r3=sqrt((points[i].x-points[j].x)*(points[i].x-points[j].x)+(points[i].y-points[j].y)*(points[i].y-points[j].y)+(points[i].z-points[j].z)*(points[i].z-points[j].z));
        
        if (equal(r2,0)) r2=2e-6;
        if (equal(r3,0)) r3=2e-6;
        
        G.neighbours[i].push_back(j);
        G.neighbours[j].push_back(i);
        G_dist.neighbours[i].push_back(j);
        G_dist.neighbours[j].push_back(i);
        G_dist_3D.neighbours[i].push_back(j);
        G_dist_3D.neighbours[j].push_back(i);

        G.weights[i][j]=1;
        G.weights[j][i]=1;
        G_dist.weights[i][j]=r2;
        G_dist.weights[j][i]=r2;
        G_dist_3D.weights[i][j]=r3;
        G_dist_3D.weights[j][i]=r3;
        
    }
    
    
    //CREATE POINTS BASED ARRAYS//
    
    
    
    
    push_time(text);
    push_output(text,"...created point base metric arrays...\n");
    update_progress(progress,0.2,0);
    
    ///CREATE SEGMENT BASED ARRAYS//
    
    
    
    for (int i=0;i<num_segments;i++){
                
        
        S.neighbours.push_back(std::vector<int>(0));
        S_dist.neighbours.push_back(std::vector<int>(0));
        S_dist_3D.neighbours.push_back(std::vector<int>(0));
        
        S.weights.push_back(std::map<int,double>());
        S_dist.weights.push_back(std::map<int,double>());
        S_dist_3D.weights.push_back(std::map<int,double>());
        
        
        for (int j=0;j<num_segments;j++){
            
            if (i!=j){
                
                                
                if (((links[i].start==links[j].start))||((links[i].start==links[j].end))||((links[i].end==links[j].start))||((links[i].end==links[j].end))){
                    
                    
                    double r2=0.5*sqrt((points[links[i].start].x-points[links[i].end].x)*(points[links[i].start].x-points[links[i].end].x)+(points[links[i].start].y-points[links[i].end].y)*(points[links[i].start].y-points[links[i].end].y))+0.5*sqrt((points[links[j].start].x-points[links[j].end].x)*(points[links[j].start].x-points[links[j].end].x)+(points[links[j].start].y-points[links[j].end].y)*(points[links[j].start].y-points[links[j].end].y));
                    double r3=0.5*sqrt((points[links[i].start].x-points[links[i].end].x)*(points[links[i].start].x-points[links[i].end].x)+(points[links[i].start].y-points[links[i].end].y)*(points[links[i].start].y-points[links[i].end].y)+(points[links[i].start].z-points[links[i].end].z)*(points[links[i].start].z-points[links[i].end].z))+0.5*sqrt((points[links[j].start].x-points[links[j].end].x)*(points[links[j].start].x-points[links[j].end].x)+(points[links[j].start].y-points[links[j].end].y)*(points[links[j].start].y-points[links[j].end].y)+(points[links[j].start].z-points[links[j].end].z)*(points[links[j].start].z-points[links[j].end].z));
                    
                    if (equal(r2,0)) r2=2e-6;
                    if (equal(r3,0)) r3=2e-6;
                    
                    S.neighbours[i].push_back(j);
                    S_dist.neighbours[i].push_back(j);
                    S_dist_3D.neighbours[i].push_back(j);
                    S.weights[i][j]=1;
                    S_dist.weights[i][j]=r2;
                    S_dist_3D.weights[i][j]=r3;
                    

                }
            }
            
        }
    }
    
    push_time(text);
    push_output(text,"...created segment based metric arrays...\n");
    update_progress(progress,0.4,0);
    
    //CREATE DUALGRAPH POINTS//
    
    for (int i=0; i<2*links.size()+points.size();i++){
        
        vertex temp;
        
        if (i<links.size()){
            temp.start=links[i].start;  //forward direction for segment
            temp.end=links[i].end;
        }
        else if(i<2*links.size()){
            temp.end=links[i-links.size()].start; //reverse direction for segment
            temp.start=links[i-links.size()].end;
            
        }
        else{
            temp.start=i-2*links.size();
            temp.end=i-2*links.size();
        }
        
        dual_points.push_back(temp);
    }
        
    std::map<vertex,int> points2dual;
    
    for (int i=0;i<dual_points.size();i++){
        vertex temp;
        temp.start = dual_points[i].start;
        temp.end = dual_points[i].end;
        points2dual[temp]=i;
    }
    
    
    
    
    num_dual_points=dual_points.size();
    num_dual_points_seg=3*links.size();
    
    push_time(text);
    push_output(text,"...created dual graph points...\n");
    update_progress(progress,0.4,0);
    
    //CREATE DUALGRAPH MATRICES//
    

    
    
    
    graph G_dual_ang,G_dual_turn,S_dual_ang,S_dual_turn;
    
    
    for (int i=0;i<num_dual_points;i++){    
        
        G_dual_ang.neighbours.push_back(std::vector<int>(0));
        G_dual_turn.neighbours.push_back(std::vector<int>(0));
        
        G_dual_ang.weights.push_back(std::map<int,double>());
        G_dual_turn.weights.push_back(std::map<int,double>());
        
        
        for (int j=0;j<num_dual_points;j++){
            
            if (((dual_points[i].start==dual_points[i].end)||(dual_points[j].start==dual_points[j].end))&&(dual_points[i].end==dual_points[j].start)){
                
                
                
                //new
                G_dual_ang.neighbours[i].push_back(j);
                G_dual_turn.neighbours[i].push_back(j);
                
                G_dual_ang.weights[i][j]=0;
                G_dual_turn.weights[i][j]=0;
                
                //end new
                
            }
            else if(dual_points[i].end==dual_points[j].start){
                                
                int old_point,mid_point,new_point;
                double x1,x2,y1,y2,theta1,theta2,temp;
                
                old_point=dual_points[i].start;
                mid_point=dual_points[i].end;
                new_point=dual_points[j].end;
                
                x1=points[new_point].x-points[mid_point].x;
                y1=points[new_point].y-points[mid_point].y;
                
                x2=points[mid_point].x-points[old_point].x;
                y2=points[mid_point].y-points[old_point].y;

                
                theta1=atan2(y1,x1)*360.0/(2.0*pi);
                theta2=atan2(y2,x2)*360.0/(2.0*pi);
                
                temp=theta1-theta2;
                if (temp>180){
                    temp-=360;
                }
                else if(temp< -180){
                    temp+=360;
                }
                
                
                //dealing with floor changes: planar angle changes only
                
                
                if ((points[old_point].x==points[mid_point].x)&&(points[old_point].y==points[mid_point].y)){ //from vertical link
                    
                    temp =0;
                    
                }
                else if ((points[new_point].x==points[mid_point].x)&&(points[new_point].y==points[mid_point].y)){ //into vertical link
                    
                    temp=0;
                }
                
                //end dealing with floor changes
                
                
                
                if (old_point==new_point){
                    //temp=180; //old version
                    temp=INFINITY; // but let's implement strictly no backtrcking
                }
                

                
                
                G_dual_ang.neighbours[i].push_back(j);
                G_dual_turn.neighbours[i].push_back(j);
                
                G_dual_ang.weights[i][j]=fabs(temp);
                double tt;
                if (fabs(temp)>root->buttons.turn_ang->value()){
                    tt=1.0;
                    if (old_point==new_point){
                        tt=INFINITY; // strictly no backtracking
                    }
                }
                else{
                    tt=0.0;
                }
                G_dual_turn.weights[i][j]=tt;
                
                
                
            }
        }
    }
    
    push_time(text);
    push_output(text,"...created point based dualgraph metric arrays...\n");
    update_progress(progress,0.6,0);
    
    
   
    
    for (int i=0;i<num_dual_points_seg;i++){
        

        
        
        S_dual_ang.neighbours.push_back(std::vector<int>(0));
        S_dual_turn.neighbours.push_back(std::vector<int>(0));
        
        S_dual_ang.weights.push_back(std::map<int,double>());
        S_dual_turn.weights.push_back(std::map<int,double>());
        
        if (i<links.size()){
            
            S_dual_ang.neighbours[i].push_back(i+2*links.size());
            S_dual_turn.neighbours[i].push_back(i+2*links.size());
            S_dual_ang.weights[i][i+2*links.size()]=0;
            S_dual_turn.weights[i][i+2*links.size()]=0;
        }
        else if (i<2*links.size()){
            
            S_dual_ang.neighbours[i].push_back(i+links.size());
            S_dual_turn.neighbours[i].push_back(i+links.size());
            S_dual_ang.weights[i][i+links.size()]=0;
            S_dual_turn.weights[i][i+links.size()]=0;
        }
        else{
            S_dual_ang.neighbours[i].push_back(i-links.size());
            S_dual_turn.neighbours[i].push_back(i-links.size());
            S_dual_ang.weights[i][i-links.size()]=0;
            S_dual_turn.weights[i][i-links.size()]=0;
            S_dual_ang.neighbours[i].push_back(i-2*links.size());
            S_dual_turn.neighbours[i].push_back(i-2*links.size());
            S_dual_ang.weights[i][i-2*links.size()]=0;
            S_dual_turn.weights[i][i-2*links.size()]=0;
        }
        
        
        
        
        
        for (int j=0;j<num_dual_points_seg;j++){
        
            
            if ((i<2*links.size())&&(j<2*links.size())){
                
                if(dual_points[i].end==dual_points[j].start){
                    
                    int old_point,mid_point,new_point;
                    double x1,x2,y1,y2,theta1,theta2,temp;
                    
                    old_point=dual_points[i].start;
                    mid_point=dual_points[i].end;
                    new_point=dual_points[j].end;
                    
                    x1=points[new_point].x-points[mid_point].x;
                    y1=points[new_point].y-points[mid_point].y;
                    
                    x2=points[mid_point].x-points[old_point].x;
                    y2=points[mid_point].y-points[old_point].y;

                    theta1=atan2(y1,x1)*360.0/(2.0*pi);
                    theta2=atan2(y2,x2)*360.0/(2.0*pi);
                    
                    temp=theta1-theta2;
                    if (temp>180){
                        temp-=360;
                    }
                    else if(temp< -180){
                        temp+=360;
                    }
                    
                    //dealing with floor changes: planar angle changes only
                    
                    if ((points[old_point].x==points[mid_point].x)&&(points[old_point].y==points[mid_point].y)){ //from vertical link
                        temp =0;
                    }
                    else if ((points[new_point].x==points[mid_point].x)&&(points[new_point].y==points[mid_point].y)){ //into vertical link
                        temp=0;
                    }
                    
                    //end dealing with floor changes
                                                            
                    if (old_point==new_point){
                        //temp=180;//old version
                        temp=INFINITY; // strictly no backtracking
                    }
                    
                    
                    
                    
                    S_dual_ang.neighbours[i].push_back(j);
                    S_dual_turn.neighbours[i].push_back(j);
                    
                    S_dual_ang.weights[i][j]=fabs(temp);
                    double tt;
                    if (fabs(temp)>root->buttons.turn_ang->value()){
                        tt=1.0;
                        if (old_point==new_point){
                            tt=INFINITY; // strictly no backtracking
                        }
                    }
                    else{
                        tt=0.0;
                    }
                    S_dual_turn.weights[i][j]=tt;
                    
                    
                }
            }
            
                        
            
        }
    }
    
    push_time(text);
    push_output(text,"...created segment based dualgraph metric arrays...\n");
    update_progress(progress,1.0,0);
     
  
    push_time(tex);
  
    push_output(tex,"...done.\n");
    
    /////////////////////END OF GRAPH CONSTRUCTION///////////////////
   
    
    ////////////////////START OF METRIC/PATH STRUCTURE GENERATION////////////
    
   
    
    push_time(tex);
    push_output(tex,"Wrapping up...\n");
    
    
    map->num_points=num_points;
    map->num_segments=num_segments;
    map->num_links=links.size();
    map->links=links;
    map->dual_points=dual_points;
    
    map->points2dual=points2dual;
    
    
    map->G=G;
    map->G_dist=G_dist;
    map->G_dist_3D=G_dist_3D;
    map->G_dual_ang=G_dual_ang;
    map->G_dual_turn=G_dual_turn;
    map->S=S;
    map->S_dist=S_dist;
    map->S_dist_3D=S_dist_3D;
    map->S_dual_ang=S_dual_ang;
    map->S_dual_turn=S_dual_turn;
    
    map->points_2_segs=points_2_segs;
    
    
    
    
    set_cache_arrays(map);
    
    
    Fl::awake(awake_map_warning_off,NULL);
   
  
    push_time(tex);
  
    push_output(tex,"done.\n");
    
    
    root->calculating = FREE;
    root->current->map_valid=1;
    
    
    Fl::lock();
    turn_ang->activate();
    
    if (floor_choice->value()>0){
        zfloor->activate();
    }
    Fl::unlock();
    
    

    std::cout<<"test out"<<std::endl;
        
        return NULL;
#if !defined(WIN32)
    return NULL;
    pthread_exit(NULL);
#else
    return NULL;
#endif
  
}

void set_cache_arrays(map_structures* map){
    
    std::vector<graph_routes> angular_array,turns_array,distance_array,distance_3d_array,seg_angular_array,seg_turns_array,seg_distance_array,seg_distance_3d_array,segments_array,seg_segments_array,seg_crow_3d_array,seg_crow_array,crow_array,crow_3d_array;
    
    
    for (int i=0;i<map->num_points;i++){
        graph_routes temp;
        temp.exist=0;
        distance_array.push_back(temp);
        distance_3d_array.push_back(temp);
        angular_array.push_back(temp);
        turns_array.push_back(temp);
        segments_array.push_back(temp);
        crow_array.push_back(temp);
        crow_3d_array.push_back(temp);
    }
    
    for (int i=0;i<map->num_links;i++){
        graph_routes temp;
        temp.exist=0;
        seg_distance_array.push_back(temp);
        seg_distance_3d_array.push_back(temp);
        seg_angular_array.push_back(temp);
        seg_turns_array.push_back(temp);
        seg_segments_array.push_back(temp);
        seg_crow_array.push_back(temp);
        seg_crow_3d_array.push_back(temp);
    }
    
    map->angular_array=angular_array;
    map->turns_array=turns_array;
    map->distance_array=distance_array;
    map->distance_3d_array=distance_3d_array;
    map->segments_array=segments_array;
    map->crow_array=crow_array;
    map->crow_3d_array=crow_3d_array;
    
    map->seg_angular_array=seg_angular_array;
    map->seg_turns_array=seg_turns_array;
    map->seg_distance_array=seg_distance_array;
    map->seg_distance_3d_array=seg_distance_3d_array;
    map->seg_segments_array=seg_segments_array;
    map->seg_crow_array=seg_crow_array;
    map->seg_crow_3d_array=seg_crow_3d_array;
 
    map->choice_cache_G_dist.clear();
    map->choice_cache_G_dist_3D.clear();
    map->choice_cache_G.clear();
    map->choice_cache_G_ang.clear();
    map->choice_cache_G_turn.clear();
    map->choice_cache_S_dist.clear();
    map->choice_cache_S_dist_3D.clear();
    map->choice_cache_S.clear();
    map->choice_cache_S_ang.clear();
    map->choice_cache_S_turn.clear();
    
    std::vector<std::vector<route_weight_number> >().swap(map->choice_cache_G_dist);
    std::vector<std::vector<route_weight_number> >().swap(map->choice_cache_G_dist_3D);
    std::vector<std::vector<route_weight_number> >().swap(map->choice_cache_G);
        std::vector<std::vector<route_weight_number> >().swap(map->choice_cache_G_ang);
        std::vector<std::vector<route_weight_number> >().swap(map->choice_cache_G_turn);
    
    std::vector<std::vector<route_weight_number> >().swap(map->choice_cache_S_dist);
    std::vector<std::vector<route_weight_number> >().swap(map->choice_cache_S_dist_3D);
    std::vector<std::vector<route_weight_number> >().swap(map->choice_cache_S);
    std::vector<std::vector<route_weight_number> >().swap(map->choice_cache_S_ang);
    std::vector<std::vector<route_weight_number> >().swap(map->choice_cache_S_turn);
   
}

void* calculate_lines(void* data){

    container* root= (container*) data;
    
    std::vector<int> line_segments;
    
    
    
    push_time(text);
    push_output(text,"Starting line identification...\n");
    
    int start_buff = text->buff->length();
    std::stringstream output;
    int size = root->current->map.links.size();
    
    int old_count=0;
    
    int size_out;
    
    size_out = ((0.01*(double)size)<1) ? 1 : 0.01*size;
    
    std::vector<int> to_do;
    
    for (unsigned i=0;i<size;i++){
        to_do.push_back(i);
    }
    
    
    graph_routes gr_temp;
    std::vector<double> weight_d;
    std::vector<int> visited_d;
    
    for (unsigned i=0;i<to_do.size();i++){
        
        
        if (root->killthread){
            Fl::lock();
            turn_ang->activate();
            Fl::unlock();
            push_time(text);
            push_output(text,"Cancelled.\n");
            update_progress(progress_lines,0,0);
            root->calculating = FREE;
            root->killthread=0;
            
#if !defined(WIN32)
                return NULL;
    pthread_exit(NULL);
#else
            return NULL;
#endif
        }
        
        
        
        if (root->current->map.seg_turns_array[to_do[i]].exist==1){
            gr_temp = root->current->map.seg_turns_array[to_do[i]];
        }
        else{
            
            
            dijkstra_reference_list_heap(&gr_temp,root->current->map.S_dual_turn,to_do[i],1,root->current->map.links.size(),weight_d,visited_d,1,&(root->current->map));
            
        }
        
        
        
        
        int found=0;
        
        for (unsigned int j=0;j<line_segments.size();j++){
            
            int end_node = line_segments[j];
            
            if (gr_temp.weight[end_node]<1){
                found=1;
            }
        }
        
        if (!found){
            line_segments.push_back(to_do[i]);
            
            for (unsigned j=i+1;j<size;j++){
                if (gr_temp.weight[to_do[j]]==0){
                    to_do.erase(to_do.begin()+j);
                    --size;
                }
            }
        }
        
        if (((double)(i-old_count)>size_out)||(i+1==size)){
            old_count=i;
            Fl::lock();
            text->buff->remove(start_buff,text->buff->length());
            Fl::unlock();
            output.str("");
            output.precision(3);
            output.setf( std::ios::fixed, std:: ios::floatfield );
            output<<"------------------- >> "<<100*((double)(i+1)/(double)(size))<<"% calculated"<<std::endl;
            push_output(text,output.str());
            update_progress(progress_lines,(double)(i+1)/(double)size,0);
            
        }
        
    }

    push_time(text);
    push_output(text,"...done.\n");
    
    std::cout<<"number of lne segments "<<line_segments.size()<<std::endl;
    
    root->current->map.line_segments=line_segments;
    
    
    root->calculating = FREE;
    root->current->lines_valid=1;
    
    Fl::lock();
    turn_ang->activate();
    Fl::unlock();
    
#if !defined(WIN32)
        return NULL;
    pthread_exit(NULL);
#else
    return NULL;
#endif
    
}

