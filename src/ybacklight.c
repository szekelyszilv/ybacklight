#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define DRIVER "intel_backlight"
#define DRIVER_PATH "/sys/class/backlight/" DRIVER
#define BRIGHTNESS DRIVER_PATH "/brightness"
#define MAX_BRIGHTNESS DRIVER_PATH "/max_brightness"

int read_sysfs_int(const char* path)
{
	FILE *f = fopen(path, "r");
	if (!f) {
		return -1;
	}

	int value;

	if(fscanf(f, "%d\n", &value) != 1) {
		value = -1;
	}

	fclose(f);
	return value;
}

int write_sysfs_int(const char* path, int value)
{
	FILE *f = fopen(path, "w");
	if (!f) {
		return -1;
	}

	int ret = 0;

	if(fprintf(f, "%d\n", value) < 0) {
		ret = -1;
	}

	fclose(f);
	return ret;
}

int max_brightness()
{
	return read_sysfs_int(MAX_BRIGHTNESS);
}

int brightness()
{
	int bcur = read_sysfs_int(BRIGHTNESS);
	int bmax = max_brightness();
	return (bcur * 100) / bmax;
}

int set_brightness(int value)
{
	int bval = (max_brightness() * value) / 100;
	return write_sysfs_int(BRIGHTNESS, bval);
}

int inc_brightness(int value)
{
	value = brightness() + value;

	if (value > 100) {
		value = 100;
	}

	return set_brightness(value);
}

int dec_brightness(int value)
{
	value = brightness() - value;

	if (value < 0) {
		value = 0;
	}

	return set_brightness(value);
}

int main(int argc, char *argv[])
{
	char *command;
	int value;


	if (argc < 2) {
		goto usage;
	}

	command = argv[1];

	if (strcmp(command, "-get") == 0) {
		printf("%d\n", brightness());
		return 0;
	}

	if (argc < 3) {
		goto usage;
	}

	value = strtol(argv[2], NULL, 10);

	if (value < 0 || value > 100) {
		goto usage;
	}

	if (strcmp(command, "-set") == 0) {
		if(set_brightness(value)) {
			goto fail;
		}
		return 0;
	}

	if (strcmp(command, "-inc") == 0) {
		if(inc_brightness(value)) {
			goto fail;
		}
		return 0;
	}

	if (strcmp(command, "-dec") == 0) {
		if(dec_brightness(value)) {
			goto fail;
		}
		return 0;
	}

fail:
	perror("failed to set brightness");
	return 2;

usage:
	fprintf(stderr, "Usage: ybacklight [-get|-set|-inc|-dec] [amount]\n");
	return 1;
}
