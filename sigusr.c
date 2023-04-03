/**
 * Description: Modded program to extend signal handling that captures SIGCONT signal, output prints "received SIGCONT" and continues running (does not exit).
 * Author names: Aden Mengistie & Sofia Silva
 * Author emails: aden.mengistie@sjsu.edu, sofia.silva@sjsu.edu
 * Last modified date: 04/02/2023
 * Creation date: 04/02/2023
 **/

#include "apue.h"

static void	sig_usr(int);	/* one handler for both signals */

int
main(void)
{
	if (signal(SIGUSR1, sig_usr) == SIG_ERR)
		err_sys("can't catch SIGUSR1");
	if (signal(SIGUSR2, sig_usr) == SIG_ERR)
		err_sys("can't catch SIGUSR2");
	for ( ; ; )
		pause();
}

static void
sig_usr(int signo)		/* argument is signal number */
{
	if (signo == SIGUSR1)
		printf("received SIGUSR1\n");
	else if (signo == SIGUSR2)
		printf("received SIGUSR2\n");
	else if (signo == SIGCOUNT)
		printf("recieved SIGCONT\n");
		continue;
	else
		err_dump("received signal %d\n", signo);
}
