================================================================================
    MICROSOFT FOUNDATION CLASS LIBRARY : SolarCellTester Project Overview
===============================================================================

The application wizard has created this SolarCellTester application for 
you.  This application not only demonstrates the basics of using the Microsoft 
Foundation Classes but is also a starting point for writing your application.

This file contains a summary of what you will find in each of the files that
make up your SolarCellTester application.

SolarCellTester.vcproj
    This is the main project file for VC++ projects generated using an application wizard. 
    It contains information about the version of Visual C++ that generated the file, and 
    information about the platforms, configurations, and project features selected with the
    application wizard.

SolarCellTester.h
    This is the main header file for the application.  It includes other
    project specific headers (including Resource.h) and declares the
    CSolarCellTesterApp application class.

SolarCellTester.cpp
    This is the main application source file that contains the application
    class CSolarCellTesterApp.

SolarCellTester.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
    Visual C++. Your project resources are in 1033.

res\SolarCellTester.ico
    This is an icon file, which is used as the application's icon.  This
    icon is included by the main resource file SolarCellTester.rc.

res\SolarCellTester.rc2
    This file contains resources that are not edited by Microsoft 
    Visual C++. You should place all resources not editable by
    the resource editor in this file.

SolarCellTester.reg
    This is an example .reg file that shows you the kind of registration
    settings the framework will set for you.  You can use this as a .reg
    file to go along with your application or just delete it and rely
    on the default RegisterShellFileTypes registration.


/////////////////////////////////////////////////////////////////////////////

For the main frame window:
    The project includes a standard MFC interface.

SolarCellTesterMainFrm.h, SolarCellTesterMainFrm.cpp
    These files contain the frame class CSolarCellTesterMainFrame, which is derived from
    CFrameWnd and controls all SDI frame features.

/////////////////////////////////////////////////////////////////////////////

The application wizard creates one document type and one view:

SolarCellTesterDoc.h, SolarCellTesterDoc.cpp - the document
    These files contain your CSolarCellTesterDoc class.  Edit these files to
    add your special document data and to implement file saving and loading
    (via CSolarCellTesterDoc::Serialize).
    The Document will have the following strings:
        File extension:      LAB
        File type ID:        SolarCellTester.Document
        Main frame caption:  Solar Simulator
        Doc type name:       SolarCellTes
        Filter name:         SolarCellTester Files (*.LAB)
        File new short name: SolarCellTes
        File type long name: SolarCellTester.Document

SolarCellTesterView.h, SolarCellTesterView.cpp - the view of the document
    These files contain your CSolarCellTesterView class.
    CSolarCellTesterView objects are used to view CSolarCellTesterDoc objects.





/////////////////////////////////////////////////////////////////////////////

Other Features:

Printing and Print Preview support
    The application wizard has generated code to handle the print, print setup, and print preview
    commands by calling member functions in the CView class from the MFC library.

/////////////////////////////////////////////////////////////////////////////

Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named SolarCellTester.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

/////////////////////////////////////////////////////////////////////////////

Other notes:

The application wizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

If your application uses MFC in a shared DLL, you will need 
to redistribute the MFC DLLs. If your application is in a language 
other than the operating system's locale, you will also have to 
redistribute the corresponding localized resources MFC80XXX.DLL. 
For more information on both of these topics, please see the section on 
redistributing Visual C++ applications in MSDN documentation. 

/////////////////////////////////////////////////////////////////////////////
