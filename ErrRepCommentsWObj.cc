
#include <string.h>
#include "ErrRepCommentsWObj.h"
#include <Xm/Text.h>

void ErrRepCommentsWObj::clearCallback(Widget w, XtPointer calldata)
{
   clear();
}

void ErrRepCommentsWObj::clearWrapper(Widget w, XtPointer clientdata,
                                      XtPointer calldata)
{
   ((ErrRepCommentsWObj*)clientdata)->clearCallback(w,calldata);
}

void ErrRepCommentsWObj::clear(void)
{
   XmTextSetString(_textarea,"");
}   

char* ErrRepCommentsWObj::getText(void)
{

   char* tmpbuf;
   char* thetext;

   // We'll get a copy from the XmText widget, and make *another* copy
   // just so the user can delete it rather than having to do an 
   // XtFree() on it.

   thetext = XmTextGetString(_textarea);
   tmpbuf = new char[strlen(thetext)+1];
   strcpy(tmpbuf,thetext);
   XtFree(thetext);

   return tmpbuf;

}

ErrRepCommentsWObj::ErrRepCommentsWObj(char* name, Widget parent)
                  : NButtonDialogWObj(name,parent)
{

   _textarea = XmCreateScrolledText(getFormWidget(),"textarea",NULL,NULL);
   XtManageChild(_textarea);

   addButton("Clear",(XtPointer)this,clearWrapper);
   addButton("Close",NULL,NULL);

   setDialogContents(_textarea);

}


