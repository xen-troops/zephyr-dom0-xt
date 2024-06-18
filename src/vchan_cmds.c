/*
 * Copyright (c) 2024 EPAM Systems
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <domain.h>
#include <vch.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/shell/shell.h>

LOG_MODULE_REGISTER(vchan_cmd);

#define ECHO_SERVER_THREAD_PRIO    7
#define PATH_TEMPLATE "/local/domain/%d/echo"
#define THREAD_NAME "vchan_echo_server"

static struct k_thread echo_thread;
static struct k_thread *pecho_thread = NULL;
static struct vch_handle handle;

static K_THREAD_STACK_DEFINE(echo_thread_stack, CONFIG_VCHAN_THREAD_STACK_SIZE);
static volatile int vchan_server_stop = 0;

static void vchan_echo_server(void *p1, void *p2, void *p3)
{
	struct vch_handle *h = (struct vch_handle *)p1;
	int size;
	static char buf[CONFIG_VCHAN_ECHO_BUFFER_SIZE];

	ARG_UNUSED(p2);
	ARG_UNUSED(p3);
	LOG_DBG("Server started\n");
	while (!vchan_server_stop) {
		size = vch_read(h, buf, sizeof(buf));
		if (size > 0) {
			vch_write(h, buf, size);
		} else {
			if (size < 0 && size != -ENOTCONN) {
				LOG_ERR("vch_read returned %d\r", size);
			}
			k_sleep(K_MSEC(500));
		}
	}
	LOG_DBG("Server stopped\n");
	vch_close(h);
}

static int vchan_shell_start(const struct shell *sh, size_t argc, char **argv)
{
	int ret;
	domid_t domid = 0;
	static char path[PATH_MAX];

	if (pecho_thread) {
		shell_warn(sh, "Server already started");
		return 1;
	}
	if (argv[1]) {
		domid = atoi(argv[1]);
	}
	snprintf(path, PATH_MAX-1, PATH_TEMPLATE, domid);
	ret = vch_open(domid, path, 10, 10, &handle);
	if (ret) {
		shell_error(sh, "vchan open error %d", ret);
		goto exit;
	}
	shell_print(sh, "Echo server openad at %s", path);
	vchan_server_stop = 0;
	k_thread_create(&echo_thread, echo_thread_stack, K_THREAD_STACK_SIZEOF(echo_thread_stack),
			vchan_echo_server, (void *)&handle, NULL, NULL,
			ECHO_SERVER_THREAD_PRIO, 0, K_NO_WAIT);
	pecho_thread = &echo_thread;
	k_thread_name_set(pecho_thread, THREAD_NAME);

exit:
	return ret;
}

static int vchan_shell_stop(const struct shell *sh, size_t argc, char **argv)
{
	if (!pecho_thread) {
		shell_warn(sh, "Server not started");
		return 0;
	}
	vchan_server_stop = 1;
	if (k_thread_join(pecho_thread, K_MSEC(1000))) {
		shell_error(sh, "Killing thread");
		k_thread_abort(pecho_thread);
	}
	pecho_thread = NULL;
	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(
	subcmd_vchan,
	SHELL_CMD_ARG(start, NULL,
		      "start echo server.\n"
		      "\tUsage:\n"
		      "\tvchan start [domid]\n",
		      vchan_shell_start, 0, 1),
	SHELL_CMD_ARG(stop, NULL,
		      "stop server.\n",
		      vchan_shell_stop, 0, 0),
	SHELL_SUBCMD_SET_END);

SHELL_CMD_ARG_REGISTER(vchan, &subcmd_vchan, "VCHAN commands", NULL, 2, 0);
