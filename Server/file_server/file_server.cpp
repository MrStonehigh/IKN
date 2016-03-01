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
#include <string>
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
	int sock, newsock, portnumber, n;
	long sizeTCP;
	socklen_t client_size;
	socklen_t server_size;
	string fileName;
	portnumber = 9000;
	struct sockaddr_in serv_addr, cli_addr;


	char buffer[256];
	bzero(buffer,1000);
	sock = socket(AF_INET, SOCK_STREAM, 0); 
	if (sock < 0 )
	{
		error("Error opening socket");
	} 
	bzero((char *) &serv_addr, sizeof(serv_addr));
   	server_size = sizeof(serv_addr);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portnumber);

    if (bind(sock, (struct sockaddr *) &serv_addr, server_size) == -1)
    {
    	error("Error on binding");
    }

    listen(sock,5);
    client_size = sizeof(cli_addr);
	newsock = accept(sock, (struct sockaddr *) &cli_addr, &client_size);
	if (newsock == -1)
	{
		error("Error on accepting");
	}
		
	fileName = readTextTCP(fileName, newsock);
	extractFileName(fileName);
	if(check_File_Exists(fileName) == 0)
	{
		writeTextTCP("File does not exist", newsock);
		error("File does not exist");
	}
	sizeTCP = getFileSizeTCP(newsock);

	sendFile(fileName, sizeTCP, newsock);

	close(sock);
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
	 char * buffer = new char [fileSize];
	 char * buf = new char[1000];
	 int newfileSize = 0;
	
	 string fileSize_s = to_string(fileSize);

	  writeTextTCP(fileSize_s, outToClient);

	 std::fstream fs;
	 fs.open(fileName, ... );
	 fs.read(buffer, fileSize);

	 newfileSize = fileSize;

	 buf = buffer[]

	 while(newfileSize > 0)
	 {

	 	if(newfileSize < 1000)
	 	{
	 		write 
	 		break;
	 	}
		 write
	
		 newfileSize = newfileSize-1000;
	 };



}

