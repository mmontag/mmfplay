/*  Sarien - A Sierra AGI resource interpreter engine
 *  Copyright (C) 1999-2001 Stuart George and Claudio Matsuoka
 *
 *  $Id: chunk.h,v 1.2 2004/06/30 17:24:12 cmatsuoka Exp $
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; see docs/COPYING for further details.
 */

#ifndef __CHUNK_H
#define __CHUNK_H

#include "list.h"

struct chunk_info {
	struct list_head list;
	char id[4];
	int id_size;
	void (*handler)();
};

void register_chunk(struct list_head *, char *, int, void(*)(unsigned char *, int));
void release_chunks(struct list_head *);
int process_chunk(struct list_head *, char *, unsigned char *, int);

#endif /* __CHUNK_H */
