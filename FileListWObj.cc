
#include <Xm/List.h>
#include "FileListWObj.h"

FileListWObj::FileListWObj(char* name, Widget parent)
            : WObj(name,parent)
{
   _baseWidget = XmCreateScrolledList(parent,"list",NULL,0);
   XtVaSetValues(_baseWidget,
                 XmNvisibleItemCount,   5,
                 NULL);
   XtManageChild(_baseWidget);
}

void FileListWObj::setFiles(char** files, int numfiles)
{
   int i;
   XmString tmpstr;

   for (i = 0; i < numfiles; i++)
      {
         tmpstr = XmStringCreateLtoR(files[i],XmFONTLIST_DEFAULT_TAG);
         XmListAddItemUnselected(_baseWidget,tmpstr,0);
         XmStringFree(tmpstr);
      }
}

bool FileListWObj::getSelectedFilePos(int& pos)
{
   int* poslist;
   int poscnt;

   if (XmListGetSelectedPos(_baseWidget, &poslist, &poscnt))
      {
         // Something is selected.
         pos = poslist[0];
         XtFree((char*) poslist);
         return true;
      }
   else
      return false;
}
