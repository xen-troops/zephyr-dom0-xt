/*
 * Copyright (c) 2023 EPAM Systems
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <domain.h>
#include <string.h>
#include <zephyr/xen/public/domctl.h>

static char* domd_dtdevs[] = {
    "/soc/dma-controller@e65a0000",
    "/soc/dma-controller@e65b0000",
    "/soc/dma-controller@e6460000",
    "/soc/dma-controller@e6470000",
    "/soc/dma-controller@e6700000",
    "/soc/dma-controller@e7300000",
    "/soc/dma-controller@e7310000",
    "/soc/ethernet@e6800000",
    "/soc/video@e6ef0000",
    "/soc/video@e6ef1000",
    "/soc/video@e6ef2000",
    "/soc/video@e6ef3000",
    "/soc/video@e6ef4000",
    "/soc/video@e6ef5000",
    "/soc/dma-controller@ec700000",
    "/soc/dma-controller@ec720000",
    "/soc/usb@ee000000",
    "/soc/usb@ee080000",
    "/soc/usb@ee0a0000",
    "/soc/usb@ee080100",
    "/soc/usb@ee0a0100",
    "/soc/mmc@ee100000",
    "/soc/mmc@ee140000",
    "/soc/mmc@ee160000",
    "/soc/pcie@fe000000",
    "/soc/pcie@ee800000",
    "/soc/imr-lx4@fe860000",
    "/soc/imr-lx4@fe870000",
    "/soc/imr-lx4@fe880000",
    "/soc/imr-lx4@fe890000",
    "/soc/fdpm@fe940000",
    "/soc/fdpm@fe944000",
    "/soc/fcp@fea27000",
    "/soc/fcp@fea2f000",
    "/soc/fcp@fea37000",
    "/soc/vspm@fe960000",
    "/soc/vspm@fe920000",
    "/soc/vspm@fe9a0000",
    "/soc/vspm@fe9b0000",
    "/soc/hdmi@fead0000",
    "/soc/impdm0",
    "/soc/gsx_pv0_domd",
    "/soc/gsx_pv1_domd",
    "/soc/gsx_pv2_domd",
    "/soc/gsx_pv3_domd",
    "/soc/fcpcs_vc0",
    "/soc/fcpcs_vc1",
    "/soc/dma-controller@ffc10000",
    "/soc/dma-controller@ffc20000",
};

static char* dt_passthrough_nodes[] = {
    "/audio_clk_a",
    "/audio_clk_b",
    "/audio_clk_c",
    "/can",
    "/extal",
    "/extalr",
    "/gsx_opp_table0",
    "/gsx_opp_table1",
    "/gsx_opp_table2",
    "/gsx_opp_table3",
    "/gsx_opp_table4",
    "/gsx_opp_table5",
    "/gsx_opp_table6",
    "/gsx_opp_table7",
    "/pcie_bus",
    "/scif",
    "/soc",
    "/usb3s0",
    "/usb_extal",
    "/audio-clkout",
    "/avb-mch",
    "/hdmi0-out",
    "/keyboard",
    "/leds",
    "/regulator0",
    "/regulator1",
    "/regulator-vcc-sdhi0",
    "/regulator-vccq-sdhi0",
    "/x12",
    "/x23-clock",
    "/vspm_if",
    "/versaclk-3",
    "/versaclk-4",
    "/clksndsel",
    "/regulator-snd_3p3v",
    "/regulator-snd_vcc5v",
    "/regulator-wlan_en",
    "/regulator@9",
    "/regulator@11",
    "/regulator@12",
    "/regulator@13",
    "/regulator@14",
    "/regulator@15",
    "/regulator@16",
    "/regulator@17",
    "/kim",
    "/sound@0",
    "/sound@1",
    "/sound@2",
    "/sound@3",
    "/lvds",
    "/hdmi-out",
    "/si468x@0",
    "/wl18xx_pcm@0",
    "/camera_clk",
    "/ov5647_camera_clk",
    "/reserved-memory",
    "/mmngr",
    "/mmngrbuf",
    "/__symbols__",
};

static struct xen_domain_iomem domd_iomems[] = {
    {.first_mfn = 0x47fd7, .first_gfn = 0xe0000, .nr_mfns = 0x1},

    // 1:1 mapped lossy_decompress region
    {.first_mfn = 0x54000, .nr_mfns = 0x3000},
    // mfis@e6260000
    {.first_mfn = 0xe6260, .nr_mfns = 0x1},
    // watchdog@e6020000
    {.first_mfn = 0xe6020, .nr_mfns = 0x1},
    // gpio@e6050000
    {.first_mfn = 0xe6050, .nr_mfns = 0x1},
    // gpio@e6051000
    {.first_mfn = 0xe6051, .nr_mfns = 0x1},
    // gpio@e6052000
    {.first_mfn = 0xe6052, .nr_mfns = 0x1},
    // gpio@e6053000
    {.first_mfn = 0xe6053, .nr_mfns = 0x1},
    // gpio@e6054000
    {.first_mfn = 0xe6054, .nr_mfns = 0x1},
    // gpio@e6055000
    // gpio@e6055400
    // gpio@e6055800
    {.first_mfn = 0xe6055, .nr_mfns = 0x1},
    // pinctrl@e6060000
    {.first_mfn = 0xe6060, .nr_mfns = 0x1},
    // avs@e60a013c
    {.first_mfn = 0xe60a0, .nr_mfns = 0x1},
    // clock-controller@e6150000
    // imprtt
    {.first_mfn = 0xe6150, .nr_mfns = 0x1},
    // reset-controller@e6160000
    {.first_mfn = 0xe6160, .nr_mfns = 0x1},
    // system-controller@e6180000
    {.first_mfn = 0xe6180, .nr_mfns = 0x1},
    // thermal@e6198000
    {.first_mfn = 0xe6198, .nr_mfns = 0x1},
    // thermal@e6198000
    {.first_mfn = 0xe61a0, .nr_mfns = 0x1},
    // thermal@e6198000
    {.first_mfn = 0xe61a8, .nr_mfns = 0x1},
    // interrupt-controller@e61c0000
    {.first_mfn = 0xe61c0, .nr_mfns = 0x1},
    // i2c@e6510000
    {.first_mfn = 0xe6510, .nr_mfns = 0x1},
    // i2c@e66d8000
    {.first_mfn = 0xe66d8, .nr_mfns = 0x1},
    // i2c@e60b0000
    {.first_mfn = 0xe60b0, .nr_mfns = 0x1},
    // serial@e6540000
    {.first_mfn = 0xe6540, .nr_mfns = 0x1},
    // serial@e6550000
    {.first_mfn = 0xe6550, .nr_mfns = 0x1},
    // usb@e6590000
    {.first_mfn = 0xe6590, .nr_mfns = 0x1},
    // dma-controller@e65a0000
    {.first_mfn = 0xe65a0, .nr_mfns = 0x1},
    // dma-controller@e65b0000
    {.first_mfn = 0xe65b0, .nr_mfns = 0x1},
    // dma-controller@e6460000
    {.first_mfn = 0xe6460, .nr_mfns = 0x1},
    // dma-controller@e6470000
    {.first_mfn = 0xe6470, .nr_mfns = 0x1},
    // crypto@e6601000
    {.first_mfn = 0xe6601, .nr_mfns = 0x1},
    // dma-controller@e6700000
    {.first_mfn = 0xe6700, .nr_mfns = 0x10},
    // dma-controller@e7300000
    {.first_mfn = 0xe7300, .nr_mfns = 0x10},
    // dma-controller@e7310000
    {.first_mfn = 0xe7310, .nr_mfns = 0x10},
    // qos@e67e0000
    {.first_mfn = 0xe67e0, .nr_mfns = 0x11},
    // ethernet@e6800000
    {.first_mfn = 0xe6800, .nr_mfns = 0x1},
    // ethernet@e6800000
    {.first_mfn = 0xe6a00, .nr_mfns = 0x10},
    // can@e6c30000
    {.first_mfn = 0xe6c30, .nr_mfns = 0x1},
    // can@e6c38000
    {.first_mfn = 0xe6c38, .nr_mfns = 0x1},
    // serial@e6e68000
    {.first_mfn = 0xe6e68, .nr_mfns = 0x1},
    // video@e6ef0000
    {.first_mfn = 0xe6ef0, .nr_mfns = 0x1},
    // video@e6ef1000
    {.first_mfn = 0xe6ef1, .nr_mfns = 0x1},
    // video@e6ef2000
    {.first_mfn = 0xe6ef2, .nr_mfns = 0x1},
    // video@e6ef3000
    {.first_mfn = 0xe6ef3, .nr_mfns = 0x1},
    // video@e6ef4000
    {.first_mfn = 0xe6ef4, .nr_mfns = 0x1},
    // video@e6ef5000
    {.first_mfn = 0xe6ef5, .nr_mfns = 0x1},
    // sound@ec500000
    {.first_mfn = 0xec500, .nr_mfns = 0x1},
    // sound@ec500000
    // avb-mch
    {.first_mfn = 0xec5a0, .nr_mfns = 0x1},
    // sound@ec500000
    {.first_mfn = 0xec540, .nr_mfns = 0x1},
    // sound@ec500000
    {.first_mfn = 0xec541, .nr_mfns = 0x1},
    // sound@ec500000
    {.first_mfn = 0xec760, .nr_mfns = 0x1},
    // sound@ec500000
    {.first_mfn = 0xec000, .nr_mfns = 0x1},
    // sound@ec500000
    {.first_mfn = 0xec008, .nr_mfns = 0x1},
    // sound@ec500000
    {.first_mfn = 0xec100, .nr_mfns = 0x100},
    // dma-controller@ec700000
    {.first_mfn = 0xec700, .nr_mfns = 0x10},
    // dma-controller@ec720000
    {.first_mfn = 0xec720, .nr_mfns = 0x10},
    // usb@ee000000
    {.first_mfn = 0xee000, .nr_mfns = 0x1},
    // usb@ee080000
    // usb@ee080100
    // usb-phy@ee080200
    {.first_mfn = 0xee080, .nr_mfns = 0x1},
    // usb@ee0a0000
    // usb@ee0a0100
    // usb-phy@ee0a0200
    {.first_mfn = 0xee0a0, .nr_mfns = 0x1},
    // mmc@ee100000
    {.first_mfn = 0xee100, .nr_mfns = 0x2},
    // mmc@ee140000
    {.first_mfn = 0xee140, .nr_mfns = 0x2},
    // mmc@ee160000
    {.first_mfn = 0xee160, .nr_mfns = 0x2},
    // gsx@fd000000
    {.first_mfn = 0xfd000, .nr_mfns = 0x40},
    // pcie@fe000000
    {.first_mfn = 0xfe000, .nr_mfns = 0x80},
    // pcie@ee800000
    {.first_mfn = 0xee800, .nr_mfns = 0x80},
    // imr-lx4@fe860000
    // imrlx4_imr0
    {.first_mfn = 0xfe860, .nr_mfns = 0x2},
    // imr-lx4@fe870000
    // imrlx4_imr1
    {.first_mfn = 0xfe870, .nr_mfns = 0x2},
    // imr-lx4@fe880000
    // imrlx4_imr2
    {.first_mfn = 0xfe880, .nr_mfns = 0x2},
    // imr-lx4@fe890000
    // imrlx4_imr3
    {.first_mfn = 0xfe890, .nr_mfns = 0x2},
    // vcp4@fe90f000
    {.first_mfn = 0xfe90f, .nr_mfns = 0x1},
    // vcp4@fe8d0000
    // vcp4@fe8d0000
    {.first_mfn = 0xfe8d0, .nr_mfns = 0x1},
    // vcp4@fe910000
    // vcp4@fe910000
    {.first_mfn = 0xfe910, .nr_mfns = 0x1},
    // vcp4@fe900000
    // vcp4@fe900000
    {.first_mfn = 0xfe900, .nr_mfns = 0x1},
    // fdpm@fe940000
    {.first_mfn = 0xfe940, .nr_mfns = 0x3},
    // fdpm@fe940000
    {.first_mfn = 0xfe950, .nr_mfns = 0x1},
    // fdpm@fe944000
    {.first_mfn = 0xfe944, .nr_mfns = 0x3},
    // fdpm@fe944000
    {.first_mfn = 0xfe951, .nr_mfns = 0x1},
    // fcp@fea27000
    {.first_mfn = 0xfea27, .nr_mfns = 0x1},
    // fcp@fea2f000
    {.first_mfn = 0xfea2f, .nr_mfns = 0x1},
    // fcp@fea37000
    {.first_mfn = 0xfea37, .nr_mfns = 0x1},
    // vsp@fe960000
    // vspm@fe960000
    {.first_mfn = 0xfe960, .nr_mfns = 0x8},
    // vspm@fe960000
    {.first_mfn = 0xfe96f, .nr_mfns = 0x1},
    // vsp@fe920000
    // vspm@fe920000
    {.first_mfn = 0xfe920, .nr_mfns = 0x8},
    // vspm@fe920000
    {.first_mfn = 0xfe92f, .nr_mfns = 0x1},
    // vsp@fea20000
    {.first_mfn = 0xfea20, .nr_mfns = 0x5},
    // vsp@fea28000
    {.first_mfn = 0xfea28, .nr_mfns = 0x5},
    // vsp@fea30000
    {.first_mfn = 0xfea30, .nr_mfns = 0x5},
    // vsp@fe9a0000
    // vspm@fe9a0000
    {.first_mfn = 0xfe9a0, .nr_mfns = 0x8},
    // vspm@fe9a0000
    {.first_mfn = 0xfe9af, .nr_mfns = 0x1},
    // vsp@fe9b0000
    // vspm@fe9b0000
    {.first_mfn = 0xfe9b0, .nr_mfns = 0x8},
    // vspm@fe9b0000
    {.first_mfn = 0xfe9bf, .nr_mfns = 0x1},
    // csi2@fea80000
    {.first_mfn = 0xfea80, .nr_mfns = 0x10},
    // csi2@feaa0000
    {.first_mfn = 0xfeaa0, .nr_mfns = 0x10},
    // hdmi@fead0000
    {.first_mfn = 0xfead0, .nr_mfns = 0x10},
    // display@feb00000
    {.first_mfn = 0xfeb00, .nr_mfns = 0x80},
    // lvds@feb90000
    {.first_mfn = 0xfeb90, .nr_mfns = 0x1},
    // chipid@fff00044
    {.first_mfn = 0xfff00, .nr_mfns = 0x1},
    // impdes0
    {.first_mfn = 0xffa00, .nr_mfns = 0x4},
    // imp0
    {.first_mfn = 0xff900, .nr_mfns = 0x20},
    // imp1
    {.first_mfn = 0xff920, .nr_mfns = 0x20},
    // imp2
    {.first_mfn = 0xff940, .nr_mfns = 0x20},
    // imp3
    {.first_mfn = 0xff960, .nr_mfns = 0x20},
    // impsc0
    {.first_mfn = 0xff980, .nr_mfns = 0x10},
    // impsc1
    {.first_mfn = 0xff990, .nr_mfns = 0x10},
    // impsl0
    {.first_mfn = 0xff9c0, .nr_mfns = 0x10},
    // impdm0
    {.first_mfn = 0xffa10, .nr_mfns = 0x4},
    // impc0
    {.first_mfn = 0xffa40, .nr_mfns = 0x20},
    // imprtt
    {.first_mfn = 0xff8d0, .nr_mfns = 0x1},
    // dma-controller@ffc10000
    {.first_mfn = 0xffc10, .nr_mfns = 0x10},
    // dma-controller@ffc20000
    {.first_mfn = 0xffc20, .nr_mfns = 0x10},
    // cmm@fea40000
    {.first_mfn = 0xfea40, .nr_mfns = 0x1},
    // cmm@fea50000
    {.first_mfn = 0xfea50, .nr_mfns = 0x1},
    // cmm@fea60000
    {.first_mfn = 0xfea60, .nr_mfns = 0x1},
    // cmm@fea70000
    {.first_mfn = 0xfea70, .nr_mfns = 0x1},
};

static uint32_t domd_irqs[] = {
    // gpio@e6050000
    36,
    // gpio@e6051000
    37,
    // gpio@e6052000
    38,
    // gpio@e6053000
    39,
    // gpio@e6054000
    40,
    // gpio@e6055000
    41,
    // gpio@e6055400
    42,
    // gpio@e6055800
    43,
    // thermal@e6198000
    99, 100, 101,
    // interrupt-controller@e61c0000
    32, 33, 34, 35, 193, 50,
    // i2c@e6510000
    318,
    // i2c@e66d8000
    51,
    // i2c@e60b0000
    205,
    // serial@e6540000
    186,
    // serial@e6550000
    187,
    // usb@e6590000
    139,
    // dma-controller@e65a0000
    141,
    // dma-controller@e65b0000
    142,
    // dma-controller@e6460000
    66,
    // dma-controller@e6470000
    67,
    // crypto@e6601000
    103,
    // dma-controller@e6700000
    231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247,
    // dma-controller@e7300000
    251, 345, 252, 346, 248, 340, 341, 342, 343, 344, 249, 250, 347, 348, 349, 350, 351,
    // dma-controller@e7310000
    448, 449, 450, 451, 452, 453, 454, 455, 456, 457, 458, 459, 460, 461, 462, 463, 429,
    // ethernet@e6800000
    71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
    // can@e6c30000
    218,
    // can@e6c38000
    219,
    // serial@e6e68000
    185,
    // video@e6ef0000
    220,
    // video@e6ef1000
    221,
    // video@e6ef2000
    222,
    // video@e6ef3000
    223,
    // video@e6ef4000
    206,
    // video@e6ef5000
    207,
    // src-0
    384,
    // src-1
    385,
    // src-2
    386,
    // src-3
    387,
    // src-4
    388,
    // src-5
    389,
    // src-6
    390,
    // src-7
    391,
    // src-8
    392,
    // src-9
    393,
    // ssi-0
    402,
    // ssi-1
    403,
    // ssi-2
    404,
    // ssi-3
    405,
    // ssi-4
    406,
    // ssi-5
    407,
    // ssi-6
    408,
    // ssi-7
    409,
    // ssi-8
    410,
    // ssi-9
    411,
    // dma-controller@ec700000
    352, 353, 354, 355, 356, 357, 358, 359, 360, 361, 362, 363, 364, 365, 366, 367, 382,
    // dma-controller@ec720000
    383, 368, 369, 370, 371, 372, 373, 374, 375, 376, 377, 378, 379, 380, 381, 414, 415,
    // usb@ee000000
    134,
    // usb@ee080000
    140,
    // usb@ee0a0000
    144,
    // mmc@ee100000
    197,
    // mmc@ee140000
    199,
    // mmc@ee160000
    200,
    // gsx@fd000000
    151,
    // pcie@fe000000
    148, 149, 150,
    // pcie@ee800000
    180, 181, 182,
    // imr-lx4@fe860000
    224,
    // imr-lx4@fe870000
    225,
    // imr-lx4@fe880000
    226,
    // imr-lx4@fe890000
    227,
    // vcp4@fe8d0000
    412, 413, 255,
    // vcp4@fe910000
    292, 293,
    // vcp4@fe900000
    272, 273,
    // fdpm@fe940000
    294,
    // fdpm@fe944000
    295,
    // vspm@fe960000
    298,
    // vspm@fe920000
    497,
    // vsp@fea20000
    498,
    // vsp@fea28000
    499,
    // vsp@fea30000
    500,
    // vspm@fe9a0000
    476,
    // vspm@fe9b0000
    477,
    // csi2@fea80000
    216,
    // csi2@feaa0000
    278,
    // hdmi@fead0000
    421,
    // display@feb00000
    288, 300, 301, 302,
    // impdes0
    471,
    // imprtt
    425,
    // dma-controller@ffc10000
    480, 481, 482, 483, 484, 485, 486, 487, 488,
    // dma-controller@ffc20000
    501, 489, 490, 491, 492, 493, 494, 495, 496,
};

extern char __img_ipl_start[];
extern char __img_ipl_end[];
extern char __dtb_ipl_start[];
extern char __dtb_ipl_end[];

static int load_ipl_image(uint8_t* buf, size_t bufsize, uint64_t image_load_offset, void* image_info)
{
    ARG_UNUSED(image_info);
    memcpy(buf, __img_ipl_start + image_load_offset, bufsize);
    return 0;
}

static ssize_t get_ipl_image_size(void* image_info, uint64_t* size)
{
    ARG_UNUSED(image_info);
    *size = __img_ipl_end - __img_ipl_start;
    return 0;
}

struct xen_domain_cfg domd_cfg = {
    .machine_dt_compat = "renesas,h3ulcb",
    .mem_kb = 0x200000, /* 2GB */

    .cmdline = "console=hvc0 clk_ignore_unused root=/dev/nfs nfsroot=10.22.64.15:/home/testrpi1/nfs/h3ulcb,vers=3 ip=dhcp",

    .flags               = (XEN_DOMCTL_CDF_hvm | XEN_DOMCTL_CDF_hap | XEN_DOMCTL_CDF_iommu),
    .max_evtchns         = 10,
    .max_vcpus           = 4,
    .gnt_frames          = 32,
    .max_maptrack_frames = 1,

    .iomems    = domd_iomems,
    .nr_iomems = ARRAY_SIZE(domd_iomems),

    .irqs    = domd_irqs,
    .nr_irqs = ARRAY_SIZE(domd_irqs),

    .gic_version = XEN_DOMCTL_CONFIG_GIC_V2,
    .tee_type    = XEN_DOMCTL_CONFIG_TEE_NONE,

    .dtdevs    = domd_dtdevs,
    .nr_dtdevs = ARRAY_SIZE(domd_dtdevs),

    .dt_passthrough    = dt_passthrough_nodes,
    .nr_dt_passthrough = ARRAY_SIZE(dt_passthrough_nodes),
    .load_image_bytes  = load_ipl_image,
    .get_image_size    = get_ipl_image_size,
    .image_info        = NULL,

    .dtb_start = __dtb_ipl_start,
    .dtb_end   = __dtb_ipl_end,
};

