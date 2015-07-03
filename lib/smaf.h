/*
 * smaf.h
 *
 * Copyright (C) Linaro SA 2015
 * Author: Benjamin Gaignard <benjamin.gaignard@linaro.org> for Linaro.
 * License terms:  GNU General Public License (GPL), version 2
 */

#ifndef _UAPI_SMAF_H_
#define _UAPI_SMAF_H_

#include <linux/ioctl.h>
#include <linux/types.h>

/**
 * struct smaf_create_data - allocation parameters
 * @length:	size of the allocation
 * @flags:	flags passed to allocator
 * @fd:		returned file descriptor
 */
struct smaf_create_data {
	size_t length;
	unsigned int flags;
	int fd;
};

/**
 * struct smaf_secure_flag - set/get secure flag
 * @fd:		file descriptor
 * @secure:	secure flag value (set or get)
 */
struct smaf_secure_flag {
	int fd;
	int secure;
};

/**
 * struct smaf_select_by_name - select an allocator
 * @fd:		file descriptor
 * @name:	name of the allocator to be selected
 */
struct smaf_select_by_name {
	int fd;
	char name[64];
};

#define SMAF_IOC_MAGIC	'S'

#define SMAF_IOC_CREATE		 _IOWR(SMAF_IOC_MAGIC, 0, \
						struct smaf_create_data)

#define SMAF_IOC_GET_SECURE_FLAG _IOWR(SMAF_IOC_MAGIC, 1, \
				       struct smaf_secure_flag)

#define SMAF_IOC_SET_SECURE_FLAG _IOWR(SMAF_IOC_MAGIC, 2, \
				       struct smaf_secure_flag)

#define SMAF_IOC_SELECT_BY_NAME	_IOWR(SMAF_IOC_MAGIC, 3, \
				      struct smaf_select_by_name)

#endif