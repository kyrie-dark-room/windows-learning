#pragma once
/*
* Module£ºHY_DLL_Model.h
*/

#ifdef HY_LIB_API

// HY_LIB_API should be defined in all of the DLL's source
// code modules before this header file is included.

// All functions/variables are being exported.
#else

// This header file is included by an EXE source code module.
// Indicate that all functions/variables are being imported.
#define HY_LIB_API extern "C" __declspec(dllimport)
#endif

////////////////////////////////////////////////////////////
// Define any date structures and symbols here.
////////////////////////////////////////////////////////////

// Define exported variables here. (NOTE: Avoid exporting variables.)
HY_LIB_API int g_nResult;

///////////////////////////////////////////////////////////

// Define exported function prototypes here.
HY_LIB_API int Add(int nLeft, int nRight);

/////////////////////////////// End of File /////////////////////////////
