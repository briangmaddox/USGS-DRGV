
#include <Xm/MessageB.h>
#include "ErrorWObj.h"

ErrorWObj::ErrorWObj(char* name, Widget parent) : WObj(name,parent)
{
   _baseWidget = XmCreateErrorDialog(parent,name,NULL,0);
   XtVaSetValues(_baseWidget,
                 XmNdialogStyle,  XmDIALOG_PRIMARY_APPLICATION_MODAL,
                 NULL);
   XtUnmanageChild(XmMessageBoxGetChild(_baseWidget,XmDIALOG_CANCEL_BUTTON));
   XtUnmanageChild(XmMessageBoxGetChild(_baseWidget,XmDIALOG_HELP_BUTTON));
}

void ErrorWObj::setMessage(char* message)
{
   XmString tmpstr;

   tmpstr = XmStringCreateLtoR(message,XmFONTLIST_DEFAULT_TAG);
   XtVaSetValues(_baseWidget,
                 XmNmessageString,     tmpstr,
                 NULL);
   XmStringFree(tmpstr);
}