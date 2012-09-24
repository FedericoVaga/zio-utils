#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <linux/zio-user.h>

#include <libzio.h>

/* print program help */
static void zds_help()
{
	printf("zio-dev-status DEVICE\n\n");
	printf("The program print the status of all ZIO attributes of DEVICE\n\n");
	printf("DEVICE: ZIO name of the device\n\n");
}

static void zds_print_attributes(struct sysfs_attr **attr, unsigned int n, char *space)
{
	uint32_t val;
	int i, err;

	for (i = 0; i < n; ++i) {
		if (attr[i]->md & (S_IRUSR | S_IRGRP | S_IROTH)) {
			err = zio_read_attribute(attr[i], &val);
			if (err)
				printf("%s%s: error\n", space, attr[i]->name);
			else
				printf("%s%s: %d\n", space, attr[i]->name, val);

		} else {
			printf("%s%s: write-only\n", space, attr[i]->name);
		}
	}
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
	zds_print_attributes(zdev->attr, zdev->n_attr, "");

	for (i = 0; i < zdev->n_cset; ++i) {
		cset = &zdev->cset[i];
		printf("\n  Channel Set: %s\n", cset->dir->name);
		zds_print_attributes(cset->attr, cset->n_attr, "  ");
		if (cset->trg) {
			printf("\n    Trigger: %s\n", cset->trg->dir->name);
			zds_print_attributes(cset->trg->attr, cset->trg->n_attr, "    ");
		}
		for (j = 0; j < cset->n_chan; ++j) {
			chan = &cset->chan[j];
			printf("\n    Channel: %s\n", chan->dir->name);
			zds_print_attributes(chan->attr, chan->n_attr, "    ");
			if (chan->buf) {
				printf("\n    Buffer: %s\n", chan->buf->dir->name);
				zds_print_attributes(chan->buf->attr, chan->buf->n_attr, "  ");
			}
		}
	}
	zio_destroy_device(zdev);
	exit(0);
}
