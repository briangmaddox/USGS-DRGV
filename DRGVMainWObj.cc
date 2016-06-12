#include <stdio.h>
#include <string.h>
#include <Xm/Xm.h>
#include <Xm/MainW.h>
#include <Xm/RowColumn.h>
#include <Xm/DrawingA.h>
#include <Xm/FileSB.h>
#include <Xm/Label.h>
#include <Xm/Scale.h>
#include "drg/usgsmaptypes.h"
#include "drg/controlpts.h"
#include "drg/ARCWorldFile.h"
#include "drg/MetFile.h"
#include "mvc/MVC.h"
#include "mvc/Component2D.h"
#include "DRGVMainWObj.h"
#include "DRGVZoomController.h"
#include "MultiViewZoomController.h"
#include "XMultiView2DWObj.h"
#include "DistController.h"
#include "DRGVControlPoints.h"

extern XMultiView2DWObj *ctrlptwin;
extern PopupWObj *mainpopup;

void DRGVMainWObj::calcControlPoints(void)
{
   int i;
   int maptype;
   int num_across, num_down;
   double radius;
   double width, height;
   double *xcoord, *ycoord;


   if (_mapscale == 24000)   { maptype = USGS24000; radius = 100.0; }
   if (_mapscale == 25000)   { maptype = USGS25000; radius = 100.0; }
   if (_mapscale == 63360)   { maptype = USGS63360; radius = 320.0; }
   if (_mapscale == 100000)  { maptype = USGS100000; radius = 500.0; }
   if (_mapscale == 250000)  { maptype = USGS250000; radius = 1200.0; }

   if (_calculated_ctrlpts != NULL)
      {
         // Delete the old control point symbols
         for (i = 0; i < _num_ctrlpts; i++)
            {
               theModel2D.deleteComponent(_calculated_ctrlpts[i]);
               delete _calculated_ctrlpts[i];
            }
         delete [] _calculated_ctrlpts;
      }

   //controlpointparams(maptype,num_across,num_down,width,height);
   //   New getControlPoints() in DRGVControlPoints.{h,cc}
   getControlPointParams(maptype,_westlong,_eastlong,_northlat,_southlat,
                         num_across,num_down,width,height);
   _num_ctrlpts = num_across * num_down;
   xcoord = new double[_num_ctrlpts];
   ycoord = new double[_num_ctrlpts];
   calcGroundCoords(maptype,_utmzone,_datum,_westlong,-1.0*_eastlong,
                    _northlat, _southlat, xcoord,ycoord,_num_ctrlpts);

   ctrlptwin->setPoints(xcoord,ycoord,_num_ctrlpts,num_across,radius);
   ((MultiViewZoomController*)ctrlptwin->getController())->
           setUTMZone(_utmzone);

   _drgvzc->setUTMZone(_utmzone);
   _distc->setUTMZone(_utmzone);

   _calculated_ctrlpts = new PlusSymbolComponent2D*[_num_ctrlpts];
   for (i = 0; i < _num_ctrlpts; i++)
      {
         _calculated_ctrlpts[i] = 
            new PlusSymbolComponent2D(xcoord[i],ycoord[i],
                                      theColorManager.getColorCookie("orange"));
         _calculated_ctrlpts[i]->setLineWidth(2);
         theModel2D.addComponent(_calculated_ctrlpts[i]);
      }

   delete [] xcoord;
   delete [] ycoord;
}

DRGVMainWObj::DRGVMainWObj(char* name, Widget parent)
            : WObj(name,parent)
{
   int i;
   WObjClientdata *wcd1, *wcd2;
   Widget tmpwidget, tmpwidget1, tmpwidget2;

   app_image = NULL;
   app_raster = NULL;
   _calculated_ctrlpts = NULL;
   _num_ctrlpts = 0;
   _cur_ctrlpt = 0;
   _inputfile_list = NULL;
   _num_inputfiles = 0;
   _cur_inputfile = -1;
   _datum = DATUM_NAD27;
   _utmzone = 0;
   _eastlong = 0.0;
   _southlat = 0.0;
   _westlong = 0.0;
   _northlat = 0.0;
   _show_scale_view = true;

   _mainwindow = XtVaCreateManagedWidget(name,
                                         xmMainWindowWidgetClass,
                                         parent,NULL);
   _baseWidget = _mainwindow;

   // ErrorWObj
   _errorwobj = new ErrorWObj("error",parent);

   // FileSelectionDialog
   _filedialog = XmCreateFileSelectionDialog(parent,"filedialog",NULL,0);
   XtAddCallback(_filedialog,XmNokCallback,fileDialogOKWrapper,(XtPointer)this);
   XtAddCallback(_filedialog,XmNcancelCallback,fileDialogCancelWrapper,
                 (XtPointer)this);

   // DistDialogWObj
   _distdialog = new DistDialogWObj("distdialog",_mainwindow,
                                    (XtPointer) this, setZoomWrapper);

   // FileListWObj
   _filelistpopup = new NButtonDialogWObj("filepopup",parent);
   _filelist = new FileListWObj("filelist",_filelistpopup->getFormWidget());
   _filelistpopup->addButton("Goto",(XtPointer) this, gotoWrapper);
   _filelistpopup->addButton("Close",NULL,NULL);
   _filelistpopup->setDialogContents(XtParent(_filelist->getWidget()));

   _menubar = XmCreateMenuBar(_mainwindow,"menubar",NULL,0);

   _filemenu = new MenuWObj("filemenu",_menubar,"File");
   _filemenu->addButtonItem("Load",(XtPointer) this , loadWrapper);
   _filemenu->addSeparatorItem();
   _filemenu->addButtonItem("List",(XtPointer) this, listWrapper);
   _filemenu->addButtonItem("Next",(XtPointer) this, nextWrapper);
   _filemenu->addButtonItem("Previous",(XtPointer) this, prevWrapper);
   _filemenu->addSeparatorItem();
   _filemenu->addButtonItem("Quit",(XtPointer) this, quitWrapper);

   _viewmenu = new MenuWObj("viewmenu",_menubar,"View");
   _viewmenu->addButtonItem("CurrentPoint",(XtPointer) this, curptWrapper);
   _viewmenu->addButtonItem("NextPoint",(XtPointer) this, nextptWrapper);
   _viewmenu->addButtonItem("PrevPoint",(XtPointer) this, prevptWrapper);
   _viewmenu->addSeparatorItem();
   _viewmenu->addButtonItem("FullView",(XtPointer)this, fullviewWrapper);
//   _viewmenu->addButtonItem("ChangeZoom",(XtPointer) this , changeZoomWrapper);
   _viewmenu->addSeparatorItem();
   _viewmenu->addToggleButtonItem("ShowCursorPos",(XtPointer)this,
                                  cursorPosWrapper);


   _toolmenu = new MenuWObj("toolmenu",_menubar,"Tools");
   _toolmenu->addButtonItem("MeasureDist",(XtPointer) this, measureDistWrapper);
   _toolmenu->addButtonItem("ZoomPan",(XtPointer) this, zoomPanWrapper);
   
   _datummenu = new MenuWObj("datummenu",_menubar,"Datum");
   wcd1 = new WObjClientdata;
   wcd1->thisptr = (XtPointer) this;
   wcd1->actual_clientdata = (XtPointer) DATUM_NAD27;
   _datummenu->addButtonItem("NAD27", (XtPointer) wcd1, datumWrapper);
   wcd2 = new WObjClientdata;
   wcd2->thisptr = (XtPointer) this;
   wcd2->actual_clientdata = (XtPointer) DATUM_NAD83;
   _datummenu->addButtonItem("NAD83", (XtPointer) wcd2, datumWrapper);

   XtManageChild(_menubar);

   _stuff = new DRGVStuffWObj("stuff",_mainwindow);

   _xview2dwobj = new XView2DWObj("display",_mainwindow);
   _drgvzc = new DRGVZoomController(_stuff);
   _distc = new DistController(_distdialog,_stuff);
   _xview2dwobj->setController(_drgvzc);

   XmMainWindowSetAreas(_mainwindow,_menubar,_stuff->getWidget(),
                        NULL,NULL,
                        _xview2dwobj->getWidget());

   // The two zoomed window views
   _mapnamewobj = new PopupWObj("mapname",parent);
   _mapnameview = new XView2DWObj("mapnameview",_mapnamewobj->getWidget());
   _mapotherwobj = new PopupWObj("mapother",parent);
   _mapotherview = new XView2DWObj("mapotherview",_mapotherwobj->getWidget());
   _mapscalewobj = new PopupWObj("mapscale",parent);
   _mapscaleview = new XView2DWObj("mapscaleview",_mapscalewobj->getWidget());
   _namectrl = new ZoomController;
   _mapnameview->setController(_namectrl);
   _otherctrl = new ZoomController;
   _mapotherview->setController(_otherctrl);
   _scalectrl = new ZoomController;
   _mapscaleview->setController(_scalectrl);
   // Change Zoom Factor Dialog
   _zoomslider = new NButtonDialogWObj("zoomslider",parent);
   tmpwidget = XtVaCreateWidget("rowcol",xmRowColumnWidgetClass,
                                _zoomslider->getFormWidget(),NULL);
   tmpwidget1 = XtVaCreateManagedWidget("label",xmLabelWidgetClass,
                                 tmpwidget,NULL);
   tmpwidget2 = XtVaCreateManagedWidget("scale",xmScaleWidgetClass,
                                 tmpwidget,
                                 XmNorientation, XmHORIZONTAL,
                                 NULL);
   XtManageChild(tmpwidget);
   _zoomslider->addButton("Close",NULL,NULL);
   _zoomslider->setDialogContents(tmpwidget);

//******************************
   _mapnamewobj->popUp();
   _mapotherwobj->popUp();
   _mapscalewobj->popUp();
//******************************
}
                      
void DRGVMainWObj::loadCallback(Widget w, XtPointer calldata)
{
   XtManageChild(_filedialog);
}
 
void DRGVMainWObj::loadWrapper(Widget w, XtPointer clientdata, XtPointer calldata)
{
   ((DRGVMainWObj*)clientdata)->loadCallback(w,calldata);
}

void DRGVMainWObj::fileDialogOKCallback(Widget w, XtPointer calldata)
{
   XmFileSelectionBoxCallbackStruct* xfsb;
   char* tmpstr;

   xfsb = (XmFileSelectionBoxCallbackStruct*) calldata;
   XmStringGetLtoR(xfsb->value,XmFONTLIST_DEFAULT_TAG,&tmpstr);

   loadFile(tmpstr);
   theModel2D.update();
   XtFree(tmpstr);

   XtUnmanageChild(_filedialog);
}
 
void DRGVMainWObj::fileDialogOKWrapper(Widget w, XtPointer clientdata, 
                                       XtPointer calldata)
{
   ((DRGVMainWObj*)clientdata)->fileDialogOKCallback(w,calldata);
}

void DRGVMainWObj::fileDialogCancelCallback(Widget w, XtPointer calldata)
{
   XtUnmanageChild(_filedialog);
}
 
void DRGVMainWObj::fileDialogCancelWrapper(Widget w, XtPointer clientdata,
                                           XtPointer calldata)
{
   ((DRGVMainWObj*)clientdata)->fileDialogCancelCallback(w,calldata);
}

void DRGVMainWObj::listCallback(Widget w, XtPointer calldata)
{
   _filelistpopup->popUp();
}
 
void DRGVMainWObj::listWrapper(Widget w, XtPointer clientdata, XtPointer calldata)
{
   ((DRGVMainWObj*)clientdata)->listCallback(w,calldata);
}

void DRGVMainWObj::nextCallback(Widget w, XtPointer calldata)
{
   int tmpfile;

   if (_num_inputfiles == 0)
      return;

   tmpfile = _cur_inputfile + 1;
   if (tmpfile >= _num_inputfiles)
      tmpfile = 0;

   if (loadFile(_inputfile_list[tmpfile]) == 1)
      {
         _cur_inputfile = tmpfile;
         theModel2D.update();
      }
}
 
void DRGVMainWObj::nextWrapper(Widget w, XtPointer clientdata, XtPointer calldata)
{
   ((DRGVMainWObj*)clientdata)->nextCallback(w,calldata);
}

void DRGVMainWObj::prevCallback(Widget w, XtPointer calldata)
{
   int tmpfile;

   if (_num_inputfiles == 0)
      return;

   tmpfile = _cur_inputfile - 1;
   if (tmpfile < 0)
      tmpfile = _num_inputfiles - 1;

   if (loadFile(_inputfile_list[tmpfile]) == 1)
      {
         _cur_inputfile = tmpfile;
         theModel2D.update();
      }
}
 
void DRGVMainWObj::prevWrapper(Widget w, XtPointer clientdata, XtPointer calldata)
{
   ((DRGVMainWObj*)clientdata)->prevCallback(w,calldata);
}


void DRGVMainWObj::gotoCallback(Widget w, XtPointer calldata)
{
   int pos;

   if (_filelist->getSelectedFilePos(pos))
      {
         _cur_inputfile = pos - 1;
         loadFile(_inputfile_list[_cur_inputfile]);
         theModel2D.update();
      }
}
 
void DRGVMainWObj::gotoWrapper(Widget w, XtPointer clientdata, XtPointer calldata)
{
   ((DRGVMainWObj*)clientdata)->gotoCallback(w,calldata);
}



void DRGVMainWObj::quitCallback(Widget w, XtPointer calldata)
{
   exit(0);
}
 
void DRGVMainWObj::quitWrapper(Widget w, XtPointer clientdata, XtPointer calldata)
{
   ((DRGVMainWObj*)clientdata)->quitCallback(w,calldata);
}

void DRGVMainWObj::fullviewCallback(Widget w, XtPointer calldata)
{
   _xview2dwobj->zoomFullView();
}
 
void DRGVMainWObj::fullviewWrapper(Widget w, XtPointer clientdata,
                                   XtPointer calldata)
{
   ((DRGVMainWObj*)clientdata)->fullviewCallback(w,calldata);
}

void DRGVMainWObj::changeZoomCallback(Widget w, XtPointer calldata)
{
   _zoomslider->popUp();
}
 
void DRGVMainWObj::changeZoomWrapper(Widget w, XtPointer clientdata,
                                     XtPointer calldata)
{
   ((DRGVMainWObj*)clientdata)->changeZoomCallback(w,calldata);
}



void DRGVMainWObj::cursorPosCallback(Widget w, XtPointer calldata)
{
   XmToggleButtonCallbackStruct* xmtb;

   xmtb = (XmToggleButtonCallbackStruct*) calldata;

   if ((xmtb->reason == XmCR_VALUE_CHANGED) &&
       (_drgvzc != NULL))
      {
         if (xmtb->set == True)
            {
               _drgvzc->reportPosition(true);
               _distc->reportPosition(true);
            }
         else
            {
               _drgvzc->reportPosition(false);
               _distc->reportPosition(false);
            }
      }
}
 
void DRGVMainWObj::cursorPosWrapper(Widget w, XtPointer clientdata,
                                    XtPointer calldata)
{
   ((DRGVMainWObj*)clientdata)->cursorPosCallback(w,calldata);
}

void DRGVMainWObj::curptCallback(Widget w, XtPointer calldata)
{
   char tmpbuf[1000];

   // Center the view on the current control point.

   if (_num_ctrlpts == 0)
      return;

   sprintf(tmpbuf,"CtrlPt %d",_cur_ctrlpt+1);
   _distdialog->setDefaultSampleLabel(tmpbuf);

   _xview2dwobj->pan(_calculated_ctrlpts[_cur_ctrlpt]->getX(),
                     _calculated_ctrlpts[_cur_ctrlpt]->getY());
}

void DRGVMainWObj::curptWrapper(Widget w, XtPointer clientdata,
                                 XtPointer calldata)
{
   ((DRGVMainWObj*)clientdata)->curptCallback(w,calldata);
}

void DRGVMainWObj::nextptCallback(Widget w, XtPointer calldata)
{
   char tmpbuf[1000];

   // Center the view on the next control point.

   if (_num_ctrlpts == 0)
      return;

   _cur_ctrlpt = _cur_ctrlpt + 1;
   if (_cur_ctrlpt >= _num_ctrlpts)
      _cur_ctrlpt = 0;

   sprintf(tmpbuf,"CtrlPt %d",_cur_ctrlpt+1);
   _distdialog->setDefaultSampleLabel(tmpbuf);

   _xview2dwobj->pan(_calculated_ctrlpts[_cur_ctrlpt]->getX(),
                     _calculated_ctrlpts[_cur_ctrlpt]->getY());
}

void DRGVMainWObj::nextptWrapper(Widget w, XtPointer clientdata,
                                 XtPointer calldata)
{
   ((DRGVMainWObj*)clientdata)->nextptCallback(w,calldata);
}

void DRGVMainWObj::prevptCallback(Widget w, XtPointer calldata)
{
   char tmpbuf[1000];

   // Center the view on the previous control point.

   if (_num_ctrlpts == 0)
      return;

   _cur_ctrlpt = _cur_ctrlpt - 1;
   if (_cur_ctrlpt < 0)
      _cur_ctrlpt = _num_ctrlpts - 1;

   sprintf(tmpbuf,"CtrlPt %d",_cur_ctrlpt+1);
   _distdialog->setDefaultSampleLabel(tmpbuf);

   _xview2dwobj->pan(_calculated_ctrlpts[_cur_ctrlpt]->getX(),
                     _calculated_ctrlpts[_cur_ctrlpt]->getY());
}

void DRGVMainWObj::prevptWrapper(Widget w, XtPointer clientdata,
                                 XtPointer calldata)
{
   ((DRGVMainWObj*)clientdata)->prevptCallback(w,calldata);
}

void DRGVMainWObj::measureDistCallback(Widget w, XtPointer calldata)
{
   _distdialog->popUp();
   _xview2dwobj->setController(_distc);
}
 
void DRGVMainWObj::measureDistWrapper(Widget w, XtPointer clientdata, 
                                      XtPointer calldata)
{
   ((DRGVMainWObj*)clientdata)->measureDistCallback(w,calldata);
}

void DRGVMainWObj::zoomPanCallback(Widget w, XtPointer calldata)
{
   _xview2dwobj->setController(_drgvzc);
}
 
void DRGVMainWObj::zoomPanWrapper(Widget w, XtPointer clientdata, 
                                      XtPointer calldata)
{
   ((DRGVMainWObj*)clientdata)->zoomPanCallback(w,calldata);
}

void DRGVMainWObj::datumCallback(Widget w, XtPointer clientdata, XtPointer calldata)
{
   _datum = (int) clientdata;
   _stuff->setDatum(_datum);
   calcControlPoints();
   theModel2D.update();
}
 
void DRGVMainWObj::datumWrapper(Widget w, XtPointer clientdata, 
                                      XtPointer calldata)
{
   // In this case, clientdata is of type WObjClientdata.

   XtPointer thisptr;
   XtPointer actual_clientdata;

   thisptr = ((WObjClientdata*)clientdata)->thisptr;
   actual_clientdata = ((WObjClientdata*) clientdata)->actual_clientdata;

   ((DRGVMainWObj*)thisptr)->datumCallback(w,actual_clientdata,calldata);

}

void DRGVMainWObj::setZoomCallback(Widget w, XtPointer calldata)
{
   _xview2dwobj->setController(_drgvzc);
}

void DRGVMainWObj::setZoomWrapper(Widget w, XtPointer clientdata,
                                  XtPointer calldata)
{
   ((DRGVMainWObj*)clientdata)->setZoomCallback(w,calldata);
}


void DRGVMainWObj::init(char** argv, int argc)
{
   int i;

   // Assume that everything left in argv is filenames...
   if (argc > 1)
      {
         _num_inputfiles = argc - 1;
         _inputfile_list = new char*[_num_inputfiles];
         for (i = 0; i < _num_inputfiles; i++)
            {
               _inputfile_list[i] = new char[strlen(argv[i+1]) + 1];
               strcpy(_inputfile_list[i],argv[i+1]);
            }
         _cur_inputfile = 0;
         loadFile(_inputfile_list[_cur_inputfile]);
      }

   _filelist->setFiles(_inputfile_list,_num_inputfiles);
   
}

int DRGVMainWObj::loadFile(char* filename)
{
   RGBImage* inputimage;
   double width,height;
   int raster_width,raster_height;
   double input_xres,input_yres;
   double input_wx,input_wy;
   double x1,y1,x2,y2;
   double wx1,wy1,wx2,wy2;
   double dx,dy;
   int input_wx_offset;
   int input_wy_offset;
   RasterComponent2D* inputraster;
   char tmp[1000],tmp1[1000],tmp2[1000];
   char* awf_filename;
   ARCWorldFile awf;
   char* met_filename;
   MetFile met;
   int count,i,j;
   char outbuf[1000];
   Projection2D *tmpproj;
   char map_type;
   double zx1,zy1,zf1,zx2,zy2,zf2,zx3,zy3,zf3;


   inputimage = new RGBImage(filename);
   if (inputimage->bad())
      {
         _errorwobj->setMessage("File not found or not in recognized format.");
         XtManageChild(_errorwobj->getWidget());
         delete inputimage;
         return 0;
      }

   if (inputimage != NULL)
      {
         strcpy(tmp1,filename);
         awf_filename = strtok(tmp1,".");
         strcat(awf_filename,".tfw");

         strcpy(tmp2,filename);
         met_filename = strtok(tmp2,".");
         strcat(met_filename,".met");

         // Metadata

         met.readFromFile(met_filename);
         if (met.readFromFile(met_filename))
            {
               _mapscale = met.getMapScale();
               _utmzone = met.getUTMZone();
               _eastlong = met.getEastLong();
               _westlong = met.getWestLong();
               _southlat = met.getSouthLat();
               _northlat = met.getNorthLat();
              
            }
         else
            {
               _errorwobj->setMessage("DRG metadata file not found.\n");
               XtManageChild(_errorwobj->getWidget());
              return 0;
            }



         // Delete old image.
         if (app_image != NULL)
            {
               theModel2D.deleteComponent(app_raster);
               delete app_raster;
               app_raster = NULL;

               delete app_image;
               app_image = NULL;
            }
 

         raster_width = inputimage -> getWidth();
         raster_height = inputimage -> getHeight();

         if (awf.read(awf_filename))
            {
               width = awf.getPixelResolutionX() * (raster_width + 1);
               height = awf.getPixelResolutionY() * (raster_height + 1);
               x1 = awf.getWorldCoordX(0,raster_height - 1);
               y1 = awf.getWorldCoordY(0,raster_height - 1);
               x2 = awf.getWorldCoordX(raster_width - 1,0);
               y2 = awf.getWorldCoordY(raster_width - 1,0);
               input_xres = awf.getPixelResolutionX();
               input_yres = awf.getPixelResolutionY();
            }
         else
            {
               width = raster_width;
               height = raster_height;
               x1 = 0.0;
               y1 = 0.0;
               x2 = width - 1;
               y2 = height - 1;
               input_xres = 1.0;
               input_yres = 1.0;
            }

         dx = width * 0.1;   // Put a 10% border around image.
         dy = height * 0.1;
         wx1 = x1 - dx; 
         wy1 = y1 - dy;
         wx2 = x2 + dx;
         wy2 = y2 + dy;
         theModel2D.setEveryFullView(wx1,wy1,wx2,wy2);
         theModel2D.zoomEveryFullView();
         if (_mapscale == 25000)
         {
            if ((_eastlong - _westlong) > 0.125)
            {
               zx1 = wx1 + (wx2-wx1)*0.86;
               zy1 = wy2 - (wy2-wy1)*0.12;
               zx2 = wx1 + (wx2-wx1)*0.86;
               zy2 = wy1 + (wy2-wy1)*0.58;
               zx3 = zx1;
               zy3 = zy1;
               zf1 = 12.0;
               zf2 = 12.0; 
               zf3 = 12.0; 
               _show_scale_view = false;
            }
            else
            {
               zx1 = wx1 + (wx2-wx1)*0.8;
               zy1 = wy1 + (wy2-wy1)*0.1;
               zx2 = wx1 + (wx2-wx1)*0.2;
               zy2 = wy1 + (wy2-wy1)*0.1;
               zx3 = wx1 + (wx2-wx1)*0.5;
               zy3 = wy1 + (wy2-wy1)*0.15;
               zf1 = 14.0;
               zf2 = 14.0; 
               zf3 = 16.0; 
               _show_scale_view = true;
            }

         }
         else if (_mapscale == 24000)
         {
            if ((_eastlong - _westlong) > 0.125)
            {
               zx1 = wx1 + (wx2-wx1)*0.86;
               zy1 = wy2 - (wy2-wy1)*0.12;
               zx2 = wx1 + (wx2-wx1)*0.86;
               zy2 = wy1 + (wy2-wy1)*0.58;
               zx3 = zx1;
               zy3 = zy1;
               zf1 = 12.0;
               zf2 = 12.0; 
               zf3 = 12.0; 
               _show_scale_view = false;
            }
            else
            {
               zx1 = wx1 + (wx2-wx1)*0.8;
               zy1 = wy1 + (wy2-wy1)*0.1;
               zx2 = wx1 + (wx2-wx1)*0.2;
               zy2 = wy1 + (wy2-wy1)*0.1;
               zx3 = wx1 + (wx2-wx1)*0.5;
               zy3 = wy1 + (wy2-wy1)*0.15;
               zf1 = 14.0;
               zf2 = 14.0; 
               zf3 = 16.0; 
               _show_scale_view = true;
            }
         }
         else if (_mapscale == 63360)
         {
            zx1 = wx1 + (wx2-wx1)*0.78;
            zy1 = wy2 - (wy2-wy1)*0.12;
            zx2 = wx1 + (wx2-wx1)*0.2;
            zy2 = wy1 + (wy2-wy1)*0.15;
            zx3 = zx1;
            zy3 = zy1;
            zf1 = 12.0;
            zf2 = 12.0; 
            zf3 = 12.0; 
            _show_scale_view = false;
         }
         else if (_mapscale == 100000)
         {
            zx1 = wx1 + (wx2-wx1)*0.87;
            zy1 = wy2 - (wy2-wy1)*0.15;
            zx2 = wx1 + (wx2-wx1)*0.87;
            zy2 = wy1 + (wy2-wy1)*0.56;
            zx3 = zx1;
            zy3 = zy1;
            zf1 = 10.0;
            zf2 = 10.0; 
            zf3 = 10.0; 
            _show_scale_view = false;
         }
         else if (_mapscale == 250000)
         {
            zx1 = wx1 + (wx2-wx1)*0.5;
            zy1 = wy2 - (wy2-wy1)*0.1;
            zx2 = wx1 + (wx2-wx1)*0.15;
            zy2 = wy1 + (wy2-wy1)*0.15;
            zx3 = zx1;
            zy3 = zy1;
            zf1 = 12.0;
            zf2 = 12.0; 
            zf3 = 12.0; 
            _show_scale_view = false;
         }
         else
         {
            zx1 = wx1 + (wx2-wx1)*0.8;
            zy1 = wy2 - (wy2-wy1)*0.1;
            zx2 = wx1 + (wx2-wx1)*0.2;
            zy2 = wy1 + (wy2-wy1)*0.1;
            zx3 = zx1;
            zy3 = zy1;
            zf1 = 16.0;
            zf2 = 16.0; 
            zf3 = 16.0; 
            _show_scale_view = false;
         }
         tmpproj = _mapnameview->getView()->getProjection();
         tmpproj->zoomTo(zx1,zy1,zf1);
         tmpproj = _mapotherview->getView()->getProjection();
         tmpproj->zoomTo(zx2,zy2,zf2);
         if (_show_scale_view)
         {
            _mapscalewobj->popUp();
            tmpproj = _mapscaleview->getView()->getProjection();
            tmpproj->zoomTo(zx3,zy3,zf3);
         }
         else
           _mapscalewobj->popDown();

         input_wx = x1;
         input_wy = y1;
         input_wx_offset = 0;
         input_wy_offset = raster_height - 1;
         inputraster = new RasterComponent2D(inputimage,
                                             input_xres,
                                             input_yres,
                                             input_wx,
                                             input_wy,
                                             input_wx_offset,
                                             input_wy_offset);
         inputraster->setLayer(-1);
         inputraster->setSelectable(false);
         theModel2D.addComponent(inputraster);
      }

   calcControlPoints();
   _cur_ctrlpt = 0;

   app_image = inputimage;
   app_raster = inputraster;

   //showFileName(filename); // Display file name in window frame.
   _distdialog->initReport(filename);

   // Screen Output

   _stuff->setCellname(met.getCellName());
   _stuff->setFilename(filename);
   _stuff->setScale((int)_mapscale);
   _stuff->setUTMZone(met.getUTMZone());
   _stuff->setDatum(_datum);

   return 1;
}

RasterComponent2D* DRGVMainWObj::getAppRaster(void)
{
   return app_raster;
}

void DRGVMainWObj::showFileName(char* filename)
{
   XmString tmpstr;

   tmpstr = XmStringCreateLtoR(filename,XmFONTLIST_DEFAULT_TAG);
   XtVaSetValues(mainpopup->getWidget(),
                 XmNtitle,   tmpstr,
                 NULL);
   XmStringFree(tmpstr);
}
