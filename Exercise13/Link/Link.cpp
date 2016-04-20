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
	char const END='A', ESC='B',  ESC_END='C',  ESC_ESC='D';
	int i=0,j=0,c=0;


	v24Write(serialPort, END);

	while(size--)
	{
		switch(buf[i])
		{
			case END:
				
				j=i;
				while(buf[j++]!='\0') 
					{}

				for(j;i<j;j--)
					{
						buf[j+1]=buf[j];
					}
					s
				buf[i]=ESC;
				buf[i+1]=ESC_END;
				break;

			case ESC:

				j=i;
				while(buf[j++]!='\0') 
					{}

				for(j;i<j;j--)
					{
						buf[j+1]=buf[j];
					}
						
				buf[i]=ESC;
				buf[i+1]=ESC_ESC;
				break;

			default:

				break;
		}
	}

	v24Write(serialPort, buf, size);

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
	char const END='A', ESC='B',  ESC_END='C',  ESC_ESC='D';
	int i=0, rcvd=0;

	char* message[size];
	v24Read(serialPort, Message, size);


	while(1)
	{	
		switch(message[i])
			{
				case END:
					return rcvd;
					break;

				case ESC:
				switch(message[i+1])
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
							error("Error debytestuffing packet");
					}
					
				default:
					buf[i++]=message[i];
					rcvd++;
					break;
				}
	}

	return rcvd;
}

} /* namespace Link */
