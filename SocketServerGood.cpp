// ServerSocket.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#ifndef UNICODE
#define UNICODE
#endif
#define DEFAULT_BUFLEN 50
#define DEFAULT_PORT "40"
#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
// Need to link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

SOCKET ClientSocket = INVALID_SOCKET;

int _tmain(int argc, _TCHAR* argv[])
{
	int iSendResult=0;
	char recvbuf[DEFAULT_BUFLEN];
	//----------------------
    // Initialize Winsock.
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        wprintf(L"WSAStartup failed with error: %ld\n", iResult);
        return 1;
    }
    //----------------------
    // Create a SOCKET for listening for
    // incoming connection requests.
    SOCKET ListenSocket;
    ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ListenSocket == INVALID_SOCKET) {
        wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    //----------------------
    // The sockaddr_in structure specifies the address family,
    // IP address, and port for the socket that is being bound.
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr("127.0.0.1");
    service.sin_port = htons(40);

    if (bind(ListenSocket,
             (SOCKADDR *) & service, sizeof (service)) == SOCKET_ERROR) {
        wprintf(L"bind failed with error: %ld\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    //----------------------
    // Listen for incoming connection requests.
    // on the created socket
    if (listen(ListenSocket, 1) == SOCKET_ERROR) {
        wprintf(L"listen failed with error: %ld\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    //----------------------
    // Create a SOCKET for accepting incoming requests.
    //SOCKET AcceptSocket;
    wprintf(L"Waiting for client to connect...\n");

    //----------------------
    // Accept the connection.
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        wprintf(L"accept failed with error: %ld\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    } else
        wprintf(L"Client connected.\n");
		

    // No longer need server socket
   


	//--------------------------------------------------------------------------------------------------------------//
	  
	int i=1;
	char b[100];
	
	char recvbuf2[50];
	
  
   strcpy (b, " From Server Sent ");
 
	 // No longer need server socket
    closesocket(ListenSocket);
	char *sendbuf2 = "Server raj sending data test";
	int x=0;
	
    // Receive until the peer shuts down the connection
	 int messageLength = 160; 
   while(x<10)
   {

	   
		


		if (ClientSocket >= 0) { 
      int messageLength = 160; 
      char message[160+1]; 
      int in, index = 0, limit = messageLength; 
 //iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
      while ( ( iResult = recv(ClientSocket, recvbuf, recvbuflen, 0))> 0 && x < 6) { 
        printf("Incoming data %s\n", recvbuf); 





		//-------------------------------------------------------------------//

		sprintf(b,"From Server sent data  i =   %d",i);// C-style string formed without null

		 //strcat(recvbuf2,b);

		 i++;
		 x++;
		//----------------------------------------------------------------------//


		 iSendResult = send( ClientSocket, b, (int)strlen(b), 0 );
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
				
            }
      } 
 
      
    } 
		x++;

		// ClientSocket = accept(ListenSocket, NULL, NULL);

   }
    // cleanup
    closesocket(ClientSocket);
   


	


	//--------------------------------------------------------------------------------------------------------------------//

    WSACleanup();

	getchar();
	

	return 0;
}


