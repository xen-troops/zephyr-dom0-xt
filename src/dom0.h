/*
 * Copyright (c) 2024 EPAM Systems
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_DOM0_XT_SRC_DOM0_H_
#define ZEPHYR_DOM0_XT_SRC_DOM0_H_

/**
 * @brief Dom0 domain's configuration data structure.
 *
 * Hold Dom0 application specific configuration data for Xen guest domains.
 */
struct dom0_domain_cfg {
	struct xen_domain_cfg *domain_cfg; /**< xenlib domain cfg */

	const char *image_kernel_path; /**< disk: domain kernel binary path */
	const char *image_dt_path; /**< disk: domain partial device-tree (PDT) binary path */
};

#endif /* ZEPHYR_DOM0_XT_SRC_DOM0_H_ */
