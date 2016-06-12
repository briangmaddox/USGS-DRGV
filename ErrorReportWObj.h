#ifndef ERRORREPORTWOBJ_H
#define ERRORREPORTWOBJ_H

#include "WObj/WObj.h"
#include "WObj/NButtonDialogWObj.h"
#include "ErrorWObj.h"
#include "ErrRepCommentsWObj.h"

struct ErrorReportRec
{
   double x1,y1;
   double x2,y2;
   double dist;
   char* samplelabel;
};

class ErrorReportWObj : public NButtonDialogWObj
{
   private:

   protected:
      Widget _errorform;
      Widget _list;
      Widget _rowcol;
      Widget _filename_label;
      Widget _numsamples_label;
      Widget _avg_dist_label;
      Widget _stderr_dist_label;
      Widget _stderr_x_label;
      Widget _stderr_y_label;
      Widget _rmse_dist_label;
      Widget _rmse_x_label;
      Widget _rmse_y_label;
      Widget _fsb;
      ErrorWObj *_errorwobj;
      ErrRepCommentsWObj *_errrepcommentswobj;

      char* _datasetname;
      double _avg_dist;
      double _stderr_dist;
      double _stderr_x;
      double _stderr_y;
      double _rmse_dist;
      double _rmse_x;
      double _rmse_y;

      ErrorReportRec _entries[100];
      int _numentries;

      Widget createLabel(char* name, Widget parent);

      void setLabel(Widget alabel, char* stuff);

      void calcReport(void);
         // Recalulates rmse and other statistics using _entries[].

      void deleteCallback(Widget w, XtPointer calldata);
      static void deleteWrapper(Widget w, XtPointer clientdata,
                                XtPointer calldata);

      void saveCallback(Widget w, XtPointer calldata);
      static void saveWrapper(Widget w, XtPointer clientdata,
                              XtPointer calldata);

      void commentsCallback(Widget w, XtPointer calldata);
      static void commentsWrapper(Widget w, XtPointer clientdata,
                                  XtPointer calldata);

      void fsbOKCallback(Widget w, XtPointer calldata);
      static void fsbOKWrapper(Widget w, XtPointer clientdata,
                               XtPointer calldata);

      void fsbCancelCallback(Widget w, XtPointer calldata);
      static void fsbCancelWrapper(Widget w, XtPointer clientdata, 
                                   XtPointer calldata);

   public:

      ErrorReportWObj(char* name, Widget parent);

      void setDatasetName(char* name);

      void addEntry(double x1, double y1,
                    double x2, double y2,
                    double dist, char* samplelbl);

      void initReport(void);
         // Initializes the report.

      void printReport(char* filename);
         // Prints a nice little report to filename.
};

#endif // ERRORREPORTWOBJ_H
