// -*- encoding: utf-8-unix; -*-
// File-name:    <serialport.cpp>
// Author:       <小苏打>
// Create:       <Thu Oct 25 16:41:10 2012>
// Time-stamp:   <Thursday October 25, 17:7:20 2012>

#include "serialport.h"

#pragma region Port Names

/****************************************************************************
 *
 *     FUNCTION: GetPortNames
 *
 *     PURPOSE:  Get a list of comm ports available on host workstation.
 *
 *     PARAMS:   LPTSTR **lpPortList - a referance pointer to receive the list of port names
 *               LPDWORD lpCount - a referance DWORD pointer to receive the list size
 *
 *     RETURNS:  LONG - ERROR_SUCCESS if successful. If the function fails, 
 *                      the return value is a nonzero error code defined in WINERROR.H. 
 *
 * History:
 *                Jan '08 - Created
 *
\****************************************************************************/

LONG GetPortNames(LPTSTR **lpPortList, LPDWORD lpCount)
{
    HKEY hKey;
    LONG res;
    DWORD myType;
    TCHAR regValue[MAX_PATH];
    TCHAR portName[MAX_PATH];

    //open the regkey where the serial port data is stored
    res = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DEVICEMAP\\SERIALCOMM"), 0, KEY_READ, &hKey);
    if (res != ERROR_SUCCESS)
        return res;

    __try
    {
        //get the number of ports
        res = RegQueryInfoKey(hKey, NULL, NULL, NULL, NULL, NULL, NULL, lpCount, NULL, NULL, NULL, NULL);
        if (res != ERROR_SUCCESS)
            __leave;

        //create array based on number of ports
        *lpPortList = (LPTSTR *)malloc((*lpCount + 1) * sizeof(PTCHAR));
        (*lpPortList)[*lpCount] = NULL;  //May '11 So FreePortNameList() will stop at the end of the list

        if(NULL == *lpPortList)
            return 0;

        myType = REG_SZ;

        for (int port = 0, lenValue, lenName; port < *lpCount; port++)
        {
            lenValue = NELEMS(regValue);
            lenName = NELEMS(portName);

            //cycle through reg values to get port names 
            res = RegEnumValue(hKey, port, regValue, (LPDWORD) &lenValue, NULL, &myType, (LPBYTE)portName, (LPDWORD) &lenName);
            if (res != ERROR_SUCCESS)
                __leave;

            //allocate and populate an array index
            (*lpPortList)[port] = (LPTSTR)calloc((size_t)lenName, sizeof(TCHAR));

            if(NULL == (*lpPortList)[port])
                continue;

            _tmemmove((*lpPortList)[port], portName, lenName);
        }
    }
    __finally
    {
        RegCloseKey(hKey);
    }
    return res;
}

/****************************************************************************
 *
 *     FUNCTION: FreePortNameList
 *
 *     PURPOSE:  Free the memory allocated for the list of available comm ports.
 *
 *     PARAMS:   LPTSTR *portList - the allocated list of port names
 *
 *     RETURNS:  VOID
 *
 * History:
 *                Jan '08 - Created
 *
\****************************************************************************/

VOID FreePortNameList(LPTSTR * portList)
{
    int Count = 0;
    while (portList[Count])
        free(portList[Count++]);
    free(portList);
}

#pragma endregion Port Names


