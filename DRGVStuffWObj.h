#ifndef DRGVSTUFFWOBJ_H
#define DRGVSTUFFWOBJ_H

// If you can think of a better name, I'm listening... -- jamie


#include "WObj/WObj.h"

class DRGVStuffWObj : public WObj
{
   private:

   protected:

      Widget _rowcol;
      Widget _cellname;
      Widget _filename;
      Widget _scale;
      Widget _utmzone;
      Widget _datum;
      Widget _geocoords;
      Widget _utmcoords;

      void setLabel(Widget w, char* txt);

   public:

      // Provides a rowcolumn of labels that are used to display 
      // "stuff" for the drg validation system. (Hence the name)

      DRGVStuffWObj(char* name, Widget parent);

      void setCellname(char* cname);
      void setFilename(char* fname);
      void setScale(int scale);
      void setUTMZone(int zone);
      void setDatum(int datum);
      void setGeoCoords(double lng, double lat);
      void setUTMCoords(double x, double y);

};

#endif // DRGVSTUFFWOBJ_H
