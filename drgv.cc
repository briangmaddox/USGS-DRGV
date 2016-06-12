
#include <Xm/Xm.h>
#include "WObj/WObj.h"
#include "WObj/TopLevelWObj.h"
#include "WObj/PopupWObj.h"
#include "WObj/NButtonDialogWObj.h"
#include "mvc/MVC.h"
#include "DRGVMainWObj.h"
#include "FileListWObj.h"
#include "XMultiView2DWObj.h"
#include "MultiViewZoomController.h"

TopLevelWObj* topLevel;
PopupWObj *ctrlptpopup;
XMultiView2DWObj *ctrlptwin;
PopupWObj *mainpopup;
DRGVMainWObj *mainwin;

void main(int argc, char** argv)
{

   topLevel = new TopLevelWObj("Drgv",&argc,argv);

   ctrlptpopup = new PopupWObj("ctrlptpopup",topLevel->getWidget());
   ctrlptwin = new XMultiView2DWObj("ctrlptwin",ctrlptpopup->getWidget());
   (ctrlptwin->getView())->setController(new MultiViewZoomController);

   mainpopup = new PopupWObj("mainpopup",topLevel->getWidget());
   mainwin = new DRGVMainWObj("mainwin",mainpopup->getWidget());

   XtRealizeWidget(topLevel->getWidget());
   mainpopup->popUp();
   ctrlptpopup->popUp();

   mainwin->init(argv,argc);

   XtAppMainLoop(topLevel->getAppContext());
}




