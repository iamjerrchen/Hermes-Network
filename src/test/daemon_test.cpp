/* daemon_test.cpp
 *	This is an example from stackoverflow to daemonize a process,
 *	writes a few log messages, sleeps 20 seconds, and terminates
 *	afterwards.
 */

#include "../main/daemonize.h"

#include <syslog.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	setup_daemon();
	while(1)
	{
		//TODO: insert daemon code here
		syslog(LOG_NOTICE, "First daemon started.");
		sleep(20);
		break;
	}

	syslog(LOG_NOTICE, "First daemon terminated.");
	teardown_daemon();

	return EXIT_SUCCESS;
}
