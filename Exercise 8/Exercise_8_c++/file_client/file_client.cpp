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
#include <arpa/inet.h>

using namespace std;

void receiveFile(string fileName, int socketfd);

int main(int argc, char *argv[])
{

	if(argc>3)
	{
		cout<<argv[1]<<argv[2]<<endl;
		error("Invalid amount of arguments");
	}

	int clientsocket, fileSize;


	struct sockaddr_in serv_addr;
	struct hostent *server;


clientsocket=socket(AF_INET, SOCK_STREAM, 0);
	if (clientsocket < 0)
	{
		error("Fejl ved bygning af socket");
	}
	
	server = gethostbyname(argv[1]);
	 if(server == NULL)
    {
        error("Host does not exist");
    }

    //Setting up server address struct
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server -> h_addr, (char *) &serv_addr.sin_addr.s_addr, server -> h_length);
	serv_addr.sin_port=htons(PORT);


	if(connect(clientsocket,(struct sockaddr *) &serv_addr, sizeof(serv_addr))==-1)
		{
			error("Fejl ved forbindelse til server");
		}


	receiveFile(argv[2],clientsocket);

	close(clientsocket);
	return 0;
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
	int fileSize;
	string Message_in;

	writeTextTCP(fileName, sockfd);
	Message_in=readTextTCP(Message_in,sockfd);
	fileSize=getFileSizeTCP(sockfd);

	long int buffer[fileSize];



	while (fileSize>BUFSIZE)
	{
		read(sockfd,buffer,BUFSIZE);
		fileSize-=1000;
	}
	read(sockfd,buffer,fileSize);

	cout << "Message Recieved: "<< Message_in << endl;

		
}

