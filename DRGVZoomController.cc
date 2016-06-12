#include <stdio.h>
#include "DRGVZoomController.h"
#include "drg/coordutils.h"
#include "mvc/Global.h"
#include "WObj/PopupWObj.h"
#include "DRGVMainWObj.h"

extern PopupWObj *mainpopup;
extern DRGVMainWObj *mainwin;

DRGVZoomController::DRGVZoomController(DRGVStuffWObj* stuff)
{
   _stuff = stuff;
   myrascomp = NULL;
   _reportpos = false;
}

DRGVZoomController::DRGVZoomController(RasterComponent2D* rc)
{
   myrascomp = rc;
   _reportpos = false;
}

void DRGVZoomController::setRaster(RasterComponent2D* rc)
{
   myrascomp = rc;
}

void DRGVZoomController::pointerMotion(int x, int y)
{
   Projection2D* tmpproj;
   RasterComponent2D* tmpras;
   double wx,wy;
   int ix,iy;
   char outbuf[1000];
   double longitude, latitude;
   double d1,m1,s1,d2,m2,s2;

   if (_reportpos)
      {
         wx = myview->getWorldSpaceX(x,y);
         wy = myview->getWorldSpaceY(x,y);   

         utm2longlat(wx,wy,_utmzone,"nad27",&longitude,&latitude);

         _stuff->setUTMCoords(wx,wy);
         _stuff->setGeoCoords(longitude,latitude);

      }
}

void DRGVZoomController::keyPressed(char* stringrep, int x, int y)
{
   RasterComponent2D* tmpras;
   char thechar;
   double wx, wy;
   int ix,iy;
   Projection2D* tmpproj;
   unsigned char* therawpix;
   int i; 
   unsigned char tmpc;
   AbstractPixel* tmppix;
   double tmpzf;

   if (strlen(stringrep) > 1)
      return;

   thechar = stringrep[0];

   tmpproj = myview->getProjection();
   wx = tmpproj->getWorldSpaceX(x,y);
   wy = tmpproj->getWorldSpaceY(x,y);   

   // tmpras = ((DRGValidateApp*)theApplication)->getAppRaster();
   tmpras = mainwin->getAppRaster();
   if (tmpras != NULL)
      {
         ix = (int) ((wx - tmpras->getUpperLeftX()) / tmpras->getResolutionX()
                     + 0.5);
         iy = (int) ((tmpras->getUpperLeftY() - wy) / tmpras->getResolutionY()
                     + 0.5);
      }

   switch(thechar)
      {
         case('d'): GlobalDebug.messagesOn(); break;
         case('D'): GlobalDebug.messagesOff(); break;
         case('q'): exit(0); break;
         case('p'): _reportpos = !_reportpos; break;
#if 0
         case('N'): ((DRGValidateApp*)theApplication)->loadNextInputFile(); break;
         case('P'): ((DRGValidateApp*)theApplication)->loadPrevInputFile(); break;
#endif
         case('h'): 
            // print list of key commands
            printf("\n\n");
            printf("q - Quit\n"); 
            printf("i - Index of pixel under mouse\n");
            printf("c - RGB Color values of pixel under mouse\n");
            printf("f - View entire image\n");
            printf("p - Toggle position reporting\n");
            printf("1 through 9 - Change zoom factor\n");
            printf("\n");
            break;
         case('i'):
            if (tmpras == NULL) break; 

            if (tmpras->getRawPixel(wx,wy,(void**)&therawpix))
               { 
                  tmpc = *therawpix;
                  i = tmpc;
                  printf("World (%f,%f) Image (%d,%d) Pallete Index: %d\n",
                         wx,wy,ix,iy,i);
               }
            else
               printf("Mouse not in image.\n");

            delete therawpix;
            break;
         case('c'):
            if (tmpras == NULL) break;

            if (tmpras->getPixel(wx,wy,&tmppix))
               {
                  printf("World (%f,%f) Image (%d,%d) <R,G,B>: <%d,%d,%d>\n",
                         wx,wy,ix,iy,tmppix->getRed(),tmppix->getGreen(),
                         tmppix->getBlue());
                  delete tmppix;
               }
            else
               printf("Mouse not in image.\n");
            break;
         case('f'):
            tmpproj->zoomFullView();
            myview->reDraw();
            break;
         case('1'):
         case('2'):
         case('3'):
         case('4'):
         case('5'):
         case('6'):
         case('7'):
         case('8'):
         case('9'):
            sscanf(stringrep,"%lf",&tmpzf);
            setZoomFactor(tmpzf);
            break;
      } // end switch;
}

void DRGVZoomController::setUTMZone(long zone)
{
   _utmzone = zone;
}


void DRGVZoomController::reportPosition(bool flag)
{
   _reportpos = flag;
}
