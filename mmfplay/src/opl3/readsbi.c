
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
	int n;

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
			printf("OPL3_TYPE_4OP,");
		} else if (cmp4(buf, '2', 'O', 'P', 0x1a)) {
			printf("OPL3_TYPE_2OP,");
		} else {
			printf("-1,");
		}

		printf ("\t/* %3d: %-20.20s */\n", n, sbi->name);

		printf("\t{ { ");
		printf("0x%02x, ", sbi->A.m_flg_mul);
		printf("0x%02x, ", sbi->A.m_ksl_tl);
		printf("0x%02x, ", sbi->A.m_ar_dr);
		printf("0x%02x, ", sbi->A.m_sl_rr);
		printf("0x%02x", sbi->A.m_ws);
		printf(" },\t/* OP1 */\n");

		printf("\t  { ");
		printf("0x%02x, ", sbi->A.c_flg_mul);
		printf("0x%02x, ", sbi->A.c_ksl_tl);
		printf("0x%02x, ", sbi->A.c_ar_dr);
		printf("0x%02x, ", sbi->A.c_sl_rr);
		printf("0x%02x", sbi->A.c_ws);
		printf(" },\t/* OP2 */\n");

		printf("\t  { ");
		printf("0x%02x, ", sbi->B.m_flg_mul);
		printf("0x%02x, ", sbi->B.m_ksl_tl);
		printf("0x%02x, ", sbi->B.m_ar_dr);
		printf("0x%02x, ", sbi->B.m_sl_rr);
		printf("0x%02x", sbi->B.m_ws);
		printf(" },\t/* OP3 */\n");

		printf("\t  { ");
		printf("0x%02x, ", sbi->B.c_flg_mul);
		printf("0x%02x, ", sbi->B.c_ksl_tl);
		printf("0x%02x, ", sbi->B.c_ar_dr);
		printf("0x%02x, ", sbi->B.c_sl_rr);
		printf("0x%02x", sbi->B.c_ws);
		printf(" } },\t/* OP4 */\n");

		printf("\t0x%02x, 0x%02x, 0x%02x, 0x%02x\n",
			sbi->A.fb_alg, sbi->B.fb_alg, sbi->fix_dur, sbi->dpitch);
		printf("      }");

		buf += ins_size;

		if (buf < b_end) printf(",");

		printf("\n");
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

