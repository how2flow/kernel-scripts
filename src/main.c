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

#define HASH_ERROR -1
#define HASH_CROSSCOMPILE 0
#define HASH_INSTALL 1
#define HASH_FTRACE 2
#define HASH_LOSETUP 3

static int options;
const char* sopts = "f:h";
static struct option lopts[] = {
	{"function", required_argument, 0, 'f'},
    {"help", 0, 0, 'h'},
    {NULL, 0, 0, 0}
};

typedef struct {
	char *key;
	int value;
} ftable;
static ftable functionKeys[] = {
	// cross-compile
	{"crosscompile", HASH_CROSSCOMPILE}, {"cc", HASH_CROSSCOMPILE},
	// native-install
	{"install", HASH_INSTALL}, {"inst", HASH_INSTALL},
	// native-install
	{"ftrace", HASH_FTRACE}, {"ft", HASH_FTRACE},
	// losetup
	{"losetup", HASH_LOSETUP}, {"lo", HASH_LOSETUP}
};

#define NKEYS (sizeof(functionKeys)/sizeof(ftable))

int fkey(char *key)
{
	int ret;

	for (int i = 0; i < NKEYS; i++) {
		ftable *tmp = &functionKeys[i];
		if (strcmp(tmp->key, key) == 0)
			return tmp->value;
	}

	return HASH_ERROR;
}

int functions(int hash) {
	int ret;

	switch (hash) {
		case HASH_CROSSCOMPILE:
			if ((ret = system("/usr/bin/bash -c do_crosscompile"))) {
				fprintf(stderr, "system function error!\n");
				return ret;
			}
			break;
		case HASH_INSTALL:
			if ((ret = system("/usr/bin/bash -c do_nativeinstall"))) {
				fprintf(stderr, "system function error!\n");
				return ret;
			}
			break;
		case HASH_FTRACE:
			if ((ret = system("/usr/bin/bash -c do_ftrace"))) {
				fprintf(stderr, "system function error!\n");
				return ret;
			}
			break;
		case HASH_LOSETUP:
			if ((ret = system("/usr/bin/bash -c do_losetup"))) {
				fprintf(stderr, "system function error!\n");
				return ret;
			}
			break;
		case HASH_ERROR:
				fprintf(stderr, "Invalid option! refert to '--help'\n");
				ret = -1;
			break;
	}

	return ret;
}

void Usage()
{
	printf("Usage: kernelsc [OPTION] ...\n");
	printf("Utils for kernel\n");
	printf("(kernel install, kernel debuging, kernel management etc ...)\n");
	printf("\n");
	printf("[OPTION]\n");
	printf("-f, --function: select and run kernel util function.\n");
	printf("\n");
	printf("                crosscompile|cc: used when kernel build target and build environment are different.\n");
	printf("                              before using it, move to the root path of the kernel source.\n\n");
	printf("                              Input architecture: x86|arm|arm64 ...\n");
	printf("                              Input config: defconfig|bcm2835_defconfig|odroidg12_defconfig ...\n");
	printf("                              Input targets: zImage|modules|dtbs|all ...\n");
	printf("\n");
	printf("                install|inst: kernel build and install.\n");
	printf("                         before using it, move to the root path of the kernel source.\n");
	printf("\n");
	printf("                ftrace|ft: during the interaction, you must write the exact name of the function\n");
	printf("                        you want to track within the kernel source. An untraceable function exists.\n");
	printf("                        need 'root' or 'sudo' privileges.\n");
	printf("\n");
	printf("                losetup|lo: losetup file in loop device.\n");
	printf("                         need 'root' or 'sudo' privileges.\n");
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
		printf("$ kernel --help\n");
		return -1;
	}

	while ((options =  getopt_long(argc, argv, sopts, lopts, NULL)) != -1) {
		switch (options) {
			case 'f':
				functions(fkey(optarg));
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
