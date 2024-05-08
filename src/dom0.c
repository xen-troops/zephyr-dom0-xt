/*
 * Copyright (c) 2023 EPAM Systems
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/sys/util.h>
#include <zephyr/sys/util_macro.h>

#define CFG_DEFINE_ONE(n, inst) extern struct xen_domain_cfg domd_cfg_##n
#define CONFIGS_DEFINE(n)       LISTIFY(n, CFG_DEFINE_ONE, (;))

CONFIGS_DEFINE(CONFIG_DOM_CFG_NUMBER);

#define FILL_ONE_CFG(n, inst) &domd_cfg_##n

#define FILL_CONFIG(n)                                                                             \
	{                                                                                          \
		LISTIFY(n, FILL_ONE_CFG, (, ))                                                     \
	}

struct xen_domain_cfg *cfgs[] = FILL_CONFIG(CONFIG_DOM_CFG_NUMBER);

int domain_get_user_cfg_count(void)
{
	return ARRAY_SIZE(cfgs);
}

struct xen_domain_cfg *domain_get_user_cfg(int index)
{
	if (index < domain_get_user_cfg_count()) {
		return cfgs[index];
	}

	return NULL;
}

int main(void)
{
	return 0;
}
