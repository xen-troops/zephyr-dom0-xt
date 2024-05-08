/*
 * Copyright (c) 2023 EPAM Systems
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/sys/util.h>
#include <zephyr/sys/util_macro.h>

extern struct xen_domain_cfg *domain_cfgs[];

int domain_get_user_cfg_count(void)
{
	int i = 0;

	while (domain_cfgs[i]) {
		i++;
	}

	return i;
}

struct xen_domain_cfg *domain_get_user_cfg(int index)
{
	if (index < domain_get_user_cfg_count()) {
		return domain_cfgs[index];
	}

	return NULL;
}

int main(void)
{
	return 0;
}
