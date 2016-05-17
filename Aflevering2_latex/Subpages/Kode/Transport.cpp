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
		if (size != ACKSIZE) 
		{
			std::cout << "Received bytes WHAT???: " << size << std::endl;
			return false;
		}
		if(!checksum->checkChecksum(buf, ACKSIZE) ||
				buf[SEQNO] != seqNo ||
				buf[TYPE] != ACK)

			{
			//std::cout << __PRETTY_FUNCTION__ << "ACK FAILURE: " << "buf[SEQNO] != seqNo - " << (int)buf[SEQNO] << ", " << (int)seqNo << "; chksum - " << 
			//		checksum->checkChecksum(buf, ACKSIZE) << "; buf[TYPE] != ACK - " << (int)buf[TYPE] << ", " << (int)ACK << std::endl;
				return false;
			}

		//std::cout << __PRETTY_FUNCTION__ << " -SeqNo:" << 
		//	(int)buf[SEQNO] << " (" << (int)old_seqNo << "), Type:" << (int)buf[TYPE] << std::endl;

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

		//std::cout << __PRETTY_FUNCTION__ << " -SeqNo:" << 
		//	(int)ackBuf[SEQNO] << " (" << (int)old_seqNo << "), Type:" << (int)ackBuf[TYPE] << std::endl;

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
		int count = 0;
		for(int i = 0; i<size; i++)
		{
			buffer[HDRSIZE+i] = buf[i];
		}

		buffer[SEQNO] = seqNo;  //SEQNO = 2
		buffer[TYPE] = DATA;  // TYPE = 3 and DATA = 0
		checksum->calcChecksum(buffer, size+HDRSIZE);
		do
		{
			link->send(buffer, size+HDRSIZE);
			count++;
			if(count == 3)
			{
			break;
			}
		}while(!receiveAck());
		old_seqNo = DEFAULT_SEQNO;
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
		bool receiveOk = false;

		for(;;)
		{
			n = link->receive(buffer,size+HDRSIZE);
			if(n>0)
			{
				receiveOk = checksum->checkChecksum(buffer,n);
				sendAck(receiveOk);
			}
			if(old_seqNo != buffer[SEQNO])
			{
				std::cout << "Sender data til applikationlag" << std::endl;
				memcpy(buf, buffer+HDRSIZE, n-HDRSIZE);
				std::cout << "Kopieret..." << std::endl;
				break;
			}
		}
		old_seqNo = buffer[SEQNO];
		return n-HDRSIZE;
	}
}