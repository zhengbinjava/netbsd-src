/* $NetBSD: ras3.c,v 1.9 2008/04/28 20:23:07 martin Exp $ */

/*-
 * Copyright (c) 2002 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ras.h>
#include <sys/time.h>
#include <sys/wait.h>

#define COUNT	10

volatile int handled = 0;
volatile int count = 0;
struct itimerval itv;

void handler(int);

void
handler(int sig)
{

	handled++;
}

RAS_DECL(main);

int
main(int argc, char *argv[])
{
	int rv;
	char *const args[] = { argv[0], "1", NULL };

        signal(SIGVTALRM, handler);

        itv.it_interval.tv_sec = 0;
        itv.it_interval.tv_usec = 0;
        itv.it_value.tv_sec = 10;
        itv.it_value.tv_usec = 0;
        setitimer(ITIMER_VIRTUAL, &itv, NULL);

	if (argc != 2) {
		if (rasctl(RAS_ADDR(main), RAS_SIZE(main), RAS_INSTALL) < 0) {
			if (errno == EOPNOTSUPP) {
				printf("RAS is not supported on this "
				    "architecture\n");
				return 0;
			}
			return (1);
		}
		if (fork() != 0) {
			wait(&rv);
			return (WEXITSTATUS(rv) == 0);
		}
		if (execvp(argv[0],args) < 0) {
			printf("exec failed\n");
			return (0);
		}
	}
	
	RAS_START(main);
	count++;
	if (count > COUNT)
		goto end;

	while (!handled) {
		continue;
	}
end:
	RAS_END(main);

	return (handled != 0);
}
