# SPDX-License-Identifier: Apache-2.0
# Copyright (c) 2024 EPAM Systems

cmake_minimum_required(VERSION 3.13.1)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)

project(xen-dom0)

include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add prebuilt to includes so assembler can find this files
include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt)

target_sources(app PRIVATE src/dom0.c src/domain_bins.S)
target_sources_ifdef(CONFIG_DOM_STORAGE_FATFS_ENABLE app PRIVATE src/storage.c)
target_sources_ifdef(CONFIG_DOM_COVERAGE_ENABLE app PRIVATE src/cmd_gcov.c)

if(CONFIG_USE_DEFAULT_DOM_CFG)
  target_sources(app PRIVATE src/domu_cfg.c)
else()
  if(CONFIG_DOM_CFG_BOARD_EXT)
    target_sources(app PRIVATE src/dom_cfg/${BOARD}_${CONFIG_DOM_CFG_BOARD_EXT}.c)
  else()
    target_sources(app PRIVATE src/dom_cfg/${BOARD}.c)
  endif()
endif()
