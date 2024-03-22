/*
 * Copyright (c) 2024 EPAM Systems
 * SPDX-License-Identifier: Apache-2.0
 */
#include <string.h>
#include <domain.h>
#include <zephyr/devicetree.h>
#include <zephyr/xen/public/domctl.h>
#include <zephyr/logging/log.h>
#include <xen_dom_mgmt.h>
#include <domu_mgmt_cfg.h>

LOG_MODULE_REGISTER(domu_cfgs, CONFIG_LOG_DEFAULT_LEVEL);

static int load_domu_image(uint8_t *buf, size_t bufsize, uint64_t image_load_offset,
			   void *image_info)
{
	ARG_UNUSED(image_info);
	memcpy(buf, __img_u_start + image_load_offset, bufsize);
	return 0;
}

static ssize_t get_domu_image_size(void *image_info, uint64_t *size)
{
	ARG_UNUSED(image_info);
	*size = __img_u_end - __img_u_start;
	return 0;
}

static int load_domd_image(uint8_t *buf, size_t bufsize, uint64_t image_load_offset,
			   void *image_info)
{
	ARG_UNUSED(image_info);
	memcpy(buf, __img_d_start + image_load_offset, bufsize);
	return 0;
}

static ssize_t get_domd_image_size(void *image_info, uint64_t *size)
{
	ARG_UNUSED(image_info);
	*size = __img_d_end - __img_d_start;
	return 0;
}

#define FILL_DOMU_VBD_CFG(node_id)                                                                 \
	{                                                                                          \
		.configured = true,                                                                \
		.backend_domain_id = DT_PROP(node_id, backend_domain_id),                          \
		.target = DT_PROP(node_id, target),                                                \
		.script = DT_PROP(node_id, script),                                                \
		.backendtype = DT_PROP(node_id, backendtype),                                      \
		.vdev = DT_PROP(node_id, vdev),                                                    \
		.access = DT_PROP(node_id, access),                                                \
	},

#define FILL_VBD_CFG_IF_COMPAT(node_id)                                                            \
	COND_CODE_1(DT_NODE_HAS_COMPAT(node_id, xen_domu_vbd), (FILL_DOMU_VBD_CFG(node_id)), ())

#define FILL_DOMU_VIF_CFG(node_id)                                                                 \
	{                                                                                          \
		.configured = true,                                                                \
		.backend_domain_id = DT_PROP(node_id, backend_domain_id),                          \
		.mac = DT_PROP(node_id, mac),                                                      \
		.script = DT_PROP(node_id, script),                                                \
		.bridge = DT_PROP(node_id, bridge),                                                \
		.type = DT_PROP_OR(node_id, vif_type, ""),                                         \
		.ip = DT_PROP_OR(node_id, ip, ""),                                                 \
	},

#define FILL_VIF_CFG_IF_COMPAT(node_id)                                                            \
	COND_CODE_1(DT_NODE_HAS_COMPAT(node_id, xen_domu_vif), (FILL_DOMU_VIF_CFG(node_id)), ())

#define FILL_DOMU_BACK_CFGS(node_id)                                                               \
	.disks = {DT_FOREACH_CHILD_STATUS_OKAY(node_id, FILL_VBD_CFG_IF_COMPAT)},                  \
	.vifs = {DT_FOREACH_CHILD_STATUS_OKAY(node_id, FILL_VIF_CFG_IF_COMPAT)},

#define IS_DOMD(node_id) (DT_PROP_BY_IDX(node_id, reg, 0) == 1)

#define LOAD_IMAGE_BYTES(node_id) (IS_DOMD(node_id) ? (load_domd_image) : (load_domu_image))

#define GET_IMAGE_SIZE(node_id) (IS_DOMD(node_id) ? (get_domd_image_size) : (get_domu_image_size))

#define DTB_START(node_id) (IS_DOMD(node_id) ? (__dtb_d_start) : (__dtb_u_start))

#define DTB_END(node_id) (IS_DOMD(node_id) ? (__dtb_d_end) : (__dtb_u_end))

#define IOMEM_INIT(node_id, prop, idx)                                                             \
	{                                                                                          \
		.first_mfn = DT_PHA_BY_IDX(node_id, prop, idx, first_mfn),                         \
		.nr_mfns = DT_PHA_BY_IDX(node_id, prop, idx, nr_mfns),                             \
		.first_gfn = DT_PHA_BY_IDX(node_id, prop, idx, first_gfn),                         \
	}

#define FILL_IOMEMS(node_id) DT_FOREACH_PROP_ELEM_SEP(node_id, iomems, IOMEM_INIT, (,))

#define GET_IOMEMS(node_id)                                                                        \
	COND_CODE_1(DT_NODE_HAS_PROP(node_id, iomems), (FILL_IOMEMS(node_id)), ())

#define FILL_DOMU_CFG_ITEM(node_id)                                                                \
	{                                                                                          \
		.name = DT_NODE_FULL_NAME(node_id),                                                \
		.machine_dt_compat = DT_PROP(node_id, machine_dt_compat),                          \
		.mem_kb = DT_PROP(node_id, mem_size) / 1024,                                       \
		.max_evtchns = DT_PROP(node_id, max_evtchns),                                      \
		.max_vcpus = DT_PROP(node_id, max_vcpus),                                          \
		.gic_version = DT_PROP(node_id, gic_version),                                      \
		.max_maptrack_frames = DT_PROP(node_id, max_maptrack_frames),                      \
		.gnt_frames = DT_PROP(node_id, gnt_frames),                                        \
		.cmdline = DT_PROP(node_id, cmdline),                                              \
                                                                                                   \
		.flags = (XEN_DOMCTL_CDF_hvm | XEN_DOMCTL_CDF_hap | XEN_DOMCTL_CDF_iommu),         \
                                                                                                   \
		.tee_type = DT_PROP(node_id, tee),                                                 \
                                                                                                   \
		.load_image_bytes = LOAD_IMAGE_BYTES(node_id),                                     \
		.get_image_size = GET_IMAGE_SIZE(node_id),                                         \
		.dtb_start = DTB_START(node_id),                                                   \
		.dtb_end = DTB_END(node_id),                                                       \
                                                                                                   \
		.irqs = (uint32_t[])DT_PROP_OR(node_id, domu_irqs, {}),                            \
		.nr_irqs = DT_PROP_LEN_OR(node_id, domu_irqs, 0),                                  \
                                                                                                   \
		.iomems = (struct xen_domain_iomem[]){GET_IOMEMS(node_id)},                        \
		.nr_iomems = DT_PROP_LEN_OR(node_id, iomems, 0),                                   \
                                                                                                   \
		.dtdevs = (char *[])DT_PROP_OR(node_id, dtdevs, {}),                               \
		.nr_dtdevs = DT_PROP_LEN_OR(node_id, dtdevs, 0),                                   \
                                                                                                   \
		.dt_passthrough = (char *[])DT_PROP_OR(node_id, dt_passthrough, {}),               \
		.nr_dt_passthrough = DT_PROP_LEN_OR(node_id, dt_passthrough, 0),                   \
                                                                                                   \
		.back_cfg = {FILL_DOMU_BACK_CFGS(node_id)},                                        \
	},

static struct xen_domain_cfg domu_cfg[] = {DT_FOREACH_STATUS_OKAY(xen_domu, FILL_DOMU_CFG_ITEM)};

static int domu_create(struct xen_domain_cfg *dom_cfg, uint32_t domid, bool unpause)
{
	int ret;

	ret = domain_create(dom_cfg, domid);
	if (ret) {
		LOG_ERR("Can't create domain %s", dom_cfg->name);
		return ret;
	}

	ret = domain_post_create(dom_cfg, domid);
	if (ret) {
		LOG_ERR("Can't post create domain %s", dom_cfg->name);
		domain_destroy(domid);
		return ret;
	}

	if (unpause) {
		ret = domain_unpause(domid);
		if (ret) {
			domain_destroy(domid);
			return ret;
		}
	}

	return 0;
}

void create_and_unpause_domus(void)
{
	/* make sure that domD will be started first */
	for (unsigned int i = 0; i < ARRAY_SIZE(domu_cfg); i++) {
		char *id = strstr(domu_cfg[i].name, "@");
		uint32_t domid = strtol(id + 1, NULL, 16);

		if (!domid) {
			LOG_ERR("Can't get domain ID for name %s or it's incorrect",
				domu_cfg[i].name);
			continue;
		}

		if (domid == 1) {
			if (domu_create(&domu_cfg[i], domid, false)) {
				LOG_ERR("DomD isn't created/started, skipping creation/starting of "
					"other domains.");
				return;
			}
		}
	}

	/* creating and start other domains */
	for (unsigned int i = 0; i < ARRAY_SIZE(domu_cfg); i++) {
		char *id = strstr(domu_cfg[i].name, "@");
		uint32_t domid = strtol(id + 1, NULL, 16);

		if (domid != 1) {
			domu_create(&domu_cfg[i], domid, true);
		}
	}
}

int find_cfg_and_create_domu(uint32_t domid)
{
	for (unsigned int i = 0; i < ARRAY_SIZE(domu_cfg); i++) {
		int ret;
		char *id = strstr(domu_cfg[i].name, "@");

		if (!id) {
			LOG_WRN("Can't find domain id begging in the config name: %s",
				domu_cfg[i].name);
			continue;
		}

		if (domid != strtol(id + 1, NULL, 16)) {
			continue;
		}

		ret = domain_create(&domu_cfg[i], domid);
		if (!ret) {
			return domain_post_create(&domu_cfg[i], domid);
		}

		return 0;
	}

	LOG_ERR("There isn't any domain in dts with requested ID: %u", domid);
	return -EINVAL;
}
