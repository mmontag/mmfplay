
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mmfplay.h"
#include "smaf.h"
#include "play.h"


void play_mmmd(unsigned char *buf, int size)
{
	printf("SMAF file loaded\n");
	read_chunk(buf, size - 2);	/* CRC at end */
}

#if 0
/*
 * Contents info chunk
 * This chunk stores information that is used to manage the SMAF file.
 */
void play_cnti(unsigned char *buf, int size)
{
	print_text("Contents info\n");

	/* Contents Class (required)
	 * Denotes the class of the contents. Parameters such as Vendor ID
	 * are stored. In the case of PDAs, for example, multi-functional
	 * devices of the future will be able to use the same data format,
	 * allowing extensive re-use of data.
	 */
	print_text("* Contents class: %02x\n", *buf++);

	/* Contents Type (required)
	 * Denotes the type of the content (for example, ring tone melody,
	 * karaoke data or commercial advertising content).
	 */
	print_text("* Contents type: %02x\n", *buf++);

	/* Contents Code Type (required)
    	 * Specifies the character code set, such as Japanese, English,
	 * Korean or Unicode.
	 */
	print_text("* Contents code type: %02x\n", *buf++);

	/* Copy Status (required)
	 * Indicates content transfer, save and copy permissions, and
	 * the like.
	 */
	print_text("* Copy status: %02x\n", *buf++);

	/* Copy Counts (required)
	 * Indicates the number of times the contents has been copied
	 * (copy history).
	 */
	print_text("* Copy counts: %02x\n", *buf++);
}

#endif

/*
 * Optional data chunk
 * This chunk stores information such as the genre, song title, artist
 * and writer or composer. This information is not necessarily used
 * for display purposes, but for recognition of particular data.
 */
void play_opda(unsigned char *buf, int size)
{
	read_chunk(buf, size);
}

void play_dchx(unsigned char *buf, int size)
{
	read_chunk16(buf, size);
}

void play_mtrx(unsigned char *buf, int size)
{
	int fmt;

	printf(" smaf: %d bytes in score track chunk\n", size);

	fmt = *buf++;			/* Format type */
	buf++;				/* Sequence type */
	mmf.time_d = *buf++;		/* Timebase D */
	mmf.time_g = *buf++;		/* Timebase G */
	size -= 4;

	printf(" smaf: timebase: D:%d G:%d\n", mmf.time_d, mmf.time_g);

	if (fmt == 2) {
		buf += 16;
		size -= 16;
	}
	else if (fmt == 0) {
		buf += 2;
		size -= 2;
	}
	else abort();

	read_chunk(buf, size);
}


/* play_mtsu */

#if 0
void play_mspi(unsigned char *buf, int size)
{
	print_text("Seek & Phrase Info chunk\n");
	while (size > 0) {
		print_text("* ");
		printf("%s ", buf);
		buf += 4;
		buf = dump_data(buf, 4);
		size -= 8;
	}
}
#endif

void play_mtsq(unsigned char *buf, int size)
{
	printf(" smaf: %d bytes in sequence data chunk\n", size);

	play_midi(buf, size);
}


#if 0

/*
 * Master track chunk
 * Under the SMAF specification, Master Track data represents both
 * sequences which describe music information and sequences which control
 * SMAF playback itself. Of the various tracks which comprise a SMAF file,
 * the Master Track differs from Score Tracks, PCM Tracks and Graphics
 * Tracks in that it does not include support for any output device.
 */
void play_mstr(unsigned char *buf, int size)
{
	int fmt, i;

	print_text("Master track chunk\n");

	/* Format Type
	 * Defines the actual format for the track chunk. At present,
	 * the following three types are defined: "Handyphone Standard",
	 * "Mobile Standard (Compressed)" and "Mobile Standard
	 * (Uncompressed)".
	 */
	print_text("* Format type: %02x\n", fmt = *buf++);

	/* Sequence Type
	 * Specifies either a single continuous stream of sequence data
	 * (stream sequence) or data from multiple consecutive phrases.
 	 */
	print_text("* Sequence type: %02x\n", *buf++);

	/* TimeBase_D
	 * Denotes standard time for Duration, which is used internally
	 * in sequence data.
	 */
	print_text("* Timebase D: %02x\n", *buf++);

	/* OptionSize
	 * Specifies the size of OptionData, which is to be used for
	 * extensions.
	 */
	print_text("* Option size: %02x\n", i = *buf++);
	size -= 4;

	/* OptionData
	 * Describes the data to be used for future extensions.
	 */
	print_text("* Option data: \n", *buf++);
	buf = dump_data(buf, i);
}
#endif


void play_unhandled(unsigned char *buffer, int size)
{
}


void play_text(unsigned char *buffer, int size)
{
	printf(" smaf: ");
	for(; size > 0; size--) putchar(*buffer++);
	printf("\n");
}

