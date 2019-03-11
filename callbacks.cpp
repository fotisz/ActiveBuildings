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

#include "callbacks.h"



#define SHOW_METRIC_INDEX 54
#define SHOW_ROUTE_INDEX 53

void close_route_cb(Fl_Widget*,void*){
    route_display->hide();
}

void close_link_win(Fl_Widget*,void*){
    add_link_win->hide();
}

void close_license(Fl_Widget*,void* ){
    
    license_win->hide();
}
void open_license(Fl_Widget*,void* ){
    license_win->show();
    license_win->resize(win->x()+0.5*win->w()-265,win->y()+0.5*win->h()-265,530,530);
}


        

void floorplan_to_map(Fl_Widget*,void* data){
    
    container* root = (container*) data;
    
    int floor =  floor_choice->value()-1;
    
    int ok = fl_ask("Read in spatial map from active layers of floorplan for this floor?");
    
    
    if (floor<0){
        push_time(text);
        push_output(text,"Select a floor!\n");
        
        return;
    }
    
    push_time(text);
    push_output(text,"Reading in map structures from active layers in current floor plan...\n");
    
    
    plan floor_plan;
    floor_plan=root->current->floor_plans[floor];
    
    double shift_x=floor_plan.shift_x;
    double shift_y=floor_plan.shift_y;
    
    
    std::vector<line_in> lines;
    lines=floor_plan.lines;
    
    std::vector<int> layers;
    layers=floor_plan.layers;
    
    
    
    
    
    int count=0;
    
    for (int ii=0;ii<lines.size();ii++){
        
        if (layers[lines[ii].layer]){
    
            count++;
        }
    }
    
    /*
    if (count>20000){
        push_time(text);
        push_output(text,"Too many lines... cancelling.\n");
        return;
    }
   */
    
    
    std::vector<vec> to_add;
    std::vector<vertex> to_link;
    
        for (int ii=0;ii<lines.size();ii++){
                        
            if (layers[lines[ii].layer]){
                
         
                double x1,x2,y1,y2;
                
                x1= lines[ii].start.x+shift_x;
                y1= lines[ii].start.y+shift_y;
                x2= lines[ii].end.x+shift_x;
                y2= lines[ii].end.y+shift_y;
                
                int p1=-1,p2=-1;
                
                for (int i=0;i<to_add.size();i++){
                    if ((equal(to_add[i].x,x1))&&(equal(to_add[i].y,y1))){
                     
                        p1=i;
                        
                    }
                    
                    if ((equal(to_add[i].x,x2))&&(equal(to_add[i].y,y2))){
                        
                        p2=i;
                        
                    }
                }
                
                
                if ((p1<0)&&(p2<0)){
                    
                    vec temp;
                    temp.x=x1;
                    temp.y=y1;
                    temp.z=root->current->floor_index[floor];
                    temp.floor=floor;
                    
                    to_add.push_back(temp);
                    p1=to_add.size()-1;
                    
                    temp.x=x2;
                    temp.y=y2;
                    temp.z=root->current->floor_index[floor];
                    temp.floor=floor;
                    
                    to_add.push_back(temp);
                    p2=to_add.size()-1;
                    
                    
                }
                else if (p1<0){
                    vec temp;
                    temp.x=x1;
                    temp.y=y1;
                    temp.z=root->current->floor_index[floor];
                    temp.floor=floor;
                    
                    to_add.push_back(temp);
                    p1=to_add.size()-1;
                }
                else if (p2<0){
                    vec temp;
                    temp.x=x2;
                    temp.y=y2;
                    temp.z=root->current->floor_index[floor];
                    temp.floor=floor;
                    
                    to_add.push_back(temp);
                    p2=to_add.size()-1;
                }
                
                vertex temp;
                temp.start=p1;
                temp.end=p2;
                to_link.push_back(temp);                
                
            }
            
        }
    
    
    for (int i=to_link.size()-1;i>=0;i--){
        
        int found =0;
        for (int j=0;j<i;j++){
            
            if (((to_link[i].start==to_link[j].start)&&(to_link[i].end==to_link[j].end))||((to_link[i].end==to_link[j].start)&&(to_link[i].start==to_link[j].end))){
                found=1;
            }
            
        }
        if (found){
            to_link.erase(to_link.begin()+i);
        }
    }
    
    int old_size = root->current->points.size();
    
    for (int i=0;i<to_link.size();i++){
        to_link[i].start+=old_size;
        to_link[i].end+=old_size;
        root->current->adj_list.push_back(to_link[i]);
    }
    
    for (int i=0;i<to_add.size();i++){
        root->current->points.push_back(to_add[i]);
    }
    
    push_time(text);
    std::stringstream s;
    s<<"...Read in "<<to_add.size()<<" points and "<<to_link.size()<<" links"<<std::endl;
    push_output(text,s.str());
    
}



void link_win_cb(Fl_Widget*,void* data){
    add_link_win->show();
    add_link_win->resize(win->x()+0.5*win->w()-100,win->y()+0.5*win->h()-80,200,160);
}

void add_link_cb(Fl_Widget*,void* data){

    container* root = (container*) data;
            
    std::stringstream s;
    s<<start_add_link->value();
    int start = atoi(s.str().c_str());
    int size= s.str().size();
    
    s.str("");
    s<<end_add_link->value();
    int end = atoi(s.str().c_str());
    
    if ((size==0)||(s.str().size()==0)){
        start=-1;
        end=-1;
    }
    
    if ((start>=0)&&(start<root->current->points.size())&&(end>=0)&&(end<root->current->points.size())&&(start!=end)){
        
        int found=0;
        for (int i=0;i<root->current->adj_list.size();i++){
            
            int p1 = root->current->adj_list[i].start;
            int p2 = root->current->adj_list[i].end;
            
            if (((p1==start)&&(p2==end))||((p1==end)&&(p2==start))){
                found=1;
            }
            
        }
        
        if (!found){
            vertex temp;
            temp.start=start;
            temp.end=end;
            root->current->adj_list.push_back(temp);
        }
        
        push_time(text);
        std::stringstream s;
        s<<"Added link between points "<<start<<" and "<<end<<std::endl;
        push_output(text,s.str());
    }
    else{
        push_time(text);
        push_output(text,"Link not added. Check input.\n");
    }
    
    add_link_win->hide();
    root->draw->redraw();
    
}


void remove_link_cb(Fl_Widget*,void* data){
    
    container* root = (container*) data;        
    
    std::stringstream s;
    s<<start_add_link->value();
    int start = atoi(s.str().c_str());
    int size= s.str().size();
    
    s.str("");
    s<<end_add_link->value();
    int end = atoi(s.str().c_str());
    
    if ((size==0)||(s.str().size()==0)){
        start=-1;
        end=-1;
    }
    
    if ((start>=0)&&(start<root->current->points.size())&&(end>=0)&&(end<root->current->points.size())&&(start!=end)){
        
        int found=-1;
        for (int i=0;i<root->current->adj_list.size();i++){
            
            int p1 = root->current->adj_list[i].start;
            int p2 = root->current->adj_list[i].end;
            
            if (((p1==start)&&(p2==end))||((p1==end)&&(p2==start))){
                found=i;
            }
            
        }
        
        if (found>-1){
            root->current->adj_list.erase(root->current->adj_list.begin()+found);
        }

        
        push_time(text);
        std::stringstream s;
        s<<"Removed link between points "<<start<<" and "<<end<<std::endl;
        push_output(text,s.str());
    }
    else{
        push_time(text);
        push_output(text,"Link not removed. Check input.\n");
    }
    
        add_link_win->hide();
        root->draw->redraw();
}


void map_consistency(Fl_Widget* /*widget*/,void* data) {
    
    container* root = (container*) data;
    

    if (!root->calculating){
        
        root->calculating=CHECKING_MAP;
        fl_create_thread(thread1,check_map,data);
#if !defined(WIN32)
        pthread_detach(thread1);
#endif
    }
    

}


void* check_map(void* data){
    container* root = (container*) data;
    
    std::vector<vec>& points = root->current->points;
    std::vector<vertex>& adj_list = root->current->adj_list;
    
    std::vector<int> include(points.size(),0);

    int duplicates=0;
    
    for (int i=0;i<adj_list.size();i++){
        
        int p1= adj_list[i].start;
        int p2= adj_list[i].end;
        
        include[p1]=1;
        include[p2]=1;
        int found =0;
        
        for (int j=0;j<i;j++){
            int p3 = adj_list[j].start;
            int p4 = adj_list[j].end;
            
            if (((p3==p1)&&(p4==p2))||((p3==p2)&&(p4==p1))){
                duplicates=1;
                push_time(text);
                std::stringstream s;
                s<<"Adjacency list elements "<<i<<" and "<<j<<" are duplicates!"<<std::endl;
                push_output(text,s.str());
            }
        }
        
        
    }
    
    int bad=0;
    for (int i=0;i<include.size();i++){
        if (include[i]==0){
            bad=1;
            push_time(text);
            std::stringstream s;
            s<<"Point "<<i<<" not connected to any others on spatial map"<<std::endl;
            push_output(text,s.str().c_str());
        }
    }
    if (duplicates){
        bad=1;
        push_time(text);
        push_output(text,"WARNING: Duplicates exist in the adjacency matrix. DO NOT CALCULATE METRICS. This can only occur if they have been read in from an adjacency list with duplicates included. Check files!\n");
    }
    if (!bad){
        push_time(text);
        push_output(text,"Every point connected to at least one other and no adjacency list duplicates exist.");
    }
    root->calculating=FREE;
    return NULL;
}


void addarc(arc data,std::vector<wall>* walls,plan floor_plan,int floor, int transparent,container* root)
{
    
    double cx=data.x;
    double cy=data.y;
    double r=data.r;

    if (r>0){
        while (data.ang1>data.ang2){ //angle 2 has to be bigger
            data.ang1-=360;
        }
    }
    
    if (r<0){
        
        data.ang2+=180; //negative radius bollocks
        data.ang1+=180;
        while (data.ang1>data.ang2){ //angle 2 has to be bigger
            data.ang1-=360;
        }
    }
    
    
    double ang1=(M_PI/180.0)*data.ang1;
    double ang2=(M_PI/180.0)*data.ang2;
    
    int num_segments=10;
    
    for (int i=0;i<num_segments-1;i++){
        
        double x1= fabs(r)*cos(ang1+i*(ang2-ang1)/((double)num_segments-1.0));
        double y1= r*sin(ang1+i*(ang2-ang1)/((double)num_segments-1.0)); //nonsese dxf format:need fabs(r) for x and r for y
        
        double x2= fabs(r)*cos(ang1+(i+1)*(ang2-ang1)/((double)num_segments-1.0));
        double y2= r*sin(ang1+(i+1)*(ang2-ang1)/((double)num_segments-1.0));
        
        
        wall temp;
        
        temp.start.x=data.x+x1;
        temp.end.x=data.x+x2;
        temp.start.y=data.y+y1;
        temp.end.y=data.y+y2;
        temp.start.z=root->current->floor_index[floor];
        temp.end.z=root->current->floor_index[floor];
        temp.start.floor=floor;
        temp.end.floor=floor;
        temp.transparent=transparent;
        
        walls->push_back(temp);
        
        
    }
    
 
    
}


void lines_to_walls(int type,container* root){
    
    
    std::vector<wall>* walls= NULL;
    
    int transparent =0;
    
    switch (type){
        case 0:
            walls=&(root->current->walls_standing);
            break;
        case 1:
            walls=&(root->current->walls_standing);
            transparent=1;
            break;
        case 2:
            walls=&(root->current->walls_sitting);
            break;
        case 3:
            walls=&(root->current->walls_sitting);
            transparent=1;
            break;
        default:
            return;
    }
    
    
    int floor =  floor_choice->value()-1;
    
    
    if (floor<0){
        push_time(text);
        push_output(text,"Select a floor!\n");
        
        return;
    }
    
        plan floor_plan;
        floor_plan=root->current->floor_plans[floor];
        
        double shift_x=floor_plan.shift_x;
        double shift_y=floor_plan.shift_y;                        
        
        std::vector<arc> arcs;
        arcs=floor_plan.arcs;
        
        std::vector<circle> circles;
        circles=floor_plan.circles;
        
        std::vector<line_in> lines;
        lines=floor_plan.lines;
        
        std::vector<int> layers;
        layers=floor_plan.layers;
        
        
        for (int ii=0;ii<lines.size();ii++){
            
            if (layers[lines[ii].layer]){
                
                
                wall temp;
                
                temp.start.x=lines[ii].start.x+shift_x;
                temp.end.x=lines[ii].end.x+shift_x;
                temp.start.y=lines[ii].start.y+shift_y;
                temp.end.y=lines[ii].end.y+shift_y;
                temp.start.z=root->current->floor_index[floor];
                temp.end.z=root->current->floor_index[floor];
                temp.start.floor=floor;
                temp.end.floor=floor;
                temp.transparent=transparent;
                
                walls->push_back(temp);
                
                
            }
            
        }
        
        for (int ii=0;ii<arcs.size();ii++){
            
            if (layers[arcs[ii].layer]){
                
                arc temp;
                temp.x=arcs[ii].x+shift_x;
                temp.y=arcs[ii].y+shift_y;
                temp.r=arcs[ii].r;
                temp.ang1=arcs[ii].ang1;
                temp.ang2=arcs[ii].ang2;
                
                
                addarc(temp,walls,floor_plan,floor, transparent,root);
            }
        }
         
    
    
    
}

void lines_to_walls_standing(Fl_Widget*, void* data){
    
    container* root = (container*) data;
    
    int proceed = fl_ask("Proceeding will add to the current set of opaque standing walls\nwith lines found in the active layers of the floorplan on this floor.\nExcessive numbers of walls will negatively impact performance. Continue?");
    
    if (!proceed){
        return;
    }
    lines_to_walls(0,root);
    
}

void lines_to_walls_standing_transparent(Fl_Widget*, void* data){
    
    container* root = (container*) data;
    
    int proceed = fl_ask("Proceeding will add to the current set of transparent standing walls\nwith lines found in the active layers of the floorplan on this floor.\nExcessive numbers of walls will negatively impact performance. Continue?");
    
    if (!proceed){
        return;
    }
    lines_to_walls(1,root);
    
}

void lines_to_walls_sitting(Fl_Widget*, void* data){
    
    container* root = (container*) data;
    
    int proceed = fl_ask("Proceeding will add to the current set of opaque sitting walls\nwith lines found in the active layers of the floorplan on this floor.\nExcessive numbers of walls will negatively impact performance. Continue?");
    
    if (!proceed){
        return;
    }
    lines_to_walls(2,root);
    
}

void lines_to_walls_sitting_transparent(Fl_Widget*, void* data){
    
    container* root = (container*) data;
    
    int proceed = fl_ask("Proceeding will add to the current set of transparent sitting walls\nwith lines found in the active layers of the floorplan on this floor.\nExcessive numbers of walls will negatively impact performance. Continue?");
    
    if (!proceed){
        return;
    }
    lines_to_walls(3,root);
    
}
    



void kill_thread(Fl_Widget*, void* data) {

    container* root = (container*) data;
    
    if ((root->calculating)&&(root->killthread==0)){
        root->killthread=1;
    }
}

    
void show_warning_map_cb(void* data){
    

    container* root = (container*) data;
    root->current->route.clear();
    root->draw->show_warning_map();
}

void show_warning_vis_cb(void* data){
    
    
    container* root = (container*) data;
    root->draw->show_warning_visibility();
}

void reset_view(Fl_Widget* widget, void* data) {

    container* root = (container*) data;
    root->draw->first_draw=1;
    root->draw->redraw();
    root->graph->ang_x=0;
    root->graph->ang_y=0;
    root->graph->redraw();
    Fl::check();
}


void metric_cb(Fl_Widget*,void* data){
    container* root = (container*) data;
    
    if (custom_metric_choice->value()!=12)
    {
        custom_metric_destination_choice->activate();
        destination_criteria_choice->activate();
        ask3->activate();
        custom_metric_range_choice->activate();
        custom_metric_range->activate();
        custom_metric_floor_choice->activate();
        custom_metric_visibility_choice->activate();
        custom_metric_self_count->activate();
        eig_tol->deactivate();
    }
    
    
    if ((custom_metric_basis->value()==2)&&(custom_metric_destination_choice->value()==1)){
        destination_criteria_choice->value(6);
        route_type_choice->value(4);
        if ((custom_metric_choice->value()!=12)&&(custom_metric_choice->value()!=6)&&(custom_metric_choice->value()!=7)&&(custom_metric_choice->value()!=8)){
            custom_metric_choice->value(6);
        }
    }
    
    
    if ((custom_metric_choice->value()>6)&&(custom_metric_choice->value()!=8)){
        custom_metric_stat_choice->deactivate();
    }
    else{
        custom_metric_stat_choice->activate();
    }
    
    int temp =custom_metric_destination_choice->value();
    
    
    
    
    
    int temp2 = custom_metric_visibility_choice->value();
    if (root->current->vis_valid){
        custom_metric_visibility_choice->clear();
        custom_metric_visibility_choice->add("No constraint");
        custom_metric_visibility_choice->add("Standing visible (all barriers)");
        custom_metric_visibility_choice->add("Sitting visible (all barriers)");
        custom_metric_visibility_choice->add("Standing visible (opaque barriers)");
        custom_metric_visibility_choice->add("Sitting visible (opaque barriers)");
        custom_metric_visibility_choice->value(temp2);
    }
    else{
        custom_metric_visibility_choice->clear();
        custom_metric_visibility_choice->add("No constraint");
        custom_metric_visibility_choice->value(0);
    }
    
    if (custom_metric_basis->value()==2){
        int size=1;
        custom_metric_destination_choice->clear();
        custom_metric_destination_choice->add("Segments");
        if (root->current->lines_valid){
            custom_metric_destination_choice->add("Turnless lines");            
            size=2;
        }
        if ((temp<size)&&(temp>=0)){
            custom_metric_destination_choice->value(temp);
        }
        else{
            custom_metric_destination_choice->value(0);
        }
        
        
        custom_metric_visibility_choice->deactivate();
    }
    else{
        
        custom_metric_destination_choice->clear();
        custom_metric_destination_choice->add("Desks");
        custom_metric_destination_choice->add("Kitchens");
        custom_metric_destination_choice->add("Male WCs");
        custom_metric_destination_choice->add("Female WCs");
        custom_metric_destination_choice->add("Printers");
        custom_metric_destination_choice->add("Meeting rooms");
        custom_metric_destination_choice->add("Exits");
        custom_metric_destination_choice->add("Stairs");
        custom_metric_destination_choice->add("Lifts");
        custom_metric_destination_choice->add("Points");
        
        
        int size =10;
        if ((temp<size)&&(temp>=0)){
            custom_metric_destination_choice->value(temp);
        }
        else{
            custom_metric_destination_choice->value(0);
        }
                
        custom_metric_visibility_choice->activate();
        if (custom_metric_basis->value()==1){
            custom_metric_visibility_choice->deactivate();
        }
        if (custom_metric_choice->value()==12){
            custom_metric_visibility_choice->deactivate();
        }
        if (custom_metric_destination_choice->value()==9){
            custom_metric_visibility_choice->deactivate();
        }
    }
    
    if ((custom_metric_choice->value()==0)||(custom_metric_choice->value()==1)||(custom_metric_choice->value()==7)||(custom_metric_choice->value()==12)){
        route_type_choice->deactivate();
        max_iterations->deactivate();
        route_stat_choice->deactivate();
        ask2->deactivate();
    }
    else{
        route_type_choice->activate();
        max_iterations->activate();
        route_stat_choice->activate();
        ask2->activate();
        
        if (custom_metric_choice->value()-2 == route_type_choice->value()){
            max_iterations->deactivate();
            route_stat_choice->deactivate();
        }
        
        if (custom_metric_choice->value()>6){
            route_stat_choice->deactivate();
        }
    }
    
    if (custom_metric_range_choice->value()>2){
        destination_criteria_choice->activate();
        ask3->activate();
        custom_metric_range->activate();
    }
    else if (custom_metric_range_choice->value()>0){
        destination_criteria_choice->activate();
        ask3->activate();
        custom_metric_range->deactivate();
    }
    else{
        destination_criteria_choice->deactivate();
        ask3->deactivate();
        custom_metric_range->deactivate();
    }
    
    
    if (custom_metric_choice->value()==12){
        custom_metric_destination_choice->deactivate();
        destination_criteria_choice->deactivate();
        ask3->deactivate();
        custom_metric_range_choice->deactivate();
        custom_metric_range->deactivate();
        custom_metric_floor_choice->deactivate();
        custom_metric_visibility_choice->deactivate();
        custom_metric_self_count->deactivate();
        eig_tol->activate();
    }
    
    
}


void colour_cb(Fl_Widget* /*widget*/, void* data) {
    
    container* root = (container*) data;
    

    if (root->bow){
        
        
        
        root->bow=0;

    }
    else{
        
        
        root->bow=1;
        
     
    }
    
    root->draw->redraw();
    Fl::check();
}



void show_metric_cb(Fl_Widget* /*widget*/, void* data) {
    
    container* root = (container*) data;
    
    if (root->draw_metric){
        root->draw_metric=0;
    }
    else{
        root->draw_metric=1;
    }
    
    root->draw->redraw();
    Fl::check();
    
}


void hide(Fl_Widget* widget){
    widget->deactivate();
}

void warning_map_cb(Fl_Widget* /*widget*/, void* data) {
    
    container* root = (container*) data;
    root->current->route.clear();
    root->draw->show_warning_map();
}

void building_data_choice_cb(Fl_Widget* widget_, void* data) {
    
    
    container* root = (container*) data;
    Fl_Choice* widget = (Fl_Choice*) widget_;
    root->current_data = root->comp_buildings[widget->value()];
    redraw_data(widget_,data);
    
    
}

void building_choice_cb(Fl_Widget* widget_, void* data) {
    
    static int old_val=0;
    
    container* root = (container*) data;
    Fl_Choice* widget = (Fl_Choice*) widget_;
    Fl_Double_Window* win = root->win;
    std::vector<std::vector<vec> >& floorarea_polygons = root->current->floorarea_polygons;
    
    if (!root->calculating){
        
      
        
        
        floorarea_polygons.pop_back();
        floorarea_polygons.push_back(std::vector<vec>(0));
        
        root->current = root->comp_buildings[widget->value()];
        root->draw->current = root->current;
        prepare_undo(data);
      
        reset_floor_choice(data);
        floor_choice->value(0);
        floor_choice->do_callback(floor_choice,data);
        root->draw->deselect();
        root->draw->first_draw=1;
        root->draw->redraw();
        
        if (root->current->vis_valid){
            update_progress(progress_vis,1,0);
            warning_visibility->hide();
        }
        else{
            update_progress(progress_vis,1,2);
            warning_visibility->show();
            win->redraw();
        }
        
        if (root->current->map_valid){
            update_progress(progress,1,0);
            warning_map->hide();
        }
        else{
            update_progress(progress,1,2);
            warning_map->show();
            win->redraw();
        }
        
        if (root->current->lines_valid){
            update_progress(progress_lines,1,0);
            warning_map->hide();
        }
        else{
            update_progress(progress_lines,1,2);
            warning_map->show();
            win->redraw();
        }
        
    }
    else{
        widget->value(old_val);
    }
    
    old_val = widget->value();
    
    
}


void quit_callback(Fl_Widget*, void*) {
    
    quit_win->show();
    
    
}



void main_callback(Fl_Widget*, void* data) {


    
    static int j=0;
    
    
    container* root = (container*) data;
    
    
    
    if (Fl::event()==FL_SHORTCUT && Fl::event_key()==FL_Escape){
        undo_fullscreen(NULL,data);
    
        return;//escape key calls main window callback apparenty
    }
    
    int i=0;
    if (!root->calculating==SAVING){
        
        
        int hotspot = fl_message_hotspot(); // save state
        fl_message_hotspot(0); // disable hotspot positioning
        i=fl_choice("Quit without saving?\nIf saving remember to save every active building slot!","Save","Cancel","Quit");
    
        fl_message_hotspot(hotspot); // restore hotspot state
    }
    
    
    if (i==2){
        exit(0);
    }
    else if (i==0){
        save_binary_cb(NULL,data);
    }
    
}




void idle_load(void* data){
    
    container* root = (container*) data;
    
    static int shown =0;
    
    clock_t timer;   //non-boost
    timer=clock();  //non-boost
    double s = (timer-start_time)/(double)CLOCKS_PER_SEC;
    
    if ((!root->draw->loaded_textures)&&(root->draw->is_valid())){
        root->draw->load_textures();
        root->draw->loaded_textures=1;
    }
    
    
    if (s>2.5){
        if (shown==0){
            win->show();
            shown=1;
        }
    }
    

    if ((root->draw->loaded_textures)&&(s>4)){
        push_output(text,"done.\n");
        win2->hide();

        Fl::remove_idle(idle_load,data);

        push_time(text);
        push_output(text,"Ready.\n");
        root->calculating = FREE;
    }
}


void clear_metrics_cb(Fl_Widget* /*widget*/, void* data){
    
    container* root = (container*) data;
    
    if (!root->calculating){
        
        root->current->matrix.clear();
        std::vector<std::vector<int> >().swap(root->current->matrix);
        
        root->current->custom_metric.clear();
        std::vector<double>().swap(root->current->custom_metric);
        root->current->map.clear();
        
        root->current->metrics.clear();
        root->current->metric_des.clear();
        
        std::vector<std::vector<double> >().swap(root->current->metrics);
        std::vector<metric_description>().swap(root->current->metric_des);
        
        root->current->route.clear();
        
        root->draw->show_warning_map();
        root->draw->redraw();
    }
    
    return;
}


void* showmain(void * /*data*/){
    
    
    win->show();
    
#if !defined(WIN32)
        return NULL;
    //pthread_exit(NULL);
#else
    return NULL;
#endif
}

void* showsecond(void * /*data*/){
    
    win2->show();
#if !defined(WIN32)
        return NULL;
    //pthread_exit(NULL);
#else
    return NULL;
#endif
}

void redraw(Fl_Widget* /*widget*/,void* data){
    container* root = (container*) data;
    root->draw->redraw();
}


void redraw_from_thread(void* data){
    container* root = (container*) data;
    root->win->redraw();
    root->draw->redraw();
    root->graph->redraw();
}

void check_from_thread(void * /*data*/){
    warning_visibility->hide();
    warning_visibility->redraw();
    warning_visibility->parent()->redraw();
    win->redraw();
    Fl::check();
    Fl::flush();
}



void idle(void* data){
    
    container* root = (container*) data;
    if  (three_d_choice->value()==3){
        root->draw->redraw();
    }
}




void redraw_show(Fl_Widget* widget, void* data){
    
    
    container* root = (container*) data;
    
    
    
    int reset=0;
    
    switch (mark_locations_choice->value()){
        case 1:
            if (!show_desks->value()){
                reset=1;
            }
            break;
        case 2:
            if (!show_kitchens->value()){
                reset=1;
            }
            break;
        case 3:
            if (!show_maleWC->value()){
                reset=1;
            }
            break;
        case 4:
            if (!show_femaleWC->value()){
                reset=1;
            }
            break;
        case 5:
            if (!show_printers->value()){
                reset=1;
            }
            break;
        case 6:
            if (!show_meeting_rooms->value()){
                reset=1;
            }
            break;
        case 7:
            if (!show_exits->value()){
                reset=1;
            }
            break;
        case 8:
            if (!show_stairs->value()){
                reset=1;
            }
            break;
        case 9:
            if (!show_lifts->value()){
                reset=1;
            }
            break;
        case 10:
            if (!show_walls_sitting->value()){
                reset=1;
            }
            break;
        case 11:
            if (!show_walls_standing->value()){
                reset=1;
            }
            break;
        case 12:
            if (!show_map->value()){
                reset=1;
            }

            break;
        case 13:
            
            if (!show_map->value()){
                reset=1;
            }
            
            break;
        case 14:
            if (!show_floor_areas->value()){
                reset=1;
            }
            break;
    }
    
    
    if (reset){
        mark_locations_choice->value(0);
        redraw_mark(widget,data);
    }
    
    
    
    root->draw->redraw();
}



void scheme0(Fl_Widget* /*widget*/,void* /*data*/){
    
    Fl::scheme("none");
    Fl::check();
}
void scheme1(Fl_Widget* /*widget*/,void* /*data*/){
    
    Fl::scheme("gtk+");
    Fl::check();
}
void scheme2(Fl_Widget* /*widget*/,void* /*data*/){
    
    Fl::scheme("plastic");
    Fl::check();
}




void rebuild_hist_input(Fl_Widget*,void* data){
    
    container* root = (container*) data;
    
    for (unsigned i=0;i<4;i++){
        
        int bd = hist_building[i]->value();
        
        if ((hist_metric[i]->size()-1)!=root->comp_buildings[bd]->metrics.size()+1){
            
           
            
            hist_metric[i]->clear();
            hist_metric[i]->add("-----");
            
            for (unsigned j=0;j<root->comp_buildings[bd]->metrics.size();j++){
                std::stringstream s;
                s<<j+1;
                hist_metric[i]->add(s.str().c_str());
            }
            hist_metric[i]->value(0);
        }
    }
    
    root->graph->histogram(&(root->current->custom_metric));
    root->graph->redraw();
    
}


void main_tab_callback(Fl_Widget* widget,void* data){
    
    container* root = (container*) data;
    
    rebuild_hist_input(widget,data);
    
    
    root->graph->histogram(&(root->current->custom_metric));
    root->graph->redraw();
    data_choice->value(0);
    redraw_data(widget,data);
}


void tab_callback(Fl_Widget* widget,void* data){
    
    container* root = (container*) data;
    
    
    if (mark_locations_choice->value()){
        mark_locations_choice->value(0);
        redraw_mark(widget,data);
        if (root->draw->set_first){
            root->draw->set_first=0;
            root->draw->redraw();
        }
    }
}



void LayerCB(Fl_Widget* /*widget*/, void* data)
{
    // Create a modal dialog with layers checkbuttons
    
    if (floor_choice->value()){
        LayerWin* dlg = new LayerWin(data);
        dlg->Create();
    }
    else{
        push_time(text);
        push_output(text,"Select a floor!\n");
    }
}





static int wtemp,htemp,xtemp,ytemp;
static int wtemp1,htemp1,xtemp1,ytemp1;
static MyWindow* new_win;
static int full =0;

void undo_fullscreen(Fl_Widget* /*widget*/, void* data){
    
    container* root= (container*) data;
    
    root->win->fullscreen_off(xtemp1,ytemp1,wtemp1,htemp1);
}


void go_fullscreen(Fl_Widget* /*widget*/, void* data){
    
     container* root= (container*) data;
    xtemp = root->draw->x();
    ytemp = root->draw->y();
    wtemp = root->draw->w();
    htemp = root->draw->h();
    
    xtemp1 = root->win->x();
    ytemp1 = root->win->y();
    wtemp1 = root->win->w();
    htemp1 = root->win->h();
    
    root->win->resize(0,0,Fl::w(),Fl::h());
    root->win->fullscreen();
    
}

void about_cb(Fl_Widget* /*widget*/, void* data){
    
    
    container* root =(container*) data;
    Fl_Double_Window* win = root->win;
    
    
    about_win->show();
    about_win->resize(win->x()+0.5*win->w()-125,win->y()+0.5*win->h()-75,250,150);
    
    
    
    
    Fl::check();
    Fl::flush();
}

void about_ok_cb(Fl_Widget* /*widget*/, void* data){
    Fl_Window* w = (Fl_Window*) data;
    w->hide();
    win->redraw();
    Fl::check();
    Fl::flush();
    
}
void guide_cb(Fl_Widget* /*widget*/, void* data){
    
    container* root = (container*) data;
    Fl_Double_Window* win = root->win;
    
    
    guide->show();
    guide->resize(win->x()+0.5*win->w()-250,win->y()+0.5*win->h()-265,500,530);
    
    
    Fl::check();
    Fl::flush();
}


void dest_guide_cb(Fl_Widget* /*widget*/, void* data){
    
    container* root = (container*) data;
    Fl_Double_Window* win = root->win;
    
    
    destination_guide->show();
    destination_guide->resize(win->x()+0.5*win->w()-215,win->y()+0.5*win->h()-200,387,430);
    Fl::check();
    Fl::flush();
}

void route_guide_cb(Fl_Widget* /*widget*/, void* data){
    
    container* root = (container*) data;
    Fl_Double_Window* win = root->win;
    
    route_guide->show();
    route_guide->resize(win->x()+0.5*win->w()-215,win->y()+0.5*win->h()-200,387,430);
    Fl::check();
    Fl::flush();
}







void ok_guide_cb(Fl_Widget* /*widget*/, void* /*data*/){
    guide->hide();
}

void ok_dest_guide_cb(Fl_Widget* /*widget*/, void* /*data*/){
    destination_guide->hide();
}

void ok_route_guide_cb(Fl_Widget* /*widget*/, void* /*data*/){
    route_guide->hide();
}


void show_route_cb(Fl_Widget*,void* data){
    container* root =(container*) data;
    if (root->draw_route){
        root->draw_route=0;
    }
    else{
        root->draw_route=1;
    }
    root->draw->redraw();
    Fl::check();
}

void show_floorplan_cb(Fl_Widget*,void* data){
    container* root =(container*) data;
    if (root->draw_floorplan){
        root->draw_floorplan=0;
    }
    else{
        root->draw_floorplan=1;
    }
    root->draw->redraw();
    Fl::check();
}

void show_grid_cb(Fl_Widget*,void* data){
    container* root =(container*) data;
    if (root->draw_grid){
        root->draw_grid=0;
    }
    else{
        root->draw_grid=1;
    }
    root->draw->redraw();
    Fl::check();
}

void snap_to_point_cb(Fl_Widget*,void* data){
    container* root =(container*) data;
    if (root->snap_to_point){
        root->snap_to_point=0;
    }
    else{
        root->snap_to_point=1;
    }
    root->draw->redraw();
    Fl::check();
}

void transparent_walls_cb(Fl_Widget*,void* data){
    container* root =(container*) data;
    if (root->transparent_walls){
        root->transparent_walls=0;
    }
    else{
        root->transparent_walls=1;
    }
    root->draw->redraw();
    Fl::check();
}


void points_label_cb(Fl_Widget*,void* data){
    container* root =(container*) data;
    if (root->draw_points_label){
        root->draw_points_label=0;
    }
    else{
        root->draw_points_label=1;
    }
    root->draw->redraw();
    Fl::check();
}

void segments_label_cb(Fl_Widget*,void* data){
    container* root =(container*) data;
    if (root->draw_segments_label){
        root->draw_segments_label=0;
    }
    else{
        root->draw_segments_label=1;
    }
    root->draw->redraw();
    Fl::check();
}

void color_scale_cb(Fl_Widget*,void* data){
    container* root =(container*) data;
    if (root->color_scale){
        root->color_scale=0;
    }
    else{
        root->color_scale=1;
    }
    root->draw->redraw();
    Fl::check();
}

void show_scale_cb(Fl_Widget*,void* data){
    container* root =(container*) data;
    if (root->show_scale){
        root->show_scale=0;
    }
    else{
        root->show_scale=1;
    }
    root->draw->redraw();
    Fl::check();
}



void* route_thread(void* data){
    
    container* root = (container*) data;
    
    map_structures& map = root->current->map;
    
    int choice=route_type_c->value();
    std::stringstream s;
    s<<start_route_input->value();
    int start = atoi(s.str().c_str());
    int size= s.str().size();
    
    s.str("");
    s<<end_route_input->value();
    int end = atoi(s.str().c_str());
    
    if ((size==0)||(s.str().size()==0)){
        start=-1;
        end=-1;
    }
    
   
    push_time(root->buttons.text);
    push_output(root->buttons.text,"Calculating route...\n");
    
    map_structures map_new = map;
    

    
    double value;
    
    std::vector<std::vector<int> > temp =calculate_route(root,&map_new,choice,start,end,root->current->dual_route,1000,value);
    
    push_time(root->buttons.text);
    
    s.str("");
    s<<"...done. "<<temp.size()<<" equivalent route(s) with metric value "<< value<<"."<<std::endl;
    push_output(root->buttons.text,s.str().c_str());
    
    Fl::lock();
    root->current->route =temp;
    root->calculating = FREE;
    root->draw->redraw();
    Fl::unlock();
    
#if !defined(WIN32)
        return NULL;
    //pthread_exit(NULL);
#else
    return NULL;
#endif
    
    
}


void ok_route_cb(Fl_Widget* /*widget*/, void* data){
    route_display->hide();
    
    container* root = (container*) data;
    THREAD_STATE& calculating = root->calculating;
    root->current->route.clear();
    std::vector<std::vector<int> >().swap(root->current->route);
    
    if ((calculating == FREE)&&(root->current->map_valid)){
        calculating=CALC_ROUTE;
        
        
        root->menu->clear();
        Fl_Menu_Item temp []={{"Route",FL_COMMAND+'r',show_route_cb,root,FL_MENU_TOGGLE|FL_MENU_VALUE,0,0,FL_NORMAL_SIZE,FL_BLACK}};//36
        root->items[SHOW_ROUTE_INDEX]=temp[0];
        root->menu->menu(root->items);
        root->draw_route=1;
        
        fl_create_thread(thread3,route_thread,data);
#if !defined(WIN32)
        pthread_detach(thread3);
#endif
        custom_metric_calc->hide();
    }
    else if(calculating == FREE){
        push_time(text);
        push_output(text,"Spatial map out of date! Recalculate structures to calculate routes.\n");
    }
}





void plot_choice_cb(Fl_Widget* /*widget*/, void* data){
    
    
    
    
    container* root= (container*) data;
    std::vector<double>* custom_metric = &(root->current->custom_metric);
    
    
    if (plot_choice->value()){
        
        for (int i=0;i<4;i++){
            
            bin_count[i]->activate();
        }
        
        hist_building[3]->activate();
        hist_metric[3]->activate();
        
        hist_interp_choice->activate();
        yaxis_choice->activate();
        show_regression->deactivate();
        
    }
    else{
        for (int i=0;i<4;i++){
            
            bin_count[i]->deactivate();
        }
        
        hist_building[3]->deactivate();
        hist_metric[3]->deactivate();
        
        hist_interp_choice->deactivate();
        yaxis_choice->deactivate();
        show_regression->activate();
        
    }
    
    
    int count=0,count1=0;;
    
    for (unsigned int i=0;i<4;i++){
        
        if (hist_metric[i]->value()){ //hist_p
            count++;
            if (i<3){
                count1++;
            }
        }
    }
    
    if (((count<2)&&(plot_choice->value()))||((count1<3)&&(!plot_choice->value()))){
        root->graph->ang_x=0;
        root->graph->ang_y=0;
        root->graph->ang_z=0;
    }

    root->graph->histogram(custom_metric);
    
    root->graph->redraw();
    
}


void shiftx_cb(Fl_Widget* /*widget*/, void* data){
    
    container* root= (container*) data;
    std::vector<plan>& floor_plans = (root->current->floor_plans);
    
    if (floor_choice->value()>0){
        
        floor_plans[floor_choice->value()-1].shift_x=xshift->value();
        
        root->draw->redraw();
        
    }
    else{
        std::stringstream s;
        s<<"Select a floor!\n";
        push_time(text);
        push_output(text,s.str());
    }
    
}

void shifty_cb(Fl_Widget* /*widget*/, void* data){
    
    container* root= (container*) data;
    std::vector<plan>& floor_plans = (root->current->floor_plans);
    
    if (floor_choice->value()>0){
        
        floor_plans[floor_choice->value()-1].shift_y=yshift->value();
        root->draw->redraw();
    }
    else{
        std::stringstream s;
        s<<"Select a floor!\n";
        push_time(text);
        push_output(text,s.str());
    }
}

void zfloor_cb(Fl_Widget* /*widget*/, void* data){
    
    
    container* root = (container*) data;
    std::vector<vec>* points = &(root->current->points);
    std::vector<location>* desks = &(root->current->desks);
    std::vector<location>* kitchens = &(root->current->kitchens);
    std::vector<location>* male_toilets = &(root->current->male_toilets);
    std::vector<location>* female_toilets = &(root->current->female_toilets);
    std::vector<location>* printers = &(root->current->printers);
    std::vector<location>* meeting_rooms = &(root->current->meeting_rooms);
    std::vector<location>* exits = &(root->current->exits);
    std::vector<location>* stairs = &(root->current->stairs);
    std::vector<location>* lifts = &(root->current->lifts);
    
    std::vector<wall>& walls_standing = (root->current->walls_standing);
    std::vector<wall>& walls_sitting = (root->current->walls_sitting);
    std::vector<double>& floor_index = root->current->floor_index;
    
    if ((floor_choice->value())&&((int)floor_index.size()>floor_choice->value()-1)){
        prepare_undo(data);
        
        floor_index[floor_choice->value()-1]=zfloor->value();
        
        
        for (unsigned int i=0;i<points->size();i++){
            
            if ((*points)[i].floor==floor_choice->value()-1){
                (*points)[i].z=floor_index[floor_choice->value()-1];
            }
            
        }
        
        for (unsigned int i=0;i<walls_sitting.size();i++){
            
            if (walls_sitting[i].start.floor==floor_choice->value()-1){
                
                walls_sitting[i].start.z=floor_index[floor_choice->value()-1];
                walls_sitting[i].end.z=floor_index[floor_choice->value()-1];
                
            }
            
        }
        
        for (unsigned int i=0;i<walls_standing.size();i++){
            
            if (walls_standing[i].start.floor==floor_choice->value()-1){
                
                walls_standing[i].start.z=floor_index[floor_choice->value()-1];
                walls_standing[i].end.z=floor_index[floor_choice->value()-1];
                
            }
            
        }
        
        std::vector<location>* loc = NULL;
        
        
        for (unsigned int place_loop=0;place_loop<9;place_loop++){//;place_loop<9;place_loop++){
            
            switch (place_loop){
                case 0:loc=desks;break;
                case 1:loc=kitchens; break;
                case 2:loc=male_toilets;break;
                case 3:loc=female_toilets;break;
                case 4:loc=printers;break;
                case 5:loc=meeting_rooms;break;
                case 6:loc=exits;break;
                case 7:loc=stairs;break;
                case 8:loc=lifts;break;
            }
            
            for (unsigned int i=0;i<loc->size();i++){
                if ((*loc)[i].loc.floor==floor_choice->value()-1){
                    (*loc)[i].loc.z=floor_index[floor_choice->value()-1];
                }
            }
        }
        
        
        root->current->route.clear();
        root->draw->show_warning_map();
        
        root->draw->redraw();
    }
}

void point_in_cb(Fl_Widget* /*widget*/, void* data){
    
    
    container* root = (container*) data;
    std::vector<vec>* points = &(root->current->points);
    std::vector<location>* desks = &(root->current->desks);
    std::vector<location>* kitchens = &(root->current->kitchens);
    std::vector<location>* male_toilets = &(root->current->male_toilets);
    std::vector<location>* female_toilets = &(root->current->female_toilets);
    std::vector<location>* printers = &(root->current->printers);
    std::vector<location>* meeting_rooms = &(root->current->meeting_rooms);
    std::vector<location>* exits = &(root->current->exits);
    std::vector<location>* stairs = &(root->current->stairs);
    std::vector<location>* lifts = &(root->current->lifts);
    
    std::vector<location>* loc = NULL;
    
    switch (mark_locations_choice->value()){
        case 1:
            loc=desks;break;
        case 2:
            loc=kitchens;break;
        case 3:
            loc=male_toilets;break;
        case 4:
            loc=female_toilets;break;
        case 5:
            loc=printers;break;
        case 6:
            loc=meeting_rooms;break;
        case 7:
            loc=exits;break;
        case 8:
            loc=stairs;break;
        case 9:
            loc=lifts;break;
        default:
            return;
    }
    
   
    
    
    if (root->draw->selector>0){
        
        if ((root->draw->selected>-1)&&(root->draw->selected<(int)loc->size())){
            
            std::stringstream s;
            s<<overwrite_location_point->value();
            
            for (unsigned int i=0;i<s.str().size();i++){
                if (!isdigit(s.str()[i])){
                    s.str("");
                    s<<"Error!";
                    overwrite_location_point->value(s.str().c_str());
                    return;
                }
            }
            
            int i = atoi(s.str().c_str());
            if ((i>-1)&&(i<(int)points->size())){
                prepare_undo(data);
                s.str("");
                s<<i;
                overwrite_location_point->value(s.str().c_str());
                (*loc)[root->draw->selected].point=i;
            }
            else{
                s.str("");
                s<<"Error!";
                overwrite_location_point->value(s.str().c_str());
            }
            
            root->draw->redraw();
            
        }
        
    }
    
    
    
}

void location_label_cb(Fl_Widget* /*widget*/, void* data){
    
    
    
    
    container* root = (container*) data;
    std::vector<location>* desks = &(root->current->desks);
    std::vector<location>* kitchens = &(root->current->kitchens);
    std::vector<location>* male_toilets = &(root->current->male_toilets);
    std::vector<location>* female_toilets = &(root->current->female_toilets);
    std::vector<location>* printers = &(root->current->printers);
    std::vector<location>* meeting_rooms = &(root->current->meeting_rooms);
    std::vector<location>* exits = &(root->current->exits);
    std::vector<location>* stairs = &(root->current->stairs);
    std::vector<location>* lifts = &(root->current->lifts);
    
    prepare_undo(data);
    
    std::vector<location>* loc = NULL;
    
    switch (mark_locations_choice->value()){
        case 1:
            loc=desks;break;
        case 2:
            loc=kitchens;break;
        case 3:
            loc=male_toilets;break;
        case 4:
            loc=female_toilets;break;
        case 5:
            loc=printers;break;
        case 6:
            loc=meeting_rooms;break;
        case 7:
            loc=exits;break;
        case 8:
            loc=stairs;break;
        case 9:
            loc=lifts;break;
        default:
            return;
    }
    
    
    
    if (root->draw->selector>0){
        
        if ((root->draw->selected>-1)&&(root->draw->selected<(int)loc->size())){
            
            std::stringstream s;
            s<<location_label->value();
            
            
            
            (*loc)[root->draw->selected].label=s.str();
            
        }
        
    }
    
    
}

void clear_last_location_cb(Fl_Widget* /*widget*/, void* data){
           
    
    container* root = (container*) data;
    std::vector<vec>* points = &(root->current->points);
    std::vector<vertex>* adj_list = &(root->current->adj_list);
    
    std::vector<location>* desks = &(root->current->desks);
    std::vector<location>* kitchens = &(root->current->kitchens);
    std::vector<location>* male_toilets = &(root->current->male_toilets);
    std::vector<location>* female_toilets = &(root->current->female_toilets);
    std::vector<location>* printers = &(root->current->printers);
    std::vector<location>* meeting_rooms = &(root->current->meeting_rooms);
    std::vector<location>* exits = &(root->current->exits);
    std::vector<location>* stairs = &(root->current->stairs);
    std::vector<location>* lifts = &(root->current->lifts);
    
    std::vector<wall>& walls_standing = (root->current->walls_standing);
    std::vector<wall>& walls_sitting = (root->current->walls_sitting);
    
    std::vector<std::vector<vec> >& floorarea_polygons = (root->current->floorarea_polygons);
    std::vector<std::vector<triangle> >& floorarea_triangles = root->current->floorarea_triangles;
    
    if (!three_d_choice->value()){
        
        prepare_undo(data);
        
        switch (mark_locations_choice->value()){
            case 1:
                if (desks->size()){
                    desks->pop_back();
                    root->draw->show_warning_visibility();
                }
                break;
            case 2:
                if (kitchens->size()){
                    kitchens->pop_back();
                    root->draw->show_warning_visibility();
                }
                break;
            case 3:
                if (male_toilets->size()){
                    male_toilets->pop_back();
                    root->draw->show_warning_visibility();
                }
                break;
            case 4:
                if (female_toilets->size()){
                    female_toilets->pop_back();
                    root->draw->show_warning_visibility();
                }
                break;
            case 5:
                if (printers->size()){
                    printers->pop_back();
                    root->draw->show_warning_visibility();
                }
                break;
            case 6:
                if (meeting_rooms->size()){
                    meeting_rooms->pop_back();
                    root->draw->show_warning_visibility();
                }
                break;
            case 7:
                if (exits->size()){
                    exits->pop_back();
                    root->draw->show_warning_visibility();
                }
                break;
            case 8:
                if (stairs->size()){
                    stairs->pop_back();
                    root->draw->show_warning_visibility();
                }
                break;
            case 9:
                if (lifts->size()){
                    lifts->pop_back();
                    root->draw->show_warning_visibility();
                }
                break;
            case 10:
                if (root->draw->set_first){
                    root->draw->set_first=0;
                }
                else{
                    if (walls_sitting.size()){
                        walls_sitting.pop_back();
                        root->draw->show_warning_visibility();
                    }
                }
                break;
            case 11:
                if (root->draw->set_first){
                    root->draw->set_first=0;
                }
                else{
                    if (walls_standing.size()){
                        walls_standing.pop_back();
                        root->draw->show_warning_visibility();
                    }
                }
                break;
            case 12:
            {
                if (points->size()){
                    
                    int ii = (int)points->size()-1;
                    
                    
                    for (int i=adj_list->size()-1;i>=0;i--){
                        int p1 = (*adj_list)[i].start;
                        int p2 = (*adj_list)[i].end;
                        
                        if ((p1==ii)||(p2==ii)){
                            adj_list->erase(adj_list->begin()+i);
                        }
                        
                    }
                    
                    points->erase(points->begin()+ii);
                    
                    root->current->route.clear();
                    
                    //need to add deletion/adaption of locations associated with point reomved and points that come after...
                    
                    update_locations(desks,ii);
                    update_locations(kitchens,ii);
                    update_locations(male_toilets,ii);
                    update_locations(female_toilets,ii);
                    update_locations(printers,ii);
                    update_locations(meeting_rooms,ii);
                    update_locations(stairs,ii);
                    update_locations(lifts,ii);
                    update_locations(exits,ii);
                    
                    root->draw->show_warning_map();
                }
                
            }
                break;
            case 14:
                
                
                if ((int)floorarea_polygons.size()>1){
                    
                
                    
                    floorarea_polygons.pop_back();
                    floorarea_polygons.pop_back();
                    floorarea_polygons.push_back(std::vector<vec>(0));
                
                    floorarea_triangles.pop_back();
                
                }
                root->draw->calculate_areas(&floorarea_polygons);
                
                
                break;
                
        }
        Fl::check(); //temporary???
        
        root->draw->redraw();
    }
}

void clear_selected_location_cb(Fl_Widget* /*widget*/, void* data){
    
    container* root = (container*) data;
    std::vector<vec>* points = &(root->current->points);
    std::vector<vertex>* adj_list = &(root->current->adj_list);
    
    std::vector<location>* desks = &(root->current->desks);
    std::vector<location>* kitchens = &(root->current->kitchens);
    std::vector<location>* male_toilets = &(root->current->male_toilets);
    std::vector<location>* female_toilets = &(root->current->female_toilets);
    std::vector<location>* printers = &(root->current->printers);
    std::vector<location>* meeting_rooms = &(root->current->meeting_rooms);
    std::vector<location>* exits = &(root->current->exits);
    std::vector<location>* stairs = &(root->current->stairs);
    std::vector<location>* lifts = &(root->current->lifts);
    
    
    std::vector<wall>* walls_standing = &(root->current->walls_standing);
    std::vector<wall>* walls_sitting = &(root->current->walls_sitting);
    
    std::vector<std::vector<vec> >& floorarea_polygons = root->current->floorarea_polygons;
    std::vector<std::vector<triangle> >& floorarea_triangles = root->current->floorarea_triangles;
    
    if ((mark_locations_choice->value())&&(!three_d_choice->value())){
        if ((root->draw->selector>0)&&((root->draw->selected>-1)||((root->draw->selected_matrix[0]>-1)&&(root->draw->selected_matrix[1]>-1)))){
            
            prepare_undo(data);
            
            std::vector<location>* temp=NULL;
            std::vector<wall>* temp_wall=NULL;
            
            int loc_or_wall=-1;
            
            switch (root->draw->selector){
                    
                case 1:
                    temp=desks;
                    loc_or_wall=0;
                    
                    break;
                case 2:
                    temp=kitchens;
                    loc_or_wall=0;
                    
                    break;
                case 3:
                    temp=male_toilets;
                    loc_or_wall=0;
                    
                    break;
                case 4:
                    temp=female_toilets;
                    loc_or_wall=0;
                    
                    break;
                case 5:
                    temp=printers;
                    loc_or_wall=0;
                    
                    break;
                case 6:
                    temp=meeting_rooms;
                    loc_or_wall=0;
                    
                    break;
                case 7:
                    temp=exits;
                    loc_or_wall=0;
                    
                    break;
                case 8:
                    temp=stairs;
                    loc_or_wall=0;
                    
                    break;
                case 9:
                    temp=lifts;
                    loc_or_wall=0;
                    
                    break;
                case 10:
                    temp_wall=walls_sitting;
                    loc_or_wall=1;
                    
                    break;
                case 11:
                    temp_wall=walls_standing;
                    loc_or_wall=1;
                    
                    break;
                case 12:
                    loc_or_wall=2;
                    break;
                case 13:
                    loc_or_wall=3;
                    break;
                case 14:
                    loc_or_wall=4;
                   
                    break;
                default:
                    break;
            }
            
            
            //std::cout<<"NOWHMM "<<draw->selector<<" "<<draw->selected<<" "<<(*temp_wall).size()<<std::endl;
            
            if ((root->draw->selected>-1)&&(loc_or_wall==0)){ //erasing location
                if (root->draw->selected<(int)(*temp).size()){
                    
                    
                    (*temp).erase((*temp).begin()+root->draw->selected);
                    root->draw->deselect();
                    
                    root->draw->redraw();
                    
                    
                    root->draw->show_warning_visibility();
                    
                }
            }
            else if ((root->draw->selected>-1)&&(loc_or_wall==1)){ //erasing wall
                if (root->draw->selected<(int)(*temp_wall).size()){
                    
                    
                    
                    
                    (*temp_wall).erase((*temp_wall).begin()+root->draw->selected);
                    
                    root->draw->deselect();
                    root->draw->redraw();
                    
                    root->draw->show_warning_visibility();
                    
                }
            }
            else if ((root->draw->selected>-1)&&(loc_or_wall==2)){ //erasing spatial map point
                
                
                int ii = root->draw->selected;
                
                for (int i=adj_list->size()-1;i>=0;i--){
                    int p1 = (*adj_list)[i].start;
                    int p2 = (*adj_list)[i].end;
                    
                    if ((p1==ii)||(p2==ii)){
                        adj_list->erase(adj_list->begin()+i);
                    }
                    
                }
                
                for (int i=0;i<adj_list->size();i++){
                    
                    int p1 = (*adj_list)[i].start;
                    int p2 = (*adj_list)[i].end;
                    
                    
                    if (p1>ii){
                        (*adj_list)[i].start--;
                    }
                    if (p2>ii){
                        (*adj_list)[i].end--;
                    }
                    
                }
             
                
                points->erase(points->begin()+root->draw->selected);
                
                root->current->route.clear();
                
                
                //need to add deletion/adaption of locations associated with point reomved and points that come after...
                
                update_locations(desks,root->draw->selected);
                update_locations(kitchens,root->draw->selected);
                update_locations(male_toilets,root->draw->selected);
                update_locations(female_toilets,root->draw->selected);
                update_locations(printers,root->draw->selected);
                update_locations(meeting_rooms,root->draw->selected);
                update_locations(stairs,root->draw->selected);
                update_locations(lifts,root->draw->selected);
                update_locations(exits,root->draw->selected);
                
                
                
                root->draw->selector=-1;
                root->draw->selected=-1;
                root->draw->selected_matrix[0]=-1;
                root->draw->selected_matrix[1]=-1;
                root->draw->show_warning_map();
                root->draw->redraw();
                
            }
            else if ((root->draw->selected>-1)&&(loc_or_wall==3)){ //erasing spatial map link
                
                adj_list->erase(adj_list->begin()+root->draw->selected);

                root->draw->selector=-1;
                root->draw->selected=-1;
                root->draw->selected_matrix[0]=-1;
                root->draw->selected_matrix[1]=-1;
                root->current->route.clear();
                root->draw->show_warning_map();
                root->draw->redraw();
            }
            else if ((root->draw->selected>-1)&&(loc_or_wall==4)){//erasing area;
                
                
                
                floorarea_polygons.erase(floorarea_polygons.begin()+root->draw->selected);
                
                floorarea_triangles.erase(floorarea_triangles.begin()+root->draw->selected);
                
                root->draw->calculate_areas(&floorarea_polygons);
                root->draw->selector=-1;
                root->draw->selected=-1;
                root->draw->redraw();
                
            }
            
            
            Fl::check();
                       
        }
    }
    
}

void update_locations(std::vector<location>* location,int removed_point){
    
    
    
    std::vector<int> for_removal;
    
    for (unsigned int i=0;i<location->size();i++){ //find all locations associated with spatial map point
        if ((*location)[i].point==removed_point){
            for_removal.push_back(i);
        }
        
    }
    
    for (unsigned int i=0;i<location->size();i++){ // change the spatial map point for all locations that which will move one down the list to fill the gap
        if ((*location)[i].point>removed_point){
            (*location)[i].point--;
        }
        
    }
    
    
    for (int i=(int)for_removal.size()-1;i>-1;i--){ //can guarantee that for_removal is ordered absolutely. HAVE to run backwards
        (*location).erase((*location).begin()+for_removal[i]);//delete the points in question: only works if running backwards otherwise the index changes
        
    }
    
    
    
}

void clear_locations_cb(Fl_Widget* /*widget*/, void* data){
    
    container* root = (container*) data;
    std::vector<vec>* points = &(root->current->points);
    std::vector<vertex>* adj_list = &(root->current->adj_list);
    
    std::vector<location>* desks = &(root->current->desks);
    std::vector<location>* kitchens = &(root->current->kitchens);
    std::vector<location>* male_toilets = &(root->current->male_toilets);
    std::vector<location>* female_toilets = &(root->current->female_toilets);
    std::vector<location>* printers = &(root->current->printers);
    std::vector<location>* meeting_rooms = &(root->current->meeting_rooms);
    std::vector<location>* exits = &(root->current->exits);
    std::vector<location>* stairs = &(root->current->stairs);
    std::vector<location>* lifts = &(root->current->lifts);
    
    std::vector<wall>& walls_sitting = root->current->walls_sitting;
    std::vector<wall>& walls_standing = root->current->walls_standing;
    std::vector<std::vector<vec> >& floorarea_polygons = root->current->floorarea_polygons;
    std::vector<std::vector<triangle> >& floorarea_triangles = root->current->floorarea_triangles;
    
    
    if (!three_d_choice->value()){
        
        prepare_undo(root);
        
        switch (mark_locations_choice->value()){
            case 1:
                desks->clear();
                std::vector<location>().swap(*desks);
                root->draw->show_warning_visibility();
                break;
            case 2:
                kitchens->clear();
                std::vector<location>().swap(*kitchens);
                root->draw->show_warning_visibility();
                break;
            case 3:
                male_toilets->clear();
                std::vector<location>().swap(*male_toilets);
                root->draw->show_warning_visibility();
                break;
            case 4:
                female_toilets->clear();
                std::vector<location>().swap(*female_toilets);
                root->draw->show_warning_visibility();
                break;
            case 5:
                printers->clear();
                std::vector<location>().swap(*printers);
                root->draw->show_warning_visibility();
                break;
            case 6:
                meeting_rooms->clear();
                std::vector<location>().swap(*meeting_rooms);
                root->draw->show_warning_visibility();
                break;
            case 7:
                exits->clear();
                std::vector<location>().swap(*exits);
                root->draw->show_warning_visibility();
                break;
            case 8:
                stairs->clear();
                std::vector<location>().swap(*stairs);
                root->draw->show_warning_visibility();
                break;
            case 9:
                lifts->clear();
                std::vector<location>().swap(*lifts);
                root->draw->show_warning_visibility();
                break;
            case 10:
                walls_sitting.clear();
                std::vector<wall>().swap(walls_sitting);
                if (root->draw->set_first){
                    root->draw->set_first=0;
                }
                root->draw->show_warning_visibility();
                break;
            case 11:
                walls_standing.clear();
                std::vector<wall>().swap(walls_standing);
                if (root->draw->set_first){
                    root->draw->set_first=0;
                }
                root->draw->show_warning_visibility();
                break;
            case 12:
                desks->clear();
                kitchens->clear();
                male_toilets->clear();
                female_toilets->clear();
                printers->clear();
                meeting_rooms->clear();
                exits->clear();
                stairs->clear();
                lifts->clear();
                std::vector<location>().swap(*desks);
                std::vector<location>().swap(*kitchens);
                std::vector<location>().swap(*male_toilets);
                std::vector<location>().swap(*female_toilets);
                std::vector<location>().swap(*printers);
                std::vector<location>().swap(*meeting_rooms);
                std::vector<location>().swap(*exits);
                std::vector<location>().swap(*stairs);
                std::vector<location>().swap(*lifts);
                
                
                
                adj_list->clear();
                points->clear();
                root->current->route.clear();
                std::vector<vec>().swap(*points);
                std::vector<vertex>().swap(*adj_list);
                
                root->draw->show_warning_map();
                break;
            case 13:
                adj_list->clear();
                std::vector<vertex>().swap(*adj_list);
                root->current->route.clear();
                root->draw->show_warning_map();
                break;
            case 14:
                floorarea_polygons.clear();
                std::vector<std::vector<vec> >().swap(floorarea_polygons);
                floorarea_polygons.push_back(std::vector<vec>(0));
                floorarea_triangles.clear();
                std::vector<std::vector<triangle> >().swap(floorarea_triangles);
                root->draw->calculate_areas(&floorarea_polygons);
                break;
        }
        root->draw->redraw();
        Fl::check();
    }
}


void data_null(void*){
    std::stringstream s;
    s<<"No data selected."<<std::endl;
    push_output(data_text,s.str());
}

void data_points(void* data){
    
    container* root = (container*) data;
    std::vector<vec>* points = &(root->current_data->points);
    
    std::stringstream s;
    
    s.precision(3);
    s.setf( std::ios::fixed, std:: ios::floatfield );
    s<<std::setw(10);
    
    if (!points->size()){
        s<<"Data not available/not yet generated."<<std::endl;
        push_output(data_text,s.str());
        return;
    }
    
    s<<"=================== Points ===================="<<std::endl<<std::setw(10)<<"Point"<<","<<std::setw(10)<<"x"<<","<<std::setw(10)<<"y"<<","<<std::setw(10)<<"z"<<","<<std::setw(10)<<"floor"<<std::endl<<std::endl;
    push_output(data_text,s.str());
    for (unsigned int i=0;i<points->size();i++){
        s.str("");
        s<<std::setw(10)<<i<<","<<std::setw(10)<<(*points)[i].x<<","<<std::setw(10)<<(*points)[i].y<<","<<std::setw(10)<<(*points)[i].z<<","<<std::setw(10)<<(*points)[i].floor<<std::endl;
        std::cout<<i<<","<<std::setw(10)<<(*points)[i].x<<","<<std::setw(10)<<(*points)[i].y<<","<<std::setw(10)<<(*points)[i].z<<","<<std::setw(10)<<(*points)[i].floor<<std::endl;
        push_output(data_text,s.str());
    }
}
void data_matrix(void* data){
    
    container* root = (container*) data;
    std::vector<vec>* points = &(root->current_data->points);
    std::vector<vertex>* adj_list = &(root->current_data->adj_list);
    
    std::stringstream s;
    
    s.precision(3);
    s.setf( std::ios::fixed, std:: ios::floatfield );
    if (!adj_list->size()){
        s<<"Data not available/not yet generated."<<std::endl;
        push_output(data_text,s.str());
        return;
    }
    
    
    s<<"=================== adjacency list ===================="<<std::endl<<std::endl;
    
    push_output(data_text,s.str());
    s.str("");
    
    for (int i=0;i<adj_list->size();i++){
        s<<i<<",\t"<<(*adj_list)[i].start<<",\t"<<(*adj_list)[i].end<<std::endl;
    }
    
    push_output(data_text,s.str());
}

void data_segments(void* data){
    
    container* root = (container*) data;
    map_structures& map = root->current_data->map;
    std::vector<vec>& points = (root->current_data->points);
    
    std::stringstream s;
    
    s.precision(3);
    s.setf( std::ios::fixed, std:: ios::floatfield );
    if (!map.links.size()){
        s<<"Data not available/not yet generated."<<std::endl;
        push_output(data_text,s.str());
        return;
    }
    s<<"=================== Segments/adjacency list ===================="<<std::endl<<std::setw(10)<<"Segment,"<<std::setw(10)<<"Point 1,"<<std::setw(10)<<"Point 2,"<<"Point 1 x,"<<std::setw(10)<<"Point 1 y,"<<std::setw(10)<<" Point 1 z,"<<std::setw(10)<<" Point 2 x,"<<std::setw(10)<<"Point 2 y,"<<std::setw(10)<<"Point 2 z"<<std::endl<<std::endl;
    push_output(data_text,s.str());
    for (unsigned int i=0;i<map.links.size();i++){
        s.str("");
        
        int st = map.links[i].start;
        int e = map.links[i].end;
        
        s<<std::setw(10)<<i<<","<<std::setw(10)<<st<<","<<std::setw(10)<<e<<","<<std::setw(10)<<points[st].x<<","<<std::setw(10)<<points[st].y<<","<<std::setw(10)<<points[st].z<<","<<std::setw(10)<<points[e].x<<","<<std::setw(10)<<points[e].y<<","<<std::setw(10)<<points[e].z<<std::endl;
        
       
        push_output(data_text,s.str());
    }
}


void data_desk(void* data){
    
    container* root = (container*) data;
    std::vector<location>* desks = &(root->current_data->desks);
    
    std::stringstream s;
    
    s.precision(3);
    s.setf( std::ios::fixed, std:: ios::floatfield );
    
    if (!desks->size()){
        s<<"Data not available/not yet generated."<<std::endl;
        push_output(data_text,s.str());
        return;
    }
    s<<"=================== Desk information ===================="<<std::endl<<std::setw(10)<<"Number"<<","<<std::setw(10)<<"Label"<<","<<std::setw(10)<<"point"<<","<<std::setw(10)<<"x"<<","<<std::setw(10)<<"y"<<","<<std::setw(10)<<"z"<<","<<std::setw(10)<<"floor"<<std::endl<<std::endl;
    push_output(data_text,s.str());
    for (unsigned int i=0;i<desks->size();i++){
        s.str("");
        s<<std::setw(10)<<i<<","<<std::setw(10)<< (*desks)[i].label<<","<<std::setw(10)<<(*desks)[i].point<<","<<std::setw(10)<<(*desks)[i].loc.x<<","<<std::setw(10)<<(*desks)[i].loc.y<<","<<std::setw(10)<<(*desks)[i].loc.z<<","<<std::setw(10)<<(*desks)[i].loc.floor<<std::endl;
        push_output(data_text,s.str());
    }
    
}

void data_kitchen(void* data){
    
    container* root = (container*) data;
    std::vector<location>& kitchens = root->current_data->kitchens;
    
    std::stringstream s;
    
    s.precision(3);
    s.setf( std::ios::fixed, std:: ios::floatfield );
    if (!kitchens.size()){
        s<<"Data not available/not yet generated."<<std::endl;
        push_output(data_text,s.str());
        return;
    }
    s<<"=================== Kitchen information ===================="<<std::endl<<" NOTE: destinations with identical spatial map points are considered to be different visibility points of the same destination"<<std::endl<<std::setw(10)<<"Number"<<","<<std::setw(10)<<"Label"<<","<<std::setw(10)<<"point"<<","<<std::setw(10)<<"x"<<","<<std::setw(10)<<"y"<<","<<std::setw(10)<<"z"<<","<<std::setw(10)<<"floor"<<std::endl<<std::endl;
    
    
    
    push_output(data_text,s.str());
    for (unsigned int i=0;i<kitchens.size();i++){
        s.str("");
        s<<std::setw(10)<<i<<","<<std::setw(10)<< kitchens[i].label<<","<<std::setw(10)<<kitchens[i].point<<","<<std::setw(10)<<kitchens[i].loc.x<<","<<std::setw(10)<<kitchens[i].loc.y<<","<<std::setw(10)<<kitchens[i].loc.z<<","<<std::setw(10)<<kitchens[i].loc.floor<<std::endl;
        push_output(data_text,s.str());
    }
    
}

void data_maleWC(void* data){
    
    container* root = (container*) data;
    std::vector<location>& male_toilets = root->current_data->male_toilets;
    
    std::stringstream s;
    
    s.precision(3);
    s.setf( std::ios::fixed, std:: ios::floatfield );
    if (!male_toilets.size()){
        s<<"Data not available/not yet generated."<<std::endl;
        push_output(data_text,s.str());
        return;
    }
    s<<"=================== Male Toilet information ===================="<<std::endl<<std::setw(10)<<"Number"<<","<<std::setw(10)<<"Label"<<","<<std::setw(10)<<"point"<<","<<std::setw(10)<<"x"<<","<<std::setw(10)<<"y"<<","<<std::setw(10)<<"z"<<","<<std::setw(10)<<"floor"<<std::endl<<std::endl;
    push_output(data_text,s.str());
    for (unsigned int i=0;i<male_toilets.size();i++){
        s.str("");
        s<<std::setw(10)<<i<<","<<std::setw(10)<< male_toilets[i].label<<","<<std::setw(10)<<male_toilets[i].point<<","<<std::setw(10)<<male_toilets[i].loc.x<<","<<std::setw(10)<<male_toilets[i].loc.y<<","<<std::setw(10)<<male_toilets[i].loc.z<<","<<std::setw(10)<<male_toilets[i].loc.floor<<std::endl;
        push_output(data_text,s.str());
    }
    
}

void data_femaleWC(void* data){
    
    container* root = (container*) data;
    std::vector<location>& female_toilets = root->current_data->female_toilets;
    
    std::stringstream s;
    
    s.precision(3);
    s.setf( std::ios::fixed, std:: ios::floatfield );
    if (!female_toilets.size()){
        s<<"Data not available/not yet generated."<<std::endl;
        push_output(data_text,s.str());
        return;
    }
    s<<"=================== Female toilet information ===================="<<std::endl<<std::setw(10)<<"Number"<<","<<std::setw(10)<<"Label"<<","<<std::setw(10)<<"point"<<","<<std::setw(10)<<"x"<<","<<std::setw(10)<<"y"<<","<<std::setw(10)<<"z"<<","<<std::setw(10)<<"floor"<<std::endl<<std::endl;
    push_output(data_text,s.str());
    for (unsigned int i=0;i<female_toilets.size();i++){
        s.str("");
        s<<std::setw(10)<<i<<","<<std::setw(10)<< female_toilets[i].label<<","<<std::setw(10)<<female_toilets[i].point<<","<<std::setw(10)<<female_toilets[i].loc.x<<","<<std::setw(10)<<female_toilets[i].loc.y<<","<<std::setw(10)<<female_toilets[i].loc.z<<","<<std::setw(10)<<female_toilets[i].loc.floor<<std::endl;
        push_output(data_text,s.str());
    }
    
}

void data_printer(void* data){
    
    container* root = (container*) data;
    std::vector<location>& printers = root->current_data->printers;
    
    std::stringstream s;
    
    s.precision(3);
    s.setf( std::ios::fixed, std:: ios::floatfield );
    if (!printers.size()){
        s<<"Data not available/not yet generated."<<std::endl;
        push_output(data_text,s.str());
        return;
    }
    s<<"=================== Printer information ===================="<<std::endl<<std::setw(10)<<"Number"<<","<<std::setw(10)<<"Label"<<","<<std::setw(10)<<"point"<<","<<std::setw(10)<<"x"<<","<<std::setw(10)<<"y"<<","<<std::setw(10)<<"z"<<","<<std::setw(10)<<"floor"<<std::endl<<std::endl;
    push_output(data_text,s.str());
    for (unsigned int i=0;i<printers.size();i++){
        s.str("");
        s<<std::setw(10)<<i<<","<<std::setw(10)<< printers[i].label<<","<<std::setw(10)<<printers[i].point<<","<<std::setw(10)<<printers[i].loc.x<<","<<std::setw(10)<<printers[i].loc.y<<","<<std::setw(10)<<printers[i].loc.z<<","<<std::setw(10)<<printers[i].loc.floor<<std::endl;
        push_output(data_text,s.str());
    }
    
}

void data_meetingroom(void* data){
    
    container* root = (container*) data;
    std::vector<location>& meeting_rooms = root->current_data->meeting_rooms;
    
    std::stringstream s;
    
    s.precision(3);
    s.setf( std::ios::fixed, std:: ios::floatfield );
    if (!meeting_rooms.size()){
        s<<"Data not available/not yet generated."<<std::endl;
        push_output(data_text,s.str());
        return;
    }
    s<<"=================== Meeting Room information ===================="<<std::endl<<std::setw(10)<<"Number"<<","<<std::setw(10)<<"Label"<<","<<std::setw(10)<<"point"<<","<<std::setw(10)<<"x"<<","<<std::setw(10)<<"y"<<","<<std::setw(10)<<"z"<<","<<std::setw(10)<<"floor"<<std::endl<<std::endl;
    push_output(data_text,s.str());
    for (unsigned int i=0;i<meeting_rooms.size();i++){
        s.str("");
        s<<std::setw(10)<<i<<","<<std::setw(10)<< meeting_rooms[i].label<<","<<std::setw(10)<<meeting_rooms[i].point<<","<<std::setw(10)<<meeting_rooms[i].loc.x<<","<<std::setw(10)<<meeting_rooms[i].loc.y<<","<<std::setw(10)<<meeting_rooms[i].loc.z<<","<<std::setw(10)<<meeting_rooms[i].loc.floor<<std::endl;
        push_output(data_text,s.str());
    }
    
}

void data_exit(void* data){
    
    container* root = (container*) data;
    std::vector<location>& exits = root->current_data->exits;
    
    std::stringstream s;
    
    s.precision(3);
    s.setf( std::ios::fixed, std:: ios::floatfield );
    if (!exits.size()){
        s<<"Data not available/not yet generated."<<std::endl;
        push_output(data_text,s.str());
        return;
    }
    s<<"=================== Entrance/Exit information ===================="<<std::endl<<"Number"<<","<<std::setw(10)<<"Label"<<","<<std::setw(10)<<"point"<<","<<std::setw(10)<<"x"<<","<<std::setw(10)<<"y"<<","<<std::setw(10)<<"z"<<","<<std::setw(10)<<"floor"<<std::endl<<std::endl;
    push_output(data_text,s.str());
    for (unsigned int i=0;i<exits.size();i++){
        s.str("");
        s<<i<<","<<std::setw(10)<< exits[i].label<<","<<std::setw(10)<<exits[i].point<<","<<std::setw(10)<<exits[i].loc.x<<","<<std::setw(10)<<exits[i].loc.y<<","<<std::setw(10)<<exits[i].loc.z<<","<<std::setw(10)<<exits[i].loc.floor<<std::endl;
        push_output(data_text,s.str());
    }
    
}

void data_stairs(void* data){
    
    container* root = (container*) data;
    std::vector<location>& stairs = root->current_data->stairs;
    
    std::stringstream s;
    
    s.precision(3);
    s.setf( std::ios::fixed, std:: ios::floatfield );
    if (!stairs.size()){
        s<<"Data not available/not yet generated."<<std::endl;
        push_output(data_text,s.str());
        return;
    }
    s<<"=================== Stairs information ===================="<<std::endl<<std::setw(10)<<"Number"<<","<<std::setw(10)<<"Label"<<","<<std::setw(10)<<"point"<<","<<std::setw(10)<<"x"<<","<<std::setw(10)<<"y"<<","<<std::setw(10)<<"z"<<","<<std::setw(10)<<"floor"<<std::endl<<std::endl;
    push_output(data_text,s.str());
    for (unsigned int i=0;i<stairs.size();i++){
        s.str("");
        s<<std::setw(10)<<i<<","<<std::setw(10)<< stairs[i].label<<","<<std::setw(10)<<stairs[i].point<<","<<std::setw(10)<<stairs[i].loc.x<<","<<std::setw(10)<<stairs[i].loc.y<<","<<std::setw(10)<<stairs[i].loc.z<<","<<std::setw(10)<<stairs[i].loc.floor<<std::endl;
        push_output(data_text,s.str());
    }
    
}

void data_lifts(void* data){
    
    container* root = (container*) data;
    std::vector<location>& lifts = root->current_data->lifts;
    
    std::stringstream s;
    
    s.precision(3);
    s.setf( std::ios::fixed, std:: ios::floatfield );
    if (!lifts.size()){
        s<<"Data not available/not yet generated."<<std::endl;
        push_output(data_text,s.str());
        return;
    }
    s<<"=================== Lift information ===================="<<std::endl<<std::setw(10)<<"Number"<<","<<std::setw(10)<<"Label"<<","<<std::setw(10)<<"point"<<","<<std::setw(10)<<"x"<<","<<std::setw(10)<<"y"<<","<<std::setw(10)<<"z"<<","<<std::setw(10)<<"floor"<<std::endl<<std::endl;
    push_output(data_text,s.str());
    for (unsigned int i=0;i<lifts.size();i++){
        s.str("");
        s<<std::setw(10)<<i<<","<<std::setw(10)<< lifts[i].label<<","<<std::setw(10)<<lifts[i].point<<","<<std::setw(10)<<lifts[i].loc.x<<","<<std::setw(10)<<lifts[i].loc.y<<","<<std::setw(10)<<lifts[i].loc.z<<","<<std::setw(10)<<lifts[i].loc.floor<<std::endl;
        push_output(data_text,s.str());
    }
    
}

void data_visibility(void* data){
    
    container* root = (container*) data;
    std::vector<location>& desks = root->current_data->desks;
    std::vector<location>& kitchens = root->current_data->kitchens;
    std::vector<location>& male_toilets = root->current_data->male_toilets;
    std::vector<location>& female_toilets = root->current_data->female_toilets;
    std::vector<location>& printers = root->current_data->printers;
    std::vector<location>& meeting_rooms = root->current_data->meeting_rooms;
    std::vector<location>& exits = root->current_data->exits;
    std::vector<location>& stairs = root->current_data->stairs;
    std::vector<location>& lifts = root->current_data->lifts;

    
    std::stringstream s;
    
    s.precision(3);
    s.setf( std::ios::fixed, std:: ios::floatfield );
    
    if (!desks.size()){
        s<<"Data not available/not yet generated."<<std::endl;
        push_output(data_text,s.str());
        return;
    }
    else if (desks[desks.size()-1].quick_vis.size()!=9){
        s<<"Data not available/not yet generated/out of date. (Re-)Calculate visibility"<<std::endl;
        push_output(data_text,s.str());
        return;
    }
    else if (desks[desks.size()-1].quick_vis[8].size()!=4){
        s<<"Data not available/not yet generated/out of date. (Re-)Calculate visibility"<<std::endl;
        push_output(data_text,s.str());
        return;
    }
    
    
    
    s<<"=================== Visibility information ===================="<<std::endl<<"Remember: each marked desk is a single desk with the position determining its unique visibility information."<<std::endl<<"All other locations can have multiple visibility points indicated by the marked points. This is achieved by having only one location per spatial map point."<<std::endl<<"Key:"<<std::endl<<"    sto = standing opaque barriers"<<std::endl<<"    sta = standing all barriers"<<std::endl<<"    sio = sitting opaque barriers"<<std::endl<<"    sia = sitting all barriers"<<std::endl<<std::setw(11)<<"Desk number"<<","<<std::setw(22)<<"Desk Label"<<","<<std::setw(22)<<"# desks (sia)"<<","<<std::setw(22)<<"# desks (sta)"<<","<<std::setw(22)<<"# desks (sio)"<<","<<std::setw(22)<<"# desks (sto)"<<","<<std::setw(22)<<"# kitchens (sia)"<<","<<std::setw(22)<<"# kitchens (sta)"<<","<<std::setw(22)<<"# kitchens (sio)"<<","<<std::setw(22)<<"# kitchens (sto)"<<","<<std::setw(22)<<    "# male WCs (sia)"<<","<<std::setw(22)<<"# male WCs (sta)"<<","<<std::setw(22)<<"# male WCs (sio)"<<","<<std::setw(22)<<"# male WCs (sto)"<<","<<std::setw(22)<<    "# female WCs (sia)"<<","<<std::setw(22)<<"# female WCs (sta)"<<","<<std::setw(22)<<"# female WCs (sio)"<<","<<std::setw(22)<<"# female WCs (sto)"<<","<<std::setw(22)<<    "# printers (sia)"<<","<<std::setw(22)<<"# printers (sta)"<<","<<std::setw(22)<<"# printers (sio)"<<","<<std::setw(22)<<"# printers (sto)"<<","<<std::setw(22)<<    "# meeting rooms (sia)"<<","<<std::setw(22)<<"# meeting rooms (sta)"<<","<<std::setw(22)<<"# meeting rooms (sio)"<<","<<std::setw(22)<<"# meeting rooms (sto)"<<","<<std::setw(22)<<    "# exits (sia)"<<","<<std::setw(22)<<"# exits (sta)"<<","<<std::setw(22)<<"# exits (sio)"<<","<<std::setw(22)<<"# exits (sto)"<<","<<std::setw(22)<<    "# stairs (sia)"<<","<<std::setw(22)<<"# stairs (sta)"<<","<<std::setw(22)<<"# stairs (sio)"<<","<<std::setw(22)<<"# stairs (sto)"<<","<<std::setw(22)<<    "# lifts (sia)"<<","<<std::setw(22)<<"# lifts (sta)"<<","<<std::setw(22)<<"# lifts (sio)"<<","<<std::setw(22)<<"# lifts (sto)"<<std::endl<<std::endl;

    push_output(data_text,s.str());
    
    
    s.str("");
    
    for (unsigned i=0;i<desks.size();i++){
        
        s<<std::setw(11)<<i<<","<<std::setw(22)<<desks[i].label;
        for (unsigned j=0;j<9;j++){
            for (unsigned k=0;k<4;k++){
                s<<","<<std::setw(22)<<desks[i].quick_vis[j][k];
            }
        }
        s<<std::endl;
        
    }
    push_output(data_text,s.str());
        
}

void data_custom_metric(void* data){
    
    container* root = (container*) data;
    std::vector<double>& custom_metric = root->current_data->custom_metric;
    
    std::stringstream s;
    
    s.precision(3);
    s.setf( std::ios::fixed, std:: ios::floatfield );
    if (!root->current->metrics.size()){
        s<<"Data not available/not yet generated."<<std::endl;
        push_output(data_text,s.str());
        return;
    }
    
    
    s<<"=================== metric information ===================="<<std::endl<<std::endl;//"Desk,\t"<<"value"<<std::endl<<std::endl;
    push_output(data_text,s.str());
    
    
    
    for (unsigned i=0;i<root->current->metric_des.size();i++){
    
        metric_description temp_des = root->current->metric_des[i];
        
    
    s.str("");
        s<<"Metric: "<<i+1<<std::endl;
        s<<"Metric basis:"<<std::endl;
        switch (temp_des.basis){
            case 0:
                s<<"\tDesk based (value for each desk)"<<std::endl;
                break;
            case 1:
                s<<"\tPoint based (value for each spatial map point)"<<std::endl;
                break;
            case 2:
                s<<"\tSegment based (value for each spatial map segment)"<<std::endl;
                break;
        }
        s<<"Metric measure: "<<std::endl;
        s<<"\tQuantity type: ";
        switch (temp_des.metric){
            case 0:
                s<<"Euclidian distance (x,y), non-spatial map distance"<<std::endl;
                break;
            case 1:
                s<<"Euclidian distance (x,y,z), non-spatial map distance"<<std::endl;
                break;
            case 2:
                s<<"(x,y) metric distance, spatial map distance"<<std::endl;
                break;
            case 3:
                s<<"(x,y,z) metric distance, spatial map distance"<<std::endl;
                break;
            case 4:
                s<<"Angular distance"<<std::endl;
                break;
            case 5:
                s<<"Number of segments"<<std::endl;
                break;
            case 6:
                s<<"Number of turns"<<" (Turn definition: individual deviation of more than "<<temp_des.metric_turn<<" degrees)"<<std::endl;
                break;
            case 7:
                s<<"Number of destinations"<<std::endl;
                break;
            case 8:
                s<<"Number of equivalent weighted shortest paths to qualifying destination(s)"<<std::endl;
                break;
            case 9:
                s<<"Number of shortest paths through basis between qualifying destination(s) (reverse route NOT double counted)"<<std::endl;
                break;
            case 10:
                s<<"Betweenness centrality (normalised to [0,1]) and generalised so as measured between qualifying destinations"<<std::endl;
                break;
            case 11:
                s<<"Length of subgraph, in metres, comprised of all shortest paths between all qualifying destinations"<<std::endl;
                break;
            case 12:
                s<<"Eigenvector centrality"<<std::endl;
                break;
        }
        
        if (temp_des.eig_tolerance>-1){
            s<<"\tEigenvector centrality calculation accuracy tolerance: "<<temp_des.eig_tolerance<<" (defined as maximum fractional difference between minimum and maximum estimate of eigenvalue)"<<std::endl;
        }
        if (temp_des.stat>-1){
            s<<"\tStatistic of metric of qualifying destinations: ";
            switch(temp_des.stat){
                case 0:
                    s<<"Minimum"<<std::endl;
                    break;
                case 1:
                    s<<"Maximum"<<std::endl;
                    break;
                case 2:
                    s<<"Mean"<<std::endl;
                    break;
                case 3:
                    s<<"Median"<<std::endl;
                    break;
                case 4:
                    s<<"Geometric mean"<<std::endl;
                    break;
                case 5:
                    s<<"Harmonic mean"<<std::endl;
                    break;
                case 6:
                    s<<"Variance"<<std::endl;
                    break;
            }
        }
        if (temp_des.inverse){
            s<<"\tInverse taken";
            
            if (temp_des.stat>-1){
                s<<" (after statistic calculated)";
            }
            
            s<<std::endl;
        }
        if (temp_des.destination>-1){
            s<<"Destination selection:"<<std::endl;
            s<<"\tDestination type: ";
            if (temp_des.basis==2){
                switch (temp_des.destination){
                    case 0:
                        s<<"Segments"<<std::endl;
                        break;
                    case 1:
                        s<<"Turnless lines"<<std::endl;
                        break;
                }
            }
            else{
                switch (temp_des.destination){
                    case 0:
                        s<<"Desks"<<std::endl;
                        break;
                    case 1:
                        s<<"Kitchens"<<std::endl;
                        break;
                    case 2:
                        s<<"Male WCs"<<std::endl;
                        break;
                    case 3:
                        s<<"Female WCs"<<std::endl;
                        break;
                    case 4:
                        s<<"Printers"<<std::endl;
                        break;
                    case 5:
                        s<<"Meeting Rooms"<<std::endl;
                        break;
                    case 6:
                        s<<"Exits"<<std::endl;
                        break;
                    case 7:
                        s<<"Stairs"<<std::endl;
                        break;
                    case 8:
                        s<<"Lifts"<<std::endl;
                        break;
                    case 9:
                        s<<"Points"<<std::endl;
                        break;
                }
            }
            if (temp_des.range_option){
                s<<"\tInclude destinations that ";
                switch (temp_des.range_option){
                    case 1:
                        s<<"Minimise selection metric"<<std::endl;
                        break;
                    case 2:
                        s<<"Maximise selection metric"<<std::endl;
                        break;
                    case 3:
                        s<<"Are less than or equal to value "<<temp_des.metric_range<<" away from desk/point/segment according to selection metric"<<std::endl;
                        break;
                    case 4:
                        s<<"Are greater than or equal to value "<<temp_des.metric_range<<" away from desk/point/segment according to selection metric"<<std::endl;
                        break;
                    case 5:
                        s<<"Maximise selection metric whilst being less than or equal to value "<<temp_des.metric_range<<" away from desk/point/segment according to selection metric"<<std::endl;
                        break;
                    case 6:
                        s<<"Minimise selection metric whilst being greater than or equal to value "<<temp_des.metric_range<<" away from desk/point/segment according to selection metric"<<std::endl;
                        break;
                    case 7:
                        s<<"Are equal to value "<<temp_des.metric_range<<" away from desk/point/segment according to selection metric"<<std::endl;
                }
            }
            if (temp_des.destination_crit>-1){
                s<<"\tSelection metric: ";
                switch (temp_des.destination_crit){
                    case 0:
                        s<<"Euclidian distance (x,y)"<<std::endl;
                        break;
                    case 1:
                        s<<"Euclidian distance (x,y,z)"<<std::endl;
                        break;
                    case 2:
                        s<<"shortest (x,y) graph distance"<<std::endl;
                        break;
                    case 3:
                        s<<"shortest (x,y,z) graph distance"<<std::endl;
                        break;
                    case 4:
                        s<<"shortest angular distance"<<std::endl;
                        break;
                    case 5:
                        s<<"smallest number of segments"<<std::endl;
                        break;
                    case 6:
                        s<<"smallest number of turns"<<" (Turn definition: individual deviation of more than "<<temp_des.metric_turn<<" degrees)"<<std::endl;
                        break;
                }
            }
                if (temp_des.floor==1){
                    s<<"\tFloor constraint: Destinations must be on the same floor"<<std::endl;
                }
                else if (temp_des.floor==0){
                    s<<"\tFloor constraint: No floor constraint"<<std::endl;
                }
            
                if (temp_des.visible>-1){
                    s<<"Visibility constraint: ";
                    switch (temp_des.visible){
                        case 0:
                            s<<"Destinations must be visible while standing counting both transparent and opaque barriers"<<std::endl;
                            break;
                        case 1:
                            s<<"Destinations must be visible while sitting counting both transparent and opaque barriers"<<std::endl;
                            break;
                        case 2:
                            s<<"Destinations must be visible while standing counting only opaque barriers"<<std::endl;
                            break;
                        case 3:
                            s<<"Destinations must be visible while sitting counting only opaque barriers"<<std::endl;
                            break;
                            
                    }
                }
                if (temp_des.self_count>-1){
                    switch (temp_des.self_count){
                        case 0:
                            s<<"\tDesk/desk's point (desk basis), point (point basis) or segment/segment's turnless lines (segment basis) not included in destinations"<<std::endl;
                            break;
                        case 1:
                            s<<"\tDesk/desk's point (desk basis), point (point basis) or segment/segment's turnless lines (segment basis) included in destinations IF it matches destination type AND passes floor/metric/visibility constraints"<<std::endl;
                            break;
                        case 2:
                            s<<"\tDesk's point (desk basis), point (point basis) or segment/segment's turnless lines (segment basis) added to destinations regardless of destination type or constraints"<<std::endl;
                            break;
                        
                    }
                }
            
        }
     
        if (temp_des.route>-1){
            s<<"Route selection:"<<std::endl;
            s<<"\tRoute defined by minimising metric: ";
            switch (temp_des.route) {
                case 0:
                    s<<"(x,y) graph distance"<<std::endl;
                    break;
                case 1:
                    s<<"(x,y,z) graph distance"<<std::endl;
                    break;
                case 2:
                    s<<"angular distance"<<std::endl;
                    break;
                case 3:
                    s<<"number of segments"<<std::endl;
                    break;
                case 4:
                    s<<"number of turns"<<" (Turn definition: individual deviation of more than "<<temp_des.metric_turn<<" degrees)"<<std::endl;
                    break;
            }
            if (temp_des.iterations>-1){
                s<<"\tNumber of attempts to find equivalent paths: "<<temp_des.iterations<<std::endl;
                if (temp_des.error){
                    s<<"\tWARNING: Maximum number of attempts reached for at least one origin/destination pair"<<std::endl;
                }
                else{
                    s<<"\tNo reported warnings regarding missing paths"<<std::endl;
                }
            }
            if (temp_des.route_break>-1){
                s<<"\tRoute ambiguity breaker: ";
                
                switch (temp_des.route_break){
                    case 0:
                        s<<"Minimum metric of paths";
                        break;
                    case 1:
                        s<<"Maximum metric of paths";
                        break;
                    case 2:
                        s<<"Mean of metrics of paths";
                        break;
                }
                s<<" (When different paths have different final metric values associated with them utilise this value)"<<std::endl;
            }
        }
        
        s<<std::endl<<std::endl;
        push_output(data_text,s.str());
    }
    

    
    
    s.str("");
    s<<std::setw(10)<<"Index"<<","<<std::setw(10)<<" label";
    for (unsigned int i=0;i<root->current->metrics.size();i++){
        std::stringstream ss;
        ss<<"Metric "<<i+1;
        s<<","<<std::setw(10)<<ss.str();
    }
    s<<std::endl<<std::endl;
    push_output(data_text,s.str());
    
    int max=0;
    for (unsigned int i=0;i<root->current->metrics.size();i++){
        
        if (root->current->metrics[i].size()>max){
            max=root->current->metrics[i].size();
        }
    }
    
    s.str("");
    for (unsigned i=0;i<max;i++){
        
        s<<std::setw(10)<<i;
        if (i<root->current->desks.size()){
            s<<","<<std::setw(10)<<root->current->desks[i].label;
        }
        else{
            s<<","<<std::setw(10)<<"N/A";
        }
        
        for (unsigned j=0;j<root->current->metrics.size();j++){
            
            if (i<root->current->metrics[j].size()){
                
                double m =root->current->metrics[j][i];
                if ((m<1000)&&(m>0.1)){
                    s<<","<<std::setw(10)<<m;
                }
                else{
                    s<<","<<std::setw(10)<<std::scientific<<m<<std::fixed;
                }
            }
            else{
                s<<","<<std::setw(10)<<"N/A";
            }
            
        }
        s<<std::endl;
    }
    push_output(data_text,s.str());

    
}

void data_floor_info(void* data){
    
    container* root = (container*) data;
    std::vector<location>& desks = root->current_data->desks;
    std::vector<double>& floor_index = root->current_data->floor_index;
    std::vector<double>& floor_areas = root->current_data->floor_areas;
    std::vector<std::vector<vec> >& floorarea_polygons = root->current_data->floorarea_polygons;
    std::stringstream s;
    
    s.precision(3);
    s.setf( std::ios::fixed, std:: ios::floatfield );
    
    s.precision(3);
    s.setf( std::ios::fixed, std:: ios::floatfield );
    if (floorarea_polygons.size()<2){
        s<<"Data not available/not yet generated."<<std::endl;
        push_output(data_text,s.str());
        return;
    }
    
    std::vector<int> no_desks(floor_index.size(),0);
    
    for (unsigned int i=0;i<desks.size();i++){
        if ((desks[i].loc.floor>=0)&&(desks[i].loc.floor<(int)floor_index.size())){
            no_desks[desks[i].loc.floor]++;
        }
    }
    
    s<<"=================== Floor wide/global metrics ===================="<<std::endl<<"Floor\t"<<"Area\t"<<"No. Desks\t"<<"Density\t"<<std::endl<<std::endl;
    push_output(data_text,s.str());
    
    for (unsigned int i=0;i<no_desks.size();i++){
        if (i<floor_areas.size()){
            s.str("");
            s<<i<<",\t"<<floor_areas[i]<<",\t"<<no_desks[i]<<",\t"<<no_desks[i]/floor_areas[i]<<std::endl;
        }
        push_output(data_text,s.str());
    }
    
    double sum=0;
    for (unsigned int i=0;i<floor_areas.size();i++){
        sum+=floor_areas[i];
    }
    
    s.str("");
    s<<"All"<<",\t"<<sum<<",\t"<<desks.size()<<",\t"<<desks.size()/sum<<std::endl;
    push_output(data_text,s.str());
}


void redraw_data(Fl_Widget* /*widget*/, void* data){
    
    clear_output(data_text);
    
    switch (data_choice->value()){
        case 0: data_null(data); break;
        case 1: data_points(data);break;
        case 2: data_matrix(data);break;
        case 3: data_desk(data); break;
        case 4: data_kitchen(data); break;
        case 5: data_maleWC(data); break;
        case 6: data_femaleWC(data); break;
        case 7: data_printer(data); break;
        case 8: data_meetingroom(data); break;
        case 9: data_exit(data); break;
        case 10: data_stairs(data); break;
        case 11: data_lifts(data); break;
        case 12: data_visibility(data); break;
        case 13: data_custom_metric(data); break;
        case 14: data_floor_info(data); break;
    }
    
    data_text->display->insert_position(0);//moves insert position to the character corresponding to the start of the line that the last character is on
    data_text->display->show_insert_position();
}


void save_data(Fl_Widget*, void* /*data*/){
    
    
    Fl_Native_File_Chooser fnfc;
    fnfc.title("Save file");
    fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
    if ( fnfc.show() ) return;
    
    std::string str=std::string(fnfc.filename());
    
    data_text->buff->outputfile(str.c_str(),0,data_text->buff->length());
    
    push_time(text);
    std::stringstream s;
    s<<"Exported to ";
    s<<str.c_str();
    s<<"\n";
    push_output(text,s.str().c_str());
    
    
    
}


void find_extrema_points(void* data){
    
    container* root = (container*) data;
    std::vector<vec>* points = &(root->current->points);
    
    double &mid_x = root->current->mid_x;
    double &mid_y = root->current->mid_y;
    double &mid_z = root->current->mid_z;
    
    double &minimum_x = root->current->minimum_x;
    double &minimum_y = root->current->minimum_y;
    double &minimum_z = root->current->minimum_z;
    
    double &maximum_x = root->current->maximum_x;
    double &maximum_y = root->current->maximum_y;
    double &maximum_z = root->current->maximum_z;
    
    double minx=9e99,maxx=-9e99,miny=9e99,maxy=-9e99,minz=9e99,maxz=-9e99;
    
    
    for (unsigned int i=0;i<points->size();i++){
        
        if ((*points)[i].x<minx)minx=(*points)[i].x;
        if ((*points)[i].x>maxx)maxx=(*points)[i].x;
        if ((*points)[i].y<miny)miny=(*points)[i].y;
        if ((*points)[i].y>maxy)maxy=(*points)[i].y;
        if ((*points)[i].z<minz)minz=(*points)[i].z;
        if ((*points)[i].z>maxz)maxz=(*points)[i].z;
        
        
    }
    mid_x=0.5*(minx+maxx);
    mid_y=0.5*(miny+maxy);
    mid_z=0.5*(minz+maxz);
    
    
    minimum_z=minz;//-midz;
    maximum_z=maxz;//-midz;
    minimum_x=minx;//-midx;
    maximum_x=maxx;//-midx;
    minimum_y=miny;//-midy;
    maximum_y=maxy;//-midy;
    
    
}

void add_floor_cb(Fl_Widget* widget,void* data){
    
    container* root = (container*) data;
    std::vector<double>& floor_areas = root->current->floor_areas;
    std::vector<double>& floor_index = root->current->floor_index;
    std::vector<plan>& floor_plans = root->current->floor_plans;
    
    
    prepare_undo(data);
    
    std::stringstream s;
    s<<floor_index.size();
    floor_index.push_back(floor_index[floor_index.size()-1]+3);
    floor_areas.push_back(0);
    
    
    floor_choice->add(s.str().c_str());
    plan temp;
    floor_plans.push_back(temp);
    floor_choice->value(floor_index.size());
    xshift->value(floor_plans[floor_choice->value()-1].shift_x);
    yshift->value(floor_plans[floor_choice->value()-1].shift_y);
    root->draw->redraw();
    
    floor_choice_cb(widget,data);
    
}

void undo(Fl_Widget* /*widget*/,void* data){
    
    
    container* root = (container*) data;
    std::vector<vec>& points = (root->current->points);
    std::vector<vertex>& adj_list = (root->current->adj_list);
    
    std::vector<location>& desks = (root->current->desks);
    std::vector<location>& kitchens = (root->current->kitchens);
    std::vector<location>& male_toilets = (root->current->male_toilets);
    std::vector<location>& female_toilets = (root->current->female_toilets);
    std::vector<location>& printers = (root->current->printers);
    std::vector<location>& meeting_rooms = (root->current->meeting_rooms);
    std::vector<location>& exits = (root->current->exits);
    std::vector<location>& stairs = (root->current->stairs);
    std::vector<location>& lifts = (root->current->lifts);
    
    std::vector<wall>& walls_sitting = root->current->walls_sitting;
    std::vector<wall>& walls_standing = root->current->walls_standing;
    
    std::vector<plan>& floor_plans = root->current->floor_plans;
    std::vector<double>& floor_index = root->current->floor_index;
    
    if (root->undo_test){
        
        int i=floor_choice->value();

        
        points=root->old_building.points;
        adj_list=root->old_building.adj_list;
        desks=root->old_building.desks;
        kitchens=root->old_building.kitchens;
        male_toilets=root->old_building.male_toilets;
        female_toilets=root->old_building.female_toilets;
        printers=root->old_building.printers;
        meeting_rooms=root->old_building.meeting_rooms;
        exits=root->old_building.exits;
        stairs=root->old_building.stairs;
        lifts=root->old_building.lifts;
        walls_sitting=root->old_building.walls_sitting;
        walls_standing=root->old_building.walls_standing;
        floor_plans=root->old_building.floor_plans;
        floor_index=root->old_building.floor_index;

        
        root->current->vis_valid=root->old_building.vis_valid;
        root->current->map_valid=root->old_building.map_valid;
        root->current->lines_valid=root->old_building.lines_valid;
        
        floor_choice->clear();
        
        floor_choice->add("All");
        
        std::stringstream s;
        
        for (unsigned int i=0; i<floor_index.size();i++){
            s.str("");
            s<<i;
            floor_choice->add(s.str().c_str());
        }
        
        while (i-1>=(int)floor_index.size()){
            i=i-1;
            if (i<0){i=0;break;}
        }
        floor_choice->value(i);
        floor_choice_cb((Fl_Widget*) NULL, data);
        
        root->draw->first_draw=1;
        root->draw->redraw();
        
        push_time(text);
        push_output(text,"Undone. WARNING: Undo functionality only one step deep. Save your progress and don't rely on undo functionality\n");
    }
    root->undo_test=0;
    
}


void reset_floor_choice(void* data){
    container* root = (container*) data;
    
    std::vector<double>& floor_index = root->current->floor_index;
    
    
    
    
    floor_choice->clear();
    floor_choice->add("All");
    
    std::stringstream s;
    for (unsigned int i=0;i<floor_index.size();i++){
        s.str("");
        s<<i;
        floor_choice->add(s.str().c_str());
    }
    
    zfloor->value(floor_index[0]);
    floor_choice->value(0);
    
    zfloor->deactivate();
    zfloor_label->deactivate();
}


void set_floor_choice(void* data){
    
    container* root = (container*) data;
    std::vector<vec>* points = &(root->current->points);
    
    std::vector<plan>& floor_plans = root->current->floor_plans;
    std::vector<double>& floor_index = root->current->floor_index;
    std::vector<double>& floor_areas = root->current->floor_areas;
    
    
    
    floor_choice->clear();
    floor_index.clear();
    floor_plans.clear();
    
    floor_areas.clear();
    
    floor_choice->add("All");

    int max=0;
    for (unsigned int i=0;i<points->size();i++){
        if ((*points)[i].floor>max) max=(*points)[i].floor;
    }
    
    std::stringstream s;
    for (int i=0; i<=max;i++){
        s.str("");
        s<<i;
        floor_choice->add(s.str().c_str());
        
        plan temp;
        floor_plans.push_back(temp);
        double k;
        k=0;
        floor_index.push_back(k);
        floor_areas.push_back(0);
        
        
    }
    floor_choice->value(0);
    
    zfloor->deactivate();
    zfloor_label->deactivate();

    
    for (unsigned int i=0;i<points->size();i++){
        floor_index[(*points)[i].floor]=(*points)[i].z;
    }
    
    for (unsigned int i=0;i<floor_index.size();i++){
        if (i==0){
            zfloor->value(floor_index[i]);
        }

    }        
    
}

void three_d_cb(Fl_Widget* widget,void* data){
    
    container* root = (container*) data;
    std::vector<double>& floor_index = root->current->floor_index;
    
    
    if (three_d_choice->value()==3){
        Fl::add_idle(idle,data);
    }
    else{
        Fl::remove_idle(idle,data);
    }
    
    
    root->draw->drag_point=-1;
    
    if(three_d_choice->value()){
        
        mark_locations_choice->value(0);
        redraw_mark(widget,data);
        
        if (three_d_choice->value()==3){
            
            if (floor_choice->value()==0){
                floor_choice->value(1);
                floor_choice_cb((Fl_Widget*) NULL, data);
            }
            
            root->draw->ang_xfp=-90;
            root->draw->ang_zfp=0;
            if (floor_choice->value()-1>=0){
                root->draw->zfp=floor_index[floor_choice->value()-1]+1.8;
            }
            else{
                root->draw->zfp=floor_index[0]+1.8;
            }
            root->draw->new_zoom=1;
            root->draw->zoom=1;
            desk_option->value(1);
        }
    }
    
    
    root->draw->redraw();
    
}

void floor_choice_cb(Fl_Widget* widget,void* data){
    
    container* root = (container*) data;
    std::vector<plan>& floor_plans = root->current->floor_plans;
    std::vector<double>& floor_index = root->current->floor_index;
    
    
    if(!floor_choice->value()){

        
        zfloor->deactivate();
        zfloor_label->deactivate();
        mark_locations_choice->value(0);
        redraw_mark(widget,data);
        
    }
    else{
        zfloor->value(floor_index[floor_choice->value()-1]);
        zfloor->activate();
        zfloor_label->activate();
        xshift->value(floor_plans[floor_choice->value()-1].shift_x);
        yshift->value(floor_plans[floor_choice->value()-1].shift_y);
        root->draw->zfp=floor_index[floor_choice->value()-1]+1.8;
    }
    
    root->draw->redraw();
    
}

void redraw_mark(Fl_Widget* /*widget*/,void* data){
    
    container* root = (container*) data;
    THREAD_STATE& calculating = root->calculating;
    std::vector<std::vector<vec> >& floorarea_polygons = root->current->floorarea_polygons;
    
    static int old_val=0;
    
    
    floorarea_polygons.pop_back();
    floorarea_polygons.push_back(std::vector<vec>(0));
    
    root->draw->ang_snap=0;
    
    
    if ((calculating==FREE)||((calculating==CALC_STRUCTURES)&&((mark_locations_choice->value()<=11)||(mark_locations_choice->value()==14)))||((calculating==CALC_VIS)&&(mark_locations_choice->value()>11))){
        
        
        
        if (mark_locations_choice->value()){
        
            root->menu->clear();
            Fl_Menu_Item temp []={{"Visualise metric",FL_COMMAND+'m',show_metric_cb,root,FL_MENU_TOGGLE,0,0,FL_NORMAL_SIZE,FL_BLACK}};//37
            root->items[SHOW_METRIC_INDEX]=temp[0];
            root->menu->menu(root->items);
         
            root->draw_metric=0;
            
        }
        
        if ((old_val!=mark_locations_choice->value())&&(mark_locations_choice->value())){ //deselect if changing item //TRIAL: allow continuous selecting if edit off: ie in 3d modes
            
            root->draw->deselect();
        }
        
        
        if (mark_locations_choice->value()==0){
            
            location_label_in->deactivate();
            point_in->deactivate();
            clear_selected_location->deactivate();
            clear_last_location->deactivate();
            clear_locations->deactivate();
            overwrite_location_point->deactivate();
            location_label->deactivate();
        }
        else{
            location_label_in->activate();
            point_in->activate();
            clear_last_location->deactivate();
            if (mark_locations_choice->value()!=13){
            clear_last_location->activate();
            }
            clear_selected_location->activate();
            
            clear_locations->activate();
            overwrite_location_point->activate();
            location_label->activate();
            
            zfloor->activate();
            zfloor_label->activate();
            
            if (floor_choice->value()==0){
                floor_choice->value(1);
            }
            if (three_d_choice->value()){
                three_d_choice->value(0);
                three_d_choice->do_callback(three_d_choice,data);
            }
            
        }
        
        if (mark_locations_choice->value()==1){
            show_desks->value(1);
           
            
        }
        if (mark_locations_choice->value()==2){
            show_kitchens->value(1);
        }
        if (mark_locations_choice->value()==3){
            show_maleWC->value(1);
        }
        if (mark_locations_choice->value()==4){
            show_femaleWC->value(1);
        }
        if (mark_locations_choice->value()==5){
            show_printers->value(1);
        }
        if (mark_locations_choice->value()==6){
            show_meeting_rooms->value(1);
        }
        if (mark_locations_choice->value()==7){
            show_exits->value(1);
        }
        if (mark_locations_choice->value()==8){
            show_stairs->value(1);
        }
        if (mark_locations_choice->value()==9){
            show_lifts->value(1);
        }
        if (mark_locations_choice->value()==10){
            show_walls_sitting->value(1);
        }
        if (mark_locations_choice->value()==11){
            show_walls_standing->value(1);
        }
        if (mark_locations_choice->value()==12){
            show_map->value(1);
        }
        if (mark_locations_choice->value()==13){
            show_map->value(1);
        }
        if (mark_locations_choice->value()==14){
            show_floor_areas->value(1);
        }
    }
    else{
        
        mark_locations_choice->value(0);
        location_label_in->deactivate();
        point_in->deactivate();
        clear_selected_location->deactivate();
        clear_last_location->deactivate();
        clear_locations->deactivate();
        overwrite_location_point->deactivate();
        location_label->deactivate();
        push_time(text);
        push_output(text,"No editing of constructs being used in calculation!\n");
        
    }
    
    
    root->draw->redraw();
    old_val=mark_locations_choice->value();
}

void scale_floorplan_cb(Fl_Widget* /*widget*/,void* data){
    
    container* root = (container*) data;
    std::vector<plan>& floor_plans = root->current->floor_plans;
    
  
    int floor=floor_choice->value();
    double scale=scale_floorplan->value();
    if ((floor)&&(scale>(1e-10))){
        
  
        floor_plans[floor-1].scale(scale);
        scale_floorplan->value(1);
        std::stringstream s;
        s<<"Scaling floorplan by factor of "<<scale<<".\n";
        push_time(text);
        push_output(text,s.str());
        root->draw->first_draw=1;
        root->draw->redraw();
        
    }
    else if(floor){
        push_time(text);
        push_output(text,"Check scaling parameter!\n");
    }
    else{
        std::stringstream s;
        s<<"Select a floor!\n";
        push_time(text);
        push_output(text,s.str());
    }
    
    
}


void save_binary_cb(Fl_Widget* /*widget*/, void* data){
    
    container* root = (container*) data;
    
    if (!root->calculating){
        
        
        
        Fl_Native_File_Chooser fnfc;
        
        fnfc.preset_file(root->current->save_file_string.c_str());
        
        fnfc.title("Save project");
        fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
        fnfc.filter("*.proj");
        fnfc.options(Fl_Native_File_Chooser::SAVEAS_CONFIRM);
        if ( fnfc.show() ) return;
        
        std::string str=std::string(fnfc.filename());
        
        root->current->save_file_string = str;
        
        container_pass* st_ct = new container_pass;
        st_ct->content = root;
        st_ct->str = str;
        
        root->calculating=SAVING;
        fl_create_thread(thread1,save_binary_thread,st_ct);
#if !defined(WIN32)
        pthread_detach(thread1);
#endif
    }
    
}





void read_binary_cb(Fl_Widget* /*widget*/, void* data){
    
    
    container* root = (container*) data;
    
    if (!root->calculating){
        
        
        Fl_Native_File_Chooser fnfc;
        fnfc.title("Open project");
        fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
        fnfc.filter("*.proj");
        if ( fnfc.show() ) return;
        
        std::string str=std::string(fnfc.filename());                        
        root->current->save_file_string = str;
        
        container_pass* st_ct = new container_pass;
        st_ct->content = root;
        st_ct->str = str;
        
        root->current->clear();
        
        floor_choice->value(0);
        floor_choice_cb(NULL,data);
        if (three_d_choice->value()==3){
            three_d_choice->value(0);
            three_d_cb(NULL,data);
        }
        
        root->draw->redraw();
        root->calculating=LOADING;
        fl_create_thread(thread1,read_binary_thread,st_ct);
#if !defined(WIN32)
        pthread_detach(thread1);
#endif
    }
    
    
    
}


void* save_binary_thread(void* data){

    
    container_pass* st_ct = (container_pass*) data;
    std::string str=st_ct->str;
    container* root = st_ct->content;
    delete st_ct;
    
    
    
    
    building* temp_obj;
    temp_obj = new building;
    
    
    (*temp_obj)=*(root->current);

    file_in_out obj;
    obj.content = (*temp_obj);
    obj.file = str;
    
    int bd;
    for (unsigned int i=0;i<root->comp_buildings.size();i++){
        if (root->current==root->comp_buildings[i]){
            bd=i;
            break;
        }
    }
    
    
    std::stringstream s;
    push_time(text);
    s<<"Writing project in slot "<<bd+1<<" to file `"<<str<<"`..."<<std::endl;
    push_output(text,s.str());
    
    save_binary(&obj);
    
    push_time(text);
    s.str("");
    s<<"...done."<<std::endl;
    push_output(text,s.str());
    
    delete temp_obj;
    root->calculating = FREE;
#if !defined(WIN32)
        return NULL;
    //pthread_exit(NULL);
#else
    return NULL;
#endif
    return data;
    
}

void* read_binary_thread(void* data){
    

    
    
    container_pass* st_ct = (container_pass*) data;
    std::string str=st_ct->str;
    container* root = st_ct->content;
    delete st_ct;
    

    
    file_in_out temp_obj;
    
    temp_obj.file = str;
    
    int bd=-1;
    
    for (unsigned int i=0;i<root->comp_buildings.size();i++){
        if (root->current==root->comp_buildings[i]){
            bd=i;
            break;
        }
    }
    
    std::stringstream s;
    push_time(text);
    s<<"Reading into slot "<<bd+1<<" from project from file `"<<str<<"`..."<<std::endl;
    push_output(text,s.str());
    
    int error=read_binary(&temp_obj);
    
    if (!error){
        
        root->current->points=temp_obj.content.points;
        root->current->adj_list=temp_obj.content.adj_list;
        root->current->map=temp_obj.content.map;
        root->current->custom_metric=temp_obj.content.custom_metric;
        root->current->desks=temp_obj.content.desks;
        root->current->kitchens=temp_obj.content.kitchens;
        root->current->male_toilets=temp_obj.content.male_toilets;
        root->current->female_toilets= temp_obj.content.female_toilets;
        root->current->printers= temp_obj.content.printers;
        root->current->meeting_rooms= temp_obj.content.meeting_rooms;
        root->current->exits=temp_obj.content.exits;
        root->current->stairs=temp_obj.content.stairs;
        root->current->lifts=temp_obj.content.lifts;
        root->current->walls_sitting=temp_obj.content.walls_sitting;
        root->current->walls_standing=temp_obj.content.walls_standing;

        
        
        root->current->mid_x=temp_obj.content.mid_x;
        root->current->mid_y=temp_obj.content.mid_y;
        root->current->mid_z=temp_obj.content.mid_z;
        root->current->minimum_x=temp_obj.content.minimum_x;
        root->current->minimum_y=temp_obj.content.minimum_y;
        root->current->minimum_z=temp_obj.content.minimum_z;
        root->current->maximum_x=temp_obj.content.maximum_x;
        root->current->maximum_y=temp_obj.content.maximum_y;
        root->current->maximum_z=temp_obj.content.maximum_z;
        
        root->current->vis_valid=temp_obj.content.vis_valid;
        root->current->map_valid=temp_obj.content.map_valid;
        root->current->lines_valid=temp_obj.content.lines_valid;
        
        
        if (root->current->vis_valid){
            update_progress(progress_vis,1,0);
            Fl::awake(awake_vis_warning_off,NULL);

        }
        else{
            update_progress(progress_vis,1,2);
            Fl::awake(awake_vis_warning_on,NULL);
            win->redraw();
        }
        
        if (root->current->map_valid){
            update_progress(progress,1,0);
            Fl::awake(awake_map_warning_off,NULL);
            
        }
        else{
            update_progress(progress,1,2);
            Fl::awake(awake_map_warning_on,NULL);
            win->redraw();
        }
        
        
        if (root->current->lines_valid){
            update_progress(progress_lines,1,0);
            Fl::awake(awake_map_warning_off,NULL);;
            
        }
        else{
            update_progress(progress_lines,1,2);
            Fl::awake(awake_map_warning_on,NULL);
            win->redraw();
        }
        
        
        set_cache_arrays(&(root->current->map));
        
        set_floor_choice(root);
        
        root->current->floor_plans=temp_obj.content.floor_plans;
        
        root->current->floor_index=temp_obj.content.floor_index;
        
        
        
        
        root->current->floorarea_polygons=temp_obj.content.floorarea_polygons;
        root->current->floorarea_triangles=temp_obj.content.floorarea_triangles;
        root->current->floor_areas=temp_obj.content.floor_areas;
        
        root->current->route.clear();
        std::vector<std::vector<int> >().swap(root->current->route);
                
        root->draw->first_draw=1;
        Fl::awake(&redraw_from_thread,root);
       
        
        push_time(text);
        s.str("");
        s<<"...done."<<std::endl;
        push_output(text,s.str());
        
    }
    root->calculating = FREE;
#if !defined(WIN32)
        return NULL;
    //pthread_exit(NULL);
#else
    return NULL;
#endif
    
  
    
}


void open_dxf_cb(Fl_Widget* /*widget*/, void* data){
    
    container* root = (container*) data;
    
    
    if (root->calculating){
        push_time(text);
        push_output(text,"Busy! Wait for current process to finish.\n");
        return;
    }
    
    if (floor_choice->value()==0){
        std::stringstream s;
        s<<"Choose a floor to be associated with the floor plan!\n";
        push_time(text);
        push_output(text,s.str());
    }
    else{
        
        Fl_Native_File_Chooser fnfc;
        fnfc.title("Open floorplan");
        fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
        fnfc.filter("*.dxf");
        if ( fnfc.show() ) return;
        
        std::string str=std::string(fnfc.filename());
        

        
        container_pass* st_ct = new container_pass;
        st_ct->content = root;
        st_ct->str = str;
        
        root->calculating=LOADING_DXF;
        fl_create_thread(thread1,open_dxf_thread,st_ct);
#if !defined(WIN32)
        pthread_detach(thread1);
#endif
    }
    
}

void* open_dxf_thread(void * data){
    

    container_pass* st_ct = (container_pass*) data;
    std::string str=st_ct->str;
    container* root = st_ct->content;
    delete st_ct;
    
    std::vector<plan>& floor_plans = root->current->floor_plans;
    
    
    std::stringstream s;
    push_time(text);
    s<<"Opening CAD file `"<<str<<"`..."<<std::endl;
    push_output(text,s.str());
    
    
    
    dxf_parser* dxf_object = new dxf_parser();
    DL_Dxf* dxf = new DL_Dxf();
    
    int i=dxf->in(str.c_str(), dxf_object);
        
    plan floor_plan;
    
    if ((dxf_object->lines.size())&&(i)){
        
        
        floor_plan.layers_name = dxf_object->layers;
        floor_plan.layers_used= dxf_object->layers_used;
        
        
        floor_plan.layers.clear();
        std::vector<int>().swap(floor_plan.layers);
        
        for (unsigned int i=0;i<dxf_object->layers.size();i++){
            floor_plan.layers.push_back(1);
        }
        
        floor_plan.lines_original=dxf_object->lines;
        floor_plan.lines=floor_plan.lines_original;//need this line!
        
        floor_plan.arcs_original=dxf_object->arcs;
        floor_plan.arcs=floor_plan.arcs_original;
        
        floor_plan.circles_original=dxf_object->circles;
        floor_plan.circles=floor_plan.circles_original;
        
        
        
        
        floor_plan.blocks_original=dxf_object->blocks;
        floor_plan.blocks=floor_plan.blocks_original;//need this line! both for scale function in order to have .blocks and .lines intialised
        
        
        floor_plan.block_definitions_original=dxf_object->block_definitions; //in nested only
        floor_plan.block_definitions=floor_plan.block_definitions_original;
        
        //reference block number for nested only
        
        for (unsigned int i=0;i<floor_plan.block_definitions.size();i++){
            
            for (unsigned int j=0;j<floor_plan.block_definitions[i].nested_blocks.size();j++){
                
                floor_plan.block_definitions[i].nested_blocks[j].list_no=-1;
                
                for (unsigned int ii=0;ii<floor_plan.block_definitions.size();ii++){
                    
                    if(!strcmp(floor_plan.block_definitions[i].nested_blocks[j].name.c_str(),floor_plan.block_definitions[ii].name.c_str())){
                        
                        floor_plan.block_definitions[i].nested_blocks[j].list_no=ii;
                        
                    }
                    
                }
                
            }
        }
        
        
        for (unsigned int i=0;i<floor_plan.blocks.size();i++){
            
            floor_plan.blocks[i].list_no=-1;
            
            for (unsigned int ii=0;ii<floor_plan.block_definitions.size();ii++){
                
                if(!strcmp(floor_plan.blocks[i].name.c_str(),floor_plan.block_definitions[ii].name.c_str())){
                    
                    floor_plan.blocks[i].list_no=ii;
                    
                }
                
            }
            
        }
        //end reference block number for nested only
        
        
        
        
        floor_plan.unit_type=dxf_object->units;
        floor_plan.scale();
        
        int floor = floor_choice->value()-1;
        if (floor>=0){
            
            floor_plans[floor]=floor_plan;//contain floor choice within another variable in case it is changed?
            
        }
        
        root->draw->first_draw=1;
        Fl::awake(&redraw_from_thread,root);
     

        
        
    }
    
    
    
    s.str("");
    if (!i){
        s<<"Error reading `"<<str<<"`. File not read!!"<<std::endl;
    }
    else{
        s<<"...done."<<std::endl;
    }
    delete dxf;
    delete dxf_object;
    push_time(text);
    push_output(text,s.str());
    root->calculating=FREE;
#if !defined(WIN32)
        return NULL;
    //pthread_exit(NULL);
#else
    return NULL;
#endif
}



void test(Fl_Widget* /*widget*/,void* data){
    
    MyWindow* t =(MyWindow*) data;
    
    std::cout<<t->w()<<" "<<t->h()<<std::endl;
    
}


void show_route_menu_cb(Fl_Widget* /*widget*/, void* data){
    
    container* root =(container*) data;
    Fl_Double_Window* win = root->win;
    
  
    
    route_display->show();
    route_display->resize(win->x()+0.5*win->w()-135,win->y()+0.5*win->h()-140,280,170);
  
}



void desk_metric_cb(Fl_Widget* widget, void* data){
    container* root = (container*) data;
    Fl_Window* win= root->win;
    
    
    if (root->calculating==FREE){
    
    

    metric_slot_choice->clear();
    
    std::stringstream s;
    for (unsigned i=0;i<root->current->metrics.size();i++){
        s.str("");
        s<<i+1;
        metric_slot_choice->add(s.str().c_str());
    }
    s.str("");
    s<<root->current->metrics.size()+1<<" (new)";
    metric_slot_choice->add(s.str().c_str());
    
    metric_slot_choice->value(root->current->metrics.size());
        
        
        custom_metric_calc->show();
        metric_cb(widget,data);

        custom_metric_calc->resize(win->x()+0.5*win->w()-200,win->y()+0.5*win->h()-314,400,627);
        
    }
    
    
}

void close_desk_metric(Fl_Widget* /*widget*/, void* data){
    
    container* root = (container*) data;
    
    custom_metric_calc->hide();
    if (!custom_metric_basis->value()){
        desk_option->value(0);
        desk_option->do_callback(desk_option,data);
        show_desks->value(1);
        show_desks->do_callback(show_desks,data);
    }
    else{
        show_map->value(1);
        show_map->do_callback(show_desks,data);
    }
    
     root->menu->clear();
    Fl_Menu_Item temp []={{"Visualise metric",FL_COMMAND+'m',show_metric_cb,root,FL_MENU_TOGGLE|FL_MENU_VALUE,0,0,FL_NORMAL_SIZE,FL_BLACK}};//37
    root->items[SHOW_METRIC_INDEX]=temp[0];
    root->menu->menu(root->items);
    
    root->draw_metric=1;
    
    
}

void desk_metric_cb1(Fl_Widget* /*widget*/, void* data){
    
    container* root = (container*) data;
    THREAD_STATE& calculating = root->calculating;
    
    
    if ((calculating == FREE)&&(root->current->map_valid)){
        calculating=CALC_METRIC;
        
        fl_create_thread(thread3,custom_metric_thread,data);
#if !defined(WIN32)
        pthread_detach(thread3);
#endif
        custom_metric_calc->hide();
        
        if (!custom_metric_basis->value()){
            desk_option->value(0);
            desk_option->do_callback(desk_option,data);
            show_desks->value(1);
            show_desks->do_callback(show_desks,data);
        }
        else{
            show_map->value(1);
            show_map->do_callback(show_desks,data);
        }
        
        
        root->menu->clear();
        Fl_Menu_Item temp []={{"Visualise metric",FL_COMMAND+'m',show_metric_cb,root,FL_MENU_TOGGLE|FL_MENU_VALUE,0,0,FL_NORMAL_SIZE,FL_BLACK}};//37
        root->items[SHOW_METRIC_INDEX]=temp[0];
        root->menu->menu(root->items);
         
        root->draw_metric=1;
        
    }
    else if(calculating == FREE){
        push_time(text);
        push_output(text,"Spatial map out of date! Recalculate structures to calculate metrics.\n");
    }
    
    
}


void line_calc_cb(Fl_Widget* /*widget*/, void* data){
    
    container* root = (container*) data;
    THREAD_STATE& calculating = root->calculating;
    
    
    
    if ((calculating == FREE)&&(root->current->map_valid)){
        calculating=CALC_LINES;
        turn_ang->deactivate();
        fl_create_thread(thread3,calculate_lines,data);
#if !defined(WIN32)
        pthread_detach(thread3);
#endif
    
    }
    else if(calculating == FREE){
        push_time(text);
        push_output(text,"Spatial map out of date! Recalculate structures to calculate lines.\n");
    }

}

void* custom_metric_thread(void* data){
    
    container* root = (container*) data;
    
    std::vector<location>* desks = &(root->current->desks);
    std::vector<location>* kitchens = &(root->current->kitchens);
    std::vector<location>* male_toilets = &(root->current->male_toilets);
    std::vector<location>* female_toilets = &(root->current->female_toilets);
    std::vector<location>* printers = &(root->current->printers);
    std::vector<location>* meeting_rooms = &(root->current->meeting_rooms);
    std::vector<location>* exits = &(root->current->exits);
    std::vector<location>* stairs = &(root->current->stairs);
    std::vector<location>* lifts = &(root->current->lifts);
    std::vector<vec>& points = root->current->points;
    
    
    map_structures& map = root->current->map;
    
    std::vector<double>& custom_metric = root->current->custom_metric;
    
    text_output* text = root->buttons.text;

    int choice1,choice2,choice3,choice4,choice5,choice6=0,choice7,choice8,error=0,is_desk=0,error_found1=0,self_count_aware=0,dest_is_points=0,dest_is_lines=0,iter=20,inverse,route_stat,do_cache;
    double range;
    std::vector<int> error_found(9,0);
    
    choice1=destination_criteria_choice->value();
    choice2=route_type_choice->value();
    choice3=custom_metric_choice->value();
    choice4=custom_metric_floor_choice->value();
    choice5=custom_metric_visibility_choice->value();
    choice6=custom_metric_stat_choice->value();
    range = custom_metric_range->value();
    choice7 = custom_metric_range_choice->value();
    choice8 = custom_metric_basis->value();

    route_stat = route_stat_choice->value();
    
    do_cache = cache_metric_data->value();
    
    {
        std::stringstream s;
        s<<max_iterations->value();
        iter = atoi(s.str().c_str());
    }
    
    double tol;
    
    {
        std::stringstream s;
        s<<eig_tol->value();
        tol = atof(s.str().c_str());
    }
    
    
    if (((custom_metric_range->active()))&&(range<0)){
        push_time(text);
        push_output(text,"No negative metric constraints allowed! Metric computation cancelled.\n");
        root->calculating = FREE;
#if !defined(WIN32)
        return NULL;
        //pthread_exit(NULL);
#else
        return NULL;
#endif
    }
    
    if (((max_iterations->active()))&&(iter<0)){
        push_time(text);
        push_output(text,"No negative iteration values allowed! Metric computation cancelled.\n");
        root->calculating = FREE;
#if !defined(WIN32)
        return NULL;
        //pthread_exit(NULL);
#else
        return NULL;
#endif
    }
    
    if (((eig_tol->active()))&&(tol<0)){
        push_time(text);
        push_output(text,"No negative tolerances allowed! Metric computation cancelled.\n");
        root->calculating = FREE;
#if !defined(WIN32)
        return NULL;
        //pthread_exit(NULL);
#else
        return NULL;
#endif
    }
    
    std::vector<location>* destination;
    
    std::vector<location> point_dest,seg_dest,line_dest;
    
    for (int i=0;i<points.size();i++){
        location temp;
        temp.loc.x=points[i].x;
        temp.loc.y=points[i].y;
        temp.loc.z=points[i].z;
        temp.loc.floor=points[i].floor;
        temp.point=i;
        point_dest.push_back(temp);
    }
    
    for (int i=0;i<root->current->map.links.size();i++){ //temp bit: only bit of segment needed is its index ie temp.point (despite not being a point)
        location temp;
        temp.point=i;
        temp.loc.floor=0;
        seg_dest.push_back(temp);
        
    }
    
    for (int i=0;i<root->current->map.line_segments.size();i++){
        location temp;
        temp.point=root->current->map.line_segments[i];
        temp.loc.floor=0;
        line_dest.push_back(temp);
        
    }
    
    if (choice8<2){
    switch (custom_metric_destination_choice->value()){
        case 0:destination=desks;is_desk=1;
            break;
        case 1:destination=kitchens;break;
        case 2:destination=male_toilets;break;
        case 3:destination=female_toilets;break;
        case 4:destination=printers;break;
        case 5:destination=meeting_rooms;break;
        case 6:destination=exits;break;
        case 7:destination=stairs;break;
        case 8:destination=lifts;break;
        case 9:destination=&point_dest;
            dest_is_points=1;
            break;

        default:
            push_time(text);
            push_output(text,"Input error. Check selectors and try again.\n");
            root->calculating = FREE;
#if !defined(WIN32)
                return NULL;
    //pthread_exit(NULL);
#else
            return NULL;
#endif
    }
    }
    else if (choice8==2){ //temp bit
        
        if (custom_metric_destination_choice->value()==0){
        destination=&seg_dest;
        self_count_aware=1;
        }
        else if (custom_metric_destination_choice->value()==1){
            destination=&line_dest;dest_is_lines=1;
        }
        else{
            root->calculating = FREE;
#if !defined(WIN32)
                return NULL;
    //pthread_exit(NULL);
#else
            return NULL;
#endif
        }
    }
    else{
        root->calculating = FREE;
#if !defined(WIN32)
            return NULL;
    //pthread_exit(NULL);
#else
        return NULL;
#endif
    }
    
    inverse = inverse_option->value();
    
    self_count_aware = custom_metric_self_count->value();
    if (self_count_aware==1){
        self_count_aware=0;
    }
    else if (self_count_aware==0){
        self_count_aware=1;
    }
    
    

    custom_metric.clear();
    
    std::vector<double>().swap(custom_metric);
    
    std::stringstream output;
    
    
    
    
    
    std::vector<std::vector<std::vector<double> > > choice_met;
    
    std::vector<std::vector<route_weight_number> > choice;
    
    graph_routes gr_route,gr_dest;
    std::vector<double> weight;
    std::vector<int> visited;
    
    int error_choice=0;
    
    if ((choice3==9)||(choice3==10)||(choice3==11)){
        

        std::vector<std::vector<route_weight_number> >* temp = desk_choice_new(&choice,&map,choice2,choice8,iter,root,error_choice,do_cache);
        
        if (do_cache){
            choice = *temp;
        }
    }
    
    
    push_time(text);
    push_output(text,"Calculating custom metric...\n");
    
    int start_buff=text->buff->length();
    
    
    int size=desks->size();
    
    if (choice8==1){
        size=points.size();
    }
    else if (choice8==2){
        size=map.links.size();
    }
    
    int counter=0;
    
    
    

    
    if (choice3==12){
        
        int ss;
        if (choice8==2){
            ss=map.links.size();
        }
        else{
            ss=map.num_points;
        }
        
        std::vector<double> eigen(ss);
        std::vector<double> e(ss,0);
                      
        
        if (choice8==2){
            power_iteration(eigen,map.S,root,tol,e,1);
        }
        else{
          
            
            power_iteration(eigen,map.G,root,tol,e,1);
        }
        
        if (root->killthread){
            push_time(text);
            push_output(text,"Cancelled.\n");
            root->calculating = FREE;
            root->killthread=0;
            
#if !defined(WIN32)
            return NULL;
            //pthread_exit(NULL);
#else
            return NULL;
#endif
        }
        
     
        for (int i=0;i<size;i++){
            if (!choice8){
                double val=eigen[(*desks)[i].point];
                if (inverse){
                    custom_metric.push_back(1.0/val);
                }
                else{
                    custom_metric.push_back(eigen[(*desks)[i].point]);
                }
            }
            else{
                double val=eigen[i];
                if (inverse){
                    custom_metric.push_back(1.0/val);
                }
                else{
                    custom_metric.push_back(eigen[i]);
                }
            }
        }
        
    }
    
    
    //end testing
    
    if (choice3!=12){
    
    for (unsigned int i=0;i<size;i++){
        
        
        if (root->killthread){
            push_time(text);
            push_output(text,"Cancelled.\n");
            root->calculating = FREE;
            root->killthread=0;
            
#if !defined(WIN32)
                return NULL;
    //pthread_exit(NULL);
#else
            return NULL;
#endif
        }
        
        
        double temp = desk_custom(points,*desks,&map,i,choice1,choice2,choice3,choice4,choice5,choice6,choice7,destination,error,is_desk,range,choice8,self_count_aware,dest_is_points,dest_is_lines,iter,&choice_met,gr_dest,gr_route,weight,visited,inverse,choice,route_stat,do_cache);
        
        
        
        counter++;
        if (((counter)>0.01*size)||((i+1)==size)){
            counter=0;
            Fl::lock();
            text->buff->remove(start_buff,text->buff->length());
            Fl::unlock();
            output.str("");
            output.precision(3);
            output.setf( std::ios::fixed, std:: ios::floatfield );
            output<<"------------------- >> "<<100*((double)(i+1)/(double)(size))<<"% calculated"<<std::endl;
            
            push_output(text,output.str());
        }
        
        
        
        if ((error==1)||(error_choice)){
            if (!error_found[1]){
                error_found[1]=1;
                text->buff->remove(start_buff,text->buff->length());
                push_time(text);
                std::stringstream s;
                s<<"Warning:  For at least one origin-destination pair reached maximum iterations ("<<iter<<")... Check parameters."<<std::endl;
                push_output(text,s.str());
                push_time(text);
                push_output(text,"Large numbers of paths or required iterations indicates existance of zero value loops or that the metric is inappropriate. Check parameters.\n");
                start_buff=text->buff->length();
            }
            error=0;
            
        }
        else if (error==7){
            if (!error_found[7]){
                error_found[7]=1;
                text->buff->remove(start_buff,text->buff->length());
                push_time(text);
                std::stringstream s;
                s<<"Warning:  For at least one origin-destination pair reached max number of iterations ("<<iter<<") and found no non-cyclic routes."<<std::endl;
                push_output(text,s.str());
                push_time(text);
                push_output(text,"Large numbers of paths or required iterations indicates existance of zero value loops or that the metric is inappropriate. Check parameters.\n");
                push_time(text);
                push_output(text,"Desk assigned invalid metric value of -1. Check paramters.\n");
                start_buff=text->buff->length();
            }
            error=0;
        }
        
        else if (error==2){
            push_time(text);
            push_output(text,"Error: destination array empty\n");
            push_time(text);
            push_output(text,"...Failed.\n");
            root->calculating = FREE;
#if !defined(WIN32)
                return NULL;
    //pthread_exit(NULL);
#else
            return NULL;
#endif
        }
        else if (error==3){
            push_time(text);
            push_output(text,"Error: Visibility information incorrect! Check entry/recalculate\n");
            push_time(text);
            push_output(text,"...Failed.\n");;
            root->calculating = FREE;
#if !defined(WIN32)
                return NULL;
    //pthread_exit(NULL);
#else
            return NULL;
#endif
        }
        else if (error==4){
            
            if (!error_found[4]){
                error_found[4]=1;
                text->buff->remove(start_buff,text->buff->length());
                push_time(text);
                push_output(text,"Warning:  Metric not valid for at least one desk/point/segment. Invalid metric takes value -1\n");
                start_buff=text->buff->length();
            }
            error=0;
        }
        else if (error==5){
            
            push_time(text);
            push_output(text,"Input error. Check selectors and try again.\n");
            push_time(text);
            push_output(text,"...Failed.\n");
            root->calculating = FREE;
#if !defined(WIN32)
                return NULL;
    //pthread_exit(NULL);
#else
            return NULL;
#endif
            
            
        }
        else if (error==6){
            if (!error_found[6]){
                error_found[6]=1;
                text->buff->remove(start_buff,text->buff->length());
                push_time(text);
                push_output(text,"Warning:  At east one instance of destination unconnected on spatial map. This will lead to infinite or garbage values. Check spatial map.\n");
                start_buff=text->buff->length();
            }
            error=0;
        }
        
        
        if (!error){
            custom_metric.push_back(temp);

        }
        else{
            //some kind of error dealing
        }
        
    }
    
        
    }
    push_time(text);
    push_output(text,"...done.\n");
    Fl::lock();
    
    
    
    
    
    metric_description temp_des;
    
    temp_des.destination_crit= ((destination_criteria_choice->active())  ?   destination_criteria_choice->value()   :   -1);
    temp_des.route=((route_type_choice->active())  ?   route_type_choice->value()   :   -1);
    temp_des.metric=((custom_metric_choice->active())  ?   custom_metric_choice->value()   :   -1);
    temp_des.visible=((custom_metric_visibility_choice->active())  ?   custom_metric_visibility_choice->value()   :   -1);
    temp_des.floor=((custom_metric_floor_choice->active())  ?   custom_metric_floor_choice->value()   :   -1);
    temp_des.stat=((custom_metric_stat_choice->active())  ?   custom_metric_stat_choice->value()   :   -1);
    temp_des.destination=((custom_metric_destination_choice->active())  ?   custom_metric_destination_choice->value()   :   -1);
    temp_des.range_option=((custom_metric_range_choice->active())  ?   custom_metric_range_choice->value()   :   -1);
    temp_des.basis=custom_metric_basis->value();
    temp_des.route_break = ((route_stat_choice->active())  ?   route_stat_choice->value()   :   -1);
    temp_des.self_count = ((custom_metric_self_count->active())  ?   custom_metric_self_count->value()   :   -1);
    
    temp_des.iterations = ((max_iterations->active())  ?   iter   :   -1);
    temp_des.inverse = inverse;
    
    temp_des.eig_tolerance = ((eig_tol->active())   ?    tol     :    -1);
    temp_des.metric_range = ((custom_metric_range->active())   ?    range    :      -1 );
    temp_des.metric_turn = root->buttons.turn_ang->value();
    
    int error_store=0;
    
    if (error_found[1]){
        error_store=1;
    }
    if (error_found[7]){
        error_store=1;
    }
    
    temp_des.error = error_store;
    
    int ch = metric_slot_choice->value();
    
    if ((ch>=0)&&(ch<root->current->metrics.size())){
        
        
        root->current->metrics[ch]=custom_metric;
        root->current->metric_des[ch]=temp_des;

    }
    else if (ch==root->current->metrics.size()){
        root->current->metrics.push_back(custom_metric);
        root->current->metric_des.push_back(temp_des);

    }

    
    root->graph->histogram(&custom_metric);
    
    Fl::unlock();
    Fl::awake(&redraw_from_thread,root);
    root->calculating = FREE;
#if !defined(WIN32)
        return NULL;
    //pthread_exit(NULL);
#else
    return NULL;
#endif
}


void visibility_cb(Fl_Widget* /*widget*/,void* data){
    
    container* root = (container*) data;
    THREAD_STATE& calculating = root->calculating;
    
    
    if (calculating == FREE){
        calculating=CALC_VIS;
        fl_create_thread(thread1,visibility_calc1,data);
#if !defined(WIN32)
        pthread_detach(thread1);
#endif
        
    }
    
}


void* visibility_calc1(void* data){
    
    
    container* root = (container*) data;
    
    std::vector<location>& desks = (root->current->desks);
    std::vector<location>& kitchens = (root->current->kitchens);
    std::vector<location>& male_toilets = (root->current->male_toilets);
    std::vector<location>& female_toilets = (root->current->female_toilets);
    std::vector<location>& printers = (root->current->printers);
    std::vector<location>& meeting_rooms = (root->current->meeting_rooms);
    std::vector<location>& exits = (root->current->exits);
    std::vector<location>& stairs = (root->current->stairs);
    std::vector<location>& lifts = (root->current->lifts);
    
    std::vector<wall>& walls_sitting = root->current->walls_sitting;
    std::vector<wall>& walls_standing = root->current->walls_standing;
    THREAD_STATE& calculating = root->calculating;
    
    std::stringstream output;
    
    
    
    
    
    if (desks.size()==0){
        
        
        output<<"No desks to calculate visibility for!"<<std::endl;
        
        push_time(text);
        push_output(text,output.str());
        
        calculating = FREE;
     
     
        
     
#if !defined(WIN32)
        return NULL;
     
#else
        return NULL;
#endif
    }
    
    output<<"Starting visibility calculation for "<<desks.size()<<" desks..."<<std::endl;
    
    push_time(text);
    push_output(text,output.str());
    
    
    std::vector<wall> w_sit_o,w_sit_t,w_sitstand_t,w_stand_o,w_stand_t;
    
    for (int i=0;i<walls_standing.size();i++){
        
        if (walls_standing[i].transparent){
            w_stand_t.push_back(walls_standing[i]);
        }
        else{
            w_stand_o.push_back(walls_standing[i]);
        }
    }
    
    
    w_sitstand_t=w_stand_t;
    
    for (int i=0;i<walls_sitting.size();i++){
        
        if (walls_sitting[i].transparent){
            w_sitstand_t.push_back(walls_sitting[i]);
            w_sit_t.push_back(walls_sitting[i]);
        }
        else{
            w_sit_o.push_back(walls_sitting[i]);
        }
    }
    
    int count=0,old_count=0;
    int count_tot=(w_stand_o.size()+w_sit_o.size()+w_stand_t.size()+w_sitstand_t.size())*desks.size()*(desks.size()+kitchens.size()+printers.size()+male_toilets.size()+female_toilets.size()+meeting_rooms.size()+exits.size()+stairs.size()+lifts.size());
    
    
    int no_walls=0;
    if (count_tot==0){
        count_tot = 4*desks.size()*(desks.size()+kitchens.size()+printers.size()+male_toilets.size()+female_toilets.size()+meeting_rooms.size()+exits.size()+stairs.size()+lifts.size());
        no_walls=1;
    }
    
    int start_buff=text->buff->length();
    

    
    std::vector<wall>* w = NULL;
    
    
    for (int choice_v=0;choice_v<4;choice_v++){
        
        
        for (int place_loop=0;place_loop<9;place_loop++){//place_loop<1;//;place_loop<9;place_loop++){
            
            std::vector<location>* loc = NULL;
            
            switch (place_loop){
                case 0:loc=&desks;break;
                case 1:loc=&kitchens; break;
                case 2:loc=&male_toilets;break;
                case 3:loc=&female_toilets;break;
                case 4:loc=&printers;break;
                case 5:loc=&meeting_rooms;break;
                case 6:loc=&exits;break;
                case 7:loc=&stairs;break;
                case 8:loc=&lifts;break;
            }
            
            
            for (int j=0;j<loc->size();j++){                                
                
                std::vector<int>* vis_array = NULL;
                
                switch(choice_v){
                    case 0:
                        vis_array=&((*loc)[j].vis_standing_trans);
                        vis_array->clear();
                        std::vector<int>().swap(*vis_array);
                        {
                            std::vector<int> temp(desks.size(),1);
                            (*vis_array)=temp;
                        }
                        w=&w_stand_o;
                        break;
                    case 1:
                        vis_array=&((*loc)[j].vis_sitting_trans);
                        (*vis_array)=(*loc)[j].vis_standing_trans;
                        w=&w_sit_o;
                        break;
                    case 2:
                        vis_array=&((*loc)[j].vis_standing);
                        (*vis_array)=(*loc)[j].vis_standing_trans;
                        w=&w_stand_t;
                        break;
                    case 3:
                        vis_array=&((*loc)[j].vis_sitting);
                        (*vis_array)=(*loc)[j].vis_sitting_trans;
                        w=&w_sitstand_t;
                        break;
                }
                
                
                
                
                
                
                for (int i=0;i<desks.size();i++){
                    
                    
                    //UPDATE
                    
                    
                    //output begin
                    
                    if (no_walls){                
                        count++;
                    }
                    else{
                        count=count+w->size();
                    }
                    
                    
                    
                    
                    if ((count==count_tot)||((double)(count-old_count)>0.01*count_tot)){
                    
                        old_count=count;
                        
                        Fl::lock();
                        text->buff->remove(start_buff,text->buff->length());
                        Fl::unlock();
                        output.str("");
                        output.precision(3);
                        output.setf( std::ios::fixed, std:: ios::floatfield );
                        output<<"------------------- >> "<<100*((double)count/(double)count_tot)<<"% calculated"<<std::endl;
                        
                        push_output(text,output.str());
                        update_progress(progress_vis,(double)count/(double)count_tot,0);
                        
                    }
                    
                    
                    //END UPDATE
                    
                    
                    
                    
                    if ((*vis_array)[i]==1){
                        
                        if (desks[i].loc.floor==(*loc)[j].loc.floor){
                            
                            
                            double x1,x2,y1,y2,x_1,x_2,y_1,y_2;
                            
                            x1=desks[i].loc.x;
                            y1=desks[i].loc.y;
                            x2=(*loc)[j].loc.x;
                            y2=(*loc)[j].loc.y;                                                        
                            
                            int visible=1;
                            
                            for (unsigned int ii=0;ii<w->size();ii++){
                                
                                if ((*w)[ii].start.floor==desks[i].loc.floor){
                                    
                                    x_1=(*w)[ii].start.x;
                                    x_2=(*w)[ii].end.x;
                                    y_1=(*w)[ii].start.y;
                                    y_2=(*w)[ii].end.y;
                                    
                                    if (!visible_test(x1,x2,y1,y2,x_1,x_2,y_1,y_2)){
                                        visible=0;
                                    }
                                    
                                }
                            }
                            (*vis_array)[i]=visible;
                        }
                        else{
                            (*vis_array)[i]=0;
                        }
                        
                    }
                    
                    
                }
                
            }
            
            
        }
        
        
    }
    

    
    
    //QUICK VIS
    
    Fl::lock();
    
    for (int i=0;i<desks.size();i++){
        
        
        desks[i].quick_vis.clear();
        
        
        for (unsigned k=0;k<9;k++){
            
            
            std::vector<int> temp;
            
            desks[i].quick_vis.push_back(temp);
            
            std::vector<location>* loc = NULL;
            
            switch(k){
                case 0:
                    loc = &desks;
                    break;
                case 1:
                    loc = &kitchens;
                    break;
                case 2:
                    loc = &male_toilets;
                    break;
                case 3:
                    loc = &female_toilets;
                    break;
                case 4:
                    loc = &printers;
                    break;
                case 5:
                    loc = &meeting_rooms;
                    break;
                case 6:
                    loc = &exits;
                    break;
                case 7:
                    loc = &stairs;
                    break;
                case 8:
                    loc = &lifts;
                    break;
                    
            }
            
            std::vector<int> vis_found;
            
            for (unsigned j=0;j<loc->size();j++){
                
                if ((k>0)||(i!=j)){
                    
                    if ((*loc)[j].vis_sitting[i]){
                        
                        int found=0;
                        
                        for (unsigned jj=0;jj<vis_found.size();jj++){
                            
                            if (vis_found[jj]==(*loc)[j].point){
                                found=1;
                                break;
                            }
                            
                        }
                        
                        if (k==0){
                            found=0;
                        }
                        
                        if (!found){
                            vis_found.push_back((*loc)[j].point);
                        }
                        
                    }
                }
                
            }
            
            desks[i].quick_vis[k].push_back(vis_found.size());
            
            
            vis_found.clear();
            
            for (unsigned j=0;j<loc->size();j++){
                
                if ((k>0)||(i!=j)){
                    
                    if ((*loc)[j].vis_standing[i]){
                        
                        int found=0;
                        
                        for (unsigned jj=0;jj<vis_found.size();jj++){
                            
                            if (vis_found[jj]==(*loc)[j].point){
                                found=1;
                                break;
                            }
                            
                        }
                        
                        if (k==0){
                            found=0;
                        }
                        
                        if (!found){
                            vis_found.push_back((*loc)[j].point);
                        }
                        
                    }
                    
                }
                
            }
            
            desks[i].quick_vis[k].push_back(vis_found.size());
            
            vis_found.clear();
            
            for (unsigned j=0;j<loc->size();j++){
                
                if ((k>0)||(i!=j)){
                    
                    if ((*loc)[j].vis_sitting_trans[i]){
                        
                        int found=0;
                        
                        for (unsigned jj=0;jj<vis_found.size();jj++){
                            
                            if (vis_found[jj]==(*loc)[j].point){
                                found=1;
                                break;
                            }
                            
                        }
                        
                        if (k==0){
                            found=0;
                        }
                        
                        if (!found){
                            vis_found.push_back((*loc)[j].point);
                        }
                        
                    }
                    
                }
                
            }
            
            desks[i].quick_vis[k].push_back(vis_found.size());
            
            vis_found.clear();
            
            for (unsigned j=0;j<loc->size();j++){
                
                if ((k>0)||(i!=j)){                                       
                    
                    if ((*loc)[j].vis_standing_trans[i]){
                        
                        int found=0;
                        
                        for (unsigned jj=0;jj<vis_found.size();jj++){
                            
                            if (vis_found[jj]==(*loc)[j].point){
                                found=1;
                                break;
                            }
                            
                        }
                        
                        if (k==0){
                            found=0;
                        }
                        
                        if (!found){
                            vis_found.push_back((*loc)[j].point);
                        }
                        
                    }
                    
                }
                
            }
            
            desks[i].quick_vis[k].push_back(vis_found.size());
            
        }
        
        
    }
    
    Fl::unlock();
    
    //END QUIK VIS
    
    update_progress(progress_vis,(double)count/(double)count_tot,0);
    
    output.str("");
    output<<"...done."<<std::endl;
    push_time(text);
    push_output(text,output.str());
    
    Fl::lock();

    root->current->vis_valid=1;
    calculating = FREE;
    Fl::unlock();
    Fl::awake(awake_vis_warning_off,NULL);
    
    Fl::awake(&check_from_thread,data);
#if !defined(WIN32)
        return NULL;
    //pthread_exit(NULL);
#else
    return NULL;
#endif
    
    
}



int visible_test(double x1,double x2,double y1,double y2,double x_1,double x_2,double y_1,double y_2){
    
    
    
    int visible=1;
    double m1,m2,c1,c2;
    
    m1=(y2-y1)/(x2-x1);
    c1=y1-(m1*x1);
    
    if ((std::isnan(m1))||(std::isinf(m1))){
        c1=x1;
    }
    if (m1==0){
        c1=y1;
    }
    
    m2=(y_2-y_1)/(x_2-x_1);
    c2=y_1-(m2*x_1);
    
    if ((std::isnan(m2))||(std::isinf(m2))){
        c2=x_1;
    }
    if (m2==0){
        c2=y_1;
    }
    
    if (fabs(y2-y1)<0.0000001){m1=0;y2+=0.01;y1-=0.01;}
    if (fabs(y_2-y_1)<0.0000001){m2=0;y_2+=0.01;y_1-=0.01;}

    
    double x=0,y=0;
    
    if (((std::isnan(m2))||(std::isinf(m2)))&&((std::isnan(m1))||(std::isinf(m1)))){
        if ((c2==c1)&&    (  ((y1>=std::min(y_1,y_2))&&(y1<=std::max(y_1,y_2))) || ((y2>=std::min(y_1,y_2))&&(y2<=std::max(y_1,y_2)))  )  ){
            visible=0;
        }
    }
    else if ((m2==0)&&(m1==0)){
        if ((c2==c1)&&    (  ((x1>=std::min(x_1,x_2))&&(x1<=std::max(x_1,x_2))) || ((x2>=std::min(x_1,x_2))&&(x2<=std::max(x_1,x_2)))  )  ){
            visible=0;
        }
    }
    else if ((std::isnan(m1))||(std::isinf(m1))){
        x=c1;
        y=m2*c1+c2;
    }
    else if ((std::isnan(m2))||(std::isinf(m2))){
        x=c2;
        y=m1*c2+c1;
    }
    else if (m1==0){
        y=c1;
        x=(c1-c2)/m2;
    }
    else if (m2==0){
        y=c2;
        x=(c2-c1)/m1;
        
    }
    else{
        x=(c1-c2)/(m2-m1);
        y=m1*((c1-c2)/(m2-m1))+c1;
    }
    
    if ((!std::isnan(m1))&&(!std::isnan(m2))){
        
        if ((y>=std::min(y1,y2))&&(y<=std::max(y1,y2))&&(x>=std::min(x1,x2))&&(x<=std::max(x1,x2))&&(y>=std::min(y_1,y_2))&&(y<=std::max(y_1,y_2))&&(x>=std::min(x_1,x_2))&&(x<=std::max(x_1,x_2))){
            visible=0;
        }
        
    }
    
    return visible;
    
}


void create_structures(Fl_Widget* /*widget*/,void* data){
    
    container* root = (container*) data;
    
    
    if (root->calculating==FREE){
        
        root->calculating=CALC_STRUCTURES;
        root->current->map.clear();
        turn_ang->deactivate();
        zfloor->deactivate();
        fl_create_thread(thread1,calculate_distances_paths,data);
#if !defined(WIN32)
        pthread_detach(thread1);
#endif
    }
    
    
    }





void clear_building(Fl_Widget*,void* data){
    
    
    container* root = (container*) data;
    
    
    
    
    
    int i = fl_ask("Clear entire selection in current building slot?");
    
    if (i){
        
        
        floor_choice->value(0);
        floor_choice_cb(NULL,data);
        if (three_d_choice->value()){
            three_d_choice->value(0);
            three_d_cb(NULL,data);
        }
        root->current->clear();
        root->draw->first_draw=1;
        root->draw->redraw();
    }
}

void* read_desks_pass(void* data){
    
    container* root = (container*) data;
    
    int i=readin_desks(root->current->points,root->current->desks,desks_file,text);
    
    Fl::lock();
    show_desks->value(1);
    Fl::unlock();
    
    
    std::vector<location>& desks = root->current->desks;
    
    for (int i=0;i<desks.size();i++){
        int floor = desks[i].loc.floor;
        desks[i].loc.z = root->current->floor_index[floor];
    }
    
    if (!i) Fl::awake(show_warning_vis_cb,data);
    root->calculating=FREE;
    return NULL;
    
}

void* read_kitchens_pass(void* data){
    
    container* root = (container*) data;
    
    int i=readin_desks(root->current->points,root->current->kitchens,desks_file,text);
    
    Fl::lock();
    show_kitchens->value(1);
    Fl::unlock();
    
    
    std::vector<location>& kitchens = root->current->kitchens;
    
    for (int i=0;i<kitchens.size();i++){
        int floor = kitchens[i].loc.floor;
        kitchens[i].loc.z = root->current->floor_index[floor];
    }
    
    if (!i) Fl::awake(show_warning_vis_cb,data);
    root->calculating=FREE;
    return NULL;
    
}

void* read_printers_pass(void* data){
    
    container* root = (container*) data;
    
    int i=readin_desks(root->current->points,root->current->printers,desks_file,text);
    
    Fl::lock();
    show_printers->value(1);
    Fl::unlock();
    
    
    std::vector<location>& printers = root->current->printers;
    
    for (int i=0;i<printers.size();i++){
        int floor = printers[i].loc.floor;
        printers[i].loc.z = root->current->floor_index[floor];
    }
    
    if (!i) Fl::awake(show_warning_vis_cb,data);
    root->calculating=FREE;
    return NULL;
    
}

void* read_maleWC_pass(void* data){
    
    container* root = (container*) data;
    
    int i=readin_desks(root->current->points,root->current->male_toilets,desks_file,text);
    
    Fl::lock();
    show_maleWC->value(1);
    Fl::unlock();
    
    
    std::vector<location>& male_toilets = root->current->male_toilets;
    
    for (int i=0;i<male_toilets.size();i++){
        int floor = male_toilets[i].loc.floor;
        male_toilets[i].loc.z = root->current->floor_index[floor];
    }
    
    if (!i) Fl::awake(show_warning_vis_cb,data);
    root->calculating=FREE;
    return NULL;
    
}

void* read_femaleWC_pass(void* data){
    
    container* root = (container*) data;
    
    int i=readin_desks(root->current->points,root->current->female_toilets,desks_file,text);
    
    Fl::lock();
    show_femaleWC->value(1);
    Fl::unlock();
    
    
    std::vector<location>& female_toilets = root->current->female_toilets;
    
    for (int i=0;i<female_toilets.size();i++){
        int floor = female_toilets[i].loc.floor;
        female_toilets[i].loc.z = root->current->floor_index[floor];
    }
    
    if (!i) Fl::awake(show_warning_vis_cb,data);
    root->calculating=FREE;
    return NULL;
    
}

void* read_meeting_pass(void* data){
    
    container* root = (container*) data;
    
    int i=readin_desks(root->current->points,root->current->meeting_rooms,desks_file,text);
    
    Fl::lock();
    show_meeting_rooms->value(1);
    Fl::unlock();
    
    
    std::vector<location>& meeting_rooms = root->current->meeting_rooms;
    
    for (int i=0;i<meeting_rooms.size();i++){
        int floor = meeting_rooms[i].loc.floor;
        meeting_rooms[i].loc.z = root->current->floor_index[floor];
    }
    
    if (!i) Fl::awake(show_warning_vis_cb,data);
    root->calculating=FREE;
    return NULL;
    
}

void* read_exit_pass(void* data){
    
    container* root = (container*) data;
    
    int i=readin_desks(root->current->points,root->current->exits,desks_file,text);
    
    Fl::lock();
    show_exits->value(1);
    Fl::unlock();
    
    
    std::vector<location>& exits = root->current->exits;
    
    for (int i=0;i<exits.size();i++){
        int floor = exits[i].loc.floor;
        exits[i].loc.z = root->current->floor_index[floor];
    }
    
    if (!i) Fl::awake(show_warning_vis_cb,data);
    root->calculating=FREE;
    return NULL;
    
}

void* read_stair_pass(void* data){
    
    container* root = (container*) data;
    
    int i=readin_desks(root->current->points,root->current->stairs,desks_file,text);
    
    Fl::lock();
    show_stairs->value(1);
    Fl::unlock();
    
    
    std::vector<location>& stairs = root->current->stairs;
    
    for (int i=0;i<stairs.size();i++){
        int floor = stairs[i].loc.floor;
        stairs[i].loc.z = root->current->floor_index[floor];
    }
    
   if (!i)  Fl::awake(show_warning_vis_cb,data);
    root->calculating=FREE;
    return NULL;
    
}

void* read_lift_pass(void* data){
    
    container* root = (container*) data;
    
    int i=readin_desks(root->current->points,root->current->lifts,desks_file,text);
    
    Fl::lock();
    show_lifts->value(1);
    Fl::unlock();
    
    
    std::vector<location>& lifts = root->current->lifts;
    
    for (int i=0;i<lifts.size();i++){
        int floor = lifts[i].loc.floor;
        lifts[i].loc.z = root->current->floor_index[floor];
    }
    
    if (!i) Fl::awake(show_warning_vis_cb,data);
    
    root->calculating=FREE;
    return NULL;
    
}

void* read_points_pass(void* data){
    
    container* root = (container*) data;
    std::vector<vec>* points = &(root->current->points);
    std::vector<vertex>* adj_list = &(root->current->adj_list);
    
    std::vector<plan>& floor_plans = root->current->floor_plans;
    
    
    //clear everything here

    Fl::lock();
    floor_choice->value(0);
    floor_choice_cb(NULL,data);
    if (three_d_choice->value()==3){
        three_d_choice->value(0);
        three_d_cb(NULL,data);
    }
    
    root->current->clear();
    root->draw->redraw();
    Fl::unlock();
    
    int i=readin_points(*points,points_file,text);

    
  
    find_extrema_points(data);
    Fl::lock();
    set_floor_choice(data);
    show_map->value(1);
    Fl::unlock();
    root->draw->first_draw=1;
    
    
    Fl::awake(show_warning_map_cb,data);
    
    Fl::awake();
    root->calculating=FREE;
#if !defined(WIN32)
        return NULL;
    //pthread_exit(NULL);
#else
    return NULL;
#endif
}

void* read_matrix_pass(void* data){
    
    container* root = (container*) data;
    std::vector<vec>* points = &(root->current->points);
    std::vector<vertex>* adj_list = &(root->current->adj_list);
    
    std::vector<plan>& floor_plans = root->current->floor_plans;
    
    
    
    //clear everything here

    adj_list->clear();
    std::vector<vertex>().swap(*adj_list);
    int i=readin_matrix(*points,*adj_list,matrix_file,text);
    
Fl::lock();
    root->draw->first_draw=1;
    
    Fl::unlock();
    Fl::awake();
    root->calculating=FREE;
#if !defined(WIN32)
        return NULL;
    //pthread_exit(NULL);
#else
    return NULL;
#endif
}


void open_adjlist_cb(Fl_Widget* /*widget*/,void* data) {
    
    container* root = (container*) data;
    
    if (root->calculating){
        push_time(text);
        push_output(text,"Busy! Wait for current process to finish.\n");
        return;
    }
    
    
    int i= fl_ask("Read in adjacency list? Doing so will clear all current links.");
    
    if (i){
        

        Fl_Native_File_Chooser fnfc;
        fnfc.title("Open file");
        fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
        if ( fnfc.show() ) return;
       
        matrix_file=std::string(fnfc.filename());
        std::stringstream s;
        root->calculating=LOADING_ADJ;
        fl_create_thread(thread1,read_adj_list_pass,data);
#if !defined(WIN32)
        pthread_detach(thread1);
#endif
    }
    
    
    
}

void* read_adj_list_pass(void* data){
    
    container* root = (container*) data;
    std::vector<vec>* points = &(root->current->points);
    std::vector<vertex>* adj_list = &(root->current->adj_list);
    
    std::vector<plan>& floor_plans = root->current->floor_plans;
    
   
    
    //clear everything here
    
    adj_list->clear();
    std::vector<vertex>().swap(*adj_list);
    int i=readin_adj_list(*points,*adj_list,matrix_file,text);
    
     Fl::lock();
    root->draw->first_draw=1;
    
    Fl::unlock();
    Fl::awake();
    root->calculating=FREE;
#if !defined(WIN32)
    return NULL;
    //pthread_exit(NULL);
#else
    return NULL;
#endif
}
void open_points_cb(Fl_Widget* /*widget*/,void* data) {
    
    container* root = (container*) data;
    std::vector<vec>* points = &(root->current->points);
    
    if (root->calculating){
        push_time(text);
        push_output(text,"Busy! Wait for current process to finish.\n");
        return;
    }
    
    int i= fl_ask("Read in points? Doing so will reset project and clear all structures.\nThis is intended to start a project as new imported floor\n and point references cannot be ensured across all existing constructs");
    
    if (i){
    
    //if (!check_save()) return;
        Fl_Native_File_Chooser fnfc;
        fnfc.title("Open file");
        fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
        if ( fnfc.show() ) return;
        std::cout<<"LOADING "<<fnfc.filename()<<std::endl;
        points_file=std::string(fnfc.filename());
        std::stringstream s;
        
        root->calculating=LOADING_POINTS;
        fl_create_thread(thread1,read_points_pass,data);
#if !defined(WIN32)
        pthread_detach(thread1);
#endif
    }
    
    
   // s<<"Entered file for points: "<<fnfc.filename()<<std::endl;
   // push_time(text);
   // push_output(text,s.str());
    
}
void open_matrix_cb(Fl_Widget* /*widget*/,void* data) {
    
    container* root = (container*) data;
    
    Fl_Native_File_Chooser fnfc;
    fnfc.title("Open file");
    fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
    if ( fnfc.show() ) return;
    
    std::cout<<"LOADING "<<fnfc.filename()<<std::endl;
    
    matrix_file=std::string(fnfc.filename());
            root->calculating=LOADING_MATRIX;
    fl_create_thread(thread1,read_matrix_pass,data);
#if !defined(WIN32)
    pthread_detach(thread1);
#endif
    
    
}



void open_desks_cb(Fl_Widget* /*widget*/,void* data) {
    
    container* root = (container*) data;
    
    if (root->calculating){
        push_time(text);
        push_output(text,"Busy! Wait for current process to finish.\n");
        return;
    }
    
    int i= fl_ask("Read in desk markers? Doing so will clear all current desks.");
    
    if (i){
        
        //if (!check_save()) return;
        Fl_Native_File_Chooser fnfc;
        fnfc.title("Open file");
        fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
        if ( fnfc.show() ) return;
        std::cout<<"LOADING "<<fnfc.filename()<<std::endl;
        desks_file=std::string(fnfc.filename());
        std::stringstream s;
                root->calculating=LOADING_DESKS;
        fl_create_thread(thread1,read_desks_pass,data);
#if !defined(WIN32)
        pthread_detach(thread1);
#endif
    }
    

    
}

void open_kitchens_cb(Fl_Widget* /*widget*/,void* data) {
    
    container* root = (container*) data;
    
    if (root->calculating){
        push_time(text);
        push_output(text,"Busy! Wait for current process to finish.\n");
        return;
    }
    
    int i= fl_ask("Read in kitchen markers? Doing so will clear all current markers.");
    
    if (i){
        
        //if (!check_save()) return;
        Fl_Native_File_Chooser fnfc;
        fnfc.title("Open file");
        fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
        if ( fnfc.show() ) return;
        std::cout<<"LOADING "<<fnfc.filename()<<std::endl;
        desks_file=std::string(fnfc.filename());
        std::stringstream s;
        
                        root->calculating=LOADING_DESKS;
        fl_create_thread(thread1,read_kitchens_pass,data);
#if !defined(WIN32)
        pthread_detach(thread1);
#endif
    }
    
    
    
}
void open_printers_cb(Fl_Widget* /*widget*/,void* data) {
    
    container* root = (container*) data;
    
    
    if (root->calculating){
        push_time(text);
        push_output(text,"Busy! Wait for current process to finish.\n");
        return;
    }
    
    int i= fl_ask("Read in printer markers? Doing so will clear all current printers.");
    
    if (i){
        
        //if (!check_save()) return;
        Fl_Native_File_Chooser fnfc;
        fnfc.title("Open file");
        fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
        if ( fnfc.show() ) return;
        std::cout<<"LOADING "<<fnfc.filename()<<std::endl;
        desks_file=std::string(fnfc.filename());
        std::stringstream s;
        
                        root->calculating=LOADING_DESKS;
        fl_create_thread(thread1,read_printers_pass,data);
#if !defined(WIN32)
        pthread_detach(thread1);
#endif
    }
    
    
    
}
void open_maleWC_cb(Fl_Widget* /*widget*/,void* data) {
    
    container* root = (container*) data;
    
    
    if (root->calculating){
        push_time(text);
        push_output(text,"Busy! Wait for current process to finish.\n");
        return;
    }
    
    int i= fl_ask("Read in male WC markers? Doing so will clear all current male WCs.");
    
    if (i){
        
        //if (!check_save()) return;
        Fl_Native_File_Chooser fnfc;
        fnfc.title("Open file");
        fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
        if ( fnfc.show() ) return;
        std::cout<<"LOADING "<<fnfc.filename()<<std::endl;
        desks_file=std::string(fnfc.filename());
        std::stringstream s;
        
                        root->calculating=LOADING_DESKS;
        fl_create_thread(thread1,read_maleWC_pass,data);
#if !defined(WIN32)
        pthread_detach(thread1);
#endif
    }
    
    
    
}
void open_femaleWC_cb(Fl_Widget* /*widget*/,void* data) {
    
    container* root = (container*) data;
    
    
    if (root->calculating){
        push_time(text);
        push_output(text,"Busy! Wait for current process to finish.\n");
        return;
    }
    int i= fl_ask("Read in female WCs? Doing so will clear all current female WCs.");
    
    if (i){
        
        //if (!check_save()) return;
        Fl_Native_File_Chooser fnfc;
        fnfc.title("Open file");
        fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
        if ( fnfc.show() ) return;
        std::cout<<"LOADING "<<fnfc.filename()<<std::endl;
        desks_file=std::string(fnfc.filename());
        std::stringstream s;
        
                        root->calculating=LOADING_DESKS;
        fl_create_thread(thread1,read_femaleWC_pass,data);
#if !defined(WIN32)
        pthread_detach(thread1);
#endif
    }
    
    
    
}
void open_meeting_cb(Fl_Widget* /*widget*/,void* data) {
    
    container* root = (container*) data;
    if (root->calculating){
        push_time(text);
        push_output(text,"Busy! Wait for current process to finish.\n");
        return;
    }
    
    int i= fl_ask("Read in meeting room markers? Doing so will clear all current meeting rooms.");
    
    if (i){
        
        //if (!check_save()) return;
        Fl_Native_File_Chooser fnfc;
        fnfc.title("Open file");
        fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
        if ( fnfc.show() ) return;
        std::cout<<"LOADING "<<fnfc.filename()<<std::endl;
        desks_file=std::string(fnfc.filename());
        std::stringstream s;
        
                        root->calculating=LOADING_DESKS;
        fl_create_thread(thread1,read_meeting_pass,data);
#if !defined(WIN32)
        pthread_detach(thread1);
#endif
    }
    
    
    
}
void open_exit_cb(Fl_Widget* /*widget*/,void* data) {
    
    container* root = (container*) data;
    
    if (root->calculating){
        push_time(text);
        push_output(text,"Busy! Wait for current process to finish.\n");
        return;
    }
    int i= fl_ask("Read in exit markers? Doing so will clear all current exits.");
    
    if (i){
        
        //if (!check_save()) return;
        Fl_Native_File_Chooser fnfc;
        fnfc.title("Open file");
        fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
        if ( fnfc.show() ) return;
        std::cout<<"LOADING "<<fnfc.filename()<<std::endl;
        desks_file=std::string(fnfc.filename());
        std::stringstream s;
        
                        root->calculating=LOADING_DESKS;
        fl_create_thread(thread1,read_exit_pass,data);
#if !defined(WIN32)
        pthread_detach(thread1);
#endif
    }
    
    
    
}
void open_stair_cb(Fl_Widget* /*widget*/,void* data) {
    
    container* root = (container*) data;
    
    if (root->calculating){
        push_time(text);
        push_output(text,"Busy! Wait for current process to finish.\n");
        return;
    }
    int i= fl_ask("Read in stair markers? Doing so will clear all current stairs.");
    
    if (i){
        
        //if (!check_save()) return;
        Fl_Native_File_Chooser fnfc;
        fnfc.title("Open file");
        fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
        if ( fnfc.show() ) return;
        std::cout<<"LOADING "<<fnfc.filename()<<std::endl;
        desks_file=std::string(fnfc.filename());
        std::stringstream s;
        
                        root->calculating=LOADING_DESKS;
        fl_create_thread(thread1,read_stair_pass,data);
#if !defined(WIN32)
        pthread_detach(thread1);
#endif
    }
    
    
    
}
void open_lift_cb(Fl_Widget* /*widget*/,void* data) {
    
    container* root = (container*) data;
    if (root->calculating){
        push_time(text);
        push_output(text,"Busy! Wait for current process to finish.\n");
        return;
    }
    
    int i= fl_ask("Read in lift markers? Doing so will clear all current lifts.");
    
    if (i){
        
        //if (!check_save()) return;
        Fl_Native_File_Chooser fnfc;
        fnfc.title("Open file");
        fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
        if ( fnfc.show() ) return;
        std::cout<<"LOADING "<<fnfc.filename()<<std::endl;
        desks_file=std::string(fnfc.filename());
        std::stringstream s;
        
                        root->calculating=LOADING_DESKS;
        fl_create_thread(thread1,read_lift_pass,data);
#if !defined(WIN32)
        pthread_detach(thread1);
#endif
    }
    
    
    
}








