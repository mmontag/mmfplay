
#ifndef __DEVICE_H
#define __DEVICE_H


struct device {
        char *description;
        int (*init)(void);
        void (*deinit)(void);
	void (*update)(void);
};

extern struct device *dev;

void __init_device(void);


#endif
