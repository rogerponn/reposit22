// RS232.cpp: implementation of the CRS232 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WhipPrtclX.h"
#include "RS232.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRS232::CRS232()
{
	m_Port = 0; 
	m_idComDev = (HANDLE)-1;
	m_Connected = 0;
	m_XonXoff = 0;
	m_FlowCtrl = 0;
	m_Parity  = 0;
	m_StopBits = 0;
	m_BaudRate = 0;
	bufRcvData = 0;
	m_bytesRcvd = 0;
	m_bytesSent = 0;
}

CRS232::~CRS232()
{

}

BOOL CRS232::OpenSerialPort(BYTE Port, DWORD BaudRate)
{
   char           szPort[15] ;
   COMMTIMEOUTS   CommTimeOuts ;

   if(m_idComDev != INVALID_HANDLE_VALUE)
      return FALSE;

   m_Port     = Port;
   m_BaudRate = BaudRate;

   wsprintf( szPort, "COM%d", m_Port) ;
   // open COMM device
   m_idComDev = CreateFile( szPort, GENERIC_READ | GENERIC_WRITE,
                            0,                    // exclusive access
                            NULL,                 // no security attrs
                            OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, // overlapped I/O
                            NULL ) ;

   if (m_idComDev == INVALID_HANDLE_VALUE)
   {     
     return ( FALSE ) ;
   }  

   // setup device buffers
   SetupComm( m_idComDev, 4096, 4096) ;
   
   if (SetupSerialPort() == FALSE)
   {
      TRACE("Setup failed COM%d, Baud:%d\r\n", m_Port, m_BaudRate);
      CloseHandle(m_idComDev);
      m_idComDev = INVALID_HANDLE_VALUE;      
      return FALSE;
   }

   // purge any information in the buffer
   PurgeComm( m_idComDev, PURGE_TXABORT | PURGE_RXABORT |
 	                       PURGE_TXCLEAR | PURGE_RXCLEAR ) ;

   // set up for overlapped I/O
   CommTimeOuts.ReadIntervalTimeout = 1; //MAXDWORD;
   CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
   CommTimeOuts.ReadTotalTimeoutConstant = 1 ;
   CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
   CommTimeOuts.WriteTotalTimeoutConstant = 1 ;

   SetCommTimeouts( m_idComDev, &CommTimeOuts ) ; 
   
   EscapeCommFunction(m_idComDev, SETRTS );
    
   TRACE("Serial port opened COM:%d, Baud:%d\r\n", m_Port, m_BaudRate);
   m_Connected = TRUE;
   
   return TRUE ;
}

BOOL CRS232::CloseSerialPort()
{
   if(! m_Connected)
      return FALSE;

   EscapeCommFunction(m_idComDev, CLRDTR);
   EscapeCommFunction(m_idComDev, CLRRTS );

   SetCommMask(m_idComDev, 0) ;
   
   BOOL ret = CloseHandle(m_idComDev); 
   
   m_idComDev = INVALID_HANDLE_VALUE;
   TRACE("Serial port closed COM%d, Baud:%d\r\n", m_Port, m_BaudRate);
   m_Connected = FALSE;
   
   return ret;
}
 

BOOL CRS232::SetupSerialPort()
{
   char strDcb[25];
   DCB   dcb ;

   if(GetCommState( m_idComDev, &dcb ) == FALSE)
      return FALSE;

   wsprintf(strDcb, "%d,n,8,1",m_BaudRate);
    
   if(BuildCommDCB(strDcb, &dcb) == FALSE) {
      return FALSE;
   }  
   
   // other various settings
   dcb.fBinary = TRUE ;
   dcb.fParity = TRUE ;

   if(SetCommState( m_idComDev, &dcb ) == FALSE)
   {
      char str[100];
      wsprintf(str, "SetCommState failed Error:%u", GetLastError());
      MessageBox(0, str, "", MB_OK);
      return FALSE;
   }

   return TRUE ;
} // end of SetupConnection()

BOOL CRS232::SetupSerialPortFromDlg()
{
	COMMCONFIG cc;
	char str[20];

    if(m_Port == 0)
        return FALSE; 

    memset(&cc, 0, sizeof(cc));
    cc.dcb.DCBlength = sizeof(cc.dcb);
    cc.dcb.BaudRate = m_BaudRate; 
    cc.dcb.fBinary = TRUE ;
    cc.dcb.fParity = TRUE ;
    cc.dcb.StopBits = 1;

	if(m_idComDev != INVALID_HANDLE_VALUE)	
    {
        // get current port settings 
	    ULONG lrc = sizeof(COMMCONFIG);				
	    GetCommConfig (m_idComDev, &cc, &lrc);
    }    
    
	// display dialog to modify settings */
	wsprintf(str, "COM%d", m_Port);
	if (!CommConfigDialog(str, NULL, &cc))
		return FALSE;
	
	m_BaudRate = cc.dcb.BaudRate; 

    if(m_idComDev != INVALID_HANDLE_VALUE)	
    {
        // write new settings */
	    if (!SetCommState (m_idComDev, &cc.dcb))
            return FALSE;   
    }

    return TRUE;
}
