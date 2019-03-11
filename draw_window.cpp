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

#include "untitled.h"

#include "draw_window.h"

#include "floor.h"
#include "ceiling.h"
#include "wall.h"


#ifndef M_PI
    #define M_PI 3.141592653589793238
#endif

#undef min
#undef max


#if defined(WIN32)
    #define SYS_ZOOM 3.0
#else
    #define SYS_ZOOM 3.0
#endif




MyWindow::MyWindow(int X, int Y, int W, int H, const char *L, container* root_): Fl_Gl_Window(X, Y, W, H, L),mark_locations_choice(root_->buttons.mark_locations_choice){//CTOR

    
    
    Fl_Gl_Window::mode(FL_RGB|FL_DOUBLE|FL_DEPTH);
    
    root=root_;
    current = root->current;
    
    pressed_for=0;
    
   xfr=0;yfr=0;zfr=0;
    
    loaded_textures=0;
    
    load_ceiling=0;
    load_wall=0;
    load_floor=0;
    
    set_first=0;
    shift_on=0;
    first_draw=1;
    zoom=0;
    drag=0;
    zoom_mag=0;
    zoom_level=1.0;
    ang_x=0;ang_y=0;ang_z=0;
    
    ang=0;ang_old=0;
    rot[0]=0;rot[1]=0;rot[2]=0;
    old_rot[0]=0;old_rot[1]=0;old_rot[2]=0;
    min_zoom=0.2;
    max_zoom=100/0.01;
    
    image_x=-1.25;//-101;
    image_y=-6.5;//-74;
    image_width=136;
    image_height=36;

    z_gap=0.5;
    selector=-1;
    selected=-1;
    snapon=0;
    snap_point=-1;
    xfp=0;yfp=0;zfp=1.8;
    xfp_move=0;
    yfp_move=0;
    zfp_move=0;
    ang_xfp=-90;
    ang_zfp=0;
    step_length=0.05;
    new_zoom=0;
    start_link=-1;
    end_link=-1;
    selected_matrix[0]=-1;
    selected_matrix[1]=-1;
    minx_overall=-50;
    maxx_overall=50;
    miny_overall=-50;
    maxy_overall=50;
    minx=-50;
    maxx=50;
    miny=-50;
    maxy=50;
    minz=0;
    current->mid_x=0;
    current->mid_y=0;
    current->mid_z=0;
    drag_point=-1;
    ang_snap=0;
    count=0;
    
    
    
}




void MyWindow::load_textures(){
    
    
    
    
    GLfloat* img;
    GLint width,height;
    std::string str;
    std::ofstream myfile;
    
    
    
    
    //floor
    
    
    
    glGenTextures(1, &floor_texture);
    glBindTexture(GL_TEXTURE_2D, floor_texture);
  
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, floor_width1, floor_height1, 0, GL_RGB, GL_FLOAT, floor_pixels1);
    glTexImage2D(GL_TEXTURE_2D, 1, GL_RGB, floor_width2, floor_height2, 0, GL_RGB, GL_FLOAT, floor_pixels2);
    glTexImage2D(GL_TEXTURE_2D, 2, GL_RGB, floor_width3, floor_height3, 0, GL_RGB, GL_FLOAT, floor_pixels3);
    glTexImage2D(GL_TEXTURE_2D, 3, GL_RGB, floor_width4, floor_height4, 0, GL_RGB, GL_FLOAT, floor_pixels4);
    glTexImage2D(GL_TEXTURE_2D, 4, GL_RGB, floor_width5, floor_height5, 0, GL_RGB, GL_FLOAT, floor_pixels5);
    glTexImage2D(GL_TEXTURE_2D, 5, GL_RGB, floor_width6, floor_height6, 0, GL_RGB, GL_FLOAT, floor_pixels6);
    glTexImage2D(GL_TEXTURE_2D, 6, GL_RGB, floor_width7, floor_height7, 0, GL_RGB, GL_FLOAT, floor_pixels7);
    glTexImage2D(GL_TEXTURE_2D, 7, GL_RGB, floor_width8, floor_height8, 0, GL_RGB, GL_FLOAT, floor_pixels8);
    glTexImage2D(GL_TEXTURE_2D, 8, GL_RGB, floor_width9, floor_height9, 0, GL_RGB, GL_FLOAT, floor_pixels9);
  
    

    
    //ceiling
    
    
    
    
    glGenTextures(1, &ceiling_texture);
    glBindTexture(GL_TEXTURE_2D, ceiling_texture);
  
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ceiling_width1, ceiling_height1, 0, GL_RGB, GL_FLOAT, ceiling_pixels1);
    glTexImage2D(GL_TEXTURE_2D, 1, GL_RGB, ceiling_width2, ceiling_height2, 0, GL_RGB, GL_FLOAT, ceiling_pixels2);
    glTexImage2D(GL_TEXTURE_2D, 2, GL_RGB, ceiling_width3, ceiling_height3, 0, GL_RGB, GL_FLOAT, ceiling_pixels3);
    glTexImage2D(GL_TEXTURE_2D, 3, GL_RGB, ceiling_width4, ceiling_height4, 0, GL_RGB, GL_FLOAT, ceiling_pixels4);
    glTexImage2D(GL_TEXTURE_2D, 4, GL_RGB, ceiling_width5, ceiling_height5, 0, GL_RGB, GL_FLOAT, ceiling_pixels5);
    glTexImage2D(GL_TEXTURE_2D, 5, GL_RGB, ceiling_width6, ceiling_height6, 0, GL_RGB, GL_FLOAT, ceiling_pixels6);
    glTexImage2D(GL_TEXTURE_2D, 6, GL_RGB, ceiling_width7, ceiling_height7, 0, GL_RGB, GL_FLOAT, ceiling_pixels7);
    glTexImage2D(GL_TEXTURE_2D, 7, GL_RGB, ceiling_width8, ceiling_height8, 0, GL_RGB, GL_FLOAT, ceiling_pixels8);
    glTexImage2D(GL_TEXTURE_2D, 8, GL_RGB, ceiling_width9, ceiling_height9, 0, GL_RGB, GL_FLOAT, ceiling_pixels9);
    
    //wall
    
    
    glGenTextures(1, &wall_texture);
    glBindTexture(GL_TEXTURE_2D, wall_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wall_width1, wall_height1, 0, GL_RGB, GL_FLOAT, wall_pixels1);
    glTexImage2D(GL_TEXTURE_2D, 1, GL_RGB, wall_width2, wall_height2, 0, GL_RGB, GL_FLOAT, wall_pixels2);
    glTexImage2D(GL_TEXTURE_2D, 2, GL_RGB, wall_width3, wall_height3, 0, GL_RGB, GL_FLOAT, wall_pixels3);
    glTexImage2D(GL_TEXTURE_2D, 3, GL_RGB, wall_width4, wall_height4, 0, GL_RGB, GL_FLOAT, wall_pixels4);
    glTexImage2D(GL_TEXTURE_2D, 4, GL_RGB, wall_width5, wall_height5, 0, GL_RGB, GL_FLOAT, wall_pixels5);
    glTexImage2D(GL_TEXTURE_2D, 5, GL_RGB, wall_width6, wall_height6, 0, GL_RGB, GL_FLOAT, wall_pixels6);
    glTexImage2D(GL_TEXTURE_2D, 6, GL_RGB, wall_width7, wall_height7, 0, GL_RGB, GL_FLOAT, wall_pixels7);
    glTexImage2D(GL_TEXTURE_2D, 7, GL_RGB, wall_width8, wall_height8, 0, GL_RGB, GL_FLOAT, wall_pixels8);
    glTexImage2D(GL_TEXTURE_2D, 8, GL_RGB, wall_width9, wall_height9, 0, GL_RGB, GL_FLOAT, wall_pixels9);
    
}


void MyWindow::calculate_areas(std::vector<std::vector<vec> >* polygons){
    
    
    std::vector<double> temp_area;
    
    for (int i=0;i<current->floor_index.size();i++){
        temp_area.push_back(0);
    }
    
    for (int i=0;i<(*polygons).size()-1;i++){ //we always have an additional dangling polygon for editing
        
        double sum=0;
        
        
        for (int ii=0;ii<(*polygons)[i].size();ii++){
            
            int next=ii+1;
            if (next==(*polygons)[i].size()){
                next=0;
            }
            
            sum+=((*polygons)[i][ii].y+(*polygons)[i][next].y)*((*polygons)[i][next].x-(*polygons)[i][ii].x);
            
        }
        
        sum*=0.5;
        
        if (sum<0){ //cannot guarantee clockwise or counter clockwise
            sum*=-1.0;
        }
        
        int k=(*polygons)[i][0].floor;
        
        if ((k<temp_area.size())&&(k>=0)){
            temp_area[k]+=sum;
        }
        
    }
    
    for (int i=0;i<temp_area.size();i++){
        if (i<current->floor_areas.size()){
            current->floor_areas[i]=temp_area[i];
        }
    }
    
}


void MyWindow::draw_floor(){
    
  
    if (three_d_choice->value()==3){
        
        double start_x = 10.0*((int)((minx_overall-50)/10));
        double start_y = 10.0*((int)((miny_overall-50)/10));
        double end_x = 10.0*((int)((maxx_overall+50)/10));
        double end_y = 10.0*((int)((maxy_overall+50)/10));
        
        
        if((end_x-start_x)<(end_y-start_y)){ //make square
            
        }
        else{
            
            double end_y_temp=end_y+10*((int)(0.5*((end_x-start_x)-(end_y-start_y)))/10);
            double start_y_temp=start_y-10*((int)(0.5*((end_x-start_x)-(end_y-start_y)))/10);
            end_y=end_y_temp;
            start_y=start_y_temp;
            
        }
        
        
        
        for (int i=0;i<current->floor_index.size();i++){
            
            double z=current->floor_index[i];
            
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,floor_texture);
            
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
            
            
            
            
            
            glBegin(GL_TRIANGLES);
            glTexCoord2f(0,0);

            glVertex3f(start_x,start_y,z);
            glTexCoord2f(0,1000);

            glVertex3f(start_x,end_y,z);
            glTexCoord2f(1000,1000);

            glVertex3f(end_x,end_y,z);
            
            glTexCoord2f(0,0);

            glVertex3f(start_x,start_y,z);
            glTexCoord2f(1000,1000);

            glVertex3f(end_x,end_y,z);
            glTexCoord2f(1000,0);

            glVertex3f(end_x,start_y,z);
            
            
            glEnd();
            glDisable(GL_TEXTURE_2D);
        }
    }
}

void MyWindow::draw_ceiling(){
    
   
    if (three_d_choice->value()==3){
        
        
        double start_x = 10.0*((int)((minx_overall-50)/10));
        double start_y = 10.0*((int)((miny_overall-50)/10));
        double end_x = 10.0*((int)((maxx_overall+50)/10));
        double end_y = 10.0*((int)((maxy_overall+50)/10));
       
        if((end_x-start_x)<(end_y-start_y)){ //make square
            
            
            
        }
        else{
            
            double end_y_temp=end_y+10*((int)(0.5*((end_x-start_x)-(end_y-start_y)))/10);
            double start_y_temp=start_y-10*((int)(0.5*((end_x-start_x)-(end_y-start_y)))/10);
            end_y=end_y_temp;
            start_y=start_y_temp;
            
        }
        
        
        for (int i=0;i<current->floor_index.size();i++){
            
            double z=current->floor_index[i]+2.95;
            
            
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,ceiling_texture);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
           
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
            glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
            glColor3ub(255,255,255);
            
            
            glBegin(GL_TRIANGLES);
            glTexCoord2f(0,0);
           
            glVertex3f(start_x,start_y,z);
            glTexCoord2f(0,1000);
           
            glVertex3f(start_x,end_y,z);
            glTexCoord2f(1000,1000);
           
            glVertex3f(end_x,end_y,z);
            
            glTexCoord2f(0,0);
           
            glVertex3f(start_x,start_y,z);
            glTexCoord2f(1000,1000);
           
            glVertex3f(end_x,end_y,z);
            glTexCoord2f(1000,0);
           
            glVertex3f(end_x,start_y,z);
            
            
            glEnd();
                     
            glDisable(GL_TEXTURE_2D);
        }
    }
}



int MyWindow::test_in_triangle(double x, double y,triangle tri){
    double area,s,t;
    
    area=0.5*(-tri.y2*tri.x3 +  tri.y1*(-tri.x2+tri.x3)  +   tri.x1*(tri.y2-tri.y3)  +tri.x2*tri.y3 );
    
   
    
    if (area<0){
        area*=-1;
    }
    
    s = 1.0/(2.0*area)*(tri.y1*tri.x3 - tri.x1*tri.y3 + (tri.y3 - tri.y1)*x + (tri.x1 - tri.x3)*y);
    t = 1.0/(2.0*area)*(tri.x1*tri.y2 - tri.y1*tri.x2 + (tri.y1 - tri.y2)*x + (tri.x2 - tri.x1)*y);
    
    if ((s+t)<=1){
        if ((s<=1)&&(s>=0)&&(t>=0)&&(t<=1)){
            return 1;
        }
        else{
            return 0;
        }
    }
    else{
        return 0;
    }
    
}

bool MyWindow::point_in_polygon(double x, double y,std::vector<vec> polygon) {
    
    int i, j, nvert = polygon.size();
    bool c = false;
    
    for(i = 0, j = nvert - 1; i < nvert; j = i++) { //loops over edges
        if (( ( (polygon[i].y) >= y) != (polygon[j].y >= y) ) &&(x <= (polygon[j].x - polygon[i].x) * (y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x)) {
            
            c = !c;
        }
    }
    
    return c;
}


int MyWindow::pnpoly(double testx, double testy,std::vector<vec> vert)
{
    int nvert=vert.size();
    int i, j, c = 0;
    for (i = 0, j = nvert-1; i < nvert; j = i++) {
        if ( ((vert[i].y>=testy) != (vert[j].y>=testy)) &&
            (testx <= (vert[j].x-vert[i].x) * (testy-vert[i].y) / (vert[j].y-vert[i].y) + vert[i].x) )
            c = !c;
    }
    return c;
}


void MyWindow::ear_clipping(std::vector<vec> polyline,std::vector<triangle>* triangles){
    
    std::vector<vec>polyline_copy=polyline;
    
    triangles->clear();
    
    int old_size = polyline.size();
    
    while (polyline.size()>3){
        
        old_size=polyline.size();
        
        for (int i=0;i<polyline.size();i++){
            
            int i_mid,i_plus,i_minus;
            
            if (i==0){
                i_mid=i;
                i_plus=i+1;
                i_minus=polyline.size()-1;
            }
            else if (i==polyline.size()-1){
                i_mid=i;
                i_plus=0;
                i_minus=i-1;
                
            }
            else{
                i_mid=i;
                i_plus=i+1;
                i_minus=i-1;
            }
            
            triangle temp;
            temp.x1=polyline[i_mid].x;
            temp.y1=polyline[i_mid].y;
            temp.x2=polyline[i_plus].x;
            temp.y2=polyline[i_plus].y;
            temp.x3=polyline[i_minus].x;
            temp.y3=polyline[i_minus].y;
            
            
            
            std::vector<vec> temp_poly;
            temp_poly.clear();
            vec temp_vec;
            
            temp_vec.x=temp.x1;
            temp_vec.y=temp.y1;
            temp_poly.push_back(temp_vec);
            temp_vec.x=temp.x2;
            temp_vec.y=temp.y2;
            temp_poly.push_back(temp_vec);
            temp_vec.x=temp.x3;
            temp_vec.y=temp.y3;
            temp_poly.push_back(temp_vec);
            
            int found_ear=1;
            
            double cx,cy;
            
            cx=(1.0/3.0)*(temp.x1+temp.x2+temp.x3);
            cy=(1.0/3.0)*(temp.y1+temp.y2+temp.y3);
            
            
            
           
            if (!pnpoly(cx,cy,polyline)){
                found_ear=0;
            }
           
            
            if (found_ear){
                
                for (int j=0;j<polyline.size();j++){
                    if ((j!=i_mid)&&(j!=i_plus)&&(j!=i_minus)){
                        
                        
                        
                        
                        if (pnpoly(polyline[j].x,polyline[j].y,temp_poly)){
                            found_ear=0;
                        }
                        
                  
                        
                    }
                }
                
            }
            
            if (found_ear){
                polyline.erase(polyline.begin()+i_mid);
                triangles->push_back(temp);
                break;
            }
            
        }
        
        if (polyline.size()==old_size){ //something is not working! //leave loop
            break;
        }
        
    }
    
    if (polyline.size()==3){
        
        triangle temp;
        temp.x1=polyline[0].x;
        temp.y1=polyline[0].y;
        
        temp.x2=polyline[1].x;
        temp.y2=polyline[1].y;
        
        temp.x3=polyline[2].x;
        temp.y3=polyline[2].y;
        
        triangles->push_back(temp);
        
    }
    
    double sum=0;
    for (int i=0;i<polyline_copy.size();i++){
        int next=i+1;
        if (next==polyline_copy.size()){
            next=0;
        }
        
        sum+=(polyline_copy[i].y+polyline_copy[next].y)*(polyline_copy[next].x-polyline_copy[i].x);
        
    }
    sum*=0.5;
    
    if (sum<0){ //cannot guarantee clockwise or counter clockwise
        sum*=-1.0;
    }
    
  }


void MyWindow::draw_areas(double depth){
    
    if (three_d_choice->value()<3){
        
        if (three_d_choice->value()){
            depth=minz-z_gap+0.01;
        }
        
   
        
        
        if (root->bow){
            glColor4f((220.0/256.0),(256.0/256.0),(220.0/256.0),0.33);
        }
        else{
            glColor4f((220.0/256.0),(256.0/256.0),(220.0/256.0),0.66);
        }
   
        if (root->bow){
            glBlendFunc(GL_ZERO,GL_SRC_COLOR);
        }
        else{
            glBlendFunc(GL_ONE,GL_ONE);
        }
        glEnable( GL_BLEND );
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
  
        
        
        int x,y;
        Fl::get_mouse(x,y);
        double xx,yy;
        
        if (snapon){
            xx=snapx;
            yy=snapy;
        }
        else{
            xx= ((double)(x-win->x()-this->x())/((double)this->w()))*(maxx-minx)+minx;
            yy= ((double)(y-win->y()-this->y())/((double)this->h()))*(miny-maxy)+maxy;
        }
               
        for (int j=0;j<current->floorarea_triangles.size();j++){
            if ((all_flrs)||((floor_choice->value()-1)==(current->floorarea_polygons[j][0].floor))){
                
                if ((selector==14)&&(selected==j)){
                    
                    
                    if (root->bow){
                        glColor4f((255.0/256.0),(255.0/256.0),(0.0/256.0),0.33);
                    }
                    else{
                        glColor4f((75.0/256.0),(75.0/256.0),(0.0/256.0),0.33);
                    }
                }
                else{
                    if (root->bow){
                        glColor4f((220.0/256.0),(256.0/256.0),(220.0/256.0),0.33);
                    }
                    else{
                        glColor4f((0.0/256.0),(50.0/256.0),(0.0/256.0),0.33);
                    }
                }
                
                int fl = current->floorarea_polygons[j][0].floor;
                
                double dd =current->floor_index[fl];

                glBegin(GL_TRIANGLES);
                
                for (int i=0;i<current->floorarea_triangles[j].size();i++){
                    glVertex3f(current->floorarea_triangles[j][i].x1,current->floorarea_triangles[j][i].y1,dd);
                    glVertex3f(current->floorarea_triangles[j][i].x2,current->floorarea_triangles[j][i].y2,dd);
                    glVertex3f(current->floorarea_triangles[j][i].x3,current->floorarea_triangles[j][i].y3,dd);
                }
                glEnd();

            }
        }
                
        glColor3f((255.0/256.0),(0.0/256.0),(0.0/256.0));
        glLineWidth(2.0);
        
        for (int i=0;i<(current->floorarea_polygons.size()-1);i++){
            
            if ((all_flrs)||((floor_choice->value()-1)==(current->floorarea_polygons[i][0].floor))){
                
                //std::cout<<" POLYS "<<current->floorarea_polygons.size()<<" "<<current->points.size()<<" "<<i<<std::endl;
                
                int fl = current->floorarea_polygons[i][0].floor;
                
                double dd =current->floor_index[fl];
                
                
                glBegin(GL_LINE_LOOP);
                for (int j=0;j<current->floorarea_polygons[i].size();j++){
                    glVertex3f(current->floorarea_polygons[i][j].x,current->floorarea_polygons[i][j].y,dd);
                   
                    
                }
                glEnd();
            }
            
        }
        
        glBegin(GL_LINE_STRIP);
        int ii=current->floorarea_polygons.size()-1;
        for (int j=0;j<current->floorarea_polygons[ii].size();j++){
            glVertex3f(current->floorarea_polygons[ii][j].x,current->floorarea_polygons[ii][j].y,depth+1.1);
        }
        if (mark_locations_choice->value()==14){
            glVertex3f(xx,yy,depth);
        }
        glEnd();
        
        
    }

    
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    
}

void MyWindow::draw_floorplan(double depth){
    
    
    if (root->draw_floorplan){
        
     
        glLineWidth(1.0);
        if (root->bow){
            glColor3ub(0,0,0);
        }
        else{
            glColor3ub(255,255,255);
        }
         
        
        for (int i=0;i<current->floor_plans.size();i++){
            
            if ((all_flrs)||(i==floor_choice->value()-1)){
                
                plan floor_plan;
                floor_plan=current->floor_plans[i];
                
                double shift_x=floor_plan.shift_x;
                double shift_y=floor_plan.shift_y;
                
                if (three_d_choice->value()){
                    depth=minz-z_gap+0.01+current->floor_index[i];
                }
 
                
                std::vector<arc> arcs;
                arcs=floor_plan.arcs;
                
                std::vector<circle> circles;
                circles=floor_plan.circles;
                
                std::vector<line_in> lines;
                lines=floor_plan.lines;
                
                std::vector<int> layers;
                layers=floor_plan.layers;
                
             
                
                std::vector<block_entry> blocks;
                blocks = floor_plan.blocks;
                
                for (int i=0;i<arcs.size();i++){
                    
                    if (layers[arcs[i].layer]){
                        
                        arc temp;
                        temp.x=arcs[i].x+shift_x;
                        temp.y=arcs[i].y+shift_y;
                        temp.r=arcs[i].r;
                        temp.ang1=arcs[i].ang1;
                        temp.ang2=arcs[i].ang2;
                        
                        
                        DrawArc(temp,depth);
                    }
                }
                
                for (int i=0;i<circles.size();i++){
                    
                    if (layers[circles[i].layer]){
                        
                        circle temp;
                        temp.x=circles[i].x+shift_x;
                        temp.y=circles[i].y+shift_y;
                        temp.r=circles[i].r;
                        
                        DrawCircle(temp,depth);
                        
                    }
                    
                }
                
                for (int i=0;i<lines.size();i++){
                    
                    if (layers[lines[i].layer]){
                        
                        
                        line_in temp;
                        
                        temp.start.x=lines[i].start.x+shift_x;
                        temp.end.x=lines[i].end.x+shift_x;
                        temp.start.y=lines[i].start.y+shift_y;
                        temp.end.y=lines[i].end.y+shift_y;
                        
                        
                        
                        DrawLines(temp,depth);
                        
                    }
                    
                }
                
                for (int i=0;i<blocks.size();i++){
                    
                    
                    if (layers[blocks[i].layer]){
                        
                        
                        block_entry temp;
                        temp = blocks[i];
                        temp.insert_x+=shift_x; //only need shift_x and shift_y once for the block; apply here
                        temp.insert_y+=shift_y;
                        draw_block_nested(&floor_plan,temp,depth,0);
                        
                        
                        
                    }
                    
                }
                

                
            }
        }
    }
}


void MyWindow::draw_block_nested(plan* floor_plan, block_entry block,double depth,int count){
    
    
    std::vector<line_in> lines;
    std::vector<arc> arcs;
    std::vector<circle> circles;
    std::vector<block_entry> blocks;
    
    
    
    double xs,ys;
    
    if (block.list_no<0){ //no record of the block
        return;
    }
    
   
    
    blocks=floor_plan->block_definitions[block.list_no].nested_blocks; //use data sytructures in relevant block definition
    lines=floor_plan->block_definitions[block.list_no].lines;
    arcs=floor_plan->block_definitions[block.list_no].arcs;
    circles=floor_plan->block_definitions[block.list_no].circles;
    xs = block.insert_x-floor_plan->block_definitions[block.list_no].start_x;//insert_x +ii*rs-start_x;
    ys = block.insert_y-floor_plan->block_definitions[block.list_no].start_y;//;insert_y + jj*cs-start_y;
      glPushMatrix();
    glTranslatef(block.insert_x,block.insert_y,0);
   
    glRotatef(block.rotation,0,0,1);
   
    glTranslatef(-block.insert_x,-block.insert_y,0);
    
    
    
    double scale_x=block.scale_x;
    double scale_y=block.scale_y;
    
    
    for (int i=0;i<arcs.size();i++){
        
        arc temp;
        temp.x=xs+scale_x*arcs[i].x;
        temp.y=ys+scale_y*arcs[i].y;
        temp.r=scale_x*arcs[i].r;
        temp.ang1=arcs[i].ang1;
        temp.ang2=arcs[i].ang2;
        
        
        DrawArc(temp,depth);
        
    }
    
    for (int i=0;i<circles.size();i++){
        
        circle temp;
        temp.x=xs+scale_x*circles[i].x;
        temp.y=ys+scale_y*circles[i].y;
        temp.r=scale_x*circles[i].r; //should be ellipse?
        
        
        DrawCircle(temp,depth);
        
    }
    
    for (int i=0;i<lines.size();i++){
        
        line_in temp;
        
        temp.start.x=xs+scale_x*lines[i].start.x;
        temp.end.x=xs+scale_x*lines[i].end.x;
        temp.start.y=ys+scale_y*lines[i].start.y;
        temp.end.y=ys+scale_y*lines[i].end.y;
        temp.start.z=depth;
        temp.end.z=depth;
        
        
        DrawLines(temp,depth);
        
    }
    if (root->bow){
        glColor3ub(0,0,0);
    }
    else{
        glColor3ub(255,255,255);
    }
    
    
    
    for (int i=0;i<blocks.size();i++){
        
        
        block_entry temp;
        
        temp = blocks[i];
        temp.insert_x+=block.insert_x;
        temp.insert_y+=block.insert_y;
        
        temp.scale_x*=block.scale_x;
        temp.scale_y*=block.scale_y;
        draw_block_nested(floor_plan,temp,depth,count+1);
        
        
    }
    glPopMatrix();
    
}


void MyWindow::DrawLines(line_in data,double depth){
    
    glBegin(GL_LINES);
    glVertex3f(data.start.x,data.start.y,depth);
    glVertex3f(data.end.x,data.end.y,depth);
    glEnd();
    
}


void MyWindow::DrawCircle(circle data,double depth)
{
    
    double cx=data.x;
    double cy=data.y;
    double r=data.r;
    
    
    
    
    
    int num_segments=20+abs((int)40*((2.0*M_PI*r)/(maxx-minx)));
	float theta = 2 * 3.1415926 / float(num_segments);
	float tangetial_factor = tanf(theta);//calculate the tangential factor
    
	float radial_factor = cosf(theta);//calculate the radial factor
	
	float x = r;//we start at angle = 0
    
	float y = 0;
    
	glBegin(GL_LINE_LOOP);
	for(int ii = 0; ii < num_segments; ii++)
	{
		glVertex3f(x + cx, y + cy,depth);//output vertex
        
		//calculate the tangential vector
		//remember, the radial vector is (x, y)
		//to get the tangential vector we flip those coordinates and negate one of them
        
		float tx = -y;
		float ty = x;
        
		//add the tangential vector
        
		x += tx * tangetial_factor;
		y += ty * tangetial_factor;
        
		//correct using the radial factor
        
		x *= radial_factor;
		y *= radial_factor;
	}
	glEnd();
}


void MyWindow::DrawArc(arc data,double depth)
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
    int num_segments=20+abs((int)40*(((ang2-ang1)*r)/(maxx-minx)));
    
    
    glBegin(GL_LINE_STRIP);
    for (int i=0;i<num_segments;i++){
        
        double x= fabs(r)*cos(ang1+i*(ang2-ang1)/((double)num_segments-1.0)); //watch for specific fasb(r) vs not behaviour is x and y: ridiculous format
        double y= r*sin(ang1+i*(ang2-ang1)/((double)num_segments-1.0));
        
        glVertex3f(cx+x,cy+y,depth);
        
    }
    
    glEnd();
    
    
    
}



void MyWindow::draw_visibility(){
    
    if (show_visibility->value()){
        
        
        double height;
      
        if ((show_visibility->value()==1)||(show_visibility->value()==2)||(show_visibility->value()==5)||(show_visibility->value()==6)){
            height=0.8;
            
        }
        else{
            height=1.2;
            
        }
        
        if (root->bow){
            glColor3ub(0,0,0);
        }
        else{
            glColor3ub(255,255,255);
        }
        glLineWidth(1.0);
        glBegin(GL_LINES);
        
        for (int i=0;i<current->desks.size();i++){
            if ((all_flrs)||(current->desks[i].loc.floor==flr_choice)){
                
                
                glVertex3f(current->desks[i].loc.x,current->desks[i].loc.y,current->desks[i].loc.z);
                glVertex3f(current->desks[i].loc.x,current->desks[i].loc.y,current->desks[i].loc.z+height);
                
            }
            
            
            
        }
        glEnd();
        
        std::vector<location>* loc = NULL;
        int draw=0;
        
        for (int place_loop=0;place_loop<9;place_loop++){
            
            switch (place_loop){
                case 0:loc=&current->desks;draw=show_desks->value();break;
                case 1:loc=&current->kitchens;draw=show_kitchens->value(); break;
                case 2:loc=&current->male_toilets;draw=show_maleWC->value();break;
                case 3:loc=&current->female_toilets;draw=show_femaleWC->value();break;
                case 4:loc=&current->printers;draw=show_printers->value();break;
                case 5:loc=&current->meeting_rooms;draw=show_meeting_rooms->value();break;
                case 6:loc=&current->exits;draw=show_exits->value();break;
                case 7:loc=&current->stairs;draw=show_stairs->value();break;
                case 8:loc=&current->lifts;draw=show_lifts->value();break;
            }
            
            
            
            if (draw){
                
                
                for (int i=0;i<(*loc).size();i++){
                    
                    
                    ////std::cout<<i<<" selector "<<selector<<" selected "<<selected<<std::endl;
                                                            
                    
                    std::vector<int> vis;
                    
                    
                    if ((show_visibility->value()==1)||(show_visibility->value()==5)){
                        vis=(*loc)[i].vis_sitting;
                        height=0.8;
                    }
                    else if ((show_visibility->value()==2)||(show_visibility->value()==6)){
                        vis=(*loc)[i].vis_sitting_trans;
                        height=0.8;
                    }
                    else if ((show_visibility->value()==3)||(show_visibility->value()==7)){
                        vis=(*loc)[i].vis_standing;
                        height=1.2;
                    }
                    else if ((show_visibility->value()==4)||(show_visibility->value()==8)){
                        vis=(*loc)[i].vis_standing_trans;
                        height=1.2;
                    }
                    
                    
                    if (root->bow){
                        glColor3ub(0,0,0);
                    }
                    else{
                        glColor3ub(255,255,255);
                    }
                    
                    glLineWidth(1.0);
                    glBegin(GL_LINES);
                    
                    for (int ii=0;ii<(*loc).size();ii++){
                        if ((all_flrs)||((*loc)[ii].loc.floor==flr_choice)){
                            
                            
                            glVertex3f((*loc)[ii].loc.x,(*loc)[ii].loc.y,(*loc)[ii].loc.z);
                            glVertex3f((*loc)[ii].loc.x,(*loc)[ii].loc.y,(*loc)[ii].loc.z+height);
                            
                        }
                        
                        
                        
                    }
                    glEnd();
                    
                    
                    
                    
                    
                    
                    
                    glColor3ub(220,100,220);
                    glLineWidth(1.0);
                    glBegin(GL_LINES);
                    
                    for (int j=0;j<current->desks.size();j++){
                       
                        if ((show_visibility->value()<5)||((show_visibility->value()>4)&&(selector==1)&&(selected==j))){
                       
                            if ((all_flrs)||((*loc)[i].loc.floor==flr_choice)){
                                
                                
                                if (vis.size()>j){
                                    if (vis[j]){
                                        glVertex3f((*loc)[i].loc.x,(*loc)[i].loc.y,(*loc)[i].loc.z+height);
                                        glVertex3f(current->desks[j].loc.x,current->desks[j].loc.y,current->desks[j].loc.z+height);
                                    }
                                }
                                
                            }
                        }
                    }
                    glEnd();
                    
                    
                    
                }
                
            }
            
        }
    }
}



void MyWindow::draw_walls(std::vector<wall> walls_sitting, std::vector<wall> walls_standing){
    

    
    GLfloat light_ambient[] = { 0.12,0.12,0.12, 1.0 };
    GLfloat light_diffuse[] = {1.0,1.0,1.0,1.0};
    GLfloat light_specular[] = {1.0,1.0,1.0,1.0};
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    
    if ((show_walls_sitting->value())||(show_walls_standing->value())){
        
       
        
        std::vector<wall> walls;
        walls.clear();
        
        int highlight=-1;
        
        if ((show_walls_sitting->value())){
            for (int i=0;i<walls_sitting.size();i++){
                if ((all_flrs)||(walls_sitting[i].start.floor==flr_choice)){
                    
                    wall temp = walls_sitting[i];
                    if ((root->transparent_walls)&&(three_d_choice->value())){
                        temp.transparent=1;
                    }
                    
                    walls.push_back(temp);
                    
                    if ((selector==10)&&(i==selected)){
                        highlight=walls.size()-1;
                    }
                    
                }
            }
        }
        
        int sitting_num=walls.size();
        
        if ((show_walls_standing->value())){
            for (int i=0;i<walls_standing.size();i++){
                if ((all_flrs)||(walls_standing[i].start.floor==flr_choice)){
                    
                    wall temp = walls_standing[i];
                    if ((root->transparent_walls)&&(three_d_choice->value())){
                        temp.transparent=1;
                    }
                    
                    walls.push_back(temp);
                    
                    if ((selector==11)&&(i==selected)){
                        highlight=walls.size()-1;
                    }
                }
            }
        }
        
        
        

        
        glLineWidth(1.51);
        double height;

        
        std::vector<triangle> w;
        w.clear();
        w.reserve(2*walls.size());
        
        for (int i=0;i<walls.size();i++){
            
            if (((show_walls_sitting->value()))&&(i<sitting_num)){
                height=1.0;
            }
            else{
                height=3.0;
            }
            
            
            triangle temp,temp1;
            
            //2D
            
            
            temp.x1=walls[i].start.x;
            temp.y1=walls[i].start.y;
            temp.z1=walls[i].start.z-0.1;
            temp.x2=walls[i].end.x;
            temp.y2=walls[i].end.y;
            temp.z2=walls[i].end.z-0.1;
            temp.x3=walls[i].start.x;
            temp.y3=walls[i].start.y;
            temp.z3=walls[i].start.z+height;
            
            temp1.x1=walls[i].start.x;
            temp1.y1=walls[i].start.y;
            temp1.z1=walls[i].start.z+height;
            temp1.x2=walls[i].end.x;
            temp1.y2=walls[i].end.y;
            temp1.z2=walls[i].end.z-0.1;
            temp1.x3=walls[i].end.x;
            temp1.y3=walls[i].end.y;
            temp1.z3=walls[i].end.z+height;
            
            
            
            w.push_back(temp);
            w.push_back(temp1);
            
            
        }
        
        
        
        for (int i=0;i<walls.size();i++){
            
            if (walls[i].transparent==1){
                glColor3ub(100,255,100);
            }
            else{
                glColor3ub(255,100,100);
            }
            
            
            
            if (((all_flrs)||(walls[i].start.floor==flr_choice))&&(!three_d_choice->value())){
                
                if (i==highlight){ //highlighting
                    glColor3ub(255,255,0);
                    
                }
                glBegin(GL_LINES);
                glVertex3f(walls[i].start.x,walls[i].start.y,walls[i].start.z);
                glVertex3f(walls[i].end.x,walls[i].end.y,walls[i].end.z);
                glEnd();
                glColor3ub(255,100,100);
            }
        }
        glColor3ub(100,100,100);
        
        
        
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        
        
        
        
        for (unsigned iii=0;iii<2;iii++){
        
        glBindTexture(GL_TEXTURE_2D,wall_texture);
        
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glEnable(GL_TEXTURE_2D);
        
        
        double cx,cy,cz;int kk=1;
        
        for (int i=0;i<w.size();i++){
            
            
            
            if ((iii==0)&&((walls[(int)(0.5*i)].transparent==1))){
                continue;
            }
            if ((iii==1)&&(walls[(int)(0.5*i)].transparent==0)){
                continue;
            }
            
            
        
            if ((walls[(int)(0.5*i)].transparent==1)){
                glEnable( GL_BLEND );
                
                if (root->bow){
                    glBlendFunc(GL_ZERO,GL_SRC_COLOR);
                    glColor3f((200.0/256.0),(200.0/256.0),(200.0/256.0));
        
                }
                else{
                    glBlendFunc(GL_ONE,GL_ONE);
                    glColor3f((30.0/256.0),(30.0/256.0),(30.0/256.0));
                }
             
                glDisable(GL_LIGHTING);
                glDisable(GL_LIGHT0);
                glDisable(GL_LIGHT1);
                
                glDepthMask(GL_FALSE);
                
                
                glDisable(GL_TEXTURE_2D);
                
            }
            
            {
                
                if (kk>0){
                
                    double ax=w[i].x2-w[i].x1;
                    double ay=w[i].y2-w[i].y1;
                    double az=w[i].z2-w[i].z1;
                    double bx=w[i].x3-w[i].x1;
                    double by=w[i].y3-w[i].y1;
                    double bz=w[i].z3-w[i].z1;
                    
                    cx=ay*bz-by*az;
                    cy=-(ax*bz)+(az*bx);
                    cz=ax*by-bx*ay;
                    
                    double norm=sqrt((cx*cx)+(cy*cy)+(cz*cz));
                    
                    cx/=norm;
                    cy/=norm;
                    cz/=norm;
                    
                }
                kk*=-1;
                
            }
            
            
            
            glBegin(GL_TRIANGLES);
            
            
            
            double length = std::max(std::max(fabs(w[i].x2-w[i].x1),fabs(w[i].x3-w[i].x2)),std::max(fabs(w[i].y2-w[i].y1),fabs(w[i].y3-w[i].y2)));
            length=length/0.05;
            
            glNormal3f(-cx,-cy,-cz);
            
            if (kk<0){
                
                glTexCoord2f(0,0);
                glVertex3f(w[i].x1,w[i].y1,w[i].z1);
                glTexCoord2f(length,0);
                glVertex3f(w[i].x2,w[i].y2,w[i].z2);
                glTexCoord2f(0,60);
                glVertex3f(w[i].x3,w[i].y3,w[i].z3);
                
            }
            else{
                
                
                glTexCoord2f(0,60);
                glVertex3f(w[i].x1,w[i].y1,w[i].z1);
                glTexCoord2f(length,0);
                glVertex3f(w[i].x2,w[i].y2,w[i].z2);
                glTexCoord2f(length,60);
                glVertex3f(w[i].x3,w[i].y3,w[i].z3);
                
                
                
            }
            glEnd();
            
            if ((walls[(int)(0.5*i)].transparent==1)){
                glDisable(GL_BLEND);
            
                glDepthMask(GL_TRUE);
                glEnable(GL_LIGHTING);
                glEnable(GL_LIGHT0);
                glEnable(GL_LIGHT1);
                glEnable(GL_TEXTURE_2D);
            
                glColor3ub(255,255,255);
            }
            else{
                
                
                
            }
            
        }
        
        
        }
        
       
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
        glDisable(GL_TEXTURE_2D);
       
        
        if (set_first){ //draggable wall
       
            if ((all_flrs)||(temp_wall.start.floor==flr_choice)){
                
                int x,y;
                Fl::get_mouse(x,y);
                double xx,yy;
                xx= ((double)(x-win->x()-this->x())/((double)this->w()))*(maxx-minx)+minx;
                yy= ((double)(y-win->y()-this->y())/((double)this->h()))*(miny-maxy)+maxy;
                
                
                if (snapon){  //snapon overwrite
                    xx=snapx;
                    yy=snapy;
                }
                
                
                
                
                glColor3ub(20,235,40);
                
                
                glBegin(GL_LINES);
                glVertex3f(temp_wall.start.x,temp_wall.start.y,temp_wall.start.z);
                glVertex3f(xx,yy,temp_wall.start.z);
                glEnd();
                glBegin(GL_TRIANGLES);
                glVertex3f(temp_wall.start.x,temp_wall.start.y,temp_wall.start.z);
                glVertex3f(xx,yy,temp_wall.start.z);
                glVertex3f(temp_wall.start.x,temp_wall.start.y,temp_wall.start.z+height);
                glVertex3f(temp_wall.start.x,temp_wall.start.y,temp_wall.start.z+height);
                glVertex3f(xx,yy,temp_wall.start.z);
                glVertex3f(xx,yy,temp_wall.start.z+height);
                glEnd();
            }
            
        }
    }
    
}

void MyWindow::draw_locations(std::vector<double> desk_metric,int metric){
    
    
    int i;
    
    if (show_desks->value()){
        
        glLineWidth(1.0);
        
   
        
        if (selector==1){
            i=1;
        }
        else{
            i=0;
        }
        
        
   
        
        if (desk_option->value()==0){
            
            if ((root->draw_metric)&&(metric_type==1)&&(desk_metric.size())){
                double min,max;
                
                min=9e99;
                for (int ii=0;ii<desk_metric.size();ii++){
                    if ((desk_metric[ii]<min)&&(desk_metric[ii]>=0)){
                        min=desk_metric[ii];
                    }
                }
                
                max=*std::max_element(desk_metric.begin(),desk_metric.end());
                draw_locations(&current->desks,&desk_metric,min,max,i);
            }
            else{
                draw_locations(&current->desks,100,100,100,i);
            }
        }
        else{
            draw_desks_model(i);
        }
        
    }
    if (show_kitchens->value()){
        if (selector==2){
            i=1;
        }
        else{
            i=0;
        }
        draw_locations(&current->kitchens,200,100,100,i);
        draw_location_links(&current->kitchens,200,100,100);
    }
    if (show_maleWC->value()){
        if (selector==3){
            i=1;
        }
        else{
            i=0;
        }
        draw_locations(&current->male_toilets,100,200,100,i);
        draw_location_links(&current->male_toilets,100,200,100);
    }
    if (show_femaleWC->value()){
        if (selector==4){
            i=1;
        }
        else{
            i=0;
        }
        draw_locations(&current->female_toilets,100,100,200,i);
        draw_location_links(&current->female_toilets,100,100,200);
    }
    if (show_printers->value()){
        if (selector==5){
            i=1;
        }
        else{
            i=0;
        }
        draw_locations(&current->printers,200,200,100,i);
        draw_location_links(&current->printers,200,200,100);
    }
    if (show_meeting_rooms->value()){
        if (selector==6){
            i=1;
        }
        else{
            i=0;
        }
        draw_locations(&current->meeting_rooms,100,200,200,i);
        draw_location_links(&current->meeting_rooms,100,200,200);
    }
    if (show_exits->value()){
        if (selector==7){
            i=1;
        }
        else{
            i=0;
        }
        draw_locations(&current->exits,200,100,200,i);
        draw_location_links(&current->exits,200,100,200);
    }
    if (show_stairs->value()){
        if (selector==8){
            i=1;
        }
        else{
            i=0;
        }
        draw_locations(&current->stairs,300,200,100,i);
        draw_location_links(&current->stairs,300,200,100);
    }
    if (show_lifts->value()){
        if (selector==9){
            i=1;
        }
        else{
            i=0;
        }
        draw_locations(&current->lifts,100,200,300,i);
        draw_location_links(&current->lifts,100,200,300);
    }
    
}


void MyWindow::draw_locations(std::vector<location>* loc,std::vector<double>* metric,double min,double max, int selection){ //metric version
    
    int x,y;
    double xx,yy;
    double pixelx,pixely;
    int draw=0;
    
        
        Fl::get_mouse(x,y);
        
        xx= ((double)(x-win->x()-this->x())/((double)this->w()))*(maxx-minx)+minx;
        yy= ((double)(y-win->y()-this->y())/((double)this->h()))*(miny-maxy)+maxy;                
        
        pixelx=(maxx-minx)/(double)(this->w());
        pixely=(maxy-miny)/(double)(this->h());
    
    
    
    for (int i=0;i<(*loc).size();i++){
        
        
        if ((all_flrs)||((*loc)[i].loc.floor==flr_choice)){
            if ((i<(*metric).size())){
                
                if ((*metric)[i]>=0){
                    glColor3ub((int)(255*(((*metric)[i]-min)/(max-min))),(int)(255-(255*(((*metric)[i]-min)/(max-min)))),0);
                    if (equal(min,max)){
                        glColor3ub(127,127,0);
                    }
                }
                else{
                    if (root->bow){
                        glColor3ub(0,0,0);
                    }
                    else{
                        glColor3ub(255,255,255);
                    }
                }
            }
            else{
                if (root->bow){
                    glColor3ub(0,0,0);
                }
                else{
                    glColor3ub(255,255,255);
                }
            }
            
            if ((selection)&&(i==selected)){ // selection highlighing
                glColor3ub(255,255,0);
            }
            
            if (show_map->value()){
                glLineWidth(1.0);
                glBegin(GL_LINES);
                
                glVertex3f(current->points[(*loc)[i].point].x,current->points[(*loc)[i].point].y,current->floor_index[current->points[(*loc)[i].point].floor]);
                glVertex3f((*loc)[i].loc.x,(*loc)[i].loc.y,current->floor_index[(*loc)[i].loc.floor]);
                glEnd();
            }
            
            double size =0.6;
            drawcube((*loc)[i].loc.x,(*loc)[i].loc.y,(*loc)[i].loc.z,size);            
            
            
            if ((!draw)&&(i<(*metric).size())&&(!three_d_choice->value())){
                
                if ((fabs(xx-(*loc)[i].loc.x)<0.5*size)&& (fabs(yy-(*loc)[i].loc.y)<0.5*size)){
                    
                    draw=1;
                    
                    double sx,sy,sz;
                    
                    double lx=70*pixelx;
                    double ly=40*pixely;
                    
                    if ((*loc)[i].loc.x<0.5*(minx+maxx)){
                        sx=(*loc)[i].loc.x+30*pixelx;
                    }
                    else{
                        sx=(*loc)[i].loc.x-30*pixelx-lx;
                    }
                    
                    if ((*loc)[i].loc.y<0.5*(miny+maxy)){
                        sy=(*loc)[i].loc.y+30*pixely;
                    }
                    else{
                        sy=(*loc)[i].loc.y-30*pixely-ly;
                    }
                    
                    //sz=(*loc)[i].loc.z+0.4;
                    sz=100;
                    
                    if (root->bow){
                        glColor3ub(255,255,255);
                    }
                    else{
                        glColor3ub(0,0,0);
                    }
                    
                    glBegin(GL_TRIANGLES);
                    
                    glVertex3f(sx,sy,sz);
                    glVertex3f(sx+lx,sy,sz);
                    glVertex3f(sx,sy+ly,sz);

                    glVertex3f(sx+lx,sy,sz);
                    glVertex3f(sx,sy+ly,sz);
                    glVertex3f(sx+lx,sy+ly,sz);
                    glEnd();
                    
                    if (root->bow){
                        glColor3ub(0,0,0);
                    }
                    else{
                        glColor3ub(255,255,255);
                    }
                    
                    glBegin(GL_LINES);
                    
                    glVertex3f((*loc)[i].loc.x,(*loc)[i].loc.y,sz);
                    glVertex3f(sx,sy,sz);
                    
                    glVertex3f((*loc)[i].loc.x,(*loc)[i].loc.y,sz);
                    glVertex3f(sx+lx,sy,sz);
                    
                    glVertex3f((*loc)[i].loc.x,(*loc)[i].loc.y,sz);
                    glVertex3f(sx,sy+ly,sz);
                    
                    glVertex3f((*loc)[i].loc.x,(*loc)[i].loc.y,sz);
                    glVertex3f(sx+lx,sy+ly,sz);
                    
                    glVertex3f(sx,sy,sz+0.1);
                    glVertex3f(sx+lx,sy,sz+0.1);

                    glVertex3f(sx,sy,sz+0.1);
                    glVertex3f(sx,sy+ly,sz+0.1);
                    
                    glVertex3f(sx+lx,sy,sz+0.1);
                    glVertex3f(sx+lx,sy+ly,sz+0.1);
                    
                    glVertex3f(sx,sy+ly,sz+0.1);
                    glVertex3f(sx+lx,sy+ly,sz+0.1);
                    
                    glEnd();
                    
                   
                    
                    glLineWidth(1.0);
                    
                   
                    if (root->bow){
                        glColor3ub(0,0,0);
                    }
                    else{
                        glColor3ub(255,255,255);
                    }
                    
                    std::stringstream s;
                    
                    
                    s<<"metric:";
                    
                    std::string t = s.str();
                    
                    glPushMatrix();
                    
                    glTranslatef(sx+8*pixelx,sy+23*pixely,sz+0.1);
                    glScalef(0.1*pixelx, 0.1*pixely, 0.1*pixelx);
                    
                    
                    for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) //(p ; *p; p++)
                    {
                        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
                    }
                    glPopMatrix();
                    
                    s.str("");
                    s<<std::setprecision(2)<<std::fixed;
                    
                    if (((*metric)[i]>1000)||((*metric)[i]<0.1)){
                        s<<std::scientific;
                    }
                    
                    if ((*metric)[i]>=0){
                        s<<(*metric)[i];
                    }
                    else{
                        s<<"N/A";
                    }
                    t = s.str();
                    
                    glPushMatrix();
                    
                    glTranslatef(sx+8*pixelx,sy+8*pixely,sz+0.1);
                    glScalef(0.1*pixelx, 0.1*pixely, 0.1*pixelx);
                    
                    
                    for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) //(p ; *p; p++)
                    {
                        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
                    }
                    glPopMatrix();
                    
                    
                    
                }
                
                
            }
            
            
            
            
        }
    }
    
}


void MyWindow::draw_desks_model(int selection){
    
    for (int i=0;i<current->desks.size();i++){
               
        
        if ((all_flrs)||(current->desks[i].loc.floor==flr_choice)){
            
            
            glPushMatrix();
            glTranslatef(current->desks[i].loc.x,current->desks[i].loc.y,0.35+current->floor_index[current->desks[i].loc.floor]);
            
            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT0);
            glEnable(GL_LIGHT1);
            
            int seg=-1;
            
            
            for (int ii=0;ii<current->adj_list.size();ii++){
                if (current->adj_list[ii].start==current->desks[i].point){
                    seg = current->adj_list[ii].end;
                    break;
                }
                if (current->adj_list[ii].end==current->desks[i].point){
                    seg = current->adj_list[ii].start;
                    break;
                }
            }                        
            
            
            
            double dx,dy;
            
            
            dx=current->points[current->desks[i].point].x-current->points[seg].x;
            dy=current->points[current->desks[i].point].y-current->points[seg].y;
            
            if (dx<0){
                dx=-dx;
                dy=-dy;
            }
            
            double angle=180;//angle2;
            angle += atan2(dy,dx)*180.0/M_PI;
                                        
            double Ax=current->points[seg].x;
            double Ay=current->points[seg].y;
            double X =current->desks[i].loc.x;
            double Y =current->desks[i].loc.y;
            
            int position = ( dx*(Y-Ay) - dy*(X-Ax) );
            
            if (position>0)angle+=180;

            glRotatef(angle,0,0,1);
            glRotatef(90,1,0,0);
            
            
            glColor3ub(100,100,100);
            
            
            double inc_r=0,inc_g=0,inc_b=0;
            if ((selection)&&(i==selected)){
                inc_r=0.2;
                inc_g=0.2;
                inc_b=0.0;
            }
            
            
            
            GLfloat light_diffuse[] = { 0.35+inc_r, 0.35+inc_g, 0.35+inc_b, 1.0 };
            glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
            
            glBegin(GL_TRIANGLES);
            for (int ii=0;ii<(int)(760*3);ii++){
                
                glNormal3f(untitledNormals[ii*3],untitledNormals[ii*3+1],untitledNormals[ii*3+2]);
                glVertex3f(untitledVerts[ii*3],untitledVerts[ii*3+1],untitledVerts[ii*3+2]);
                
            }
            glEnd();
            
            GLfloat light_diffuse1[] = { 0.68+inc_r, 0.46+inc_g, 0.31+inc_b, 1.0 };
            glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse1);
            
            glBegin(GL_TRIANGLES);
            for (int ii=761*3;ii<(int)(untitledNumVerts);ii++){
                
                glNormal3f(untitledNormals[ii*3],untitledNormals[ii*3+1],untitledNormals[ii*3+2]);
                glVertex3f(untitledVerts[ii*3],untitledVerts[ii*3+1],untitledVerts[ii*3+2]);
                
            }
            glEnd();
            
            glDisable(GL_LIGHTING);
            glDisable(GL_LIGHT0);
            glDisable(GL_LIGHT1);
            
            glPopMatrix();
            
            
            
            
        }
        
    }
    
}

void MyWindow::draw_location_links(std::vector<location>* loc,int c1,int c2, int c3){
    
    glColor3ub(c1,c2,c3);
    for (int i=0;i<loc->size();i++){
        for (int j=0;j<loc->size();j++){
            
            if ((*loc)[i].point==(*loc)[j].point){
                
                if ((all_flrs)||((*loc)[i].loc.floor==flr_choice)){
                    
                    
                    glLineWidth(1.0);
                    glBegin(GL_LINES);
                    glVertex3f((*loc)[i].loc.x,(*loc)[i].loc.y,current->floor_index[(*loc)[i].loc.floor]);
                    glVertex3f((*loc)[j].loc.x,(*loc)[j].loc.y,current->floor_index[(*loc)[j].loc.floor]);
                    glEnd();

                    
                }
                
                
            }
            
            
        }
    }
}

void MyWindow::draw_locations(std::vector<location>* loc,int c1,int c2, int c3, int selection){
    
    for (int i=0;i<(*loc).size();i++){
        
        
        if ((all_flrs)||((*loc)[i].loc.floor==flr_choice)){
            
            glColor3ub(c1,c2,c3);
            
            if ((selection)&&(i==selected)){ // selection highlighing
                glColor3ub(255,255,0);
            }
            if (show_map->value()){
                glLineWidth(1.0);
                glBegin(GL_LINES);
                glVertex3f(current->points[(*loc)[i].point].x,current->points[(*loc)[i].point].y,current->floor_index[current->points[(*loc)[i].point].floor]);
                glVertex3f((*loc)[i].loc.x,(*loc)[i].loc.y,current->floor_index[(*loc)[i].loc.floor]);
                glEnd();
            }
            

            drawcube((*loc)[i].loc.x,(*loc)[i].loc.y,current->floor_index[(*loc)[i].loc.floor],0.4);
            drawcubeedge((*loc)[i].loc.x,(*loc)[i].loc.y,current->floor_index[(*loc)[i].loc.floor],0.4,1.51,0);
            
            
            
            
        }
    }
    
}

void MyWindow::draw_desks(){
    
    if (show_desks_choice->value()){
        
        
        for (int i=0;i<current->desks.size();i++){
            
            
            if ((all_flrs)||(current->desks[i].loc.floor==flr_choice)){
                glColor3ub(100,100,100);
                glLineWidth(1.0);
                glBegin(GL_LINES);
                glVertex3f(current->points[current->desks[i].point].x,current->points[current->desks[i].point].y,current->points[current->desks[i].point].z);
                glVertex3f(current->desks[i].loc.x,current->desks[i].loc.y,current->desks[i].loc.z);
                glEnd();
                drawcube(current->desks[i].loc.x,current->desks[i].loc.y,current->desks[i].loc.z,0.4);
                drawcubeedge(current->desks[i].loc.x,current->desks[i].loc.y,current->desks[i].loc.z,0.4,1.51,0);
            }
        }
    }
    
}

void MyWindow::drawpyramid(double x, double y, double z, double length){
    
    double height=sqrt(0.6666666666)*length;
    glBegin(GL_TRIANGLES);
    
    glVertex3f(x-0.5*length,y-0.3333*length,z-0.5*height);
    glVertex3f(x+0.5*length,y-0.3333*length,z-0.5*height);
    glVertex3f(x,y,z+0.5*height);
    
    glVertex3f(x-0.5*length,y-0.3333*length,z-0.5*height);
    glVertex3f(x,y+0.66666*length,z-0.5*height);
    glVertex3f(x,y,z+0.5*height);
    
    glVertex3f(x+0.5*length,y-0.3333*length,z-0.5*height);
    glVertex3f(x,y+0.666666*length,z-0.5*height);
    glVertex3f(x,y,z+0.5*height);
    
    glVertex3f(x-0.5*length,y-0.3333*length,z-0.5*height);
    glVertex3f(x+0.5*length,y-0.3333*length,z-0.5*height);
    glVertex3f(x,y+0.666666*length,z-0.5*height);
    glEnd();
}

void MyWindow::drawcube(double x, double y, double z, double length){
    glBegin(GL_TRIANGLES);
    glVertex3f(x-0.5*length,y-0.5*length,z-0.5*length);
    glVertex3f(x+0.5*length,y-0.5*length,z-0.5*length);
    glVertex3f(x-0.5*length,y+0.5*length,z-0.5*length);
    glVertex3f(x-0.5*length,y+0.5*length,z-0.5*length);
    glVertex3f(x+0.5*length,y-0.5*length,z-0.5*length);
    glVertex3f(x+0.5*length,y+0.5*length,z-0.5*length);
    
    glVertex3f(x-0.5*length,y-0.5*length,z+0.5*length);
    glVertex3f(x+0.5*length,y-0.5*length,z+0.5*length);
    glVertex3f(x-0.5*length,y+0.5*length,z+0.5*length);
    glVertex3f(x-0.5*length,y+0.5*length,z+0.5*length);
    glVertex3f(x+0.5*length,y-0.5*length,z+0.5*length);
    glVertex3f(x+0.5*length,y+0.5*length,z+0.5*length);
    
    
    glVertex3f(x-0.5*length,y-0.5*length,z-0.5*length);
    glVertex3f(x-0.5*length,y-0.5*length,z+0.5*length);
    glVertex3f(x-0.5*length,y+0.5*length,z-0.5*length);
    glVertex3f(x-0.5*length,y+0.5*length,z-0.5*length);
    glVertex3f(x-0.5*length,y-0.5*length,z+0.5*length);
    glVertex3f(x-0.5*length,y+0.5*length,z+0.5*length);
    
    glVertex3f(x+0.5*length,y-0.5*length,z-0.5*length);
    glVertex3f(x+0.5*length,y-0.5*length,z+0.5*length);
    glVertex3f(x+0.5*length,y+0.5*length,z-0.5*length);
    glVertex3f(x+0.5*length,y+0.5*length,z-0.5*length);
    glVertex3f(x+0.5*length,y-0.5*length,z+0.5*length);
    glVertex3f(x+0.5*length,y+0.5*length,z+0.5*length);
    
    
    glVertex3f(x-0.5*length,y-0.5*length,z-0.5*length);
    glVertex3f(x-0.5*length,y-0.5*length,z+0.5*length);
    glVertex3f(x+0.5*length,y-0.5*length,z-0.5*length);
    glVertex3f(x+0.5*length,y-0.5*length,z-0.5*length);
    glVertex3f(x-0.5*length,y-0.5*length,z+0.5*length);
    glVertex3f(x+0.5*length,y-0.5*length,z+0.5*length);
    
    glVertex3f(x-0.5*length,y+0.5*length,z-0.5*length);
    glVertex3f(x-0.5*length,y+0.5*length,z+0.5*length);
    glVertex3f(x+0.5*length,y+0.5*length,z-0.5*length);
    glVertex3f(x+0.5*length,y+0.5*length,z-0.5*length);
    glVertex3f(x-0.5*length,y+0.5*length,z+0.5*length);
    glVertex3f(x+0.5*length,y+0.5*length,z+0.5*length);
    glEnd();
}


void MyWindow::drawcube(double x, double y, double z, double length, GLuint texture){
    
    glBindTexture(GL_TEXTURE_2D,texture);
    
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0,0);
    glVertex3f(x-0.5*length,y-0.5*length,z-0.5*length);
    glTexCoord2f(0,1);
    glVertex3f(x+0.5*length,y-0.5*length,z-0.5*length);
    glTexCoord2f(1,0);
    glVertex3f(x-0.5*length,y+0.5*length,z-0.5*length);
    glTexCoord2f(1,0);
    glVertex3f(x-0.5*length,y+0.5*length,z-0.5*length);
    glTexCoord2f(1,0);
    glVertex3f(x+0.5*length,y-0.5*length,z-0.5*length);
    glTexCoord2f(1,1);
    glVertex3f(x+0.5*length,y+0.5*length,z-0.5*length);
    
    glTexCoord2f(0,0);
    glVertex3f(x-0.5*length,y-0.5*length,z+0.5*length);
    glTexCoord2f(1,0);
    glVertex3f(x+0.5*length,y-0.5*length,z+0.5*length);
    glTexCoord2f(0,1);
    glVertex3f(x-0.5*length,y+0.5*length,z+0.5*length);
    glTexCoord2f(0,1);
    glVertex3f(x-0.5*length,y+0.5*length,z+0.5*length);
    glTexCoord2f(1,0);
    glVertex3f(x+0.5*length,y-0.5*length,z+0.5*length);
    glTexCoord2f(1,1);
    glVertex3f(x+0.5*length,y+0.5*length,z+0.5*length);
    
    glTexCoord2f(0,0);
    glVertex3f(x-0.5*length,y-0.5*length,z-0.5*length);
    glTexCoord2f(0,1);
    glVertex3f(x-0.5*length,y-0.5*length,z+0.5*length);
    glTexCoord2f(1,0);
    glVertex3f(x-0.5*length,y+0.5*length,z-0.5*length);
    glTexCoord2f(1,0);
    glVertex3f(x-0.5*length,y+0.5*length,z-0.5*length);
    glTexCoord2f(0,1);
    glVertex3f(x-0.5*length,y-0.5*length,z+0.5*length);
    glTexCoord2f(1,1);
    glVertex3f(x-0.5*length,y+0.5*length,z+0.5*length);
    
    glTexCoord2f(0,0);
    glVertex3f(x+0.5*length,y-0.5*length,z-0.5*length);
    glTexCoord2f(0,1);
    glVertex3f(x+0.5*length,y-0.5*length,z+0.5*length);
    glTexCoord2f(1,0);
    glVertex3f(x+0.5*length,y+0.5*length,z-0.5*length);
    glTexCoord2f(1,0);
    glVertex3f(x+0.5*length,y+0.5*length,z-0.5*length);
    glTexCoord2f(0,1);
    glVertex3f(x+0.5*length,y-0.5*length,z+0.5*length);
    glTexCoord2f(1,1);
    glVertex3f(x+0.5*length,y+0.5*length,z+0.5*length);
    
    
    glTexCoord2f(0,0);
    glVertex3f(x-0.5*length,y-0.5*length,z-0.5*length);
    glTexCoord2f(0,1);
    glVertex3f(x-0.5*length,y-0.5*length,z+0.5*length);
    glTexCoord2f(1,0);
    glVertex3f(x+0.5*length,y-0.5*length,z-0.5*length);
    glTexCoord2f(1,0);
    glVertex3f(x+0.5*length,y-0.5*length,z-0.5*length);
    glTexCoord2f(0,1);
    glVertex3f(x-0.5*length,y-0.5*length,z+0.5*length);
    glTexCoord2f(1,1);
    glVertex3f(x+0.5*length,y-0.5*length,z+0.5*length);
    
    
    glTexCoord2f(0,0);
    glVertex3f(x-0.5*length,y+0.5*length,z-0.5*length);
    glTexCoord2f(0,1);
    glVertex3f(x-0.5*length,y+0.5*length,z+0.5*length);
    glTexCoord2f(1,0);
    glVertex3f(x+0.5*length,y+0.5*length,z-0.5*length);
    glTexCoord2f(1,0);
    glVertex3f(x+0.5*length,y+0.5*length,z-0.5*length);
    glTexCoord2f(0,1);
    glVertex3f(x-0.5*length,y+0.5*length,z+0.5*length);
    glTexCoord2f(1,1);
    glVertex3f(x+0.5*length,y+0.5*length,z+0.5*length);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}


void MyWindow::drawcubeedge(double x,double y,double z,double length,double lwidth, int a){
    glLineWidth(lwidth);
    if (root->bow){
        glColor3ub(0,0,0);
    }
    else{
        glColor3ub(255,255,255);
    }
    glBegin(GL_LINES);
    
    if (!a){
        glVertex3f(x-0.5*length,y-0.5*length,z-0.5*length);
        glVertex3f(x+0.5*length,y-0.5*length,z-0.5*length);
    }
    glVertex3f(x-0.5*length,y-0.5*length,z+0.5*length);
    glVertex3f(x+0.5*length,y-0.5*length,z+0.5*length);
    glVertex3f(x-0.5*length,y+0.5*length,z-0.5*length);
    glVertex3f(x+0.5*length,y+0.5*length,z-0.5*length);
    glVertex3f(x-0.5*length,y+0.5*length,z+0.5*length);
    glVertex3f(x+0.5*length,y+0.5*length,z+0.5*length);
    if (!a){
        glVertex3f(x-0.5*length,y-0.5*length,z-0.5*length);
        glVertex3f(x-0.5*length,y+0.5*length,z-0.5*length);
    }
    glVertex3f(x-0.5*length,y-0.5*length,z+0.5*length);
    glVertex3f(x-0.5*length,y+0.5*length,z+0.5*length);
    glVertex3f(x+0.5*length,y-0.5*length,z-0.5*length);
    glVertex3f(x+0.5*length,y+0.5*length,z-0.5*length);
    glVertex3f(x+0.5*length,y-0.5*length,z+0.5*length);
    glVertex3f(x+0.5*length,y+0.5*length,z+0.5*length);
    if (!a){
        glVertex3f(x-0.5*length,y-0.5*length,z-0.5*length);
        glVertex3f(x-0.5*length,y-0.5*length,z+0.5*length);
    }
    glVertex3f(x-0.5*length,y+0.5*length,z-0.5*length);
    glVertex3f(x-0.5*length,y+0.5*length,z+0.5*length);
    glVertex3f(x+0.5*length,y-0.5*length,z-0.5*length);
    glVertex3f(x+0.5*length,y-0.5*length,z+0.5*length);
    glVertex3f(x+0.5*length,y+0.5*length,z-0.5*length);
    glVertex3f(x+0.5*length,y+0.5*length,z+0.5*length);
    
    glEnd();
}



void MyWindow::translate_frustum(){
    

    if ((three_d_choice->value()==2)){ //EYE SPACE TRANSFORM
        glTranslatef(-0.5*(minx+maxx),-0.5*(miny+maxy),-z_dist);  //in conjuction with frustum
        
    }
    else if(three_d_choice->value()==3){
        
        

            glTranslatef(-xfp,-yfp,-zfp);
            
            
            
            xfr=xfp;
            yfr=yfp;
            zfr=zfp;
            
            axx=ang_xfp;
            azz=ang_zfp;
            
        
    }
    //END FRUSTRUM PLACEMENT
}


void MyWindow::setzoom(){
    
    
    double ppsx,ppsy;
    
    if (!first_draw){ //first draw will set up initial values
        
        if (zoom){ //mouse has been scrolled
            
            
            if (zoom_mag>5)zoom_mag=5;  //limit zooming from the mouse
            if (zoom_mag<-5)zoom_mag=-5;
            
         
            double xmin,xmax,ymin,ymax;
            
            double dx=0,dy=0;
            
           
            xmin=minx-0.5*0.05*((double)(zoom_mag))*(maxx-minx); //place holder versions of ortho coordinates
            xmax=maxx+0.5*0.05*((double)(zoom_mag))*(maxx-minx); // has to take into account changes relative to window size/shape too
            ymin=miny-0.5*0.05*((double)(zoom_mag))*(maxy-miny);
            ymax=maxy+0.5*0.05*((double)(zoom_mag))*(maxy-miny);
            
           
            if(three_d_choice->value()<2){ //dont try to correct if using perspective
                dx=frac_x*((maxx-xmax)-(minx-xmin))-(xmin-minx); //correction to keep point in same place as mouse (ish)
                dy=frac_y*((maxy-ymax)-(miny-ymin))-(ymin-miny);
            }
            
            
            xmin+=dx;  //apply correction to placeholder
            xmax+=dx;
            ymin+=dy;
            ymax+=dy;
            
            
            
            if (new_zoom){
                
                
                
                xmin=0.5*(maxx_overall+minx_overall) - (1/new_zoom)*(0.5*(maxx_overall+minx_overall)-minx_overall);
                xmax=0.5*(maxx_overall+minx_overall) + (1/new_zoom)*(maxx_overall-0.5*(maxx_overall+minx_overall));
                new_zoom=0;
            
            }
            
            
            
            if (((xmax-xmin)>0.05)&&((xmax-xmin)<(5*(maxx_overall-minx_overall)))){
                
                minx=xmin; //set actual values to the placeholders
                maxx=xmax;
                miny=ymin;
                maxy=ymax;

            }
            
            
            if(three_d_choice->value()<2){
                
                if((xmax-xmin)>(5*(maxx_overall-minx_overall))){
                    
                    
                    double xmid=0.5*(maxx_overall+minx_overall);
                    double dx=0.5*((maxx-xmid)-(xmid-minx));
                    minx-=0.5*dx;
                    maxx-=0.5*dx;
                    
                    double ymid=0.5*(maxy_overall+miny_overall);
                    double dy=0.5*((maxy-ymid)-(ymid-miny));
                    miny-=0.5*dy;
                    maxy-=0.5*dy;
                    
                }
            }
            
           
            
            zoom=0;drag=0; //finished zooming
        }
        else if (drag){
            
            
            minx=minx_down-(xdragdown-xdown)*(maxx_down-minx_down)/(this->w());
            maxx=maxx_down-(xdragdown-xdown)*(maxx_down-minx_down)/(this->w());
            miny=miny_down+(ydragdown-ydown)*(maxy_down-miny_down)/(this->h());
            maxy=maxy_down+(ydragdown-ydown)*(maxy_down-miny_down)/(this->h());
            drag=0;zoom=0; //finished dragging
        }
        
        
        
    }
    else{ //first draw
        
        
        maxx = 100;
        minx = -100;
        maxx_overall = 100;
        minx_overall = -100;
        
        maxy = 100;
        miny = -100;
        maxy_overall = 100;
        miny_overall = -100;
        
        
        minz = 0;
        
        double maxx_t=-9e99,minx_t=9e99,maxy_t=-9e99,miny_t=9e99;
        double minz_t=9e99,maxz_t=-9e99; //find extremes of current->points
        
        for (int i=0;i<current->points.size();i++){
            if (current->points[i].x<minx_t){
                minx_t=current->points[i].x;
            }
            if (current->points[i].y<miny_t){
                miny_t=current->points[i].y;
            }
            if (current->points[i].x>maxx_t){
                maxx_t=current->points[i].x;
            }
            if (current->points[i].y>maxy_t){
                maxy_t=current->points[i].y;
            }
            if (current->points[i].z<minz_t){
                minz_t=current->points[i].z;
            }
            if (current->points[i].z>maxz_t){
                maxz_t=current->points[i].z;
            }
        }
        
        for (int j=0;j<current->floor_plans.size();j++){
            
            plan floor_plan;
            floor_plan=current->floor_plans[j];
            std::vector<int> layers = floor_plan.layers;
            
            for (int i=0;i<floor_plan.lines.size();i++){
                
                if (layers[floor_plan.lines[i].layer]){
                    
                    if (floor_plan.lines[i].start.x+floor_plan.shift_x<minx_t){
                        minx_t=floor_plan.lines[i].start.x+floor_plan.shift_x;
                    }
                    if (floor_plan.lines[i].start.x+floor_plan.shift_x>maxx_t){
                        maxx_t=floor_plan.lines[i].start.x+floor_plan.shift_x;
                    }
                    if (floor_plan.lines[i].start.y+floor_plan.shift_y<miny_t){
                        miny_t=floor_plan.lines[i].start.y+floor_plan.shift_y;
                    }
                    if (floor_plan.lines[i].start.y+floor_plan.shift_y>maxy_t){
                        maxy_t=floor_plan.lines[i].start.y+floor_plan.shift_y;
                    }
                    if (floor_plan.lines[i].start.z<minz_t){
                        minz_t=floor_plan.lines[i].start.z;
                    }
                    if (floor_plan.lines[i].start.z>maxz_t){
                        maxz_t=floor_plan.lines[i].start.z;
                    }
                    
                    if (floor_plan.lines[i].end.x+floor_plan.shift_x<minx_t){
                        minx_t=floor_plan.lines[i].start.x+floor_plan.shift_x;
                    }
                    if (floor_plan.lines[i].end.x+floor_plan.shift_x>maxx_t){
                        maxx_t=floor_plan.lines[i].start.x+floor_plan.shift_x;
                    }
                    if (floor_plan.lines[i].end.y+floor_plan.shift_y<miny_t){
                        miny_t=floor_plan.lines[i].start.y+floor_plan.shift_y;
                    }
                    if (floor_plan.lines[i].end.y+floor_plan.shift_y>maxy_t){
                        maxy_t=floor_plan.lines[i].start.y+floor_plan.shift_y;
                    }
                    if (floor_plan.lines[i].end.z<minz_t){
                        minz_t=floor_plan.lines[i].start.z;
                    }
                    if (floor_plan.lines[i].end.z>maxz_t){
                        maxz_t=floor_plan.lines[i].start.z;
                    }
                    
                }
                
            }
        }
        
        
        minx_t-=5; //pad the current->points
        miny_t-=5;
        maxx_t+=5;
        maxy_t+=5;
        minz_t-=5;
        maxz_t+=5;
        
        
        
        
        ppsx=(maxx_t-minx_t)/w(); //find metres per pixel
        ppsy=(maxy_t-miny_t)/h();
        
        
        double dif_x=0,dif_y=0; //do the maths...
        if (ppsx>ppsy){
            dif_y=0.5*(ppsx*h()-(maxy_t-miny_t));
        }
        if (ppsy>ppsx){
            dif_x=0.5*(ppsy*w()-(maxx_t-minx_t));
            
        }
        
        
        minx_t=minx_t-dif_x; //apply corrections
        maxx_t=maxx_t+dif_x;
        miny_t=miny_t-dif_y;
        maxy_t=maxy_t+dif_y;

        z_dist=1.5*(maxx_overall-minx_overall);
        
        
        if (minx_t<9e98){ //so we default if we havent read anything
            
            minx_overall=minx_t; //remember the limits of zoom one level
            maxx_overall=maxx_t;
            miny_overall=miny_t;
            maxy_overall=maxy_t;
            
            minx=minx_t;
            maxx=maxx_t;
            miny=miny_t;
            maxy=maxy_t;
            
            current->mid_x=0.5*(minx_overall+maxx_overall);
            current->mid_y=0.5*(miny_overall+maxy_overall);
            current->mid_z=0.5*(minz_t+maxz_t);
                       
        }
        
        z_dist=1.5*(maxx_overall-minx_overall);
        
        w_initial=w();
        h_initial=h();
        
        
        ang_x=0;
        ang_y=0;
        ang_z=0;
        zoom_level=1.0; //zoom level defined to be 1.
        
        
    }
    
    
    
    //back to main loop: deal with any resizing by user manipulation of window
    
    
    ppsx=(maxx-minx)/w();
    ppsy=(maxy-miny)/h();
    
    
    double dif_x=0,dif_y=0;
    
    if (ppsy>ppsx){
        dif_y=0.5*(ppsx*h()-(maxy-miny)); //this time only ever change y: otherwise keeps getting smaller with each resize!
    }
    if (ppsy<ppsx){
        dif_y=0.5*(ppsx*h()-(maxy-miny));
    }
    
    
    
    minx=minx-dif_x;
    maxx=maxx+dif_x;
    miny=miny-dif_y;
    maxy=maxy+dif_y;
    
    
}


void MyWindow::set_rotation(){
    
    
    if (three_d_choice->value()==3){
       
        
        
        
        glTranslatef(xfr,yfr,zfr);
        glRotatef(axx,1,0,0);
        
        glRotatef(azz,0,0,1);
        glTranslatef(-xfr,-yfr,-zfr);
        
        
    }
    
    else if (three_d_choice->value()){
        
        glTranslatef(current->mid_x,current->mid_y,current->mid_z);
        glTranslatef(0,0,-current->mid_z-z_gap);
        glRotatef(ang_x,1,0,0);
        glRotatef(ang_z,0,0,1);
        glTranslatef(0,0,+current->mid_z+z_gap);
        glTranslatef(-current->mid_x,-current->mid_y,-current->mid_z);
    }
}

void MyWindow::draw_background(double back_z){
    
    //START background grid: relative to orthographic coords so always there
    if (!three_d_choice->value()){ //only for 2D drawing
        glLineWidth(1.0);
        glBegin(GL_LINES);
        
        if (root->bow){
            glColor3ub(180,180,180);
        }
        else{
            glColor3ub(76,76,76);
        }
        
        for (int i=0;i<4+(int)(maxx/10) - (int)(minx/10);i++){
            int ii=-1+i+(int)(minx/10);
            glVertex3f(ii*10,miny-10,back_z);
            glVertex3f(ii*10,maxy+10,back_z);
        }
        for (int j=0;j<4+(int)(maxy/10) - (int)(miny/10);j++){
            int jj=-1+j+(int)(miny/10);
            glVertex3f(minx-10,(jj-1)*10,back_z);
            glVertex3f(maxx+10,(jj-1)*10,back_z);
            
        }
        glEnd();
        
        glLineWidth(1.0);
        glBegin(GL_LINES);
        
        if (root->bow){
            glColor3ub(215,215,215);
        }
        else{
            glColor3ub(41,41,41);
        }
        
        for (int i=0;i<4+(int)(maxx/2) - (int)(minx/2);i++){
            int ii=-1+i+(int)(minx/2);
            glVertex3f(ii*2,miny-10,back_z);
            glVertex3f(ii*2,maxy+10,back_z);
        }
        for (int j=0;j<4+(int)(maxy/2) - (int)(miny/2);j++){
            int jj=-1+j+(int)(miny/2);
            glVertex3f(minx-10,(jj-1)*2,back_z);
            glVertex3f(maxx+10,(jj-1)*2,back_z);
            
        }
        glEnd();
    }
    else{
        
        
        
        //BEGIN RELATIVE BACKGROUND GRID
        glLineWidth(1.0);
        glBegin(GL_LINES);
        
        if (root->bow){
            glColor3ub(180,180,180);
        }
        else{
            glColor3ub(76,76,76);
        }
        
        
        double start_x = 10.0*((int)((minx_overall-50)/10));
        double start_y = 10.0*((int)((miny_overall-50)/10));
        double end_x = 10.0*((int)((maxx_overall+50)/10));
        double end_y = 10.0*((int)((maxy_overall+50)/10));

        
        
        
        if((end_x-start_x)<(end_y-start_y)){ //make square
            
            
            
        }
        else{
            
            double end_y_temp=end_y+10*((int)(0.5*((end_x-start_x)-(end_y-start_y)))/10);
            double start_y_temp=start_y-10*((int)(0.5*((end_x-start_x)-(end_y-start_y)))/10);
            end_y=end_y_temp;
            start_y=start_y_temp;
            
        }
        
        
        
        for (double xx=start_x;xx<=end_x;xx+=10){
            
            glVertex3f(xx,start_y,minz-z_gap);
            glVertex3f(xx,end_y,minz-z_gap);
        }
        for (double yy=start_y;yy<=end_y;yy+=10){
            
            glVertex3f(start_x,yy,minz-z_gap);
            glVertex3f(end_x,yy,minz-z_gap);
            
        }                       
        
        glEnd();
        
        glLineWidth(1.0);
        glBegin(GL_LINES);
        
        
        if (root->bow){
            glColor3ub(215,215,215);
        }
        else{
            glColor3ub(41,41,41);
        }
        
        
        
        for (double xx=start_x;xx<=end_x;xx+=2){
            glVertex3f(xx,start_y,minz-z_gap);
            glVertex3f(xx,end_y,minz-z_gap);
        }
        for (double yy=start_y;yy<=end_y;yy+=2){
            
            glVertex3f(start_x,yy,minz-z_gap);
            glVertex3f(end_x,yy,minz-z_gap);
            
        }
     
        
        glEnd();
        //END RELATIVE BACKGROUND GRID
    }
    
    //END background grid
    
}

void MyWindow::showpoints(){

    if (root->draw_points_label){//show current->points
        if (three_d_choice->value()<2){
            
            glPushMatrix();
            //glDisable(GL_DEPTH_TEST);
            
            
            double pixel=(maxx-minx)/(double)(this->w());
            glLineWidth(1.0);
            
            for (int i=0;i<current->points.size();i++){
                
                
                if ((all_flrs)||(current->points[i].floor==flr_choice)){
                    
                   
                    gl_font(1, 14);
                    if (root->bow){
                        glColor3ub(0,0,0);
                    }
                    else{
                        glColor3ub(255,255,255);
                    }
                    glRasterPos2f(current->points[i].x+0.01,current->points[i].y+0.01);
                    std::stringstream s;
                    s<<i;
                    std::string t = s.str();
                   
                    glPushMatrix();
                    
                    double dd = 0.5*point_size->value();
                    
                    glTranslatef(current->points[i].x+dd,current->points[i].y+dd, current->points[i].z+dd);
                    glScalef(0.1*pixel, 0.1*pixel, 0.1*pixel);
                    
                    if ((three_d_choice->value()>0)){
                        glRotatef(-ang_z,0,0,1);
                        glRotatef(-ang_x,1,0,0);
                    }
                    
                    for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) //(p ; *p; p++)
                    {
                        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
                    }
                    glPopMatrix();
                    
               
                    
                    
                }
            }
            
            glPopMatrix();
            
            
        }
    }
}

void MyWindow::showsegments(){
   
   if (root->draw_segments_label){
       
       if (three_d_choice->value()<2){
   
        gl_font(1, 14);
       if (root->bow){
           glColor3ub(0,0,0);
       }
       else{
           glColor3ub(255,255,255);
       }
       
        double pixel=(maxx-minx)/(double)(this->w());
        glLineWidth(1.0);
        
        
        for (int i=0;i<current->adj_list.size();i++){
            
            int i1=current->adj_list[i].start;
            int i2=current->adj_list[i].end;
            
            if ((i1<current->adj_list.size())&&(i2<current->adj_list.size())){
            
            if ((all_flrs)||((current->points[i1].floor==flr_choice)&&(current->points[i2].floor==flr_choice))){
                
                const char *p;
                
                std::stringstream s;
                s<<i;
                std::string t=s.str();
                
                glPushMatrix();
                
                glTranslatef(0.5*(current->points[i1].x+current->points[i2].x)+0.1,0.5*(current->points[i1].y+current->points[i2].y)+0.1, 0.5*(current->points[i1].z+current->points[i2].z)+0.1);
                glScalef(0.1*pixel, 0.1*pixel, 0.1*pixel);
                
                
                if (three_d_choice->value()>0){
                    glRotatef(-ang_z,0,0,1);
                    glRotatef(-ang_x,1,0,0);
                }
                
                for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) //(p ; *p; p++)
                {
                    glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
                }
                glPopMatrix();
                
            }
            }
            
        }
           
      
    }//end show segments
   }
}

void MyWindow::showscale(){
              
    if ((!three_d_choice->value())&&(root->show_scale)){
        
        
        double scale_a=0.0001;
        double next_scale=0.0005;
        int i=1;
        while (fabs(scale_a-0.1*(maxx-minx))>fabs(next_scale-0.1*(maxx-minx))){
            if (i>0){
                scale_a*=5;
                next_scale*=2;
            }
            else{
                scale_a*=2;
                next_scale*=5;
            }
            i*=-1;
        }
        
        double xwrite,ywrite,pixel;
        
        pixel=(maxx-minx)/(double)(this->w()); //should be the same for y!!!
        
        
        xwrite =minx+ 20*pixel;
        ywrite = miny+ 20*pixel;
        glPushMatrix();
        if (root->bow){
            glColor3ub(0,0,0);
        }
        else{
            glColor3ub(255,255,255);
        }
        glLineWidth(1.0);
        glBegin(GL_LINES);
        glVertex3f(xwrite,ywrite,50);
        glVertex3f(xwrite+scale_a,ywrite,50);
        
        glVertex3f(xwrite,ywrite-5*pixel,50);
        glVertex3f(xwrite,ywrite+5*pixel,50);
        
        
        glVertex3f(xwrite+0.2*scale_a,ywrite-3*pixel,50);
        glVertex3f(xwrite+0.2*scale_a,ywrite+3*pixel,50);
        
        glVertex3f(xwrite+0.4*scale_a,ywrite-3*pixel,50);
        glVertex3f(xwrite+0.4*scale_a,ywrite+3*pixel,50);
        
        glVertex3f(xwrite+0.6*scale_a,ywrite-3*pixel,50);
        glVertex3f(xwrite+0.6*scale_a,ywrite+3*pixel,50);
        
        glVertex3f(xwrite+0.8*scale_a,ywrite-3*pixel,50);
        glVertex3f(xwrite+0.8*scale_a,ywrite+3*pixel,50);
        
        glVertex3f(xwrite+scale_a,ywrite-5*pixel,50);
        glVertex3f(xwrite+scale_a,ywrite+5*pixel,50);
        
        
        //yaxis
        
        
        glVertex3f(xwrite,ywrite,50);
        glVertex3f(xwrite,ywrite+scale_a,50);
        
        glVertex3f(xwrite-5*pixel,ywrite,50);
        glVertex3f(xwrite+5*pixel,ywrite,50);
        
        
        glVertex3f(xwrite-3*pixel,ywrite+0.2*scale_a,50);
        glVertex3f(xwrite+3*pixel,ywrite+0.2*scale_a,50);
        
        glVertex3f(xwrite-3*pixel,ywrite+0.4*scale_a,50);
        glVertex3f(xwrite+3*pixel,ywrite+0.4*scale_a,50);
        
        glVertex3f(xwrite-3*pixel,ywrite+0.6*scale_a,50);
        glVertex3f(xwrite+3*pixel,ywrite+0.6*scale_a,50);
        
        glVertex3f(xwrite-3*pixel,ywrite+0.8*scale_a,50);
        glVertex3f(xwrite+3*pixel,ywrite+0.8*scale_a,50);
        
        glVertex3f(xwrite-5*pixel,ywrite+scale_a,50);
        glVertex3f(xwrite+5*pixel,ywrite+scale_a,50);
        
        
        glEnd();
        
        glPopMatrix();
        
        
        gl_font(1, 10);const char *p;
        std::stringstream s;
        s.precision(2);
        s<<std::fixed<<0.0;
        xwrite =minx+ 10*pixel;
        ywrite = miny+ 5*pixel;
       
        std::string t= s.str();
        
        glPushMatrix();
        
        glTranslatef(xwrite,ywrite,50); //this one works
        glScalef(0.08*pixel, 0.08*pixel, 0.08*pixel);
        glLineWidth(1.0);
        
        for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) //(p ; *p; p++)
        {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }
        glPopMatrix();
        
        
        
        s.str("");
        s<<std::fixed<<scale_a;
        xwrite =scale_a+minx+ 10*pixel;
       
        t= s.str();
        
        glPushMatrix();
        
        glTranslatef(xwrite,ywrite,50); //this one works
        glScalef(0.08*pixel, 0.08*pixel, 0.08*pixel);
        glLineWidth(1.0);
        
        for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) //(p ; *p; p++)
        {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }
        glPopMatrix();
        
        
        
        
    }
    
    
    
    
    
    
    
}

void MyWindow::showcolorscale(double fore_z,std::vector<double> metric){
    
    
    
    
    double min,max;
    if(metric.size()>0){

        min=9e99;
        for (int i=0;i<metric.size();i++){
            if ((metric[i]<min)&&(metric[i]>=0)){
                min=metric[i];
            }
        }
        
        max=*std::max_element(metric.begin(),metric.end());
    }
    else{
        min=0;//not sure how we would get here...
        max=0;
    }
    

    if ((root->color_scale)&&(show_map->value()!=0)){//start colour scale
        
        
        glPushMatrix();
        
        glBegin(GL_TRIANGLES);
        
        
        double sx=minx+0.9*(maxx-minx);
        double sy=miny+0.1*(maxy-miny);
        double dx=0.05*(maxy-miny),dy=0.8*(maxy-miny)/(double)256;
        
        gl_font(1, 14);
        
        
        for (int i=0;i<=255;i++){
            
            glColor3ub((int)i,(int)(255-i),0);
            
            
            glVertex3f(sx,sy+(i*dy),fore_z);
            glVertex3f(sx+dx,sy+(i*dy),fore_z);
            glVertex3f(sx,sy+((i+1)*dy),fore_z);
            glVertex3f(sx,sy+((i+1)*dy),fore_z);
            glVertex3f(sx+dx,sy+((i+1)*dy),fore_z);
            glVertex3f(sx+dx,sy+(i*dy),fore_z);
            
            
        }
        glEnd();
        
        if (root->bow){
            glColor3ub(0,0,0);
        }
        else{
            glColor3ub(255,255,255);
        }
        
        std::stringstream s;
        s<<min;
        
        glRasterPos3f((sx+1.1*dx),sy+0.8*(maxy-miny),fore_z);
        s.str("");
        s<<max;
        
        double pixel=(maxx-minx)/(double)(this->w());
        
        glPushMatrix();
        
        s.str("");
        s<<std::setprecision(2)<<std::fixed;
        
        if ((min>1000)||(min<0.1)){
            s<<std::scientific;
        }
            
        
        s<<min;
        
        std::string t = s.str();
        
        
        glTranslatef((sx+1.1*dx),sy,fore_z);
        glScalef(0.1*pixel, 0.1*pixel, 0.1*pixel);
        
        glLineWidth(1.0);
        
        for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) //(p ; *p; p++)
        {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }
        glPopMatrix();
        
        glPushMatrix();
        
        s.str("");
        s<<std::setprecision(2)<<std::fixed;
        
        if ((max>1000)||(max<0.1)){
            s<<std::scientific;
        }
        
        s<<max;
        
        t=s.str();
        
        
        glTranslatef((sx+1.1*dx),sy+0.8*(maxy-miny),fore_z);
        glScalef(0.1*pixel, 0.1*pixel, 0.1*pixel);
        
        glLineWidth(1.0);
        
        
        for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) //(p ; *p; p++)
        {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }
        glPopMatrix();
        
        
        glPopMatrix();
        
    }
}

void MyWindow::showrotatinglegend(double fore_z){
    
    
    
    
    if (three_d_choice->value()){ //3d rotating legend
        glPushMatrix();
        double dy=(maxy-miny)*0.05;
        double startx=2*dy+minx;
        double starty=2*dy+miny;
        double startz=fore_z-dy;
        
        double ang_xx,ang_zz;
     
        
        glTranslatef(startx,starty,startz); //translate back to corner
        if (three_d_choice->value()==3){
            glRotatef(ang_xfp,1,0,0);
            glRotatef(ang_zfp,0,0,1);
            ang_xx=(ang_xfp/(360.0))*2.0*M_PI;
            ang_zz=(ang_zfp/(360.0))*2.0*M_PI;
     
        }
        else{
            glRotatef(ang_x,1,0,0);
            glRotatef(ang_z,0,0,1);
            ang_xx=(ang_x/(360.0))*2.0*M_PI;
            ang_zz=(ang_z/(360.0))*2.0*M_PI;
     
        }
        glTranslatef(-startx,-starty,-startz); //translate to 0 in order to rotate
        glLineWidth(1.0);
        drawcubeedge(startx+0.5*dy,starty+0.5*dy,startz+0.5*dy,dy,1,1);
        dy*=1.25;
        glBegin(GL_LINES);
        glColor3ub(255,0,0);
        glVertex3f(startx,starty,startz);glVertex3f(startx+dy,starty,startz);
        glColor3ub(0,255,0);
        glVertex3f(startx,starty,startz);glVertex3f(startx,starty+dy,startz);
        glColor3ub(0,0,255);
        glVertex3f(startx,starty,startz);glVertex3f(startx,starty,startz+dy);
        glEnd();
        
        if (root->bow){
            glColor3ub(0,0,0);
        }
        else{
            glColor3ub(255,255,255);
        }
 
        glPopMatrix();
        
        
        double pixel=(maxx-minx)/(double)(this->w());
        std::stringstream s;
        std::string t;
        
        
        
        
        glPushMatrix();
        
        s.str("");
        s<<"x";
        t = s.str();

        glTranslatef(startx+1.1*dy*cos(ang_zz),starty+1.1*dy*sin(ang_zz)*cos(ang_xx),startz+1.1*dy*sin(ang_zz)*sin(ang_xx)); //this one works
        glScalef(0.1*pixel, 0.1*pixel, 0.1*pixel);
        glLineWidth(1.0);
        
        for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) //(p ; *p; p++)
        {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }
        glPopMatrix();
        
        glPushMatrix();
        s.str("");
        s<<"y";
        t = s.str();
        
        glTranslatef(startx-1.1*dy*sin(ang_zz),starty+1.1*dy*cos(ang_zz)*cos(ang_xx),startz+1.1*dy*cos(ang_zz)*sin(ang_xx));
        glScalef(0.1*pixel, 0.1*pixel, 0.1*pixel);
        glLineWidth(1.0);
        
        for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) //(p ; *p; p++)
        {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }
        glPopMatrix();
        
        glPushMatrix();
        s.str("");
        s<<"z";
        t = s.str();
        
        glTranslatef(startx,starty-1.1*dy*sin(ang_xx),startz+1.1*dy*sin(ang_xx));
        glScalef(0.1*pixel, 0.1*pixel, 0.1*pixel);
        glLineWidth(1.0);
        
        for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) //(p ; *p; p++)
        {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }
        
        glPopMatrix();
        
    }//end 3d rotating legend
    
}


void MyWindow::drawroute(){
    
    glColor3ub(255,0,0);
    glLineWidth(line_thickness->value());
    glBegin(GL_LINES);
    
    double dz =0.5/current->route.size();
    
    for (int i=0;i<current->route.size();i++){
        
        int ii=(current->route.size()-1);
        
        double x=(double)i/(double)ii;
        
        glColor3ub(255-(int)(255*x),(int)(255*x),0);
        
        
        if (current->dual_route){
        
            int j=0;
            while ((j+1)<current->route[i].size()){
                int start = current->route[i][j];
                int end = current->route[i][j+1];
                glVertex3f(current->points[start].x+0.01,current->points[start].y,current->points[start].z+dz*(i+1));
                glVertex3f(current->points[end].x+0.01,current->points[end].y,current->points[end].z+dz*(i+1));
                j+=2;
            }
        }
        else{
            int j=0;
            while (j<current->route[i].size()-1){
                int start = current->route[i][j];
                int end = current->route[i][j+1];
                glVertex3f(current->points[start].x+0.01,current->points[start].y,current->points[start].z+dz*(i+1));
                glVertex3f(current->points[end].x+0.01,current->points[end].y,current->points[end].z+dz*(i+1));
                j++;
            }
            
            
            
        }
      
    }
    glEnd();
    
    
}

void MyWindow::drawaxialmap(std::vector<double> metric,int metric_type){
    
    if (show_map->value()){
        
        
        
        int draw=0;
        int x,y;
        Fl::get_mouse(x,y);
        double xx,yy;
        double pixelx,pixely;
        
        xx= ((double)(x-win->x()-this->x())/((double)this->w()))*(maxx-minx)+minx;
        yy= ((double)(y-win->y()-this->y())/((double)this->h()))*(miny-maxy)+maxy;
        
        pixelx=(maxx-minx)/(double)(this->w());
        pixely=(maxy-miny)/(double)(this->h());
        
        
        double min,max;
        
        if(metric.size()>0){
            //min=*std::min_element(metric.begin(),metric.end());
            
            min=9e99;
            for (int i=0;i<metric.size();i++){
                if ((metric[i]<min)&&(metric[i]>=0)){
                    min=metric[i];
                }
            }
            
            max=*std::max_element(metric.begin(),metric.end());
        }
        else{
            min=0;//not sure how we would get here...
            max=0;
        }
        
        ///BEGIN DRAWING OF ACTUAL GRAPH
        
        double size=point_size->value();//0.2
                        
        
        if ((metric_type==2)&&(root->draw_metric)){
            drawpoints(metric);
         
        }
        else if (!((metric_type==3)&&(root->draw_metric))){
            for (int i=0;i<current->points.size();i++){  //draw the current->points as cubes
                
                glColor3ub(0,0,255);
                if ((selector==12)&&(selected==i)){
                    glColor3ub(255,255,0);
                }
                if ((all_flrs)||((current->points[i].floor==flr_choice))){
                    
                    if (three_d_choice->value()){
                        drawcube(current->points[i].x,current->points[i].y,current->points[i].z,size);
                    }
                    else{
                        drawcube(current->points[i].x,current->points[i].y,0,size);
                        
                    }
                    
                }
            }
       
        }
        
        
        
        for (int ii=0;ii<current->adj_list.size();ii++){
            
            int i = current->adj_list[ii].start;
            int j = current->adj_list[ii].end;
            
            
                
                
                if ((all_flrs)||((current->points[i].floor==flr_choice)&&(current->points[j].floor==flr_choice))){
                    
                    
                    if ((metric_type==3)&&(root->draw_metric)){
                        
                        double metric_mean;
                        
                        int iii=-1;
                        
                        vertex temp;
                        temp.start = i;
                        temp.end =j;
                        
                        iii= current->map.points_2_segs[temp];
                        
               
                        
                        if ((iii>=0)&&(iii<metric.size())){
                            metric_mean=metric[iii];
                            
                            
                            if ((!three_d_choice->value())&&(!draw)){
                                //start distance to segement
                                
                                double x1=current->points[i].x;
                                double x2=current->points[j].x;
                                double y1=current->points[i].y;
                                double y2=current->points[j].y;
                                
                                
                                
                                
                                double px=x2-x1;
                                double py=y2-y1;
                                double s=px*px+py*py;
                                double u=((xx-x1)*px+(yy-y1)*py)/s;
                                if (u>1)u=1;
                                if (u<0)u=0;
                                double dx=x1+u*px-xx;
                                double dy=y1+u*py-yy;
                                double dist=sqrt(dx*dx+dy*dy);
                                
                                
                                
                                //end distance to segement
                                
                                if (dist<0.5){
                                    
                                    draw=1;
                                    
                                    double cx=x1+u*px;
                                    double cy=y1+u*py;
                                    
                                    double sx,sy,sz;
                                    
                                    double lx=70*pixelx;
                                    double ly=40*pixely;
                                    
                                    if (current->points[i].x<0.5*(minx+maxx)){
                                        sx=cx+30*pixelx;
                                    }
                                    else{
                                        sx=cx-30*pixelx-lx;
                                    }
                                    
                                    if (current->points[i].y<0.5*(miny+maxy)){
                                        sy=cy+30*pixely;
                                    }
                                    else{
                                        sy=cy-30*pixely-ly;
                                    }
                                    
                                   
                                    sz=100;
                                    
                                    if (root->bow){
                                        glColor3ub(255,255,255);
                                    }
                                    else{
                                        glColor3ub(0,0,0);
                                    }
                                    
                                    glBegin(GL_TRIANGLES);
                                    
                                    glVertex3f(sx,sy,sz);
                                    glVertex3f(sx+lx,sy,sz);
                                    glVertex3f(sx,sy+ly,sz);
                                    
                                    glVertex3f(sx+lx,sy,sz);
                                    glVertex3f(sx,sy+ly,sz);
                                    glVertex3f(sx+lx,sy+ly,sz);
                                    glEnd();
                                    
                                    if (root->bow){
                                        glColor3ub(0,0,0);
                                    }
                                    else{
                                        glColor3ub(255,255,255);
                                    }
                                    
                                    glLineWidth(1.0);
                                    
                                    glBegin(GL_LINES);
                                    
                                    glVertex3f(cx,cy,sz);
                                    glVertex3f(sx,sy,sz);
                                    
                                    glVertex3f(cx,cy,sz);
                                    glVertex3f(sx+lx,sy,sz);
                                    
                                    glVertex3f(cx,cy,sz);
                                    glVertex3f(sx,sy+ly,sz);
                                    
                                    glVertex3f(cx,cy,sz);
                                    glVertex3f(sx+lx,sy+ly,sz);
                                    
                                    glVertex3f(sx,sy,sz+0.1);
                                    glVertex3f(sx+lx,sy,sz+0.1);
                                    
                                    glVertex3f(sx,sy,sz+0.1);
                                    glVertex3f(sx,sy+ly,sz+0.1);
                                    
                                    glVertex3f(sx+lx,sy,sz+0.1);
                                    glVertex3f(sx+lx,sy+ly,sz+0.1);
                                    
                                    glVertex3f(sx,sy+ly,sz+0.1);
                                    glVertex3f(sx+lx,sy+ly,sz+0.1);
                                    
                                    glEnd();                                                                        
                                    
                                    glLineWidth(1.0);
                                    
                                    
                                    if (root->bow){
                                        glColor3ub(0,0,0);
                                    }
                                    else{
                                        glColor3ub(255,255,255);
                                    }
                                    
                                    std::stringstream s;
                                    
                                    
                                    s<<"metric:";
                                    
                                    std::string t = s.str();
                                    
                                    glPushMatrix();
                                    
                                    glTranslatef(sx+8*pixelx,sy+23*pixely,sz+0.1);
                                    glScalef(0.1*pixelx, 0.1*pixely, 0.1*pixelx);
                                    
                                    
                                    for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) //(p ; *p; p++)
                                    {
                                        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
                                    }
                                    glPopMatrix();
                                    
                                    s.str("");
                                    s<<std::setprecision(2)<<std::fixed;
                                    
                                    if ((metric[iii]>1000)||(metric[iii]<0.1)){
                                        s<<std::scientific;
                                    }
                                    
                                    if (metric[iii]>=0){
                                        s<<metric[iii];
                                    }
                                    else{
                                        s<<"N/A";
                                    }
                                    t = s.str();
                                    
                                    glPushMatrix();
                                    
                                    glTranslatef(sx+8*pixelx,sy+8*pixely,sz+0.1);
                                    glScalef(0.1*pixelx, 0.1*pixely, 0.1*pixelx);
                                    
                                    
                                    for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) //(p ; *p; p++)
                                    {
                                        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
                                    }
                                    glPopMatrix();
                                    
                                    glColor3ub(0,0,255);
                                    
                                }
                                
                                
                                
                            }
                                                                                                                
                            
                            glColor3ub((int)(255*((metric_mean-min)/(max-min))),(int)(255-(255*((metric_mean-min)/(max-min)))),0);
                            
                            if (equal(min,max)){
                                glColor3ub(127,127,0);
                            }
                            
                            
                            if (metric_mean<0){
                                if (root->bow){
                                    glColor3ub(0,0,0);
                                }
                                else{
                                    glColor3ub(255,255,255);
                                }
                            }
                            
                            
                        }
                        else{
                            
                            if (root->bow){
                                glColor3ub(0,0,0);
                            }
                            else{
                                glColor3ub(255,255,255);
                            }
                        }
                        
                        
                        
                        
                        
                    }
                    else if ((metric_type==2)&&(root->draw_metric)){
                        
                        if (root->bow){
                            glColor3ub(0,0,0);
                        }
                        else{
                            glColor3ub(255,255,255);
                        }
                    }
                    else{
                        glColor3ub(0,0,255);
                    }
                    
                    
                    
                    if ((selector==13)&&(selected==ii)){
                        glColor3ub(255,255,0);
                    }
                    
                    glLineWidth(line_thickness->value());
                    glBegin(GL_LINES);
                    if (three_d_choice->value()){
                        glVertex3f(current->points[i].x,current->points[i].y,current->points[i].z);
                        glVertex3f(current->points[j].x,current->points[j].y,current->points[j].z);
                    }
                    else{
                        glVertex3f(current->points[i].x,current->points[i].y,0);
                        glVertex3f(current->points[j].x,current->points[j].y,0);
                        
                    }
                    glEnd();
                }
                
            
            
        }
        
        
    }
    /// END DRAWING OF ACTUAL GRAPH
}

void MyWindow::drawpoints(std::vector<double> metric){
    
    int draw=0;
    int x,y;
    double xx,yy;
    double pixelx,pixely;
    
    Fl::get_mouse(x,y);
    
    
    xx= ((double)(x-win->x()-this->x())/((double)this->w()))*(maxx-minx)+minx;
    yy= ((double)(y-win->y()-this->y())/((double)this->h()))*(miny-maxy)+maxy;
    
    pixelx=(maxx-minx)/(double)(this->w());
    pixely=(maxy-miny)/(double)(this->h());
    
    double min,max;
    if(metric.size()>0){
        
        min=9e99;
        for (int i=0;i<metric.size();i++){
            if ((metric[i]<min)&&(metric[i]>=0)){
                min=metric[i];
            }
        }
        
        max=*std::max_element(metric.begin(),metric.end());
    }
    else{
        min=0;//not sure how we would get here...
        max=0;
    }
    
    
    for (int i=0;i<current->points.size();i++){
        
        double metric_mean;
        
        if (i<metric.size()){
            metric_mean=metric[i];
            glColor3ub((int)(255*((metric_mean-min)/(max-min))),(int)(255-(255*((metric_mean-min)/(max-min)))),0);
            
            if (equal(min,max)){
                glColor3ub(127,127,0);
            }
            if (metric_mean<0){
                if (root->bow){
                    glColor3ub(0,0,0);
                }
                else{
                    glColor3ub(255,255,255);
                }
            }
        }
        else{
            
            if (root->bow){
                glColor3ub(0,0,0);
            }
            else{
                glColor3ub(255,255,255);
            }
        }
        
        
        if ((selector==12)&&(i==selected)){ // selection highlighing
            glColor3ub(255,255,0);
        }
        
        
        if ((all_flrs)||(current->points[i].floor==flr_choice)){
            
            if (three_d_choice->value()){
                drawcube(current->points[i].x,current->points[i].y,current->points[i].z,point_size->value());
                drawcubeedge(current->points[i].x,current->points[i].y,current->points[i].z,point_size->value(),1.51,0);
                
            }
            else{
                drawcube(current->points[i].x,current->points[i].y,0,point_size->value());
                drawcubeedge(current->points[i].x,current->points[i].y,0,point_size->value(),1.51,0);
                
                
                //start
                
                
                if ((!draw)&&(i<metric.size())){
                    
                    double size = point_size->value();
                    if ((fabs(xx-current->points[i].x)<0.5*size)&& (fabs(yy-current->points[i].y)<0.5*size)){
                        
                        draw=1;
                        
                        double sx,sy,sz;
                        
                        double lx=70*pixelx;
                        double ly=40*pixely;
                        
                        if (current->points[i].x<0.5*(minx+maxx)){
                            sx=current->points[i].x+30*pixelx;
                        }
                        else{
                            sx=current->points[i].x-30*pixelx-lx;
                        }
                        
                        if (current->points[i].y<0.5*(miny+maxy)){
                            sy=current->points[i].y+30*pixely;
                        }
                        else{
                            sy=current->points[i].y-30*pixely-ly;
                        }
                                                
                        sz=100;
                        
                        if (root->bow){
                            glColor3ub(255,255,255);
                        }
                        else{
                            glColor3ub(0,0,0);
                        }
                        
                        glBegin(GL_TRIANGLES);
                        
                        glVertex3f(sx,sy,sz);
                        glVertex3f(sx+lx,sy,sz);
                        glVertex3f(sx,sy+ly,sz);
                        
                        glVertex3f(sx+lx,sy,sz);
                        glVertex3f(sx,sy+ly,sz);
                        glVertex3f(sx+lx,sy+ly,sz);
                        glEnd();
                        
                        if (root->bow){
                            glColor3ub(0,0,0);
                        }
                        else{
                            glColor3ub(255,255,255);
                        }
                        
                        glLineWidth(1.0);
                        
                        glBegin(GL_LINES);
                        
                        glVertex3f(current->points[i].x,current->points[i].y,sz);
                        glVertex3f(sx,sy,sz);
                        
                        glVertex3f(current->points[i].x,current->points[i].y,sz);
                        glVertex3f(sx+lx,sy,sz);
                        
                        glVertex3f(current->points[i].x,current->points[i].y,sz);
                        glVertex3f(sx,sy+ly,sz);
                        
                        glVertex3f(current->points[i].x,current->points[i].y,sz);
                        glVertex3f(sx+lx,sy+ly,sz);
                        
                        glVertex3f(sx,sy,sz+0.1);
                        glVertex3f(sx+lx,sy,sz+0.1);
                        
                        glVertex3f(sx,sy,sz+0.1);
                        glVertex3f(sx,sy+ly,sz+0.1);
                        
                        glVertex3f(sx+lx,sy,sz+0.1);
                        glVertex3f(sx+lx,sy+ly,sz+0.1);
                        
                        glVertex3f(sx,sy+ly,sz+0.1);
                        glVertex3f(sx+lx,sy+ly,sz+0.1);
                        
                        glEnd();
                        
                        
                        glLineWidth(1.0);
                        
                        
                        if (root->bow){
                            glColor3ub(0,0,0);
                        }
                        else{
                            glColor3ub(255,255,255);
                        }
                        
                        std::stringstream s;
                        
                        
                        s<<"metric:";
                        
                        std::string t = s.str();
                        
                        glPushMatrix();
                        
                        glTranslatef(sx+8*pixelx,sy+23*pixely,sz+0.1);
                        glScalef(0.1*pixelx, 0.1*pixely, 0.1*pixelx);
                        
                        
                        for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) //(p ; *p; p++)
                        {
                            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
                        }
                        glPopMatrix();
                        
                        s.str("");
                        s<<std::setprecision(2)<<std::fixed;
                        
                        if ((metric[i]>1000)||(metric[i]<0.1)){
                            s<<std::scientific;
                        }
                        
                        if (metric[i]>=0){
                            s<<metric[i];
                        }
                        else{
                            s<<"N/A";
                        }
                        t = s.str();
                        
                        glPushMatrix();
                        
                        glTranslatef(sx+8*pixelx,sy+8*pixely,sz+0.1);
                        glScalef(0.1*pixelx, 0.1*pixely, 0.1*pixelx);
                        
                        
                        for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) //(p ; *p; p++)
                        {
                            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
                        }
                        glPopMatrix();
                        
                        
                        
                    }
                    
                    
                }
                
                
                //end
                
            }
            
        }
        
        
    }        
    
}

void MyWindow::showcoords(){
    // mouse point
    if ((!three_d_choice->value())&&(context_valid())&&(!first_draw)){
        int x,y;
        Fl::get_mouse(x,y);
        double xx,yy;
        xx= ((double)(x-win->x()-this->x())/((double)this->w()))*(maxx-minx)+minx;
        yy= ((double)(y-win->y()-this->y())/((double)this->h()))*(miny-maxy)+maxy;
        
        glPushMatrix();
        
        
        draw_snapon(xx,yy); //use mouse call to draw snap on...
        
        
        if (xx>maxx)xx=maxx;
        if (xx<minx)xx=minx;
        if (yy>maxy)yy=maxy;
        if (yy<miny)yy=miny;
        
        
        if (snapon){
            xx=snapx;
            yy=snapy;
        }
        
        
        gl_font(1, 10);
        
        if (root->bow){
            glColor3ub(0,0,0);
        }
        else{
            glColor3ub(255,255,255);
        }
        const char *p;
        std::stringstream s;
        s.precision(2);
        s<<std::fixed<<xx<<", "<<yy;
        
        double xwrite,ywrite;
        
        double pixel=(maxx-minx)/(double)(this->w());
        
        
        double width_leg=7;
        
        
        if (fabs(xx)<=1){
            width_leg+=0.7;
        }
        else{
            width_leg+=0.7*(floor(log10(fabs(xx)))+1);
        }
        if (fabs(yy)<=1){
            width_leg+=0.7;
        }
        else{
            width_leg+=0.7*(floor(log10(fabs(yy)))+1);
        }
        
       
        if (xx<0)width_leg+=1.5;
        if (yy<0)width_leg+=1.5;
        
        
        xwrite =maxx-width_leg*10*pixel;//- 90*pixel;
        ywrite = miny+ 5*pixel;
        glRasterPos3f(xwrite,ywrite,0);
       
        std::string t = s.str();
        
        
        glPushMatrix();
        
        glTranslatef(xwrite,ywrite,0);
        glScalef(0.1*pixel, 0.1*pixel, 0.1*pixel);
       
        glLineWidth(1.0);
       
        for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) //(p ; *p; p++)
        {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }
        glPopMatrix();
        
        
        glPopMatrix();
        
        
        
        
    }
}

void MyWindow::draw_snapon(double /*xx*/,double /*yy*/){
    
    if (snapon){
        
        
        
        
        double dx=7.0*(maxx-minx)/(double)this->w();
        

        glColor3ub(255,0,0);
        glLineWidth(1.0);
        glBegin(GL_LINES);
        glVertex3f(snapx-dx,snapy,50);
        glVertex3f(snapx+dx,snapy,50);
        glVertex3f(snapx,snapy-dx,50);
        glVertex3f(snapx,snapy+dx,50);
        glEnd();
        circle temp;
        temp.x=snapx;
        temp.y=snapy;
        temp.r=dx;
        DrawCircle(temp,50);
        
        
        if ((mark_locations_choice->value()==12)&&(snap_point>-1)&&(snap_point<current->points.size())){  //snap line when drawing current->points
            
            
            if (root->bow){
                glColor3ub(0,0,0);
            }
            else{
                glColor3ub(255,255,255);
            }
            glBegin(GL_LINES);
            glVertex3f(current->points[snap_point].x,current->points[snap_point].y,current->points[snap_point].z);
            glVertex3f(snapx,snapy,0);
            glEnd();
            
            double pixels=35*(maxx-minx)/(double)(this->w());

            glColor3ub(255,0,0);
            glBegin(GL_LINES);
            glVertex3f(current->points[snap_point].x,current->points[snap_point].y,current->points[snap_point].z);
            glVertex3f(current->points[snap_point].x+pixels*cos(ang_snap),current->points[snap_point].y+pixels*sin(ang_snap),0);

            glVertex3f(current->points[snap_point].x,current->points[snap_point].y,current->points[snap_point].z);
            glVertex3f(current->points[snap_point].x+pixels*cos(ang_snap+0.5*M_PI),current->points[snap_point].y+pixels*sin(ang_snap+0.5*M_PI),0);
            glEnd();
            
        }
        else if((mark_locations_choice->value()==1)&&(snap_point>-1)&&(snap_point<current->points.size())){
            
            if (root->bow){
                glColor3ub(0,0,0);
            }
            else{
                glColor3ub(255,255,255);
            }
            glBegin(GL_LINES);
            glVertex3f(current->desks[snap_point].loc.x,current->desks[snap_point].loc.y,current->desks[snap_point].loc.z);
            glVertex3f(snapx,snapy,0);
            glEnd();
            
            double pixels=35*(maxx-minx)/(double)(this->w());
            
            glColor3ub(255,0,0);
            glBegin(GL_LINES);
            glVertex3f(current->desks[snap_point].loc.x,current->desks[snap_point].loc.y,current->desks[snap_point].loc.z);
            glVertex3f(current->desks[snap_point].loc.x+pixels*cos(ang_snap),current->desks[snap_point].loc.y+pixels*sin(ang_snap),0);
            
            glVertex3f(current->desks[snap_point].loc.x,current->desks[snap_point].loc.y,current->desks[snap_point].loc.z);
            glVertex3f(current->desks[snap_point].loc.x+pixels*cos(ang_snap+0.5*M_PI),current->desks[snap_point].loc.y+pixels*sin(ang_snap+0.5*M_PI),0);
            glEnd();
            
        }
        
        
        if ((snap_point>-1)&&( ((mark_locations_choice->value()==12)  && (snap_point<current->points.size())     ) || ((mark_locations_choice->value()==1) && (current->desks.size())       )    ) ){
            
            double xx,yy;
            switch (mark_locations_choice->value()){
                case 1:
                    xx=current->desks[snap_point].loc.x;
                    yy=current->desks[snap_point].loc.y;
                    break;
                case 12:
                    xx=current->points[snap_point].x;
                    yy=current->points[snap_point].y;
                    break;
            }
            
            std::stringstream s;
            s.precision(2);
            s<<std::fixed<<ang_snap*180.0/M_PI<<"°";
            
            gl_font(1, 10);const char *p;
            double xwrite,ywrite,pixel;
            pixel=(maxx-minx)/(double)(this->w());
            xwrite =xx+ 7*pixel;
            ywrite = yy+ 7*pixel;
            glRasterPos3f(xwrite,ywrite,50);
           
            
            std::string t = s.str();
            
            glPushMatrix();
            
            glTranslatef(xwrite,ywrite,50); //this one works
            glScalef(0.1*pixel, 0.1*pixel, 0.1*pixel);
            glLineWidth(1.0);
            
            for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) //(p ; *p; p++)
            {
                glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
            }
            glPopMatrix();
            
        }
        
        
    }
    
    
}


void MyWindow::show_FOV(){
    
    if (three_d_choice->value()>1){
        std::stringstream s;
        s.precision(2);
        if (three_d_choice->value()==3){
            s<<std::fixed<<"x: "<<xfp<<" y: "<<yfp<<" z: "<<zfp<<" ";
        }
        else{
            s<<std::fixed<<"distance: "<<z_dist<<" ";
        }
        s<<std::fixed<<"FOV: "<<180.0*(2.0*(atan(0.5*(maxx-minx)/(maxx_overall-minx_overall)))/M_PI)<<"°";
        
      
        
        gl_font(1, 10);const char *p;
        double xwrite,ywrite,pixel;
        pixel=(maxx-minx)/(double)(this->w());
        xwrite =minx+ 10*pixel;
        ywrite = miny+ 5*pixel;
        glRasterPos3f(xwrite,ywrite,50);

        std::string t= s.str();
        
        glPushMatrix();
        glTranslatef(xwrite, ywrite, 0);
        glScalef(0.1*pixel, 0.1*pixel, 1/152.38);
        for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) //(p ; *p; p++)
        {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }
        glPopMatrix();
        
      
    }
}



bool is_number(const std::string &str)
{
    return str.find_first_not_of("-.0123456789") == std::string::npos;
}





void MyWindow::move_alt(){
    
    if(three_d_choice->value()==3){ //from handle e==12 swithc statemtn
        
        
        
        double convert=2.0*M_PI*0.0027777777777777777777777777; //2pi/360
        
        
        double ang_zz;
    
        ang_zz=ang_zfp*convert;
        
        
        
        double zz_z=0;//cos(ang_xx); //how much use z is described in terms of model z
        
        double xx_z=-sin(ang_zz);//sx*sin(ang_zz);//how much user z is described in terms of world x //DONT DELETE
        double yy_z=-cos(ang_zz);//sx*cos(ang_zz);//how much user z is described in terms of world y
        
        
        if ((!pressed_for)&&(Fl::event_key(87))){ //FORWARD KEY
            pressed_for=1;
            move_start_time=boost::posix_time::microsec_clock::local_time(); //boost
            move_time=move_start_time;
            
        }
        else if ((pressed_for)&&(!Fl::event_key(87))){
            pressed_for=0;
            
        }
        else if (pressed_for){
            
            
            double time_to_max,max_step_length;
            double step_length_overall;
            
            boost::posix_time::ptime time =boost::posix_time::microsec_clock::local_time();
            boost::posix_time::time_duration dur = time-move_start_time;
            double time_since_press=dur.total_milliseconds()*0.001;
            
            time_to_max=1.5;
            max_step_length=10.0;
            step_length_overall= (time_since_press<time_to_max) ? (0.5+0.5*time_since_press*(1.0/time_to_max))*max_step_length : max_step_length;
            
            dur = time-move_time; //boost
            step_length=step_length_overall*dur.total_milliseconds()*0.001; //boost
            
            if (step_length>max_step_length)step_length=max_step_length; //just for safety: shouldnt need it with initialisation of move_time
            
            move_time=time;
            
            
            xfp-=step_length*xx_z;
            yfp-=step_length*yy_z;
            zfp-=step_length*zz_z;
                        
            
        }

        
    }
}


void MyWindow::every_frame_calc_alt(){
    
    move_alt();
    
}


void MyWindow::draw() {  //massive need to tidy up

    if ((minx>maxx)||(miny>maxy)){ //something has gone horribly wrong: start again.
        first_draw=1;
        this->redraw();
    }
    

    
        every_frame_calc_alt();
    

    
    if (!valid()){        
        load_textures();
    }

        if (floor_choice->value()){
            all_flrs=0;
        }
        else{
            all_flrs=1;
        }
        flr_choice=floor_choice->value()-1;
        
    
        glViewport(0,0,w(),h());
    
        glLoadIdentity();
        
        if (root->bow){
            glClearColor(0.95,0.95,0.95,0);//background colour
        }
        else{
            glClearColor(0,0,0,0);
        }
        
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        
        double back_z=-(maxx_overall-minx_overall)/min_zoom,fore_z=-back_z; //arbitrary z limits : change?
        
        
        
      
        
        
        
        setzoom(); // figure out what projection to setup
                
        
        //SET UP PROJECTION
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        
        
        
        //START PROJECTION SETUP
        
        
        
        double xplane=(maxx-minx),yplane=(maxy-miny);
        
        
        
        if ((three_d_choice->value()==2)){
            
            glFrustum(-0.001*(0.5*xplane),0.001*(0.5*xplane),-0.001*(0.5*yplane),0.001*(0.5*yplane),0.001*(maxx_overall-minx_overall),5000);
            
        }
        else if (three_d_choice->value()==3){
            // glFrustum(-0.0001,0.0001,-0.0001,0.0001,0.0001,10000000); //if not FOV changes allowed
            glFrustum(-0.00001*(0.5*xplane),0.00001*(0.5*xplane),-0.00001*(0.5*yplane),0.00001*(0.5*yplane),0.00001*(maxx_overall-minx_overall),4000000);
        }
        else{
            glOrtho(minx,maxx,miny,maxy,back_z-(maxy-miny)*0.05-1,fore_z+(maxy-miny)+1); //set the orthographic current->matrix (back_z creates room for rotating legend
        }
        //END PROJECTION SET UP
        
        
        
        //START DRAWING THINGS
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        translate_frustum();
        
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPushMatrix();
        
        
        set_rotation();
        
        
        //lighting for walls
        
        GLfloat mat_ambient[]    = { 1.0, 1.0, 1.0, 1.0 };  // RGBA
        GLfloat mat_diffuse[]    = { 1.0, 1.0, 1.0, 1.0 };  // RGBA
        GLfloat mat_specular[]   = { 1.0, 1.0, 1.0, 1.0 };  // RGBA
        GLfloat light_position[] = { minx_overall-65.0, maxy_overall +10.0, 100.0, 1.0 };  // XYZ
        glShadeModel(GL_SMOOTH);
        //
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   mat_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   mat_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  mat_specular);
        glMaterialf(GL_FRONT_AND_BACK,  GL_SHININESS, 20.0);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        
        
    
                GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
        GLfloat light_diffuse[] = { 0.5, 0.4, 0.4, 1.0 };
        GLfloat light_specular[] = { 0.0, 0.0, 0.0, 1.0 };
        GLfloat light_position1[] = { maxx_overall+65.0, miny_overall-10.0, 100.0, 1.0 };  // XYZ
        
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        
        glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
        glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0);
    
        
        glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
        glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
        
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
        
        //end lighting
        
        glEnable(GL_DEPTH_TEST);
    
        
        if (root->draw_grid){
            draw_background(back_z);
        }
        
        
        
        std::vector<double> metric;
        std::vector<double> desk_metric;
        int exit_draw=0;
        
       
        
        metric_type=0;
        
        if ((metric_slot_choice->value()>=0)&&(metric_slot_choice->value()<root->current->metrics.size())){
            
            
            desk_metric = root->current->metrics[metric_slot_choice->value()];
            metric=desk_metric;
            metric_type = root->current->metric_des[metric_slot_choice->value()].basis+1;
            
        }
            
        
            
            
            glColor3ub(255,255,255);
            draw_floor();
        draw_ceiling();
        
            if (three_d_choice->value()<3){draw_floorplan(back_z+0.01);}
            
        
        if (show_floor_areas->value()){
            draw_areas(back_z+0.01);
        }
        
        
        
                drawaxialmap(metric,metric_type);
        
            if (root->draw_route){
                drawroute();
            }
            
            
            draw_locations(desk_metric,metric_type);
            
          
            
            
            if ((!three_d_choice->value())&&(start_link>-1)&&(end_link<0)){
                
                
                int x,y;
                Fl::get_mouse(x,y);
                double xx,yy;
                xx= ((double)(x-win->x()-this->x())/((double)this->w()))*(maxx-minx)+minx;
                yy= ((double)(y-win->y()-this->y())/((double)this->h()))*(miny-maxy)+maxy;
                glColor3ub(255,0,0);
                glLineWidth(1.0);
                glBegin(GL_LINES);
                glVertex3f(current->points[start_link].x,current->points[start_link].y,0);
                glVertex3f(xx,yy,0);
                glEnd();
            }
            
            
            
            draw_visibility();
            
        
        /* begin modern openGL
        GLuint triangleVBO;
        
        //Vertices of a triangle (counter-clockwise winding)
        float data[] = {1.0, 0.0, 1.0, 0.0, 0.0, -1.0, -1.0, 0.0, 1.0};
        //try float data[] = {0.0, 1.0, 0.0, -1.0, -1.0, 0.0, 1.0, -1.0, 0.0}; if the above doesn't work.
        
        //Create a new VBO and use the variable id to store the VBO id
        glGenBuffers(1, &triangleVBO);
        
        //Make the new VBO active
        glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
        
        //Upload vertex data to the video device
        glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
        
        //Make the new VBO active. Repeat here incase changed since initialisation
        glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
        
        //Draw Triangle from VBO - do each time window, view point or data changes
        //Establish its 3 coordinates per vertex with zero stride in this array; necessary here
        glVertexPointer(3, GL_FLOAT, 0, NULL);
        
        //Establish array contains vertices (not normals, colours, texture coords etc)
        glEnableClientState(GL_VERTEX_ARRAY);
        
        //Actually draw the triangle, giving the number of vertices provided
        glDrawArrays(GL_TRIANGLES, 0, sizeof(data) / sizeof(float) / 3);
        END MODERN OPENGL*/
        
            
            
            draw_walls(current->walls_sitting,current->walls_standing);
                
            showpoints();
            showsegments();
    
            
            glPopMatrix();

            
            gl_color(FL_GRAY);
           
            glEnable(GL_DEPTH_TEST);
            
            
            
            if (three_d_choice->value()==2){ //reset perspective for anotations
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                glOrtho(minx,maxx,miny,maxy,back_z-(maxy-miny)*0.05-1,fore_z+(maxy-miny)+1);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                
            }
            else if (three_d_choice->value()==3){ //reset perspective for anotations
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                glOrtho(minx,maxx,miny,maxy,back_z-(maxy-miny)*0.05-1,fore_z+(maxy-miny)+1);
                glMatrixMode(GL_MODELVIEW);
                glLoadIdentity();
                
            }
            
            showrotatinglegend(fore_z);
            showscale();
            show_FOV();
            showcolorscale(fore_z,metric);
            
       
            
            showcoords();
            
            
        

    
        if (first_draw){
            first_draw=0; //finished first draw
        }
    
    
    
}



int intersect_test(double x1,double x2,double y1,double y2, double x3,double x4,double y3,double y4){//doesnt't need to be so fast
    
    int intersect=0;
    
    
    double x=((x1*y2-y1*x2)*(x3-x4)-(x1-x2)*(x3*y4-y3*x4))/((x1-x2)*(y3-y4)-(y1-y2)*(x3-x4));
    double y=((x1*y2-y1*x2)*(y3-y4)-(y1-y2)*(x3*y4-y3*x4))/((x1-x2)*(y3-y4)-(y1-y2)*(x3-x4));
    
    if ((y>=std::min(y1,y2))&&(y<=std::max(y1,y2))&&(x>=std::min(x1,x2))&&(x<=std::max(x1,x2))&&(y>=std::min(y3,y4))&&(y<=std::max(y3,y4))&&(x>=std::min(x3,x4))&&(x<=std::max(x3,x4))){
        intersect=1; //include end current->points
    }    
    
    return intersect;
    
}





void MyWindow::deselect(){
    
    selected=-1;
    selector=-1;
    drag_point=-1;
    selected_matrix[0]=-1;
    selected_matrix[1]=-1;
    start_link=-1;
    end_link=-1;
    set_first=0;
    
}





int MyWindow::handle(int e) {
    
    
       
    int ret = Fl_Gl_Window::handle(e);
    
    
    if ((ret==FL_FOCUS)||(ret==FL_UNFOCUS)){
        return 1;
    }
    
   // snap_point=-1;
    shift_on=Fl::event_shift();
    
    
    if (!shift_on){
        snap_point=-1;
    }
    
    int inside=Fl::event_inside(this);
    
  
    if((inside)&&(snapon)){
        cursor(FL_CURSOR_NONE);
    }
    else if(inside){ //should this all be in tabs or windows handle?
        cursor(FL_CURSOR_CROSS);
    }
    else{
        cursor(FL_CURSOR_DEFAULT);
    }
    
        
    if ((shift_on)&&(!three_d_choice->value())&&(snap_point==-1)){
            
            //snap_point=-1;
            
            
            if (mark_locations_choice->value()==12){ //points
                
                double min=9e99,dist;
                int x,y;
                Fl::get_mouse(x,y);
                double xx,yy;
                xx= ((double)(x-win->x()-this->x())/((double)this->w()))*(maxx-minx)+minx;
                yy= ((double)(y-win->y()-this->y())/((double)this->h()))*(miny-maxy)+maxy;
                
                for (int i=0;i<current->points.size();i++){
                    if (i!=drag_point){
                        dist=(xx-current->points[i].x)*(xx-current->points[i].x)+(yy-current->points[i].y)*(yy-current->points[i].y);
                        if ((dist<min)&&(current->points[i].floor==flr_choice)){
                            min=dist;
                            snap_point=i;
                        }
                    }
                }
                
            }
            else if (mark_locations_choice->value()==1){ //desks
                double min=9e99,dist;
                int x,y;
                Fl::get_mouse(x,y);
                double xx,yy;
                xx= ((double)(x-win->x()-this->x())/((double)this->w()))*(maxx-minx)+minx;
                yy= ((double)(y-win->y()-this->y())/((double)this->h()))*(miny-maxy)+maxy;
                
                for (int i=0;i<current->desks.size();i++){
                    dist=(xx-current->desks[i].loc.x)*(xx-current->desks[i].loc.x)+(yy-current->desks[i].loc.y)*(yy-current->desks[i].loc.y);
                    if ((dist<min)&&(current->desks[i].loc.floor==flr_choice)){
                        min=dist;
                        snap_point=i;
                    }
                    
                }
                
                
            }
            
            
        }
      
    
    
    
    if (e==12){ // if a button has gone downn
    
        //apparently shift is only counted as a button press on first press, other keys continuously press: who knew
     
        if ((three_d_choice->value()==0)&&(selected>-1)){
            
            if ((selector==10)&&(Fl::event_key(79))){
                if ((selected>=0)&&(selected<current->walls_sitting.size())){
                    current->walls_sitting[selected].transparent=0;
                }
            }
            else if ((selector==10)&&(Fl::event_key(84))){
                if ((selected>=0)&&(selected<current->walls_sitting.size())){
                    current->walls_sitting[selected].transparent=1;
                }
            }
            else if ((selector==11)&&(Fl::event_key(79))){
                if ((selected>=0)&&(selected<current->walls_standing.size())){
                    current->walls_standing[selected].transparent=0;
                }
                
            }
            else if ((selector==11)&&(Fl::event_key(84))){
                if ((selected>=0)&&(selected<current->walls_standing.size())){
                    current->walls_standing[selected].transparent=1;
                }
            }
            return 1;
            
        }
        
        

        if (three_d_choice->value()==2){
            if ((Fl::event_key(FL_Up))||(Fl::event_key(87))){//119

                if (z_dist>1){
                    z_dist-=1;
                }
                
                this->redraw();
                
                return 1;
            }
            else if ((Fl::event_key(FL_Down))||(Fl::event_key(83))){
                if (z_dist<4000){
                    z_dist+=1;
                }
                
                this->redraw();
                
                return 1;
            }
            
            
        }
        
        
    }
    
    
   

    
    // perform snap_on function test
    
    int old_snapon =snapon;
    test_snapon();
    //end snap_on function test
    
    
    
    int  x,y;
    switch (e) {
        case FL_ENTER:

            
            return 1;
            break;

        case FL_MOUSEWHEEL:
            if (Fl::event_inside(this)){
                
                if ((three_d_choice->value()==2)&&(Fl::event_ctrl())){
                 
                    int zooming = Fl::event_dy();
                    
                    if (zooming>0){
                        if (z_dist+zooming<4000){
                            z_dist+=zooming;
                        }
                    }
                    else{
                        if (z_dist+zooming>0){
                            z_dist+=zooming;
                        }
                    }
                    
                }
                else{
                
                
                x= Fl::event_x(); //event_x is relative to 'this', event_x_root is relative to whole window (DUH)
                this->frac_x = (double)x/(double)this->w();
                y= Fl::event_y(); //same as above!!
                this->frac_y = 1.0-(double)y/(double)this->h();
                
                    this->zoom=1;
                    
                
                
                this->zoom_mag=SYS_ZOOM*Fl::event_dy();
                    
                }
                
                this->redraw();
                return 1;
                
            }
            break;
        case FL_RELEASE:
            
            
            return 1;
            break;
        case FL_PUSH:
            
            
            
            
            
            this->xdown=Fl::event_x();
            this->ydown=Fl::event_y();
            this->xdragdown_old=this->xdown;
            this->ydragdown_old=this->ydown;
            this->miny_down=this->miny;
            this->maxy_down=this->maxy;
            this->minx_down=this->minx;
            this->maxx_down=this->maxx;
            
            
        
            
            if (( Fl::event_button() == FL_RIGHT_MOUSE )&&(!three_d_choice->value())) { //right mouse: selection event or close geometry
                
                deselect();
                selector =mark_locations_choice->value();
                
                
                
                
                std::vector<location> temp;temp.clear();
                std::vector<wall> temp_wall;temp_wall.clear();
                std::vector<vec> temp_points;temp_points.clear();
                std::vector<vertex> temp_links;temp_links.clear();
                std::vector<vertex> temp_adj_list; temp_adj_list.clear();
                
                
                int loc_or_wall=-1;
                
                // -1:default
                // 0: location
                // 1: wall
                // 2: spatial graph point
                // 3: spatial graph link
                
                switch (mark_locations_choice->value()) {
                        
                    case 0:
                        loc_or_wall=-1; //just selecting point in space!
                        break;
                    case 1:
                        temp=current->desks;
                        loc_or_wall=0;
                        break;
                    case 2:
                        temp=current->kitchens;
                        loc_or_wall=0;
                        break;
                    case 3:
                        temp=current->male_toilets;
                        loc_or_wall=0;
                        break;
                    case 4:
                        temp=current->female_toilets;
                        loc_or_wall=0;
                        break;
                    case 5:
                        temp=current->printers;
                        loc_or_wall=0;
                        break;
                    case 6:
                        temp=current->meeting_rooms;
                        loc_or_wall=0;
                        break;
                    case 7:
                        temp=current->exits;
                        loc_or_wall=0;
                        break;
                    case 8:
                        temp=current->stairs;
                        loc_or_wall=0;
                        break;
                    case 9:
                        temp=current->lifts;
                        loc_or_wall=0;
                        break;
                    case 10:
                        temp_wall=current->walls_sitting;
                        loc_or_wall=1;
                        break;
                    case 11:
                        temp_wall=current->walls_standing;
                        loc_or_wall=1;
                        break;
                    case 12:
                        temp_points=current->points;
                        loc_or_wall=2;
                        break;
                    case 13:
                        temp_adj_list=current->adj_list;
                      
                        loc_or_wall=3;
                        break;
                    case 14:
                    {
                        
                        selector=0;
                        
                        int ii=current->floorarea_polygons.size();
                        int jj=current->floorarea_polygons[ii-1].size();
                                            
                        
                        //close geometry
                        if (jj>2){
                            int intersect=0;// only close if shape won't intersect itself
                            for (int i=1;i<jj-2;i++){ //no point in testing first or last segment: gets confused anyway.. think about it....
                                
                                intersect+=intersect_test(current->floorarea_polygons[ii-1][0].x,current->floorarea_polygons[ii-1][jj-1].x,current->floorarea_polygons[ii-1][0].y,current->floorarea_polygons[ii-1][jj-1].y,current->floorarea_polygons[ii-1][i].x,current->floorarea_polygons[ii-1][i+1].x,current->floorarea_polygons[ii-1][i].y,current->floorarea_polygons[ii-1][i+1].y);
                                
                            }
                            if (intersect==0){
                                
                                current->floorarea_triangles.push_back(std::vector<triangle>(0));
                                
                                ear_clipping(current->floorarea_polygons[current->floorarea_polygons.size()-1],&current->floorarea_triangles[current->floorarea_triangles.size()-1]);
                                current->floorarea_polygons.push_back(std::vector<vec>(0));
                                calculate_areas(&current->floorarea_polygons);
                            }
                            
                        }
                        else if (jj==0){
                            
                            
                            for (int i=0;i<current->floorarea_polygons.size();i++){
                                
                                double x=((double)Fl::event_x()/(double)this->w())*(maxx-minx)+minx;
                                double y=((double)Fl::event_y()/(double)this->h())*(miny-maxy)+maxy;
                                
                                if (pnpoly(x,y,current->floorarea_polygons[i])){
                                    selector=mark_locations_choice->value();
                                    selected=i;
                                }
                                
                            }
                            
                            
                        }
                    }
                        
                        break;
                    default:
                        break;
                }
                double x=((double)Fl::event_x()/(double)this->w())*(maxx-minx)+minx;
                double y=((double)Fl::event_y()/(double)this->h())*(miny-maxy)+maxy;
                double min=999999;
                
                
                this->xfp=x;
                this->yfp=y;
                this->zfp=1.8;
                    
                
                if ((loc_or_wall==0)&&(temp.size())){ //if selecting location
                    
                    
                    for (int i=0;i<temp.size();i++){
                        if ((all_flrs)||(temp[i].loc.floor==flr_choice)){
                            double dist=sqrt((temp[i].loc.x-x)*(temp[i].loc.x-x)+(temp[i].loc.y-y)*(temp[i].loc.y-y));
                            
                            if ((dist<min)&&(dist<0.5)&&(temp[i].loc.floor==(floor_choice->value()-1))){
                                min=dist;
                                selected=i;
                                this->xfp=temp[i].loc.x;
                                this->yfp=temp[i].loc.y;
                                this->zfp=temp[i].loc.z+1.8;
                            }
                        }
                    }
                    
                    if ((selected>-1)&&(selected<temp.size())){
                        location_label->value(temp[selected].label.c_str());
                        std::stringstream s;
                        s<<temp[selected].point;
                        overwrite_location_point->value(s.str().c_str());
                    }
                    
                }
                else if ((loc_or_wall==1)&&(temp_wall.size())){ // if selecting wall
                    
                    
                    
                    for (int i=0;i<temp_wall.size();i++){
                        if ((all_flrs)||(temp_wall[i].start.floor==flr_choice)){
                            
                            //start distance to segement
                            
                            double x1=temp_wall[i].start.x;
                            double x2=temp_wall[i].end.x;
                            double y1=temp_wall[i].start.y;
                            double y2=temp_wall[i].end.y;
                            
                            double px=x2-x1;
                            double py=y2-y1;
                            double s=px*px+py*py;
                            double u=((x-x1)*px+(y-y1)*py)/s;
                            if (u>1)u=1;
                            if (u<0)u=0;
                            double dx=x1+u*px-x;
                            double dy=y1+u*py-y;
                            double dist=sqrt(dx*dx+dy*dy);
                            
                            //end distance to segement
                            
                            if ((dist<min)&&(dist<0.5)&&(temp_wall[i].start.floor==(floor_choice->value()-1))){
                                min=dist;
                                selected=i;
                            }
                        }
                    }
                }
                else if ((loc_or_wall==2)&&(temp_points.size())){ // if selecting spatial graph point
                    
                    
                    for (int i=0;i<temp_points.size();i++){
                        if ((all_flrs)||(temp_points[i].floor==flr_choice)){
                            double dist=sqrt((temp_points[i].x-x)*(temp_points[i].x-x)+(temp_points[i].y-y)*(temp_points[i].y-y));
                            
                            if ((dist<min)&&(dist<0.5)){
                                min=dist;
                                selected=i;
                                this->xfp=temp_points[i].x;
                                this->yfp=temp_points[i].y;
                                this->zfp=temp_points[i].z+1.8;
                            }
                        }
                    }
                    
                    
                }
                else if ((loc_or_wall==3)&&(temp_adj_list.size())){ // if selecting spatial graph link
                    
                    
                    for (int ii=0;ii<temp_adj_list.size();ii++){
                        int i = temp_adj_list[ii].start;
                        int j = temp_adj_list[ii].end;
                        
                        if ((all_flrs)||((current->points[i].floor==flr_choice)&&(current->points[j].floor==flr_choice))){
                            
                            
                                
                                double x1=current->points[i].x;
                                double x2=current->points[j].x;
                                double y1=current->points[i].y;
                                double y2=current->points[j].y;
                                
                                double px=x2-x1;
                                double py=y2-y1;
                                double s=px*px+py*py;
                                double u=((x-x1)*px+(y-y1)*py)/s;
                                if (u>1)u=1;
                                if (u<0)u=0;
                                double dx=x1+u*px-x;
                                double dy=y1+u*py-y;
                                double dist=sqrt(dx*dx+dy*dy);
                                
                                //end distance to segement
                                
                                if ((dist<min)&&(dist<0.5)){
                                    min=dist;
                                    selected = ii;
                                    
                                }
                            
                        }
                        
                    }                                    
                    
                }
                
               
            } // end of selection/beginning of marking
            else if ((Fl::event_button() == FL_LEFT_MOUSE)&&(!three_d_choice->value())&&(!Fl::event_ctrl())){ //left mouse addition event : has to be 2D and not cancelled with cntrl
                
                
                
                if ((((mark_locations_choice->value()==10)||(mark_locations_choice->value()==11)))){ //drawing walls
                    
                    int temp_first=set_first;
                    deselect();
                    set_first=temp_first;
                    
                    wall temp;
                    temp.start.x=((double)Fl::event_x()/(double)this->w())*(maxx-minx)+minx;
                    temp.start.y=((double)Fl::event_y()/(double)this->h())*(miny-maxy)+maxy;
                    
                    
                    if (snapon){    //snap on overwrite
                        temp.start.x=snapx;
                        temp.start.y=snapy;
                    }
                    
                    
                    //temp
                    
                    temp.start.z=0;
                    temp.start.floor=0;
                    
                    //end temp
                    
                    
                    
                    temp.start.floor=floor_choice->value()-1;
                    temp.start.z=current->floor_index[floor_choice->value()-1];
                    
                    
                    if (set_first==0){
                        temp_wall.start=temp.start;
                        set_first=1;
                    }
                    else{
                        temp_wall.end=temp.start;
                        

                                            
                        if (Fl::event_alt()){
                            temp_wall.transparent=1;
                        }
                        else{
                            temp_wall.transparent=0;
                        }
                        
                        prepare_undo(root);
                        
                        if (mark_locations_choice->value()==10){
                            current->walls_sitting.push_back(temp_wall);
                        }
                        else{
                            current->walls_standing.push_back(temp_wall);
                        }
                        set_first=0;
                        show_warning_visibility();
                    }
                    
                    this->redraw();
                }
                else if((mark_locations_choice->value()==12)){ //marking/editing spatial graph current->points
                    
                    
                    deselect();
                    selector=mark_locations_choice->value();
                                                            
                    
                    if (Fl::event_alt()){ //start dragging a point around
                        
                        double x=((double)Fl::event_x()/(double)this->w())*(maxx-minx)+minx;
                        double y=((double)Fl::event_y()/(double)this->h())*(miny-maxy)+maxy;
                        
                        double min=9e99;
                        double dist_crit =0.5;
                        if (point_size->value()>dist_crit){
                            dist_crit=point_size->value();
                        }
                        
                        for (int i=0;i<current->points.size();i++){
                            if ((all_flrs)||(current->points[i].floor==flr_choice)){
                                double dist=sqrt((current->points[i].x-x)*(current->points[i].x-x)+(current->points[i].y-y)*(current->points[i].y-y));
                                
                                if ((dist<min)&&(dist<dist_crit)){//&&(temp_points[i].loc.floor==(floor_choice->value()-1))){
                                    min=dist;
                                    drag_point=i;
                                }
                            }
                        }

                        
                        
                    }
                    else{ //normal point adding
                        
                        vec temp;
                        temp.x=((double)Fl::event_x()/(double)this->w())*(maxx-minx)+minx;
                        temp.y=((double)Fl::event_y()/(double)this->h())*(miny-maxy)+maxy;
                        temp.floor=floor_choice->value()-1;
                        temp.z=current->floor_index[floor_choice->value()-1];
                        
                        if (snapon){   //snap on overwrite
                            temp.x=snapx;
                            temp.y=snapy;
                        }
                        
                        prepare_undo(root);
                        
                        current->points.push_back(temp);
                                                                        
                        show_warning_map();
                        
                    }
                    
                }
                else if ((mark_locations_choice->value()==13)){ //selecting current->matrix link
                    
                    
                    selected=-1; //can't use deselect() here
                    selector=-1;
                    selected_matrix[0]=-1;
                    selected_matrix[1]=-1;
                    drag_point=-1;
                    
                    double x=((double)Fl::event_x()/(double)this->w())*(maxx-minx)+minx;
                    double y=((double)Fl::event_y()/(double)this->h())*(miny-maxy)+maxy;
                    
                    double min=9e99;
                    
                    int finish_link=0;
                    
                    if ((start_link>-1)&&(end_link<0)){
                        finish_link=1;
                    }                                       
                    
                    double dist_crit =0.5*point_size->value();
                    
                    for (int i=0;i<current->points.size();i++){
                        if ((all_flrs)||(current->points[i].floor==flr_choice)){
                            double dist=sqrt((current->points[i].x-x)*(current->points[i].x-x)+(current->points[i].y-y)*(current->points[i].y-y));
                            
                            
                            
                            if ((dist<min)&&(dist<dist_crit)){
                                min=dist;
                                if (start_link>-1){
                                    end_link=i;
                                                                        
                                    if (start_link!=end_link){
                                        
                                        int found=0;
                                        for (int ii=0;ii<current->adj_list.size();ii++){
                                            
                                            if (((current->adj_list[ii].start==start_link)&&(current->adj_list[ii].end==end_link))||((current->adj_list[ii].start==end_link)&&(current->adj_list[ii].end==start_link))){
                                                found=1;
                                            }
                                            
                                            
                                        }
                                        
                                        if (!found){
                                            vertex temp;
                                            temp.start=start_link;
                                            temp.end=end_link;
                                            current->adj_list.push_back(temp);
                                            show_warning_map();
                                        }
                                        
                                    }
                                    start_link=-1;
                                    end_link=-1;
                                    
                                }
                                else{
                                    start_link=i;                                    
                                }
                            }
                        }
                    }
                    
                    if (finish_link){ //selected first but failed with second
                        if ((start_link>-1)&&(end_link<0)){
                            start_link=-1;
                        }
                    }
                    
                    
                }
                
                else if ((mark_locations_choice->value()==14)){ //floor areas
                    
                    
                    deselect();
                    
                    double x=((double)Fl::event_x()/(double)this->w())*(maxx-minx)+minx;
                    double y=((double)Fl::event_y()/(double)this->h())*(miny-maxy)+maxy;
                    
                    vec temp;
                    temp.x=x;
                    temp.y=y;
                    temp.z=0;
                    temp.floor=floor_choice->value()-1;
                    
                    if (snapon){   //snap on overwrite
                        temp.x=snapx;
                        temp.y=snapy;
                    }
                    
                    
                    int ii=current->floorarea_polygons.size();
                    int jj=current->floorarea_polygons[ii-1].size();
                    
                    //add point
                    
                    int intersect=0;// only add point if created segment doesn't overlap
                    for (int i=0;i<jj-2;i++){ //no point in testing last segment: gets confused anyway
                        intersect+=intersect_test(temp.x,current->floorarea_polygons[ii-1][jj-1].x,temp.y,current->floorarea_polygons[ii-1][jj-1].y,current->floorarea_polygons[ii-1][i].x,current->floorarea_polygons[ii-1][i+1].x,current->floorarea_polygons[ii-1][i].y,current->floorarea_polygons[ii-1][i+1].y);
                    }
                    if (intersect==0){
                        
                        current->floorarea_polygons[current->floorarea_polygons.size()-1].push_back(temp);
                    }
                                                            
                }
                
                else if (mark_locations_choice->value()){ //marking normal locations
                    
                    deselect();
                    
                    
                    selector=mark_locations_choice->value();
                    
                    Fl::focus(location_label);                                     
                    
                    if (Fl::event_alt()){ //start dragging a point around
                        
                        
                        std::vector<location> temp_location;temp_location.clear();
                        
                        switch (mark_locations_choice->value()) {
                                
                            case 0:
                                
                                break;
                            case 1:
                                temp_location=current->desks;
                                
                                break;
                            case 2:
                                temp_location=current->kitchens;
                                
                                break;
                            case 3:
                                temp_location=current->male_toilets;
                                
                                break;
                            case 4:
                                temp_location=current->female_toilets;
                                
                                break;
                            case 5:
                                temp_location=current->printers;
                                
                                break;
                            case 6:
                                temp_location=current->meeting_rooms;
                                
                                break;
                            case 7:
                                temp_location=current->exits;
                                
                                break;
                            case 8:
                                temp_location=current->stairs;
                                
                                break;
                            case 9:
                                temp_location=current->lifts;
                                
                                break;
                        }
                        
                        
                        
                        double x=((double)Fl::event_x()/(double)this->w())*(maxx-minx)+minx;
                        double y=((double)Fl::event_y()/(double)this->h())*(miny-maxy)+maxy;
                        
                        double min=9e99;
                        for (int i=0;i<temp_location.size();i++){
                            if ((all_flrs)||(temp_location[i].loc.floor==flr_choice)){
                                double dist=sqrt((temp_location[i].loc.x-x)*(temp_location[i].loc.x-x)+(temp_location[i].loc.y-y)*(temp_location[i].loc.y-y));
                                
                                if ((dist<min)&&(dist<0.5)){
                                    min=dist;
                                    drag_point=i;
                                }
                            }
                        }
                                               
                        
                        
                    }
                    else{ //actual adding of locations here
                        
                        
                        int found=0;
                        for (int i=0;i<current->points.size();i++){
                            if (current->points[i].floor==floor_choice->value()-1){
                                found=1;
                                break;
                            }
                        }
                        if (found){
                            
                            location temp;
                            temp.loc.x=((double)Fl::event_x()/(double)this->w())*(maxx-minx)+minx;
                            temp.loc.y=((double)Fl::event_y()/(double)this->h())*(miny-maxy)+maxy;
                            
                            
                            if (snapon){   //snap on overwrite
                                temp.loc.x=snapx;
                                temp.loc.y=snapy;
                            }
                            
                            for (int i=0;i<current->points.size();i++){
                                if (current->points[i].floor==(floor_choice->value()-1)){                                                                      
                                    temp.loc.floor=floor_choice->value()-1;
                                    temp.loc.z=current->floor_index[floor_choice->value()-1];
                                    
                                }
                            }
                            
                            
                            
                            double min=99999999; //point set
                            int ii=0;
                            
                            for (int i=0;i<current->points.size();i++){
                                
                                if (current->points[i].floor==temp.loc.floor){
                                    double r= sqrt((current->points[i].x-temp.loc.x)*(current->points[i].x-temp.loc.x)+(current->points[i].y-temp.loc.y)*(current->points[i].y-temp.loc.y));
                                    if (r<min){
                                        ii=i;
                                        min=r;
                                    }
                                }
                            }
                            
                            temp.point=ii;                            
                            
                            temp.segment=0;
                            temp.is_desk=0;
                            
                            temp.label="";
                            
                            prepare_undo(root);
                            
                            switch (mark_locations_choice->value()){
                                case 1:
                                    temp.is_desk=1;
                                    current->desks.push_back(temp);
                                    show_warning_visibility();
                                    break;
                                case 2:
                                    current->kitchens.push_back(temp);
                                    show_warning_visibility();
                                    break;
                                case 3:
                                    current->male_toilets.push_back(temp);
                                    show_warning_visibility();
                                    break;
                                case 4:
                                    current->female_toilets.push_back(temp);
                                    show_warning_visibility();
                                    break;
                                case 5:
                                    current->printers.push_back(temp);
                                    show_warning_visibility();
                                    break;
                                case 6:
                                    current->meeting_rooms.push_back(temp);
                                    show_warning_visibility();
                                    break;
                                case 7:
                                    current->exits.push_back(temp);
                                    show_warning_visibility();
                                    break;
                                case 8:
                                    current->stairs.push_back(temp);
                                    show_warning_visibility();
                                    break;
                                case 9:
                                    current->lifts.push_back(temp);
                                    show_warning_visibility();
                                    break;
                            }
                            std::stringstream s;
                            s<<ii;
                            overwrite_location_point->value(s.str().c_str());
                        }
                        else{
                            push_time(text);
                            push_output(text,"No spatial map points found on this floor! Complete the spatial map to continue.\n");
                        }
                    }
                    this->redraw();
                    
                }
                
            }
            return 1;
            
        case FL_DRAG:
            if (Fl::event_inside(this)){

                
                if ((drag_point>-1)&&(Fl::event_alt())){
                    
                    double x=((double)Fl::event_x()/(double)this->w())*(maxx-minx)+minx;
                    double y=((double)Fl::event_y()/(double)this->h())*(miny-maxy)+maxy;
                    
                    if (snapon){   //snap on overwrite
                        x=snapx;
                        y=snapy;
                    }
                    
                    
                    switch (selector){
                            
                        case 0:
                            break;
                        case 1:
                            current->desks[drag_point].loc.x=x;
                            current->desks[drag_point].loc.y=y;
                            show_warning_visibility();
                            break;
                        case 2:
                            current->kitchens[drag_point].loc.x=x;
                            current->kitchens[drag_point].loc.y=y;
                            show_warning_visibility();
                            break;
                        case 3:
                            current->male_toilets[drag_point].loc.x=x;
                            current->male_toilets[drag_point].loc.y=y;
                            show_warning_visibility();
                            break;
                        case 4:
                            current->female_toilets[drag_point].loc.x=x;
                            current->female_toilets[drag_point].loc.y=y;
                            show_warning_visibility();
                            break;
                        case 5:
                            current->printers[drag_point].loc.x=x;
                            current->printers[drag_point].loc.y=y;
                            show_warning_visibility();
                            break;
                        case 6:
                            current->meeting_rooms[drag_point].loc.x=x;
                            current->meeting_rooms[drag_point].loc.y=y;
                            show_warning_visibility();
                            break;
                        case 7:
                            current->exits[drag_point].loc.x=x;
                            current->exits[drag_point].loc.y=y;
                            show_warning_visibility();
                            break;
                        case 8:
                            current->stairs[drag_point].loc.x=x;
                            current->stairs[drag_point].loc.y=y;
                            show_warning_visibility();
                            break;
                        case 9:
                            current->lifts[drag_point].loc.x=x;
                            current->lifts[drag_point].loc.y=y;
                            show_warning_visibility();
                            break;
                        case 10:
                            current->walls_sitting[drag_point].start.x=x;
                            current->walls_sitting[drag_point].start.y=y;
                            show_warning_visibility();
                            break;
                        case 11:
                            current->walls_standing[drag_point].start.x=x;
                            current->walls_standing[drag_point].start.y=y;
                            show_warning_visibility();
                            break;
                        case 12:
                            current->points[drag_point].x=x;
                            current->points[drag_point].y=y;
                            show_warning_map();
                            break;
                        case 13:
                            break;
                            
                    }
                    
                }
                
                
                
                this->xdragdown=Fl::event_x();
                this->ydragdown=Fl::event_y();
                this->drag=1;
                
                if ((three_d_choice->value())&&(!Fl::event_ctrl())&&(Fl::event_button()!=FL_MIDDLE_MOUSE)){ //only rotate if dragging in 3D!!
                    double ydrag=(this->ydragdown-this->ydragdown_old)/2.0;
                    double xdrag=(this->xdragdown-this->xdragdown_old)/2.0;
                    
                    if (three_d_choice->value()==3){ //more manageable rotation in first person
                        
                        
                        
                        double FOV = 180.0*(2.0*(atan(0.5*(maxx-minx)/(maxx_overall-minx_overall)))/M_PI);
                        
                        xdrag*=FOV/120.0;
                        ydrag*=FOV/120.0;
                       
                        
                        
                            if ((ang_xfp+ydrag<0)&&(ang_xfp+ydrag>-180)){
                                this->ang_xfp+=ydrag;
                            }
                            this->ang_zfp+=xdrag;
                            
                            
                        
                    }
                    else{
                        
                        this->ang_x+=ydrag;
                        this->ang_z+=xdrag;
                    }
                
                    this->drag=0;//only rotate or drag at once
                }
                
                if ((mark_locations_choice->value())&&(!Fl::event_ctrl())&&(Fl::event_button()==FL_LEFT_MOUSE)){ //stop dragging whilst marking unless holding cntrl;
                    this->drag=0;
                }
                
                if ((snap_point>-1)&&((mark_locations_choice->value()==12)||(mark_locations_choice->value()==1))&&(Fl::event_ctrl())){//current->desks or current->points
                    
                  
                    double x=((double)Fl::event_x()/(double)this->w())*(maxx-minx)+minx;
                    double y=((double)Fl::event_y()/(double)this->h())*(miny-maxy)+maxy;
                    
                    double  xx,yy;
                    
                    switch (mark_locations_choice->value()){
                        case 1:
                            xx=current->desks[snap_point].loc.x;
                            yy=current->desks[snap_point].loc.y;
                            break;
                        case 12:
                            xx=current->points[snap_point].x;
                            yy=current->points[snap_point].y;
                            break;
                    }
                                       
                    ang_snap=atan2(y-yy,x-xx);
                    this->drag=0;
                    
                }
                
                
                if (Fl::event_alt()){ // no dragging whilst
                    this->drag=0;
                }
                if (Fl::event_button()==FL_MIDDLE_MOUSE){
                    this->drag=1;
                }
                               
                this->redraw();
                this->xdragdown_old=this->xdragdown;
                this->ydragdown_old=this->ydragdown;
                
                
                
            }
            return 1;
            break;
            
            
        case FL_MOVE:
                        
            this->redraw();
            return 1;
            break;
        default:
            //this->redraw();//for snapon
            break;
    }
  
    
    
    
    if (snapon!=old_snapon){
        this->redraw();
    }
    
    return ret;

}


void MyWindow::test_snapon(){
    
    plan floor_plan;
    if (floor_choice->value()>0){
        floor_plan=current->floor_plans[floor_choice->value()-1];
    }
    int x,y;
    Fl::get_mouse(x,y);
    double xx,yy;
    xx= ((double)(x-win->x()-this->x())/((double)this->w()))*(maxx-minx)+minx;
    yy= ((double)(y-win->y()-this->y())/((double)this->h()))*(miny-maxy)+maxy;
  
    
    
    if ((shift_on)&&(!three_d_choice->value())){
        
        
        
        if ((mark_locations_choice->value()==12)&&(snap_point>-1)&&(snap_point<current->points.size())){ 
                       
            snapon=0;
            
            double min=9e99;
            
            double x1=current->points[snap_point].x;
            double y1=current->points[snap_point].y;
            
            
            double x2= x1+cos(ang_snap);
            double y2= y1+sin(ang_snap);
            
            double px=x2-x1;
            double py=y2-y1;
            double s=px*px+py*py;
            double u=((xx-x1)*px+(yy-y1)*py)/s;
            double dx=x1+u*px-xx;
            double dy=y1+u*py-yy;
            double dist=dx*dx+dy*dy;
            
            
            if ((dist<min)){
                snapon=1;
                snapx=xx+dx;
                snapy=yy+dy;
                min=dist;
            }
            
            
            x2= x1+cos(ang_snap+0.5*M_PI);
            y2= y1+sin(ang_snap+0.5*M_PI);
            
            px=x2-x1;
            py=y2-y1;
            s=px*px+py*py;
            u=((xx-x1)*px+(yy-y1)*py)/s;
            dx=x1+u*px-xx;
            dy=y1+u*py-yy;
            dist=dx*dx+dy*dy;
            
            
            if ((dist<min)){
                snapon=1;
                snapx=xx+dx;
                snapy=yy+dy;            
                min=dist;
            }
            
            
            
        }
        else if((mark_locations_choice->value()==1)&&(snap_point>-1)&&(snap_point<current->desks.size())){ //drawing current->desks
            
            snapon=0;
            
            double min=9e99;
            
            double x1=current->desks[snap_point].loc.x;
            double y1=current->desks[snap_point].loc.y;
            
            
            double x2= x1+cos(ang_snap);
            double y2= y1+sin(ang_snap);
            
            double px=x2-x1;
            double py=y2-y1;
            double s=px*px+py*py;
            double u=((xx-x1)*px+(yy-y1)*py)/s;
            double dx=x1+u*px-xx;
            double dy=y1+u*py-yy;
            double dist=dx*dx+dy*dy;
            
            
            if ((dist<min)){
                snapon=1;
                snapx=xx+dx;
                snapy=yy+dy;                
                min=dist;
            }
            
            
            x2= x1+cos(ang_snap+0.5*M_PI);
            y2= y1+sin(ang_snap+0.5*M_PI);
            
            px=x2-x1;
            py=y2-y1;
            s=px*px+py*py;
            u=((xx-x1)*px+(yy-y1)*py)/s;
            dx=x1+u*px-xx;
            dy=y1+u*py-yy;
            dist=dx*dx+dy*dy;
            
            
            if ((dist<min)){
                snapon=1;
                snapx=xx+dx;
                snapy=yy+dy;               
                min=dist;
            }                        
            
        }
        else{ //regular snap on: onto floor plans and walls
            
            
            
            double shift_x=floor_plan.shift_x;
            double shift_y=floor_plan.shift_y;
            snapon=0;
            
            double min=9e99;
            double cutoff=(0.04*(maxx-minx))*(0.04*(maxx-minx));
            
            
            for (int i=0;i<floor_plan.lines.size();i++){ //snap on to lines
                
                if (floor_plan.layers[floor_plan.lines[i].layer]){
                                       
                    double x1=floor_plan.lines[i].start.x+shift_x;
                    double x2=floor_plan.lines[i].end.x+shift_x;
                    double y1=floor_plan.lines[i].start.y+shift_y;
                    double y2=floor_plan.lines[i].end.y+shift_y;
                    
                    double px=x2-x1;
                    double py=y2-y1;
                    double s=px*px+py*py;
                    double u=((xx-x1)*px+(yy-y1)*py)/s;
                    if (u>1)u=1;
                    if (u<0)u=0;
                    double dx=x1+u*px-xx;
                    double dy=y1+u*py-yy;
                    double dist=dx*dx+dy*dy;
                    
                    //end distance to segement
                    
                    if ((dist<min)&&(dist<cutoff)){
                        snapon=1;
                        snapx=xx+dx;
                        snapy=yy+dy;
                        min=dist;
                    }
                    
                }
            }
            
            //snap on to walls
            if ((mark_locations_choice->value()==10)||(mark_locations_choice->value()==11)){//if editing walls:snap on to walls too
                for (int i=0;i<current->walls_sitting.size();i++){
                    
                    double x1=current->walls_sitting[i].start.x;
                    double x2=current->walls_sitting[i].end.x;
                    double y1=current->walls_sitting[i].start.y;
                    double y2=current->walls_sitting[i].end.y;
                    
                    double px=x2-x1;
                    double py=y2-y1;
                    double s=px*px+py*py;
                    double u=((xx-x1)*px+(yy-y1)*py)/s;
                    if (u>1)u=1;
                    if (u<0)u=0;
                    double dx=x1+u*px-xx;
                    double dy=y1+u*py-yy;
                    double dist=dx*dx+dy*dy;
                    
                    //end distance to segement
                    
                    if ((dist<min)&&(dist<cutoff)){
                        snapon=1;
                        snapx=xx+dx;
                        snapy=yy+dy;
                        min=dist;
                    }
                    
                    
                }
                for (int i=0;i<current->walls_standing.size();i++){
                    
                    
                    double x1=current->walls_standing[i].start.x;
                    double x2=current->walls_standing[i].end.x;
                    double y1=current->walls_standing[i].start.y;
                    double y2=current->walls_standing[i].end.y;
                    
                    double px=x2-x1;
                    double py=y2-y1;
                    double s=px*px+py*py;
                    double u=((xx-x1)*px+(yy-y1)*py)/s;
                    if (u>1)u=1;
                    if (u<0)u=0;
                    double dx=x1+u*px-xx;
                    double dy=y1+u*py-yy;
                    double dist=dx*dx+dy*dy;
                    
                    //end distance to segement
                    
                    if ((dist<min)&&(dist<cutoff)){
                        snapon=1;
                        snapx=xx+dx;
                        snapy=yy+dy;
                        min=dist;
                    }
                    
                }
            }//end snap on to walls
            
            //snap on to current->points: TAKES PRECEDENCE SO RESET MIN
                       
            if (root->snap_to_point){
                min=9e99;
                double r1,r2; //reset MIN: crucial
                for (int i=0;i<floor_plan.lines.size();i++){
                    
                    if (floor_plan.layers[floor_plan.lines[i].layer]){
                        
                        r1=(floor_plan.lines[i].start.x+shift_x-xx)*(floor_plan.lines[i].start.x+shift_x-xx)+(floor_plan.lines[i].start.y+shift_y-yy)*(floor_plan.lines[i].start.y+shift_y-yy);
                        r2=(floor_plan.lines[i].end.x+shift_x-xx)*(floor_plan.lines[i].end.x+shift_x-xx)+(floor_plan.lines[i].end.y+shift_y-yy)*(floor_plan.lines[i].end.y+shift_y-yy);
                        
                        if ((r1<min)&&(r1<cutoff)){
                            min=r1;
                            snapx=floor_plan.lines[i].start.x+shift_x;
                            snapy=floor_plan.lines[i].start.y+shift_y;
                            snapon=1;
                            
                        }
                        if ((r2<min)&&(r2<cutoff)){
                            min=r2;
                            snapx=floor_plan.lines[i].end.x+shift_x;
                            snapy=floor_plan.lines[i].end.y+shift_y;
                            snapon=1;
                        }
                        
                    }
                    
                }
            }
            
            
        }
        
    }
    else{
        snapon=0;
    }
    
    
}

void MyWindow::mouseless_zoom(int direction){

    
    this->frac_x = 0.5;
    this->frac_y = 0.5;
    this->zoom_mag=2*direction;
    this->zoom=1;
    this->redraw();
    Fl::check();
}


void MyWindow::mouseless_zoom_out_entry(Fl_Widget*, void* data){
    
    container* root =(container*) data;
    root->draw->mouseless_zoom(1);
}

void MyWindow::mouseless_zoom_in_entry(Fl_Widget*, void* data){
    
    container* root =(container*) data;
    root->draw->mouseless_zoom(-1);
}

