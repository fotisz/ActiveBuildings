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

/* 
 This header (.h) file is a derived work, derived from the SPLINE library authored by
 John Burkardt which was originally distributed under the GNU LGPL license.
 
 The modifications comprise inclusion of only the functions r8vec_bracket and 
 spline_b_val, incorporation into a single header file and adaption of parameters
 so that the functions deal with std::vectors, where appropriate, instead of arrays.
 
 Since this derived work is statically linked we comply with the LGPL license 
 by providing the source code of this implementation which is now distributed under the
 GNU GPL license according to the above header which is compatible with the terms of the
 GNU LGPL license. Original comments/licensing information included in the original 
 work is included for reference below and before the code.
 */
 
 
 //****************************************************************************80
 //
 //  Purpose:
 //
 //    R8VEC_BRACKET searches a sorted array for successive brackets of a value.
 //
 //  Discussion:
 //
 //    If the values in the vector are thought of as defining intervals
 //    on the real line, then this routine searches for the interval
 //    nearest to or containing the given value.
 //
 //    It is always true that RIGHT = LEFT+1.
 //
 //    If XVAL < X[0], then LEFT = 1, RIGHT = 2, and
 //      XVAL   < X[0] < X[1];
 //    If X(1) <= XVAL < X[N-1], then
 //      X[LEFT-1] <= XVAL < X[RIGHT-1];
 //    If X[N-1] <= XVAL, then LEFT = N-1, RIGHT = N, and
 //      X[LEFT-1] <= X[RIGHT-1] <= XVAL.
 //
 //    For consistency, this routine computes indices RIGHT and LEFT
 //    that are 1-based, although it would be more natural in C and
 //    C++ to use 0-based values.
 //
 //  Licensing:
 //
 //    This code is distributed under the GNU LGPL license.
 //
 //  Modified:
 //
 //    24 February 2004
 //
 //  Author:
 //
 //    John Burkardt
 //
 //  Parameters:
 //
 //    Input, int N, length of input array.
 //
 //    Input, double X[N], an array that has been sorted into ascending order.
 //
 //    Input, double XVAL, a value to be bracketed.
 //
 //    Output, int *LEFT, *RIGHT, the results of the search.
 //


 
 //****************************************************************************80
 //
 //  Purpose:
 //
 //    SPLINE_B_VAL evaluates a cubic B spline approximant.
 //
 //  Discussion:
 //
 //    The cubic B spline will approximate the data, but is not
 //    designed to interpolate it.
 //
 //    In effect, two "phantom" data values are appended to the data,
 //    so that the spline will interpolate the first and last data values.
 //
 //  Licensing:
 //
 //    This code is distributed under the GNU LGPL license.
 //
 //  Modified:
 //
 //    24 February 2004
 //
 //  Author:
 //
 //    John Burkardt
 //
 //  Reference:
 //
 //    Carl deBoor,
 //    A Practical Guide to Splines,
 //    Springer, 2001,
 //    ISBN: 0387953663.
 //
 //  Parameters:
 //
 //    Input, int NDATA, the number of data values.
 //
 //    Input, double TDATA[NDATA], the abscissas of the data.
 //
 //    Input, double YDATA[NDATA], the data values.
 //
 //    Input, double TVAL, a point at which the spline is to be evaluated.
 //
 //    Output, double SPLINE_B_VAL, the value of the function at TVAL.
 //
 

#ifndef SPLINE_INTERP_H
#define SPLINE_INTERP_H

#include <vector>



void r8vec_bracket (  std::vector<double>& x, double xval, int *left,
                    int *right )
{
    int n=x.size();
    int i;
    
    for ( i = 2; i <= n - 1; i++ )
    {
        if ( xval < x[i-1] )
        {
            *left = i - 1;
            *right = i;
            return;
        }
        
    }
    
    *left = n - 1;
    *right = n;
    
    return;
}

double spline_b_val ( std::vector<double>& tdata, std::vector<double>& ydata, double tval )

{
    int ndata=tdata.size();
    double bval;
    int left;
    int right;
    double u;
    double yval;
    //
    //  Find the nearest interval [ TDATA(LEFT), TDATA(RIGHT) ] to TVAL.
    //
    r8vec_bracket ( tdata, tval, &left, &right );
    //
    //  Evaluate the 5 nonzero B spline basis functions in the interval,
    //  weighted by their corresponding data values.
    //
    u = ( tval - tdata[left-1] ) / ( tdata[right-1] - tdata[left-1] );
    yval = 0.0;
    //
    //  B function associated with node LEFT - 1, (or "phantom node"),
    //  evaluated in its 4th interval.
    //
    bval = ( ( (     - 1.0
                * u + 3.0 )
              * u - 3.0 )
            * u + 1.0 ) / 6.0;
    
    if ( 0 < left-1 )
    {
        yval = yval + ydata[left-2] * bval;
    }
    else
    {
        yval = yval + ( 2.0 * ydata[0] - ydata[1] ) * bval;
    }
    //
    //  B function associated with node LEFT,
    //  evaluated in its third interval.
    //
    bval = ( ( (       3.0
                * u - 6.0 )
              * u + 0.0 )
            * u + 4.0 ) / 6.0;
    
    yval = yval + ydata[left-1] * bval;
    //
    //  B function associated with node RIGHT,
    //  evaluated in its second interval.
    //
    bval = ( ( (     - 3.0
                * u + 3.0 )
              * u + 3.0 )
            * u + 1.0 ) / 6.0;
    
    yval = yval + ydata[right-1] * bval;
    //
    //  B function associated with node RIGHT+1, (or "phantom node"),
    //  evaluated in its first interval.
    //
    bval = pow ( u, 3 ) / 6.0;
    
    if ( right+1 <= ndata )
    {
        yval = yval + ydata[right] * bval;
    }
    else
    {
        yval = yval + ( 2.0 * ydata[ndata-1] - ydata[ndata-2] ) * bval;
    }
    
    return yval;
}




#endif /*SPLINE_INTERP*/