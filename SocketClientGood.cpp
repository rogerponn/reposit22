// SocketClientGood.cpp : Defines the entry point for the console application.
//

// Win32Client4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 50
#define DEFAULT_PORT 40



int _tmain(int argc, _TCHAR* argv[])
{
	
	
	
	
	//----------------------
    // Declare and initialize variables.
    int iResult;
    WSADATA wsaData;

    SOCKET ConnectSocket = INVALID_SOCKET;
    struct sockaddr_in clientService; 

    int recvbuflen = DEFAULT_BUFLEN;
    char *sendbuf = "Client: sending data test";
    char recvbuf[DEFAULT_BUFLEN] = "";
	char recvbuf2[50];
    //----------------------
	//char recvbuf2[DEFAULT_BUFLEN] = "";

    //----------------------
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != NO_ERROR) {
        wprintf(L"WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    //----------------------
    // Create a SOCKET for connecting to server
    ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ConnectSocket == INVALID_SOCKET) {
        wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    //----------------------
    // The sockaddr_in structure specifies the address family,
    // IP address, and port of the server to be connected to.
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr( "127.0.0.1" );
    clientService.sin_port = htons( DEFAULT_PORT );

    //----------------------
    // Connect to server.
    iResult = connect( ConnectSocket, (SOCKADDR*) &clientService, sizeof(clientService) );
    if (iResult == SOCKET_ERROR) {
        wprintf(L"connect failed with error: %d\n", WSAGetLastError() );
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
  }

    //----------------------
    // Send an initial buffer
    iResult = send( ConnectSocket, sendbuf, (int)strlen(sendbuf), 0 );
    if (iResult == SOCKET_ERROR) {
        wprintf(L"send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    printf("Bytes Sent: %d\n", iResult);

    // shutdown the connection since no more data will be sent
    //iResult = shutdown(ConnectSocket, SD_SEND);
    /*if (iResult == SOCKET_ERROR) {
        wprintf(L"shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
	*/


	int i=1;
	char b[100];
	char  buffer[200], s[] = "computer", c = 'l';
   int    j=9;
   float fp = 1.7320534f;
   strcpy (b, " herettetet  kk");
   int p=0;
   
    // Receive until the peer closes the connection
    

   while( i < 10 )
   {

        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);

			printf("Bytes received: %s\n", recvbuf);
		 wprintf(L"Bytes received in string lenght : %d\n", recvbuflen);
		 i++;


	//sprintf( buffer , "   Integer:   %d\n", i );
	 // max is the maximum no of digits in a number
sprintf(b," hey this from client  %d",i);// C-style string formed without null

		// strcpy(recvbuf2,b);

		//printf("Bytes received: %s\n", recvbuf);
		// wprintf(L"Bytes received in string lenght : %d\n", recvbuflen);

        if ( iResult > 0 )
            wprintf(L"Bytes received: %d\n", iResult);
        else if ( iResult == 0 )
            wprintf(L"Connection closed\n");
        else
            wprintf(L"recv failed with error: %d\n", WSAGetLastError());

		 iResult = send( ConnectSocket, b, (int)strlen(b), 0 );

    } 

 

    // close the socket
    iResult = closesocket(ConnectSocket);
    if (iResult == SOCKET_ERROR) {
        wprintf(L"close failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    WSACleanup();
	getchar();
	return 0;
}



