#include <stdio.h>
#include <Xm/Xm.h>
#include <Xm/RowColumn.h>
#include <Xm/Label.h>
#include <Xm/Frame.h>
#include <Xm/Separator.h>
#include "drg/usgsmaptypes.h"
#include "drg/coordutils.h"
#include "DRGVStuffWObj.h"

void DRGVStuffWObj::setLabel(Widget w, char* txt)
{
   XmString xmstr;

   xmstr = XmStringCreateLtoR(txt,XmFONTLIST_DEFAULT_TAG);
   XtVaSetValues(w,
                 XmNlabelString,   xmstr,
                 NULL);
   XmStringFree(xmstr);
}

DRGVStuffWObj::DRGVStuffWObj(char* name, Widget parent)
             : WObj(name,parent)
{
   Widget tmpwidget;

   _baseWidget = XtVaCreateManagedWidget(name,xmFrameWidgetClass,
                                         parent,NULL);

   _rowcol   = XtVaCreateManagedWidget(name,xmRowColumnWidgetClass,
                    _baseWidget,
                    XmNorientation,   XmVERTICAL,
                    XmNisAligned,     False,
                    NULL);

   _cellname = XtVaCreateManagedWidget("cellname",xmLabelWidgetClass,
                   _rowcol,
                   XmNalignment,   XmALIGNMENT_BEGINNING,
                   NULL);

   _filename = XtVaCreateManagedWidget("filename",xmLabelWidgetClass,
                   _rowcol,
                   XmNalignment,   XmALIGNMENT_BEGINNING,
                   NULL);

   _scale    = XtVaCreateManagedWidget("scale",xmLabelWidgetClass,
                   _rowcol,
                   XmNalignment,   XmALIGNMENT_BEGINNING,
                   NULL);

   _utmzone  = XtVaCreateManagedWidget("utmzone",xmLabelWidgetClass,
                   _rowcol,
                   XmNalignment,   XmALIGNMENT_BEGINNING,
                   NULL);

   _datum    = XtVaCreateManagedWidget("datum",xmLabelWidgetClass,
                   _rowcol,
                   XmNalignment,   XmALIGNMENT_BEGINNING,
                   NULL);

   tmpwidget = XtVaCreateManagedWidget("separator1",xmSeparatorWidgetClass,
                                       _rowcol,
                                       XmNorientation, XmHORIZONTAL,
                                       NULL);
   tmpwidget = XtVaCreateManagedWidget("coordtitle",xmLabelWidgetClass,
                                       _rowcol,
                                       XmNalignment,XmALIGNMENT_CENTER,
                                       NULL);
   setLabel(tmpwidget,"Cursor Coordinates");

   _geocoords = XtVaCreateManagedWidget("geocoords",xmLabelWidgetClass,
                   _rowcol,
                   XmNalignment,   XmALIGNMENT_BEGINNING,
                   NULL);

   _utmcoords = XtVaCreateManagedWidget("utmcoords",xmLabelWidgetClass,
                   _rowcol,
                   XmNalignment,   XmALIGNMENT_BEGINNING,
                   NULL);

   setCellname("none");
   setFilename("none");
   setScale(0);
   setUTMZone(0);
   setDatum(DATUM_NAD27);
   setGeoCoords(0.0,0.0);
   setUTMCoords(0.0,0.0);
}

void DRGVStuffWObj::setCellname(char* cname)
{
   char tmpbuf[1000];

   sprintf(tmpbuf,"Cell name: %s",cname);
   setLabel(_cellname,tmpbuf);
}

void DRGVStuffWObj::setFilename(char* fname)
{
   char tmpbuf[1000];

   sprintf(tmpbuf,"File name: %s",fname);
   setLabel(_filename,tmpbuf);
}

void DRGVStuffWObj::setScale(int scale)
{
   char tmpbuf[1000];

   sprintf(tmpbuf,"Scale: 1:%d",scale);
   setLabel(_scale,tmpbuf);
}

void DRGVStuffWObj::setUTMZone(int zone)
{
   char tmpbuf[1000];

   sprintf(tmpbuf,"UTM Zone: %d",zone);
   setLabel(_utmzone,tmpbuf);
}

void DRGVStuffWObj::setDatum(int datum)
{
   char tmpbuf[1000];

   strcpy(tmpbuf,"DRGVStuffWOBj: Error in datum");

   switch(datum)
      {
         case DATUM_NAD27: sprintf(tmpbuf,"Datum: NAD27"); break;
         case DATUM_NAD83: sprintf(tmpbuf,"Datum: NAD83"); break;
      }

   setLabel(_datum,tmpbuf);

}

void DRGVStuffWObj::setGeoCoords(double lng, double lat)
{
   char tmpbuf[1000];
   double d1,m1,s1;
   double d2,m2,s2;

   dec2dms(lng,d1,m1,s1);
   dec2dms(lat,d2,m2,s2);
   sprintf(tmpbuf,"Geographic: %3.0lf %3.0lf %6.3lf  %3.0lf %3.0lf %6.3lf",d1,m1,s1,d2,m2,s2);
   setLabel(_geocoords,tmpbuf);
}

void DRGVStuffWObj::setUTMCoords(double x, double y)
{
   char tmpbuf[1000];

   sprintf(tmpbuf,"UTM: %0.4lf %0.4lf",x,y);
   setLabel(_utmcoords,tmpbuf);
}
