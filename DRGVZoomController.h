
#ifndef DRGVZOOMCONTROLLER_H
#define DRGVZOOMCONTROLLER_H

#include "mvc/ZoomController.h"
#include "mvc/Component2D.h"
#include "DRGVStuffWObj.h"

class DRGVZoomController : public ZoomController
{

   private:

   protected:

      RasterComponent2D* myrascomp;
      long _utmzone;
      DRGVStuffWObj* _stuff;

   public:

      DRGVZoomController(DRGVStuffWObj* stuff);
      DRGVZoomController(RasterComponent2D* rc);

      void setRaster(RasterComponent2D* rc);
      void pointerMotion(int x, int y);
      void keyPressed(char* stringrep, int x, int y);
      void setUTMZone(long zone);
      void reportPosition(bool flag);
};

#endif // DRGVZOOMCONTROLLER_H

