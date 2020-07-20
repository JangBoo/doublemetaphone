// MetaphoneCOM.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{845FE5AF-CC53-4C37-9464-EE20A866B9B0}", 
		 name = "MetaphoneCOM", 
		 helpstring = "Adam J. Nelson's Double Metaphone Implementations",
		 resource_name = "IDR_METAPHONECOM") ];

//Expose the value assigned to short alternate keys when no alternate key is available
//This way, VB users can use "MetaphoneKey.Invalid" to test for the absence of alternate keys
[ export, library_block]
enum MetaphoneKey {
	Invalid = -1
};