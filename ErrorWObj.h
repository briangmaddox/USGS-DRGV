#ifndef ERRORWOBJ_H
#define ERRORWOBJ_H

#include "WObj/WObj.h"

class ErrorWObj : public WObj
{
   private:

   protected:

   public:

      // Displays a message in an Motif Error Dialog.
      // Popup and popdown by using XtManageChild and XtUnmanageChild on
      // the baseWidget (getWidget()).

      ErrorWObj(char* name, Widget parent);
      void setMessage(char* message);
};

#endif // ERRORWOBJ_H