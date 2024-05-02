/*
 * Copyright (c) 2024 EPAM Systems
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <domain.h>
#include <zephyr/xen/public/domctl.h>

#include <string.h>

static char *domd_dtdevs[] = {
};

static struct xen_domain_iomem domd_iomems[] = {
};

static uint32_t domd_irqs[] = {
};

extern char __img_domd_start_0[];
extern char __img_domd_end_0[];
extern char __dtb_domd_start_0[];
extern char __dtb_domd_end_0[];

static int load_domd_image_bytes(uint8_t *buf, size_t bufsize,
		   uint64_t offset, void *image_info)
{
	ARG_UNUSED(image_info);

	memcpy(buf, __img_domd_start_0 + offset, bufsize);
	return 0;
}

static ssize_t get_domd_image_size(void *image_info, uint64_t *size)
{
	ARG_UNUSED(image_info);

	*size = __img_domd_end_0 - __img_domd_start_0;
	return 0;
}

struct xen_domain_cfg domd_cfg_0 = {
	.name = "rpi_5_domu",
	.mem_kb = 16384,
	.flags = (XEN_DOMCTL_CDF_hvm | XEN_DOMCTL_CDF_hap),
	.max_evtchns = 10,
	.max_vcpus = 1,
	.gnt_frames = 32,
	.max_maptrack_frames = 1,
	.gic_version = XEN_DOMCTL_CONFIG_GIC_V2,
	.tee_type = XEN_DOMCTL_CONFIG_TEE_NONE,
	.cmdline="",

	.iomems = domd_iomems,
	.nr_iomems = 0,

	.irqs = domd_irqs,
	.nr_irqs = 0,

	.dtdevs = domd_dtdevs,
	.nr_dtdevs = 0,

	.load_image_bytes = load_domd_image_bytes,
	.get_image_size = get_domd_image_size,

	.dtb_start = __dtb_domd_start_0,
	.dtb_end = __dtb_domd_end_0,
};
