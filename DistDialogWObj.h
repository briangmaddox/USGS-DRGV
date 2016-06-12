#ifndef DISTDIALOGWOBJ_H
#define DISTDIALOGWOBJ_H

#include "WObj/NButtonDialogWObj.h"
#include "ErrorReportWObj.h"

class DistDialogWObj : public NButtonDialogWObj
{
   private:

   protected:
      Widget _label;
      Widget _textfield;
      ErrorReportWObj *_errorwobj;
      bool _havefirstpoint,_havebothpoints;
      double _last_x1, _last_y1;
      double _last_x2, _last_y2;
      double _last_dist;
      CallbackWrapper _closeWrapper;
      XtPointer _closethisptr;

      void clearCallback(Widget w, XtPointer calldata);
      void reportCallback(Widget w, XtPointer calldata);
      void addEntryCallback(Widget w, XtPointer calldata);
      void closeButtonCallback(Widget w, XtPointer calldata);

      static void clearWrapper(Widget w, XtPointer clientdata,
                               XtPointer calldata);
      static void reportWrapper(Widget w, XtPointer clientdata, 
                                XtPointer calldata);
      static void addEntryWrapper(Widget w, XtPointer clientdata,
                                  XtPointer calldata);
      static void closeButtonWrapper(Widget w, XtPointer clientdata,
                                     XtPointer calldata);

   public:

      DistDialogWObj(char* name, Widget parent,
                     XtPointer thisptr, CallbackWrapper closeWrapper);
         // You can pass a Wrapper to be registered as a callback
         // for the Close button.

      double calcDistance(double x1, double y1, double x2, double y2);
         // Calculates the straightline distance between (x1, y1) and (x2,y2)
         // displays it.

      void setPoint(double x, double y);

      void setFirstPoint(double x1, double y1);
      void setSecondPoint(double x2, double y2);
         // These two methods provide an alterate interface.
         // The advantage here is that the user gets immediate feedback
         // when they select points.

      void setDefaultSampleLabel(char* defsamplbl);

      void initReport(char* name);
};

#endif // DISTDIALOGWOBJ_H
