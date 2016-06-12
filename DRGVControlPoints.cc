#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include "gctpc/gctpc.h"
#include "drg/usgsmaptypes.h"

void getControlPointParams(int maptype, double wlong, double elong,
                           double nlat, double slat, int& num_across,
                           int& num_down, double& width, double& height)
{
   double tick_width;
   double tick_height;

   width = elong - wlong;
   height = nlat - slat;

   switch (maptype)
   {
      case USGS24000:
      case USGS25000:   tick_width  = 0.04166666667;
                        tick_height = 0.04166666667;
                        break;
      case USGS50000:   tick_width  = 0.08333333333;
                        tick_height = 0.08333333333;
                        break;
      case USGS63360:   if (width < 0.5)
                           tick_width  = 0.08333333333;
                        else
                           tick_width  = 0.16666666667;
                        tick_height = 0.08333333333;
                        break;
      case USGS100000:  tick_width  = 0.25;
                        tick_height = 0.25;
                        break;
      case USGS250000:  tick_width  = 0.5;
                        tick_height = 0.5;
                        break;
      case BEAR_LCC:    tick_width  = 1.0;
                        tick_height = 1.0;
                        break;
      default:          tick_width  = width/20.0;
                        tick_height = height/20.0;
                        break;
   }

   num_across = (int) ((width/tick_width)+0.5) + 1; 
   num_down   = (int) ((height/tick_height)+0.5) + 1;
}



void calcGroundCoords(int maptype, int utmzone, int datum,
                      double wlong, double elong, double nlat, double slat,
                      double x[], double y[], int& numpoints)
{

   // Input:
   //
   //    maptype -- one of the defined constants USGS24000, USGS100000
   //    utmzone -- the utm zone.
   //    datum -- one of the defined constants DATUM_NAD27, DATUM_NAD83
   //    elong, slat -- longitude and latitude of southeast corner of map.
   //
   // Output:
   //    x[],y[] -- coordinates for generated control points. Each array is
   //               assumed to be large enough to hold all generated 
   //               control points.
   //    numpoints -- number of control points generated. 


   int num_across,num_down;
   int cur;
   int i,j;
   double width,height;
   double start_x,start_y;
   double cur_x,cur_y;
   double dx,dy;
   

   double incoor[2];
   long insys;
   long inzone;
   double inparm[15];
   long inunit;
   long indatum;
   long ipr;
   char* efile;
   long jpr;
   char* pfile;
   double outcoor[2];
   long outsys;
   long outzone;
   double outparm[15];
   long outunit;
   long outdatum;
   char* fn27;
   char* fn83;
   long iflg;


   // Calculates ground coordinates for the points in controlpoints[].

   getControlPointParams(maptype,wlong,-1.0*elong,nlat,slat,
                         num_across,num_down,width,height);

   insys = GEO;
   inzone = 0;
   inunit = 4; // Degrees of arc

   switch (datum)
      {
         case DATUM_NAD27:
            indatum =  0;  // NAD27
            outdatum = 0;
            break;

         case DATUM_NAD83:
            indatum =  8;  // NAD83
            outdatum = 8;
            break;

         case DATUM_KRASSOVSKY:
            indatum =  15; // Krassovsky (for Soviet Union)
            outdatum = 15;
            break;

         default:
            printf("Unsupported datum in calculate_groundcoords.\n");
            abort();
            break;
      }

   ipr = 0;  // Error messages to terminal
   efile = NULL;
   jpr = -1; // Don't print projection parameters
   pfile = NULL;

   switch(maptype)
      {
         case USGS24000:
         case USGS25000:
         case USGS50000:
         case USGS63360:
         case USGS100000:
         case USGS250000:
            inparm[0] = inparm[1] = 0;
            outsys = UTM;
            outzone = utmzone;
            break;

         case BEAR_LCC:
            outsys = LAMCC;  // Lambert Conformal Conic
            // Lats and longs for the outparms are 'packed': DDDMMMSSS.SS
            outparm[0] = 0.0;
            outparm[1] = 0.0;
            outparm[2] =  58000000.00;   // First standard parallel: 58N
            outparm[3] =  50000000.00;   // Second std. parallel: 50N
            outparm[4] = 108000000.00;   // Central Meridian: 108E
            outparm[5] =  54000000.00;   // Origin Latitude: 54N
            outparm[6] = 1000000.0;      // False Easting, meters
            outparm[7] = 1000000.0;      // False Northing, meters
            break;

         default:
            printf("Unsupported map type in calculate_groundcoords.\n");
            abort();
            break;
      }
   outunit = 2; // meters
   fn27 = NULL;
   fn83 = NULL;


   // Determine lat,long of northwest corner.
   start_x = (-1.0 * elong) - width;   // gctpc expects west 
                                        // longitudes to be negative.
   start_y = slat + height;   

   cur = 0;
   dx = width / (num_across - 1);
   dy = height / (num_down - 1);
   for (j = 0; j < num_down; j++)
      {
         cur_y = start_y - (j * dy);
         for (i = 0; i < num_across; i++)
            {
               cur_x = start_x + (i * dx);
               incoor[0] = cur_x;
               incoor[1] = cur_y;
         
               gctp(incoor,&insys,&inzone,inparm,&inunit,&indatum,&ipr,efile,
                    &jpr,pfile,outcoor,&outsys,&outzone,outparm,&outunit,
                    &outdatum,fn27,fn83,&iflg);

               x[cur] = outcoor[0];
               y[cur] = outcoor[1];
               cur = cur + 1;
            }
      }
   numpoints = cur;
}


