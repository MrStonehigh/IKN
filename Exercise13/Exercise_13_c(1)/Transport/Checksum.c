#include <string.h>
#include <stdio.h>
#include <Checksum.h>
#include <TransConst.h>

unsigned long checksum(unsigned char buf[], short size)
{
	int i = 0, length = size;
	unsigned long sum = 0;
	while (length > 0) 
	{
   		sum	+= (buf[i++]&0xff) << 8;
		if ((--length)==0) break;
		sum += (buf[i++]&0xff);
   		--length;
		}
	return (~((sum & 0xFFFF)+(sum >> 16)))&0xFFFF;
}

char checkChecksum(char buf[], short size)
{
	int idx;
	unsigned char buffer[size-CHKSUMSIZE];

	memcpy(buffer, buf+CHKSUMSIZE, size-CHKSUMSIZE);

	return( checksum(buffer, size-CHKSUMSIZE) == ((((unsigned char)buf[CHKSUMHIGH]) << 8) | ((unsigned char)buf[CHKSUMLOW])));
}

void calcChecksum (char buf[], short size)
{
	unsigned char buffer[size-CHKSUMSIZE];
	unsigned long sum = 0;

	memcpy(buffer, buf+CHKSUMSIZE, size-CHKSUMSIZE); 
	sum = checksum(buffer, size-CHKSUMSIZE);
	buf[CHKSUMHIGH] = ((sum >> 8) & 255);
	buf[CHKSUMLOW] = (sum & 255);
}