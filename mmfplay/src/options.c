
#include "mmfplay.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef HAS_GETOPT_LONG
#include <getopt.h>
#else
#include "getopt.h"
#endif

struct options opt;

static void help(int argc, char **argv)
{
	printf(
"MMFplay version " VERSION " (built " __DATE__ " " __TIME__ " by " USERNAME ")\n"
"Copyright (C) 2004 Claudio Matsuoka\n"
#ifndef HAS_GETOPT_LONG
"Portions Copyright (C) 2000 The NetBSD Foundation, Inc. All rights reserved.\n"
#endif
	"\n");

	printf(
"Usage: %s [-hPpsV -Sn -Ddevice] <mmf_file>\n"
"	-D --device dev	Choose output device\n"
"	-h --help	Show list of command line options\n"
"	-P --perc	Play only percussive channels\n"
"	-p --noperc	Don't play percussive channels\n"
"	-S --solo n	Solo track (range 1-16)\n"
"	-s --show	Show structure of MMF file\n"
"	-V --version	Show version\n"
	, argv[0]);
}


int parse_cli (int argc, char **argv)
{
	int o, optidx = 0;

#define OPTIONS "D:hPpS:sV"
	static struct option lopt[] = {
		{ "device",		1, 0, 'D' },
		{ "help",		0, 0, 'h' },
		{ "perc",		0, 0, 'P' },
		{ "noperc",		0, 0, 'p' },
		{ "solo",		1, 0, 'S' },
		{ "show",		0, 0, 's' },
		{ "version",		0, 0, 'V' }
	};

	/* Set defaults */
	memset (&opt, 0, sizeof (struct options));
	opt.mode = MMFPLAY_PLAY;
	opt.device = "fm"; 
	opt.solotrk = 0;

	while ((o = getopt_long(argc, argv, OPTIONS, lopt, &optidx)) != -1) {
		switch (o) {
		case 'D':
			opt.device = strdup(optarg);
			break;
		case 'h':
			help(argc, argv);
			exit(0);
		case 'P':
			opt.perc = 1;
			break;
		case 'p':
			opt.perc = -1;
			break;
		case 'S':
			opt.solotrk = strtoul(optarg, NULL, 0);
			break;
		case 's':
			opt.mode = MMFPLAY_SHOW;
			break;
		case 'V':
			printf("mmfplay " VERSION "\n");
			exit(0);
		default:
			exit(-1);
		}
	}

	return 0;
}


