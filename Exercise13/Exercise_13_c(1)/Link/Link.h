
#ifndef __LINKLAGET_H__
#define __LINKLAGET_H__

#include <ezV24/ezV24.h>

#define DELIMITER 'A'

void openLink(int bufsize );
void closeLink();
void sendLink(char [], short size);
short receiveLink(char buf[]);

#endif /* LINKLAGET_H_ */
