#ifndef DRGVMAINWOBJ_H
#define DRGVMAINWOBJ_H

#include "Image/RGBImage.h"
#include "mvc/Component2D.h"
#include "WObj/WObj.h"
#include "WObj/MenuWObj.h"
#include "WObj/NButtonDialogWObj.h"
#include "DistDialogWObj.h"
#include "ErrorWObj.h"
#include "FileListWObj.h"
#include "XView2DWObj.h"
#include "DRGVZoomController.h"
#include "mvc/ZoomController.h"
#include "DistController.h"
#include "DRGVStuffWObj.h"

// The following structure allows us to pass the thisptr and some useful
// client data to a callback.
struct WObjClientdata
{
   XtPointer thisptr;
   XtPointer actual_clientdata;
};

class DRGVMainWObj : public WObj
{

   private:

   protected:

      Widget _mainwindow, _menubar;
      Widget _filedialog;
      XView2DWObj *_xview2dwobj;
      DRGVZoomController* _drgvzc;
      DistController* _distc;
      MenuWObj *_filemenu, *_viewmenu, *_toolmenu, *_datummenu;
      DistDialogWObj *_distdialog;
      ErrorWObj *_errorwobj;
      NButtonDialogWObj *_filelistpopup;
      FileListWObj *_filelist;
      DRGVStuffWObj *_stuff;
      NButtonDialogWObj *_zoomslider;
      XView2DWObj *_mapnameview;
      XView2DWObj *_mapotherview;
      XView2DWObj *_mapscaleview;
      PopupWObj   *_mapnamewobj;
      PopupWObj   *_mapotherwobj;
      PopupWObj   *_mapscalewobj;
      ZoomController  *_namectrl;
      ZoomController  *_otherctrl;
      ZoomController  *_scalectrl;
      bool _show_scale_view;

      RGBImage* app_image;
      RasterComponent2D* app_raster;
      PlusSymbolComponent2D** _calculated_ctrlpts;
      int _num_ctrlpts;
      int _cur_ctrlpt;
      char** _inputfile_list;
      int _num_inputfiles;
      int _cur_inputfile;

      // Control Point stuff
      double _mapscale;
      int _utmzone;
      int _datum;
      double _eastlong,_southlat;
      double _northlat,_westlong;

      // Callbacks

      void loadCallback(Widget w, XtPointer calldata);
      void fileDialogOKCallback(Widget w, XtPointer calldata);
      void fileDialogCancelCallback(Widget w, XtPointer calldata);
      void listCallback(Widget w, XtPointer calldata);
      void nextCallback(Widget w, XtPointer calldata);
      void prevCallback(Widget w, XtPointer calldata);
      void gotoCallback(Widget w, XtPointer calldata);
      void quitCallback(Widget w, XtPointer calldata);
      void fullviewCallback(Widget w, XtPointer calldata);
      void changeZoomCallback(Widget w, XtPointer calldata);
      void cursorPosCallback(Widget w, XtPointer calldata);
      void curptCallback(Widget w, XtPointer calldata);
      void nextptCallback(Widget w, XtPointer calldata);
      void prevptCallback(Widget w, XtPointer calldata);
      void measureDistCallback(Widget w, XtPointer calldata);
      void zoomPanCallback(Widget w, XtPointer calldata);
      void datumCallback(Widget w, XtPointer clientdata, XtPointer calldata);

      void setZoomCallback(Widget w, XtPointer calldata);
         // Makes the zoom controller the controller for the main window.

      // Regular methods

      void calcControlPoints(void);

   public:

      DRGVMainWObj(char* name, Widget parent);

      // Callback Wrappers

      static void loadWrapper(Widget w, XtPointer clientdata, XtPointer calldata);
         // For menu option "File|Load".

      static void fileDialogOKWrapper(Widget w, XtPointer clientdata, 
                                      XtPointer calldata);
         // For OK button on "Load" option's FileSelectionDialog.

      static void fileDialogCancelWrapper(Widget w, XtPointer clientdata,
                                          XtPointer calldata);
         // For CANCEL button on "Load" option's FileSelectionDialog.

      static void listWrapper(Widget w, XtPointer clientdata, XtPointer calldata);
         // For "File|List" option.

      static void nextWrapper(Widget w, XtPointer clientdata, XtPointer calldata);
         // For "File|Next" option.

      static void prevWrapper(Widget w, XtPointer clientdata, XtPointer calldata);
         // For "File|Prev" option.

      static void gotoWrapper(Widget w, XtPointer clientdata, XtPointer calldata);
         // For the Filelist's "Goto" button.

      static void quitWrapper(Widget w, XtPointer clientdata, XtPointer calldata);
         // For menu option "File|Quit".

      static void fullviewWrapper(Widget w, XtPointer clientdata,
                                  XtPointer calldata);
         // For menu option "View|FullView".

      static void changeZoomWrapper(Widget w, XtPointer clientdata,
                                    XtPointer calldata);
         // For menu option "View|Change Zoom Factor"

      static void cursorPosWrapper(Widget w, XtPointer clientdata, 
                                   XtPointer calldata);
         // For menu option "View|ShowCursorPos"

      static void curptWrapper(Widget w, XtPointer clientdata,
                               XtPointer calldata);
         // For menu option "View|CurrentPoint".

      static void nextptWrapper(Widget w, XtPointer clientdata,
                                XtPointer calldata);
         // For menu option "View|NextPoint".

      static void prevptWrapper(Widget w, XtPointer clientdata,
                                XtPointer calldata);
         // For menu option "View|PrevPoint".

      static void measureDistWrapper(Widget w, XtPointer clientdata,
                                     XtPointer calldata);
         // For menu option "Tools|MeasureDist".

      static void zoomPanWrapper(Widget w, XtPointer clientdata, XtPointer calldata);
         // For menu option "Tools|ZoomPan".

      static void datumWrapper(Widget w, XtPointer clientdata, XtPointer calldata);
         // For menu options "Datum|NAD27" and "Datum|NAD83".

      static void setZoomWrapper(Widget w, XtPointer clientdata, 
                                 XtPointer calldata);
         // Make zoom controller current controller in main window.

      // Regular Methods

      void init(char** argv, int argc);
      int loadFile(char* filename);
      RasterComponent2D* getAppRaster(void);
      void showFileName(char* filename);
};

#endif // DRGVMAINWOBJ_H
