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
	// Setting up variables
	int sock, portnumber, n, count;
	portnumber = 9000;
	socklen_t server_size;
	socklen_t client_size;
	int bufferSize = 256;
	char buffer[bufferSize];
	string fileName;
	string file;

	struct sockaddr_in serv_addr, cli_addr;


	sock = socket(AF_INET, SOCK_DGRAM, 0); //Setting up socket
	if (sock < 0 )
	{
		error("Error opening socket"); //Error handling of socket
	} 	
	server_size = sizeof(serv_addr); // finding server size
	bzero((char *) &serv_addr, server_size); // setting the serv_addr to 0

	serv_addr.sin_family = AF_INET; // setting family
    serv_addr.sin_addr.s_addr = INADDR_ANY; // setting address
    serv_addr.sin_port = htons(portnumber); // setting portnumber

    if (bind(sock, (struct sockaddr *) &serv_addr, server_size) == -1) // Binding the serv_addr to the socket
    {
    	error("Error on binding"); // Error handling of binding 
    }

    client_size = sizeof(cli_addr); // finding the client size

    while(1)
    {
    	bzero(buffer, bufferSize); // Setting the buffer array to zero
    	n = recvfrom(sock,buffer,bufferSize,0, (struct sockaddr *) &cli_addr, &client_size); // receiving message from client and storing it in buffer
    	if (n < 0)
    	{
    		error("Error in recvform"); // Error handling of received message
    	}

    	cout << "Sending file" << endl; // Help to know where in the code we are 

		std::ifstream FileIn; // setting up our stream
		switch(buffer[0]) // checking if letter is U,u,L,l. 
		{
			case 'U':
			case 'u': // if u/U sending /proc/uptime
				fileName = "/proc/uptime"; 
				FileIn.open(fileName.c_str(), std::ios::in); // opening file
				if(FileIn.is_open())
				{
					cout << "Sending /proc/uptime" << endl;
					getline(FileIn, file); // storing /proc/uptime data in string file
				}
				else
				{
					error("Could not open file"); // Error Handling 
				}
	 			break;


			case 'L': // if L/l sending /proc/loadavg
			case 'l':
				fileName = "/proc/loadavg";
				FileIn.open(fileName.c_str(), std::ios::in); // opening file
				if(FileIn.is_open())
				{
					cout << "Sending /proc/loadavg" << endl;
					getline(FileIn, file); // storing /proc/loadavg data in string file
				}
				else
				{
					error("Could not open file"); // Error Handling
				}
	 			break;

			default:
					error("Wrong letter"); // Error Handling - shut down if not U/u or L/l
				break;
		}
		n = sendto(sock, file.c_str(), file.length()+1, 0, (struct sockaddr *) &cli_addr, client_size); // sending file information to client. 
	
		if(n < 0)
		{
			error("Error in sendTo"); // Error Handling
		}
		FileIn.close(); // closing our stream
		break;

    }
    cout << "closing socket" << endl;
    close(sock); // closing our socket. 

}   

   