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


#include "desk_metrics.h"


double dist_2d(int& q1,int& q2,std::vector<vec>& points){
    vec p1 = points[q1];
    vec p2 = points[q2];
    
    return sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
    
}

double dist_3d(int& q1,int& q2,std::vector<vec>& points){
    vec p1 = points[q1];
    vec p2 = points[q2];
    
    return sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y)+(p1.z-p2.z)*(p1.z-p2.z));
    
}

double seg_dist_2d(int& q1,int& q2,map_structures* map,std::vector<vec>& points){
    
    vec p1 = points[map->links[q1].start];
    vec p2 = points[map->links[q1].end];
    vec p3 = points[map->links[q2].start];
    vec p4 = points[map->links[q2].end];
    
    
    return sqrt( (0.5*(p1.x+p2.x) - 0.5*(p3.x+p4.x))*(0.5*(p1.x+p2.x) - 0.5*(p3.x+p4.x))   +     (0.5*(p1.y+p2.y) - 0.5*(p3.y+p4.y))*(0.5*(p1.y+p2.y) - 0.5*(p3.y+p4.y)));
    
}

double seg_dist_3d(int& q1,int& q2,map_structures* map,std::vector<vec>& points){
    
    vec p1 = points[map->links[q1].start];
    vec p2 = points[map->links[q1].end];
    vec p3 = points[map->links[q2].start];
    vec p4 = points[map->links[q2].end];
    
    
    return sqrt( (0.5*(p1.x+p2.x) - 0.5*(p3.x+p4.x))*(0.5*(p1.x+p2.x) - 0.5*(p3.x+p4.x))   +     (0.5*(p1.y+p2.y) - 0.5*(p3.y+p4.y))*(0.5*(p1.y+p2.y) - 0.5*(p3.y+p4.y))     +     (0.5*(p1.z+p2.z) - 0.5*(p3.z+p4.z))*(0.5*(p1.z+p2.z) - 0.5*(p3.z+p4.z)));
    
}

void power_iteration (std::vector<double>& eigenvector,graph& G,container* root,double tol,std::vector<double>& e,double alpha){
    
    double acc = 1,acc_old=1;
    double norm;
    double max=-9e99;
    int n = G.neighbours.size();
    
    std::vector<double> temp (n,0);
    std::vector<double> b(n,1);
    std::vector<double> diff(n,0);
    std::vector<double> eigval(n,0);
    
    int start_buff = text->buff->length();
    
    
    clock_t start_time;
    clock_t timer;   //non-boost
    start_time = clock();        
    
    int count=0;
    
    while (acc>tol){
        
        
        count++;      
        
        if (root->killthread){
            return;
        }
        
        norm=0;
        
        
        for (int i=0;i<n;i++){
            temp[i]=0;
            for (int j=0;j<G.neighbours[i].size();j++){
                int k= G.neighbours[i][j];
                temp[i]+=G.weights[i][k]*b[k];
            }
            norm+=temp[i]*temp[i];
        }
        
        
        norm=1.0/sqrt(norm);
        
        max=-9e99;
        
        double maxeigval=-9e99;
        double mineigval=9e99;
        
        for (int i=0;i<n;i++){
                     
            b[i]=norm*temp[i];
           
            
            double ttt =0;
            for (int j=0;j<G.neighbours[i].size();j++){
                int k= G.neighbours[i][j];
                ttt+=G.weights[i][k]*b[k];
                               
            }
            
            eigval[i]=b[i]/ttt;
            if (eigval[i]>maxeigval)maxeigval=eigval[i];
            if (eigval[i]<mineigval)mineigval=eigval[i];
            //end new
            
            
        }
        
        acc = (maxeigval-mineigval)/maxeigval;
        
        acc_old=max;
        
        timer=clock();  //non-boost
        double s = (timer-start_time)/(double)CLOCKS_PER_SEC;
        
        if (s>0.5){
            Fl::lock();
            text->buff->remove(start_buff,text->buff->length());
            Fl::unlock();
        
            std::stringstream s;
            
            s<<"------------------- >> Acceptance tolerance: "<<tol<<std::endl;
            s<<"------------------- >> Fractional difference between minimum and maximum eigenvalue estimates on last iteration: "<<acc<<std::endl;
            push_output(text,s.str());
            start_time=timer;
        }
        
        
    }
    
  
    
    Fl::lock();
    text->buff->remove(start_buff,text->buff->length());
    Fl::unlock();
    
    std::stringstream s;
  
    s<<"------------------- >> Acceptance tolerance: "<<tol<<std::endl;
    s<<"------------------- >> Fractional difference between minimum and maximum eigenvalue estimates on last iteration: "<<acc<<std::endl;
    push_output(text,s.str());
    
    eigenvector=b;
    
    return;
}


std::vector<std::vector<int> > calculate_route(container* root,map_structures* map,int choice,int start, int end,int& dual_marker,int iterations,double &value){
    
    
    graph* route_metric_array;
    std::vector<std::vector<int> > temp;
    
    if ((start<0)||(start>root->current->points.size()-1)||(end<0)||(end>root->current->points.size()-1)){
        push_time(root->buttons.text);
        push_output(root->buttons.text,"No path calculated. Check points.\n");
        return temp;
    }
    
    if (iterations<1){
        push_time(root->buttons.text);
        push_output(root->buttons.text,"Error: need at least one iteration.\n");
        return temp;
    }
    

    int dual=0;
    switch (choice){ //select route determinining
            
        case 0:

            route_metric_array=&(map->G_dist);
            dual=0;
            dual_marker=0;
            break;
        case 1:

            route_metric_array=&(map->G_dist_3D);
            dual=0;
            dual_marker=0;
            break;
        case 2:

            route_metric_array=&(map->G_dual_ang);
            dual=1;
            dual_marker=1;
            break;
        case 3:

            route_metric_array=&(map->G);
            dual=0;
            dual_marker=0;
            break;
        case 4:

            route_metric_array=&(map->G_dual_turn);
            dual=1;
            dual_marker=1;
            break;
        default:
   
            return temp;
            
    }

    
    std::vector<double> weight_d;
    std::vector<int> visited_d;
    
    graph_routes gr;
    dijkstra_reference_list_heap(&gr,*route_metric_array,start,dual,map->links.size(),weight_d,visited_d,0,map);
    value = gr.weight[end];
    
    return_iterate answer;
    iterate_path_reference(&answer,gr.path,gr.mult_path, gr.node_origin,gr.end_node[end],gr.end_node[end],0,map->num_points,iterations,iterations);//metric_array->size());
    
    
    temp = answer.paths;
    if (dual){
        temp.clear();
        for (int i=0;i<answer.paths.size();i++){
            temp.push_back(std::vector<int>(0));
            for (int j=0;j<answer.paths[i].size();j++){
            
                temp[i].push_back(map->dual_points[answer.paths[i][j]].start);
                temp[i].push_back(map->dual_points[answer.paths[i][j]].end);
                                              
            }
        }
    }
    
    
    if (answer.error==1){
        
        push_time(root->buttons.text);
        push_output(root->buttons.text,"Warning: Reached max number of iterations (1000). Not all paths calculated.\n");
        push_time(root->buttons.text);
        push_output(root->buttons.text,"Large numbers of paths or required iterations indicates existance of zero value loops or that the metric is inappropriate. Check paramters.\n");
        
    }
    else if (answer.error==2){
        push_time(root->buttons.text);
        push_output(root->buttons.text,"Warning: Reached max number of iterations (1000). No paths found, but this doesn't mean they do not exist.\n");
        push_time(root->buttons.text);
        push_output(root->buttons.text,"Large numbers of paths or required iterations indicates existance of zero value loops or that the metric is inappropriate. Check paramters.\n");
    }
    
    
    
    if (value>9e98){ //if no path exists
        temp.clear();
    }
    
    return temp;
    
}


std::vector<std::vector<route_weight_number> >* desk_choice_new(std::vector<std::vector<route_weight_number> >* choice_, map_structures* map, int route_choice_metric, int desk_or_point,int iterations,container* root,int& error,int do_cache){

    int seg;
    
    if (desk_or_point==2){        
        seg=1;
    }
    else{
        seg=0;
    }
    
    

    graph* route_metric_array;
    int dual=0,dual1=0;
    
    std::vector<std::vector<route_weight_number> >* choice;
    
    switch (route_choice_metric){ //select route determinining
            
        case 0:

            route_metric_array=&(map->G_dist);
            choice = &(map->choice_cache_G_dist);
            if (desk_or_point==2){

                route_metric_array=&(map->S_dist);
                choice = &(map->choice_cache_S_dist);
            }
            dual=0;
            break;
        case 1:

            route_metric_array=&(map->G_dist_3D);
            choice = &(map->choice_cache_G_dist_3D);
            if (desk_or_point==2){

                route_metric_array=&(map->S_dist_3D);
            choice = &(map->choice_cache_S_dist_3D);
            }
            dual=0;
            break;
        case 2:

            route_metric_array=&(map->G_dual_ang);
            choice = &(map->choice_cache_G_ang);
            if (desk_or_point==2){

                route_metric_array=&(map->S_dual_ang);
                choice = &(map->choice_cache_S_ang);
            }
            dual=1;
            break;
        case 3:

            route_metric_array=&(map->G);
            choice = &(map->choice_cache_G);
            if (desk_or_point==2){

                route_metric_array=&(map->S);
                choice = &(map->choice_cache_S);
            }
            dual=0;
            break;
        case 4:

            route_metric_array=&(map->G_dual_turn);
            choice = &(map->choice_cache_G_turn);
            if (desk_or_point==2){

                route_metric_array=&(map->S_dual_turn);
                choice = &(map->choice_cache_S_turn);
            }
            dual=1;
            break;
            
        default:
            error=1;
            return choice_;
            //error=5;
            //return -1;
            break;
    }
    
    
    if (choice->size()>0){
        return choice;
    }
    else{
        
        if (!do_cache){
            choice = choice_;
        }
        populate_raw(choice,*route_metric_array,seg, dual,map,iterations, error,root->killthread);
    }
    
    return choice;
}




double desk_custom(std::vector<vec>& points,std::vector<location>& desks, map_structures* map, int desk, int dest_choice_metric,int route_choice_metric,int metric, int same_floor, int visible,int stat,int range_option,std::vector<location>* destination_, int &error,int is_desk,double range,int desk_or_point, int self_count_aware,int dest_is_points,int dest_is_lines,int iterations,std::vector<std::vector<std::vector<double> > >* choice,graph_routes& gr_dest,graph_routes& gr_route,std::vector<double>& weight_d,std::vector<int>& visited_d, int inverse,std::vector<std::vector<route_weight_number> >& choice_new,int route_stat, int do_cache){
    
    
    std::vector<location>* destination = destination_; //will this do?
    
    if (dest_is_points==2){
        visible=0;
    }
    
    
    error=0;
    
    
 
    if (!destination->size()){
        std::cout<<"IN HERE "<<destination->size()<<std::endl;
        error=2;
        return -1;
    }
    
    
    
    int dual_dest =0;
    graph* dest_metric_array = NULL;
    std::vector<graph_routes>* dest_choice_graph = NULL; //graph which determines destination selection criteria (crow flies, SP, angular distance etc.)
    
    
    switch (dest_choice_metric){
            
        case 0:
            dest_choice_graph= &(map->distance_array); //<--- just has to be *any* valid array of the right size: not actually used
            if (desk_or_point==2){
                dest_choice_graph= &(map->seg_distance_array); //same again
            }
            dual_dest=0;
            break;
        case 1:
            dest_choice_graph= &(map->distance_array); //<--- just has to be *any* valid array of the right size: not actually used
            if (desk_or_point==2){
                dest_choice_graph= &(map->seg_distance_array); //same again
            }
            dual_dest=0;
            break;
        case 2:
            dest_choice_graph=&(map->distance_array);
            dest_metric_array=&(map->G_dist);
            if (desk_or_point==2){
                dest_choice_graph=&(map->seg_distance_array);
                dest_metric_array=&(map->S_dist);
            }
            dual_dest=0;
            break;
        case 3:
            dest_choice_graph=&(map->distance_3d_array);
            dest_metric_array=&(map->G_dist_3D);
            if (desk_or_point==2){
                dest_choice_graph=&(map->seg_distance_3d_array);
                dest_metric_array=&(map->S_dist_3D);
            }
            dual_dest=0;
            break;
        case 4:
            dest_choice_graph=&(map->angular_array);
            dest_metric_array=&(map->G_dual_ang);
            if (desk_or_point==2){
                dest_choice_graph=&(map->seg_angular_array);
                dest_metric_array=&(map->S_dual_ang);
            }
            dual_dest=1;
            break;
        case 5:
            dest_choice_graph=&(map->segments_array);
            dest_metric_array=&(map->G);
            if (desk_or_point==2){
                dest_choice_graph=&(map->seg_segments_array);
                dest_metric_array=&(map->S);
            }
            dual_dest=0;
            break;
        case 6:
            dest_choice_graph=&(map->turns_array);
            dest_metric_array=&(map->G_dual_turn);
            if (desk_or_point==2){
                dest_choice_graph=&(map->seg_turns_array);
                dest_metric_array=&(map->S_dual_turn);
            }
            dual_dest=1;
            break;
        default:
            std::cout<<"ARE WE IN HERE?"<<std::endl;
            error=5;
            return -1;
    }
    
    

    if ((range_option==0)){
        dest_metric_array=NULL; //explicitly don't need to calculate metric for destinations if we dont use a destination metric constraint

    }
    else{
        
        if (!desk_or_point){ //desk based
            if (((*dest_choice_graph)[desks[desk].point].exist==1)&&(dest_choice_metric>1)){
                gr_dest = (*dest_choice_graph)[desks[desk].point];
            }
            else{
                
                if (dest_choice_metric==0){
                    
                    int point = desks[desk].point;
 
                    int n = map->num_points;
                    
                    
                    gr_dest.weight.resize(n);
                    for (int i=0;i<n;i++){
                        
                        gr_dest.weight[i]= dist_2d(point,i,points);
                        
                        
                    }
                }
                else if (dest_choice_metric==1){
                    
                    int point = desks[desk].point;
                    
                    int n = map->num_points;
                    
                    
                    gr_dest.weight.resize(n);
                    for (int i=0;i<n;i++){
                        
                        gr_dest.weight[i]=dist_3d(point,i,points);
                        
                        
                    }
                }
                else{
                
                    dijkstra_reference_list_heap(&gr_dest,*dest_metric_array,desks[desk].point,dual_dest,map->links.size(),weight_d,visited_d,0,map);
                }
                
                if (do_cache){
                    gr_dest.exist=1;
                    (*dest_choice_graph)[desks[desk].point]=gr_dest;
                }
            }
        }
        else{//point or segment based
           
            
            if (((*dest_choice_graph)[desk].exist==1)&&(dest_choice_metric>1)){
                gr_dest = (*dest_choice_graph)[desk];
            }
            else{
                
                if (dest_choice_metric==0){
                    
                    int point =desk;
                    
                    int n = map->num_points;
                    if (desk_or_point==2){
                        n=map->num_links;
                    }
                    
                    gr_dest.weight.resize(n);
                    for (int i=0;i<n;i++){
                        if (desk_or_point==2){
                            gr_dest.weight[i]=seg_dist_2d(point,i,map,points);
                        }
                        else{
                            gr_dest.weight[i]=dist_2d(point,i,points);
                        }
                        
                    }
                }
                else if (dest_choice_metric==1){
                    
                    int point =desk;
                    
                    int n = map->num_points;
                    if (desk_or_point==2){
                        n=map->num_links;
                    }
                    
                    gr_dest.weight.resize(n);
                    for (int i=0;i<n;i++){
                        if (desk_or_point==2){
                            gr_dest.weight[i]=seg_dist_3d(point,i,map,points);
                        }
                        else{
                            gr_dest.weight[i]=dist_3d(point,i,points);
                        }
                        
                        
                    }
                }
                else{
                    dijkstra_reference_list_heap(&gr_dest,*dest_metric_array,desk,dual_dest,map->links.size(),weight_d,visited_d,desk_or_point-1,map);
                }
                
                if (do_cache){
                    gr_dest.exist=1;
                    (*dest_choice_graph)[desk]=gr_dest;
                }
                
             
            }
            
            
        }
    }
    
    std::vector<int> destinations,destination_points;
    
    for (unsigned int i=0;i<(*destination).size();i++){ //generate list of valid destinations
        
        int ok = 1;
        
        if (self_count_aware>0){ // ie if self_count_aware = 1 (NO SELF COUNTING AT ALL)  or self_count_aware = 2 (FORCE POINT IN REGARDLESS (IE SKIP SELECTION); ADDED LATER)
                                    // self_count_aware = 0 (SELF COUNT IF PASSES SELECTION SO MUST BE INCLUDED HERE)
            if (desk_or_point==2){
                
                if (dest_is_lines){
                    
                    graph_routes temp;
                    
                    if (map->seg_turns_array[desk].exist==1){
                        temp = map->seg_turns_array[desk];
                    }
                    else{
                        
                        dijkstra_reference_list_heap(&temp,map->S_dual_turn,desk,1,map->links.size(),weight_d,visited_d,desk_or_point-1,map);
                        
                        if (do_cache){
                            temp.exist=1;
                            map->seg_turns_array[desk]=temp;
                        }
                    }
                    
                    if (temp.weight[(*destination)[i].point]==0){
                        ok=0;
                    }
                    
                }
                else if (desk==i){
                    ok=0;
                }
            }
            else if (desk_or_point==1){
                if ((desk==i)&&(dest_is_points)){
                    ok=0;
                }
                
            }
            else if(!desk_or_point){
                if (is_desk){
                    if (desk == (int) i){
                        ok=0;
                    }
                }
                else if(dest_is_points){
                    if (desks[desk].point==(*destination)[i].point){
                        ok=0;
                    }
                }
            }
        
        }
        
       
        
        if (ok){
            int floor=0;
            int floor1=(*destination)[i].loc.floor;
 
            if (desk_or_point==0){
                floor=desks[desk].loc.floor;
            }
            else if (desk_or_point==1){
                floor=points[desk].floor;
            }
            else if (desk_or_point==2){
                
                int p1=map->links[desk].start;
                int p2=map->links[desk].end;

                int p3=map->links[(*destination)[i].point].start;
                int p4=map->links[(*destination)[i].point].end;
                
                if ((points[p1].floor)==(points[p2].floor)){
                    floor=(points[p1].floor);
                }
                else{
                    floor=-1;
                }
                
                if ((points[p3].floor)==(points[p4].floor)){
                    floor1=(points[p3].floor);
                }
                else{
                    floor1=floor-1;
                }
                
              
            }
                                                
            if (((same_floor)&&(floor==floor1))||(!same_floor)){ //same floor or any floor
                std::vector<int> vis_mat;
                
                if (visible==1){
                    vis_mat=(*destination)[i].vis_standing;
                }
                else if (visible==2){
                    vis_mat=(*destination)[i].vis_sitting;
                }
                else if (visible==3){
                    vis_mat=(*destination)[i].vis_standing_trans;
                }
                else if (visible==4){
                    vis_mat=(*destination)[i].vis_sitting_trans;
                }
                else if (visible==0){
                    std::vector<int> temp(desks.size(),1);
                    vis_mat=temp;
                }
                
                
                
                if (dest_is_points){
                    std::vector<int> temp(desks.size(),1);
                    vis_mat=temp;
                }
                
                if (desk_or_point==1){
                    std::vector<int> temp(points.size(),1);
                    vis_mat=temp;
                }
                else if (desk_or_point==2){
                    std::vector<int> temp( map->links.size(),1);
                    vis_mat=temp;
                }
                
                
                if ((!desk_or_point)&&(desks.size()!=vis_mat.size())){
                    
                    error=3;
                    return -1;
                }
                
                
                if ((vis_mat[desk])){  //if visible or no constraint
                    
                    
                    if ((range_option==3)||(range_option==5)){   //if within range
                        
                        
                        
                        if (gr_dest.weight[(*destination)[i].point]<=range){
                           
                            
                            destinations.push_back((*destination)[i].point);
                            
                        }
                        
                    }
                    else if ((range_option==4)||(range_option==6)){ //if beyond range
                        
                        if (gr_dest.weight[(*destination)[i].point]>=range){
                            
                            destinations.push_back((*destination)[i].point);
                        }
                    }
                    else if (range_option==7){
                        
                        if (equal(gr_dest.weight[(*destination)[i].point],range)){

                            destinations.push_back((*destination)[i].point);
                        }
                        
                    }
                    else{    //no range constraint
                        
                        destinations.push_back((*destination)[i].point);
                        
                    }
                    
                }
            }
        }
    }

    
    
    // multiple locations at a single point means severl visibility points for one location: if we have several destinations with the same point they need to be counted as a single destination (only the visible ones have passed the test to get to this point) so we remove ones which have the same point more than once.
    
    
    if ((desk_or_point<2)&&((!is_desk)&&(!dest_is_points)&&(!dest_is_lines))){  //if not desks limit to one destination per point
        
        std::vector<int> found_points,to_delete;
        found_points.clear();
        to_delete.clear();
        
        for (unsigned int i=0;i<destinations.size();i++){
    
            
            int found=0;
            for (unsigned int j=0;j<found_points.size();j++){
             
                
                if (destinations[i]==found_points[j]){
                    found=1;
                    to_delete.push_back(i);
                    break;
                }
                
            }
            if(!found){
                
                found_points.push_back(destinations[i]);
            }
        }
        
        for (int i=(int)to_delete.size()-1;i>=0;i--){

            destinations.erase(destinations.begin()+to_delete[i]);
            
        }
        
        
    }
    
    
    //usage criteria
    
        
    if ((range_option==1)||(range_option==2)||(range_option==5)||(range_option==6)){ //minimiseing or maximising
        
        double min=9e99,max=-9e99;
        int ii=0;
        for (unsigned int i=0;i<destinations.size();i++){
            
            
            double temp = gr_dest.weight[destinations[i]];
            
            if ((temp<min)&&(temp>=0)){
                min=temp;
                ii=i;
            }
            
            if ((temp>max)&&(temp>=0)){
                max=temp;
            }
            
        }
        
        for (unsigned int i=0;i<destinations.size();i++){
           
            
            if ((range_option==1)||(range_option==6)){
                if (equal(gr_dest.weight[destinations[i]],min)){
                    destination_points.push_back(destinations[i]);
                }
                
            }
            else if ((range_option==2)||(range_option==5)){
                if (equal(gr_dest.weight[destinations[i]],max)){
                    destination_points.push_back(destinations[i]);
                }
            }
            
        }
        
    }
    else //alll others just use all valid selected locations
    {
        
        for (unsigned int i=0;i<destinations.size();i++){
            
            
            destination_points.push_back(destinations[i]);
            
            
        }
    }
   
    
    //end usage_criteria
    
    
    //start force self include
    if (self_count_aware==2){
        int ii =desk;
        if (!desk_or_point){
            ii=desks[desk].point;
        }
        
        
        
        if (desk_or_point==2){
            
            if (dest_is_lines){
                
                graph_routes temp;
                
                if (map->seg_turns_array[desk].exist==1){
                    temp = map->seg_turns_array[desk];
                }
                else{
                    
                    dijkstra_reference_list_heap(&temp,map->S_dual_turn,desk,1,map->links.size(),weight_d,visited_d,1,map);
                    
                    if (do_cache){
                        temp.exist=1;
                        map->seg_turns_array[desk]=temp;
                    }
                }
                
                
                for (int i=0;i<destination->size();i++){
                    if (temp.weight[(*destination)[i].point]==0){
                        ii=i;
                    }
                }
                               
                
            }
            
        }


        destination_points.push_back(ii);
        
        
        
    }
    //end force self include
    
    
    
    
    if (metric==7){
        
        if (inverse){
            
            if (destination_points.size()==0){
                return -1;
            }
            
            return 1.0/(double)destination_points.size();
        }
        
        return destination_points.size();
        
    }
    else if (metric==9){
        
        if (destination_points.size()==0){
            error=4; //no valid metric; no valid destinations
            return -1;
        }
        
        int ii=desk;
        
        if (desk_or_point==0){
            ii=desks[desk].point;
        }
        
        double sum=0;
        
       
        
        
        
        int dual_between;
        
        switch (route_choice_metric){ //select route determinining
                
            case 0:
                dual_between=0;
                break;
            case 1:
                dual_between=0;
                break;
            case 2:
                dual_between=1;
                break;
            case 3:
                dual_between=0;
                break;
            case 4:
                dual_between=1;
                break;
            default:
                error=5;
                return -1;
        }
        
        
        int point = ii;
        double s=0;
        // start not dual
        
        
        
        if (!dual_between){ //points or segs normal
            
            for (int i=0;i<destination_points.size();i++){
                for (int j=i+1;j<destination_points.size();j++){
                    
                    int p1 = destination_points[i];
                    int p2 = destination_points[j];
                    
                    
                    if (equal(choice_new[p1][p2].weight, (choice_new[p1][point].weight + choice_new[point][p2].weight))){
                        
                        s+=( (double)choice_new[p1][point].number * (double)choice_new[point][p2].number );
                        
                    }
                }
            }                        
            
        }
        else{ //dual graph (ie with direction)
            
            
            
            if (desk_or_point==2){ //directed segment based
                
                std::vector<int> point_lines;
                point_lines.push_back(desk);
                point_lines.push_back(desk+map->num_links);
                point_lines.push_back(desk+2*map->num_links);
                
                
                for (int i=0;i<destination_points.size();i++){
                    for (int j=i+1;j<destination_points.size();j++){
                        
                        
                        
                        int p1 = destination_points[i]+2*map->links.size();
                        int p2 = destination_points[j]+2*map->links.size();
                        
                        double sum=0;
                        
                        for (int k=0;k<point_lines.size();k++){
                            
                            point=point_lines[k];
                            
                            
                            if (equal(choice_new[p1][p2].weight, (choice_new[p1][point].weight + choice_new[point][p2].weight))){
                                
                                if ((point<2*map->num_links)||((point==p1)||(point==p2))){
                                    
                                    s +=((double) choice_new[p1][point].number * (double) choice_new[point][p2].number ) ;
                                    
                                }
                                
                            }
                            
                            
                        }
                    }
                    
                    
                }
                
                
            }
            else{ //directed point based (most complicated
                
                
                std::vector<int> point_lines;
                
                for (int i=0;i<map->dual_points.size();i++)
                {
                    
                    if ((map->dual_points[i].start==desk)||(map->dual_points[i].end==desk)){
                        point_lines.push_back(i);
                        
                    }
                    
                }
                
                
                
                
                for (int i=0;i<destination_points.size();i++){
                    for (int j=i+1;j<destination_points.size();j++){
                        
                        
                        
                        int p1 = destination_points[i]+2*map->links.size();
                        int p2 = destination_points[j]+2*map->links.size();
                        
                        double sum=0;
                        
                        for (int k=0;k<point_lines.size();k++){
                            
                            point=point_lines[k];
                            
                            
                            if (equal(choice_new[p1][p2].weight, (choice_new[p1][point].weight + choice_new[point][p2].weight))){
                                
                                if ((point<2*map->num_links)||((point==p1)||(point==p2))){ //can't count source/sink segments which aren't the source/sink of the destinations i and j
                                    
                                    s +=0.5*((double) choice_new[p1][point].number * (double) choice_new[point][p2].number );
                                    //has to be 0.5* as each path will go through the point twice (for preceded and following segment)
                                }
                                
                            }
                            
                            
                        }
                    }
                    
                    
                }
                
                
                
            }
        }
        
      
        
        
        /* //ARGH NEED TO REMOVE THIS!!!!!
         if (same_floor){
         return sum;
         }
         */
        
        if (inverse){
            
            if (equal(s,0)){
                return -1;
            }
            return 1.0/s;
        }
        else{
            return s;
        }
        
        
        
    }
    else if (metric==10){
        
        if (destination_points.size()==0){
            error=4; //no valid metric; no valid destinations
            return -1;
        }
        
        int ii=desk;
        
        if (desk_or_point==0){
            ii=desks[desk].point;
        }
        
        double sum=0;
       
        
        int dual_between;
        
        switch (route_choice_metric){ //select route determinining
                
            case 0:
                dual_between=0;
                break;
            case 1:
                dual_between=0;
                break;
            case 2:
                dual_between=1;
                break;
            case 3:
                dual_between=0;
                break;
            case 4:
                dual_between=1;
                break;
            default:
                error=5;
                return -1;
        }
                        
        
        int point = ii;
        double s=0;
        // start not dual
        

        
        if (!dual_between){ //points or segs normal
            
            for (int i=0;i<destination_points.size();i++){
                for (int j=i+1;j<destination_points.size();j++){
                    
                    int p1 = destination_points[i];
                    int p2 = destination_points[j];
                    
                    
                    if (equal(choice_new[p1][p2].weight, (choice_new[p1][point].weight + choice_new[point][p2].weight))){
                        
                        s+=( (double)choice_new[p1][point].number * (double)choice_new[point][p2].number ) / (double)choice_new[p1][p2].number;
                        
                    }                                    
                }
            }
            
           
            
        }
        else{ //dual graph (ie with direction)
            
            
            
            if (desk_or_point==2){ //directed segment based
                
                std::vector<int> point_lines;
                point_lines.push_back(desk);
                point_lines.push_back(desk+map->num_links);
                point_lines.push_back(desk+2*map->num_links);
                
                
                for (int i=0;i<destination_points.size();i++){
                    for (int j=i+1;j<destination_points.size();j++){
                        
                        
                        
                        int p1 = destination_points[i]+2*map->links.size();
                        int p2 = destination_points[j]+2*map->links.size();
                        
                        double sum=0;
                        
                        for (int k=0;k<point_lines.size();k++){
                            
                            point=point_lines[k];
                            
                            
                            if (equal(choice_new[p1][p2].weight, (choice_new[p1][point].weight + choice_new[point][p2].weight))){
                                
                                if ((point<2*map->num_links)||((point==p1)||(point==p2))){
                                    
                                    s +=((double) choice_new[p1][point].number * (double) choice_new[point][p2].number ) / ( (double)choice_new[p1][p2].number );
                                    
                                }
                                
                            }
                            
                            
                        }
                    }
                    
                    
                }
                
                
            }
            else{ //directed point based (most complicated
                
                
                std::vector<int> point_lines;

                for (int i=0;i<map->dual_points.size();i++)
                {                   
                    
                    if ((map->dual_points[i].start==desk)||(map->dual_points[i].end==desk)){
                        point_lines.push_back(i);
                        
                    }

                }
                
          
                
                
                for (int i=0;i<destination_points.size();i++){
                    for (int j=i+1;j<destination_points.size();j++){
                        
                        
                        
                        int p1 = destination_points[i]+2*map->links.size();
                        int p2 = destination_points[j]+2*map->links.size();
                        
                        double sum=0;
                        
                        for (int k=0;k<point_lines.size();k++){
                            
                            point=point_lines[k];
                            
                            
                            if (equal(choice_new[p1][p2].weight, (choice_new[p1][point].weight + choice_new[point][p2].weight))){
                                
                                if ((point<2*map->num_links)||((point==p1)||(point==p2))){ //can't count source/sink segments which aren't the source/sink of the destinations i and j
                                    
                                    s +=0.5*((double) choice_new[p1][point].number * (double) choice_new[point][p2].number ) / ( (double)choice_new[p1][p2].number );                                    
                                    //has to be 0.5* as each path will go through the point twice (for preceded and following segment)
                                }
                                
                            }
                            
                            
                        }
                    }
                    
                  
                }
               
                
                
            }
        }
        
        s/=(0.5*(double)destination_points.size()*(double)(destination_points.size()-1));
        
       /* //ARGH NEED TO REMOVE THIS!!!!!
        if (same_floor){
            return sum;
        }
        */
        
        if (inverse){
            
            if (equal(s,0)){
                return -1;
            }
            return 1.0/s;
        }
        else{
            return s;
        }
        
        
        
    }
    else if (metric==11){
        
        if (destination_points.size()<2){
            error=4; //no valid metric; no valid destinations
            //return -1;
        }
    
        
        std::vector<int> include(points.size(),0);
        
        if (desk_or_point==2){
            include.resize(map->links.size(),0);
        }
        
        
        int point=desk;
        
        if (desk_or_point==0){
            point=desks[desk].point;
        }
        
        
        
        //new
        
        int dual_between;
        
        switch (route_choice_metric){ //select route determinining
                
            case 0:
                dual_between=0;
                break;
            case 1:
                dual_between=0;
                break;
            case 2:
                dual_between=1;
                break;
            case 3:
                dual_between=0;
                break;
            case 4:
                dual_between=1;
                break;
            default:
                error=5;
                return -1;
        }
        
        
        if (!dual_between){
            
            for (int i=0;i<destination_points.size();i++){
                for (int j=i+1;j<destination_points.size();j++){
                    
                    int p1 = destination_points[i];
                    int p2 = destination_points[j];
                    
                    for (int point = 0;point<include.size();point++){
                    
                    
                        if (equal(choice_new[p1][p2].weight, (choice_new[p1][point].weight + choice_new[point][p2].weight))){
                        
                            include[point]=1;
                    
                        }
                    }
                }
            }
            
            
        }
        else{
             
            
            
            if (desk_or_point==2){
                                
                for (int i=0;i<destination_points.size();i++){
                    for (int j=i+1;j<destination_points.size();j++){
                        
                        
                        int p1 = destination_points[i]+2*map->links.size();
                        int p2 = destination_points[j]+2*map->links.size();
                        
                        
                        
                        for (int point=0;point<2*include.size();point++){
                            
                            if (equal(choice_new[p1][p2].weight, (choice_new[p1][point].weight + choice_new[point][p2].weight))){
                                
                                int pp = point;
                                
                                if (pp>map->links.size()){
                                    pp-=map->links.size();
                                }
                                
                                include[pp]=1;
                            }
                            
                            
                        }
                    }
                    
                    
                }
                
                
            }
            else{                                                
                
                for (int i=0;i<destination_points.size();i++){
                    for (int j=i+1;j<destination_points.size();j++){
                        
                        
                        
                        int p1 = destination_points[i]+2*map->links.size();
                        int p2 = destination_points[j]+2*map->links.size();
                        
                       
                        
                        for (int point=0;point<2*map->links.size();point++){
                    
                            
                            
                            if (equal(choice_new[p1][p2].weight, (choice_new[p1][point].weight + choice_new[point][p2].weight))){
                                
                                int pp1 = map->dual_points[point].start;
                                int pp2 = map->dual_points[point].end;
                                
                                include[pp1]=1;
                                include[pp2]=1;
                                
                            }
                            
                            
                        }
                    }
                    
                    
                }
                
                
                
            }
        }
        
        
        double sum=0;        
        
        if (desk_or_point==2){
            for (int i=0;i<include.size();i++){
                if (!include[i]){
                    continue;
                }
                
                double temp;
                
                int p1 = map->links[i].start;
                int p2 = map->links[i].end;
                
                temp = map->G_dist_3D.weights[p1][p2];
                
                if (temp>0){
                    sum+=temp;
                }
            }
        }
        else{
            
            for (int i=0;i<include.size();i++){
                if (!include[i]){
                    continue;
                }
                for (int j=i+1;j<include.size();j++){
                    if (include[j]){
                        double temp;
                        
                        temp = map->G_dist_3D.weights[i][j];
                        
                        
                        if (temp>0){
                            sum+=temp;
                        }
                    }
                }
            }
        }
        
        if (inverse){
            if (equal(sum,0)){
                return -1;
            }
            
            return 1.0/sum;
        }
        
        return sum;
        
    }

    
    
    
    
    
    
            
    
    std::vector<graph_routes>* route_graph = NULL;
    graph* route_metric_array = NULL;
    int dual=0,dual1=0;
    
    switch (route_choice_metric){ //select route determinining
            
        case 0:
            route_graph=&(map->distance_array);
            route_metric_array=&(map->G_dist);
            if (desk_or_point==2){
                route_graph=&(map->seg_distance_array);
                route_metric_array=&(map->S_dist);
            }
            dual=0;
            break;
        case 1:
            route_graph=&(map->distance_3d_array);
            route_metric_array=&(map->G_dist_3D);
            if (desk_or_point==2){
                route_graph=&(map->seg_distance_3d_array);
                route_metric_array=&(map->S_dist_3D);
            }
            dual=0;
            break;
        case 2:
            route_graph=&(map->angular_array);
            route_metric_array=&(map->G_dual_ang);
            if (desk_or_point==2){
                route_graph=&(map->seg_angular_array);
                route_metric_array=&(map->S_dual_ang);
            }
            dual=1;
            break;
        case 3:
            route_graph=&(map->segments_array);
            route_metric_array=&(map->G);
            if (desk_or_point==2){
                route_graph=&(map->seg_segments_array);
                route_metric_array=&(map->S);
            }
            dual=0;
            break;
        case 4:
            route_graph=&(map->turns_array);
            route_metric_array=&(map->G_dual_turn);
            if (desk_or_point==2){
                route_graph=&(map->seg_turns_array);
                route_metric_array=&(map->S_dual_turn);
            }
            dual=1;
            break;
        
        default:
            error=5;
            return -1;
    }
    

    
    if (route_metric_array==dest_metric_array){ //already calculated
        gr_route= gr_dest;
    }
    else{

        
        
        if (!desk_or_point){
            if ((*route_graph)[desks[desk].point].exist==1){
                gr_route = (*route_graph)[desks[desk].point];
            }
            else{

                
                dijkstra_reference_list_heap(&gr_route,*route_metric_array,desks[desk].point,dual,map->links.size(),weight_d,visited_d,0,map);
                
                if (do_cache){
                    gr_route.exist=1;
                    (*route_graph)[desks[desk].point]=gr_route;
                }
            }
        }
        else{
            if ((*route_graph)[desk].exist==1){
                gr_route = (*route_graph)[desk];
            }
            else{
                
                dijkstra_reference_list_heap(&gr_route,*route_metric_array,desk,dual,map->links.size(),weight_d,visited_d,desk_or_point-1,map);
                
                if (do_cache){
                    gr_route.exist=1;
                    (*route_graph)[desk]=gr_route;
                }
            }
        }
    
    }
    

    
    
    graph* metric_array;
    
    switch (metric){ //select actual metric
            
        case 0:
            
            dual1=0;
            break;
        case 1:
            
            dual1=0;
            break;
        case 2:
            metric_array=&(map->G_dist);
            if (desk_or_point==2){
                metric_array=&(map->S_dist);
            }
            dual1=0;
            break;
        case 3:
            metric_array=&(map->G_dist_3D);
            if (desk_or_point==2){
                metric_array=&(map->S_dist_3D);
            }
            dual1=0;
            break;
        case 4:
            metric_array=&(map->G_dual_ang);
            if (desk_or_point==2){
                metric_array=&(map->S_dual_ang);
            }
            dual1=1;
            break;
        case 5:
            metric_array=&(map->G);
            if (desk_or_point==2){
                metric_array=&(map->S);
            }
            dual1=0;
            break;
        case 6:
            metric_array=&(map->G_dual_turn);
            if (desk_or_point==2){
                metric_array=&(map->S_dual_turn);
            }
            dual1=1;
            break;
        case 8: break;
        default:
            error=5;
            return -1;
    }
    
   
    
    
    int start_node;
    
     if (!desk_or_point){
     start_node= desks[desk].point;
     }
     else{
        start_node=desk;
     }
     
    
    
    std::vector<double> sum_tot;
    
    for (unsigned int i=0;i<destination_points.size();i++){ //run through all valid destinations (might only be one if the minimum)
        
        
        int end_node = destination_points[i];
        
        
        if (metric==0){//euclidian distance xy
        
            if (desk_or_point==2){
            
               
                sum_tot.push_back(seg_dist_2d(start_node,end_node,map,points));//map->crow_array[start_node].weight[end_node];
                
            }
            else{

               
                sum_tot.push_back(dist_2d(start_node,end_node,points));//map->crow_array[start_node].weight[end_node];
                
            }
           
        }
        else if (metric==1){ //euclidian distance xyz
            if (desk_or_point==2){
                
               
                sum_tot.push_back(seg_dist_3d(start_node,end_node,map,points));//map->crow_array_3D[start_node].weight[end_node];
                
            }
            else{
               
                sum_tot.push_back(dist_3d(start_node,end_node,points));//map->crow_array_3D[start_node].weight[end_node];
                
            }
        }
        else{ //need to find graph based metric
            
            if (metric-2==route_choice_metric){ //route matches metric type so just look up value
                                
                
                sum_tot.push_back(gr_route.weight[end_node]); //no need to iterate path: can just pick out the weight

                
            }
            else if (gr_route.weight[end_node]>9e98){ // no valid route between the points; value of metric 'infinite'
                

                sum_tot.push_back(INFINITY);//ie infinity...
                
                
            }
            else{ //need to calculate path and count metric value along path
                
                
                return_iterate answer;
                iterate_path_reference(&answer,gr_route.path,gr_route.mult_path, gr_route.node_origin,gr_route.end_node[end_node],gr_route.end_node[end_node],0,map->num_points,iterations,iterations);
               
                
                if (answer.error==1){ //reached max iterations but have found a path
                    
                    error=1;
               
                }
                else if (answer.error==2){ //reached max iterations and no valid path (ie one with no loops); try again...
                    error=7;
                    return -1;
                }
                
                
                if (metric==8){//if number of possible paths
                    
                    if (gr_route.weight[end_node]>9e98){
                        
                        sum_tot.push_back(0);
                        
                    }
                    else{
                       
                        
                        sum_tot.push_back(answer.paths.size());
                        
                    }
                }
                else{
                    
                    
                    
                    std::vector<double> path_values;
                                       
                    
                    if (desk_or_point==2){
                        for (int j=0;j<answer.paths.size();j++){
                            double sum=0;
                            for (int jj=0;jj<answer.paths[j].size()-1;jj++){

                                
                                if (!dual1){//metric
                                   
                                    
                                    int seg_old,seg_new;
                                    
                                    seg_old=answer.paths[j][jj];
                                    seg_new=answer.paths[j][jj+1];                                                                    
                                    
                                    if (dual){
                                        if (seg_old>=2*map->links.size()){
                                            seg_old=-1;
                                        }
                                        else if (seg_old>=map->links.size()){
                                            seg_old-=map->links.size();
                                        }
                                        
                                        if (seg_new>=2*map->links.size()){
                                            seg_new=-1;
                                        }
                                        else if (seg_new>=map->links.size()){
                                            seg_new-=map->links.size();
                                        }
                                        
                                    }
                                    
                                    
                                    if (((seg_old)>-1)&&((seg_new)>-1)){
                                        double temp=(*metric_array).weights[seg_old][seg_new];
                                        if (temp>=0){
                                            sum+=temp;
                                        }
                                    }
                                }
                                else{ //metric is dual
                                    
                                    int seg_old=answer.paths[j][jj];
                                    int seg_new= answer.paths[j][jj+1];
                                    
                                    if (!dual){ //route
                                    
                                        
                                        if (map->links[seg_old].start==map->links[seg_new].start){
                                            seg_new+=map->links.size();
                                        }
                                        else if (map->links[seg_old].end==map->links[seg_new].end){
                                            seg_old+=map->links.size();
                                        }
                                        else if (map->links[seg_new].start==map->links[seg_old].end){
                                            int temp=seg_old;
                                            seg_old=seg_new;
                                            seg_new=temp;
                                        }                                                                              
                                        
                                    }
                                    
                                    
                                    if (((seg_old)>-1)&&((seg_new)>-1)){
                                        double temp=(*metric_array).weights[seg_new][seg_old];
                                        if (temp>=0){
                                            sum+=temp;
                                        }
                                    }
                                    
                                    
                                }
                                
                            }
                            
                            
                            path_values.push_back(sum);
                            
                        }
                    }
                    else{
                        
                        for (unsigned int j=0;j<answer.paths.size();j++){
                            
                            double sum=0;
                            
                            if ((dual)&&(dual1)){
                                
                                for (unsigned int jj=0;jj<answer.paths[j].size();jj++){
                                    
                                    if (jj<answer.paths[j].size()-1){
                                        double temp=(*metric_array).weights[answer.paths[j][jj+1]][answer.paths[j][jj]];
                                        if (temp>=0){
                                            sum+=temp;
                                        }
                                    }
                                    
                                }
                                
                                
                            }
                            
                            if ((dual)&&(!dual1)){
                                
                                for (unsigned int jj=0;jj<answer.paths[j].size();jj++){
                                    
                                    int i1,i2;
                                    
                                    i1=map->dual_points[answer.paths[j][jj]].start;
                                    i2=map->dual_points[answer.paths[j][jj]].end;
                                    
                                    double temp=(*metric_array).weights[i1][i2];
                                    
                                    if (temp>=0){
                                        sum+=temp;
                                    }
                                }
                                
                            }
                            
                            if ((!dual)&&(dual1)){
                                
                                for (unsigned int jj=0;jj<answer.paths[j].size();jj++){
                                    
                                    
                                    
                                    if ((int)jj<((int)(answer.paths[j].size())-2)){
                                        
                                        int i1,i2;
                                        
                                        vertex temp1, temp2;
                                        
                                        temp1.start = answer.paths[j][jj+1];
                                        temp1.end = answer.paths[j][jj];
                                        
                                        temp2.start = answer.paths[j][jj+2];
                                        temp2.end = answer.paths[j][jj+1];
                                        
                                        i1 = map->points2dual[temp1];
                                        i2 = map->points2dual[temp2];
                                        
                                        double temp=(*metric_array).weights[i2][i1];
                                        
                                        if (temp>=0){
                                            sum+=temp;
                                        }
                                        
                                    }
                                    
                                }
                                
                            }
                            
                            if ((!dual)&&(!dual1)){
                                
                                for (unsigned int jj=0;jj<answer.paths[j].size();jj++){
                                    
                                    if ((int)jj<(int)answer.paths[j].size()-1){
                                        
                                        double temp=(*metric_array).weights[answer.paths[j][jj+1]][answer.paths[j][jj]];
                                        
                                        if (temp>=0){
                                            sum+=temp;
                                        }
                                        
                                    }
                                    
                                }
                                
                                
                            }
                            
                            
                            path_values.push_back(sum);
                            
                            
                        }
                    }
                    
                    
                    if (route_stat==0){
                    
                    
                    // if lots of different paths with different metric values need to distinguish between them
                        double min=9e99;
                        for (unsigned int j=0;j<path_values.size();j++){ //if there are many paths we choose the path with the minimum metric
                        
                            if (path_values[j]<min){
                                min=path_values[j];
                            }
                            
                        }
                    
                    
                        sum_tot.push_back(min); //running total
                        
                    }
                    else if (route_stat==1){
                        double max=-9e99;
                        for (unsigned int j=0;j<path_values.size();j++){ //if there are many paths we choose the path with the maximum metric
                            
                            if (path_values[j]>max){
                                max=path_values[j];
                            }
                            
                        }
                        
                        
                        sum_tot.push_back(max); //running total
                    }
                    else{
                        double sum=0;
                        for (unsigned int j=0;j<path_values.size();j++){ //if there are many paths we use the mean
                            
                            sum+=path_values[j];
                        
                        }
                        sum/=(double)path_values.size();
                        
                        sum_tot.push_back(sum);
                        
                    }
                }
                
            }
            
            
            if (sum_tot[sum_tot.size()-1]>9e98){
                error=6;
            }
            
        }
        
       
    }
    
   
    if (destination_points.size()==0){
        error=4; //no valid metric; no valid destinations
        return -1;
    }
    else{
    
        double return_val=0;
        
       
        if (stat==0){
            
            double min=9e99;
            
            for (int i=0;i<sum_tot.size();i++){
                if (sum_tot[i]<min){
                    min=sum_tot[i];
                }
            }
            
            return_val = min;
            
        }
        else if (stat==1){
            double max=-9e99;
            
            for (int i=0;i<sum_tot.size();i++){
                if (sum_tot[i]>max){
                    max=sum_tot[i];
                }
                
               
            }
            
            return_val = max;
        }
        else if (stat==2){
            
            double sum=0;
            for (int iii=0;iii<sum_tot.size();iii++){
                sum+=sum_tot[iii];
            }
            return_val=sum/(double)sum_tot.size();
            
            
        }
        else if (stat==3){
            std::sort(sum_tot.begin(),sum_tot.end());
            
            
            double median;
            size_t size = sum_tot.size();
            
            
            
            if (size  % 2 == 0)
            {
                median = (sum_tot[size / 2 - 1] + sum_tot[size / 2]) / 2;
            }
            else 
            {
                median = sum_tot[size / 2];
            }
            
            return_val = median;
            
            
        }
        else if (stat==4){//geometric mean
            double sum=1.0;
            for (int iii=0;iii<sum_tot.size();iii++){
                sum+=log(sum_tot[iii]);
            }
            sum=sum/(double)sum_tot.size();
            sum=exp(sum);
            return_val=sum;
        }
        else if (stat==5){//harmonic mean
            
            double sum=0;
            for (int iii=0;iii<sum_tot.size();iii++){
                sum+=1.0/sum_tot[iii];
            }
            
            
            if (sum<9e99){
                sum = (double)sum_tot.size()/sum;
            }
            else{
                sum=0;
            }
            
            return_val=sum;
            
        }
        else if (stat==6){
            
            double mean=0;
            for (int iii=0;iii<sum_tot.size();iii++){
                mean+=sum_tot[iii];
            }
            mean/=(double)sum_tot.size();
            
            double var=0;
            
            for (int iii=0;iii<sum_tot.size();iii++){
                var+=(sum_tot[iii]-mean)*(sum_tot[iii]-mean);
            }
            
            var/=(double)sum_tot.size();
            return_val= var;
        }
        
        
        
        if (inverse){
            
            if (equal(return_val,0)){
                return -1;
            }
            return_val=1.0/return_val;
        }
        
        return return_val;
    }
    
}

