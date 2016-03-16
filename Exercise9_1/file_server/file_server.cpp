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


int main(int argc, char *argv[])
{
	int sock, portnumber, n, count;
	portnumber = 9000;
	socklen_t server_size;
	socklen_t client_size;
	int bufferSize = 1024;
	char buffer[bufferSize];
	string fileName;
	string file;

	struct sockaddr_in serv_addr, cli_addr;


	sock = socket(AF_INET, SOCK_DGRAM, 0); 
	if (sock < 0 )
	{
		error("Error opening socket");
	} 	
	server_size = sizeof(serv_addr);
	bzero((char *) &serv_addr, server_size);

	serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portnumber);

    if (bind(sock, (struct sockaddr *) &serv_addr, server_size) == -1)
    {
    	error("Error on binding");
    }

    client_size = sizeof(cli_addr);

    while(1)
    {
    	bzero(buffer, bufferSize);
    	n = recvfrom(sock,buffer,bufferSize,0, (struct sockaddr *) &cli_addr, &client_size);
    	if (n < 0)
    	{
    		error("Error in recvform");
    	}

    	cout << "Sending file" << endl;

		std::ifstream FileIn;
		switch(buffer[0])
		{
			case 'U':
			case 'u':
				fileName = "/proc/uptime"; 
				FileIn.open(fileName.c_str(), std::ios::in);
				if(FileIn.is_open())
				{
					cout << "Sending /proc/uptime" << endl;
					getline(FileIn, file);
				}
				else
				{
					error("Could not open file");
				}
	 			break;


			case 'L':
			case 'l':
				fileName = "/proc/loadavg";
				FileIn.open(fileName.c_str(), std::ios::in);
				if(FileIn.is_open())
				{
					cout << "Sending /proc/loadavg" << endl;
					getline(FileIn, file);
				}
				else
				{
					error("Could not open file");
				}
	 			break;

			default:
					error("Wrong letter");
				break;
		}
		n = sendto(sock, file.c_str(), file.length()+1, 0, (struct sockaddr *) &cli_addr, client_size);
	
		if(n < 0)
		{
			error("Error in sendTo");
		}
		FileIn.close();
		break;

    }
    cout << "closing socket" << endl;
    close(sock);

}   

   