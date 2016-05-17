#include <Link.h>
#include <cstdio>
#include <iostream>

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

    /*rc=v24SetTimeouts(serialPort,5);
    if ( rc!=V24_E_OK )
    {
        fputs("error: setup of the port timeout failed!\n",stderr);
        v24ClosePort(serialPort);
        exit(1);
    }*/

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
void Link::send(const char buf[], short size)
{
	const unsigned char  END='A', ESC='B',  ESC_END='C',  ESC_ESC='D';
	v24FlushRxQueue(serialPort);
	//Sending startbit
	v24Putc(serialPort, END);

	for(int i = 0; i < size; ++i)
	{
		if(buf[i] == END)
		{
			v24Putc(serialPort, ESC);
			v24Putc(serialPort, ESC_END);
		} 
		else if(buf[i] == ESC) 
		{
			v24Putc(serialPort, ESC);
			v24Putc(serialPort, ESC_ESC);
		} 
		else
			v24Putc(serialPort, buf[i]);

	}

	v24Putc(serialPort, END);

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
	int i=0, START_FLAG=0;
	short rcvd=0;
	size=size*2;

	char message, message_next;
	//int message_int, message_int_next;
	v24FlushTxQueue(serialPort);

	while(size--)
	{	
		message=(char)v24Getc(serialPort);

		//Typecasting int to char
		//message=(char) message_int;	
		//std::cout << "\n link message: " << message	<< std::endl;
		//Ignoring startbit
		if(message==END && START_FLAG==0)
		{
			START_FLAG=1;
		}

		else if(message==END && START_FLAG==1)
		{
			START_FLAG=0;
			return rcvd;
		}

		/*if(message==ESC)
		{
			
		}
		*/
		else
		{
			switch(message)
				{
					/*case END:
						return rcvd;
						break;*/

					case ESC:
						message_next=(char)v24Getc(serialPort);
						//message_next=(char) message_int_next;
						switch(message_next)
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
									fputs("Error debytestuffing packet\n",stderr);
									exit(1);
							}
						break;
						
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
