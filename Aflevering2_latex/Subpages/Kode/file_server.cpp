#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <Transport.h>
#include <lib.h>
#include <file_server.h>

/// <summary>
/// The BUFSIZE
/// </summary>
#define BUFSIZE 1000

using namespace std;

/// <summary>
/// Initializes a new instance of the <see cref="file_server"/> class.
/// </summary>
file_server::file_server ()
{
	Transport::Transport transport(BUFSIZE);
	char buffer[BUFSIZE];
	
	bzero(buffer,BUFSIZE); // Saetter alle vaerdier til 0 i array. 
	int n = transport.receive(buffer, BUFSIZE); 
	if(n<0)
	{
		cout << "Nothing received" << endl;
	}
	string fileName = string(buffer);
	cout << "Getting Filename " << "'" << fileName << "'" << endl;
	
	long fileSize = check_File_Exists(fileName);
	if(fileSize == 0)
	{
		error("File does not exist");
	}
	cout << "File does exist" << endl;
	sendFile(fileName, fileSize, &transport);
	fileName = "";
	cout << "File send" << endl;
}

/// <summary>
/// Sends the file.
/// </summary>
/// <param name='fileName'>
/// File name.
/// </param>
/// <param name='fileSize'>
/// File size.
/// </param>
/// <param name='transport'>
/// Transport lag.
/// </param>
void file_server::sendFile(std::string fileName, long fileSize, Transport::Transport *transport)
{
	char buffer[BUFSIZE];
	sprintf(buffer, "%ld",fileSize);
	transport->send(buffer, strlen(buffer));

	bzero(buffer,BUFSIZE);

	std::ifstream FileIn; // setting up our stream
	FileIn.open(fileName.c_str(), std::ios::in | std::ios::binary);
	
	if(fileSize > 0)
	{
		if(FileIn.is_open()) // checking if file is open. 
	 	{
	 		long rest = fileSize;
	 		while (rest > 0)
	 		{
	 		   	int count = FileIn.readsome(buffer,BUFSIZE); // Reading file 
	 		   	if(count >= 0)
	 		   	{
					transport->send(buffer,count); // sending file
					rest -= count;
	 		    }
	 		}
	 	}
	 	else
	 	error("Could not open file");
		
	}
	FileIn.close();
}

/// <summary>
/// The entry point of the program, where the program control starts and ends.
/// </summary>
/// <param name='args'>
/// The command-line arguments.
/// </param>
int main(int argc, char **argv)
{
	new file_server();
	
	return 0;
}
