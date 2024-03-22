# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)

project(xen-dom0)

include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add prebuilt to includes so assembler can find this files
include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt)

target_sources(app PRIVATE src/dom0.c domains/domd/domain_bins.S)

# Include source files for DomD support
target_sources_ifdef(CONFIG_BOARD_RCAR_H3ULCB_CA57     app PRIVATE domains/domd/domd_cfg_h3ulcb.c)
target_sources_ifdef(CONFIG_BOARD_RCAR_SALVATOR_XS_M3  app PRIVATE domains/domd/domd_cfg_salvator_xs.c)
target_sources_ifdef(CONFIG_BOARD_RCAR_SPIDER          app PRIVATE domains/domd/domd_cfg_spider.c)

# Include source files for DomU support
if(CONFIG_DOMU)
    target_sources_ifdef(CONFIG_SHIELD_XEN_DOM0 app PRIVATE domains/domu/domu_bins.S)
endif()
target_sources(app PRIVATE domains/domu/domu_cfgs.c)
