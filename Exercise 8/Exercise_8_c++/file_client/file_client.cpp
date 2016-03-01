//============================================================================
// Name        : file_client.cpp
// Author      : Lars Mortensen
// Version     : 1.0
// Description : file_client in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <lib.h>

using namespace std;

void receiveFile(string fileName, int socketfd);

int main(int argc, char *argv[])
{
	int clientsocket, fileSize;
	clientsocket==socket(AF_INET, SOCK_STREAM, 0);
	string Message_out, Message_in;
	cin>>Message_out;
	string serverName ("Superman");
	clientsocket.connect(serverName,PORT);



	receiveFile(Message_out,serverName);

}

/**
 * Modtager filstørrelsen og udskriver meddelelsen: "Filen findes ikke" hvis 
 * størrelsen = 0
 * ellers
 * Åbnes filen for at skrive de bytes som senere modtages fra serveren 
 * (HUSK kun selve filnavn)
 * Modtag og gem filen i blokke af 1000 bytes indtil alle bytes er modtaget.
 * Luk filen, samt input output streams
 *
 * @param fileName Det fulde filnavn incl. evt. stinavn
 * @param sockfd Stream for at skrive til/læse fra serveren
 */
void receiveFile(string fileName, int sockfd)
{
	if (!check_File_Exists(fileName))
	{
		error("File does not exist");
	}

	writeTextTCP(fileName, sockfd);
	readTextTCP(Message_in,sockfd);
	fileSize=getFileSizeTCP(sockfd);

	int buffer[];

	while (fileSize>BUFSIZE)
	{
		
	}
}

