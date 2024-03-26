/*
 * Copyright (c) 2023 EPAM Systems
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <errno.h>

__weak int create_domains(void)
{
    return -EINVAL;
}

__weak void create_and_unpause_domus(void)
{
    return;
}

void main(void)
{
    int ret = create_domains();

    if (ret) {
        return;
    }

    create_and_unpause_domus();
}
