#include "pch.h"
/*
* Module£ºHY_DLL_Model.cpp
*/

// Include the standart Windows and C-Runtime header files here.
#include <Windows.h>

// This DLL source code file exports functions and variables.
#define HY_LIB_API extern "C" __declspec(dllexport)

// Include the exported data structures, symbols, functions, and variables.
#include "HY_DLL_Model.h"

//////////////////////////////////////////////////////

// Place the code for this DLL source code file here.
int g_nResult;

int Add(int nLeft, int nRight)
{
	g_nResult = nLeft + nRight;
	return(g_nResult);
}