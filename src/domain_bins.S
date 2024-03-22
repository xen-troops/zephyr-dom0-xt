/*
 * Copyright (c) 2023-2024 EPAM Systems
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr/toolchain.h>

.section .data.img_d,"a"
GDATA(__img_d_start)
GDATA(__img_d_end)
__img_d_start:
.incbin CONFIG_DOMD_BIN_PATH
__img_d_end:

.section .data.dtb_d,"a"
GDATA(__dtb_d_start)
GDATA(__dtb_d_end)
__dtb_d_start:
.align 8
.incbin CONFIG_DOMD_DTB_PATH
__dtb_d_end:

.section .data.img_u,"a"
GDATA(__img_u_start)
GDATA(__img_u_end)
__img_u_start:
.incbin CONFIG_DOMU_BIN_PATH
__img_u_end:

.section .data.dtb_u,"a"
GDATA(__dtb_u_start)
GDATA(__dtb_u_end)
__dtb_u_start:
.align 8
.incbin CONFIG_DOMU_DTB_PATH
__dtb_u_end:
