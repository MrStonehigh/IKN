#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <Transport.h>
#include <lib.h>
#include <file_server.h>
#include <Link.h>

/// <summary>
/// The BUFSIZE
/// </summary>
#define BUFSIZE 10

/// <summary>
/// Initializes a new instance of the <see cref="file_server"/> class.
/// </summary>
file_server::file_server ()
{
	Transport::Transport transport(BUFSIZE);
	const char tester[]={'A','B','C'};
	printf("Sending message from app-layer\n");

	transport.send(tester,sizeof tester);
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
