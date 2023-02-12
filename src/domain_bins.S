/*
 * Copyright (c) 2023 EPAM Systems
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/toolchain.h>

/* DomU0 binary from include/ dir */
.section .data.domu0_img,"a"

GDATA(__img_domu0_start)
GDATA(__img_domu0_end)

__img_domu0_start:
.incbin		"domu0_img.bin"
__img_domu0_end:

/* DomU0 dtb from include/ dir */
.section .data.dtb_domu0,"a"

GDATA(__dtb_domu0_start)
GDATA(__dtb_domu0_end)

__dtb_domu0_start:
.incbin		"domu0.dtb"
__dtb_domu0_end:
