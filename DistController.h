#ifndef DISTCONTROLLER_H
#define DISTCONTROLLER_H

#include "mvc/Controller.h"
#include "DistDialogWObj.h"
#include "DRGVStuffWObj.h"

class DistController : public Controller
{

   private:

   protected:
      bool _firstpoint;
      double _p1x,_p1y;
      DistDialogWObj* _distdialog;
      bool _reportpos;
      DRGVStuffWObj* _stuff;
      long _utmzone;

   public:

      DistController(DistDialogWObj *ddwobj,
                     DRGVStuffWObj* _stuff);

      void install(void);

      void button1Release(int x, int y);
      void pointerMotion(int x, int y);
      void setUTMZone(long zone);      
      void reportPosition(bool flag);
};

#endif //DISTCONTROLLER_H
