/*
 * Copyright (c) 2024 EPAM Systems
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <zephyr/debug/gcov.h>

static int gcov_dump(const struct shell *shell, size_t argc, char **argv)
{
	gcov_coverage_dump();

	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(
	subcmd_gcov,
	SHELL_CMD_ARG(dump, NULL,
			"Print gcov dump\n",
			gcov_dump, 0, 0),
	SHELL_SUBCMD_SET_END);

SHELL_CMD_ARG_REGISTER(gcov, &subcmd_gcov, "GCOV commands", NULL, 0, 0);
