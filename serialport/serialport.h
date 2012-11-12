/* -*- encoding: utf-8-unix; -*- */
/* File-name:    <serialport.h> */
/* Author:       <小苏打> */
/* Create:       <Thu Oct 25 16:41:17 2012> */
/* Time-stamp:   <Thursday October 25, 16:52:26 2012> */
/* Mail:         <Xsoda@Live.com> */

#ifndef _SERIALPORT_H_
#define _SERIALPORT_H_

#if defined __cplusplus
extern "C" {
#endif

#include <Windows.h>
#include <tchar.h>
#include <stdio.h>

#define SERIAL_PORT_GLOBAL

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
#if 0
typedef struct _tagSerialPort {
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
#endif
typedef struct {
	LPTSTR *lpPortlist;
    DWORD dwPortCount;
	HANDLE hPort;
	BOOL hPort_Opened;
	HANDLE hRcv;
	DWORD ThreadId;
	DWORD objResult;
	DWORD ThreadCode;
//	int waitTimeInMs;
	BOOL Terminate;
	CRITICAL_SECTION rxLock;
	DWORD rxErrors;
	int rxIndex;
	int rxReadPos;
	int maxRetries;
	int rxReceived;
	DWORD rxBufLen;
	BOOL rxLockInitialized;
	OVERLAPPED ov_Read, ov_Write;
	unsigned char rxBuffer[];
} SerialPort, *LPSerialPort;

#if ！defined SERIAL_PORT_GLOBAL
BOOL OpenComPort( int port, int rate, int parity, int retries );
BOOL CloseComPort( LPSerialPort *sp );

BOOL WriteComPort( LPSerialPort *sp, char *sendString, unsigned char txCount );
BOOL FlushComPort( LPSerialPort *sp );
BOOL ClearRx( LPSerialPort *sp );
BOOL ReadRxChar( LPSerialPort *sp, unsigned char *c );
BOOL WaitXmitDone( LPSerialPort *sp );
BOOL WriteComSignal(LPSerialPort *sp, DWORD func );
void PurgeRx();
void DisplaySystemError(DWORD errorCode);
int ReadData( void *buffer, int limit );
LONG SerialPort_GetPortNames(LPSerialPort sp, LPTSTR **lpPortList, LPDWORD lpCount);
LPSerialPort InitializeSerialPort();
#else
LONG SerialPort_GetPortNames(LPTSTR **lpPortList, LPDWORD lpCount);
BOOL OpenComPort( int port, int rate, int parity, int retries );
BOOL CloseComPort( void );
BOOL WriteComPort( char *sendString, unsigned char txCount );
BOOL FlushComPort( void );
BOOL ClearRx( void );
BOOL ReadRxChar( unsigned char *c );
BOOL WaitXmitDone( void );
BOOL WriteComSignal( DWORD func );
void PurgeRx();
void DisplaySystemError(DWORD errorCode);
int ReadData( void *buffer, int limit );
int InitializeSerialPort();
#endif
ULONG RxThread( void *arg );


#if defined __cplusplus
}
#endif

#endif /* _SERIALPORT_H_ */
