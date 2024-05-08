/*
 * Copyright (c) 2024 EPAM Systems
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <domain.h>
#include <zephyr/xen/public/domctl.h>
#include <string.h>

/*
 * Example Xen guest domains configuration file for "rpi_5" which contains
 * - domd_cfg_0 as DomD example which uses real HW "/soc/gpio@7d517c00" and can be used to run
 *   samples/basic/blinky sample.
 * - domd_cfg_1 as DomU, no real HW. It can be used to run any Zephyr example not accesing HW.
 *   For example samples/synchronization
 */

static char *domd_dtdevs_1[] = {
};

static struct xen_domain_iomem domd_iomems_1[] = {
};

static uint32_t domd_irqs_1[] = {
};

extern char __img_domd_start_1[];
extern char __img_domd_end_1[];
extern char __dtb_domd_start_1[];
extern char __dtb_domd_end_1[];

static int load_domd_image_bytes_1(uint8_t *buf, size_t bufsize,
		   uint64_t offset, void *image_info)
{
	ARG_UNUSED(image_info);

	memcpy(buf, __img_domd_start_1 + offset, bufsize);
	return 0;
}

static ssize_t get_domd_image_size_1(void *image_info, uint64_t *size)
{
	ARG_UNUSED(image_info);

	*size = __img_domd_end_1 - __img_domd_start_1;
	return 0;
}

struct xen_domain_cfg domd_cfg_1 = {
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

	.iomems = domd_iomems_1,
	.nr_iomems = 0,

	.irqs = domd_irqs_1,
	.nr_irqs = 0,

	.dtdevs = domd_dtdevs_1,
	.nr_dtdevs = 0,

	.load_image_bytes = load_domd_image_bytes_1,
	.get_image_size = get_domd_image_size_1,

	.dtb_start = __dtb_domd_start_1,
	.dtb_end = __dtb_domd_end_1,
};

static char *domd_dtdevs_0[] = {
};

static struct xen_domain_iomem domd_iomems_0[] = {
	{ .first_mfn = 0x107D517, .nr_mfns = 0x1, .first_gfn = 0},
};

static uint32_t domd_irqs_0[] = {
};

static char *domd_dt_passthrough_0[] = {
	"/soc/gpio@7d517c00",
};

extern char __img_domd_start_0[];
extern char __img_domd_end_0[];
extern char __dtb_domd_start_0[];
extern char __dtb_domd_end_0[];

static int load_domd_image_bytes_0(uint8_t *buf, size_t bufsize,
		   uint64_t offset, void *image_info)
{
	ARG_UNUSED(image_info);

	memcpy(buf, __img_domd_start_0 + offset, bufsize);
	return 0;
}

static ssize_t get_domd_image_size_0(void *image_info, uint64_t *size)
{
	ARG_UNUSED(image_info);

	*size = __img_domd_end_0 - __img_domd_start_0;
	return 0;
}

struct xen_domain_cfg domd_cfg_0 = {
	.name = "rpi_5_domd",
	.mem_kb = 16384,
	.flags = (XEN_DOMCTL_CDF_hvm | XEN_DOMCTL_CDF_hap),
	.max_evtchns = 10,
	.max_vcpus = 1,
	.gnt_frames = 32,
	.max_maptrack_frames = 1,
	.gic_version = XEN_DOMCTL_CONFIG_GIC_V2,
	.tee_type = XEN_DOMCTL_CONFIG_TEE_NONE,
	.cmdline="",

	.iomems = domd_iomems_0,
	.nr_iomems = ARRAY_SIZE(domd_iomems_0),

	.irqs = domd_irqs_0,
	.nr_irqs = 0,

	.dtdevs = domd_dtdevs_0,
	.nr_dtdevs = ARRAY_SIZE(domd_dtdevs_0),

	.dt_passthrough = domd_dt_passthrough_0,
	.nr_dt_passthrough = ARRAY_SIZE(domd_dt_passthrough_0),

	.load_image_bytes = load_domd_image_bytes_0,
	.get_image_size = get_domd_image_size_0,

	.dtb_start = __dtb_domd_start_0,
	.dtb_end = __dtb_domd_end_0,
};
