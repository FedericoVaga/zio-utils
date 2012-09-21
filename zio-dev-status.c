#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>

#include <linux/zio-user.h>

#include <libzio.h>

/* print program help */
static void zds_help()
{
	printf("zio-dev-status DEVICE\n\n");
	printf("The program print the status of all ZIO attributes of DEVICE\n\n");
	printf("DEVICE: ZIO name of the device\n\n");
}

static void zds_print_attribute(struct sysfs_attr *attr, unsigned int n)
{

}

int main(int argc, char *argv[])
{
	struct zio_device *zdev;
	struct zio_cset *cset;
	struct zio_channel *chan;
	unsigned int i, j;

	if (argc == 1) {
		zds_help();
		exit(1);
	}

	zdev = zio_create_device(argv[1]);
	if (!zdev) {
		printf("Error");
		exit(1);
	}

	printf("Device status\n");
	printf("Device name: %s\n", argv[1]);
	for (i = 0; i < zdev->n_cset; ++i) {
		cset = &zdev->cset[i];
		printf("Channel Set: %s\n", cset->dir->name);
		for (j = 0; j < cset->n_chan; ++j) {
			chan = cset->chan[j];
			printf("Channel: %s\n", chan->dir->name);
		}
	}
	exit(0);
}
