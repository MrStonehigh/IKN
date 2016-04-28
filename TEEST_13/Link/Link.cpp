#include <Link.h>
#include <cstdio>

namespace Link {

/**
  * Explicitet constructor for linklaget.
  * Åbner den serielle port og sætter baudraten, paritet og antal stopbits
  */
Link::Link(int bufsize)
{
	buffer = new char[(bufsize*2)];
	
    serialPort=v24OpenPort("/dev/ttyS1",V24_STANDARD);
    if ( serialPort==NULL )
    {
        fputs("error: sorry, open failed!\n",stderr);
        exit(1);
    }

    int rc=v24SetParameters(serialPort,V24_B115200,V24_8BIT,V24_NONE);
    if ( rc!=V24_E_OK )
    {
        fputs("error: setup of the port failed!\n",stderr);
        v24ClosePort(serialPort);
        exit(1);
    }

    rc=v24SetTimeouts(serialPort,5);
    if ( rc!=V24_E_OK )
    {
        fputs("error: setup of the port timeout failed!\n",stderr);
        v24ClosePort(serialPort);
        exit(1);
    }

    rc=v24FlushRxQueue(serialPort);
    if ( rc!= V24_E_OK )
    {
    	fputs("error: flushing receiverqueue\n", stderr);
    	v24ClosePort(serialPort);
    	exit(1);
    }

    rc=v24FlushTxQueue(serialPort);
    if ( rc!= V24_E_OK )
    {
    	fputs("error: flushing transmitterqueue\n", stderr);
    	v24ClosePort(serialPort);
    	exit(1);
    }
}

/**
  * Destructor for linklaget.
  * Lukker den serielle port
  */
Link::~Link()
{
	if(serialPort != NULL)
		v24ClosePort(serialPort);
	if(buffer != NULL)
		delete [] buffer;
}

/**
 * Bytestuffer buffer, dog maximalt det antal som angives af size
 * Herefter sendes de til modtageren via den serielle port
 *
 * @param buffer De bytes som skal sendes
 * @param size Antal bytes der skal sendes
 */
void Link::send(char buf[], short size)
{
	const unsigned char  END='A', ESC='B',  ESC_END='C',  ESC_ESC='D';
	int i=0,j=1;
	unsigned char message[2*size];

	//Sending startbit
	message[0]=END;

	while(message[i] != NULL)
	{
		
		switch(buf[i])
		{
			case END:
				message[j++]=ESC;
				message[j++]=ESC_END;
				i++;
				break;

			case ESC:
				message[j++]=ESC;
				message[j++]=ESC_ESC;
				i++;
				break;

			default:
				message[j++]=buf[i++];
				break;
		}
		printf("DEBUG SEND: %c\n", message[i]);
		v24Putc(serialPort, message[i]);
	}
	//printf("DEBUG SEND: last A send");
	//Sending stopbit
	v24Putc(serialPort,END);
		
	

}

/**
 * Modtager data fra den serielle port og debytestuffer data og gemmer disse i buffer.
 * Modtager højst det antal bytes som angives af size.
 * Husk kun at læse en byte ad gangen fra den serielle port.
 *
 * @param buffer De bytes som modtages efter debytestuffing
 * @param size Antal bytes der maximalt kan modtages
 * @return Antal bytes modtaget
 */
short Link::receive(char buf[], short size)
{
	const char  END='A', ESC='B',  ESC_END='C',  ESC_ESC='D';
	int i=0, rcvd=0, START_FLAG=0;

	char message, message_intern;
	int message_int, message_int_intern;


	while(size--)
	{	
		
		message_int=v24Getc(serialPort);

		//Check for error from v24Getc()
		if(message_int==-1)
		{
			fputs("Error: v24Getc\n", stderr);
		}
		//printf("DEBUG RECEIVE (int): %d\n",message_int);
		message=(char) message_int;		
		//printf("DEBUG RECEIVE (char): %c\n",message);

		//Ignoring startbit
		if(message==END && START_FLAG==0)
		{
			START_FLAG=1;
		}
		else
		{
			switch(message)
				{
					case END:
						return rcvd;
						break;

					case ESC:
						message_int_intern=v24Getc(serialPort);
						message_intern=(char) message_int_intern;
						switch(message)
							{
								
								case ESC_END:
									buf[i++]=END;
									rcvd++;
									break;

								case ESC_ESC:
									buf[i++]=ESC;
									rcvd++;
									break;

								default:
									fputs("Error debytestuffing packet",stderr);
									exit(1);
							}
						
					default:
						buf[i++]=message;
						rcvd++;
						break;
				}
		}
	}

	return rcvd;
}

} /* namespace Link */
