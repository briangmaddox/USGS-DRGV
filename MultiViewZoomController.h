
#include "mvc/Projection2D.h"
#include "mvc/Controller.h"

#ifndef MULTIVIEWZOOMCONTROLLER_H
#define MULTIVIEWZOOMCONTROLLER_H

class MultiViewZoomController : public Controller
{

   private:

   protected:
      double _zoomfactor;
      bool _reportpos;
      long _utmzone;

   public:

      MultiViewZoomController();

      virtual void install(void);

      virtual void setZoomFactor(double f);
      virtual double getZoomFactor(void);

      void button1Release(int x, int y);
         // Called when Button1 has been released. (x,y) represent
         // location of pointer when event occured.

      void button2Release(int x, int y);
         // Called when Button2 has been released. (x,y) represent
         // location of pointer when event occured.
 
      void button3Release(int x, int y);
         // Called when Button3 has been released. (x,y) represent
         // location of pointer when event occured.

      void pointerMotion(int x, int y);
         // Called when pointer moves. Pointer is now at
         // Device coordinates (x,y).

      void button1Motion(int x, int y);
         // Called when pointer moves and Button1 is depressed.

      void button2Motion(int x, int y);
         // Called when pointer moves and Button2 is depressed.

      void button3Motion(int x, int y);
         // Called when pointer moves and Button3 is depressed.

      void pointerEnter(int x, int y);
         // Called when pointer enters input area. (x,y) location
         // pointer entered.

      void pointerLeave(int x, int y);
         // Called when pointer leaves input area. (x,y) location
         // pointer left.

      void keyPressed(char* stringrep, int x, int y);

      void setUTMZone(long zone);

};

#endif // MULTIVIEWZOOMCONTROLLER_H
