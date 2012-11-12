// -*- encoding: utf-8-unix; -*-
// File-name:    <serialport.cpp>
// Author:       <小苏打>
// Create:       <Thu Oct 25 16:41:10 2012>
// Time-stamp:   <Thursday October 25, 17:7:20 2012>
#include "serialport.h"

#define NELEMS(x) sizeof x / sizeof x[0]
#if defined SERIAL_PORT_GLOBAL
static SerialPort *sp = NULL;
#endif

#pragma region Port Names
LONG _GetPortNames(LPTSTR **lpPortList, LPDWORD lpCount)
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

		if(*lpPortList == NULL)
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

			memmove((*lpPortList)[port], portName, lenName);
		}
	}
	__finally
	{
		RegCloseKey(hKey);
	}
	return res;
}
#if defined SERIAL_PORT_GLOBAL
LONG SerialPort_GetPortNames(LPTSTR **lpPortList, LPDWORD lpCount)
#else
LONG SerialPort_GetPortNames( LPSerialPort sp, LPTSTR **lpPortList, LPDWORD lpCount)
#endif
{
	*lpPortList = sp->lpPortlist;
	*lpCount = sp->dwPortCount;
	return 1;
}

VOID _FreePortNameList(LPTSTR * portList)
{
	int Count = 0;
	while (portList[Count])
		free(portList[Count++]);
	free(portList);
}

#pragma endregion Port Names

#if defined SERIAL_PORT_GLOBAL
BOOL OpenComPort( int port, int rate, int parity, int retries )
#else
BOOL OpenComPort( LPSerialPort sp, int rate, int port, int parity, int retries )
#endif
{
	DWORD fdwEventMask;
	DCB newdcb;
	WCHAR buf[64];

	if(port > 9)//http://support.microsoft.com/default.aspx?scid=kb;EN-US;q115831#appliesto
		wsprintf( buf,_T("\\\\.\\COM%u"), port);	//"\\\\.\\COM10",
	else
		wsprintf( buf,_T("COM%u"), port);
	sp->hPort = CreateFile( buf, GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

	if (sp->hPort != INVALID_HANDLE_VALUE)
	{
		sp->maxRetries = retries;
		GetCommState(sp->hPort, &newdcb);
		newdcb.fBinary  = 1;    /*set binary mode*/
		//waitTimeInMs = waitTable[rate];
		newdcb.BaudRate = rate; //rateTable[rate];
		newdcb.ByteSize = 8;
		if (! parity)
			newdcb.Parity = NOPARITY;
		else
			newdcb.Parity = EVENPARITY;
		newdcb.StopBits = ONESTOPBIT;


		newdcb.fDsrSensitivity = FALSE;
		newdcb.fOutX = FALSE;
		newdcb.fInX = FALSE;
		newdcb.fErrorChar = FALSE;
		newdcb.fRtsControl =RTS_CONTROL_ENABLE;
		newdcb.fAbortOnError = TRUE;
		newdcb.fOutxCtsFlow = FALSE;
		newdcb.fOutxDsrFlow = FALSE;


		SetCommState(sp->hPort, &newdcb);

		// EV_BREAK   Set when a break is detected
		// EV_ERR     Set when Framing CE_FRAME, overrun CE_OVERRUN // or parity CE_RXPARITY error detected
		// EV_RING    Ring indicator was detected.
		// EV_RLSD    (received line signal detect) DCD changed state.
		// EV_CTS     Set when CTS changes state
		// EV_DSR     Set when DSR changes state
		// EV_RXCHAR  A character was placed in the input buffer
		// EV_TXEMPTY Last character in the output buffer was sent.

		if (!GetCommMask (sp->hPort, &fdwEventMask)) //get current event mask
			fdwEventMask = 0;
		fdwEventMask |=	EV_RXCHAR	|
			EV_TXEMPTY	|
			EV_BREAK	|
			EV_ERR		|
			EV_RING		|
			EV_RLSD		|
			EV_CTS		|
			EV_DSR		;

		SetCommMask(sp->hPort, fdwEventMask);
		EscapeCommFunction(sp->hPort,SETDTR);
		FlushComPort();
		if ( sp->rxLockInitialized == FALSE )
		{
			InitializeCriticalSection( &sp->rxLock );
			sp->rxLockInitialized = TRUE;
		}
		sp->Terminate = FALSE; // TRUE causes rxThread to exit
		sp->rxErrors	= 0;	 // overrun, parity and framing errors
		sp->rxIndex	= 0;	 // next location to store in rxBuffer
		sp->rxReadPos = 0;	 // next position to read in rxBuffer
		memset(sp->rxBuffer, 0, sp->rxBufLen);


		//if (hRcv = (HANDLE) _beginthread((void*) &RxThread,4096, &ThreadIx))

		if ((sp->hRcv = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&RxThread, sp, (DWORD) NULL, &sp->ThreadId))/* && (rxReceived)*/)
		{
			sp->hPort_Opened = TRUE;
			return(sp->hPort_Opened);
		}
		else
		{
			sp->ThreadId = GetLastError();    // For debug
			printf("Failed to create RxThread.\n");
			return(FALSE);
		}
	}
	return(FALSE);
}


/////////////////////////////////////////////////////////////////////////
#if defined SERIAL_PORT_GLOBAL
BOOL CloseComPort( void )
#else
BOOL CloseComPort(LPSerialPort sp)
#endif
{
	DWORD objResult;
	BOOL result;
	char c = 0;

	if ( sp->hPort == INVALID_HANDLE_VALUE )			return(FALSE);
	/*if(!hPort_Opened || hPort == INVALID_HANDLE_VALUE) return(TRUE);*/

	sp->Terminate = TRUE;
	EscapeCommFunction(sp->hPort,CLRDTR);

	FlushComPort();
	WriteComPort( &c, 1 );  // Event to wake RX thread

	objResult = WaitForSingleObject( sp->hRcv, (DWORD)2000 );
	if ( objResult != WAIT_OBJECT_0 )
	{
		printf("RxThread failed to terminate properly.\n");	  
	}
	else
		CloseHandle( sp->hRcv );

	//delete critical section
	if ( sp->rxLockInitialized == TRUE )
	{
		DeleteCriticalSection(&sp->rxLock);
		sp->rxLockInitialized = FALSE;
	}	
	result = CloseHandle( sp->hPort );
	sp->hPort_Opened = FALSE;
	sp->hPort = INVALID_HANDLE_VALUE;

	return(result);
}


/////////////////////////////////////////////////////////////////////////
#if defined SERIAL_PORT_GLOBAL
BOOL WriteComSignal( DWORD func )
#else
BOOL WriteComSignal(LPSerialPort sp, DWORD func)
#endif
{
	if ( sp->hPort == INVALID_HANDLE_VALUE)
		return(FALSE);
	EscapeCommFunction(sp->hPort, func);
	return(TRUE);
}


/////////////////////////////////////////////////////////////////////////
#if defined SERIAL_PORT_GLOBAL
BOOL WriteComPort( char *sendString, unsigned char txCount )
#else
BOOL WriteComPort( LPSerialPort sp, char *sendString, unsigned char txCount )
#endif
{
	DWORD		bWritten;
	HANDLE		writeEvnt;
	OVERLAPPED	writeSync;
	BOOL		rslt = TRUE;

	if ( sp->hPort == INVALID_HANDLE_VALUE)
		return(FALSE);
	writeEvnt = CreateEvent(NULL, TRUE, FALSE, _T("WRT_SYNC"));
	if(writeEvnt == INVALID_HANDLE_VALUE)
	{
		DisplaySystemError(GetLastError());
		return FALSE;
	}
	memset(&writeSync, 0, sizeof(OVERLAPPED));
	writeSync.hEvent = writeEvnt;
	rslt = WriteFile( sp->hPort, sendString, txCount, &bWritten, &writeSync);
	if(!rslt)
	{
		if(GetLastError() == ERROR_IO_PENDING)
		{
			if(!GetOverlappedResult(sp->hPort, &writeSync, &bWritten, TRUE))
				DisplaySystemError(GetLastError());
			else
				if(bWritten == txCount)
					rslt = TRUE;
		}
	}
	CloseHandle(writeEvnt);
	return rslt;		
}


/////////////////////////////////////////////////////////////////////////////
#if defined SERIAL_PORT_GLOBAL
BOOL FlushComPort( void )
#else
BOOL FlushComPort( LPSerialPort sp )
#endif
{
	if (sp->hPort != INVALID_HANDLE_VALUE)
	{ // Port was opened successfully
		PurgeComm(sp->hPort, PURGE_TXABORT | PURGE_TXCLEAR);
		PurgeComm(sp->hPort, PURGE_RXABORT | PURGE_RXCLEAR);
		return(TRUE);
	}
	return(FALSE);
}


/////////////////////////////////////////////////////////////////////////////
#if defined SERIAL_PORT_GLOBAL
void PurgeRx()
#else
void PurgeRx(LPSerialPort sp)
#endif
{
	DWORD commErr;
	COMSTAT	commStat;

	//PurgeComm(sp->hPort, PURGE_RXABORT | PURGE_RXCLEAR);
	ClearCommError(sp->hPort, &commErr, &commStat);
	PurgeComm(sp->hPort, PURGE_RXCLEAR);
	sp->rxErrors = 0;
	sp->rxIndex	= 0;
	sp->rxReadPos = 0;
	memset(sp->rxBuffer, 0, sp->rxBufLen);
}
/////////////////////////////////////////////////////////////////////////////
#if defined SERIAL_PORT_GLOBAL
BOOL ClearRx( void )
#else
BOOL ClearRx(LPSerialPort sp)
#endif
{
	if (sp->hPort != INVALID_HANDLE_VALUE)
	{ // Port was opened successfully
		EnterCriticalSection(&sp->rxLock);
		PurgeRx();
		LeaveCriticalSection(&sp->rxLock);
		return(TRUE);
	}
	return(FALSE);
}
/////////////////////////////////////////////////////////////////////////////
#if defined SERIAL_PORT_GLOBAL
BOOL ReadRxChar( unsigned char *c )
#else
BOOL ReadRxChar (LPSerialPort sp, unsigned char *c)
#endif
{
	if (sp->hPort != INVALID_HANDLE_VALUE)
	{ // Port was opened successfully
		EnterCriticalSection(&sp->rxLock);
		if ( sp->rxReadPos < sp->rxIndex )
		{		
			*c = sp->rxBuffer[sp->rxReadPos++];
			LeaveCriticalSection(&sp->rxLock);
			return(TRUE);
		}
		LeaveCriticalSection(&sp->rxLock);
	}
	return(FALSE);
}
/////////////////////////////////////////////////////////////////////////////
#if defined SERIAL_PORT_GLOBAL
BOOL WaitXmitDone( void )
#else
BOOL WaitXmitDone( LPSerialPort sp)
#endif
{
	COMSTAT curStatus;
	DWORD errs;
	int waitTime;

	// 96=833us 48=1.66ms 24=3.32ms 12=6.64ms 600=13.28ms 300=26.56ms
	if (sp->hPort != INVALID_HANDLE_VALUE)
	{ // Port was opened successfully
		EnterCriticalSection(&sp->rxLock);
		ClearCommError(sp->hPort, &errs, &curStatus);
		LeaveCriticalSection(&sp->rxLock);

		if (! curStatus.cbOutQue)
			return(TRUE);  // transmitter is empty

		//if ( (waitTime = (sp->waitTimeInMs * (curStatus.cbOutQue + 1))) < 100 )
		waitTime = 100;
		Sleep ( waitTime );

		EnterCriticalSection(&sp->rxLock);
		ClearCommError(sp->hPort, &errs, &curStatus);
		LeaveCriticalSection(&sp->rxLock);

		if (! curStatus.cbOutQue)
			return(TRUE);  // transmitter is empty
	}

	return(FALSE);
}
/////////////////////////////////////////////////////////////////////////////
ULONG RxThread( void *arg )
{
	int			x;
	DWORD		rxCount, fdwEventMask;
	COMSTAT		curStatus;
	char		recBuf[128];
	HANDLE		commEvnt;
	OVERLAPPED	commSync;
	DWORD		commErr;
	COMSTAT		commStat;
	BOOL		waitState;
#if !defined SERIAL_PORT_GLOBAL
	LPSerialPort sp = (LPSerialPort)arg;
#endif
	memset(&commSync, 0, sizeof(OVERLAPPED));
	commEvnt = CreateEvent(NULL, TRUE, FALSE, _T("RX_THREAD_COMM_EVENT"));
	if(commEvnt == INVALID_HANDLE_VALUE)
		ExitThread( (DWORD)-1 );  // dll exiting
	commSync.hEvent = commEvnt;
	ClearCommError(sp->hPort, &commErr, &commStat);

	while (sp->hPort != INVALID_HANDLE_VALUE)
	{
		waitState = WaitCommEvent(sp->hPort, &fdwEventMask, &commSync);
		if (!waitState)
		{
			if(GetLastError() == ERROR_IO_PENDING)
			{
				//MessageBox(NULL, "io pend", NULL, MB_OK);
				WaitForSingleObject(commEvnt,3000);
				GetOverlappedResult(sp->hPort, &commSync, &rxCount, TRUE);
			}
			else
				ExitThread( (DWORD)0 );  // dll exiting
		}

		if (sp->Terminate == TRUE)
			ExitThread( (DWORD)0 );  // dll exiting

		if (fdwEventMask & (EV_RXCHAR | EV_BREAK | EV_ERR | EV_TXEMPTY))
		{
			EnterCriticalSection(&sp->rxLock);
			ClearCommError(sp->hPort, &sp->rxErrors, &curStatus);
			sp->rxErrors &= CE_BREAK+CE_RXOVER+CE_OVERRUN+CE_RXPARITY+CE_FRAME+CE_IOE;
			if ( sp->rxErrors )
			{
				MessageBox(NULL, _T("rxerrors"), NULL, MB_OK);
				if ( sp->rxErrors & (CE_BREAK | CE_IOE) )
					FlushComPort();
				else
					PurgeRx();
			}
			else
			{
				HANDLE		rdEvnt;
				OVERLAPPED	rdSync;
				BOOL		rdRslt;
				memset(&rdSync, 0, sizeof(OVERLAPPED));
				rdEvnt = CreateEvent(NULL, TRUE, FALSE, _T("RD_SYNC_EVENT"));
				//memset(&rdSync, 0, sizeof(OVERLAPPED));
				rdSync.hEvent = rdEvnt;

				if (x = curStatus.cbInQue)
				{ 
					// There is receive data //MessageBox(NULL, "rec data", NULL, MB_OK);
					sp->rxReceived = 1;
					if (x > sizeof(recBuf))
						x = sizeof(recBuf);
					//ReadFile(hPort, recBuf, x, &rxCount, NULL);
					rdRslt = ReadFile(sp->hPort, recBuf, x, &rxCount, &rdSync);
					if(!rdRslt)
					{
						if(GetLastError() == ERROR_IO_PENDING)
						{
							if(!GetOverlappedResult(sp->hPort, &rdSync, &rxCount, TRUE))
								DisplaySystemError(GetLastError());
						}
					}
					if ( rxCount > ((unsigned int)sp->rxBufLen - sp->rxIndex) )
						rxCount = ((unsigned int)sp->rxBufLen - sp->rxIndex);
					memcpy(&sp->rxBuffer[sp->rxIndex], recBuf, rxCount);
					sp->rxIndex += rxCount;
				}

			}
			LeaveCriticalSection(&sp->rxLock);
		}
	}
	MessageBox(NULL, _T("end rxthread"), NULL, MB_OK);
	CloseHandle(commEvnt);
	ExitThread( (DWORD)0 );  // error return
	return( (DWORD)0 );
}

void DisplaySystemError(DWORD errorCode)
{
	LPVOID lpMsgBuf;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),		// Default language
		(LPTSTR) &lpMsgBuf,    0,    NULL );			// Process any inserts in lpMsgBuf.
	// Display the string.
#ifdef _DEBUG
	MessageBox( NULL, (LPCTSTR)lpMsgBuf, _T("Error"), MB_OK | MB_ICONINFORMATION );
#endif
	// Free the buffer.
	LocalFree( lpMsgBuf );
}


///////////////////////////////////////////////////////////////////////////////
#if defined SERIAL_PORT_GLOBAL
int ReadData( void *buffer, int limit )
#else
int ReadData( LPSerialPort sp, void *buffer, int limit)
#endif
{

	if( !sp->hPort_Opened || sp->hPort == NULL ) return( 0 );

	BOOL bReadStatus;
	DWORD dwBytesRead, dwErrorFlags;
	COMSTAT ComStat;

	ClearCommError( sp->hPort, &dwErrorFlags, &ComStat );
	if( !ComStat.cbInQue ) return( 0 );

	dwBytesRead = (DWORD) ComStat.cbInQue;
	if( limit < (int) dwBytesRead ) dwBytesRead = (DWORD) limit;

	bReadStatus = ReadFile( sp->hPort, buffer, dwBytesRead, &dwBytesRead, &sp->ov_Read );
	if( !bReadStatus ){
		if( GetLastError() == ERROR_IO_PENDING ){
			WaitForSingleObject( sp->ov_Read.hEvent, 2000 );
			return( (int) dwBytesRead );
		}
		return( 0 );
	}
	return( (int) dwBytesRead );
}
#if defined SERIAL_PORT_GLOBAL
int InitializeSerialPort()
#else
LPSerialPort InitializeSerialPort()
#endif
{
	LPSerialPort _sp;
	if ((_sp = (LPSerialPort)malloc(sizeof *_sp)) == NULL)
		return NULL;
	_GetPortNames(&_sp->lpPortlist, &_sp->dwPortCount);
#if defined SERIAL_PORT_GLOBAL
	sp = _sp;
#else
	return _sp;
#endif
}