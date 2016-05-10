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

  /* rc=v24SetTimeouts(serialPort,5);
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
	const unsigned char  DELIMITER='A', ESC='B',  ESC_END='C',  ESC_ESC='D';

	//Sending startbit
	v24Putc(serialPort, DELIMITER);

	for(int i = 0; i < size; ++i)
	{
		if(buf[i] == DELIMITER)
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

	v24Putc(serialPort, DELIMITER);

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

	char message, message_next;
	int message_int, message_int_next;


	//for(int i=0; i<size; i++)
	while(size--)
	{	
		
		message_int=v24Getc(serialPort);

		//Check for error from v24Getc()
		if(message_int==-1)
		{
			//fputs("Error: v24Getc\n", stderr);
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
						message_int_next=v24Getc(serialPort);
						message_next=(char) message_int_next;
						//printf("DEBUG RECEIVE (char_next\n): %c\n",message_next);
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
