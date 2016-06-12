#ifndef ERRREPCOMMENTSWOBJ_H
#define ERRREPCOMMENTSWOBJ_H

#include "WObj/WObj.h"
#include "WObj/NButtonDialogWObj.h"

class ErrRepCommentsWObj : public NButtonDialogWObj
{
   private:

   protected:

      Widget _textarea;

      void clearCallback(Widget w, XtPointer calldata);
      static void clearWrapper(Widget w, XtPointer clientdata,
                               XtPointer calldata);

   public:

      ErrRepCommentsWObj(char* name, Widget parent);

      void clear(void);
         // Deletes any text in the comment dialog.

      char* getText(void);
         // Returns a copy of the text in the comment dialog. The
         // caller must free this copy.

};

#endif // ERRREPCOMMENTSWOBJ_H
