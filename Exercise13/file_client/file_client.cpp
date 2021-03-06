#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <Transport.h>
#include <lib.h>
#include <file_client.h>
#include <iostream>

/// <summary>
/// The BUFSIZE
/// </summary>
#define BUFSIZE 1000

/// <summary>
/// Initializes a new instance of the <see cref="file_client"/> class.
/// 
/// file_client metoden opretter en peer-to-peer forbindelse
/// Sender en forspørgsel for en bestemt fil om denne findes på serveren
/// Modtager filen hvis denne findes eller en besked om at den ikke findes (jvf. protokol beskrivelse)
/// Lukker alle streams og den modtagede fil
/// Udskriver en fejl-meddelelse hvis ikke antal argumenter er rigtige
/// </summary>
/// <param name='args'>
/// Filnavn med evtuelle sti.
/// </param>
file_client::file_client(int argc, char **argv)
{
	   	if(argc!=2)
			{
				error("Invalid amount of arguments");
			}
	transport= new Transport(BUFSIZE);
			
	receiveFile(argv[1],transport);

}

/// <summary>
/// Receives the file.
/// </summary>
/// <param name='fileName'>
/// File name.
/// </param>
/// <param name='transport'>
/// Transportlaget
/// </param>
void file_client::receiveFile (std::string fileName, Transport::Transport *transport)
{
	char * file_requested[]=filename;

	int fileSize=getFileSizeTCP(sockfd);
	if(fileSize == 0)
		{
			error("File doesnt exist");
			return;
		}
	std::ofstream FileIn;
	FileIn.open(fileName.c_str(),std::ios::binary|std::ios::out);
	std::cout << "Filesize: = " << fileSize << " Bytes" << endl;
	char buffer[BUFSIZE];

	std::cout << "Message in transfer" << endl;

	long rest = fileSize;

	while (rest>0)
	{
		int count =transport->recieve(buf,size);
		FileIn.write(buf,count);
		rest-=count;
	}

	FileIn.close();
	cout << "File Recieved "<< endl;
}		

/// <summary>
/// The entry point of the program, where the program control starts and ends.
/// </summary>
/// <param name='args'>
/// First argument: Filname
/// </param>
int main(int argc, char** argv)
{
	new file_client(argc, argv);
	
	return 0;
}
