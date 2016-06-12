#include <math.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/RowColumn.h>
#include <Xm/Label.h>
#include <Xm/List.h>
#include <Xm/FileSB.h>
#include "ErrorReportWObj.h"

#define STDERR(a,b,c) (sqrt(fabs((c-((b*b)/a)) / (a-1))))
#define RMSE(a,b) (sqrt(fabs((b/a))))

Widget ErrorReportWObj::createLabel(char* name, Widget parent)
{
   Widget retval;

   retval = XtVaCreateManagedWidget(name,xmLabelWidgetClass,
                                    parent,NULL);
   return retval;
}

void ErrorReportWObj::setLabel(Widget alabel, char* stuff)
{
   XmString tmpstr;

   tmpstr = XmStringCreateLtoR(stuff,XmFONTLIST_DEFAULT_TAG);
   XtVaSetValues(alabel,
                 XmNlabelString,     tmpstr,
                 NULL);
   XmStringFree(tmpstr);
}

void ErrorReportWObj::calcReport(void)
{

   char tmpbuf[1000];
   double dx,dy;
   double s_dist,ss_dist;
   double s_dx,ss_dx;
   double s_dy,ss_dy;
   int i;

   _avg_dist = 0.0;
   _stderr_dist = 0.0;
   _stderr_x = 0.0;
   _stderr_y = 0.0;
   _rmse_dist = 0.0;
   _rmse_x = 0.0;
   _rmse_y = 0.0;

   if (_numentries > 0)
      {
         // ss_blah means "Sum of Squares of blah"
         // s_blah means "Sum of blah" 
         ss_dist = 0.0;    s_dist = 0.0;
         ss_dx   = 0.0;    s_dx  = 0.0;
         ss_dy   = 0.0;    s_dy   = 0.0;
         for (i = 0; i < _numentries; i++)
            {
               s_dist = s_dist + _entries[i].dist;
               ss_dist = ss_dist + (_entries[i].dist * _entries[i].dist);
               dx = _entries[i].x2 - _entries[i].x1;
               dy = _entries[i].y2 - _entries[i].y1;
               s_dx = s_dx + dx;
               ss_dx = ss_dx + (dx * dx);
               s_dy = s_dy + dy;
               ss_dy = ss_dy + (dy * dy);
            }

         _avg_dist = s_dist / _numentries;

         if (_numentries > 1)
            {
               _stderr_dist = STDERR(_numentries,s_dist,ss_dist);
               _stderr_x    = STDERR(_numentries,s_dx,ss_dx);
               _stderr_y    = STDERR(_numentries,s_dy,ss_dy);
            }
         _rmse_dist = RMSE(_numentries,ss_dist);
         _rmse_x    = RMSE(_numentries,ss_dx);
         _rmse_y    = RMSE(_numentries,ss_dy);
      }

   sprintf(tmpbuf,"Number of samples: %d",_numentries);
   setLabel(_numsamples_label,tmpbuf);

   sprintf(tmpbuf,"Average Distance: %0.2lf",_avg_dist);
   setLabel(_avg_dist_label,tmpbuf);

   sprintf(tmpbuf,"Std. Error (Distance): %0.4lf",_stderr_dist);
   setLabel(_stderr_dist_label,tmpbuf);

   sprintf(tmpbuf,"Std. Error (DeltaX): %0.4lf",_stderr_x);
   setLabel(_stderr_x_label,tmpbuf);

   sprintf(tmpbuf,"Std. Error (DeltaY): %0.4lf",_stderr_y);
   setLabel(_stderr_y_label,tmpbuf);

   sprintf(tmpbuf,"Distance RMSE: %0.4lf",_rmse_dist);
   setLabel(_rmse_dist_label,tmpbuf);

   sprintf(tmpbuf,"DeltaX RMSE: %0.4lf",_rmse_x);
   setLabel(_rmse_x_label,tmpbuf);

   sprintf(tmpbuf,"DeltaY RMSE: %0.4lf",_rmse_y);
   setLabel(_rmse_y_label,tmpbuf);
}

void ErrorReportWObj::deleteCallback(Widget w, XtPointer calldata)
{
   // Delete an entry from the list.

   int i;
   int pos;
   int *poslist;
   int poscount;

   if (XmListGetSelectedPos(_list,&poslist,&poscount))
      {
         pos = poslist[0];
         XmListDeletePos(_list,pos);

         // Delete corresponding record in _entries[]
         for (i = pos - 1; i < _numentries - 1; i++)
            _entries[i] = _entries[i+1];

         _numentries = _numentries - 1;

         calcReport();
      }
}

void ErrorReportWObj::deleteWrapper(Widget w, XtPointer clientdata,
                                    XtPointer calldata)
{
   ((ErrorReportWObj*)clientdata)->deleteCallback(w,calldata);
}

void ErrorReportWObj::saveCallback(Widget w, XtPointer calldata)
{
   XtManageChild(_fsb);
}

void ErrorReportWObj::saveWrapper(Widget w, XtPointer clientdata,
                                  XtPointer calldata)
{
   ((ErrorReportWObj*)clientdata)->saveCallback(w,calldata);
}

void ErrorReportWObj::commentsCallback(Widget w, XtPointer calldata)
{
   _errrepcommentswobj->popUp();
}

void ErrorReportWObj::commentsWrapper(Widget w, XtPointer clientdata,
                                      XtPointer calldata)
{
   ((ErrorReportWObj*)clientdata)->commentsCallback(w,calldata);
}

void ErrorReportWObj::fsbOKCallback(Widget w, XtPointer calldata)
{
   XmFileSelectionBoxCallbackStruct *xfsb;
   char* tmpstr;

   xfsb = (XmFileSelectionBoxCallbackStruct*) calldata;

   XmStringGetLtoR(xfsb->value,XmSTRING_DEFAULT_CHARSET,&tmpstr);
   printReport(tmpstr);
   XtFree(tmpstr);

   XtUnmanageChild(_fsb);
}

void ErrorReportWObj::fsbOKWrapper(Widget w, XtPointer clientdata,
                                   XtPointer calldata)
{
   ((ErrorReportWObj*)clientdata)->fsbOKCallback(w,calldata);
}

void ErrorReportWObj::fsbCancelCallback(Widget w, XtPointer calldata)
{
   XtUnmanageChild(_fsb);
}

void ErrorReportWObj::fsbCancelWrapper(Widget w, XtPointer clientdata,
                                       XtPointer calldata)
{
   ((ErrorReportWObj*)clientdata)->fsbCancelCallback(w,calldata);
}


ErrorReportWObj::ErrorReportWObj(char* name, Widget parent)
               : NButtonDialogWObj(name,parent)
{
   _numentries = 0;

   _errorwobj = new ErrorWObj("errorwobj",parent);
   _errrepcommentswobj = new ErrRepCommentsWObj("commentswobj",parent);

   _errorform = XtVaCreateManagedWidget("errorreportform",
                                   xmFormWidgetClass,
                                   getFormWidget(),NULL);

   _list = XmCreateScrolledList(_errorform,"list",NULL,0);
   XtVaSetValues(_list,
                 XmNvisibleItemCount, 16,
                 NULL);
   XtVaSetValues(XtParent(_list),
                 XmNleftAttachment,    XmATTACH_FORM,
                 XmNtopAttachment,     XmATTACH_FORM,
                 XmNrightAttachment,   XmATTACH_FORM,
                 NULL);
   XtManageChild(_list);

   _rowcol = XtVaCreateManagedWidget("rowcol", xmRowColumnWidgetClass,
                _errorform,
                XmNleftAttachment,   XmATTACH_FORM,
                XmNtopAttachment,    XmATTACH_WIDGET,
                XmNtopWidget,        _list,
                XmNrightAttachment,  XmATTACH_FORM,
                XmNbottomAttachment, XmATTACH_FORM,
                XmNorientation,      XmVERTICAL,
                NULL);

   _filename_label    = createLabel("filenamelabel",_rowcol);
   _numsamples_label  = createLabel("numsampleslabel",_rowcol);
   _avg_dist_label    = createLabel("avgdistlabel",_rowcol);
   _stderr_dist_label = createLabel("stderrdistlabel",_rowcol);
   _stderr_x_label    = createLabel("stderrxlabel",_rowcol);
   _stderr_y_label    = createLabel("stderrylabel",_rowcol);
   _rmse_dist_label   = createLabel("rmsedistlabel",_rowcol);
   _rmse_x_label      = createLabel("rmsexlabel",_rowcol);
   _rmse_y_label      = createLabel("rmseylabel",_rowcol);

   setLabel(_filename_label,"Dataset:");
   setLabel(_numsamples_label,"Number of samples:");
   setLabel(_avg_dist_label,"Avgerage Distance:");
   setLabel(_stderr_dist_label,"Std. Error (Distance):");
   setLabel(_stderr_x_label,"Std. Error (DeltaX):");
   setLabel(_stderr_y_label,"Std. Error (DeltaY):");
   setLabel(_rmse_dist_label,"Distance RMSE:");
   setLabel(_rmse_x_label,"DeltaX RMSE:");
   setLabel(_rmse_y_label,"DeltaY RMSE:");

   addButton("Save",(XtPointer)this, saveWrapper);
   addButton("DeleteEntry",(XtPointer)this, deleteWrapper);
   addButton("Comments",(XtPointer)this, commentsWrapper);
   addButton("Close",NULL,NULL);

   setDialogContents(_errorform);

   // FileSelectionBoxDialog

   _fsb = XmCreateFileSelectionDialog(_baseWidget,"fsb",NULL,0);
   XtAddCallback(_fsb,XmNokCallback,fsbOKWrapper,(XtPointer)this);
   XtAddCallback(_fsb,XmNcancelCallback,fsbCancelWrapper,
                 (XtPointer)this);

   _datasetname = NULL;
   setDatasetName("");
}

void ErrorReportWObj::setDatasetName(char* name)
{
   char tmpbuf[1000];
   char* suffix;
   Widget tmpw;

   if (_datasetname != NULL)
      delete [] _datasetname;

   _datasetname = new char[strlen(name)+1];
   strcpy(_datasetname,name);

   sprintf(tmpbuf,"Dataset: %s",_datasetname);
   setLabel(_filename_label,tmpbuf);

   // Make up a default name to save the report under.
   strcpy(tmpbuf,name);
   suffix = strstr(tmpbuf,".tif");
   if (suffix != NULL)
      {
         strcpy(suffix,".txt");
         // Set this to be the default file name.
         tmpw = XmFileSelectionBoxGetChild(_fsb,XmDIALOG_TEXT);
         XtVaSetValues(tmpw,
                       XmNvalue,   tmpbuf,
                       NULL);
      }
}


void ErrorReportWObj::addEntry(double x1, double y1,
                               double x2, double y2,
                               double dist, char* samplelbl)
{
   char tmpbuf[1000];
   XmString tmpxmstr;

   sprintf(tmpbuf,"(%lf,%lf) , (%lf,%lf)     Distance: %lf",
           x1,y1,x2,y2,dist);

   _entries[_numentries].x1 = x1;
   _entries[_numentries].y1 = y1;
   _entries[_numentries].x2 = x2;
   _entries[_numentries].y2 = y2;
   _entries[_numentries].dist = dist;
   _entries[_numentries].samplelabel = new char[strlen(samplelbl)+1]; 
   strcpy(_entries[_numentries].samplelabel,samplelbl);
   if (strcmp(_entries[_numentries].samplelabel,"") != 0)
      {
         strcat(tmpbuf,"  Label: ");
         strcat(tmpbuf,_entries[_numentries].samplelabel);
      }
   _numentries = _numentries + 1;

   tmpxmstr = XmStringCreateLtoR(tmpbuf,XmFONTLIST_DEFAULT_TAG);
   XmListAddItemUnselected(_list,tmpxmstr,0);
   XmStringFree(tmpxmstr);

   calcReport();
}

void ErrorReportWObj::initReport(void)
{
   setDatasetName("");
   _numentries = 0;
   XmListDeleteAllItems(_list);
   _errrepcommentswobj->clear();
   calcReport();
}

void ErrorReportWObj::printReport(char* filename)
{
   FILE* outfile;
   int i;
   char tmpbuf[500];
   char* comments;
   struct passwd* pwent;
   struct tm* lcltime;
   time_t t;

   outfile = fopen(filename,"w");
   if (outfile == NULL)
      {
         sprintf(tmpbuf,"Error opening output file: %s",strerror(errno));
         _errorwobj->setMessage(tmpbuf);
         XtManageChild(_errorwobj->getWidget());
         return;
      }

   if (_numentries == 0)
      {
         fprintf(outfile,"\n\nNo entries in report.\n\n");
         fclose(outfile);
         return;
      }

   fprintf(outfile,"\n                          *** DRG Validation System Error Report ***\n\n");

   fprintf(outfile,"Dataset: %s\n",_datasetname);

   pwent = getpwuid(getuid());
   fprintf(outfile,"User: %s\n",pwent->pw_name);

   time(&t);
   lcltime = localtime(&t);
   strftime(tmpbuf,500,"%A %B %e %r %Z",lcltime);
   fprintf(outfile,"Date: %s\n\n\n",tmpbuf);

   
   fprintf(outfile,
           "    X1          Y1          X2          Y2          DeltaX      DeltaY    Distance   Label\n\n");

   for (i = 0; i < _numentries; i++)
      {
         fprintf(outfile,"%10.2lf  %10.2lf  %10.2lf  %10.2lf  %10.2lf  %10.2lf  %10.2lf   %s\n",
                 _entries[i].x1,
                 _entries[i].y1,
                 _entries[i].x2,
                 _entries[i].y2,
                 _entries[i].x2 - _entries[i].x1,
                 _entries[i].y2 - _entries[i].y1,
                 _entries[i].dist,
                 _entries[i].samplelabel);
      }

   fprintf(outfile,"\n\n");
   fprintf(outfile,"Number of samples: %d\n\n",_numentries);

   fprintf(outfile,"Average Distance: %0.2lf\n",_avg_dist);
   fprintf(outfile,"Std. Error (Distance): %0.4lf\n",_stderr_dist);
   fprintf(outfile,"Std. Error (DeltaX)  : %0.4lf\n",_stderr_x);
   fprintf(outfile,"Std. Error (DeltaY)  : %0.4lf\n",_stderr_y);
   fprintf(outfile,"RMSE Distance: %0.4lf\n",_rmse_dist);
   fprintf(outfile,"RMSE DeltaX  : %0.4lf\n",_rmse_x);
   fprintf(outfile,"RMSE DeltaY  : %0.4lf\n",_rmse_y);
   fprintf(outfile,"\n\n");

   // Print the comments (if any).

   comments = _errrepcommentswobj->getText();
   if (strlen(comments) > 0)
      {
         fprintf(outfile,"Comments:\n\n");
         fprintf(outfile,"%s\n",comments);
      }
   delete [] comments;

   fclose(outfile);
}
