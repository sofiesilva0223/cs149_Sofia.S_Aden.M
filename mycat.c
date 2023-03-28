/**
 * Description: The modified mycat.c program to write to stderr the number of bytes that were read from stdin each time.
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 03/27/2023
 * Creation date: 03/27/2023
 **/
#include "apue.h"

#define	BUFFSIZE	4096

int
main(void)
{
	int		n;
	char	buf[BUFFSIZE];

	while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0){
		fprintf(stderr,"%d bytes from stdin ", n);
		if (write(STDOUT_FILENO, buf, n) != n){
			err_sys("write error");
		}
	}
	if (n < 0){
		err_sys("read error");
	}

	exit(0);
}
