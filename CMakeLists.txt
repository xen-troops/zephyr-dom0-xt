# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.13.1)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)

project(xen-dom0)

include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add prebuilt to includes so assembler can find this files
include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt)

target_sources(app PRIVATE src/dom0.c src/domu_cfg.c src/domain_bins.S)
