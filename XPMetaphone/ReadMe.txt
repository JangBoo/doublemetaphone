=========================================================================
    EXTENDED STORED PROCEDURE : XPMetaphone Project Overview
========================================================================


AppWizard has created XPMetaphone.dll for you.  

This file contains a summary of what you will find in each of the files that
make up your XPMetaphone application.

XPMetaphone.vcproj
    This is the main project file for VC++ projects generated using an Application Wizard. 
    It contains information about the version of Visual C++ that generated the file, and 
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

XPMetaphone.cpp
    This is the main dll source file.

proc.cpp
    This file contains the stored procedure xp_metaphone

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named XPMetaphone.pch and a precompiled types file named StdAfx.obj.


/////////////////////////////////////////////////////////////////////////////
Other notes:

After completing this Wizard, copy the XPMetaphone.dll over to your SQL Server 
\Binn directory.

Add your new Extended Stored Procedure from a Visual Studio Data Project, 
or using the SQL Server Enterprise Manager, or by executing the following 
SQL command:
  sp_addextendedproc 'xp_metaphone', 'XPMetaphone.DLL'

You may drop the extended stored procedure by using the SQL command:
  sp_dropextendedproc 'xp_metaphone'

You may release the DLL from the Server (to delete or replace the file), by 
using the SQL command:
  DBCC xp_metaphone(FREE)


/////////////////////////////////////////////////////////////////////////////
