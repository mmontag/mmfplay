
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>

#include "mmfplay.h"
#include "sbi.h"
#include "opl3.h"




int read_ibk(unsigned char *buf, const int size, char *name)
{
	/*unsigned char *b_end;*/
	struct ibk *ibk;
	int n;

	/*b_end = buf + size;*/

	ibk = (struct ibk *)buf;
	if (!cmp4(buf, 'I', 'B', 'K', 0x1a))
		return -1;

	printf("#include \"opl3.h\"\n\n");
	printf("struct opl2_instrument %s[128] = {\n", name);

	for (n = 0; n < 128; n++) {
		ibk = (struct ibk *)buf;
		
		printf("      { ");

		printf ("\t/* %3d */\n", n);

		printf("\t{ { ");
		printf("0x%02x, ", ibk->i.m_flg_mul);
		printf("0x%02x, ", ibk->i.m_ksl_tl);
		printf("0x%02x, ", ibk->i.m_ar_dr);
		printf("0x%02x, ", ibk->i.m_sl_rr);
		printf("0x%02x", ibk->i.m_ws);
		printf(" },\t/* OP1 */\n");

		printf("\t  { ");
		printf("0x%02x, ", ibk->i.c_flg_mul);
		printf("0x%02x, ", ibk->i.c_ksl_tl);
		printf("0x%02x, ", ibk->i.c_ar_dr);
		printf("0x%02x, ", ibk->i.c_sl_rr);
		printf("0x%02x", ibk->i.m_ws);
		printf(" } },\t/* OP2 */\n");

		printf("\t0x%02x\n", ibk->i.fb_alg);
		printf("      }");

		if (n < 127)
			printf(",");

		printf("\n");

		buf += 16;
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

	
int load_instruments(char *n, char *f)
{
	unsigned char *buf;
	int i, size;

	buf = load_file(f, &size);
	i = read_ibk(buf, size, n);

	return i;
}



int main(int argc, char **argv)
{
	if (argc < 3) {
		fprintf(stderr, "Usage: %s <o3file>\n", argv[0]);
		exit(1);
	}
	if (load_instruments(argv[1], argv[2]) < 0) {
		fprintf(stderr, "Not an IBK file.\n");
		exit(1);
	}

	return 0;
}

