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

int main(int argc, char *argv[])
{
	struct zio_device *zdev;

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

	exit(0);
}
