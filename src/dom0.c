/*
 * Copyright (c) 2023 EPAM Systems
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <errno.h>
#include <xen_dom_mgmt.h>

extern struct xen_domain_cfg domd_cfg;

__weak void create_and_unpause_domus(void)
{
    return;
}

void main(void)
{
    if (domain_create(&domd_cfg, 1)) {
        return;
    }

    create_and_unpause_domus();
}
