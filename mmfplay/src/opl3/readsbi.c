
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>

#include "mmfplay.h"
#include "sbi.h"
#include "opl3.h"




static int ins_size = SBI_4OP_SIZE;


int read_sbi(unsigned char *buf, const int size)
{
	unsigned char *b_end;
	struct sbi *sbi;
	int type, n;

	b_end = buf + size;

	sbi = (struct sbi *)buf;
	if (cmp4(buf, '4', 'O', 'P', 0x1a)) {
		ins_size = SBI_4OP_SIZE;
	} else if (cmp4(buf, 'S', 'B', 'I', 0x1a)) {
		ins_size = SBI_2OP_SIZE;
	}

	assert(ins_size == SBI_4OP_SIZE);

	printf("#include \"opl3.h\"\n\n");
	printf("struct opl3_instrument opl3_ins[] = {\n");

	for (n = 0; buf < b_end; n++) {
		sbi = (struct sbi *)buf;
		
		printf("      { ");

		if (cmp4(buf, '4', 'O', 'P', 0x1a)) {
			type = SBI_4OP;
			/*opl3_ins[n].type = OPL3_TYPE_4OP;*/
			printf("OPL3_TYPE_4OP,");
		} else if (cmp4(buf, '2', 'O', 'P', 0x1a)) {
			type = SBI_2OP;
			/*opl3_ins[n].type = OPL3_TYPE_2OP;*/
			printf("OPL3_TYPE_2OP,");
		}

		printf ("\t/* %3d: %-20.20s */\n", n, sbi->name);

/* #define SET_OPL3(n,o,y,x) opl3_ins[n].op[o].x = sbi->A.y ## _ ## x */
#define SET_OPL3(n,o,y,x) printf("0x%02x", sbi->A.y ## _ ## x); 
#define _ printf(", ");

		printf("\t{ { ");
		SET_OPL3(n, 0, m, flg_mul); _;
		SET_OPL3(n, 0, m, ksl_tl); _;
		SET_OPL3(n, 0, m, ar_dr); _;
		SET_OPL3(n, 0, m, sl_rr); _;
		SET_OPL3(n, 0, m, ws);
		printf(" },\t/* OP1 */\n");

		printf("\t  { ");
		SET_OPL3(n, 1, c, flg_mul); _;
		SET_OPL3(n, 1, c, ksl_tl); _;
		SET_OPL3(n, 1, c, ar_dr); _;
		SET_OPL3(n, 1, c, sl_rr); _;
		SET_OPL3(n, 1, c, ws);
		printf(" },\t/* OP2 */\n");

		printf("\t  { ");
		SET_OPL3(n, 2, m, flg_mul); _;
		SET_OPL3(n, 2, m, ksl_tl); _;
		SET_OPL3(n, 2, m, ar_dr); _;
		SET_OPL3(n, 2, m, sl_rr); _;
		SET_OPL3(n, 2, m, ws);
		printf(" },\t/* OP3 */\n");

		printf("\t  { ");
		SET_OPL3(n, 3, c, flg_mul); _;
		SET_OPL3(n, 3, c, ksl_tl); _;
		SET_OPL3(n, 3, c, ar_dr); _;
		SET_OPL3(n, 3, c, sl_rr); _;
		SET_OPL3(n, 3, c, ws);
		printf(" } },\t/* OP4 */\n");

		/*opl3_ins[n].fb_algA = sbi->A.fb_alg;
		opl3_ins[n].fb_algB = sbi->B.fb_alg;*/

		printf("\t0x%02x, 0x%02x\n", sbi->A.fb_alg, sbi->B.fb_alg);
		printf("      },\n");

		buf += ins_size;
	}

	printf("};\n");

	return n;
}


unsigned char *load_file(char *name, int *s)
{
	FILE *f;
	struct stat st;
	unsigned char *buffer;

	if ((f = fopen(name, "r")) == NULL)
		return NULL;

	fstat(fileno(f), &st);
	buffer = malloc(st.st_size);
	fread(buffer, 1, st.st_size, f);
	fclose(f);
	*s = st.st_size;

	return buffer;
}

	
int load_instruments(char *f)
{
	unsigned char *buf;
	int n, size;

	buf = load_file(f, &size);
	n = read_sbi(buf, size);

	return n;
}



int main(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <o3file>\n", argv[0]);
		exit(1);
	}
	load_instruments(argv[1]);

	return 0;
}

