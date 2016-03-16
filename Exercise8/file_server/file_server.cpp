//============================================================================
// Name        : file_server.cpp
// Author      : Lars Mortensen
// Version     : 1.0
// Description : file_server in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <lib.h>

using namespace std;

void sendFile(string fileName, long fileSize, int outToClient);

/**
 * main starter serveren og venter på en forbindelse fra en klient
 * Læser filnavn som kommer fra klienten.
 * Undersøger om filens findes på serveren.
 * Sender filstørrelsen tilbage til klienten (0 = Filens findes ikke)
 * Hvis filen findes sendes den nu til klienten
 *
 * HUSK at lukke forbindelsen til klienten og filen nÃ¥r denne er sendt til klienten
 *
 * @throws IOException
 *
 */
int main(int argc, char *argv[])
{
	// Setting up variables
	int sock, newsock, portnumber, n;
	long int sizeTCP;
	socklen_t client_size;
	socklen_t server_size;
	string fileName;
	portnumber = 9000;
	struct sockaddr_in serv_addr, cli_addr;


	sock = socket(AF_INET, SOCK_STREAM, 0);  // Creating af socket
	if (sock < 0 )
	{
		error("Error opening socket"); // Checking if socket is created
	} 
	bzero((char *) &serv_addr, sizeof(serv_addr)); // Setting serv_addr to 0
   	server_size = sizeof(serv_addr); // Finding the server address' size
	
    serv_addr.sin_family = AF_INET; // setting up servers family. 
    serv_addr.sin_addr.s_addr = INADDR_ANY; // setting up server adress
    serv_addr.sin_port = htons(portnumber); // setting up servers portnumber. 

    if (bind(sock, (struct sockaddr *) &serv_addr, server_size) == -1) // Binding the server address to the socket. 
    {
    	error("Error on binding");
    }

    listen(sock,5); // Listing for an incomming connection. Queue size is 5. 
    client_size = sizeof(cli_addr); // Finding the client address' size
	newsock = accept(sock, (struct sockaddr *) &cli_addr, &client_size); //Making af new socket from the incomming connection. 
	if (newsock == -1)
	{
		error("Error on accepting"); // Error handling of new socket. 
	}
		cout << "Accepting new socket" << endl;
	fileName = readTextTCP(fileName, newsock); // Reading fileName from client
	cout << "Getting filename" << endl;
	if((sizeTCP= check_File_Exists(fileName)) == 0) // checking if fileName exist and the size of fileName
	{
		writeTextTCP("File does not exist", newsock); // Writing that the file does not exist
		error("File does not exist"); // Error Handling 
	}
	cout << "File does exist" << endl;
	sendFile(fileName, sizeTCP, newsock); // Calling function sendFile
	cout << "File send" << endl;
	close(newsock); // closing new socket. 
	cout << "Closing newsocket" << endl;

	return 0;
}

/**
 * Sender filen som har navnet fileName til klienten
 *
 * @param fileName Filnavn som skal sendes til klienten
 * @param fileSize Størrelsen på filen, 0 hvis den ikke findes
 * @param outToClient Stream som der skrives til socket
	 */
void sendFile(string fileName, long fileSize, int outToClient)
{
	 cout << "Inside sendFile" << endl;
	 int bufferSize = 1000; // Setting up bufferSize
	 char buffer[bufferSize]; // making a char buffer with 1000 places
	 char sizeBuffer[256]; // making another buffer
	
	
	 cout << "Sending file" << endl;
	 sprintf(sizeBuffer, "%ld",fileSize); // Tranforming our fileSize to a char and storing it in sizeBuffer. 
	

	 writeTextTCP(sizeBuffer, outToClient); // writing the fileSize to client
	
	 std::ifstream FileIn; // setting up our stream
	 FileIn.open(fileName.c_str(), std::ios::in | std::ios::binary); // opening our file

	 if(fileSize > 0) // checking if file exist - not nessary 
	 {
	 	if(FileIn.is_open()) // checking if file is open. 
	 	{
	 		
	 		long rest = fileSize;

	 		while (rest > 0)
	 		{
	 		   	int count = FileIn.readsome(buffer,bufferSize); // Reading file 

	 		   	if(count >= 0)
	 		   	{
	 		   
	 		   write(outToClient, buffer, count); // sending file
	 		   rest -= count;
	 		  
	 		    }
	 		}
	 		   
	 	}
	 	else
	 	error("Could not open file");
	}

	FileIn.close(); // closing our stream. 
	}

