/*
 * Copyright 2012 Federico Vaga <federico.vaga@gmail.com>
 *
 * lszio is a ZIO tool which show details about connected ZIO device.
 * It is a clone of lspci, but for ZIO devices.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <linux/zio-user.h>

#include <libzio.h>

/* print program help */
static void zds_help()
{
	printf("\nlszio [options]\n\n");
	printf("The program list connected ZIO devices\n\n");
	printf("Options:");
	printf("-v: verbose output, show device attributes status\n");
	printf("-d <device name>: look for a particular device\n");
	printf("-w: show ZIO hardware device\n");
	printf("-h: show this help\n\n");
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

static void zds_print_all_attributes(char *name)
{
	struct zio_device *zdev;
	struct zio_cset *cset;
	struct zio_channel *chan;
	int i, j;

	zdev = zio_create_device(name);
	if (!zdev) {
		printf("Error");
		exit(1);
	}

	printf("    Device name: %s\n", name);
	zds_print_attributes(zdev->attr, zdev->n_attr, "    ");

	for (i = 0; i < zdev->n_cset; ++i) {
		cset = &zdev->cset[i];
		printf("\n        Channel Set: %s\n", cset->dir->name);
		zds_print_attributes(cset->attr, cset->n_attr, "        ");
		if (cset->trg) {
			printf("\n            Trigger: %s\n", cset->trg->dir->name);
			zds_print_attributes(cset->trg->attr, cset->trg->n_attr, "            ");
		}
		for (j = 0; j < cset->n_chan; ++j) {
			chan = &cset->chan[j];
			printf("\n            Channel: %s\n", chan->dir->name);
			zds_print_attributes(chan->attr, chan->n_attr, "            ");
			if (chan->buf) {
				printf("\n                Buffer: %s\n", chan->buf->dir->name);
				zds_print_attributes(chan->buf->attr, chan->buf->n_attr, "                ");
			}
		}
	}

	zio_destroy_device(zdev);
}

int main(int argc, char *argv[])
{
	struct dirent **namelist;
	int c, v = 0, n, i;
	int show_hw = 0;
	char *device = NULL;

	while ((c = getopt (argc, argv, "d:vwh")) != -1) {
		switch (c) {
		case 'd':
			device = optarg;
			break;
		case 'v':
			v = 1;
			break;
		case 'w':
			show_hw = 1;
			break;
		case 'h':
			zds_help();
			exit(1);
			break;
		}
	}

	n = scandir(ZIO_DIR_DEV, &namelist, 0, alphasort);
	if (n < 0)
		exit(1);
	for (i = 0; i < n; ++i) {
		/* If search is disabled, look only for specified device */
		if (device && strcmp(namelist[i]->d_name, device))
			continue;

		if (!strcmp(namelist[i]->d_name, ".") || !strcmp(namelist[i]->d_name, ".."))
			continue;
		if (!show_hw && !strncmp("hw-", namelist[i]->d_name, 3)) {
			continue;
		}

		printf("%s\n", namelist[i]->d_name);
		if (v)
			zds_print_all_attributes(namelist[i]->d_name);
	}

	exit(0);
}
