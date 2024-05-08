Overview
********

.. _XEN: https://xenproject.org/
.. _Xen-Troops: https://github.com/xen-troops
.. _zephyr-xenlib: https://github.com/xen-troops/zephyr-xenlib

A simple `XEN`_ Zephyr Dom0 application **zephyr-dom0-xt** showcasing the use of `Xen-Troops`_
library `zephyr-xenlib`_ to create and manage `XEN`_ domains from
Zephyr application by using `zephyr-xenlib`_ API and SHELL interface command.

The **zephyr-dom0-xt** application intended to be started by `XEN`_ as Dom0 domain.

The **zephyr-dom0-xt** dosn't create any `XEN`_ guest domains on start by default.

Requirements
************

Supported platforms:

- TODO: what is the board for default cfg domd_cfg.c
- **rpi_5** `Raspberry Pi 5 <https://www.raspberrypi.com/products/raspberry-pi-5/>`_

Prepared Xen guest domains configuration files in ``src/dom_cfg/`` folder.

Prepared Xen guest domains binary image files of kernel and device-tree in ``prebuilt/`` folder.

The Xen quest domains which uses real HW shell be created and started only once even though
`zephyr-xenlib`_ doesn't prevent it. From another side, the Xen quest domain which doesn't use
any real HW can be started as many times as `zephyr-xenlib`_ allows using the same Xen guest domains
configuration.

Devicetree details
==================

The **zephyr-dom0-xt** on devicetree configuration depends on used board/platform.

Xen guest domains configuration
*******************************

The **zephyr-dom0-xt** application uses Xen guest domains configuration in `zephyr-xenlib`_
domain configuration format defined by ``struct xen_domain_cfg``.

The number of Xen guest domains configurations supported by **zephyr-dom0-xt** is specified
by Kconfig option **CONFIG_DOM_CFG_NUMBER** (default 1).

.. note::

    Max number of supported Xen domains configuration now is 2 due to inability to integrate
    Xen guest domains binary image files for more than 2 Xen domain configurations.

Each Xen guest domains configurations shell be defined as global variable named **domd_cfg_X**,
where X is in [0, CONFIG_DOM_CFG_NUMBER].

One configuration example, ``CONFIG_DOM_CFG_NUMBER=1``:

.. code:: C

    struct xen_domain_cfg domd_cfg_0 = {
    }

Two configurations example, ``CONFIG_DOM_CFG_NUMBER=2``:

.. code:: C

    struct xen_domain_cfg domd_cfg_0 = {
    }

    struct xen_domain_cfg domd_cfg_1 = {
    }

There is an assumption that if some Xen quest domain intend to be defined as
``Xen drivers domain (DomD)``, its configuration shall be defined as ``domd_cfg_0``.

The **zephyr-dom0-xt** selects Xen guest domains configurations file (containing ``domd_cfg_X``)
basing on:

- The **CONFIG_USE_DEFAULT_DOM_CFG** (default=y) Kconfig option. if selected the ``src/domd_cfg.c``
  file will be used (backward compatible behavior).
- if CONFIG_USE_DEFAULT_DOM_CFG=n the Xen guest domains configurations file will be selected basing
  on Zephyr board and **CONFIG_DOM_CFG_BOARD_EXT** Kconfig option as following

.. code-block:: text

      if DEFINED(CONFIG_DOM_CFG_BOARD_EXT)
        use  src/dom_cfg/${BOARD}_${CONFIG_DOM_CFG_BOARD_EXT}.c
      else()
        use src/dom_cfg/${BOARD}.c

For example Zephyr board is **rpi_5** and `CONFIG_USE_DEFAULT_DOM_CFG=n``:

.. code-block:: text

    src/dom_cfg/rpi_5.c
    CONFIG_DOM_CFG_BOARD_EXT=\"domu\" ---> src/dom_cfg/rpi_5_domu.c
    CONFIG_DOM_CFG_BOARD_EXT=\"domd\" ---> src/dom_cfg/rpi_5_domd.c

Xen guest domains binary images
*******************************

The **zephyr-dom0-xt** takes pre-built Xen guest domains images from ``prebuilt/`` folder and
they are integrated directly into Zephyr **zephyr-dom0-xt**. Now only two sets of Xen guest images
is supported:

- **domd.dtb/domd_img.bin** to be used with **domd_cfg_0** Xen guest domain configuration.
  By default it's TODO:.
- **domu.dtb/domu_img.bin** to be used with **domd_cfg_1** Xen guest domain configuration.
  By default it's DomU based on Zephyr **xenvm** board and samples/synchronization sample.
  No real HW.

To use custom binary images they shell be copied into ``prebuilt/`` folder keeping above
naming conventions.

This approach is not very convenient and in todo list.

Building and Running
********************

The **zephyr-dom0-xt** can be built for multiple supported boards.

For example, to build it for **rpi_5** board with one Xen guest domain configuration
(DomU, configuration in ``src/dom_cfg/rpi_5_domu.c``) run below command:

.. code-block:: bash

    west build -b rpi_5 -p always -S xen_dom0 zephyr-dom0-xt -- \
    -DCONFIG_USE_DEFAULT_DOM_CFG=n -DCONFIG_DOM_CFG_BOARD_EXT=\"domu\"

For example, to build it for **rpi_5** board with two Xen guest domain configurations
(DomD/DomU, configuration in ``src/dom_cfg/rpi_5.c``) run below command:

.. code-block:: bash

    west build -b rpi_5 -p always -S xen_dom0 zephyr-dom0-xt -- \
    -DCONFIG_USE_DEFAULT_DOM_CFG=n -DCONFIG_DOM_CFG_NUMBER=2

Running
=======

The **zephyr-dom0-xt** application shell be started by `XEN`_ as Dom0 domain.
This process is out of scope of this document.


RPI5 Dom0 run example with two domains configuration
****************************************************

The **zephyr-dom0-xt** shell be build with already prepared Xen guest domain configuration file
``src/dom_cfg/rpi_5.c`` for RPI5 which contains two Xen guest domain configurations:

- **domd_cfg_0**: It mockups DomD behavior and uses real RPI5 GPIO HW ``/soc/gpio@7d517c00``.
  It's intended to be used with Zephyr Blinky sample.
- **domd_cfg_1**: It mockups DomU which pure Xen guest domain without using real RPI5 HW.
  It's intended to be used with Zephyr samples/synchronization sample.

Prepare Xen guest domains binaries
==================================

Build Zephyr Blinky sample for Zephyr **xenvm** board with RPI5 snippet ``rpi_5_xen_domd``:

.. code-block:: bash

    west build -b xenvm -p always -S rpi_5_xen_domd samples/basic/blinky

Once build is finished copy ``zephyr.bin`` into the ``zephyr/zephyr-dom0-xt/prebuilt/domd_img.bin``.

Build Zephyr samples/synchronization sample for Zephyr **xenvm** board:

.. code-block:: bash

    west build -b xenvm -p always samples/synchronization

Once build is finished copy ``zephyr.bin`` into the ``zephyr/zephyr-dom0-xt/prebuilt/domu_img.bin``.

Build zephyr-dom0-xt
====================

Run below command to build **zephyr-dom0-xt**:

.. code-block:: bash

    west build -b rpi_5 -p always -S xen_dom0 zephyr-dom0-xt -- \
    -DCONFIG_USE_DEFAULT_DOM_CFG=n -DCONFIG_DOM_CFG_NUMBER=2

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

    uart:~$ xu create rpi_5_domu -d 2
    W: Domain device tree generation is not supported
    I: rambase = 40000000, ramsize = 16777216
    I: kernbase = 40000000 kernsize = 364548, dtbsize = 11058
    I: kernsize_aligned = 2097152
    I: DTB will be placed on addr = 0x40e00000
    I: Created domain is paused
    To unpause issue: xu unpause 2
    uart:~$ xu unpause 2

Create DomU by using ``xu create`` with DomU configuration and Xen domainId ``-d 0``.
Note the `zephyr-xenlib`_ will assign Xen domainId automatically with ``-d 0``.

.. code-block:: console

    uart:~$ xu create rpi_5_domu -d 0
    W: Domain device tree generation is not supported
    I: rambase = 40000000, ramsize = 16777216
    I: kernbase = 40000000 kernsize = 364548, dtbsize = 11058
    I: kernsize_aligned = 2097152
    I: DTB will be placed on addr = 0x40e00000
    I: Created domain is paused
    To unpause issue: xu unpause 3
                    ^^^^^^^^^^^^^^ Xen domainId assigned to quest domain

Now unpause DomU quest domains with ``xu unpause``:

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
