/*  Sarien - A Sierra AGI resource interpreter engine
 *  Copyright (C) 1999-2001 Stuart George and Claudio Matsuoka
 *
 *  $Id: chunk.h,v 1.1 2004/06/29 13:16:43 cmatsuoka Exp $
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; see docs/COPYING for further details.
 */

#ifndef __CHUNK_H
#define __CHUNK_H


struct chunk_info {
    char id[4];
    int id_size;
    void (*handler)();
    struct chunk_info *next;
    struct chunk_info *prev;
};

void register_chunk(struct chunk_info **, char *, int, void(*)(unsigned char *, int));
void release_chunks(struct chunk_info **);
int process_chunk(struct chunk_info *, char *, unsigned char *, int);

#endif /* __CHUNK_H */
