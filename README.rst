Overview
********

.. _XEN: https://xenproject.org/
.. _Xen-Troops: https://github.com/xen-troops
.. _zephyr-xenlib: https://github.com/xen-troops/zephyr-xenlib

A simple `XEN`_ Zephyr Dom0 application **zephyr-dom0-xt** showcasing the use of `Xen-Troops`_
library `zephyr-xenlib`_ to create and manage `XEN`_ domains from
Zephyr application by using `zephyr-xenlib`_ API and SHELL interface command.

The **zephyr-dom0-xt** application intended to be started by `XEN`_ as Dom0 domain.

The **zephyr-dom0-xt** doesn't create any `XEN`_ guest domains on start by default.

Getting Started
***************

Follow Zephyr `Getting Started guide <https://docs.zephyrproject.org/latest/develop/getting_started/index.html#getting-started-guide>`_ to setup Zephyr prerequisites.

In section `Get Zephyr and install Python dependencies <https://docs.zephyrproject.org/latest/develop/getting_started/index.html#get-zephyr-and-install-python-dependencies>`_
replace step **Get the Zephyr source code:** with below commands:

.. code-block:: bash

    mkdir ~/zephyr-dom0-xt
    cd ~/zephyr-dom0-xt
    west init -m https://github.com/xen-troops/zephyr-dom0-xt.git --mr rpi5_dom0_dev
    west update

Requirements
************

Supported platforms:

The **zephyr-dom0-xt** can be built and run on any platform for which Xen and Xen Dom0 support
is enabled:

.. code-block:: text

    CONFIG_XEN=y
    CONFIG_XEN_DOM0=y

    See snippets/xen_dom0/README.rst

The primary platform is **rpi_5** `Raspberry Pi 5 <https://www.raspberrypi.com/products/raspberry-pi-5/>`_

Prepared Xen guest domains configuration files in ``src/dom_cfg/`` folder.

Prepared Xen guest domains binary image files of kernel and device-tree in ``prebuilt/`` folder.

The Xen guest domains which use real HW shell be created and started only once even though
`zephyr-xenlib`_ does not prevent it. From another side, the Xen guest domain which doesn't use
any real HW can be started as many times as `zephyr-xenlib`_ allows, using the same Xen guest
domains configuration.

Xen guest domains configuration
*******************************

For Xen guest domains configuration the **zephyr-dom0-xt** application uses domain's configuration
format defined by ``struct dom0_domain_cfg`` which includes pointer on `zephyr-xenlib`_
domain configuration ``struct xen_domain_cfg``.

.. note::

    The number of supported guest domain's kernel and DTB binary images is limited to 2.

Each Xen guest domain's configurations shell be defined as  ``struct dom0_domain_cfg`` and listed
in public array named "domain_cfgs". The last member of ``domain_cfgs`` array should
be filled with all 0x0.

    For example:

.. code:: C

    struct xen_domain_cfg domu_cfg_0 = {
    }

    struct xen_domain_cfg domu_cfg_1 = {
    }

struct dom0_domain_cfg domain_cfgs[] = {
	{
		.domain_cfg = &domu_cfg_0,
	},
	{
		.domain_cfg = &domu_cfg_1,
	},
	{ 0 },
};

It is possible to use the same guest domain kernel or DTB image for a number of
domain's configurations.

The **zephyr-dom0-xt** selects Xen guest domains configurations file
(containing ``domain_cfgs`` array) basing on:

- The **CONFIG_USE_DEFAULT_DOM_CFG** (default=y) Kconfig option. if selected the ``src/domu_cfg.c``
  file will be used.
- if CONFIG_USE_DEFAULT_DOM_CFG=n the Xen guest domains configurations file will be selected basing
  on Zephyr board and **CONFIG_DOM_CFG_BOARD_EXT** Kconfig option as following

.. code-block:: text

      if DEFINED(CONFIG_DOM_CFG_BOARD_EXT)
        use  src/dom_cfg/${BOARD}_${CONFIG_DOM_CFG_BOARD_EXT}.c
      else()
        use src/dom_cfg/${BOARD}.c

For example Zephyr board is **rpi_5** and `CONFIG_USE_DEFAULT_DOM_CFG=n``:

.. code-block:: text

    CONFIG_DOM_CFG_BOARD_EXT=n ---> src/dom_cfg/rpi_5.c
    CONFIG_DOM_CFG_BOARD_EXT=\"domd\" ---> src/dom_cfg/rpi_5_domd.c

Xen guest domains binary images
*******************************

Built-in domains binary images
==============================

The **zephyr-dom0-xt** can take pre-built Xen guest domains images from ``prebuilt/`` folder and
they are integrated directly into Zephyr **zephyr-dom0-xt**. Now only two sets of Xen guest images
is supported which names are defined by following Kconfig options:

- **CONFIG_DOM_CFG_DOMU0_IMAGE_BIN_FILE**: Location for Domain-U0 kernel binary.
  For RPI 5 it is ``zephyr_blinky.bin`` corresponding to Zephyr ``samples/basic/blinky`` sample.
- **CONFIG_DOM_CFG_DOMU0_DTB_BIN_FILE**: Location for Domain-U0 device-tree binary.
- **CONFIG_DOM_CFG_DOMU1_IMAGE_BIN_FILE**: Location for Domain-U1 kernel binary.
  For RPI 5 it is ``zephyr_sync.bin`` corresponding to Zephyr ``samples/synchronization`` sample.
- **CONFIG_DOM_CFG_DOMU1_DTB_BIN_FILE**: Location for Domain-U1 device-tree binary.

To use custom binary images they should be copied into ``prebuilt/`` folder.

For Domain-U1/U0 device-tree and kernel binaries special linker sections will be defined which then
can be used in `zephyr-xenlib`_ domain configuration ``struct xen_domain_cfg`` and to implement
.load_image_bytes()/.get_image_size() callbacks. See ``dom_cfg/rpi_5.c`` for an example.

This functionality is configured by **CONFIG_DOM_CFG_BUILTIN_IMAGES** Kconfig option.


Domains binary images on storage
================================

On **rpi_5** platform the Zephyr **zephyr-dom0-xt** supports loading guest domains
device-tree/kernel image binaries from FATFS partition on SD-card. The **zephyr-dom0-xt** as MBR
as GPT SD-card format.

This option can be enabled using the **CONFIG_DOM_STORAGE_FATFS_ENABLE** Kconfig option.

By default, the **first** physical FATFS partition is used to look up for guest domains
device-tree/kernel image binaries in **dom0** folder.

The default images folder name can be changed using **CONFIG_DOM_STORAGE_FATFS_DIR** Kconfig option.

The default fs mount point is ``/SD:``.

Multi FAT volume support
------------------------

The Zephyr **zephyr-dom0-xt** supports Multi FAT volumes. This feature can be enabled using
**CONFIG_FS_MULTI_PARTITION** Kconfig option. When this option is enabled the fs mount point should
be defined in format::

    "/0:" - first physical FATFS partition
    "/1:" - second physical FATFS partition
    ...

.. note::

    if more then 2 physical FATFS partitions is used the ``PARTITION VolToPart[FF_VOLUMES]``
    array need to be updated in s``storage.c``.

The physical FATFS partition to be used is selected by
**CONFIG_DOM_STORAGE_FATFS_LOGICAL_DRIVE_NAME** Kconfig option which is "/0:" by default - the first
physical FATFS partition.

For example, SD-card with two FATFS partitions. The second FATFS partitions is used to store
guest domain binaries. The Kconfig configuration to be used:

.. code-block:: text

    CONFIG_DOM_STORAGE_FATFS_ENABLE=y
    CONFIG_FS_MULTI_PARTITION=y
    CONFIG_DOM_STORAGE_FATFS_LOGICAL_DRIVE_NAME="1"
    CONFIG_DOM_STORAGE_FATFS_DIR=""

Building and Running
********************

The **zephyr-dom0-xt** can be built and run on any platform for which Xen and Xen Dom0 support
is enabled:

For example, to build it for **rpi_5** board with one Xen guest domain configuration
(DomD, ``src/dom_cfg/rpi_5_domd.c``) run below command:

.. code-block:: bash

    west build -b rpi_5 -p always zephyr-dom0-xt -- \
    -DCONFIG_USE_DEFAULT_DOM_CFG=n -DCONFIG_DOM_CFG_BOARD_EXT=\"domd\"

For example, to build it for **rpi_5** board with two Xen guest domain configurations
(DomD/DomU, ``src/dom_cfg/rpi_5.c``) run below command:

.. code-block:: bash

    west build -b rpi_5 -p always zephyr-dom0-xt -- \
    -DCONFIG_USE_DEFAULT_DOM_CFG=n

Running
=======

The **zephyr-dom0-xt** application should be started by `XEN`_ as Dom0 domain.
This process is out of scope of this document.

For reference use:
`Zephyr boot under XEN from u-boot as Dom0 <https://github.com/xen-troops/meta-xt-rpi5/wiki/RPI-5-Zephyr#zephyr-boot-under-xen-from-u-boot-as-dom0>`_

RPI5 Dom0 run example with two domains configuration
****************************************************

The **zephyr-dom0-xt** should be build with already prepared Xen guest domain configuration file
``src/dom_cfg/rpi_5.c`` for RPI5 which contains two Xen guest domain configurations:

- **domu_cfg_0**: It mockups DomD behavior and uses real RPI5 GPIO HW ``/soc/gpio@7d517c00``.
  It's intended to be used with Zephyr Blinky sample.
- **domu_cfg_1**: It mockups DomU which pure Xen guest domain without using real RPI5 HW.
  It's intended to be used with Zephyr samples/synchronization sample.

Prepare Xen guest domains binaries
==================================

Build Zephyr Blinky sample for Zephyr **xenvm** board with RPI5 snippet ``rpi_5_xen_domd``:

.. code-block:: bash

    west build -b xenvm -p always -S rpi_5_xen_domd samples/basic/blinky

Once build is finished copy ``zephyr.bin`` into the
``zephyr/zephyr-dom0-xt/prebuilt/zephyr_blinky.bin``.

Build Zephyr samples/synchronization sample for Zephyr **xenvm** board:

.. code-block:: bash

    west build -b xenvm -p always samples/synchronization

Once build is finished copy ``zephyr.bin`` into the
``zephyr/zephyr-dom0-xt/prebuilt/zephyr_sync.bin``.

.. note::

    For RPI5 It's default configuration, so this step can be skipped.

RPI5 Build zephyr-dom0-xt
====================

Run below command to build **zephyr-dom0-xt**:

.. code-block:: bash

    west build -b rpi_5 -p always zephyr-dom0-xt

Once build is finished copy ``zephyr.bin`` into RPI5 ``bootfs``, so it can be picked up by
booting process.

Once boot is finished the command line interface should appear:

.. code-block:: console

    (XEN) *** Serial input to DOM0 (type 'CTRL-a' three times to switch input)
    (XEN) Freed 344kB init memory.
    (XEN) d0v0: vGICD: unhandled word write 0x000000ffffffff to ICACTIVER4
    ...
    (XEN) d0v0: vGICD: unhandled word write 0x000000ffffffff to ICACTIVER0
    (XEN) common/grant_table.c:1909:d0v0 Expanding d0 grant table from 1 to 2 frames
    ...
    (XEN) common/grant_table.c:1909:d0v0 Expanding d0 grant table from 63 to 64 frames
    *** Booting Zephyr OS build v3.6.0-72-g702a8af0cbe9 ***
    I: dom0less: attached 0 domains

    uart:~$

Run ``xu config_list`` to see list of available configurations:

.. code-block:: console

    uart:~$ xu config_list
    rpi_5_domd
    rpi_5_domu

DomD control
============

Create DomD by using ``xu create`` with DomD configuration and Xen domainId ``-d 1``.
Note the `zephyr-xenlib`_ starts automatically domain with Xen domainId ``1``.

.. code-block:: console

    uart:~$ xu create rpi_5_domd -d 1
    W: Domain device tree generation is not supported
    I: rambase = 40000000, ramsize = 16777216
    I: kernbase = 40000000 kernsize = 360452, dtbsize = 11058
    I: kernsize_aligned = 2097152
    I: DTB will be placed on addr = 0x40e00000
    (XEN) memory_map:add: dom1 gfn=107d517 mfn=107d517 nr=1
    uart:~$ (XEN) d1v0: vGICD: unhandled word write 0x000000ffffffff to ICACTIVER0
    (XEN) common/grant_table.c:1909:d1v0 Expanding d1 grant table from 1 to 2 frames
    ...
    (XEN) common/grant_table.c:1909:d1v0 Expanding d1 grant table from 15 to 16 frames

At this moment RPI5 led should start blinking.

Pause DomD - RPI5 led should stop blinking:

.. code-block:: console

    uart:~$ xu pause 1

Unpause DomD - RPI5 led should start blinking again:

.. code-block:: console

    uart:~$ xu unpause 1

Attach to DomD console with ``xu console``, use ``Ctrl-']'`` to exit console:

.. code-block:: console

    uart:~$ xu console 1
    Attached to a domain console
    LED state: OFF
    [00:00:00.000,000] <inf> xen_events: xen_events_init: events inited

    [00:00:00.000,000] <inf> uart_hvc_xen: Xen HVC inited successfully

    *** Booting Zephyr OS build v3.6.0-72-g702a8af0cbe9 ***
    LED state: ON
    LED state: OFF

DomU control
============

Create DomU by using ``xu create`` with DomU configuration and Xen domainId ``-d 2``.

.. code-block:: console

    uart:~$ xu create rpi_5_domu -d 2 -p
    W: Domain device tree generation is not supported
    I: rambase = 40000000, ramsize = 16777216
    I: kernbase = 40000000 kernsize = 364548, dtbsize = 11058
    I: kernsize_aligned = 2097152
    I: DTB will be placed on addr = 0x40e00000
    I: Created domain is paused
    To unpause issue: xu unpause 2
                      ^^^^^^^^^^^^^^ Xen domainId assigned to guest domain

Create DomU by using ``xu create`` with DomU configuration and Xen domainId ``-d 0``.
Note the `zephyr-xenlib`_ will assign Xen domainId automatically with ``-d 0``.

.. code-block:: console

    uart:~$ xu create rpi_5_domu -d 0 -p
    W: Domain device tree generation is not supported
    I: rambase = 40000000, ramsize = 16777216
    I: kernbase = 40000000 kernsize = 364548, dtbsize = 11058
    I: kernsize_aligned = 2097152
    I: DTB will be placed on addr = 0x40e00000
    I: Created domain is paused
    To unpause issue: xu unpause 3
                    ^^^^^^^^^^^^^^ Xen domainId assigned to guest domain

Now unpause DomU guest domains with ``xu unpause``:

.. code-block:: console

    uart:~$ xu unpause 2
    uart:~$ xu unpause 3

Attach to DomU console with ``xu console``, use ``Ctrl-']'`` to exit console:

.. code-block:: console

    uart:~$ xu console 2
    Attached to a domain console
    [00:00:00.000,000] <inf> xen_events: xen_events_init: events inited

    [00:00:00.000,000] <inf> uart_hvc_xen: Xen HVC inited successfully

    *** Booting Zephyr OS build v3.6.0-72-g702a8af0cbe9 ***
    thread_a: Hello World from cpu 0 on xenvm!
    thread_b: Hello World from cpu 0 on xenvm!
    thread_a: Hello World from cpu 0 on xenvm!

Linux pv domain control
=======================

Create linux-pv-domu by using ``xu create`` with linux_pv_domu configuration and Xen
domainId ``-d 3``.

.. code-block:: console

    uart:~$ xu create linux_pv_domu -d 3
    W: Domain device tree generation is not supported
    I: rambase = 40000000, ramsize = 16777216
    I: kernbase = 40000000 kernsize = 364548, dtbsize = 11058
    I: kernsize_aligned = 2097152
    I: DTB will be placed on addr = 0x40e00000
    I: Created domain is paused
    ...

Attach to DomU console with ``xu console``, use ``Ctrl-']'`` to exit console:

.. code-block:: console

    uart:~$ xu console 3
    Attached to a domain console
    CPU 0
    [    0.001728] RCU Tasks: Setting shift to 1 and lim to 1 rcu_task_cb_adjust=1.
    [    0.001788] RCU Tasks Rude: Setting shift to 1 and lim to 1 rcu_task_cb_adjust=1.
    [    0.001832] RCU Tasks Trace: Setting shift to 1 and lim to 1 rcu_task_cb_adjust=1.
    ...
    [    0.514939] Freeing unused kernel memory: 8320K
    [    0.515149] Run /init as init process
    sh: write error: Invalid argument
    /bin/sh: can't access tty; job control turned off
    ~ #

Check PV block connection to the Domain:

.. code-block:: console
    ~ # ls -la /dev/xvda

Check PV net connection to the Domain:

.. code-block:: console
    ~ # ip addr add dev eth0 192.168.0.2/24
    ~ # ip link set eth0 up
    ~ # ping -c 3 192.168.0.1

.. note::
   -c 3 parameter for ping is needed because Ctrl+C is not working.

Also, linux-pv-domu can be accessed from DomD. For this please switch to DomD
and execute the following command:

.. code-block:: console
    ~ # ping -c 3 192.168.0.2

