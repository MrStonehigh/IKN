
/*
 * lib.c
 *
 *  Created on: Sep 8, 2012
 *      Author: Lars Mortensen
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/stat.h>
#include <lib.h>

/**
 * Udskriver fejlmeddelelse og stoppe programmet
 *
 * @param msg Fejlmeddelelse
 */
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

/**
 * Udtrækker et filnavn fra en tekststreng
 *
 * @param fileName Filnavn incl. evt. stinavn
 * @return Kun filnavn
 */
char *extractFileName(const char *fileName)
{
	return (strrchr(fileName, '/') ? (strrchr(fileName, '/'))+1 : (char *)fileName);
}

/**
 * Se om filen fileName eksisterer
 *
 * @param fileName Filnavn
 * @return Størrelsen på filen, 0 hvis den ikke findes
 */
long check_File_Exists(const char *fileName)
{
	struct stat sts;
	if ((stat (fileName, &sts)) == -1)
		return 0;

	return sts.st_size;
}