
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mmfplay.h"
#include "smaf.h"
#include "tree.h"
#include "show.h"

struct ma3_instrument ma_ins[128];

static char *show_note(int num)
{
	static char *note[] = {
		"C", "C#", "D", "D#", "E", "F",
		"F#", "G", "G#", "A", "A#", "B"
	};
	static char buf[16];

	num -= 24;

	snprintf(buf, 16, "%-2.2s%d", note[num % 12], num / 12);
	
	return buf;
}

static unsigned char *show_fm_instrument(int n, unsigned char *buf)
{
	int i;

	buf++;
	buf++;
	ma_ins[n].oct = *buf & 0x07;
	buf++;
	ma_ins[n].alg = *buf & 0x07;

	print_text("  oct:%d alg:%d\n", ma_ins[n].oct, ma_ins[n].alg);

	for (i = 0; i < 4; i++) {
		buf++;
		ma_ins[n].op[i].rr = (*buf & 0xf0) >> 4;
		ma_ins[n].op[i].dr = 16 - (*buf & 0x0f);
		buf++;
		ma_ins[n].op[i].ar = (*buf & 0xf0) >> 4;
		ma_ins[n].op[i].sl = 16 - (*buf & 0x0f);
		buf++;
		ma_ins[n].op[i].tl = (*buf & 0xfc) >> 2;
		ma_ins[n].op[i].ksl = *buf & 0x03;
		buf++;
		buf++;
		buf++;
		ma_ins[n].op[i].mul = (*buf & 0xf0) >> 4;
		ma_ins[n].op[i].det = *buf & 0x0f;
		buf++;
		ma_ins[n].op[i].ws = (*buf & 0xf8) >> 3;
		ma_ins[n].op[i].fb = *buf & 0x07;
		buf++;

		print_text("  op%d: tl:%2d ar:%d dr:%d sl:%2d rr:%d "
			"mul:%d det:%d ws:%d fb:%d\n",
			i, ma_ins[n].op[i].tl,
			ma_ins[n].op[i].ar, ma_ins[n].op[i].dr,
			ma_ins[n].op[i].sl, ma_ins[n].op[i].dr,
			ma_ins[n].op[i].mul, ma_ins[n].op[i].det,
			ma_ins[n].op[i].ws, ma_ins[n].op[i].fb);
	}

	return buf;
}

static unsigned char *show_sysex(unsigned char *buf)
{
	int i, n, vendor;
	const int l = 11;

	buf = read_varlen(buf, &i, &n);

	printf("(%02x) ", i);

	switch (vendor = *buf++) {
	case 0x43:
		printf("Yamaha: ");
		break;
	default:
		printf("Vendor %02x: ", vendor);
	}

	if (cmp4(buf, 0x79, 0x06, 0x7F, 0x01)) {
		int b, type, num;

		buf += 4;
		i = 4;
		switch (b = *buf++) {
		case 0x7c:
			printf("Patch ");
			type = *buf++;
			num = *buf++;
			i += 2;
			printf("%s %02x (%s)\n",
				type == 0x00 ? "FM" : "WT",
				num, gm_ins[num]);
			if (type == 0) {
				buf += 2; /* unknown */
				buf = show_fm_instrument(num, buf);
			}
			break;
		case 0x7d:
			printf("Drum\n");
			break;
		default:
			printf("Unknown: %02x\n", b);
		}
		for (; *buf != 0xf7; buf++, i++);
		buf++;
	} else {
		for (i = 0; *buf != 0xf7; buf++, i++) {
			if (i < l) printf("%02x ", *buf);
		}
		if (i > l) printf(".. ");
		printf("%02x [%02x]\n", *buf++, i + 2);
	}

	return buf;
}

void show_mmmd(unsigned char *buf, int size)
{
	print_text("SMAF file\n");
	read_chunk(buf, size - 2);	/* CRC at end */
}

/*
 * Contents info chunk
 * This chunk stores information that is used to manage the SMAF file.
 */
void show_cnti(unsigned char *buf, int size)
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

/*
 * Optional data chunk
 * This chunk stores information such as the genre, song title, artist
 * and writer or composer. This information is not necessarily used
 * for display purposes, but for recognition of particular data.
 */
void show_opda(unsigned char *buf, int size)
{
	print_text("Optional data chunk\n");

	read_chunk(buf, size);
}

void show_dchx(unsigned char *buf, int size)
{
	print_text("DchX chunk\n");

	read_chunk16(buf, size);
}

/*
 * This chunk stores sequence data for sound generator playback.
 *  - Format Type (required)
 *  - Sequence Type (required)
 *  - TimeBase_D (required)
 *  - TimeBase_G (required)
 *  - Channel Status (required)
 *  - Seek & Phrase Info Chunk (optional)
 *  - Set-up Data Chunk (optional)
 *  - Sequence Data Chunk (required)
 *  - Stream PCM Data Chunk (optional)
 *    (only if Format Type= "Mobile Standard") 
 */
void show_mtrx(unsigned char *buf, int size)
{
	int fmt;

	print_text("Score track chunk\n");

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

	/* TimeBase_G
	 * Denotes standard time for GateTime, which is used internally
	 * in sequence data.
	 */
	print_text("* Timebase G: %02x\n", *buf++);
	size -= 4;

	if (fmt == 2) {
		print_text("* Channel status: ");
		buf = dump_data(buf, 16);
		size -= 16;
	}
	else if (fmt == 0) {
		print_text("* Channel status: ");
		buf = dump_data(buf, 2);
		size -= 2;
	}
	else abort();

	read_chunk(buf, size);
}

/*
 * Example: The Yamaha TX16W sampler sysex format is (simplified for
 * educational purposes): F0 43 sn rr xx xx ... F7. F0 is again start
 * of exclusive. 43 = Yamaha. In the third byte, the 'n' again is the
 * midi channel, and the 's' depends on the function-group being called
 * upon. Finally 'rr' specifies the function being called.
 */
void show_mtsu(unsigned char *buf, const int size)
{
	unsigned char *b_end;

	print_text("Setup data chunk\n");

	b_end = buf + size;

	while (buf < b_end) {
		int b, i;

		b = *buf++;

		switch (b) {
		case 0xf0: msg_sysex:
			print_text("* SysEx ");
			buf = show_sysex(buf);
			break;
		case 0xff:
			b = *buf++;
			switch (b) {
			case 0xf0:
				goto msg_sysex;
			default:
				print_text("* Meta event %02x: ", b);
				break;
			}
		default:
			print_text("%02x ", b);
			printf("%02x: ", i = *buf++);
			dump_data(buf, i > 20 ? 20 : i);
			buf += i;
		}
	}
}

void show_mspi(unsigned char *buf, int size)
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

void show_mtsq(unsigned char *buf, int size)
{
	int delta, b, i;
	unsigned char *b_end;

	print_text("Sequence data chunk\n");

	b_end = buf + size;

	while (buf < b_end) {
		int bh, bl;

		buf = read_varlen(buf, &delta, &i);

		print_text("%02x: ", delta);
		b = *buf++;

		bh = b & 0xf0;
		bl = b & 0x0f;

		if (b == 0xf0) {
			printf("SysEx ");
			buf = show_sysex(buf);
		} else if (b == 0xff) {
			int n;
			printf("Sequence number ");
			/*n = read16_be(buf);
			buf += 2;*/
			n = *buf++;
			printf ("%02x\n", n);
		} else if (bh == 0x80) {
			int note, vel, i;
			printf("CH%02x Note On: ", bl);
			note = *buf++;
			buf = read_varlen(buf, &vel, &i);
			printf("Note %s (%02x) Vel %02x\n",
				show_note(note), note, vel);
		} else if (bh == 0x90) {
			int note, vel, time, i;
			printf("CH%02x Note On: ", bl);
			note = *buf++;
			buf = read_varlen(buf, &vel, &i);
			buf = read_varlen(buf, &time, &i);
			printf("Note %s (%02x) Vel %02x Gate Time %02x\n",
				show_note(note), note, vel, time);
		} else if (bh == 0xb0) {
			int ctr, val;
			printf("CH%02x Controller: ", bl);
			ctr = *buf++;
			val = *buf++;
			switch (ctr) {
			case 0x07: printf("Main volume"); break;
			case 0x0a: printf("Pan"); break;
			case 0x0b: printf("Expression"); break;
			default: printf("ctr%02x", ctr); break;
			}
			printf(" value %02x\n", val);
		} else if (bh == 0xc0) {
			int prg;
			printf("CH%02x Program ", bl);
			prg = *buf++;
			printf("%02x (%s)\n", prg, gm_ins[prg]);
		} else if (bh == 0xe0) {
			int bend;
			printf("CH%02x Pitchbend ", bl);
			bend = read16_be(buf);
			buf += 2;
			printf("%04x\n", bend);
		} else {
			printf("%02x ", b);
			buf = dump_data(buf, 4);
		}
	}
}

/*
 * Master track chunk
 * Under the SMAF specification, Master Track data represents both
 * sequences which describe music information and sequences which control
 * SMAF playback itself. Of the various tracks which comprise a SMAF file,
 * the Master Track differs from Score Tracks, PCM Tracks and Graphics
 * Tracks in that it does not include support for any output device.
 */
void show_mstr(unsigned char *buf, int size)
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

void show_unhandled(unsigned char *buffer, int size)
{
	print_text("(unhandled)");
}


/*
 * The various types of optional data are listed below.
 * Vendor
 * Carrier
 * Category
 * Song title
 * Artist
 * Writer
 * Composer
 * Arranger
 * Copyright
 * Management organization
 * Management information
 * Creation date
 * Date of last change
 * Post-editing status
 * vCard
 * Picture illustrator
 * Owner of copyright to picture
 * Picture editor
 * Owner of copyright to text
 * Text editor
 * Editor of overall content
 */
void show_ca(unsigned char *buf, int size)
{
	print_text("* Category: ");
	print_data(buf, size);
}

void show_cr(unsigned char *buf, int size)
{
	print_text("* Copyright: ");
	print_data(buf, size);
}

void show_st(unsigned char *buf, int size)
{
	print_text("* Song title: ");
	print_data(buf, size);
}

void show_es(unsigned char *buf, int size)
{
	print_text("* ES data: ");
	dump_data(buf, size);
}

void show_as(unsigned char *buf, int size)
{
	print_text("* AS data: ");
	dump_data(buf, size);
}

void show_a0(unsigned char *buf, int size)
{
	print_text("* A0 data: ");
	print_data(buf, size);
}

void show_a2(unsigned char *buf, int size)
{
	print_text("* A2 data: ");
	print_data(buf, size);
}


