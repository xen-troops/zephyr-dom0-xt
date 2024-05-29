/*
 * Copyright (c) 2024 EPAM Systems
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <domain.h>
#include <zephyr/xen/public/domctl.h>
#include <string.h>

#include "../dom0.h"

/*
 * Example Xen guest domains configuration file for "rpi_5" which contains
 * - domu_cfg_0 as DomD example which uses real HW "/soc/gpio@7d517c00" and can be used to run
 *   samples/basic/blinky sample.
 * - domu_cfg_1 as DomU, no real HW. It can be used to run any Zephyr example not accesing HW.
 *   For example samples/synchronization
 */

extern char __img_domu1_start[];
extern char __img_domu1_end[];
extern char __dtb_domu1_start[];
extern char __dtb_domu1_end[];

static int load_domu_image_bytes(uint8_t *buf, size_t bufsize, uint64_t offset, void *image_info)
{
	ARG_UNUSED(image_info);

	memcpy(buf, __img_domu1_start + offset, bufsize);
	return 0;
}

static ssize_t get_domu_image_size(void *image_info, uint64_t *size)
{
	ARG_UNUSED(image_info);

	*size = __img_domu1_end - __img_domu1_start;
	return 0;
}

static struct xen_domain_cfg domu_cfg_1 = {
	.name = "rpi_5_domu",
	.mem_kb = 16384,
	.flags = (XEN_DOMCTL_CDF_hvm | XEN_DOMCTL_CDF_hap),
	.max_evtchns = 10,
	.max_vcpus = 1,
	.gnt_frames = 32,
	.max_maptrack_frames = 1,
	.gic_version = XEN_DOMCTL_CONFIG_GIC_V2,
	.tee_type = XEN_DOMCTL_CONFIG_TEE_NONE,
	.cmdline = "",

	.load_image_bytes = load_domu_image_bytes,
	.get_image_size = get_domu_image_size,

	.dtb_start = __dtb_domu1_start,
	.dtb_end = __dtb_domu1_end,
};

static struct xen_domain_iomem domd_iomems[] = {
	{.first_mfn = 0x107D517, .nr_mfns = 0x1, .first_gfn = 0},
};

static char *domd_dt_passthrough[] = {
	"/soc/gpio@7d517c00",
};

extern char __img_domu0_start[];
extern char __img_domu0_end[];
extern char __dtb_domu0_start[];
extern char __dtb_domu0_end[];

static int load_domd_image_bytes(uint8_t *buf, size_t bufsize, uint64_t offset, void *image_info)
{
	ARG_UNUSED(image_info);

	memcpy(buf, __img_domu0_start + offset, bufsize);
	return 0;
}

static ssize_t get_domd_image_size(void *image_info, uint64_t *size)
{
	ARG_UNUSED(image_info);

	*size = __img_domu0_end - __img_domu0_start;
	return 0;
}

static struct xen_domain_cfg domu_cfg_0 = {
	.name = "rpi_5_domd",
	.mem_kb = 16384,
	.flags = (XEN_DOMCTL_CDF_hvm | XEN_DOMCTL_CDF_hap),
	.max_evtchns = 10,
	.max_vcpus = 1,
	.gnt_frames = 32,
	.max_maptrack_frames = 1,
	.gic_version = XEN_DOMCTL_CONFIG_GIC_V2,
	.tee_type = XEN_DOMCTL_CONFIG_TEE_NONE,
	.cmdline = "",

	.iomems = domd_iomems,
	.nr_iomems = ARRAY_SIZE(domd_iomems),

	.dt_passthrough = domd_dt_passthrough,
	.nr_dt_passthrough = ARRAY_SIZE(domd_dt_passthrough),

	.load_image_bytes = load_domd_image_bytes,
	.get_image_size = get_domd_image_size,

	.dtb_start = __dtb_domu0_start,
	.dtb_end = __dtb_domu0_end,
};

struct dom0_domain_cfg domain_cfgs[] = {
	{
		.domain_cfg = &domu_cfg_0,
	},
	{
		.domain_cfg = &domu_cfg_1,
	},
	{ 0 },
};
