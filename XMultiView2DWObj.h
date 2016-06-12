#ifndef XMULTIVIEW2DWOBJ_H
#define XMULTIVIEW2DWOBJ_H

#include "mvc/XMultiView2D.h"
#include "WObj/WObj.h"

class XMultiView2DWObj : public WObj
{
   private:

   protected:
      XMultiView2D* _myview;
      bool _expose_firsttime;

      static void exposeWrapper(Widget w,
                                XtPointer clientdata,
                                XtPointer calldata);
         // Will be hooked the the expose event callback to capture the
         // window id and connect the XMultiView2D obj to the drawing area widget.

      void exposeCallback(Widget w, XtPointer calldata);

   public:

      XMultiView2DWObj(char* name, Widget parent);

      virtual bool setPoints(double x[], double y[], int numpoints,
                             int num_across, double radius);
         // x[],y[] are coordinates for a list of points. The DisplayDevice
         // will be split into numpoints view ports; each centered on a point.
         // XMultiView2D will attempt to place num_across viewports across
         // the display device. If num_across does not divide numpoints evenly,
         // the remaining points will be equally spaced on the last row of
         // viewports. 'radius' is in WorldSpace coordinates and specifies a
         // minimum area around a point to be displayed. More area may
         // be shown, but it is guaranteed that the minimum area will 
         // be visible.

      virtual XView2D* getView(void);
      virtual Controller* getController(void);
};

#endif // XMULTIVIEW2DWOBJ_H
