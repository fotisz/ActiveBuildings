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
#include "plot.h"

#include "spline_interp.h"


#undef min
#undef max

#ifndef M_PI
#define M_PI 3.141592653589793238
#endif


void MyPlot::stat_functions(){
 
    
    got_formula=0;
    
    std::vector<double>* metric1 = NULL;
    std::vector<double>* metric2 = NULL;
    
    int build = hist_building[0]->value();
    int met = hist_metric[0]->value()-1;
    
    if ((met>=0)&&(root->comp_buildings[build]->metrics.size()>met)){
        
        metric1 = &(root->comp_buildings[build]->metrics[met]);
    }
    else{
        got_formula=0;
        return;
    }
    
    build = hist_building[1]->value();
    met = hist_metric[1]->value()-1;
    
    if ((met>=0)&&(root->comp_buildings[build]->metrics.size()>met)){
        
        metric2 = &(root->comp_buildings[build]->metrics[met]);
    }
    else{
        got_formula=0;
        return;
    }
    
    
    if (metric1->size()!=metric2->size()){
        return;
    }
    
    double x=0,x_2=0,y=0,y_2=0,xy=0;
    
    int n=0;
    
    for (int i=0;i<metric1->size();i++){
        
        
        if (((*metric1)[i]>=0)&&((*metric2)[i]>=0)){
        
            x+=(*metric1)[i];
            x_2+=((*metric1)[i])*((*metric1)[i]);
            y+=(*metric2)[i];
            y_2+=(*metric2)[i]*(*metric2)[i];
            xy+=(*metric1)[i]*(*metric2)[i];
            ++n;
        }
    }
    
    double n_1 = 1.0/(double)n;
    
    plot_m = (xy-n_1*x*y)/(x_2-n_1*x*x);
    plot_c = (n_1)*(y - plot_m*x);
    std::cout<<"plot_m "<<plot_m<<" "<<xy<<" "<<x<<" "<<y<<" "<<" "<<x_2<<std::endl;
    
    double top=0,bottom=0;
    for (int i=0;i<metric1->size();i++){
        
        double x_i = (*metric1)[i];
        double y_i = (*metric2)[i];
        
        if ((x_i>=0)&&(y_i>=0)){
        
            top+=(y_i-(plot_m*x_i+plot_c))*(y_i-(plot_m*x_i+plot_c));
            bottom+=(y_i-(n_1*y))*(y_i-(n_1*y));
        }
        
    }
    
    plot_r2 = 1.0-(top/bottom);
    
    plot_r2a = 1.0 - (1.0-plot_r2)*((n-1.0)/(n-2.0));
       
    got_formula=1;
    
    return;
    
}

void MyPlot::histogram(std::vector<double>* /*metric_*/){
    
    
    
    for (int bd=0;bd<4;bd++){
        
        sum_hist[bd]=0;
        hist_n[bd].clear();
        hist_nc[bd].clear();
        hist_x[bd].clear();
        bin_min[bd].clear();
        bin_max[bd].clear();
        bin_sum[bd].clear();
        poly_hist_x[bd].clear();
        poly_hist_y[bd].clear();
        
        
        
        int build = hist_building[bd]->value();
        int met = hist_metric[bd]->value()-1;
        
        std::vector<double>* metric = NULL;
        
        if ((met>=0)&&(root->comp_buildings[build]->metrics.size()>met)){
            
            metric = &(root->comp_buildings[build]->metrics[met]);
        }
        else{
            
            continue;
        }
        
        
        
        int n = metric->size();
        
        if (n>0){
            
            

            this->num_bins = bin_count[bd]->value();
            

            
            double min=9e99,max;
            
            
            for (int i=0;i<n;i++){
                
                if (((*metric)[i]<min)&&((*metric)[i]>=0)){
                    min=(*metric)[i];
                }
                
            }
  
            max=*std::max_element(metric->begin(),metric->end());
            
            if (max==min){
                min-=1.0;
                max+=1.0;
            }
            
            
            if (max>min){
                
                
                
                
                
                
                double gap=(max-min)/(double)(num_bins-1);
                this->hist_gap[bd]=gap;                                               
                
                this->hist_min[bd]=min;
                this->hist_max[bd]=max;
                
                std::vector<double> hist(this->num_bins,0);
                std::vector<double> xx(this->num_bins,0);
                std::vector<double> bmin(this->num_bins,9e99);
                std::vector<double> bmax(this->num_bins,-9e99);
                std::vector<double> bsum(this->num_bins,0);
                
                
                
                
                
                for (int i=0;i<num_bins;i++){
                    xx[i]=min+i*gap;
                }
                
                
                h_mean[0]=0;
                h_std1[0]=0;
                
                for (int i=0;i<n;i++){
                    double m=(*metric)[i];
                    
                    if (m>=0){
                        
                        double x =m-min+0.5*gap;
                        
                        int ii = x/gap;
                        
                        
                        
                        if ((ii<hist.size())&&(ii>=0)){
                            hist[ii]++;
                            sum_hist[bd]++;
                            bsum[ii]+=m;
                            if (m<bmin[ii]){ bmin[ii]=m;}
                            if (m>bmax[ii]){ bmax[ii]=m;}
                            h_mean[0]+=m;
                            h_std1[0]+=m*m;
                        }
                    }
                    
                }
                
                h_std1[0]=(1.0/(double)sum_hist[bd])*sqrt((double)sum_hist[bd]*h_std1[0]-h_mean[0]*h_mean[0]);
                h_mean[0]=h_mean[0]/(double)sum_hist[bd];
                h_std2[0]=h_mean[0]+h_std1[0];
                h_std1[0]=h_mean[0]-h_std1[0];
                
                
                
                
                hist_n[bd]=hist;
                hist_nc[bd]=hist;
                
                
                if (yaxis_choice->value()){
                for (int i=0;i<hist_n[bd].size();i++){ //CONVERSION TO PROBABILITY DENSITY
                    
                    hist_n[bd][i]=hist_n[bd][i]/((double)sum_hist[bd]*gap);
                }
                }
                
               
                hist_x[bd]=xx;
                bin_min[bd]=bmin;
                bin_max[bd]=bmax;
                bin_sum[bd]=bsum;
                              
                
                //START B SPLINE
                
                
                
                poly_hist_x[bd].clear();
                poly_hist_y[bd].clear();
                
                
               
                
                std::vector<double> hist_nd,hist_xd;
                
                hist_xd.push_back(hist_x[bd][0]-hist_gap[bd]);
                hist_nd.push_back(0);
                
                for (int i=0;i<hist_n[bd].size();i++){
                    hist_xd.push_back((double)hist_x[bd][i]);
                    
                   
                        hist_nd.push_back((double)hist_n[bd][i]);
                    
                }
                
                hist_xd.push_back(hist_x[bd][hist_x[bd].size()-1]+hist_gap[bd]);
                hist_nd.push_back(0);
                

                
                
                double start =hist_xd[0];
                double dif=hist_xd[hist_xd.size()-1]-start;
                double n1= 1.0/999.0;
                
                for (int i=0;i<1000;i++){
                    poly_hist_x[bd].push_back(start+(double)i*n1*dif);
                    double temp = spline_b_val(hist_xd,hist_nd,poly_hist_x[bd][i]);
                    poly_hist_y[bd].push_back(temp);
                }
                h_mean[1]=spline_b_val(hist_xd,hist_nd,h_mean[0]);
                h_std1[1]=spline_b_val(hist_xd,hist_nd,h_std1[0]);
                h_std2[1]=spline_b_val(hist_xd,hist_nd,h_std2[0]);
                
             
                
                
                //END B SPLINE
                
               
                
                
            }
        }
        
    }
    
    stat_functions();
}




void MyPlot::setzoom(){
    //START ZOOM LEVELS ETC
    
    double ppsx,ppsy;
 
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
    
    ppsx=(maxx-minx)/w();
    ppsy=(maxy-miny)/h();
    
    
    //END ZOOM LEVELS
    
    
}

void MyPlot::draw_axes(){
    
   
    glColor3ub(0,0,0);
   
    
    
    double ang_xx=(ang_x/(360.0))*2.0*M_PI;
    double ang_yy=(ang_y/(360.0))*2.0*M_PI;
    
    double zz=cos(ang_xx)*cos(ang_yy);
    double yy=sin(ang_xx);//*fabs(cos(ang_x)));
    double xx=-cos(ang_xx)*sin(ang_yy);
    
    double x_z,z_x;
    
    if ((0.5*xx+0.5*yy+0*zz)<(0.5*xx+0.5*yy+(-1)*zz)){
        x_z=-1;
    }
    else{
        x_z=0;
    }
    
    if ((0*xx+0.5*yy+0.5*zz)<(1*xx+0.5*yy+0.5*zz)){
        z_x=1;
    }
    else{
        z_x=0;
    }
    
    z_x=0;
    x_z=0;
    glBegin(GL_LINES);
    //xaxis
    glVertex3f(0,0,x_z);
    glVertex3f(1,0,x_z);
    //yaxis
    glVertex3f(0,0,0);
    glVertex3f(0,1,0);
    //zaxis
    glVertex3f(z_x,0,0);
    glVertex3f(z_x,0,-1);
    
    glEnd();
    
    
}

void MyPlot::draw_grid(){
    
    glColor3ub(200,200,200);
    
 
    
    double dx=1.0/7.0;
    double dy=1.0/7.0;
    double dz=1.0/7.0;
    
    glBegin(GL_LINES);
    
    for (int i=0;i<8;i++){
        
      
        
        //include depth test here for grid face decision: need some kind of transformation similar to previous one
        
        double ang_xx=(ang_x/(360.0))*2.0*M_PI;
        double ang_yy=(ang_y/(360.0))*2.0*M_PI;
        
        double zz=cos(ang_xx)*cos(ang_yy);
        double yy=sin(ang_xx);//*fabs(cos(ang_x)));
        double xx=-cos(ang_xx)*sin(ang_yy);
        glColor3ub(0,0,0);
        glVertex3f(0.5,0.5,-0.5);
        glVertex3f(0.5+xx,0.5+yy,-0.5+zz);
  
        glColor3ub(200,200,200);
        if ((0.5*xx+0*yy+0.5*zz)<(0.5*xx+1*yy+0.5*zz)){
            
  
            glVertex3f(i*dx,0,0);
            glVertex3f(i*dx,0,-1);
            
            glVertex3f(0,0,-(i*dz));
            glVertex3f(1,0,-(i*dz));
  
            
        }
        else{
            glVertex3f(i*dx,1,0);
            glVertex3f(i*dx,1,-1);
            
            glVertex3f(0,1,-(i*dz));
            glVertex3f(1,1,-(i*dz));
        }
        
        if ((0*xx+0.5*yy+0.5*zz)<(1*xx+0.5*yy+0.5*zz)){
            
            

            glVertex3f(0,i*dy,0);
            glVertex3f(0,i*dy,-1);
            
            glVertex3f(0,0,-(i*dz));
            glVertex3f(0,1,-(i*dz));
            //}
            
        }
        else{
            glVertex3f(1,i*dy,0);
            glVertex3f(1,i*dy,-1);
            
            glVertex3f(1,0,-(i*dz));
            glVertex3f(1,1,-(i*dz));
        }
        
        if ((0.5*xx+0.5*yy+(-1)*zz)<(0.5*xx+0.5*yy+0*zz)){
            
            glVertex3f(0,i*dy,-1);
            glVertex3f(1,i*dy,-1);
            
            glVertex3f(0+i*dx,0,-1);
            glVertex3f(0+i*dx,1,-1);
        }
        else{
            if (i>0){
                glVertex3f(0,i*dy,0);
                glVertex3f(1,i*dy,0);
                
                glVertex3f(0+i*dx,0,0);
                glVertex3f(0+i*dx,1,0);
            }
        }
        
        
    }
    glEnd();
}

void MyPlot::draw_hist(){
    
    
    
    

        
        int do_plot=0;
        
        
        double minmin=9e99,maxmax=-9e99,binmax=-9e99,ymax=-9e99;
        int bb;
        
        for (int i=0;i<4;i++){
            

            if ((hist_metric[i]->value())&&(hist_n[i].size())){
                
                do_plot=1;
                
                if (hist_min[i]<minmin){
                    minmin=hist_min[i];
                }
                if (hist_max[i]>maxmax){
                    maxmax=hist_max[i];
                }
                if (hist_gap[i]>binmax){
                    binmax=hist_gap[i];
                    bb=i;
                }
                double yy=(double)(*std::max_element(hist_n[i].begin(),hist_n[i].end()));
                if (yy>ymax){
                    ymax=yy;
                }
            }
            
            
        }
        
        
        if (do_plot>0){
            
            
            
            
            
            xaxis_min=(minmin-0.25*binmax)-binmax;
            xaxis_max=(maxmax+0.25*binmax)+binmax;
            
            yaxis_min=0;
            yaxis_max=1.1*ymax;
            
            
            
            
            double pixelx=(maxx-minx)/(double)(this->w());
            double pixely=(maxy-miny)/(double)(this->h());
            glLineWidth(1.0);
            
            int tic=0;
            double ytick=1,lastytick=1;
            
            if (yaxis_choice->value()){
                ytick=0.01*ymax/binmax;
                lastytick=ytick;
            }
            
            double div=yaxis_max/ytick,lastdiv=div+1;
            
            while (fabs(div-10)<fabs(lastdiv-10)){
                
                
                lastytick=ytick;
                if (!tic){
              
                    ytick*=5;
              
                    
                    tic=1;
                }
                else{
              
                    ytick*=2;
              
                    tic=0;
                }
                
                lastdiv=div;
                div=yaxis_max/ytick;
                
              
            }

            
            for (int i=0;i<lastdiv;i++){
                
                double y=i*lastytick/(yaxis_max-yaxis_min);
                
                
                if (y<=1){
                    const char *p;
                    gl_font(1, 14);
                    glColor3f(0,0,0);
                    
                    std::stringstream s;
                    double shift=0;
                    double scale=0.1;
                    if (yaxis_choice->value()){
                        s.precision(2);
                        shift=0.03;
                    }
                    else{
                        s.precision(0);
                    }
                    s<<std::fixed;
                    
                    if (((yaxis_min+(lastdiv-1)*lastytick)>1000)||((yaxis_min+(lastdiv-1)*lastytick)<0.1)){
                        s<<std::scientific;
                        shift=0.04;
                        scale=0.09;
                    }
                    
                   
                    
                    s<<yaxis_min+i*lastytick;
                   
                    std::string t= s.str();
                    
                    
                    glPushMatrix();
                    
                    glTranslatef(-0.03-shift,y-3*pixely,0.02);
                    glScalef(scale*pixelx, scale*pixely, scale*pixelx);
                    
                    glRotatef(-ang_y,0,1,0);
                    glRotatef(-ang_x,1,0,0);
                    
                    for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) //(p ; *p; p++)
                    {
                        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
                    }
                    
                    glPopMatrix();
                    
                    
                    glBegin(GL_LINES);
                    
                    glVertex3f(0,i*lastytick/(yaxis_max-yaxis_min),0);
                    glVertex3f(0.01,i*lastytick/(yaxis_max-yaxis_min),0);
                    glEnd();
                }
                
            }
            
            div=num_bins/1; lastdiv=div+1;
            int ii=1;
            
            while (fabs(div-10)<fabs(lastdiv-10)){
                
                
                ii++;
                
                lastdiv=div;
                div=num_bins/ii;                              
                
            }
            
            --ii;
            if (!ii)ii=1;
            
            
            if (hist_interp_choice->value()!=2){
                
                
                
                for (int bd=0;bd<4;bd++){
                    
                    if ((hist_metric[bd]->value())&&(hist_n[bd].size())){
                                                                                                
                        double gap = hist_gap[bd]/(xaxis_max-xaxis_min);
                        
                        for (int i=0;i<hist_x[bd].size();i++){
                            
                            double xs,xe,ys,ye,zs,ze;
                            
                            zs=(bd+1)*0.1+bd*0.12;
                            ze=zs+0.12;
                            
                            
                            
                            
                            xs=(hist_x[bd][i]-0.5*hist_gap[bd]-xaxis_min)/(xaxis_max-xaxis_min);
                            xe=xs+gap;
                            ys=0;
                            ye=(double)(hist_n[bd][i]-yaxis_min)/(double)(yaxis_max-yaxis_min);
                            
                           
                            
                            if (i==hist_select){
                                
                           
                                double sx=189*pixelx,sy=97*pixely;
                           
                                
                                double x=xs+0.5*gap-0.5*sx;
                                
                                double xd=0.5*sx-0.5*gap;
                                
                                x=x+(0.5-(xs+0.5*gap))*xd;
                                
                                double y=ye;
                                
                                y=y+0.1;
                                if ((y+sy)>1){
                                    y=0.7;
                                    if ((xs+0.5*gap)>0.5){
                                        x=xs-sx-0.05;
                                    }
                                    else{
                                        x=xe+0.05;
                                    }
                                }
                                
                                
                                glColor3ub(255,255,255);
                                glBegin(GL_TRIANGLES);
                                
                                glVertex3f(x,y,0.10);
                                glVertex3f(x+sx,y,0.10);
                                glVertex3f(x+sx,y+sy,0.10);
                                
                                glVertex3f(x,y,0.10);
                                glVertex3f(x,y+sy,0.10);
                                glVertex3f(x+sx,y+sy,0.10);
                                
                                
                                
                                
                                glEnd();
                                glColor3ub(0,0,0);
                                glBegin(GL_LINES);
                                glVertex3f(x,y,0.11);
                                glVertex3f(x+sx,y,0.11);
                                glVertex3f(x+sx,y,0.11);
                                glVertex3f(x+sx,y+sy,0.11);
                                glVertex3f(x+sx,y+sy,0.11);
                                glVertex3f(x,y+sy,0.11);
                                glVertex3f(x,y+sy,0.11);
                                glVertex3f(x,y,0.11);
                                
                                glVertex3f(xs+0.5*gap,ye,0.09);
                                glVertex3f(x,y,0.09);
                                glVertex3f(xs+0.5*gap,ye,0.09);
                                glVertex3f(x+sx,y,0.09);
                                glVertex3f(xs+0.5*gap,ye,0.09);
                                glVertex3f(x,y+sy,0.09);
                                glVertex3f(xs+0.5*gap,ye,0.09);
                                glVertex3f(x+sx,y+sy,0.09);
                                glEnd();
                                
                                
                                
                                
                                const char *p;
                                gl_font(1, 14);
                                glColor3f(0,0,0);
                                
                                std::stringstream s;
                                s.precision(2);
                                s.setf( std::ios::fixed, std:: ios::floatfield );
                                
                                for (int iii=0;iii<5;iii++){
                                    s.str("");
                                    
                                    
                                    if ((bin_min[bd][i]>1000)||(bin_min[bd][i]<0.1)){
                                        s<<std::scientific;
                                    }
                                    
                                    switch (iii) {
                                        case 0:
                                            s<<"n: "<<(int)round(hist_nc[bd][i])<<"/"<<sum_hist[bd]<<" ("<<100.0*((double)hist_nc[bd][i]/(double)sum_hist[bd])<<"%)";
                                            break;
                                            
                                        case 1:
                                            s<<"range: ["<<hist_x[bd][i]-0.5*hist_gap[bd]<<", "<<hist_x[bd][i]+0.5*hist_gap[bd]<<")";
                                            break;
                                        case 2:
                                            s<<"bin mean: "<<bin_sum[bd][i]/hist_nc[bd][i];
                                            break;
                                        case 3:
                                            s<<"bin min: "<<bin_min[bd][i];
                                            break;
                                        case 4:
                                            s<<"bin max: "<<bin_max[bd][i];
                                            break;
                                        default:
                                            break;
                                    }
                                                                        
                                    std::string t = s.str();
                                    glPushMatrix();
                                    
                                    glTranslatef(x+pixelx+18*pixelx,y+sy-18*(iii+1)*pixely,0.11);
                                    glScalef(0.11*pixelx, 0.11*pixely, 0.11*pixelx);
                                    
                                    
                                    for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) //(p ; *p; p++)
                                    {
                                        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
                                    }
                                    
                                    glPopMatrix();
                                    
                                    
                                }
                                
                                
                                
                                glColor3ub(255,255,20);
                            }
                            else{
                                glColor3ub(0,0,255);
                            }
                            glBegin(GL_TRIANGLES);
                            glVertex3f(xs,ys,-zs);
                            glVertex3f(xe,ys,-zs);
                            glVertex3f(xs,ye,-zs);
                            
                            glVertex3f(xe,ys,-zs);
                            glVertex3f(xe,ye,-zs);
                            glVertex3f(xs,ye,-zs);
                            
                            
                            glVertex3f(xs,ys,-ze);
                            glVertex3f(xe,ys,-ze);
                            glVertex3f(xs,ye,-ze);
                            
                            glVertex3f(xe,ys,-ze);
                            glVertex3f(xe,ye,-ze);
                            glVertex3f(xs,ye,-ze);
                            
                            
                            glVertex3f(xs,ys,-zs);
                            glVertex3f(xs,ys,-ze);
                            glVertex3f(xs,ye,-ze);
                            
                            glVertex3f(xs,ys,-zs);
                            glVertex3f(xs,ye,-zs);
                            glVertex3f(xs,ye,-ze);
                            
                            
                            glVertex3f(xe,ys,-zs);
                            glVertex3f(xe,ys,-ze);
                            glVertex3f(xe,ye,-ze);
                            
                            glVertex3f(xe,ys,-zs);
                            glVertex3f(xe,ye,-zs);
                            glVertex3f(xe,ye,-ze);
                            
                            
                            glVertex3f(xs,ye,-zs);
                            glVertex3f(xe,ye,-zs);
                            glVertex3f(xs,ye,-ze);
                            
                            glVertex3f(xe,ye,-zs);
                            glVertex3f(xe,ye,-ze);
                            glVertex3f(xs,ye,-ze);
                            
                            
                            glEnd();
                            
                            glColor3ub(0,0,0);
                            glBegin(GL_LINES);
                            glVertex3f(xs-0.0001,ys-0.0001,-zs+0.0001);
                            glVertex3f(xs-0.0001,ye+0.0001,-zs+0.0001);
                            glVertex3f(xe+0.0001,ys-0.0001,-zs+0.0001);
                            glVertex3f(xe+0.0001,ye+0.0001,-zs+0.0001);
                            glVertex3f(xs-0.0001,ye+0.0001,-zs+0.0001);
                            glVertex3f(xe+0.0001,ye+0.0001,-zs+0.0001);
                            
                            glVertex3f(xs-0.0001,ys-0.0001,-ze-0.0001);
                            glVertex3f(xs-0.0001,ye+0.0001,-ze-0.0001);
                            glVertex3f(xe+0.0001,ys-0.0001,-ze-0.0001);
                            glVertex3f(xe+0.0001,ye+0.0001,-ze-0.0001);
                            glVertex3f(xs-0.0001,ye+0.0001,-ze-0.0001);
                            glVertex3f(xe+0.0001,ye+0.0001,-ze-0.0001);
                            
                            
                            glVertex3f(xs-0.0001,ye+0.0001,-zs+0.0001);
                            glVertex3f(xs-0.0001,ye+0.0001,-ze-0.0001);
                            glVertex3f(xe+0.0001,ye+0.0001,-zs+0.0001);
                            glVertex3f(xe+0.0001,ye+0.0001,-ze-0.0001);
                            glEnd();
                            
                            
                           
                        }
                        
                        
                    }
                }
            }
            
            
            
            
            
            
            
            for (int i=0;i<=hist_x[bb].size();i++){
                
                if (!(i%ii)){
                    const char *p;
                    gl_font(1, 14);
                    glColor3f(0,0,0);
                    
                    std::stringstream s;
                    s.precision(2);
                    s.setf( std::ios::fixed, std:: ios::floatfield );
                    
                    
                    
                    
                    if ((hist_x[bb][0]+((double)(hist_x[bb].size()-1)-0.5)*hist_gap[bb]>1000)||(hist_x[bb][0]+((double)(hist_x[bb].size())-0.5)*hist_gap[bb]<0.1)){
                        s<<std::scientific;
                    }
                  
                    
                    s<<hist_x[bb][0]+((double)i-0.5)*hist_gap[bb];
                    
                    std::string t = s.str();
                    
                    
                    glPushMatrix();
                    
                    glTranslatef((hist_x[bb][0]+((double)i-0.5)*hist_gap[bb]-xaxis_min)/(xaxis_max-xaxis_min)-10*pixelx,-0.025,0.05);
                    glScalef(0.1*pixelx, 0.1*pixely, 0.1*pixelx);
                    glRotatef(-ang_y,0,1,0);
                    glRotatef(-ang_x,1,0,0);
                    
                    for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) //(p ; *p; p++)
                    {
                        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
                    }
                    
                    glPopMatrix();
                    
                    
                    glBegin(GL_LINES);
                    
                    glVertex3f((hist_x[bb][0]+((double)i-0.5)*hist_gap[bb]-xaxis_min)/(xaxis_max-xaxis_min),0,0);
                    glVertex3f((hist_x[bb][0]+((double)i-0.5)*hist_gap[bb]-xaxis_min)/(xaxis_max-xaxis_min),0.01,0);
                    glEnd();
                    
                }
            }
            
            {
                int i=1;
            
                while (hist_x[bb][0]-hist_gap[bb]*((double)i-0.5)>minmin){
                    
                    if (!((i-1)%ii)){
                        std::stringstream s;
                        s.precision(2);
                        s.setf( std::ios::fixed, std:: ios::floatfield );
                        
                        
                        if ((hist_x[bb][0]+((double)(hist_x[bb].size()-1)-0.5)*hist_gap[bb]>1000)||(hist_x[bb][0]+((double)(hist_x[bb].size())-0.5)*hist_gap[bb]<0.1)){
                            s<<std::scientific;
                        }
                        
                       
                        
                        s<<hist_x[bb][0]-((double)i-0.5)*hist_gap[bb];
                       
                        std::string t = s.str();
                        
                        
                        glPushMatrix();
                        
                        glTranslatef((hist_x[bb][0]-((double)i-0.5)*hist_gap[bb]-xaxis_min)/(xaxis_max-xaxis_min)-10*pixelx,-0.025,0.05);
                        glScalef(0.1*pixelx, 0.1*pixely, 0.1*pixelx);
                        glRotatef(-ang_y,0,1,0);
                        glRotatef(-ang_x,1,0,0);
                        
                        for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) //(p ; *p; p++)
                        {
                            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
                        }
                        
                        glPopMatrix();
                        
                        
                        glBegin(GL_LINES);
                        
                        glVertex3f((hist_x[bb][0]-((double)i-0.5)*hist_gap[bb]-xaxis_min)/(xaxis_max-xaxis_min),0,0);
                        glVertex3f((hist_x[bb][0]-((double)i-0.5)*hist_gap[bb]-xaxis_min)/(xaxis_max-xaxis_min),0.01,0);
                        glEnd();
                    }
                    ++i;
                }
            }
            
            {
                int i=1;
                
                while (hist_x[bb][hist_x[bb].size()-1]+hist_gap[bb]*((double)i-0.5)<maxmax){
                    
                    if (!((i-1)%ii)){
                        std::stringstream s;
                        s.precision(2);
                        s.setf( std::ios::fixed, std:: ios::floatfield );
                        
                        
                        if ((hist_x[bb][0]+((double)(hist_x[bb].size()-1)-0.5)*hist_gap[bb]>1000)||(hist_x[bb][0]+((double)(hist_x[bb].size())-0.5)*hist_gap[bb]<0.1)){
                            s<<std::scientific;
                        }
                        
                      
                      
                        s<<hist_x[bb][0]-((double)i-0.5)*hist_gap[bb];
                      
                        std::string t = s.str();
                        
                        
                        glPushMatrix();
                        
                        glTranslatef((hist_x[bb][hist_x[bb].size()-1]+((double)i-0.5)*hist_gap[bb]-xaxis_min)/(xaxis_max-xaxis_min)-10*pixelx,-0.025,0.05);
                        glScalef(0.1*pixelx, 0.1*pixely, 0.1*pixelx);
                        glRotatef(-ang_y,0,1,0);
                        glRotatef(-ang_x,1,0,0);
                        
                        for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) //(p ; *p; p++)
                        {
                            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
                        }
                        
                        glPopMatrix();
                        
                        
                        glBegin(GL_LINES);
                        
                        glVertex3f((hist_x[bb][hist_x[bb].size()-1]+((double)i-0.5)*hist_gap[bb]-xaxis_min)/(xaxis_max-xaxis_min),0,0);
                        glVertex3f((hist_x[bb][hist_x[bb].size()-1]+((double)i-0.5)*hist_gap[bb]-xaxis_min)/(xaxis_max-xaxis_min),0.01,0);
                        glEnd();
                    }
                    ++i;
                }
            }
            
            
            if (hist_interp_choice->value()!=1){
                
                
                for (int bd=0;bd<4;bd++){
                    
                   
                    if ((hist_metric[bd]->value())&&(hist_n[bd].size())){
                        
                        double zs=(bd+1)*0.1+(bd*0.12);                                                                   
                        
                        glLineWidth(2.0);
                        for (int i=0;i<(poly_hist_x[bd].size()-1);i++){
                            glColor3ub(255,0,0);
                            
                            glBegin(GL_LINES);
                            
                            double x1=(poly_hist_x[bd][i]-xaxis_min)/(xaxis_max-xaxis_min);
                            double x2=(poly_hist_x[bd][i+1]-xaxis_min)/(xaxis_max-xaxis_min);
                            double y1=(poly_hist_y[bd][i]-yaxis_min)/(yaxis_max-yaxis_min);
                            double y2=(poly_hist_y[bd][i+1]-yaxis_min)/(yaxis_max-yaxis_min);
                            
                            if (y1<0){
                                y1=0;
                            }
                            if (y2<0){
                                y2=0;
                            }
                            glVertex3f(x1,y1,-zs+0.01);
                            glVertex3f(x2,y2,-zs+0.01);
                            glEnd();
                            
                            
                            
                            
                            glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
                            glEnable( GL_BLEND );
                            glEnable(GL_DEPTH_TEST);
                            glDepthMask(GL_FALSE);
                            
                            
                            
                            glBegin(GL_TRIANGLES);
                            
                            switch (bd){
                                case 0:
                                    glColor4f((255.0/256.0),(0.0/256.0),(0.0/256.0),0.25);
                                    break;
                                case 1:
                                    glColor4f((0.0/256.0),(255.0/256.0),(0.0/256.0),0.25);
                                    break;
                                case 2:
                                    glColor4f((155.0/256.0),(0.0/256.0),(255.0/256.0),0.25);
                                    break;
                                case 3:
                                    glColor4f((0.0/256.0),(255.0/256.0),(255.0/256.0),0.25);
                                    break;
                            }
                            
                            glVertex3f(x1,0,-zs+0.01);
                            glVertex3f(x2,0,-zs+0.01);
                            glVertex3f(x1,y1,-zs+0.01);
                            glVertex3f(x1,y1,-zs+0.01);
                            glVertex3f(x2,y2,-zs+0.01);
                            glVertex3f(x2,0,-zs+0.01);
                            glEnd();
                            
                            glDisable(GL_BLEND);
                            glDepthMask(GL_TRUE);
                            
                        }
                        
                        glLineWidth(1.0);
                    }
                    
                }
                
                
            }
            
        }
    
}

void MyPlot::draw_points(){
    
  
        int draw=0;
        int x,y;
        Fl::get_mouse(x,y);
        double xx,yy;
        double pixelx,pixely;
        
        xx= ((double)(x-win->x()-this->x())/((double)this->w()))*(maxx-minx)+minx;
        yy= ((double)(y-win->y()-this->y())/((double)this->h()))*(miny-maxy)+maxy;
        
        pixelx=(maxx-minx)/(double)(this->w());
        pixely=(maxy-miny)/(double)(this->h());
        
        
        
        std::vector<std::vector<double> > plot_metrics;
        
        
        for (int bd=0;bd<3;bd++){
            
        
            
            int build = hist_building[bd]->value();
            int met = hist_metric[bd]->value()-1;
            
            std::vector<double>* metric = NULL;
            
            if (((bd==0)&&(met<0))||((bd==1)&&(met<0))){
                return;                
            }
            
            if ((met>=0)&&(root->comp_buildings[build]->metrics.size()>met)){
                
                metric = &(root->comp_buildings[build]->metrics[met]);
                plot_metrics.push_back(*metric);
            }
            
            
            
        }
        
        int min=-1;
        for (int i=0;i<plot_metrics.size();i++){
            if ((plot_metrics[i].size()<min)||(min<0)){
                min=plot_metrics[i].size();
            }
        }
        
        for (int i=0;i<plot_metrics.size();i++){
            
            
            std::vector<double>::const_iterator first = plot_metrics[i].begin();
            std::vector<double>::const_iterator last = plot_metrics[i].begin() + min -1;
            std::vector<double> temp(first, last);
            plot_metrics[i]=temp;
        }
        
        
        
        
        double xaxis_min0=9e99;//=*std::min_element(plot_metrics[0].begin(),plot_metrics[0].end());
        
        for (int i=0;i<plot_metrics[0].size();i++){
            double temp = plot_metrics[0][i];
            if ((temp<xaxis_min0)&&(temp>=0)){
                xaxis_min0=temp;
            }
        }
        
        double xaxis_max0=*std::max_element(plot_metrics[0].begin(),plot_metrics[0].end());
        double xaxis_min = xaxis_min0 - (((xaxis_max0-xaxis_min0)>0) ? 0.05*(xaxis_max0-xaxis_min0) : 1);
        double xaxis_max = xaxis_max0 + (((xaxis_max0-xaxis_min0)>0) ? 0.05*(xaxis_max0-xaxis_min0) : 1);
        
        double yaxis_min0=9e99;
        
        for (int i=0;i<plot_metrics[1].size();i++){
            double temp = plot_metrics[1][i];
            if ((temp<yaxis_min0)&&(temp>=0)){
                yaxis_min0=temp;
            }
        }
        
        double yaxis_max0=*std::max_element(plot_metrics[1].begin(),plot_metrics[1].end());
        double yaxis_min = yaxis_min0 - (((yaxis_max0-yaxis_min0)>0) ? 0.05*(yaxis_max0-yaxis_min0) : 1);
        double yaxis_max = yaxis_max0 + (((yaxis_max0-yaxis_min0)>0) ? 0.05*(yaxis_max0-yaxis_min0) : 1);
        
        double zaxis_min=0;
        double zaxis_max=0;
        
        if (plot_metrics.size()>2){
            double zaxis_min0=9e99;//*std::min_element(plot_metrics[2].begin(),plot_metrics[2].end());
            
            for (int i=0;i<plot_metrics[2].size();i++){
                double temp = plot_metrics[2][i];
                if ((temp<zaxis_min0)&&(temp>=0)){
                    zaxis_min0=temp;
                }
            }
            
            double zaxis_max0=*std::max_element(plot_metrics[2].begin(),plot_metrics[2].end());
            zaxis_min = zaxis_min0 - (((zaxis_max0-zaxis_min0)>0) ? 0.05*(zaxis_max0-zaxis_min0) : 1);
            zaxis_max = zaxis_max0 + (((zaxis_max0-zaxis_min0)>0) ? 0.05*(zaxis_max0-zaxis_min0) : 1);
        }
        
        
        for (int i=0;i<min;i++){

            
            int valid=1;
            
            double xyz[3];
            
            xyz[0]=0;xyz[1]=0;xyz[2]=0;
            
            for (int ii=0;ii<plot_metrics.size();ii++){
                
                xyz[ii]=plot_metrics[ii][i];
                
                if(xyz[ii]<0){
                    valid=0;
                }
            }
            
            if (!valid){
                continue;
            }
            
            xyz[0]=(xyz[0]-xaxis_min)/(xaxis_max-xaxis_min);
            xyz[1]=(xyz[1]-yaxis_min)/(yaxis_max-yaxis_min);
            xyz[2]=(xyz[2]-zaxis_min)/(zaxis_max-zaxis_min);
            
            if (plot_metrics.size()<3){
                xyz[2]=-0.5;
            }
            
            double ratio =(double)this->h()/(double)this->w();
            
            double xsize=0.02*ratio;
            double ysize=0.02;
            double zsize=0.02;
            
           
            
            glColor3ub(0,0,255);
            
            if ((plot_metrics.size()<3)&&(!draw)){
                
                
            if ((fabs(xx-xyz[0])<0.5*xsize)&&(fabs(yy-xyz[1])<0.5*ysize)){
            
                draw=1;
                
                double cx=xyz[0];
                double cy=xyz[1];
                
                double sx,sy,sz;
                
                double lx=155*pixelx;
                double ly=55*pixely;
                
                if (xyz[0]<0.5*(minx+maxx)){
                    sx=cx+30*pixelx;
                }
                else{
                    sx=cx-30*pixelx-lx;
                }
                
                if (xyz[1]<0.5*(miny+maxy)){
                    sy=cy+30*pixely;
                }
                else{
                    sy=cy-30*pixely-ly;
                }
                                
                sz=100;
                
                
                glColor3ub(255,255,255);
                

                glBegin(GL_TRIANGLES);
                
                glVertex3f(sx,sy,sz);
                glVertex3f(sx+lx,sy,sz);
                glVertex3f(sx,sy+ly,sz);
                
                glVertex3f(sx+lx,sy,sz);
                glVertex3f(sx,sy+ly,sz);
                glVertex3f(sx+lx,sy+ly,sz);
                glEnd();
                
                
                glColor3ub(0,0,0);

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
                
               
                glColor3ub(0,0,0);
               
                std::stringstream s;
                
                
                s<<"Index: "<<i;
                
                std::string t = s.str();
                
                glPushMatrix();
                
                glTranslatef(sx+8*pixelx,sy+38*pixely,sz+0.1);
                glScalef(0.1*pixelx, 0.1*pixely, 0.1*pixelx);
                
                
                for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) //(p ; *p; p++)
                {
                    glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
                }
                glPopMatrix();
                
                
                s.str("");
                s<<std::setprecision(2)<<std::fixed;
                s<<"Variable 1: ";
                
                if ((plot_metrics[0][i]>999)||(plot_metrics[0][i]<0.1)){
                    s<<std::scientific;
                }
                
                if (plot_metrics[0][i]>=0){
                    s<<plot_metrics[0][i];
                }
                else{
                    s<<"N/A";
                }
                t = s.str();
                
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
                s<<"Variable 2: ";
                
                if ((plot_metrics[1][i]>999)||(plot_metrics[1][i]<0.1)){
                    s<<std::scientific;
                }
                
                if (plot_metrics[1][i]>=0){
                    s<<plot_metrics[1][i];
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
                
                
                glColor3ub(255,255,0);
                xyz[2]=-99;
                
            }
            
            }
            
            
            
            drawcross(xyz[0],xyz[1],-xyz[2],xsize,ysize,zsize);
     
        }
        
        glColor3ub(0,0,0);
        
        for (int ax =0;ax<plot_metrics.size();ax++){
            
            
            for (int i=0;i<8;i++){
            
            std::stringstream s;
            s.precision(2);
            s.setf( std::ios::fixed, std:: ios::floatfield );
                
                
             glPushMatrix();   
                
                double sci=0;
                
                switch (ax){
                    case 0:
                        
                        if ((0.14285714285*7*(xaxis_max-xaxis_min)+xaxis_min>1000)||(0.14285714285*7*(xaxis_max-xaxis_min)+xaxis_min<0.1)){
                            s<<std::scientific;
                            sci= 0.0275;
                        }
                        
                        s<<0.14285714285*i*(xaxis_max-xaxis_min)+xaxis_min;
                        glTranslatef(0.14285714285*i-0.02-sci,-0.03,0.05);
                        break;
                    case 1:
                        if ((0.14285714285*7*(yaxis_max-yaxis_min)+yaxis_min>1000)||(0.14285714285*7*(yaxis_max-yaxis_min)+yaxis_min<0.1)){
                            s<<std::scientific;
                            sci=0.025;
                        }
                        s<<0.14285714285*i*(yaxis_max-yaxis_min)+yaxis_min;
                        glTranslatef(-0.06-sci,0.14285714285*i,0.05);
                        break;
                    case 2:
                        if ((0.14285714285*7*(zaxis_max-zaxis_min)+zaxis_min>1000)||(0.14285714285*7*(zaxis_max-zaxis_min)+zaxis_min<0.1)){
                            s<<std::scientific;
                        }
                        s<<0.14285714285*i*(zaxis_max-zaxis_min)+zaxis_min;
                        glTranslatef(-0.05,-0.05,-0.14285714285*i);
                        break;
                }
            

            std::string t = s.str();
            glScalef(0.1*pixelx, 0.1*pixely, 0.1*pixelx);
            glRotatef(-ang_y,0,1,0);
            glRotatef(-ang_x,1,0,0);
            
            for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) //(p ; *p; p++)
            {
                glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
            }
            
            glPopMatrix();
     
            }
            
            
            std::stringstream s;
            s<<"Variable "<<ax+1;
            glPushMatrix(); 
            switch (ax){
                case 0:
                    glTranslatef(0.45,-0.07,0.07);
                    break;
                case 1:
                    glTranslatef(-0.09,0.5,0.07);
                    break;
                case 2:                    
                    glTranslatef(-0.07,-0.07,-0.5);
                    break;
            }
            
            std::string t = s.str();
            

            glScalef(0.1*pixelx, 0.1*pixely, 0.1*pixelx);
            glRotatef(-ang_y,0,1,0);
            glRotatef(-ang_x,1,0,0);
            
            for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) //(p ; *p; p++)
            {
                glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
            }
            
            glPopMatrix();
            
        }
        
        if ((show_regression->value())&&(plot_metrics.size()==2)&&(got_formula)){
            
            
            glColor3ub(255,0,0);
            
            
            double x1 = xaxis_min0;
            double x2 = xaxis_max0;
            double y1 = plot_m * x1 + plot_c;
            double y2 = plot_m * x2 + plot_c;
            
            
            if (y1<yaxis_min){
                y1=yaxis_min;
                x1 = (y1 -plot_c)/plot_m;
            }
            if (y1>yaxis_max){
                y1=yaxis_max;
                x1 = (y1 -plot_c)/plot_m;
            }
            
            if (y2<yaxis_min){
                y2=yaxis_min;
                x2 = (y2 -plot_c)/plot_m;
            }
            if (y2>yaxis_max){
                y2=yaxis_max;
                x2 = (y2 -plot_c)/plot_m;
            }
            
            
            x1=(x1-xaxis_min)/(xaxis_max-xaxis_min);
            x2=(x2-xaxis_min)/(xaxis_max-xaxis_min);
            y1=(y1-yaxis_min)/(yaxis_max-yaxis_min);
            y2=(y2-yaxis_min)/(yaxis_max-yaxis_min);
            
            glBegin(GL_LINES);
            
            
            glVertex3f(x1,y1,-1);
            glVertex3f(x2,y2,-1);
            glEnd();
            
            
            glColor3ub(255,255,255);
            
            double c1_x = 0.2;
            double c1_y = 1.02;
            double c2_x = 0.8;
            double c2_y = c1_y;
            double c3_x = c2_x;
            double c3_y = 1.08;
            double c4_x = c1_x;
            double c4_y = c3_y;
            
            glBegin(GL_TRIANGLES);
            
            glVertex3f(c1_x,c1_y,-1);
            glVertex3f(c2_x,c2_y,-1);
            glVertex3f(c3_x,c3_y,-1);

            glVertex3f(c1_x,c1_y,-1);
            glVertex3f(c3_x,c3_y,-1);
            glVertex3f(c4_x,c4_y,-1);
            glEnd();
            
            glColor3ub(0,0,0);
            
            glBegin(GL_LINES);
            glVertex3f(c1_x,c1_y,0);
            glVertex3f(c2_x,c2_y,0);
            glVertex3f(c2_x,c2_y,0);
            glVertex3f(c3_x,c3_y,0);
            glVertex3f(c3_x,c3_y,0);
            glVertex3f(c4_x,c4_y,0);
            glVertex3f(c4_x,c4_y,0);
            glVertex3f(c1_x,c1_y,0);
            
            glEnd();
            
            
            std::stringstream s;
            s.precision(2);
            
            
            s<<std::scientific;
            
            
            if (fabs(plot_r2)<=1){
            
                s<<"Slope: "<<plot_m<<" Intercept: "<<plot_c<<" R-squared: "<<std::fixed<<plot_r2<<std::endl;
            }
            else{
                s<<"Slope: "<<plot_m<<" Intercept: "<<plot_c<<" R-squared: NaN"<<std::endl;   
            }
            
            
            glPushMatrix();
            
            
            std::string t = s.str();
            
            
            
            
            glTranslatef(c1_x+5*pixelx,c1_y+12*pixely,0);
            glScalef(0.1*pixelx, 0.1*pixely, 0.1*pixelx);
            
            
            for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) //(p ; *p; p++)
            {
                glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
            }
            
            glPopMatrix();
            
            
            
        }
        

    
    
}


void MyPlot::drawcross(double x, double y, double z, double xlength,double ylength,double zlength){
    
    glLineWidth(1.0);
    
    glBegin(GL_LINES);
    
    glVertex3f(x-0.5*xlength,y-0.5*ylength,z-0.5*zlength);
    glVertex3f(x+0.5*xlength,y+0.5*ylength,z+0.5*zlength);

    glVertex3f(x-0.5*xlength,y+0.5*ylength,z+0.5*zlength);
    glVertex3f(x+0.5*xlength,y-0.5*ylength,z-0.5*zlength);
    
    glVertex3f(x-0.5*xlength,y-0.5*ylength,z+0.5*zlength);
    glVertex3f(x+0.5*xlength,y+0.5*ylength,z-0.5*zlength);
    
    glVertex3f(x-0.5*xlength,y+0.5*ylength,z-0.5*zlength);
    glVertex3f(x+0.5*xlength,y-0.5*ylength,z+0.5*zlength);
    
    glEnd();
    
}

void MyPlot::drawcuboid(double x, double y, double z, double xlength,double ylength,double zlength){
    glBegin(GL_TRIANGLES);
    glVertex3f(x-0.5*xlength,y-0.5*ylength,z-0.5*zlength);
    glVertex3f(x+0.5*xlength,y-0.5*ylength,z-0.5*zlength);
    glVertex3f(x-0.5*xlength,y+0.5*ylength,z-0.5*zlength);
    glVertex3f(x-0.5*xlength,y+0.5*ylength,z-0.5*zlength);
    glVertex3f(x+0.5*xlength,y-0.5*ylength,z-0.5*zlength);
    glVertex3f(x+0.5*xlength,y+0.5*ylength,z-0.5*zlength);
    
    glVertex3f(x-0.5*xlength,y-0.5*ylength,z+0.5*zlength);
    glVertex3f(x+0.5*xlength,y-0.5*ylength,z+0.5*zlength);
    glVertex3f(x-0.5*xlength,y+0.5*ylength,z+0.5*zlength);
    glVertex3f(x-0.5*xlength,y+0.5*ylength,z+0.5*zlength);
    glVertex3f(x+0.5*xlength,y-0.5*ylength,z+0.5*zlength);
    glVertex3f(x+0.5*xlength,y+0.5*ylength,z+0.5*zlength);
    
    
    glVertex3f(x-0.5*xlength,y-0.5*ylength,z-0.5*zlength);
    glVertex3f(x-0.5*xlength,y-0.5*ylength,z+0.5*zlength);
    glVertex3f(x-0.5*xlength,y+0.5*ylength,z-0.5*zlength);
    glVertex3f(x-0.5*xlength,y+0.5*ylength,z-0.5*zlength);
    glVertex3f(x-0.5*xlength,y-0.5*ylength,z+0.5*zlength);
    glVertex3f(x-0.5*xlength,y+0.5*ylength,z+0.5*zlength);
    
    glVertex3f(x+0.5*xlength,y-0.5*ylength,z-0.5*zlength);
    glVertex3f(x+0.5*xlength,y-0.5*ylength,z+0.5*zlength);
    glVertex3f(x+0.5*xlength,y+0.5*ylength,z-0.5*zlength);
    glVertex3f(x+0.5*xlength,y+0.5*ylength,z-0.5*zlength);
    glVertex3f(x+0.5*xlength,y-0.5*ylength,z+0.5*zlength);
    glVertex3f(x+0.5*xlength,y+0.5*ylength,z+0.5*zlength);
    
    
    glVertex3f(x-0.5*xlength,y-0.5*ylength,z-0.5*zlength);
    glVertex3f(x-0.5*xlength,y-0.5*ylength,z+0.5*zlength);
    glVertex3f(x+0.5*xlength,y-0.5*ylength,z-0.5*zlength);
    glVertex3f(x+0.5*xlength,y-0.5*ylength,z-0.5*zlength);
    glVertex3f(x-0.5*xlength,y-0.5*ylength,z+0.5*zlength);
    glVertex3f(x+0.5*xlength,y-0.5*ylength,z+0.5*zlength);
    
    glVertex3f(x-0.5*xlength,y+0.5*ylength,z-0.5*zlength);
    glVertex3f(x-0.5*xlength,y+0.5*ylength,z+0.5*zlength);
    glVertex3f(x+0.5*xlength,y+0.5*ylength,z-0.5*zlength);
    glVertex3f(x+0.5*xlength,y+0.5*ylength,z-0.5*zlength);
    glVertex3f(x-0.5*xlength,y+0.5*ylength,z+0.5*zlength);
    glVertex3f(x+0.5*xlength,y+0.5*ylength,z+0.5*zlength);
    glEnd();
}


void MyPlot::drawcuboidedge(double x,double y,double z,double xlength,double ylength, double zlength,double lwidth){
    glLineWidth(lwidth);
    glColor3ub(0,0,0);
    glBegin(GL_LINES);
    
    
    glVertex3f(x-0.5*xlength,y-0.5*ylength,z-0.5*zlength);
    glVertex3f(x+0.5*xlength,y-0.5*ylength,z-0.5*zlength);
    
    glVertex3f(x-0.5*xlength,y-0.5*ylength,z+0.5*zlength);
    glVertex3f(x+0.5*xlength,y-0.5*ylength,z+0.5*zlength);
    glVertex3f(x-0.5*xlength,y+0.5*ylength,z-0.5*zlength);
    glVertex3f(x+0.5*xlength,y+0.5*ylength,z-0.5*zlength);
    glVertex3f(x-0.5*xlength,y+0.5*ylength,z+0.5*zlength);
    glVertex3f(x+0.5*xlength,y+0.5*ylength,z+0.5*zlength);
    
    glVertex3f(x-0.5*xlength,y-0.5*ylength,z-0.5*zlength);
    glVertex3f(x-0.5*xlength,y+0.5*ylength,z-0.5*zlength);
    
    glVertex3f(x-0.5*xlength,y-0.5*ylength,z+0.5*zlength);
    glVertex3f(x-0.5*xlength,y+0.5*ylength,z+0.5*zlength);
    glVertex3f(x+0.5*xlength,y-0.5*ylength,z-0.5*zlength);
    glVertex3f(x+0.5*xlength,y+0.5*ylength,z-0.5*zlength);
    glVertex3f(x+0.5*xlength,y-0.5*ylength,z+0.5*zlength);
    glVertex3f(x+0.5*xlength,y+0.5*ylength,z+0.5*zlength);
    
    glVertex3f(x-0.5*xlength,y-0.5*ylength,z-0.5*zlength);
    glVertex3f(x-0.5*xlength,y-0.5*ylength,z+0.5*zlength);
    
    glVertex3f(x-0.5*xlength,y+0.5*ylength,z-0.5*zlength);
    glVertex3f(x-0.5*xlength,y+0.5*ylength,z+0.5*zlength);
    glVertex3f(x+0.5*xlength,y-0.5*ylength,z-0.5*zlength);
    glVertex3f(x+0.5*xlength,y-0.5*ylength,z+0.5*zlength);
    glVertex3f(x+0.5*xlength,y+0.5*ylength,z-0.5*zlength);
    glVertex3f(x+0.5*xlength,y+0.5*ylength,z+0.5*zlength);
    
    glEnd();
}


void MyPlot::draw() {  
    
    
    
    
    
    //initialise
    glClearColor(0.925,0.925,0.925,0);//background colour
    glLoadIdentity();
    glViewport(0,0,w(),h());
    glEnable(GL_DEPTH_TEST);
    
    minx=-0.3;maxx=1.3;
    miny=-0.3;maxy=1.3;
    
        
            if (plot_choice->value()){
                minx=-0.08;maxx=1.08;
                miny=-0.08;maxy=1.08;
            }
            else{
                minx=-0.10;maxx=1.10;
                miny=-0.10;maxy=1.10;
            }
        
        int count=0;
        for (int i=0;i<4;i++){
            

            if (hist_metric[i]->value()){
                count++;
                
            }
        }
        
        if (count>1){
            
            
            double ay =ang_y*M_PI*0.005555555555555555555;
            double d=sin(0.5*ay);
            d*=d*0.22;
            double th=0.22*sin(0.25*M_PI)*sin(0.25*M_PI);
            if (d>th){
                d=th;
                
            }

            
            minx-=d;
            miny-=d;
            maxx+=d;
            maxy+=d;
        }
        
    
    
    
    
    minz=-200;maxz=200;
    
    glOrtho(minx,maxx,miny,maxy,minz,maxz); //set the orthographic matrix (back_z creates room for rotating legend
    
    
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    //rotate start
    //drawbackground_and_setrotation(back_z);
    
    double mid_x=0.5;
    double mid_y=0.5;
    double mid_z=-0.5;
    
    glTranslatef(mid_x,mid_y,mid_z);
    glRotatef(ang_x,1,0,0);
    glRotatef(ang_y,0,1,0);
    glTranslatef(-mid_x,-mid_y,-mid_z);
    
    glLineWidth(1.0);
    if (!plot_choice->value()){
        draw_points();
    }
    else{
        draw_hist();
    }
    
    draw_axes();
    draw_grid();
    
    
   
    
    glPopMatrix();
    gl_color(FL_GRAY);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_TEST);
    
    //non rotated or translated things from here on....
    
    
    
    if (first_draw){
        first_draw=0; //finished first draw
    }
    
}

int MyPlot::handle(int e) {
    
   
    switch (e) {

        case FL_FOCUS:
            return 1;
            break;
        case FL_UNFOCUS:
            return 1;
            break;
        case FL_ENTER: cursor(FL_CURSOR_CROSS);
            
            
            return 1;
            break;
        case FL_LEAVE: cursor(FL_CURSOR_DEFAULT); break;
        case FL_MOUSEWHEEL:
            return 1;
            break;
        case FL_PUSH:
            this->xdragdown_old=Fl::event_x();
            this->ydragdown_old=Fl::event_y();
            return 1;
        case FL_DRAG:
        {
            
            int count=0;
            
            for (int i=0;i<4;i++){
                
                if (hist_metric[i]->value()){
                    count++;
                    
                }
            }
            
            if (!plot_choice->value()){
                if ((hist_metric[0]->value())&&(hist_metric[1]->value())&&(hist_metric[2]->value())){
                    count=2;
                }
                else{
                    count=0;
                }
            }
            
            if ((count>1)){
                this->xdragdown=Fl::event_x();
                this->ydragdown=Fl::event_y();
                double ydrag=(this->ydragdown-this->ydragdown_old)/2.0;
                double xdrag=(this->xdragdown-this->xdragdown_old)/2.0;
                this->ang_x+=ydrag;
                this->ang_y+=xdrag;
                this->redraw();
                this->xdragdown_old=this->xdragdown;
                this->ydragdown_old=this->ydragdown;
            }
        }

            break;
        case FL_MOVE:
        {
            
            
            hist_select=-1;
            
            int count=0,bd=-1;
            
            for (int i=0;i<4;i++){
                
                if (hist_metric[i]->value()){
                    count++;
                    bd=i;
                }
            }
            
            if ((count==1)&&(hist_x[bd].size())){
                
                
                if (hist_x.size()){
                    double xx=((double)Fl::event_x()/(double)this->w())*(maxx-minx)+minx;
                    double yy=((double)Fl::event_y()/(double)this->h())*(miny-maxy)+maxy;
                    
                    xx=xx*(xaxis_max-xaxis_min)+xaxis_min;
                    yy=yy*(yaxis_max-yaxis_min)+yaxis_min;
                    
                    double ax=(xx-(hist_x[bd][0]-0.5*hist_gap[bd]))/hist_gap[bd];
                    
                    int i;
                    if (ax>0){
                        i=(int)ax;
                    }
                    else{
                        i=-1;
                    }
                    
                    if ((i<hist_n[bd].size())&&(i>=0)){
                        if ((yy>0)&&(yy<=hist_n[bd][i])){
                            hist_select=i;
                        }
                    }
                    redraw();

                }
            }
            
            redraw();
            

            return 1;
            break;
        }
    }
    
    
    
    return Fl_Gl_Window::handle(e);
}

