#ifndef XVIEW2DWOBJ_H
#define XVIEW2DWOBJ_H

#include "WObj/WObj.h"
#include "mvc/XView2D.h"

class XView2DWObj : public WObj
{
   private:

   protected:
      XView2D* _myview;
      Controller* _tmpcontroller;  // If view is not attached to the window
                                   // and widget when setController is called,
                                   // we remember it here and will set it when
                                   // we receive the first expose event.
      bool _expose_firsttime;

      static void exposeWrapper(Widget thewidget,
                                 XtPointer clientdata,
                                 XtPointer call);
         // Will be hooked the the expose event callback to capture the
         // window id and connect the XView2D obj to the drawing area widget.

      void exposeCallback(Widget thewidget, XtPointer call);
      

   public:

      XView2DWObj(char* name, Widget parent);

      virtual void disconnectFromModel(void);
         // Sends message to XView2D to tell it to disconnect from the model
         // it is currently attached to (if any).

      virtual XView2D* getView(void);

      virtual bool setController(Controller* c);
      virtual Controller* getController(void);
      virtual Projection2D* getProjection(void);

      virtual void pan(double x, double y);

      virtual void zoomFullView(void);
};

#endif // XVIEW2DWOBJ_H
