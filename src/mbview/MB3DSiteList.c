
/* Begin user code block <abstract> */
/* End user code block <abstract> */

/**
 * README: Portions of this file are merged at file generation
 * time. Edits can be made *only* in between specified code blocks, look
 * for keywords <Begin user code> and <End user code>.
 */
/*
 * Generated by the ICS Builder Xcessory (BX).
 *
 * BuilderXcessory Version 6.1.3
 * Code Generator Xcessory 6.1.3 (08/19/04) CGX Scripts 6.1 Motif 2.1 
 *
 */
#include <Xm/Xm.h>
#include <Xm/BulletinB.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/ScrolledW.h>
#include <Xm/List.h>
#include <Xm/BulletinB.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/ScrolledW.h>
#include <Xm/List.h>
#include "MB3DSiteList.h"

/**
 * Common constant and pixmap declarations.
 */
#include "creation-c.h"

/**
 * Convenience functions from utilities file.
 */
extern void RegisterBxConverters(XtAppContext);
extern XtPointer BX_CONVERT(Widget, char *, char *, int, Boolean *);
extern XtPointer BX_DOUBLE(double);
extern XtPointer BX_SINGLE(float);
extern void BX_MENU_POST(Widget, XtPointer, XEvent *, Boolean *);
extern Pixmap XPM_PIXMAP(Widget, char **);
extern void BX_SET_BACKGROUND_COLOR(Widget, ArgList, Cardinal *, Pixel);

/**
 * Declarations for callbacks and handlers.
 */
extern void do_mbview_sitelist_delete(Widget, XtPointer, XtPointer);
extern void do_mbview_sitelist_popdown(Widget, XtPointer, XtPointer);
extern void do_mbview_sitelistselect(Widget, XtPointer, XtPointer);

/*
 * This table is used to define class resources that are placed
 * in app-defaults. This table is necessary so each instance
 * of this class has the proper default resource values set.
 * This eliminates the need for each instance to have
 * its own app-defaults values. This table must be NULL terminated.
 */
typedef struct _UIAppDefault {
    char*      cName;       /* Class name */
    char*      wName;       /* Widget name */
    char*      cInstName;   /* Name of class instance (nested class) */
    char*      wRsc;        /* Widget resource */
    char*      value;       /* value read from app-defaults */
} UIAppDefault;

static Boolean doInitAppDefaults = True;
static UIAppDefault  appDefaults[] = {
    {NULL, NULL, NULL, NULL, NULL}
};
/*
 * The functions to call in the apputils.c
 */
extern void InitAppDefaults(Widget, UIAppDefault*);
extern void SetAppDefaults(Widget, UIAppDefault*, char*, Boolean);

MB3DSiteListDataPtr
MB3DSiteListCreate ( MB3DSiteListDataPtr class_in, Widget parent, String name, ArgList args_in, Cardinal ac_in)
{
    Cardinal ac = 0;
    Arg      args[256];
    Cardinal cdc = 0;
    Boolean  argok = False;
    
    /**
     * Register the converters for the widgets.
     */
    RegisterBxConverters(XtWidgetToApplicationContext(parent));
    XtInitializeWidgetClass((WidgetClass)xmBulletinBoardWidgetClass);
    XtInitializeWidgetClass((WidgetClass)xmLabelWidgetClass);
    XtInitializeWidgetClass((WidgetClass)xmPushButtonWidgetClass);
    XtInitializeWidgetClass((WidgetClass)xmScrolledWindowWidgetClass);
    XtInitializeWidgetClass((WidgetClass)xmListWidgetClass);
    /**
     * Setup app-defaults fallback table if not already done.
     */
    if (doInitAppDefaults)
    {
        InitAppDefaults(parent, appDefaults);
        doInitAppDefaults = False;
    }
    /**
     * Now set the app-defaults for this instance.
     */
    SetAppDefaults(parent, appDefaults, name, False);
    
    ac = 0;
    {
        XmString    tmp0;
        
        tmp0 = (XmString) BX_CONVERT(parent, (char *)"Site List", 
                XmRXmString, 0, &argok);
        XtSetArg(args[ac], XmNdialogTitle, tmp0); if (argok) ac++;
        XtSetArg(args[ac], XmNresizePolicy, XmRESIZE_GROW); ac++;
        XtSetArg(args[ac], XmNx, 8); ac++;
        XtSetArg(args[ac], XmNy, 32); ac++;
        XtSetArg(args[ac], XmNwidth, 425); ac++;
        XtSetArg(args[ac], XmNheight, 385); ac++;
        class_in->MB3DSiteList = XmCreateBulletinBoard(parent,
            (char *)name,
            args, 
            ac);
        
        /**
         * Free any memory allocated for resources.
         */
        XmStringFree((XmString)tmp0);
    }
    
    
    ac = 0;
    {
        XmString    tmp0;
        
        tmp0 = (XmString) BX_CONVERT(class_in->MB3DSiteList, (char *)"Site List:", 
                XmRXmString, 0, &argok);
        XtSetArg(args[ac], XmNlabelString, tmp0); if (argok) ac++;
        XtSetArg(args[ac], XmNalignment, XmALIGNMENT_BEGINNING); ac++;
        XtSetArg(args[ac], XmNx, 20); ac++;
        XtSetArg(args[ac], XmNy, 40); ac++;
        XtSetArg(args[ac], XmNwidth, 390); ac++;
        XtSetArg(args[ac], XmNheight, 30); ac++;
        XtSetArg(args[ac], XmNfontList, 
            BX_CONVERT(class_in->MB3DSiteList, (char *)"-*-helvetica-bold-r-*-*-*-140-75-75-*-*-iso8859-1", 
            XmRFontList, 0, &argok)); if (argok) ac++;
        class_in->mbview_sitelist_label = XmCreateLabel(class_in->MB3DSiteList,
            (char *)"mbview_sitelist_label",
            args, 
            ac);
        XtManageChild(class_in->mbview_sitelist_label);
        
        /**
         * Free any memory allocated for resources.
         */
        XmStringFree((XmString)tmp0);
    }
    
    
    ac = 0;
    {
        XmString    tmp0;
        
        tmp0 = (XmString) BX_CONVERT(class_in->MB3DSiteList, (char *)"Delete Selected Sites", 
                XmRXmString, 0, &argok);
        XtSetArg(args[ac], XmNlabelString, tmp0); if (argok) ac++;
        XtSetArg(args[ac], XmNx, 20); ac++;
        XtSetArg(args[ac], XmNy, 10); ac++;
        XtSetArg(args[ac], XmNwidth, 170); ac++;
        XtSetArg(args[ac], XmNheight, 30); ac++;
        XtSetArg(args[ac], XmNfontList, 
            BX_CONVERT(class_in->MB3DSiteList, (char *)"-*-helvetica-bold-r-*-*-*-140-75-75-*-*-iso8859-1", 
            XmRFontList, 0, &argok)); if (argok) ac++;
        class_in->mbview_pushButton_sitelist_delete = XmCreatePushButton(class_in->MB3DSiteList,
            (char *)"mbview_pushButton_sitelist_delete",
            args, 
            ac);
        XtManageChild(class_in->mbview_pushButton_sitelist_delete);
        
        /**
         * Free any memory allocated for resources.
         */
        XmStringFree((XmString)tmp0);
    }
    
    XtAddCallback(class_in->mbview_pushButton_sitelist_delete, XmNactivateCallback, do_mbview_sitelist_delete, (XtPointer)0);
    
    ac = 0;
    {
        XmString    tmp0;
        
        tmp0 = (XmString) BX_CONVERT(class_in->MB3DSiteList, (char *)"Dismiss", 
                XmRXmString, 0, &argok);
        XtSetArg(args[ac], XmNlabelString, tmp0); if (argok) ac++;
        XtSetArg(args[ac], XmNx, 200); ac++;
        XtSetArg(args[ac], XmNy, 10); ac++;
        XtSetArg(args[ac], XmNwidth, 110); ac++;
        XtSetArg(args[ac], XmNheight, 30); ac++;
        XtSetArg(args[ac], XmNfontList, 
            BX_CONVERT(class_in->MB3DSiteList, (char *)"-*-helvetica-bold-r-*-*-*-140-75-75-*-*-iso8859-1", 
            XmRFontList, 0, &argok)); if (argok) ac++;
        class_in->mbview_pushButton_sitelist_dismiss = XmCreatePushButton(class_in->MB3DSiteList,
            (char *)"mbview_pushButton_sitelist_dismiss",
            args, 
            ac);
        XtManageChild(class_in->mbview_pushButton_sitelist_dismiss);
        
        /**
         * Free any memory allocated for resources.
         */
        XmStringFree((XmString)tmp0);
    }
    
    XtAddCallback(class_in->mbview_pushButton_sitelist_dismiss, XmNactivateCallback, do_mbview_sitelist_popdown, (XtPointer)0);
    
    ac = 0;
    XtSetArg(args[ac], XmNscrollingPolicy, XmAPPLICATION_DEFINED); ac++;
    XtSetArg(args[ac], XmNx, 20); ac++;
    XtSetArg(args[ac], XmNy, 70); ac++;
    XtSetArg(args[ac], XmNwidth, 390); ac++;
    XtSetArg(args[ac], XmNheight, 300); ac++;
    class_in->mbview_scrolledWindow_sitelist = XmCreateScrolledWindow(class_in->MB3DSiteList,
        (char *)"mbview_scrolledWindow_sitelist",
        args, 
        ac);
    XtManageChild(class_in->mbview_scrolledWindow_sitelist);
    
    ac = 0;
    XtSetArg(args[ac], XmNselectionPolicy, XmEXTENDED_SELECT); ac++;
    XtSetArg(args[ac], XmNlistSizePolicy, XmRESIZE_IF_POSSIBLE); ac++;
    XtSetArg(args[ac], XmNwidth, 390); ac++;
    XtSetArg(args[ac], XmNheight, 300); ac++;
    XtSetArg(args[ac], XmNfontList, 
        BX_CONVERT(class_in->mbview_scrolledWindow_sitelist, (char *)"-*-helvetica-bold-r-*-*-*-140-75-75-*-*-iso8859-1", 
        XmRFontList, 0, &argok)); if (argok) ac++;
    class_in->mbview_list_sitelist = XmCreateList(class_in->mbview_scrolledWindow_sitelist,
        (char *)"mbview_list_sitelist",
        args, 
        ac);
    XtManageChild(class_in->mbview_list_sitelist);
    XtAddCallback(class_in->mbview_list_sitelist, XmNsingleSelectionCallback, do_mbview_sitelistselect, (XtPointer)0);
    XtAddCallback(class_in->mbview_list_sitelist, XmNextendedSelectionCallback, do_mbview_sitelistselect, (XtPointer)0);
    XtAddCallback(class_in->mbview_list_sitelist, XmNbrowseSelectionCallback, do_mbview_sitelistselect, (XtPointer)0);
    
    /*
     * Assign functions to class record
     */
    
    
    /* Begin user code block <end_MB3DSiteListCreate> */
    /* End user code block <end_MB3DSiteListCreate> */
    
    return( class_in );
}