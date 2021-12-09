
#include <stdio.h>
#include <stdarg.h>

#include "tree.h"

static int outline = 0;
static int branch[10];

void print_data(unsigned char *buf, int size)
{
	while (size--) printf("%c", *buf++);
	printf("\n");
}

unsigned char *dump_data(unsigned char *buf, int size)
{
	while (size--) printf("%02x ", *buf++);
	printf("\n");
	return buf;
}

void print_tree(int leaf)
{
	int i;

	for (i = 1; i <= (outline - (leaf ? 1 : 0)); i++) {
		if (branch[i])
			printf("|   ");
		else
			printf("    ");
	}

	if (leaf)
		printf("+---");
}

int print_text(char *fmt, ...)
{
	va_list v;
	int i;

	print_tree(0);

	va_start(v, fmt);
	i = vfprintf(stdout, fmt, v);
	va_end(v);

	return i;
}

int print_leaf(int last, char *fmt, ...)
{
	va_list v;
	int i;

	if (last)
		branch[outline] = 0;

	print_tree(1);

	va_start(v, fmt);
	i = vfprintf(stdout, fmt, v);
	va_end(v);

	return i;
}

void new_branch()
{
	outline++;
	branch[outline] = 1;
}

void end_branch()
{
	outline--;
}

