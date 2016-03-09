﻿#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define PORT 9000
#define BUFFLENGTH 1024

using namespace std;

void error(const std::string msg);
void sendAndRecieve(char msg);
int main()
{

/*	if(argc>3)
	{
		error("Invalid amount of arguments");
	}
	*/
	char msg[BUFFLENGTH];
	int clientsocket;
	socklen_t serv_length;
 	clientsocket = socket(AF_INET,SOCK_DGRAM,0);

 	struct sockaddr_in serv_addr;
 	serv_addr.sin_family = AF_INET;
 	serv_addr.sin_port = htons(PORT);
 	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
 	serv_length=sizeof(serv_addr);

 	cout<< "What information would you like to pull?" << endl
 		<< "U/u: Uptime of server" << endl
 		<< "L/l: Load on server" << endl;

 	cin>>msg;

 	if(msg[0]=='U'||msg[0]=='u'||msg[0]=='L'||msg[0]=='l')
 		sendto(clientsocket,msg,BUFFLENGTH,0,(struct sockaddr*)&serv_addr, serv_length);

 	else
		error("Invalid input");
	
	recvfrom(clientsocket,msg,BUFFLENGTH,0,(struct sockaddr*) &serv_addr, &serv_length);

	cout<<"Incoming Message:" << endl
		<< msg << endl;

	close(clientsocket);
	return 0;
}

void error(const std::string msg)
{
    perror(msg.c_str());
    exit(0);
}

