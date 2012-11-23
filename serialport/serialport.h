/****************************************************************************\
*            
*     FILE:     SerialPort.h
*
*     PURPOSE:  SerialPort IO Wrapper
*
*     COMMENTS:
*               This source is distributed in the hope that it will be useful,
*               but WITHOUT ANY WARRANTY; without even the implied warranty of
*               MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
*     Copyright 2009 David MacDermot.
*
* History:
*                Sep '09 - Created
*                Aug '10 - Fixed allocation bug in GetPortNames()
*                May '11 - Several bug fixes (labled in code as follows: //May '11 ...)
*
\****************************************************************************/
#ifndef _SERIALPORT_H_
#define _SERIALPORT_H_

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>

#if defined __cplusplus
extern "C" {
#endif

#if defined _DEBUG
#include "../logger/logger.h"
#pragma comment(lib, "../Debug/logger.lib")
#endif
/****************************************************************************/
// Public Messages
#define SPM_DISPATCHNOTIFICATIONS  0x01
#define SPM_GETPORTNAMES           0x02
#define SPM_SETCONFIG              0x03
#define SPM_GETCONFIG              0x04
#define SPM_SETREADTIMEOUT         0x05
#define SPM_GETREADTIMEOUT         0x06
#define SPM_SETWRITETIMEOUT        0x07
#define SPM_GETWRITETIMEOUT        0x08
#define SPM_GETCTS                 0x09
#define SPM_GETDSR                 0x10
#define SPM_BYTESTOREAD            0x11
#define SPM_BYTESTOWRITE           0x12
#define SPM_WRITEBYTES             0x13
#define SPM_READBYTES              0x14
#define SPM_WRITESTRING            0x15
#define SPM_READSTRING             0x16
#define SPM_CLOSE                  0x17
#define SPM_OPEN                   0x18
#define SPM_FLUSH                  0x19
#define SPM_SETRXTHRESHOLD         0x1A
#define SPM_GETRXTHRESHOLD         0x1B
#define SPM_CREATE                 0x1C
#define SPM_DESTORY                0x1D
/****************************************************************************/
// Notifications
#define SPM_DATARECEIVED           0x2A
#define SPM_PINCHANGED             0x2B
#define SPM_ERRORRECEIVED          0x2C
/****************************************************************************/
// Public Enums

typedef enum tagFlowControl {
    NoFlowControl,
    CtsRtsFlowControl,
    CtsDtrFlowControl,
    DsrRtsFlowControl,
    DsrDtrFlowControl,
    XonXoffFlowControl
}FLOWCONTROL;

/****************************************************************************/
// Public Structs

typedef struct _tagConfig {
    UINT mask;
    LPTSTR pszPortName;
    INT cchTextMax;
    DWORD dwBaudRate;
    BYTE bParity;
    BYTE bDataBits;
    BYTE bStopBits;
    BOOL fDiscardNull;
    FLOWCONTROL flowControl;
} CONFIG, *LPCONFIG;

// Mask values
#define SPCF_PORTNAME    0x0001
#define SPCF_BAUDRATE    0x0002
#define SPCF_PARITY      0x0004
#define SPCF_DATABITS    0x0008
#define SPCF_STOPBITS    0x0010
#define SPCF_NULLDISCARD 0x0020
#define SPCF_FLOWCONT    0x0040

#define SPCF_ALLSETTINGS SPCF_PORTNAME|SPCF_BAUDRATE|SPCF_PARITY| \
                          SPCF_DATABITS|SPCF_STOPBITS|SPCF_NULLDISCARD|SPCF_FLOWCONT
typedef struct _tagCallback {
	int (*error)(char *error_info);
	int (*received)();
	int (*pin_change)(int signal);
}Callback, *LPCallback;

typedef struct _tagINSTANCEDATA {
	LPTSTR lpComm;
	HANDLE hComm;
	BOOL fEndListner; //May '11 Added
	HANDLE hListnerEvent; //May '11 Added
	HANDLE hEventThread;
	HANDLE hStartEvent;
	HANDLE hIOEvent;
	DWORD dwReceiveByteThreshold;
	DWORD dwEventThread;
	LPTSTR *lpPortlist;
	DWORD dwPortCount;
	FLOWCONTROL flowControl;
	Callback sp_callback;
} INSTANCEDATA  , *LPINSTANCEDATA;

/****************************************************************************/
// Prototypes
LRESULT SerialPort_Proc(UINT msg, WPARAM wParam, LPARAM lParam);
BOOL InitializeSerialPort();
BOOL ReleaseSerialPort();
int SetSerialPortCallback(int (*error)(char*),
						  int (*received)(),
						  int (*pin_change)(int));
/****************************************************************************/
// Macroes

#define SerialPort_GetPortNames(lpCount)  (LPTSTR*)SerialPort_Proc(SPM_GETPORTNAMES,(WPARAM)(lpCount),(LPARAM)0L)
#define SerialPort_SetConfigurations(lpConfig)  (BOOL)SerialPort_Proc(SPM_SETCONFIG,(WPARAM)0,(LPARAM)(const LPCONFIG)(lpConfig))
#define SerialPort_GetConfigurations(lpConfig)  (BOOL)SerialPort_Proc(SPM_GETCONFIG,(WPARAM)0,(LPARAM)(LPCONFIG)(lpConfig))
#define SerialPort_SetReadTimeout(dwTimeout)  (BOOL)SerialPort_Proc(SPM_SETREADTIMEOUT,(WPARAM)(dwTimeout),(LPARAM)0L)
#define SerialPort_GetReadTimeout()  (DWORD)SerialPort_Proc(SPM_GETREADTIMEOUT,(WPARAM)0,(LPARAM)0L)
#define SerialPort_SetWriteTimeout(dwTimeout)  (BOOL)SerialPort_Proc(SPM_SETWRITETIMEOUT,(WPARAM)(dwTimeout),(LPARAM)0L)
#define SerialPort_GetWriteTimeout()  (DWORD)SerialPort_Proc(SPM_GETWRITETIMEOUT,(WPARAM)0,(LPARAM)0L)
#define SerialPort_GetCTS()  (BOOL)SerialPort_Proc(SPM_GETCTS,(WPARAM)0,(LPARAM)0L)
#define SerialPort_GetDSR()  (BOOL)SerialPort_Proc(SPM_GETDSR,(WPARAM)0,(LPARAM)0L)
#define SerialPort_BytesToRead()  (DWORD)SerialPort_Proc(SPM_BYTESTOREAD,(WPARAM)0,(LPARAM)0L)
#define SerialPort_BytesToWrite()  (DWORD)SerialPort_Proc(SPM_BYTESTOWRITE,(WPARAM)0,(LPARAM)0L)
#define SerialPort_WriteBytes(lpData, dwSize)  (BOOL)SerialPort_Proc(SPM_WRITEBYTES,(WPARAM)(dwSize),(LPARAM)(lpData))
#define SerialPort_ReadBytes(lpBuf, dwSize)  (DWORD)SerialPort_Proc(SPM_READBYTES,(WPARAM)(dwSize),(LPARAM)(lpBuf))
#define SerialPort_QueryBytes(lpData, lpdwSize) (DWORD)SerialPort_Proc( SPM_QUERYBYTES,(WPARAM)(lpdwSize),(LPARAM)(lpBuf))
#define SerialPort_WriteString(lpsztext)  (BOOL)SerialPort_Proc(SPM_WRITESTRING,(WPARAM)0,(LPARAM)(lpsztext))
#define SerialPort_ReadString(lpszBuf, dwSize)  (DWORD)SerialPort_Proc(SPM_READSTRING,(WPARAM)(dwSize),(LPARAM)(lpszBuf))
#define SerialPort_Close()  (BOOL)SerialPort_Proc(SPM_CLOSE,(WPARAM)0,(LPARAM)0L)
#define SerialPort_Open(lpConfig)  (BOOL)SerialPort_Proc(SPM_OPEN,(WPARAM)0,(LPARAM)(lpConfig))
#define SerialPort_FlushReadBuf()  (BOOL)SerialPort_Proc(SPM_FLUSH,(WPARAM)PURGE_RXCLEAR,(LPARAM)0L)
#define SerialPort_FlushWriteBuf()  (BOOL)SerialPort_Proc(SPM_FLUSH,(WPARAM)PURGE_TXCLEAR,(LPARAM)0L)
#define SerialPort_SetReceiveByteThreshold(dwNumBytes)  (BOOL)SerialPort_Proc(SPM_SETRXTHRESHOLD,(WPARAM)(dwNumBytes),(LPARAM)0L)
#define SerialPort_GetReceiveByteThreshold()  (DWORD)SerialPort_Proc(SPM_GETRXTHRESHOLD,(WPARAM)0,(LPARAM)0L)

#if defined __cplusplus
}
#endif

#endif // _SERIALPORT_H_