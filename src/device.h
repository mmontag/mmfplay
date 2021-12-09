
#ifndef __DEVICE_H
#define __DEVICE_H

#include "list.h"

struct device {
	char *name;
        char *description;
        int (*init)(void);
        void (*deinit)(void);
	void (*update)(void);
};

struct device_list {
	struct list_head list;
	struct device *dev;
};

extern struct device *dev;

void __init_device(void);


#endif
