/* Extended Module Player
 * Copyright (C) 1997 Claudio Matsuoka and Hipolito Carraro Jr
 * $Id: chunk.c,v 1.1 2004/06/29 13:16:43 cmatsuoka Exp $
 *
 * This file is part of the Extended Module Player and is distributed
 * under the terms of the GNU General Public License. See doc/COPYING
 * for more information.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "chunk.h"

/*static struct chunk_info *chunk_head = NULL;*/


void register_chunk(struct chunk_info **head, char *id, int size, void (*handler)(unsigned char *, int))
{
	struct chunk_info *f;

	f = malloc (sizeof (struct chunk_info));
	strcpy (f->id, id);
	f->id_size = size;
	f->handler = handler;
	if (!*head) {
		*head = f;
		f->prev = NULL;
	} else {
		struct chunk_info *i;
		for (i = *head; i->next; i = i->next) {}
		i->next = f;
		f->prev = i;
	}
	f->next = NULL;
}

void release_chunks(struct chunk_info **head)
{
	struct chunk_info *i;

	for (i = *head; i->next; i = i->next) {}
	while (i->prev) {
		i = i->prev;
		free (i->next);
		i->next = NULL;
	}
	free (*head);
	*head = NULL;
}

int process_chunk(struct chunk_info *head, char *id, unsigned char *buffer, int size)
{
	struct chunk_info *i = NULL;

	for (i = head; i; i = i->next) {
		if (id && !strncmp(id, i->id, i->id_size)) {
			i->handler(buffer, size);
			break;
		}
	}

	return 0;
}

