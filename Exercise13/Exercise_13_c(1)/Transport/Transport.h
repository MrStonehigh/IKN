#ifndef __TRANSPORT_H__
#define __TRANSPORT_H__

#include <Link.h>
#include <Checksum.h>

/// <summary>
/// Transport.
/// </summary>
void openTransport(short BUFSIZE);
void closeTransport();
void sendTransport(const char buf[], short size);
short receiveTransport(char buf[], short size);

#endif /* __TRANSPORT_H__ */
