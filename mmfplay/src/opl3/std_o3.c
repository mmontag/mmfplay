#include "opl3.h"

struct opl3_instrument opl3_ins[128] = {
      { OPL3_TYPE_4OP,	/*   0: Acoustic Grand       */
	{ { 0x23, 0x15, 0xfd, 0x7c, 0x03 },	/* OP1 */
	  { 0x01, 0x08, 0x84, 0xf5, 0x03 },	/* OP2 */
	  { 0x03, 0x5d, 0xf2, 0x35, 0x00 },	/* OP3 */
	  { 0x01, 0x08, 0xf4, 0xf5, 0x00 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*   1: Bright Acoustic      */
	{ { 0x03, 0x7d, 0xf3, 0xf5, 0x00 },	/* OP1 */
	  { 0x01, 0x08, 0xf3, 0xf5, 0x00 },	/* OP2 */
	  { 0x03, 0x55, 0xf2, 0xf5, 0x00 },	/* OP3 */
	  { 0x01, 0x08, 0xf2, 0xf5, 0x00 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*   2: Electric Grand Piano */
	{ { 0x03, 0x4d, 0xfb, 0x55, 0x04 },	/* OP1 */
	  { 0x01, 0x08, 0xf3, 0xc5, 0x04 },	/* OP2 */
	  { 0x03, 0xce, 0xf3, 0x55, 0x00 },	/* OP3 */
	  { 0x01, 0x08, 0xf3, 0xd5, 0x00 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*   3: Honky-tonk           */
	{ { 0x03, 0xd7, 0xf3, 0xf5, 0x04 },	/* OP1 */
	  { 0x01, 0x08, 0xf2, 0xf5, 0x04 },	/* OP2 */
	  { 0x03, 0xd5, 0xf3, 0xf5, 0x00 },	/* OP3 */
	  { 0x01, 0x08, 0xf3, 0xf5, 0x00 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*   4: Rhodes Piano         */
	{ { 0x16, 0x57, 0xff, 0x8a, 0x00 },	/* OP1 */
	  { 0x01, 0x08, 0xf3, 0x66, 0x00 },	/* OP2 */
	  { 0x0b, 0x2b, 0xe5, 0xb5, 0x00 },	/* OP3 */
	  { 0x81, 0x08, 0xe2, 0x86, 0x00 } },	/* OP4 */
	0x1c, 0x11
      },
      { OPL3_TYPE_4OP,	/*   5: Chorused Piano       */
	{ { 0x0a, 0x5f, 0xf4, 0x87, 0x00 },	/* OP1 */
	  { 0x01, 0x08, 0xf4, 0x86, 0x00 },	/* OP2 */
	  { 0x03, 0xcc, 0xf2, 0x86, 0x00 },	/* OP3 */
	  { 0x01, 0x08, 0xe2, 0x86, 0x00 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*   6: Harpsichord          */
	{ { 0x22, 0xd1, 0xe3, 0x79, 0x04 },	/* OP1 */
	  { 0x01, 0x88, 0xf3, 0xa7, 0x04 },	/* OP2 */
	  { 0x22, 0x96, 0xf2, 0x79, 0x04 },	/* OP3 */
	  { 0x01, 0x08, 0xf3, 0xa7, 0x04 } },	/* OP4 */
	0x14, 0x11
      },
      { OPL3_TYPE_4OP,	/*   7: Clavinet             */
	{ { 0x22, 0x9c, 0xf4, 0x19, 0x05 },	/* OP1 */
	  { 0x01, 0x08, 0xf3, 0xa6, 0x05 },	/* OP2 */
	  { 0x22, 0x97, 0xf4, 0x19, 0x04 },	/* OP3 */
	  { 0x01, 0x08, 0xf3, 0xe6, 0x04 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*   8: Celesta              */
	{ { 0x1b, 0x5a, 0xf4, 0xf6, 0x00 },	/* OP1 */
	  { 0x19, 0x09, 0xe3, 0xa2, 0x00 },	/* OP2 */
	  { 0x1a, 0x57, 0xf3, 0xb2, 0x00 },	/* OP3 */
	  { 0x11, 0x08, 0xf2, 0xa3, 0x00 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*   9: Glockenspiel         */
	{ { 0x1b, 0x5a, 0xd6, 0x53, 0x00 },	/* OP1 */
	  { 0x17, 0x0a, 0xf3, 0x53, 0x00 },	/* OP2 */
	  { 0x1a, 0x49, 0xd1, 0x52, 0x00 },	/* OP3 */
	  { 0x11, 0x08, 0xf3, 0x52, 0x00 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  10: Musicbox             */
	{ { 0x1b, 0x5a, 0xf6, 0xf6, 0x00 },	/* OP1 */
	  { 0x14, 0x08, 0x63, 0xa3, 0x00 },	/* OP2 */
	  { 0x1b, 0x52, 0xd3, 0xb2, 0x00 },	/* OP3 */
	  { 0x11, 0x08, 0xf2, 0xa3, 0x00 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  11: Vibraphone           */
	{ { 0x8b, 0x5a, 0xd7, 0x15, 0x00 },	/* OP1 */
	  { 0x84, 0x08, 0xf1, 0x95, 0x00 },	/* OP2 */
	  { 0x9b, 0x49, 0xdc, 0x25, 0x00 },	/* OP3 */
	  { 0x81, 0x08, 0xd2, 0xb5, 0x00 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  12: Marimba              */
	{ { 0x08, 0xc0, 0xfd, 0x56, 0x00 },	/* OP1 */
	  { 0x01, 0x08, 0xf6, 0x68, 0x00 },	/* OP2 */
	  { 0x95, 0x40, 0xfe, 0x27, 0x00 },	/* OP3 */
	  { 0x81, 0x08, 0xf0, 0x05, 0x00 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  13: Xylophone            */
	{ { 0x08, 0xc0, 0xfa, 0x56, 0x00 },	/* OP1 */
	  { 0x03, 0x08, 0xf8, 0x66, 0x00 },	/* OP2 */
	  { 0x97, 0x40, 0xdf, 0x26, 0x00 },	/* OP3 */
	  { 0x81, 0x08, 0xf8, 0x06, 0x00 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  14: Tubular Bells        */
	{ { 0x03, 0x95, 0xf3, 0x33, 0x00 },	/* OP1 */
	  { 0x00, 0x88, 0xf3, 0x23, 0x00 },	/* OP2 */
	  { 0x03, 0x8c, 0xf4, 0x23, 0x00 },	/* OP3 */
	  { 0x00, 0x88, 0xf3, 0x03, 0x00 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  15: Dulcimer             */
	{ { 0x01, 0x8b, 0xa7, 0xf9, 0x06 },	/* OP1 */
	  { 0x00, 0x08, 0x94, 0x36, 0x06 },	/* OP2 */
	  { 0x02, 0x18, 0xf6, 0x79, 0x04 },	/* OP3 */
	  { 0x81, 0x88, 0xf4, 0x55, 0x04 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  16: Hammond Organ        */
	{ { 0x24, 0x40, 0xfd, 0xfd, 0x00 },	/* OP1 */
	  { 0x24, 0x0a, 0xfb, 0x0e, 0x00 },	/* OP2 */
	  { 0x20, 0x95, 0xfb, 0x0e, 0x00 },	/* OP3 */
	  { 0x20, 0x08, 0xf6, 0x0e, 0x00 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  17: Percussive Organ     */
	{ { 0x04, 0x17, 0xfe, 0xb7, 0x00 },	/* OP1 */
	  { 0x24, 0x08, 0xf7, 0x67, 0x00 },	/* OP2 */
	  { 0x20, 0x14, 0xf9, 0x06, 0x00 },	/* OP3 */
	  { 0xa0, 0x08, 0xf6, 0x07, 0x00 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  18: Rock Organ           */
	{ { 0xa2, 0x1e, 0xb9, 0x36, 0x00 },	/* OP1 */
	  { 0x20, 0x08, 0xa9, 0x0e, 0x00 },	/* OP2 */
	  { 0xb4, 0x1a, 0x79, 0x51, 0x04 },	/* OP3 */
	  { 0x20, 0x08, 0x77, 0x4c, 0x04 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  19: Church Organ         */
	{ { 0x25, 0x23, 0x74, 0x11, 0x00 },	/* OP1 */
	  { 0xa1, 0x08, 0x60, 0x05, 0x00 },	/* OP2 */
	  { 0xb0, 0xd3, 0x74, 0x12, 0x04 },	/* OP3 */
	  { 0xb0, 0x08, 0x60, 0x05, 0x04 } },	/* OP4 */
	0x12, 0x11
      },
      { OPL3_TYPE_4OP,	/*  20: Reed Organ           */
	{ { 0x22, 0x24, 0x76, 0x06, 0x04 },	/* OP1 */
	  { 0x21, 0x48, 0x76, 0x06, 0x04 },	/* OP2 */
	  { 0x20, 0x21, 0x76, 0x06, 0x00 },	/* OP3 */
	  { 0x21, 0x08, 0x66, 0x06, 0x00 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  21: Accordion            */
	{ { 0x22, 0x1b, 0x76, 0x07, 0x00 },	/* OP1 */
	  { 0x21, 0x08, 0x76, 0x07, 0x00 },	/* OP2 */
	  { 0x21, 0x19, 0x76, 0x07, 0x00 },	/* OP3 */
	  { 0x21, 0x08, 0x66, 0x07, 0x00 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  22: Harmonica            */
	{ { 0x21, 0x20, 0x93, 0x09, 0x04 },	/* OP1 */
	  { 0x21, 0x08, 0x73, 0x09, 0x04 },	/* OP2 */
	  { 0x21, 0x27, 0x76, 0x09, 0x04 },	/* OP3 */
	  { 0x21, 0x08, 0x76, 0x09, 0x04 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  23: Tango Accordion      */
	{ { 0x21, 0x1e, 0x76, 0x09, 0x04 },	/* OP1 */
	  { 0x21, 0x08, 0x66, 0x19, 0x04 },	/* OP2 */
	  { 0x21, 0x22, 0x76, 0x29, 0x04 },	/* OP3 */
	  { 0x21, 0x08, 0x66, 0x19, 0x04 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  24: Nylon Guitar         */
	{ { 0x1b, 0x4c, 0xfe, 0xfe, 0x04 },	/* OP1 */
	  { 0x11, 0x08, 0xf2, 0xe3, 0x04 },	/* OP2 */
	  { 0x31, 0x4c, 0xf2, 0x45, 0x04 },	/* OP3 */
	  { 0x11, 0x08, 0xf2, 0xf4, 0x04 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  25: Steel Guitar         */
	{ { 0x30, 0x08, 0xf2, 0x49, 0x02 },	/* OP1 */
	  { 0x05, 0x0b, 0xfe, 0x43, 0x02 },	/* OP2 */
	  { 0x01, 0x08, 0xf2, 0xe7, 0x01 },	/* OP3 */
	  { 0x00, 0x08, 0xf3, 0x9d, 0x01 } },	/* OP4 */
	0x11, 0x11
      },
      { OPL3_TYPE_4OP,	/*  26: Jazz Guitar          */
	{ { 0x34, 0x0d, 0xf2, 0x49, 0x00 },	/* OP1 */
	  { 0x04, 0x16, 0xfd, 0x43, 0x00 },	/* OP2 */
	  { 0x01, 0x08, 0xf2, 0xe7, 0x00 },	/* OP3 */
	  { 0x01, 0x0c, 0xf3, 0x9d, 0x00 } },	/* OP4 */
	0x11, 0x11
      },
      { OPL3_TYPE_4OP,	/*  27: Clean Guitar         */
	{ { 0x03, 0x00, 0xfb, 0x49, 0x06 },	/* OP1 */
	  { 0x23, 0x00, 0xfc, 0x64, 0x06 },	/* OP2 */
	  { 0x01, 0x08, 0xf4, 0xc4, 0x02 },	/* OP3 */
	  { 0x01, 0x08, 0xf6, 0xc4, 0x02 } },	/* OP4 */
	0x19, 0x11
      },
      { OPL3_TYPE_4OP,	/*  28: Muted Guitar         */
	{ { 0x01, 0x06, 0xf6, 0x27, 0x05 },	/* OP1 */
	  { 0x01, 0x0d, 0xf3, 0xe7, 0x05 },	/* OP2 */
	  { 0x02, 0x05, 0xfa, 0x96, 0x04 },	/* OP3 */
	  { 0x01, 0x08, 0x65, 0x9b, 0x04 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  29: OverdrivenGuitar     */
	{ { 0x21, 0xc1, 0xf2, 0x13, 0x00 },	/* OP1 */
	  { 0x21, 0x48, 0xf3, 0x16, 0x00 },	/* OP2 */
	  { 0x21, 0x03, 0x97, 0xae, 0x03 },	/* OP3 */
	  { 0x21, 0x08, 0x96, 0x26, 0x03 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  30: Distortion Guitar    */
	{ { 0x21, 0xc1, 0xf2, 0x16, 0x00 },	/* OP1 */
	  { 0x21, 0x08, 0xf2, 0x16, 0x00 },	/* OP2 */
	  { 0x21, 0x03, 0xc7, 0x46, 0x03 },	/* OP3 */
	  { 0x20, 0x08, 0x90, 0x16, 0x03 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  31: Guitar Harmonics     */
	{ { 0x0b, 0x40, 0x65, 0x1a, 0x00 },	/* OP1 */
	  { 0x01, 0x48, 0xf2, 0x3e, 0x00 },	/* OP2 */
	  { 0x05, 0x00, 0xe2, 0x1a, 0x02 },	/* OP3 */
	  { 0x0a, 0x49, 0x54, 0x1a, 0x02 } },	/* OP4 */
	0x1c, 0x11
      },
      { OPL3_TYPE_2OP,	/*  32: Acoustic Bass        */
	{ { 0x21, 0x12, 0xe4, 0x15, 0x00 },	/* OP1 */
	  { 0x21, 0x08, 0xd3, 0xa6, 0x00 },	/* OP2 */
	  { 0x01, 0x3f, 0x82, 0x67, 0x00 },	/* OP3 */
	  { 0x01, 0x3f, 0xf2, 0x6d, 0x00 } },	/* OP4 */
	0x10, 0x01
      },
      { OPL3_TYPE_4OP,	/*  33: Finger Bass          */
	{ { 0x21, 0x08, 0xf2, 0x49, 0x04 },	/* OP1 */
	  { 0x01, 0x0b, 0xf2, 0x97, 0x04 },	/* OP2 */
	  { 0x01, 0x09, 0xf6, 0xe7, 0x01 },	/* OP3 */
	  { 0x01, 0x08, 0xf3, 0x9d, 0x01 } },	/* OP4 */
	0x11, 0x11
      },
      { OPL3_TYPE_4OP,	/*  34: Pick Bass            */
	{ { 0x21, 0x08, 0xf2, 0x49, 0x04 },	/* OP1 */
	  { 0x01, 0x08, 0xf2, 0x97, 0x04 },	/* OP2 */
	  { 0x01, 0x08, 0xf6, 0xe7, 0x01 },	/* OP3 */
	  { 0x01, 0x08, 0xf3, 0x9d, 0x01 } },	/* OP4 */
	0x11, 0x11
      },
      { OPL3_TYPE_4OP,	/*  35: Fretless Bass        */
	{ { 0x01, 0xc8, 0xf2, 0x6a, 0x00 },	/* OP1 */
	  { 0x01, 0x08, 0xc4, 0x97, 0x00 },	/* OP2 */
	  { 0x01, 0x11, 0x91, 0xe7, 0x00 },	/* OP3 */
	  { 0x01, 0x08, 0xf2, 0x6d, 0x00 } },	/* OP4 */
	0x19, 0x11
      },
      { OPL3_TYPE_4OP,	/*  36: Slap Bass 1          */
	{ { 0x01, 0x06, 0xfd, 0x49, 0x06 },	/* OP1 */
	  { 0x0b, 0x08, 0xfa, 0x87, 0x06 },	/* OP2 */
	  { 0x01, 0x08, 0xf3, 0xb7, 0x04 },	/* OP3 */
	  { 0x01, 0x08, 0xf3, 0x9d, 0x04 } },	/* OP4 */
	0x11, 0x11
      },
      { OPL3_TYPE_2OP,	/*  37: Slap Bass 2          */
	{ { 0x26, 0x24, 0xf6, 0x87, 0x04 },	/* OP1 */
	  { 0x21, 0x08, 0xf2, 0xf7, 0x04 },	/* OP2 */
	  { 0x01, 0x3f, 0xf3, 0x9d, 0x02 },	/* OP3 */
	  { 0x01, 0x3f, 0xf3, 0x9d, 0x02 } },	/* OP4 */
	0x10, 0x01
      },
      { OPL3_TYPE_4OP,	/*  38: Synth Bass 1         */
	{ { 0x21, 0x08, 0xf5, 0x47, 0x00 },	/* OP1 */
	  { 0x01, 0x48, 0xf2, 0xf7, 0x00 },	/* OP2 */
	  { 0x21, 0x07, 0xf5, 0x47, 0x00 },	/* OP3 */
	  { 0x01, 0x48, 0xf2, 0xf7, 0x00 } },	/* OP4 */
	0x10, 0x19
      },
      { OPL3_TYPE_4OP,	/*  39: Synth Bass 2         */
	{ { 0xc1, 0x08, 0xf2, 0x48, 0x00 },	/* OP1 */
	  { 0x01, 0x21, 0xf2, 0x97, 0x00 },	/* OP2 */
	  { 0x01, 0x0a, 0xf2, 0x97, 0x00 },	/* OP3 */
	  { 0x01, 0x08, 0xf2, 0x77, 0x00 } },	/* OP4 */
	0x11, 0x10
      },
      { OPL3_TYPE_4OP,	/*  40: Violin               */
	{ { 0x22, 0xca, 0xb4, 0x26, 0x04 },	/* OP1 */
	  { 0x21, 0x11, 0x44, 0x06, 0x04 },	/* OP2 */
	  { 0x22, 0xcc, 0x89, 0x16, 0x04 },	/* OP3 */
	  { 0x01, 0x10, 0x5d, 0x06, 0x04 } },	/* OP4 */
	0x1c, 0x11
      },
      { OPL3_TYPE_4OP,	/*  41: Viola                */
	{ { 0x24, 0xcb, 0x88, 0x26, 0x04 },	/* OP1 */
	  { 0x21, 0x10, 0x66, 0x16, 0x04 },	/* OP2 */
	  { 0x34, 0xc4, 0x86, 0x66, 0x04 },	/* OP3 */
	  { 0x21, 0x10, 0x66, 0x06, 0x04 } },	/* OP4 */
	0x1c, 0x11
      },
      { OPL3_TYPE_4OP,	/*  42: Cello                */
	{ { 0x22, 0xcb, 0x72, 0x46, 0x04 },	/* OP1 */
	  { 0x01, 0x1c, 0x5f, 0x86, 0x04 },	/* OP2 */
	  { 0x22, 0xcd, 0x76, 0x36, 0x04 },	/* OP3 */
	  { 0x21, 0x08, 0x54, 0x06, 0x04 } },	/* OP4 */
	0x16, 0x11
      },
      { OPL3_TYPE_4OP,	/*  43: Contrabass           */
	{ { 0x22, 0xff, 0x86, 0x06, 0x04 },	/* OP1 */
	  { 0x01, 0x08, 0x6f, 0x08, 0x04 },	/* OP2 */
	  { 0x22, 0xd2, 0x74, 0x16, 0x04 },	/* OP3 */
	  { 0x21, 0x08, 0x54, 0x06, 0x04 } },	/* OP4 */
	0x16, 0x11
      },
      { OPL3_TYPE_4OP,	/*  44: Tremolo Strings      */
	{ { 0x60, 0x00, 0x68, 0x04, 0x02 },	/* OP1 */
	  { 0x61, 0x09, 0x53, 0x45, 0x02 },	/* OP2 */
	  { 0x61, 0x11, 0x54, 0x54, 0x01 },	/* OP3 */
	  { 0x60, 0x08, 0x78, 0x35, 0x01 } },	/* OP4 */
	0x11, 0x11
      },
      { OPL3_TYPE_4OP,	/*  45: Pizzicato String     */
	{ { 0x00, 0x07, 0xf9, 0xf1, 0x03 },	/* OP1 */
	  { 0x00, 0x08, 0xf6, 0x34, 0x03 },	/* OP2 */
	  { 0x00, 0x0c, 0x89, 0xf1, 0x03 },	/* OP3 */
	  { 0x00, 0x08, 0xb5, 0x34, 0x03 } },	/* OP4 */
	0x16, 0x11
      },
      { OPL3_TYPE_4OP,	/*  46: Orchestral Harp      */
	{ { 0x12, 0xd0, 0xb5, 0x23, 0x04 },	/* OP1 */
	  { 0x01, 0x17, 0xc3, 0xe3, 0x04 },	/* OP2 */
	  { 0x12, 0xd9, 0xe5, 0x15, 0x04 },	/* OP3 */
	  { 0x11, 0x08, 0xa2, 0xf3, 0x04 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  47: Timpani              */
	{ { 0x11, 0xd0, 0xe4, 0xe4, 0x01 },	/* OP1 */
	  { 0x10, 0x08, 0xd3, 0xf4, 0x01 },	/* OP2 */
	  { 0x14, 0x14, 0xf6, 0xf4, 0x02 },	/* OP3 */
	  { 0x10, 0x08, 0xf3, 0xf4, 0x02 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  48: String Ensemble1     */
	{ { 0x21, 0x1b, 0xa9, 0x23, 0x00 },	/* OP1 */
	  { 0x21, 0xc8, 0x63, 0x25, 0x00 },	/* OP2 */
	  { 0x21, 0x22, 0xa9, 0x23, 0x00 },	/* OP3 */
	  { 0x21, 0x48, 0x43, 0x24, 0x00 } },	/* OP4 */
	0x1e, 0x11
      },
      { OPL3_TYPE_4OP,	/*  49: String Ensemble2     */
	{ { 0x21, 0x1f, 0xa9, 0x23, 0x00 },	/* OP1 */
	  { 0x21, 0xc8, 0x33, 0x15, 0x00 },	/* OP2 */
	  { 0x21, 0x23, 0x60, 0x23, 0x00 },	/* OP3 */
	  { 0x21, 0x88, 0x33, 0x25, 0x00 } },	/* OP4 */
	0x1e, 0x11
      },
      { OPL3_TYPE_4OP,	/*  50: SynthStrings 1       */
	{ { 0x20, 0x08, 0x52, 0xb6, 0x02 },	/* OP1 */
	  { 0x01, 0x0c, 0x50, 0x15, 0x02 },	/* OP2 */
	  { 0x21, 0x09, 0x44, 0x04, 0x01 },	/* OP3 */
	  { 0x20, 0x51, 0x41, 0x27, 0x01 } },	/* OP4 */
	0x11, 0x11
      },
      { OPL3_TYPE_4OP,	/*  51: SynthStrings 2       */
	{ { 0x22, 0x48, 0x51, 0xa4, 0x04 },	/* OP1 */
	  { 0x01, 0xc2, 0x31, 0xf4, 0x04 },	/* OP2 */
	  { 0x21, 0x08, 0x70, 0xf4, 0x01 },	/* OP3 */
	  { 0x21, 0x48, 0x34, 0x74, 0x01 } },	/* OP4 */
	0x11, 0x11
      },
      { OPL3_TYPE_4OP,	/*  52: Choir Aahs           */
	{ { 0x21, 0x10, 0x61, 0x04, 0x00 },	/* OP1 */
	  { 0x21, 0x08, 0x66, 0x45, 0x00 },	/* OP2 */
	  { 0x20, 0x1c, 0x63, 0x05, 0x00 },	/* OP3 */
	  { 0x21, 0x08, 0x63, 0x04, 0x00 } },	/* OP4 */
	0x1a, 0x11
      },
      { OPL3_TYPE_4OP,	/*  53: Voice Oohs           */
	{ { 0x21, 0x16, 0xa6, 0x66, 0x00 },	/* OP1 */
	  { 0x21, 0x08, 0x77, 0x06, 0x00 },	/* OP2 */
	  { 0x20, 0xd8, 0x66, 0x36, 0x00 },	/* OP3 */
	  { 0x21, 0x08, 0x68, 0x06, 0x00 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  54: Synth Voice          */
	{ { 0x21, 0x12, 0x75, 0x66, 0x00 },	/* OP1 */
	  { 0x21, 0x08, 0x67, 0x06, 0x00 },	/* OP2 */
	  { 0x21, 0x2d, 0x62, 0x36, 0x00 },	/* OP3 */
	  { 0x21, 0x08, 0x68, 0x06, 0x00 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  55: Orchestra Hit        */
	{ { 0x02, 0x0e, 0x76, 0x34, 0x00 },	/* OP1 */
	  { 0x01, 0x08, 0x64, 0x35, 0x00 },	/* OP2 */
	  { 0x01, 0x2d, 0x66, 0x35, 0x04 },	/* OP3 */
	  { 0x00, 0x08, 0x63, 0x04, 0x04 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  56: Trumpet              */
	{ { 0x21, 0x23, 0x72, 0x9b, 0x01 },	/* OP1 */
	  { 0x21, 0x08, 0x72, 0x3b, 0x01 },	/* OP2 */
	  { 0x21, 0x23, 0x82, 0x9b, 0x01 },	/* OP3 */
	  { 0x21, 0x08, 0x92, 0x3b, 0x01 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  57: Trombone             */
	{ { 0x21, 0x23, 0x63, 0x9b, 0x01 },	/* OP1 */
	  { 0x21, 0x08, 0x62, 0x3b, 0x01 },	/* OP2 */
	  { 0x21, 0x21, 0x73, 0x9b, 0x01 },	/* OP3 */
	  { 0x21, 0x08, 0x82, 0x3b, 0x01 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  58: Tuba                 */
	{ { 0x01, 0x14, 0x62, 0x0b, 0x05 },	/* OP1 */
	  { 0x01, 0x08, 0xe6, 0x2b, 0x05 },	/* OP2 */
	  { 0x21, 0x13, 0x55, 0x1b, 0x04 },	/* OP3 */
	  { 0x22, 0x08, 0x72, 0x2b, 0x04 } },	/* OP4 */
	0x16, 0x11
      },
      { OPL3_TYPE_4OP,	/*  59: Muted Trumpet        */
	{ { 0x01, 0x11, 0x60, 0xfb, 0x05 },	/* OP1 */
	  { 0x21, 0x18, 0x62, 0x7b, 0x05 },	/* OP2 */
	  { 0x01, 0x10, 0x60, 0x3b, 0x05 },	/* OP3 */
	  { 0x21, 0x17, 0x62, 0x7b, 0x05 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_2OP,	/*  60: French Horn          */
	{ { 0x21, 0x23, 0x41, 0x94, 0x00 },	/* OP1 */
	  { 0x21, 0x08, 0x81, 0xf6, 0x00 },	/* OP2 */
	  { 0x21, 0x3f, 0x41, 0x94, 0x00 },	/* OP3 */
	  { 0x21, 0x3f, 0x81, 0xf6, 0x00 } },	/* OP4 */
	0x1e, 0x0f
      },
      { OPL3_TYPE_4OP,	/*  61: Brass Section        */
	{ { 0x22, 0x4c, 0x75, 0x37, 0x01 },	/* OP1 */
	  { 0x61, 0x48, 0xf1, 0xb7, 0x01 },	/* OP2 */
	  { 0x21, 0x4d, 0x56, 0x07, 0x05 },	/* OP3 */
	  { 0x20, 0x08, 0x72, 0x17, 0x05 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  62: Synth Brass 1        */
	{ { 0x21, 0x04, 0x82, 0xcb, 0x01 },	/* OP1 */
	  { 0x21, 0x09, 0x71, 0x2b, 0x01 },	/* OP2 */
	  { 0x21, 0x11, 0x75, 0x8b, 0x00 },	/* OP3 */
	  { 0x21, 0x08, 0x80, 0x3b, 0x00 } },	/* OP4 */
	0x16, 0x11
      },
      { OPL3_TYPE_4OP,	/*  63: Synth Brass 2        */
	{ { 0x21, 0x83, 0x84, 0x3b, 0x01 },	/* OP1 */
	  { 0x21, 0x4d, 0xa2, 0x3b, 0x01 },	/* OP2 */
	  { 0x21, 0x0e, 0x74, 0x7b, 0x00 },	/* OP3 */
	  { 0x21, 0x08, 0x73, 0x3b, 0x00 } },	/* OP4 */
	0x16, 0x11
      },
      { OPL3_TYPE_4OP,	/*  64: Soprano Sax          */
	{ { 0x01, 0x15, 0x70, 0xe7, 0x05 },	/* OP1 */
	  { 0x02, 0x08, 0x70, 0xe8, 0x05 },	/* OP2 */
	  { 0x01, 0x42, 0x70, 0xf8, 0x07 },	/* OP3 */
	  { 0x00, 0x08, 0x70, 0xe8, 0x07 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  65: Alto Sax             */
	{ { 0x01, 0x13, 0x70, 0xe9, 0x05 },	/* OP1 */
	  { 0x01, 0x08, 0x70, 0xe9, 0x05 },	/* OP2 */
	  { 0x01, 0x47, 0x70, 0xe9, 0x07 },	/* OP3 */
	  { 0x40, 0x48, 0x70, 0xe9, 0x07 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  66: Tenor Sax            */
	{ { 0x01, 0x15, 0x70, 0xeb, 0x05 },	/* OP1 */
	  { 0x01, 0x08, 0x70, 0xea, 0x05 },	/* OP2 */
	  { 0x01, 0x09, 0x70, 0xeb, 0x07 },	/* OP3 */
	  { 0x40, 0x08, 0x70, 0xeb, 0x07 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  67: Baritone Sax         */
	{ { 0x01, 0x11, 0xa4, 0xbb, 0x00 },	/* OP1 */
	  { 0x04, 0x0d, 0x67, 0x6a, 0x00 },	/* OP2 */
	  { 0x01, 0x0c, 0xd0, 0xeb, 0x01 },	/* OP3 */
	  { 0x02, 0x08, 0x80, 0xeb, 0x01 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  68: Oboe                 */
	{ { 0x11, 0x17, 0x70, 0x29, 0x00 },	/* OP1 */
	  { 0x34, 0x08, 0x61, 0x29, 0x00 },	/* OP2 */
	  { 0x31, 0xff, 0x60, 0x09, 0x00 },	/* OP3 */
	  { 0x34, 0x7f, 0x60, 0x09, 0x00 } },	/* OP4 */
	0x10, 0x01
      },
      { OPL3_TYPE_4OP,	/*  69: English Horn         */
	{ { 0x01, 0x11, 0xf0, 0x29, 0x00 },	/* OP1 */
	  { 0x24, 0x08, 0x80, 0x09, 0x00 },	/* OP2 */
	  { 0x31, 0xda, 0x80, 0x09, 0x00 },	/* OP3 */
	  { 0x24, 0x08, 0x80, 0x09, 0x00 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  70: Bassoon              */
	{ { 0x01, 0x13, 0x60, 0x22, 0x00 },	/* OP1 */
	  { 0x24, 0x0c, 0x80, 0x09, 0x00 },	/* OP2 */
	  { 0x31, 0x86, 0x80, 0x05, 0x00 },	/* OP3 */
	  { 0x24, 0x18, 0x80, 0x09, 0x00 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  71: Clarinet             */
	{ { 0x32, 0x1d, 0xa2, 0x16, 0x04 },	/* OP1 */
	  { 0x31, 0x08, 0x51, 0x26, 0x04 },	/* OP2 */
	  { 0x32, 0x45, 0xfe, 0x52, 0x04 },	/* OP3 */
	  { 0x31, 0x08, 0x55, 0x15, 0x04 } },	/* OP4 */
	0x1a, 0x11
      },
      { OPL3_TYPE_4OP,	/*  72: Piccolo              */
	{ { 0x24, 0x20, 0x67, 0xf6, 0x00 },	/* OP1 */
	  { 0xa2, 0x08, 0x65, 0x06, 0x00 },	/* OP2 */
	  { 0x24, 0x1c, 0x7f, 0x27, 0x00 },	/* OP3 */
	  { 0xa2, 0x08, 0x67, 0x06, 0x00 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  73: Flute                */
	{ { 0x22, 0x26, 0x65, 0x66, 0x00 },	/* OP1 */
	  { 0xa1, 0x10, 0x67, 0x06, 0x00 },	/* OP2 */
	  { 0x02, 0x2a, 0xa2, 0x36, 0x00 },	/* OP3 */
	  { 0x21, 0x10, 0x6f, 0x06, 0x00 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  74: Recorder             */
	{ { 0x21, 0x11, 0x85, 0x66, 0x00 },	/* OP1 */
	  { 0x31, 0x1b, 0x68, 0x06, 0x00 },	/* OP2 */
	  { 0x32, 0x19, 0xd5, 0x39, 0x00 },	/* OP3 */
	  { 0x31, 0x08, 0x68, 0x06, 0x00 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  75: Pan Flute            */
	{ { 0x20, 0x00, 0x77, 0x08, 0x00 },	/* OP1 */
	  { 0xa0, 0x15, 0x77, 0x58, 0x00 },	/* OP2 */
	  { 0x24, 0xd3, 0x76, 0xa9, 0x04 },	/* OP3 */
	  { 0x21, 0x08, 0x76, 0x08, 0x04 } },	/* OP4 */
	0x1e, 0x11
      },
      { OPL3_TYPE_4OP,	/*  76: Bottle Blow          */
	{ { 0x2b, 0x12, 0x73, 0x68, 0x00 },	/* OP1 */
	  { 0x26, 0x24, 0x67, 0x97, 0x00 },	/* OP2 */
	  { 0x22, 0x14, 0x66, 0x46, 0x00 },	/* OP3 */
	  { 0x21, 0x17, 0x60, 0xb6, 0x00 } },	/* OP4 */
	0x1e, 0x11
      },
      { OPL3_TYPE_4OP,	/*  77: Shakuhachi           */
	{ { 0x61, 0x16, 0x67, 0x56, 0x02 },	/* OP1 */
	  { 0xa1, 0x11, 0x77, 0x06, 0x02 },	/* OP2 */
	  { 0x21, 0x15, 0xd7, 0x66, 0x02 },	/* OP3 */
	  { 0x01, 0x08, 0x40, 0xb6, 0x02 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  78: Whistle              */
	{ { 0x61, 0x31, 0xc4, 0x56, 0x02 },	/* OP1 */
	  { 0xa1, 0x08, 0x50, 0xb6, 0x02 },	/* OP2 */
	  { 0x61, 0x30, 0xc4, 0x66, 0x01 },	/* OP3 */
	  { 0x81, 0x08, 0x50, 0xb6, 0x01 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  79: Ocarina              */
	{ { 0x31, 0x2b, 0x57, 0x46, 0x04 },	/* OP1 */
	  { 0x31, 0x08, 0x68, 0x06, 0x04 },	/* OP2 */
	  { 0x32, 0x30, 0x47, 0x69, 0x04 },	/* OP3 */
	  { 0x31, 0x08, 0x68, 0x06, 0x04 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  80: Lead1 squareea       */
	{ { 0x21, 0x0d, 0xd4, 0x1b, 0x04 },	/* OP1 */
	  { 0x21, 0x0d, 0xc2, 0x26, 0x04 },	/* OP2 */
	  { 0x21, 0x15, 0xc6, 0x19, 0x04 },	/* OP3 */
	  { 0x21, 0x15, 0xd0, 0x27, 0x04 } },	/* OP4 */
	0x11, 0x11
      },
      { OPL3_TYPE_4OP,	/*  81: Lead2 sawtooth       */
	{ { 0x21, 0x11, 0xd4, 0x1b, 0x04 },	/* OP1 */
	  { 0x20, 0x08, 0xc2, 0x36, 0x04 },	/* OP2 */
	  { 0x21, 0x10, 0xc9, 0x49, 0x04 },	/* OP3 */
	  { 0x21, 0x08, 0xd0, 0x37, 0x04 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  82: Lead 3 calliope      */
	{ { 0x22, 0x0e, 0x76, 0x66, 0x00 },	/* OP1 */
	  { 0xa1, 0x14, 0x67, 0x06, 0x00 },	/* OP2 */
	  { 0x22, 0x1e, 0x75, 0x06, 0x02 },	/* OP3 */
	  { 0x21, 0x14, 0x75, 0x06, 0x02 } },	/* OP4 */
	0x16, 0x11
      },
      { OPL3_TYPE_4OP,	/*  83: Lead4 chiff          */
	{ { 0xd1, 0x00, 0xb7, 0x66, 0x02 },	/* OP1 */
	  { 0xa0, 0x08, 0x87, 0x06, 0x02 },	/* OP2 */
	  { 0x01, 0x14, 0x75, 0x66, 0x02 },	/* OP3 */
	  { 0x60, 0x08, 0xb5, 0x06, 0x02 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  84: Lead 5 charang       */
	{ { 0x30, 0x0c, 0xf4, 0xa0, 0x00 },	/* OP1 */
	  { 0x22, 0x0c, 0x62, 0x0e, 0x00 },	/* OP2 */
	  { 0x20, 0x14, 0x77, 0x4e, 0x00 },	/* OP3 */
	  { 0x21, 0x0c, 0x90, 0x1c, 0x00 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  85: Lead 6 voice         */
	{ { 0x20, 0x1a, 0x76, 0x64, 0x00 },	/* OP1 */
	  { 0x32, 0x0c, 0x47, 0x05, 0x00 },	/* OP2 */
	  { 0x31, 0x1b, 0x75, 0x03, 0x00 },	/* OP3 */
	  { 0x21, 0x0c, 0x75, 0x05, 0x00 } },	/* OP4 */
	0x16, 0x11
      },
      { OPL3_TYPE_4OP,	/*  86: Lead 7 fifths        */
	{ { 0x03, 0x0c, 0xf2, 0xa6, 0x00 },	/* OP1 */
	  { 0x06, 0x3f, 0xf0, 0xfb, 0x00 },	/* OP2 */
	  { 0x01, 0x15, 0xf0, 0xf8, 0x00 },	/* OP3 */
	  { 0x00, 0x14, 0xf3, 0xc8, 0x00 } },	/* OP4 */
	0x17, 0x10
      },
      { OPL3_TYPE_4OP,	/*  87: Lead8 brass+ld       */
	{ { 0x02, 0x0b, 0xf3, 0xfb, 0x01 },	/* OP1 */
	  { 0x21, 0x09, 0xf1, 0x2b, 0x01 },	/* OP2 */
	  { 0x01, 0xe1, 0xf3, 0xfb, 0x05 },	/* OP3 */
	  { 0x21, 0x10, 0xf0, 0x2b, 0x05 } },	/* OP4 */
	0x16, 0x11
      },
      { OPL3_TYPE_4OP,	/*  88: Pad 1 new age        */
	{ { 0x12, 0xc0, 0xf2, 0x23, 0x00 },	/* OP1 */
	  { 0x12, 0x14, 0xe2, 0x55, 0x00 },	/* OP2 */
	  { 0x31, 0x1c, 0x69, 0x33, 0x02 },	/* OP3 */
	  { 0x30, 0x0d, 0x52, 0x15, 0x02 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  89: Pad 2 warm           */
	{ { 0x01, 0x18, 0x21, 0x53, 0x00 },	/* OP1 */
	  { 0x31, 0x08, 0x22, 0x55, 0x00 },	/* OP2 */
	  { 0x11, 0xd4, 0x21, 0x53, 0x01 },	/* OP3 */
	  { 0x31, 0x08, 0x21, 0x45, 0x01 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  90: Pad3 polysynth       */
	{ { 0x20, 0x00, 0xc3, 0xf4, 0x02 },	/* OP1 */
	  { 0x01, 0x08, 0xf0, 0xf5, 0x02 },	/* OP2 */
	  { 0x01, 0x08, 0x32, 0x14, 0x05 },	/* OP3 */
	  { 0x21, 0x12, 0x20, 0x04, 0x05 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_2OP,	/*  91: Pad 4 choir          */
	{ { 0x30, 0xd1, 0xa2, 0x40, 0x00 },	/* OP1 */
	  { 0x20, 0x04, 0x63, 0x48, 0x00 },	/* OP2 */
	  { 0x00, 0x3f, 0x00, 0x00, 0x00 },	/* OP3 */
	  { 0x00, 0x3f, 0x00, 0x00, 0x00 } },	/* OP4 */
	0x10, 0x00
      },
      { OPL3_TYPE_4OP,	/*  92: Pad 5 bowed          */
	{ { 0x06, 0x08, 0xf0, 0xe4, 0x00 },	/* OP1 */
	  { 0x21, 0x16, 0x45, 0x36, 0x00 },	/* OP2 */
	  { 0x01, 0x46, 0xe5, 0x34, 0x04 },	/* OP3 */
	  { 0x21, 0x0c, 0x40, 0x05, 0x04 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  93: Pad6 metallic        */
	{ { 0x21, 0x40, 0x63, 0x83, 0x00 },	/* OP1 */
	  { 0x21, 0x08, 0x30, 0x13, 0x00 },	/* OP2 */
	  { 0x21, 0x17, 0x63, 0xa3, 0x05 },	/* OP3 */
	  { 0x21, 0x09, 0x40, 0x13, 0x05 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  94: Pad 7 halo           */
	{ { 0x31, 0x08, 0x60, 0x15, 0x00 },	/* OP1 */
	  { 0x30, 0xc4, 0x64, 0xf8, 0x00 },	/* OP2 */
	  { 0x31, 0x14, 0xb0, 0xc5, 0x00 },	/* OP3 */
	  { 0x31, 0x0c, 0xd0, 0x45, 0x00 } },	/* OP4 */
	0x15, 0x11
      },
      { OPL3_TYPE_4OP,	/*  95: Pad 8 sweep          */
	{ { 0x31, 0x00, 0x44, 0xf4, 0x00 },	/* OP1 */
	  { 0x12, 0x8c, 0x12, 0xb3, 0x00 },	/* OP2 */
	  { 0x10, 0x10, 0x30, 0xb4, 0x01 },	/* OP3 */
	  { 0x30, 0x08, 0x74, 0x04, 0x01 } },	/* OP4 */
	0x15, 0x11
      },
      { OPL3_TYPE_4OP,	/*  96: FX1 rain             */
	{ { 0x08, 0xc0, 0xfb, 0x66, 0x00 },	/* OP1 */
	  { 0x83, 0x08, 0xf5, 0x68, 0x00 },	/* OP2 */
	  { 0x21, 0x14, 0xf0, 0x25, 0x00 },	/* OP3 */
	  { 0x21, 0x08, 0xde, 0x05, 0x00 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  97: FX2 soundtrack       */
	{ { 0x08, 0x01, 0x33, 0xf2, 0x00 },	/* OP1 */
	  { 0x01, 0x17, 0x31, 0xf2, 0x00 },	/* OP2 */
	  { 0x01, 0x04, 0x31, 0xf2, 0x01 },	/* OP3 */
	  { 0x01, 0x08, 0x32, 0xf2, 0x01 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/*  98: FX3 crystal          */
	{ { 0x17, 0xc0, 0xf1, 0x31, 0x00 },	/* OP1 */
	  { 0x91, 0x08, 0xf1, 0x22, 0x00 },	/* OP2 */
	  { 0x95, 0x9b, 0xf1, 0x21, 0x00 },	/* OP3 */
	  { 0x91, 0x09, 0xf1, 0x23, 0x00 } },	/* OP4 */
	0x18, 0x11
      },
      { OPL3_TYPE_4OP,	/*  99: FX 4 atmosphere      */
	{ { 0x01, 0x00, 0xd2, 0x24, 0x00 },	/* OP1 */
	  { 0x17, 0x0c, 0xd8, 0x34, 0x00 },	/* OP2 */
	  { 0x01, 0x17, 0xea, 0x34, 0x02 },	/* OP3 */
	  { 0x20, 0x08, 0xe1, 0x14, 0x02 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/* 100: FX5 brightness       */
	{ { 0x06, 0x08, 0xf0, 0xe4, 0x00 },	/* OP1 */
	  { 0x01, 0x08, 0xf5, 0x34, 0x00 },	/* OP2 */
	  { 0x01, 0x44, 0xe5, 0x34, 0x02 },	/* OP3 */
	  { 0xa0, 0x08, 0xe0, 0x44, 0x02 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/* 101: FX6 goblins          */
	{ { 0x04, 0x02, 0x11, 0x32, 0x00 },	/* OP1 */
	  { 0x01, 0x08, 0x21, 0x22, 0x00 },	/* OP2 */
	  { 0x01, 0x41, 0x11, 0x24, 0x00 },	/* OP3 */
	  { 0x01, 0x08, 0x11, 0x23, 0x00 } },	/* OP4 */
	0x18, 0x11
      },
      { OPL3_TYPE_4OP,	/* 102: FX 7 echoes          */
	{ { 0x05, 0xc0, 0xf8, 0xd3, 0x00 },	/* OP1 */
	  { 0x01, 0x0c, 0xe1, 0x03, 0x00 },	/* OP2 */
	  { 0x21, 0x10, 0x69, 0x33, 0x01 },	/* OP3 */
	  { 0x21, 0x08, 0x52, 0x13, 0x01 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/* 103: FX8 sci-fi           */
	{ { 0x02, 0x00, 0x83, 0xf3, 0x01 },	/* OP1 */
	  { 0x21, 0x08, 0x61, 0x23, 0x01 },	/* OP2 */
	  { 0x01, 0x0b, 0x73, 0xf3, 0x05 },	/* OP3 */
	  { 0x21, 0x08, 0x52, 0x23, 0x05 } },	/* OP4 */
	0x16, 0x11
      },
      { OPL3_TYPE_4OP,	/* 104: Sitar                */
	{ { 0x25, 0x12, 0xf7, 0x44, 0x04 },	/* OP1 */
	  { 0x01, 0x0f, 0xf8, 0x04, 0x04 },	/* OP2 */
	  { 0xa7, 0x17, 0x80, 0x64, 0x04 },	/* OP3 */
	  { 0x01, 0x10, 0xf3, 0x74, 0x04 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/* 105: Banjo                */
	{ { 0x01, 0x08, 0xf4, 0xa6, 0x04 },	/* OP1 */
	  { 0x05, 0x25, 0xf8, 0xfb, 0x04 },	/* OP2 */
	  { 0x01, 0x10, 0xf4, 0xfb, 0x00 },	/* OP3 */
	  { 0x02, 0x08, 0xf6, 0xc7, 0x00 } },	/* OP4 */
	0x15, 0x10
      },
      { OPL3_TYPE_4OP,	/* 106: Shamisen             */
	{ { 0x04, 0x09, 0xfa, 0xa6, 0x00 },	/* OP1 */
	  { 0x07, 0x08, 0xf8, 0xfb, 0x00 },	/* OP2 */
	  { 0x01, 0x16, 0xf3, 0x95, 0x04 },	/* OP3 */
	  { 0x01, 0x08, 0xf3, 0xc5, 0x04 } },	/* OP4 */
	0x14, 0x11
      },
      { OPL3_TYPE_4OP,	/* 107: Koto                 */
	{ { 0x01, 0x08, 0xf4, 0xa4, 0x04 },	/* OP1 */
	  { 0x05, 0x25, 0xfa, 0xfb, 0x04 },	/* OP2 */
	  { 0x01, 0x18, 0xf4, 0xf5, 0x04 },	/* OP3 */
	  { 0x01, 0x08, 0xf3, 0xc5, 0x04 } },	/* OP4 */
	0x15, 0x10
      },
      { OPL3_TYPE_4OP,	/* 108: Kalimba              */
	{ { 0x09, 0x00, 0xf8, 0x6e, 0x04 },	/* OP1 */
	  { 0x03, 0x08, 0xf8, 0x6e, 0x04 },	/* OP2 */
	  { 0x01, 0x00, 0xf4, 0x16, 0x00 },	/* OP3 */
	  { 0x86, 0x09, 0xf7, 0xea, 0x00 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/* 109: Bagpipe              */
	{ { 0x11, 0x0c, 0xd0, 0xf3, 0x00 },	/* OP1 */
	  { 0x32, 0x08, 0x80, 0x05, 0x00 },	/* OP2 */
	  { 0x31, 0xc9, 0x40, 0x09, 0x00 },	/* OP3 */
	  { 0x24, 0x0c, 0x50, 0x09, 0x00 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/* 110: Fiddle               */
	{ { 0x24, 0xc9, 0xaa, 0x58, 0x04 },	/* OP1 */
	  { 0x20, 0x0c, 0x95, 0x08, 0x04 },	/* OP2 */
	  { 0x24, 0xc9, 0xc7, 0x88, 0x05 },	/* OP3 */
	  { 0x20, 0x08, 0x72, 0x28, 0x05 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/* 111: Shanai               */
	{ { 0x11, 0x0c, 0xd0, 0xf3, 0x00 },	/* OP1 */
	  { 0x32, 0x08, 0x80, 0x05, 0x00 },	/* OP2 */
	  { 0x31, 0xcc, 0x60, 0x06, 0x00 },	/* OP3 */
	  { 0x24, 0x0c, 0x60, 0x06, 0x00 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/* 112: Tinkle Bell          */
	{ { 0x16, 0x4f, 0xf6, 0x53, 0x00 },	/* OP1 */
	  { 0x19, 0x08, 0xf3, 0x53, 0x00 },	/* OP2 */
	  { 0x15, 0x41, 0xf1, 0x52, 0x06 },	/* OP3 */
	  { 0x14, 0x08, 0xf3, 0x52, 0x06 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/* 113: Agogo Bells          */
	{ { 0x17, 0x00, 0xfa, 0x57, 0x00 },	/* OP1 */
	  { 0x12, 0x08, 0xf5, 0x58, 0x00 },	/* OP2 */
	  { 0x14, 0x40, 0xf7, 0x52, 0x06 },	/* OP3 */
	  { 0x12, 0x08, 0xf5, 0x59, 0x06 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/* 114: Steel Drums          */
	{ { 0x08, 0x1f, 0xc6, 0x37, 0x00 },	/* OP1 */
	  { 0x01, 0x08, 0x82, 0x95, 0x00 },	/* OP2 */
	  { 0x05, 0x0f, 0x65, 0x55, 0x00 },	/* OP3 */
	  { 0x11, 0x0f, 0x52, 0x75, 0x00 } },	/* OP4 */
	0x16, 0x11
      },
      { OPL3_TYPE_4OP,	/* 115: Woodblock            */
	{ { 0x02, 0x09, 0xfc, 0xd6, 0x02 },	/* OP1 */
	  { 0x01, 0x08, 0xf7, 0xf7, 0x02 },	/* OP2 */
	  { 0x07, 0x3f, 0xfa, 0x4e, 0x00 },	/* OP3 */
	  { 0x03, 0xff, 0xf5, 0xfe, 0x00 } },	/* OP4 */
	0x10, 0x01
      },
      { OPL3_TYPE_4OP,	/* 116: Taiko Drum           */
	{ { 0x1a, 0x00, 0xfb, 0x57, 0x00 },	/* OP1 */
	  { 0x30, 0x00, 0xf3, 0x54, 0x00 },	/* OP2 */
	  { 0x11, 0x48, 0xc5, 0x52, 0x01 },	/* OP3 */
	  { 0x10, 0x08, 0xc5, 0x57, 0x01 } },	/* OP4 */
	0x11, 0x11
      },
      { OPL3_TYPE_4OP,	/* 117: Melodic Tom          */
	{ { 0x12, 0x01, 0xfb, 0xa7, 0x00 },	/* OP1 */
	  { 0x30, 0x08, 0xf3, 0x53, 0x00 },	/* OP2 */
	  { 0x10, 0xff, 0xf4, 0x52, 0x00 },	/* OP3 */
	  { 0x10, 0x08, 0xc4, 0x57, 0x00 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/* 118: Synth Drum           */
	{ { 0x2b, 0x00, 0xff, 0x0e, 0x00 },	/* OP1 */
	  { 0x0b, 0x08, 0xf7, 0xfe, 0x00 },	/* OP2 */
	  { 0x00, 0xc0, 0xf6, 0xfe, 0x02 },	/* OP3 */
	  { 0x20, 0x09, 0xff, 0x0c, 0x02 } },	/* OP4 */
	0x1e, 0x11
      },
      { OPL3_TYPE_4OP,	/* 119: Reverse Cymbal       */
	{ { 0x2a, 0x00, 0xff, 0x0e, 0x04 },	/* OP1 */
	  { 0x0b, 0x08, 0x2f, 0x0e, 0x04 },	/* OP2 */
	  { 0x28, 0x00, 0xf9, 0x0e, 0x04 },	/* OP3 */
	  { 0x09, 0x08, 0x29, 0x0e, 0x04 } },	/* OP4 */
	0x1e, 0x11
      },
      { OPL3_TYPE_4OP,	/* 120: Guitar Fret Noise    */
	{ { 0x0b, 0x00, 0x64, 0xf8, 0x04 },	/* OP1 */
	  { 0x0b, 0x25, 0x77, 0xf8, 0x04 },	/* OP2 */
	  { 0x0b, 0x10, 0x56, 0xf8, 0x04 },	/* OP3 */
	  { 0x09, 0x08, 0x59, 0xe8, 0x04 } },	/* OP4 */
	0x1e, 0x11
      },
      { OPL3_TYPE_4OP,	/* 121: Breath Noise         */
	{ { 0x2b, 0x05, 0x79, 0x05, 0x00 },	/* OP1 */
	  { 0x25, 0x08, 0x77, 0xa5, 0x00 },	/* OP2 */
	  { 0x26, 0x10, 0x66, 0xb9, 0x04 },	/* OP3 */
	  { 0x01, 0x0a, 0x76, 0xde, 0x04 } },	/* OP4 */
	0x1e, 0x11
      },
      { OPL3_TYPE_2OP,	/* 122: Seashore             */
	{ { 0x30, 0x00, 0x11, 0x00, 0x00 },	/* OP1 */
	  { 0x10, 0x08, 0x11, 0x32, 0x00 },	/* OP2 */
	  { 0x26, 0x3f, 0x66, 0xb9, 0x04 },	/* OP3 */
	  { 0x01, 0x3f, 0x76, 0xde, 0x04 } },	/* OP4 */
	0x1e, 0x01
      },
      { OPL3_TYPE_2OP,	/* 123: Bird Tweet           */
	{ { 0x0a, 0x00, 0x59, 0xfe, 0x00 },	/* OP1 */
	  { 0x09, 0x00, 0x69, 0xfe, 0x00 },	/* OP2 */
	  { 0x07, 0x3f, 0xfc, 0xfe, 0x03 },	/* OP3 */
	  { 0x09, 0x3f, 0x79, 0xf7, 0x03 } },	/* OP4 */
	0x11, 0x01
      },
      { OPL3_TYPE_4OP,	/* 124: Telephone            */
	{ { 0x0a, 0x0b, 0xe5, 0xf5, 0x05 },	/* OP1 */
	  { 0x02, 0x08, 0xe4, 0xf5, 0x05 },	/* OP2 */
	  { 0x08, 0x43, 0xf5, 0xf5, 0x03 },	/* OP3 */
	  { 0x02, 0x08, 0xf4, 0xf5, 0x03 } },	/* OP4 */
	0x10, 0x11
      },
      { OPL3_TYPE_4OP,	/* 125: Helicopter Blade     */
	{ { 0x0b, 0x00, 0x82, 0xfa, 0x00 },	/* OP1 */
	  { 0x04, 0x88, 0x77, 0xfa, 0x00 },	/* OP2 */
	  { 0x20, 0x11, 0xf7, 0xfa, 0x00 },	/* OP3 */
	  { 0x20, 0x1d, 0xfa, 0xfb, 0x00 } },	/* OP4 */
	0x1e, 0x11
      },
      { OPL3_TYPE_4OP,	/* 126: Applause/Noise       */
	{ { 0x2b, 0x00, 0xf0, 0x01, 0x00 },	/* OP1 */
	  { 0x27, 0x08, 0x23, 0x13, 0x00 },	/* OP2 */
	  { 0x20, 0x7f, 0xfb, 0xe5, 0x00 },	/* OP3 */
	  { 0x20, 0x3f, 0xf4, 0xf5, 0x00 } },	/* OP4 */
	0x1e, 0x01
      },
      { OPL3_TYPE_4OP,	/* 127: Gunshot              */
	{ { 0x0b, 0x40, 0xf5, 0x67, 0x00 },	/* OP1 */
	  { 0x01, 0xc8, 0xf7, 0x64, 0x00 },	/* OP2 */
	  { 0x07, 0x3f, 0xf7, 0x34, 0x04 },	/* OP3 */
	  { 0x01, 0x00, 0xfa, 0x9e, 0x04 } },	/* OP4 */
	0x0e, 0x01
      }
};
