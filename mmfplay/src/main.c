
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "mmfplay.h"
#include "smaf.h"
#include "tree.h"
#include "chunk.h"
#include "show.h"
#include "play.h"
#include "sound.h"


static struct chunk_info *chunk_head = NULL;
static struct chunk_info *chunk16_head = NULL;

unsigned char *handle_chunk(unsigned char *buffer, int tsz, int *sz)
{
	char id[4];
	int size;

	memcpy(id, buffer, 4);
	size = *sz = read32_be(buffer + 4);
	buffer += 8;

	if (opt.mode == MMFPLAY_SHOW) {
		print_text ("\n");
		print_leaf(size + 8 >= tsz,
			"Chunk: %02x %02x %02x %02x [%4.4s]\n",
			id[0], id[1], id[2], id[3], id);
		print_text("Size: %d\n", size);
	}

	process_chunk(chunk_head, id, buffer, size);

	return buffer + size;
}

unsigned char *read_chunk(unsigned char *buffer, int size)
{
	int i;

	new_branch();
	while (size >= 8) {
		buffer = handle_chunk(buffer, size, &i);
		size -= (i + 8);
	}
	end_branch();

	return buffer;
}

unsigned char *handle_chunk16(unsigned char *buffer, int tsz, int *sz)
{
	char id[2];
	int size;

	memcpy(id, buffer, 2);
	size = *sz = read16_be(buffer + 2);
	buffer += 4;

	if (opt.mode == MMFPLAY_SHOW) {
		print_text ("\n");
		print_leaf(size + 4 >= tsz,
			"Chunk16: %02x %02x [%2.2s]\n", id[0], id[1], id);
		print_text("Size: %d\n", size);
	}

	process_chunk(chunk16_head, id, buffer, size);

	return buffer + size;
}

unsigned char *read_chunk16(unsigned char *buffer, int size)
{
	int i;

	new_branch();
	while (size >= 4) {
		buffer = handle_chunk16(buffer, size, &i);
		size -= (i + 4);
	}
	end_branch();

	return buffer;
}


int main(int argc, char **argv)
{
	FILE *f;
	struct stat st;
	unsigned char *buffer;

	printf("MMFplay version " VERSION " (built "
		__DATE__ " " __TIME__ " by " USERNAME ")\n");

	parse_cli(argc, argv);

 	f = fopen(argv[optind], "r");
	fstat(fileno(f), &st);
	buffer = malloc(st.st_size);
	fread(buffer, 1, st.st_size, f);
	fclose(f);


	if (opt.mode == MMFPLAY_SHOW) {
		printf("\nMMF file structure for %s\n", argv[1]);

		register_chunk(&chunk_head, "MMMD", 4, show_mmmd);
		register_chunk(&chunk_head, "CNTI", 4, show_cnti);
		register_chunk(&chunk_head, "OPDA", 4, show_opda);
		register_chunk(&chunk_head, "Dch",  3, show_dchx);
		register_chunk(&chunk_head, "MTR",  3, show_mtrx);
		register_chunk(&chunk_head, "Mtsu", 4, show_mtsu);
		register_chunk(&chunk_head, "MspI", 4, show_mspi);
		register_chunk(&chunk_head, "Mtsq", 4, show_mtsq);
		register_chunk(&chunk_head, "MSTR", 4, show_mstr);
		register_chunk(&chunk_head, "",     0, show_unhandled);
	
		register_chunk(&chunk16_head, "ST", 2, show_st);
		register_chunk(&chunk16_head, "CA", 2, show_ca);
		register_chunk(&chunk16_head, "CR", 2, show_cr);
		register_chunk(&chunk16_head, "ES", 2, show_es);
		register_chunk(&chunk16_head, "AS", 2, show_as);
		register_chunk(&chunk16_head, "A0", 2, show_a0);
		register_chunk(&chunk16_head, "A2", 2, show_a2);
		register_chunk(&chunk16_head, "",   0, show_unhandled);

		read_chunk(buffer, st.st_size);
	}
	else if (opt.mode == MMFPLAY_PLAY) {
		register_chunk(&chunk_head, "MMMD", 4, play_mmmd);
		register_chunk(&chunk_head, "OPDA", 4, play_opda);
		register_chunk(&chunk_head, "Dch",  3, play_dchx);
		register_chunk(&chunk_head, "MTR",  3, play_mtrx);
		register_chunk(&chunk_head, "Mtsq", 4, play_mtsq);
		register_chunk(&chunk_head, "",     0, play_unhandled);

		register_chunk(&chunk16_head, "ST", 2, play_text);
		register_chunk(&chunk16_head, "CA", 2, play_text);
		register_chunk(&chunk16_head, "CR", 2, play_text);
		register_chunk(&chunk16_head, "",   0, play_unhandled);

		init_sound();
		read_chunk(buffer, st.st_size);
		deinit_sound();
	}


	release_chunks(&chunk_head);
	release_chunks(&chunk16_head);

	return 0;
}

