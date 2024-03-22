/*
 * Copyright (c) 2024 EPAM Systems
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __DOMU_MGMT_CFG_H__
#define __DOMU_MGMT_CFG_H__

#include <stdint.h>

extern char __img_d_start[];
extern char __img_d_end[];

extern char __dtb_d_start[];
extern char __dtb_d_end[];

extern char __img_u_start[];
extern char __img_u_end[];

extern char __dtb_u_start[];
extern char __dtb_u_end[];

int find_cfg_and_create_domu(uint32_t domid);
void create_and_unpause_domus(void);

#endif /* __DOMU_MGMT_CFG_H__ */
