
#ifndef __TREE_H
#define __TREE_H

void print_data(unsigned char *buf, int size);
unsigned char *dump_data(unsigned char *buf, int size);
void print_tree(int leaf);
int print_text(char *fmt, ...);
int print_leaf(int last, char *fmt, ...);
void new_branch(void);
void end_branch(void);

#endif


