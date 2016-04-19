#include <iostream>
#include <cstring>
#include <cstdio>
#include <Transport.h>
#include <TransConst.h>

#define DEFAULT_SEQNO 2

namespace Transport
{
	/// <summary>
	/// Initializes a new instance of the <see cref="Transport"/> class.
	/// </summary>
	Transport::Transport (short BUFSIZE)
	{
		link = new Link::Link(BUFSIZE+ACKSIZE);
		checksum = new Checksum();
		buffer = new char[BUFSIZE+ACKSIZE];
		seqNo = 0;
		old_seqNo = DEFAULT_SEQNO;
		errorCount = 0;
	}

	/// <summary>
	/// Delete dynamics attribute before delete this object.
	/// </summary>
	Transport::~Transport()
	{
		if(link) delete link;
		if(checksum) delete checksum;
		if(buffer) delete [] buffer;
	}
	
	/// <summary>
	/// Receives the ack.
	/// </summary>
	/// <returns>
	/// The ack.
	/// </returns>
	bool Transport::receiveAck()
	{
		char buf[ACKSIZE];
		short size = link->receive(buf, ACKSIZE);
		if (size != ACKSIZE) return false;
		if(!checksum->checkChecksum(buf, ACKSIZE) ||
				buf[SEQNO] != seqNo ||
				buf[TYPE] != ACK)
			return false;
			
		seqNo = (buffer[SEQNO] + 1) % 2;
			
		return true;
	}

	/// <summary>
	/// Sends the ack.
	/// </summary>
	/// <param name='ackType'>
	/// Ack type.
	/// </param>
	void Transport::sendAck (bool ackType)
	{
		char ackBuf[ACKSIZE];
		ackBuf [SEQNO] = (ackType ? buffer[SEQNO] : (buffer[SEQNO] + 1) % 2);
		ackBuf [TYPE] = ACK;
		checksum->calcChecksum (ackBuf, ACKSIZE);

		link->send(ackBuf, ACKSIZE);
	}

	/// <summary>
	/// Send the specified buffer and size.
	/// </summary>
	/// <param name='buffer'>
	/// Buffer.
	/// </param>
	/// <param name='size'>
	/// Size.
	/// </param>
	void Transport::send(const char buf[], short size)
	{
		while(receiveAck() == false)
		{
			for(int i = 0; i<size; i++)
			{
				buffer[4+i] = buf[i];
			}

			buffer[SEQNO] = seqNo;  //SEQNO = 2
			buffer[TYPE] = DATA;  // TYPE = 3 and DATA = 0
			checksum->calcChecksum(buffer, size+4);
		}
	}

	/// <summary>
	/// Receive the specified buffer.
	/// </summary>
	/// <param name='buffer'>
	/// Buffer.
	/// </param>
	short Transport::receive(char buf[], short size)
	{
		short n = 0;

		n = link->receive(buffer,size);

		while(receiveAck() == true)
		{
			 if(checksum->checkChecksum(buf, n) == true)
			 {
			 	for(int i = 4; i<size; i++)
			 	{
			 		buffer[i-4] = buf[i];
			 	}
			 }
			 else
		
			return n-4; 	 				
		}
		
	}
}


