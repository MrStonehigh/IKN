#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <Transport.h>
#include <lib.h>
#include <file_client.h>
#include <iostream>
#include <Link.h>
using namespace std;
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
	Transport::Transport transport(BUFSIZE);// = new Transport(BUFSIZE);
	cout << "\n argv 0: " << argv[0] << "\n argv 1: " << argv[1]<<  "\n argc: " << argc << endl;
	receiveFile(argv[1],&transport);
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
	char rcv[BUFSIZE];
	transport->send(fileName.c_str(), fileName.length() +1);

	bzero(rcv,BUFSIZE);
	long n=transport->receive(rcv, BUFSIZE);

	long fileSize=atol(rcv);
	if(fileSize == 0)
		{
		error("File doesnt exist");
		}
	cout << "Filesize: = " << fileSize << " Bytes" << endl;

	std::ofstream FileIn;
	FileIn.open(fileName.c_str(),std::ios::binary|std::ios::out);

	long rest=fileSize;
	bzero(rcv,BUFSIZE);//fylder rcv med 0

	while(rest>0)
	{
		int count=transport->receive(rcv, BUFSIZE);
		cout << "1" << endl;
		FileIn.write(rcv,count);
		cout << "2" << endl;
		rest-=count;
		cout << rest << endl;
	}

	char buffer[BUFSIZE];
	cout << "File received" << endl;
	FileIn.close();
}		

/// <summary>
/// The entry point of the program, where the program control starts and ends.
/// </summary>
/// <param name='args'>
/// First argument: Filname
/// </param>
int main(int argc, char** argv)
{
	if(argc!=2)
	{
		error("Invalid amount of arguments");
	}
	new file_client(argc, argv);

	cout << "\n argv 0: " << argv[0] << "\n argv 1: " << argv[1] << "\n argc: " << argc << endl;
	return 0;
}
