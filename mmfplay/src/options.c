
#include "mmfplay.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

struct options opt;

static void help(int argc, char **argv)
{
	fprintf (stdout,
"Usage: %s [-Vhs] <mmf_file>\n"
"	-h --help	Show list of command line options\n"
"	-s --show	Show structure of MMF file\n"
"	-V --version	Show version\n"
	, argv[0]);
}


int parse_cli (int argc, char **argv)
{
	int o, optidx = 0;

#define OPTIONS "Vhs"
	static struct option lopt[] = {
		{ "version",		0, 0, 'V' },
		{ "help",		0, 0, 'h' },
		{ "show",		0, 0, 's' }
	};

	/* Set defaults */
	memset (&opt, 0, sizeof (struct options));
	opt.mode = MMFPLAY_PLAY;

	while ((o = getopt_long(argc, argv, OPTIONS, lopt, &optidx)) != -1) {
		switch (o) {
		case 'V':
			printf("mmfplay " VERSION "\n");
			exit(0);
		case 's':
			opt.mode = MMFPLAY_SHOW;
			break;
		case 'h':
			help(argc, argv);
			exit(0);
		default:
			exit(-1);
		}
	}

	return 0;
}


