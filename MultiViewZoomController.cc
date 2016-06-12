#include <math.h>
#include "MultiViewZoomController.h"
#include "mvc/Global.h"
#include "drg/coordutils.h"

MultiViewZoomController::MultiViewZoomController()
{
   _zoomfactor = 2.0;
   _reportpos = false;
   _utmzone = 0;
}

void MultiViewZoomController::install(void)
{
   (myview->getDisplayDevice())->setCursor(DISPDEV_ZOOM);
}

void MultiViewZoomController::setZoomFactor(double f)
{
   _zoomfactor = f;
}

double MultiViewZoomController::getZoomFactor(void)
{
   return _zoomfactor;
}

void MultiViewZoomController::button1Release(int x, int y)
{
   myview->zoomBy(_zoomfactor);
}

void MultiViewZoomController::button2Release(int x, int y)
{
   myview->zoomBy(-1.0 * _zoomfactor);
}

void MultiViewZoomController::button3Release(int x, int y)
{

}

void MultiViewZoomController::pointerMotion(int x, int y)
{
   // Called when pointer moves. Pointer is now at
   // Device coordinates (x,y).

   double wx,wy;
   char outbuf[1000];
   double longitude,latitude;
   double d1,m1,s1,d2,m2,s2;

#if 0
   if (_reportpos)
      {
         wx = myview->getWorldSpaceX(x,y);
         wy = myview->getWorldSpaceY(x,y);

         utm2longlat(wx,wy,_utmzone,"nad27",&longitude,&latitude);

         sprintf(outbuf,"UTM: %lf,%lf",wx,wy);
         move(11,5); clrtoeol(); addstr(outbuf);

         dec2dms(longitude,d1,m1,s1);
         dec2dms(latitude,d2,m2,s2);
         sprintf(outbuf,"Geographic: %3.0lf %3.0lf %6.3lf  %3.0lf %3.0lf %6.3lf",d1,m1,s1,d2,m2,s2);
         move(12,5); clrtoeol(); addstr(outbuf);

         refresh();
      }
#endif
}

void MultiViewZoomController::button1Motion(int x, int y)
{
}

void MultiViewZoomController::button2Motion(int x, int y)
{
   // Called when pointer moves and Button2 is depressed.
}

void MultiViewZoomController::button3Motion(int x, int y)
{
   // Called when pointer moves and Button3 is depressed.
}

void MultiViewZoomController::pointerEnter(int x, int y)
{
   // Called when pointer enters input area. (x,y) location
   // pointer entered.
}

void MultiViewZoomController::pointerLeave(int x, int y)
{
   // Called when pointer leaves input area. (x,y) location
   // pointer left.
}

void MultiViewZoomController::keyPressed(char* stringrep, int x, int y)
{
   char thechar;
   double wx, wy;
   Projection2D* tmpproj;
   int i; 
   unsigned char tmpc;
   double tmpzf;

   if (strlen(stringrep) > 1)
      return;

   thechar = stringrep[0];

   tmpproj = myview->getProjection();
   wx = tmpproj->getWorldSpaceX(x,y);
   wy = tmpproj->getWorldSpaceY(x,y);   

   switch(thechar)
      {
         case('d'): GlobalDebug.messagesOn(); break;
         case('D'): GlobalDebug.messagesOff(); break;
         case('q'): exit(0); break;
         case('p'): _reportpos = !_reportpos; break;
         case('h'): 
            // print list of key commands
            printf("\n\n");
            printf("q - Quit\n"); 
            printf("f - view entire image\n");
            printf("p - Toggle position reporting\n");
            printf("1 through 9 - Change zoom factor\n");
            printf("\n");
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


void MultiViewZoomController::setUTMZone(long zone)
{
   _utmzone = zone;
}