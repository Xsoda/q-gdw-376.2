/* -*- encoding: utf-8-unix; -*- */
/* File-name:    <serialport.h> */
/* Author:       <小苏打> */
/* Create:       <Thu Oct 25 16:41:17 2012> */
/* Time-stamp:   <Thursday October 25, 16:52:26 2012> */
/* Mail:         <Xsoda@Live.com> */

#ifndef _SERIALPORT_H_
#define _SERIALPORT_H_

/****************************************************************************/
// Public Enums

typedef enum tagFlowControl {
    NoFlowControl,
    CtsRtsFlowControl,
    CtsDtrFlowControl,
    DsrRtsFlowControl,
    DsrDtrFlowControl,
    XonXoffFlowControl
}FlowControl;

/****************************************************************************/
// Public Structs

typedef struct tagConfig {
    UINT mask;
    LPTSTR pszPortName;
    INT cchTextMax;
    DWORD dwBaudRate;
    BYTE bParity;
    BYTE bDataBits;
    BYTE bStopBits;
    BOOL fDiscardNull;
    FlowControl flowControl;
} Config, *LPConfig;

typedef struct _tagSerialPort {
    HWND hwndParent;
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
    FlowControl flowControl;
} SerialPort  , *LPSerialPort;
LONG SerialPort_GetPortNames(LPTSTR **lpPortList, LPDWORD lpCount);
VOID SerialPort_FreePortNameList(LPTSTR * portList);

#endif /* _SERIALPORT_H_ */
