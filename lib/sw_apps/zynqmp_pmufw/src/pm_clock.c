/*
 * Copyright (C) 2014 - 2016 Xilinx, Inc.  All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Except as contained in this notice, the name of the Xilinx shall not be used
 * in advertising or otherwise to promote the sale, use or other dealings in
 * this Software without prior written authorization from Xilinx.
 */
#include "xpfw_config.h"
#ifdef ENABLE_PM

#include "pm_clock.h"
#include "pm_power.h"
#include "pm_usb.h"
#include "pm_periph.h"
#include "pm_ddr.h"
#include "crf_apb.h"
#include "crl_apb.h"

static const PmClockSel2Pll advSel2Pll[] = {
	{
		.pll = &pmApll_g,
		.select = 0U,
	}, {
		.pll = &pmDpll_g,
		.select = 2U,
	}, {
		.pll = &pmVpll_g,
		.select = 3U,
	},
};

static PmClockMux advMux = {
	.inputs = advSel2Pll,
	.size = ARRAY_SIZE(advSel2Pll),
};

static const PmClockSel2Pll avdSel2Pll[] = {
	{
		.pll = &pmApll_g,
		.select = 0U,
	}, {
		.pll = &pmVpll_g,
		.select = 2U,
	}, {
		.pll = &pmDpll_g,
		.select = 3U,
	},
};

static PmClockMux avdMux = {
	.inputs = avdSel2Pll,
	.size = ARRAY_SIZE(avdSel2Pll),
};

static const PmClockSel2Pll aiodSel2Pll[] = {
	{
		.pll = &pmApll_g,
		.select = 0U,
	}, {
		.pll = &pmIOpll_g,
		.select = 2U,
	}, {
		.pll = &pmDpll_g,
		.select = 3U,
	},
};

static PmClockMux aiodMux = {
	.inputs = aiodSel2Pll,
	.size = ARRAY_SIZE(aiodSel2Pll),
};

static const PmClockSel2Pll vdrSel2Pll[] = {
	{
		.pll = &pmVpll_g,
		.select = 0U,
	}, {
		.pll = &pmDpll_g,
		.select = 2U,
	}, {
		.pll = &pmRpll_g,
		.select = 3U,
	},
};

static PmClockMux vdrMux = {
	.inputs = vdrSel2Pll,
	.size = ARRAY_SIZE(vdrSel2Pll),
};

static const PmClockSel2Pll dvSel2Pll[] = {
	{
		.pll = &pmDpll_g,
		.select = 0U,
	}, {
		.pll = &pmVpll_g,
		.select = 1U,
	},
};

static PmClockMux dvMux = {
	.inputs = dvSel2Pll,
	.size = ARRAY_SIZE(dvSel2Pll),
};

static const PmClockSel2Pll iovdSel2Pll[] = {
	{
		.pll = &pmIOpll_g,
		.select = 0U,
	}, {
		.pll = &pmVpll_g,
		.select = 2U,
	}, {
		.pll = &pmDpll_g,
		.select = 3U,
	},
};

static PmClockMux iovdMux = {
	.inputs = iovdSel2Pll,
	.size = ARRAY_SIZE(iovdSel2Pll),
};

static const PmClockSel2Pll ioadSel2Pll[] = {
	{
		.pll = &pmIOpll_g,
		.select = 0U,
	}, {
		.pll = &pmApll_g,
		.select = 2U,
	}, {
		.pll = &pmDpll_g,
		.select = 3U,
	},
};

static PmClockMux ioadMux = {
	.inputs = ioadSel2Pll,
	.size = ARRAY_SIZE(ioadSel2Pll),
};

static const PmClockSel2Pll iodaSel2Pll[] = {
	{
		.pll = &pmIOpll_g,
		.select = 0U,
	}, {
		.pll = &pmDpll_g,
		.select = 2U,
	}, {
		.pll = &pmApll_g,
		.select = 3U,
	},
};

static PmClockMux iodaMux = {
	.inputs = iodaSel2Pll,
	.size = ARRAY_SIZE(iodaSel2Pll),
};

static const PmClockSel2Pll iorSel2Pll[] = {
	{
		.pll = &pmIOpll_g,
		.select = 0U,
	}, {
		.pll = &pmRpll_g,
		.select = 2U,
	},
};

static PmClockMux iorMux = {
	.inputs = iorSel2Pll,
	.size = ARRAY_SIZE(iorSel2Pll),
};

static const PmClockSel2Pll iordSel2Pll[] = {
	{
		.pll = &pmIOpll_g,
		.select = 0U,
	}, {
		.pll = &pmRpll_g,
		.select = 2U,
	}, {
		.pll = &pmDpll_g,
		.select = 3U,
	},
};

static PmClockMux iordMux = {
	.inputs = iordSel2Pll,
	.size = ARRAY_SIZE(iordSel2Pll),
};

static const PmClockSel2Pll iorvSel2Pll[] = {
	{
		.pll = &pmIOpll_g,
		.select = 0U,
	}, {
		.pll = &pmRpll_g,
		.select = 2U,
	}, {
		.pll = &pmVpll_g,
		.select = 3U,
	},
};

static PmClockMux iorvMux = {
	.inputs = iorvSel2Pll,
	.size = ARRAY_SIZE(iorvSel2Pll),
};

static const PmClockSel2Pll riodSel2Pll[] = {
	{
		.pll = &pmRpll_g,
		.select = 0U,
	}, {
		.pll = &pmIOpll_g,
		.select = 2U,
	}, {
		.pll = &pmDpll_g,
		.select = 3U,
	},
};

static PmClockMux riodMux = {
	.inputs = riodSel2Pll,
	.size = ARRAY_SIZE(riodSel2Pll),
};

/* CRF_APB clocks */

static PmClock pmClockAcpu = {
	.mux = &advMux,
	.ctrlAddr = CRF_APB_ACPU_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

/* Floating clock */
static PmClock pmClockDbgTrace = {
	.mux = &iodaMux,
	.ctrlAddr = CRF_APB_DBG_TRACE_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

/* Floating clock */
static PmClock pmClockDbgFpd = {
	.mux = &iodaMux,
	.ctrlAddr = CRF_APB_DBG_FPD_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockDpVideo = {
	.mux = &vdrMux,
	.ctrlAddr = CRF_APB_DP_VIDEO_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockDpAudio = {
	.mux = &vdrMux,
	.ctrlAddr = CRF_APB_DP_AUDIO_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockDpStc = {
	.mux = &vdrMux,
	.ctrlAddr = CRF_APB_DP_STC_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockDdr __attribute__((__section__(".srdata"))) = {
	.mux = &dvMux,
	.ctrlAddr = CRF_APB_DDR_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockGpu = {
	.mux = &iovdMux,
	.ctrlAddr = CRF_APB_GPU_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockSata = {
	.mux = &ioadMux,
	.ctrlAddr = CRF_APB_SATA_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockPcie = {
	.mux = &iordMux,
	.pll = NULL,
	.users = NULL,
	.ctrlAddr = CRF_APB_PCIE_REF_CTRL,
	.ctrlVal = 0U,
};

static PmClock pmClockGdma = {
	.mux = &avdMux,
	.ctrlAddr = CRF_APB_GDMA_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockDpDma = {
	.mux = &avdMux,
	.ctrlAddr = CRF_APB_DPDMA_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockTopSwMain __attribute__((__section__(".srdata"))) = {
	.mux = &avdMux,
	.ctrlAddr = CRF_APB_TOPSW_MAIN_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockTopSwLsBus __attribute__((__section__(".srdata"))) = {
	.mux = &aiodMux,
	.ctrlAddr = CRF_APB_TOPSW_LSBUS_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

/* Floating clock */
static PmClock pmClockGtgRef0 = {
	.mux = &ioadMux,
	.ctrlAddr = CRF_APB_GTGREF0_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

/* Floating clock */
static PmClock pmClockDbgTstmp = {
	.mux = &advMux,
	.ctrlAddr = CRF_APB_DBG_TSTMP_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

/* CRL_APB clocks */

/* Floating clock */
static PmClock pmClockUsb3Dual = {
	.mux = &iordMux,
	.ctrlAddr = CRL_APB_USB3_DUAL_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockGem0 = {
	.mux = &iordMux,
	.ctrlAddr = CRL_APB_GEM0_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockGem1 = {
	.mux = &iordMux,
	.ctrlAddr = CRL_APB_GEM1_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockGem2 = {
	.mux = &iordMux,
	.ctrlAddr = CRL_APB_GEM2_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockGem3 = {
	.mux = &iordMux,
	.ctrlAddr = CRL_APB_GEM3_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockUsb0Bus = {
	.mux = &iordMux,
	.ctrlAddr = CRL_APB_USB0_BUS_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockUsb1Bus = {
	.mux = &iordMux,
	.ctrlAddr = CRL_APB_USB1_BUS_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockQSpi = {
	.mux = &iordMux,
	.ctrlAddr = CRL_APB_QSPI_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockSdio0 = {
	.mux = &iorvMux,
	.ctrlAddr = CRL_APB_SDIO0_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockSdio1 = {
	.mux = &iorvMux,
	.ctrlAddr = CRL_APB_SDIO1_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockUart0 = {
	.mux = &iordMux,
	.ctrlAddr = CRL_APB_UART0_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockUart1 = {
	.mux = &iordMux,
	.ctrlAddr = CRL_APB_UART1_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockSpi0 = {
	.mux = &iordMux,
	.ctrlAddr = CRL_APB_SPI0_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockSpi1 = {
	.mux = &iordMux,
	.ctrlAddr = CRL_APB_SPI1_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockCan0 = {
	.mux = &iordMux,
	.ctrlAddr = CRL_APB_CAN0_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockCan1 = {
	.mux = &iordMux,
	.ctrlAddr = CRL_APB_CAN1_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockCpuR5 = {
	.mux = &riodMux,
	.ctrlAddr = CRL_APB_CPU_R5_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

/* Floating clock */
static PmClock pmClockIouSwitch = {
	.mux = &riodMux,
	.ctrlAddr = CRL_APB_IOU_SWITCH_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockCsuPll = {
	.mux = &iordMux,
	.ctrlAddr = CRL_APB_CSU_PLL_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockPcap = {
	.mux = &iordMux,
	.ctrlAddr = CRL_APB_PCAP_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

/* Floating clock */
static PmClock pmClockLpdSwitch = {
	.mux = &riodMux,
	.ctrlAddr = CRL_APB_LPD_SWITCH_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockLpdLsBus = {
	.mux = &riodMux,
	.ctrlAddr = CRL_APB_LPD_LSBUS_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

/* Floating clock */
static PmClock pmClockDbgLpd = {
	.mux = &riodMux,
	.ctrlAddr = CRL_APB_DBG_LPD_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockNand = {
	.mux = &iordMux,
	.ctrlAddr = CRL_APB_NAND_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockAdma = {
	.mux = &riodMux,
	.ctrlAddr = CRL_APB_ADMA_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockPl0 = {
	.mux = &iordMux,
	.ctrlAddr = CRL_APB_PL0_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockPl1 = {
	.mux = &iordMux,
	.ctrlAddr = CRL_APB_PL1_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockPl2 = {
	.mux = &iordMux,
	.ctrlAddr = CRL_APB_PL2_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockPl3 = {
	.mux = &iordMux,
	.ctrlAddr = CRL_APB_PL3_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockGemTsu = {
	.mux = &iordMux,
	.ctrlAddr = CRL_APB_GEM_TSU_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockDll = {
	.mux = &iorMux,
	.ctrlAddr = CRL_APB_DLL_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

/* Floating clock */
static PmClock pmClockAms = {
	.mux = &riodMux,
	.ctrlAddr = CRL_APB_AMS_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockI2C0 = {
	.mux = &iordMux,
	.ctrlAddr = CRL_APB_I2C0_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock pmClockI2C1 = {
	.mux = &iordMux,
	.ctrlAddr = CRL_APB_I2C1_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

/* Floating clock */
static PmClock pmClockTimeStamp = {
	.mux = &riodMux,
	.ctrlAddr = CRL_APB_TIMESTAMP_REF_CTRL,
	.pll = NULL,
	.users = NULL,
	.ctrlVal = 0U,
};

static PmClock* pmClocks[] = {
	&pmClockAcpu,
	&pmClockDbgTrace,
	&pmClockDbgFpd,
	&pmClockDpVideo,
	&pmClockDpAudio,
	&pmClockDpStc,
	&pmClockDdr,
	&pmClockGpu,
	&pmClockSata,
	&pmClockPcie,
	&pmClockGdma,
	&pmClockDpDma,
	&pmClockTopSwMain,
	&pmClockTopSwLsBus,
	&pmClockGtgRef0,
	&pmClockDbgTstmp,
	&pmClockUsb3Dual,
	&pmClockGem0,
	&pmClockGem1,
	&pmClockGem2,
	&pmClockGem3,
	&pmClockUsb0Bus,
	&pmClockUsb1Bus,
	&pmClockQSpi,
	&pmClockSdio0,
	&pmClockSdio1,
	&pmClockUart0,
	&pmClockUart1,
	&pmClockSpi0,
	&pmClockSpi1,
	&pmClockCan0,
	&pmClockCan1,
	&pmClockCpuR5,
	&pmClockIouSwitch,
	&pmClockCsuPll,
	&pmClockPcap,
	&pmClockLpdSwitch,
	&pmClockLpdLsBus,
	&pmClockDbgLpd,
	&pmClockNand,
	&pmClockAdma,
	&pmClockPl0,
	&pmClockPl1,
	&pmClockPl2,
	&pmClockPl3,
	&pmClockGemTsu,
	&pmClockDll,
	&pmClockAms,
	&pmClockI2C0,
	&pmClockI2C1,
	&pmClockTimeStamp,
};

static PmClockHandle pmClockHandles[] = {
	{
		.clock = &pmClockAcpu,
		.node = &pmPowerIslandApu_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockDpVideo,
		.node = &pmSlaveDP_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockDpAudio,
		.node = &pmSlaveDP_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockDpStc,
		.node = &pmSlaveDP_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockDdr,
		.node = &pmSlaveDdr_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockGpu,
		.node = &pmSlaveGpu_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockSata,
		.node = &pmSlaveSata_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockPcie,
		.node = &pmSlavePcie_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockGdma,
		.node = &pmSlaveGdma_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockDpDma,
		.node = &pmSlaveDP_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockTopSwMain,
		.node = &pmSlaveDdr_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockTopSwLsBus,
		.node = &pmSlaveDdr_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockGem0,
		.node = &pmSlaveEth0_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockGem1,
		.node = &pmSlaveEth1_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockGem2,
		.node = &pmSlaveEth2_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockGem3,
		.node = &pmSlaveEth3_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockUsb3Dual,
		.node = &pmSlaveUsb0_g.slv.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockUsb3Dual,
		.node = &pmSlaveUsb1_g.slv.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockUsb0Bus,
		.node = &pmSlaveUsb0_g.slv.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockUsb1Bus,
		.node = &pmSlaveUsb1_g.slv.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockQSpi,
		.node = &pmSlaveQSpi_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockSdio0,
		.node = &pmSlaveSD0_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockSdio1,
		.node = &pmSlaveSD1_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockUart0,
		.node = &pmSlaveUart0_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockUart1,
		.node = &pmSlaveUart1_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockSpi0,
		.node = &pmSlaveSpi0_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockSpi1,
		.node = &pmSlaveSpi1_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockCan0,
		.node = &pmSlaveCan0_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockCan1,
		.node = &pmSlaveCan1_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockCpuR5,
		.node = &pmPowerIslandRpu_g.power.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockCsuPll,
		.node = &pmSlavePcap_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockPcap,
		.node = &pmSlavePcap_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockLpdLsBus,
		.node = &pmSlaveTtc0_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockLpdLsBus,
		.node = &pmSlaveTtc1_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockLpdLsBus,
		.node = &pmSlaveTtc2_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockLpdLsBus,
		.node = &pmSlaveTtc3_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockNand,
		.node = &pmSlaveNand_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockAdma,
		.node = &pmSlaveAdma_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockPl0,
		.node = &pmPowerDomainPld_g.power.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockPl1,
		.node = &pmPowerDomainPld_g.power.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockPl2,
		.node = &pmPowerDomainPld_g.power.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockPl3,
		.node = &pmPowerDomainPld_g.power.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockGemTsu,
		.node = &pmSlaveEth0_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockGemTsu,
		.node = &pmSlaveEth1_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockGemTsu,
		.node = &pmSlaveEth2_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockGemTsu,
		.node = &pmSlaveEth3_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockDll,
		.node = &pmSlaveSD0_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockDll,
		.node = &pmSlaveSD1_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockI2C0,
		.node = &pmSlaveI2C0_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	}, {
		.clock = &pmClockI2C1,
		.node = &pmSlaveI2C1_g.node,
		.nextClock = NULL,
		.nextNode = NULL,
	},
};

/**
 * PmClockGetParent() - Get PLL parent of the clock based on MUX select value
 * @clock	Pointer to the clock whose PLL parent shall be find
 * @muxSel	Multiplexer select value
 *
 * @return	Pointer to the PLL parent of the clock
 */
static PmPll* PmClockGetParent(PmClock* const clock, const u32 sel)
{
	u32 i;
	PmPll* parent = NULL;

	for (i = 0U; i < clock->mux->size; i++) {
		if (sel == clock->mux->inputs[i].select) {
			parent = clock->mux->inputs[i].pll;
			break;
		}
	}

	return parent;
}

/**
 * PmClockConstructList() - Link clock handles into clock's/node's lists
 */
void PmClockConstructList(void)
{
	u32 i;

	for (i = 0U; i < ARRAY_SIZE(pmClockHandles); i++) {
		PmClockHandle* ch = &pmClockHandles[i];

		/* Add the clock at the beginning of the node's clocks list */
		ch->nextClock = ch->node->clocks;
		ch->node->clocks = ch;

		/* Add the node at the beginning of the clock's users list */
		ch->nextNode = ch->clock->users;
		ch->clock->users = ch;
	}
}

/**
 * @PmClockIsActive() Check if any clock for a given node is active
 * @node	Node whose clocks need to be checked
 *
 * @return XST_SUCCESS if any one clock for given node is active
 *         XST_FAILURE if all clocks for given node are inactive
 */
int PmClockIsActive(PmNode* const node)
{
	PmClockHandle* ch = node->clocks;
	int status = XST_FAILURE;

        while (NULL != ch) {
		/* FIXME */
		ch = ch->nextClock;
	}

done:
	return status;
}

/**
 * @PmClockSave() - Save control register values for clocks used by the node
 * @node	Node whose clock control regs need to be saved
 */
void PmClockSave(PmNode* const node)
{
	PmClockHandle* ch = node->clocks;

	while (NULL != ch) {
		ch->clock->ctrlVal = XPfw_Read32(ch->clock->ctrlAddr);
		ch = ch->nextClock;
	}
}

/**
 * PmClockRestore() - Restore control register values for clocks of the node
 * @node	Node whose clock control registers need to be restored
 */
void PmClockRestore(PmNode* const node)
{
	PmClockHandle* ch = node->clocks;

	while (NULL != ch) {
		/* Restore the clock configuration if needed */
		if (0U != ch->clock->ctrlVal) {
			XPfw_Write32(ch->clock->ctrlAddr, ch->clock->ctrlVal);
		}
		ch = ch->nextClock;
	}
}

/**
 * PmClockRequest() - Request clocks used by the given node
 * @node	Node whose clocks need to be requested
 * @return	XST_SUCCESS if the request is processed correctly, or error code
 *		if a PLL parent needed to be locked and the locking has failed.
 * @note	The dependency toward a PLL parent is automatically resolved
 */
int PmClockRequest(PmNode* const node)
{
	PmClockHandle* ch = node->clocks;
	int status = XST_SUCCESS;

	if (0U != (NODE_LOCKED_CLOCK_FLAG & node->flags)) {
		PmWarn("%s double request\r\n", node->name);
		goto done;
	}
	while (NULL != ch) {
		const u32 val = XPfw_Read32(ch->clock->ctrlAddr);
		const u32 sel = val & PM_CLOCK_MUX_SELECT_MASK;

		ch->clock->pll = PmClockGetParent(ch->clock, sel);

		/* If parent is not a known pll it's the oscillator clock */
		if (NULL == ch->clock->pll) {
			ch = ch->nextClock;
			continue;
		}

		status = PmPllRequest(ch->clock->pll);
		if (XST_SUCCESS != status) {
			goto done;
		}

		ch = ch->nextClock;
	}
	node->flags |= NODE_LOCKED_CLOCK_FLAG;

done:
	return status;
}

/**
 * PmClockRelease() - Release clocks used by the given node
 * @node	Node whose clocks are released
 *
 * @note	If a PLL parent of a released clock have no other users, the
 *		PM framework will suspend that PLL.
 */
void PmClockRelease(PmNode* const node)
{
	PmClockHandle* ch = node->clocks;

	if (0U == (NODE_LOCKED_CLOCK_FLAG & node->flags)) {
		PmWarn("%s double release\r\n", node->name);
		goto done;
	}
	while (NULL != ch) {
		if (NULL != ch->clock->pll) {
			PmPllRelease(ch->clock->pll);
		}
		ch = ch->nextClock;
	}
	node->flags &= ~NODE_LOCKED_CLOCK_FLAG;

done:
	return;
}

#endif
