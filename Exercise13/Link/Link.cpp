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

	char message[size];
	message[0]=END;

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
						message[j+1]=buf[j];
					}

				message[i]=ESC;
				message[i+1]=ESC_END;
				break;

			case ESC:

				j=i;
				while(buf[j++]!='\0') 
					{}

				for(j;i<j;j--)
					{
						message[j+1]=buf[j];
					}
						
				message[i]=ESC;
				message[i]=ESC_ESC;
				break;

			default:

				break;
		}
	}

	//Sendes til den serielle port herfra?

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
	int i=0;

	char message[size];

	while(1)
	{	
		switch(buf[i])
			{
				case END:
				break;

				case ESC:
				switch(buf[i+1])
					{
						case ESC_END:
						message[i++]=END;
						break;

						case ESC_ESC:
						message[i++]=ESC;
						break;
					}
					
				default:
				message[i++]=buf[i];
				break;
			}
	}


}

} /* namespace Link */
