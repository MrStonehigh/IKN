#include <string.h>
#include <stdio.h>
#include <Transport.h>
#include <TransConst.h>

#define DEFAULT_SEQNO 2

static char *buffer;
static unsigned char seqNo;
static unsigned char old_seqNo;
static short errorCount;

char receiveAck();
void sendAck (char ackType);

/// <summary>
/// Initializes a new instance of the <see cref="Transport"/> class.
/// </summary>
void openTransport (short BUFSIZE)
{
	buffer = (char *) malloc(BUFSIZE+ACKSIZE);
	seqNo = 0;
	old_seqNo = DEFAULT_SEQNO;
	errorCount = 0;
	openLink(BUFSIZE+ACKSIZE);
}

/// <summary>
/// Delete dynamics attribute before delete this object.
/// </summary>
void closeTransport()
{
	if(buffer) free(buffer);
	closeLink();
}

/// <summary>
/// Receives the ack.
/// </summary>
/// <returns>
/// The ack.
/// </returns>
char receiveAck()
{
	char buf[ACKSIZE];
	short size = receiveLink(buf);
	if (size != ACKSIZE) return 0;
	if(!checkChecksum(buf, ACKSIZE) ||
		buf[SEQNO] != seqNo ||
		buf[TYPE] != ACK)
	{
		return 0;
	}
			
	seqNo = (char)((buffer[SEQNO]+1) % 2);
	
	return 1;
}

/// <summary>
/// Sends the ack.
/// </summary>
/// <param name='ackType'>
/// Ack type.
/// </param>
void sendAck (char ackType)
{
	char ackBuf[ACKSIZE];

	ackBuf [SEQNO] = (ackType ? buffer[SEQNO] : (char)((buffer[SEQNO]+1) % 2));
	ackBuf [TYPE] = ACK;
	calcChecksum (ackBuf, ACKSIZE);

	sendLink(ackBuf, ACKSIZE);
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
void sendTransport(const char buf[], short size)
{
	// TO DO Your own code
}

/// <summary>
/// Receive the specified buffer.
/// </summary>
/// <param name='buffer'>
/// Buffer.
/// </param>
short receiveTransport(char buf[], short size)
{
	// TO DO Your own code
}
