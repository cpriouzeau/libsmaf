/*
 * libsmaf.c
 *
 * Copyright (C) Linaro SA 2015
 * Author: Benjamin Gaignard <benjamin.gaignard@linaro.org> for Linaro.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Suite 500, Boston, MA  02110-1335  USA
 */

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "smaf.h"
#include "libsmaf.h"

#define SMAF_DEV "/dev/smaf"

static int open_count = 0;
static int smaf_fd = -1;

int smaf_open(void)
{
	if (open_count)
		goto add;

	smaf_fd = open(SMAF_DEV, O_RDWR, 0);

	if (smaf_fd == -1)
		return -1;

add:
	open_count++;
	return 0;
}

void smaf_close(void)
{
	open_count--;

	if (open_count)
		return;

	close(smaf_fd);
	smaf_fd = -1;
}

int smaf_create_buffer(unsigned int length, unsigned int flags, int *fd)
{
	struct smaf_create_data create;
	int ret;

	if (smaf_fd == -1)
		return -1;

	memset (&create, 0, sizeof (create));

	create.length = length;
	create.flags = flags;

	ret = ioctl(smaf_fd, SMAF_IOC_CREATE, &create);
	if (ret) {
		*fd = -1;
		return ret;
	}

	*fd = create.fd;
	return 0;
}

int smaf_set_secure(int fd, int secure)
{
	struct smaf_secure_flag flag;
	int ret;

	if (smaf_fd == -1)
		return -1;

	memset (&flag, 0, sizeof(flag));
	flag.fd = fd;
	flag.secure = secure;

	ret = ioctl(smaf_fd, SMAF_IOC_SET_SECURE_FLAG, &flag);

	return ret;
}

int smaf_get_secure(int fd)
{
	struct smaf_secure_flag flag;
	int ret;

	if (smaf_fd == -1)
		return 0;

	memset (&flag, 0, sizeof(flag));
	flag.fd = fd;

	ret = ioctl(smaf_fd, SMAF_IOC_GET_SECURE_FLAG, &flag);

	return flag.secure;
}

int smaf_select_allocator(int fd, char *name)
{
	struct smaf_select_by_name select;
	int ret;

	if (smaf_fd == -1)
		return -1;

	memset(&select, 0, sizeof(select));
	select.fd = fd;
	strncpy(select.name, name, sizeof(select.name));

	ret = ioctl(smaf_fd, SMAF_IOC_SELECT_BY_NAME, &select);

	return ret;
}
