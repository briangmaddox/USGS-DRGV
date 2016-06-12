#ifndef FILELISTWOBJ_H
#define FILELISTWOBJ_H

#include "WObj/WObj.h"

class FileListWObj : public WObj
{
   private:

   protected:

   public:

      FileListWObj(char* name, Widget parent);
    
      void setFiles(char** files, int numfiles);

      bool getSelectedFilePos(int& pos);
         // if returns true, pos is position of selected item in the list.
         // if false, no items selected.
};

#endif // FILELISTWOBJ_H
