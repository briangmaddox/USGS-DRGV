
#include <Xm/DrawingA.h>
#include "mvc/MVC.h"
#include "XMultiView2DWObj.h"

XMultiView2DWObj::XMultiView2DWObj(char* name, Widget parent)
                : WObj(name,parent)
{
   _baseWidget = XtVaCreateManagedWidget(name,xmDrawingAreaWidgetClass,
                                         parent,NULL);

   XtAddCallback(_baseWidget,XmNexposeCallback,exposeWrapper,(XtPointer)this);

   _myview = new XMultiView2D();
   (_myview->getProjection())->setMaintainAspectRatio(true);

   _expose_firsttime = true;
}

void XMultiView2DWObj::exposeWrapper(Widget widget,
                                     XtPointer clientdata,
                                     XtPointer calldata)
{
   ((XMultiView2DWObj*) clientdata) -> exposeCallback(widget,calldata);
}

void XMultiView2DWObj::exposeCallback(Widget widget, XtPointer calldata)
{

   XmDrawingAreaCallbackStruct* xda;

   xda = (XmDrawingAreaCallbackStruct*) calldata;

   if (_expose_firsttime)
      {
         _myview -> attach(&theColorManager,widget,xda->window);
         _myview -> setModel(&theModel2D);

         _myview->setSmartRefresh(true);
         _expose_firsttime = false;
      }

}

bool XMultiView2DWObj::setPoints(double x[], double y[], int numpoints,
                                 int num_across, double radius)
{
   if (_myview != NULL)
      _myview -> setPoints(x,y,numpoints,num_across,radius);
}

XView2D* XMultiView2DWObj::getView(void)
{
   return _myview;
}

Controller* XMultiView2DWObj::getController(void)
{
   return _myview->getController();
}