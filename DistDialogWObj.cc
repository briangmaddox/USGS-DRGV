#include <stdio.h>
#include <math.h>
#include <Xm/Label.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>
#include <Xm/TextF.h>
#include "DistDialogWObj.h"

void DistDialogWObj::clearCallback(Widget w, XtPointer calldata)
{
   // Clear the first and second points.

   XmString tmpstr;
   char tmpbuf[500];

   sprintf(tmpbuf,"Point 1:\nPoint 2:\nDistance:\nDelta X:\nDelta Y:");
   tmpstr = XmStringCreateLtoR(tmpbuf,XmFONTLIST_DEFAULT_TAG);
   XtVaSetValues(_label,
                 XmNlabelString,    tmpstr,
                 NULL);
   XmStringFree(tmpstr);

   _havefirstpoint = false;
   _havebothpoints = false;
}

void DistDialogWObj::reportCallback(Widget w, XtPointer calldata)
{
   _errorwobj->popUp();
}

void DistDialogWObj::clearWrapper(Widget w, XtPointer clientdata,
                                  XtPointer calldata)
{
   ((DistDialogWObj*)clientdata)->clearCallback(w,calldata);
}

void DistDialogWObj::reportWrapper(Widget w, XtPointer clientdata,
                                   XtPointer calldata)
{
   ((DistDialogWObj*)clientdata)->reportCallback(w,calldata);
}

void DistDialogWObj::addEntryCallback(Widget w, XtPointer calldata)
{
   char* labelstr;

   if (_havebothpoints)
      {
         labelstr = XmTextFieldGetString(_textfield);
         _errorwobj->addEntry(_last_x1,_last_y1,_last_x2,_last_y2,
                             _last_dist,labelstr);
         XtFree(labelstr);
      }
}

void DistDialogWObj::addEntryWrapper(Widget w, XtPointer clientdata,
                                     XtPointer calldata)
{
   ((DistDialogWObj*)clientdata)->addEntryCallback(w,calldata);
}

void DistDialogWObj::closeButtonCallback(Widget w, XtPointer calldata)
{
   // Call the callback the User wanted attached to the close button,
   // then popdown the dialog.

   (_closeWrapper)(w,_closethisptr,calldata);
   this->popDown();
}

void DistDialogWObj::closeButtonWrapper(Widget w, XtPointer clientdata,
                                        XtPointer calldata)
{
   ((DistDialogWObj*)clientdata)->closeButtonCallback(w,calldata);
}

DistDialogWObj::DistDialogWObj(char* name, Widget parent, XtPointer thisptr,
                               CallbackWrapper closeWrapper)
              : NButtonDialogWObj(name,parent)
{
   char tmpbuf[500];
   XmString tmpstr;
   Widget rowcol;
   Widget tmprowcol;
   Widget tmplabel;
 
   // The callback the user wants called when the close button
   // is pressed.
   _closethisptr = thisptr;
   _closeWrapper = closeWrapper;


   // One rowcol to manage everything...
   rowcol = XtVaCreateWidget("rowcol",xmRowColumnWidgetClass,
                             getFormWidget(),NULL);

   // Create the distance display label.
   _label = XtVaCreateManagedWidget("label",xmLabelWidgetClass,
                                    rowcol,
                                    XmNalignment,        XmALIGNMENT_BEGINNING,
                                    NULL);

   // Create the sample label entry area.
   tmprowcol = XtVaCreateWidget("rowcol",xmRowColumnWidgetClass,
                                rowcol,
                                XmNorientation,   XmHORIZONTAL,
                                NULL);
   tmpstr = XmStringCreateLtoR("Sample Label:",XmFONTLIST_DEFAULT_TAG);
   tmplabel = XtVaCreateManagedWidget("label1",xmLabelWidgetClass,tmprowcol,
                                      XmNlabelString,    tmpstr,
                                      NULL);
   XmStringFree(tmpstr);

   _textfield = XtVaCreateManagedWidget("textfield",xmTextFieldWidgetClass,
                                        tmprowcol,
                                        NULL);
   
   XtManageChild(tmprowcol);
   XtManageChild(rowcol);


   addButton("Report",     (XtPointer)this, reportWrapper);
   addButton("AddToReport",(XtPointer)this, addEntryWrapper);
   addButton("Clear",      (XtPointer)this, clearWrapper);
   addButton("Close",      (XtPointer)this, closeButtonWrapper);

   setDialogContents(_label);

   _havefirstpoint = false;
   _havebothpoints = false;

   sprintf(tmpbuf,"Point 1:\nPoint 2:\nDistance:\nDelta X(1->2):\nDelta Y(1->2):");
   tmpstr = XmStringCreateLtoR(tmpbuf,XmFONTLIST_DEFAULT_TAG);
   XtVaSetValues(_label,
                 XmNlabelString,    tmpstr,
                 NULL);
   XmStringFree(tmpstr);

   _errorwobj = new ErrorReportWObj("errorreport",parent);
}

double DistDialogWObj::calcDistance(double x1, double y1, double x2, double y2)
{
   double a,b,dist;

   a = (x1 - x2) * (x1 - x2);
   b = (y1 - y2) * (y1 - y2);
   dist = sqrt(a+b);

   return dist;
}

void DistDialogWObj::setPoint(double x, double y)
{

   if (_havefirstpoint == false)
      setFirstPoint(x,y);
   else
      setSecondPoint(x,y);

}

void DistDialogWObj::setFirstPoint(double x1, double y1)
{
   XmString tmpstr;
   char outbuf[1000];
   char tempbuf[1000];

   _last_x1 = x1;
   _last_y1 = y1;
   sprintf(outbuf,
	"Point 1: (%lf,%lf)\nPoint 2:\nDistance:",x1,y1);
   sprintf(tempbuf, "%s\nDelta X(1->2):\nDelta Y(1->2):", outbuf);

   tmpstr = XmStringCreateLtoR(tempbuf,XmFONTLIST_DEFAULT_TAG);
   XtVaSetValues(_label,XmNlabelString,tmpstr,NULL);
   XmStringFree(tmpstr);

   _havefirstpoint = true;
}

void DistDialogWObj::setSecondPoint(double x2, double y2)
{
   XmString tmpstr;
   char outbuf[1000];
   char tempbuf[1000];

   _last_x2 = x2;
   _last_y2 = y2;
   _last_dist = calcDistance(_last_x1,_last_y1,_last_x2,_last_y2);


   sprintf(outbuf, "Point 1: (%lf,%lf)\nPoint 2: (%lf,%lf)\nDistance:%lf",
           _last_x1,_last_y1,_last_x2,_last_y2,_last_dist);
   sprintf(tempbuf, "%s\nDelta X(1->2): (%lf)\nDelta Y(1->2): (%lf)", outbuf,
		_last_x2 - _last_x1, _last_y2 - _last_y1);
   tmpstr = XmStringCreateLtoR(tempbuf,XmFONTLIST_DEFAULT_TAG);
   XtVaSetValues(_label,
                 XmNlabelString,   tmpstr,
                 NULL);
   XmStringFree(tmpstr);

   _havefirstpoint = false;
   _havebothpoints = true;
}

void DistDialogWObj::setDefaultSampleLabel(char* defsamplbl)
{
   XmTextFieldSetString(_textfield,defsamplbl);
}

void DistDialogWObj::initReport(char* name)
{
   if (_errorwobj != NULL)
      {
         _errorwobj->initReport();
         _errorwobj->setDatasetName(name);
      }
}
