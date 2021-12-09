/* Extended Module Player
 * Copyright (C) 1997 Claudio Matsuoka and Hipolito Carraro Jr
 * $Id: chunk.c,v 1.3 2004/06/30 17:56:22 cmatsuoka Exp $
 *
 * This file is part of the Extended Module Player and is distributed
 * under the terms of the GNU General Public License. See doc/COPYING
 * for more information.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "chunk.h"


void register_chunk(struct list_head *head, char *id, int size, void (*handler)(unsigned char *, int))
{
	struct chunk_info *i;

	i = malloc(sizeof (struct chunk_info));
	strcpy(i->id, id);
	i->id_size = size;
	i->handler = handler;

	list_add_tail(&i->list, head);
}

void release_chunks(struct list_head *head)
{
	struct list_head *h;
	struct chunk_info *i;

	list_for_each (h, head, prev) {
		i = list_entry(h, struct chunk_info, list);
		list_del(h);
	}
}

int process_chunk(struct list_head *head, char *id, unsigned char *buffer, int size)
{
	struct list_head *h;
	struct chunk_info *i;

	list_for_each (h, head, next) {
		i = list_entry(h, struct chunk_info, list);
		if (id && !strncmp(id, i->id, i->id_size)) {
			i->handler(buffer, size);
			break;
		}
	}

	return 0;
}

