
#include <Xm/DrawingA.h>
#include "mvc/MVC.h"
#include "mvc/ZoomController.h"
#include "XView2DWObj.h"

XView2DWObj::XView2DWObj(char* name, Widget parent)
           : WObj(name,parent)
{
   _baseWidget = XtVaCreateManagedWidget(name,xmDrawingAreaWidgetClass,
                                         parent,NULL);

   XtAddCallback(_baseWidget,XmNexposeCallback,exposeWrapper,(XtPointer) this);

   _myview = new XView2D();
   _myview->setModel(&theModel2D);
   (_myview->getProjection())->setMaintainAspectRatio(true);

   _expose_firsttime = true;
   _tmpcontroller = NULL;
}

void XView2DWObj::exposeWrapper(Widget w, XtPointer clientdata, XtPointer calldata)
{
   ((XView2DWObj*) clientdata)->exposeCallback(w,calldata);
}

void XView2DWObj::exposeCallback(Widget w, XtPointer calldata)
{
   XmDrawingAreaCallbackStruct* xda;
   Projection2D* proj;
   double wx1,wy1,wx2,wy2;

   xda = (XmDrawingAreaCallbackStruct*) calldata;

   if (_expose_firsttime)
      {
         // When we attach the view to the IO devices, it'll change the
         // size of the World Space Window. Let's save it and restore it
         // after it's attached.
         proj = _myview->getProjection();
         proj->getWorldSpaceWin(&wx1,&wy1,&wx2,&wy2);

         _myview -> attach(&theColorManager,w,xda->window);

         proj->setWorldSpaceWin(wx1,wy1,wx2,wy2);

         if (_tmpcontroller != NULL)
            _myview -> setController(_tmpcontroller);

         _myview->setSmartRefresh(true);
         _expose_firsttime = false;
      }
}

void XView2DWObj::disconnectFromModel(void)
{
   _myview->setModel(NULL);
}

XView2D* XView2DWObj::getView(void)
{
   return _myview;
}

bool XView2DWObj::setController(Controller* c)
{
   if (_expose_firsttime) 
      _tmpcontroller = c;  // We haven't attached the View to the input and
                           // output devices yet.
   else
      _myview->setController(c);

   return true;
}

Controller* XView2DWObj::getController(void)
{
   if (_expose_firsttime)
      return _tmpcontroller;
   else
      return _myview->getController();
}

Projection2D* XView2DWObj::getProjection(void)
{
   return _myview->getProjection();
}

void XView2DWObj::pan(double x, double y)
{
   _myview->pan(x,y);
}

void XView2DWObj::zoomFullView(void)
{
   _myview->zoomFullView();
}