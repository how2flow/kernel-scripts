/*
 * Utils for kernel program
 *
 * Copyright (C) 2022 Steve Jeong <how2soft@gmail.com>
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int options;
const char* sopts = "f:h";
static struct option lopts[] = {
	{"function", required_argument, 0, 'f'},
    {"help", 0, 0, 'h'},
    {NULL, 0, 0, 0}
};

void run(char *optarg)
{
	if (!strcmp(optarg, "crosscompile")) {
		system("/usr/bin/bash -c do_crosscompile");
	}
	else if (!strcmp(optarg, "ftrace")) {
		system("/usr/bin/bash -c do_ftrace");
	}
	else if (!strcmp(optarg, "install")) {
		system("/usr/bin/bash -c do_nativeinstall");
	}
	else if (!strcmp(optarg, "losetup")) {
		system("/usr/bin/bash -c do_losetup");
	}
	else {
		printf("Invalid function\n");
	}
}

void Usage()
{
	printf("Usage: kernel [OPTION] ...\n");
	printf("Utils for kernel\n");
	printf("(kernel install, kernel debuging, kernel management etc ...)\n");
	printf("\n");
	printf("[OPTION]\n");
	printf("-f, --function: select and run kernel util function.\n");
	printf("\n");
	printf("                crosscompile: used when kernel build target and build environment are different.\n");
	printf("                              before using it, move to the root path of the kernel source.\n");
	printf("\n");
	printf("                ftrace: during the interaction, you must write the exact name of the function\n");
	printf("                        you want to track within the kernel source. An untraceable function exists.\n");
	printf("\n");
	printf("                install: kernel build and install.\n");
	printf("                         before using it, move to the root path of the kernel source.\n");
	printf("\n");
	printf("                losetup: losetup file in loop device.\n");
	printf("\n");
	printf("-h, --help: print Usage.\n");
	printf("\n\n");
	printf("[e.g]\n");
	printf("  $ kernel -f crosscompile\n");
}

int main(int argc, char *argv[])
{
	if (argc == 1) {
		printf("Need options. refer to run with \"--help\".\n");
		printf("\n");
		printf("$ kernelscripts --help\n");
		return -1;
	}

	while ((options =  getopt_long(argc, argv, sopts, lopts, NULL)) != -1) {
		switch (options) {
			case 'f':
				run(optarg);
				break;
			case 'h':
				Usage();
				break;
			case '?':
				/* already printed err message. */
				break;
		}
	}

	return 0;
}
