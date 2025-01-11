/*
* Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
#ifndef _PCIE_HOST
#define _PCIE_HOST

#include "pcie/r_pcie_ctrl.h"

/*------------------------- PCIe common definition and structure ------*/
/**
 * @typedef pcie_id_t
 * @brief A unique PCI(e) identifier (vendor ID, device ID).
 *
 * The PCIE_CONF_ID register for each endpoint is a (vendor ID, device ID)
 * pair, which is meant to tell the system what the PCI(e) endpoint is. Again,
 * look to PCIE_ID_* macros in include/dt-bindings/pcie/pcie.h for more.
 */
typedef uint32_t pcie_id_t;

/*------------------------------- PUBLIC APIs -------------------------*/

/** Callback type used for scanning for PCI endpoints
 *
 * @param[in] bdf      BDF value for a found endpoint.
 * @param[in] id       Vendor & Device ID for the found endpoint.
 * @param[inout] cb_data  Custom, use case specific data.
 *
 * @return true to continue scanning, false to stop scanning.
 */
typedef bool (*pcie_scan_cb_t)(pcie_bdf_t bdf, pcie_id_t id, void *cb_data);

enum {
    /** Scan all available PCI host controllers and sub-busses */
    PCIE_SCAN_RECURSIVE = 0x00000001,     // Bit 0
    /** Do the callback for all endpoint types, including bridges */
    PCIE_SCAN_CB_ALL = 0x00000002,         // Bit 1
};

// /** API mapping functions for Host */
// struct st_pcie_host_api
// {
//     /* Functions for Host */
//     bool     (*region_allocate)(struct st_pcie_host *host, pcie_bdf_t bdf, bool mem, bool mem64, size_t bar_size, uint32_t *bar_bus_addr);
//     bool     (*region_get_allocate_base)(struct st_pcie_host *host, pcie_bdf_t bdf, bool mem, bool mem64, size_t align, uint32_t *bar_base_addr);
//     bool     (*region_translate)(struct st_pcie_host *host, pcie_bdf_t bdf, bool mem, bool mem64, size_t bar_size, uint32_t *bar_addr);
//     int32_t  (*scan)(struct st_pcie_host *host, const struct st_pcie_scan_opt *opt);
//     bool     (*get_mbar)(struct st_pcie_host *host, pcie_bdf_t bdf, unsigned int bar_index, struct pcie_bar *mbar);
//     bool     (*probe_mbar)(struct st_pcie_host *host, pcie_bdf_t bdf, unsigned int index, struct pcie_bar *mbar);
//     bool     (*get_iobar)(struct st_pcie_host *host, pcie_bdf_t bdf, unsigned int bar_index, struct pcie_bar *iobar);
//     bool     (*probe_iobar)(struct st_pcie_host *host, pcie_bdf_t bdf, unsigned int index, struct pcie_bar *iobar);
//     void     (*set_cmd)(struct st_pcie_host *host, pcie_bdf_t bdf, uint32_t bits, bool on);
//     uint32_t (*alloc_irq)(struct st_pcie_host *host, pcie_bdf_t bdf);
//     uint32_t (*get_irq)(struct st_pcie_host *host, pcie_bdf_t bdf);
//     void     (*enable_irq)(struct st_pcie_host *host, pcie_bdf_t bdf, unsigned int irq);
//     uint32_t (*get_cap)(struct st_pcie_host *host, pcie_bdf_t bdf, uint32_t cap_id);
//     uint32_t (*get_ext_cap)(struct st_pcie_host *host, pcie_bdf_t bdf, uint32_t cap_id);
//     bool     (*connect_dynamic_irq)(struct st_pcie_host *host, 
//                             pcie_bdf_t bdf,
//                             unsigned int irq,
//                             unsigned int priority,
//                             void (*routine)(const void *parameter),
//                             const void *parameter,
//                             uint32_t flags);
// };

/**
 * @brief Struct to describe the PCIe Host device
 */
struct st_pcie_host
{
    struct st_pcie_ctrl ctrl; // The controller that control this host
    pcie_irq_t irq[];         // List of interrupt source and its callback function
};


/** Options for performing a scan for PCI devices */
struct st_pcie_scan_opt {
    /** Initial bus number to scan */
    uint8_t bus;
    /** Function to call for each found endpoint */
    pcie_scan_cb_t cb;
    /** Custom data to pass to the scan callback */
    void *cb_data;
    /** Scan flags */
    uint32_t flags;
};

/**
 * @brief Init a PCIe device channel to a Host Device
 * 
 */
void R_PCIE_InitHost(struct st_pcie_host *host, uint16_t channel);

/** Scan for PCIe devices.
 *
 * Scan the PCI bus (or buses) for available endpoints.
 *
 * @param[in] ctrl PCI Express Controller device pointer
 * @param opt Options determining how to perform the scan.
 * @return 0 on success, negative POSIX error number on failure.
 */
int R_PCIE_Scan(struct st_pcie_host *host, const struct st_pcie_scan_opt *opt);

/**
 * @brief Get the MBAR at a specific BAR index
 * @param[in] ctrl PCI Express Controller device pointer
 * @param bdf the PCI(e) endpoint
 * @param bar_index 0-based BAR index
 * @param mbar Pointer to struct pcie_bar
 * @return true if the mbar was found and is valid, false otherwise
 */
bool R_PCIE_GetMBAR(struct st_pcie_host *host, pcie_bdf_t bdf, unsigned int bar_index, struct pcie_bar *mbar);

/**
 * @brief Probe the nth MMIO address assigned to an endpoint.
 * @param[in] ctrl PCI Express Controller device pointer
 * @param bdf the PCI(e) endpoint
 * @param index (0-based) index
 * @param mbar Pointer to struct pcie_bar
 * @return true if the mbar was found and is valid, false otherwise
 *
 * A PCI(e) endpoint has 0 or more memory-mapped regions. This function
 * allows the caller to enumerate them by calling with index=0..n.
 * Value of n has to be below 6, as there is a maximum of 6 BARs. The indices
 * are order-preserving with respect to the endpoint BARs: e.g., index 0
 * will return the lowest-numbered memory BAR on the endpoint.
 */
bool R_PCIE_ProbeMBAR(struct st_pcie_host *host, pcie_bdf_t bdf, unsigned int index, struct pcie_bar *mbar);

/**
 * @brief Get the I/O BAR at a specific BAR index
 * @param[in] ctrl PCI Express Controller device pointer
 * @param bdf the PCI(e) endpoint
 * @param bar_index 0-based BAR index
 * @param iobar Pointer to struct pcie_bar
 * @return true if the I/O BAR was found and is valid, false otherwise
 */
bool R_PCIE_GetIOBAR(struct st_pcie_host *host, pcie_bdf_t bdf, unsigned int bar_index, struct pcie_bar *iobar);

/**
 * @brief Probe the nth I/O BAR address assigned to an endpoint.
 * @param[in] ctrl PCI Express Controller device pointer
 * @param bdf the PCI(e) endpoint
 * @param index (0-based) index
 * @param iobar Pointer to struct pcie_bar
 * @return true if the I/O BAR was found and is valid, false otherwise
 *
 * A PCI(e) endpoint has 0 or more I/O regions. This function
 * allows the caller to enumerate them by calling with index=0..n.
 * Value of n has to be below 6, as there is a maximum of 6 BARs. The indices
 * are order-preserving with respect to the endpoint BARs: e.g., index 0
 * will return the lowest-numbered I/O BAR on the endpoint.
 */
bool R_PCIE_ProbeIOBAR(struct st_pcie_host *host, pcie_bdf_t bdf, unsigned int index, struct pcie_bar *iobar);

/**
 * @brief Set or reset bits in the endpoint command/status register.
 *
 * @param[in] ctrl PCI Express Controller device pointer
 * @param bdf the PCI(e) endpoint
 * @param bits the powerset of bits of interest
 * @param on use true to set bits, false to reset them
 */
void R_PCIE_SetCmd(struct st_pcie_host *host, pcie_bdf_t bdf, uint32_t bits, bool on);

/**
 * @brief Allocate an IRQ for an endpoint.
 *
 * This function first checks the IRQ register and if it contains a valid
 * value this is returned. If the register does not contain a valid value
 * allocation of a new one is attempted.
 * Such function is only exposed if CONFIG_PCIE_CONTROLLER is unset.
 * It is thus available where architecture tied dynamic IRQ allocation for
 * PCIe device makes sense.
 *
 * @param[in] ctrl PCI Express Controller device pointer
 * @param bdf the PCI(e) endpoint
 * @return the IRQ number, or PCIE_CONF_INTR_IRQ_NONE if allocation failed.
 */
unsigned int R_PCIE_AllocIRQ(struct st_pcie_host *host, pcie_bdf_t bdf);

/**
 * @brief Return the IRQ assigned by the firmware/board to an endpoint.
 *
 * @param[in] ctrl PCI Express Controller device pointer
 * @param bdf the PCI(e) endpoint
 * @return the IRQ number, or PCIE_CONF_INTR_IRQ_NONE if unknown.
 */
unsigned int R_PCIE_GetIRQ(struct st_pcie_host *host, pcie_bdf_t bdf);

/**
 * @brief Enable the PCI(e) endpoint to generate the specified IRQ.
 *
 * @param bdf the PCI(e) endpoint
 * @param irq the IRQ to generate
 *
 * If MSI is enabled and the endpoint supports it, the endpoint will
 * be configured to generate the specified IRQ via MSI. Otherwise, it
 * is assumed that the IRQ has been routed by the boot firmware
 * to the specified IRQ, and the IRQ is enabled (at the I/O APIC, or
 * wherever appropriate).
 */
void R_PCIE_EnableIRQ(struct st_pcie_host *host, pcie_bdf_t bdf, unsigned int irq);

/**
 * @brief Find a PCI(e) capability in an endpoint's configuration space.
 *
 * @param bdf the PCI endpoint to examine
 * @param cap_id the capability ID of interest
 * @return the index of the configuration word, or 0 if no capability.
 */
uint32_t R_PCIE_GetCap(struct st_pcie_host *host, pcie_bdf_t bdf, uint32_t cap_id);

/**
 * @brief Find an Extended PCI(e) capability in an endpoint's configuration space.
 *
 * @param bdf the PCI endpoint to examine
 * @param cap_id the capability ID of interest
 * @return the index of the configuration word, or 0 if no capability.
 */
uint32_t R_PCIE_GetExtCap(struct st_pcie_host *host, pcie_bdf_t bdf, uint32_t cap_id);

/**
 * @brief Dynamically connect a PCIe endpoint IRQ to an ISR handler
 *
 * @param bdf the PCI endpoint to examine
 * @param irq the IRQ to connect (see pcie_alloc_irq())
 * @param priority priority of the IRQ
 * @param routine the ISR handler to connect to the IRQ
 * @param parameter the parameter to provide to the handler
 * @param flags IRQ connection flags
 * @return true if connected, false otherwise
 */
bool R_PCIE_ConnectDynamicIRQ(struct st_pcie_host *host, 
                            pcie_bdf_t bdf,
                            unsigned int irq,
                            unsigned int priority,
                            void (*routine)(const void *parameter),
                            const void *parameter,
                            uint32_t flags);

/**
 * @brief Function called to allocate a memory region subset for an endpoint Base Address Register.
 *
 * When enumerating PCIe Endpoints, Type0 endpoints can require up to 6 memory zones
 * via the Base Address Registers from I/O or Memory types.
 *
 * This call allocates such zone in the PCI Express Controller memory regions if
 * such region is available and space is still available.
 * 
 * @param[in] ctrl PCI Express Controller device pointer
 * @param[in] bdf PCI(e) endpoint
 * @param[in] mem True if the BAR is of memory type
 * @param[in] mem64 True if the BAR is of 64bit memory type
 * @param[in] bar_size Size in bytes of the Base Address Register as returned by HW
 * @param[in] bar_bus_addr bus-centric address allocated to be written in the BAR register
 */
void R_PCIE_RegionAllocate(struct st_pcie_host *host, pcie_bdf_t bdf, bool mem, bool mem64, size_t bar_size, uint32_t *bar_bus_addr);

/**
 * @brief Function called to get the current allocation base of a memory region subset
 * for an endpoint Base Address Register.
 *
 * When enumerating PCIe Endpoints, Type1 bridge endpoints requires a range of memory
 * allocated by all endpoints in the bridged bus.
 *
 * @param[in] ctrl PCI Express Controller device pointer
 * @param[in] bdf PCI(e) endpoint
 * @param[in] mem True if the BAR is of memory type
 * @param[in] mem64 True if the BAR is of 64bit memory type
 * @param[in] align size to take in account for alignment
 * @param[out] bar_base_addr bus-centric address allocation base
 * @return True if allocation was possible, False if allocation failed
 */
bool R_PCIE_RegionGetAllocateBase(struct st_pcie_host *host, pcie_bdf_t bdf, bool mem, bool mem64, size_t align, uint32_t *bar_base_addr);

/**
 * @brief Function called to translate an endpoint Base Address Register bus-centric address
 * into Physical address.
 *
 * When enumerating PCIe Endpoints, Type0 endpoints can require up to 6 memory zones
 * via the Base Address Registers from I/O or Memory types.
 *
 * The bus-centric address set in this BAR register is not necessarily accessible from the CPU,
 * thus must be translated by using the PCI Express Controller memory regions translation
 * ranges to permit mapping from the CPU.
 *
 * @param[in] ctrl PCI Express Controller device pointer
 * @param[in] bdf PCI(e) endpoint
 * @param[in] mem True if the BAR is of memory type
 * @param[in] mem64 True if the BAR is of 64bit memory type
 * @param[in] bar_bus_addr bus-centric address written in the BAR register
 * @param[out] bar_addr CPU-centric address translated from the bus-centric address
 * @return True if translation was possible, False if translation failed
 */
bool R_PCIE_RegionTranslate(struct st_pcie_host *host, pcie_bdf_t bdf, bool mem, bool mem64, size_t bar_size, uint32_t *bar_addr);

/*---------------------------------------- Configuration bits -----------------------------------------*/

/*
 * Configuration word 13 contains the head of the capabilities list.
 */

#define PCIE_CONF_CAPPTR    13U    /* capabilities pointer */
#define PCIE_CONF_CAPPTR_FIRST(w)    (((w) >> 2) & 0x3FU)

/*
 * The first word of every capability contains a capability identifier,
 * and a link to the next capability (or 0) in configuration space.
 */

#define PCIE_CONF_CAP_ID(w)        ((w) & 0xFFU)
#define PCIE_CONF_CAP_NEXT(w)        (((w) >> 10) & 0x3FU)

/*
 * The extended PCI Express capabilities lie at the end of the PCI configuration space
 */

#define PCIE_CONF_EXT_CAPPTR    64U

/*
 * The first word of every capability contains an extended capability identifier,
 * and a link to the next capability (or 0) in the extended configuration space.
 */

#define PCIE_CONF_EXT_CAP_ID(w)        ((w) & 0xFFFFU)
#define PCIE_CONF_EXT_CAP_VER(w)    (((w) >> 16) & 0xFU)
#define PCIE_CONF_EXT_CAP_NEXT(w)    (((w) >> 20) & 0xFFFU)

/*
 * Configuration word 0 aligns directly with pcie_id_t.
 */

#define PCIE_CONF_ID        0U

/*
 * Configuration word 1 contains command and status bits.
 */

#define PCIE_CONF_CMDSTAT    1U    /* command/status register */

#define PCIE_CONF_CMDSTAT_IO        0x00000001U  /* I/O access enable */
#define PCIE_CONF_CMDSTAT_MEM        0x00000002U  /* mem access enable */
#define PCIE_CONF_CMDSTAT_MASTER    0x00000004U  /* bus master enable */
#define PCIE_CONF_CMDSTAT_INTERRUPT    0x00080000U  /* interrupt status */
#define PCIE_CONF_CMDSTAT_CAPS        0x00100000U  /* capabilities list */

/*
 * Configuration word 2 has additional function identification that
 * we only care about for debug output (PCIe shell commands).
 */

#define PCIE_CONF_CLASSREV    2U    /* class/revision register */

#define PCIE_CONF_CLASSREV_CLASS(w)    (((w) >> 24) & 0xFFU)
#define PCIE_CONF_CLASSREV_SUBCLASS(w)  (((w) >> 16) & 0xFFU)
#define PCIE_CONF_CLASSREV_PROGIF(w)    (((w) >> 8) & 0xFFU)
#define PCIE_CONF_CLASSREV_REV(w)    ((w) & 0xFFU)

/*
 * The only part of configuration word 3 that is of interest to us is
 * the header type, as we use it to distinguish functional endpoints
 * from bridges (which are, for our purposes, transparent).
 */

#define PCIE_CONF_TYPE        3U

#define PCIE_CONF_MULTIFUNCTION(w)    (((w) & 0x00800000U) != 0U)
#define PCIE_CONF_TYPE_BRIDGE(w)    (((w) & 0x007F0000U) != 0U)
#define PCIE_CONF_TYPE_GET(w)        (((w) >> 16) & 0x7F)

#define PCIE_CONF_TYPE_STANDARD         0x0U
#define PCIE_CONF_TYPE_PCI_BRIDGE       0x1U
#define PCIE_CONF_TYPE_CARDBUS_BRIDGE   0x2U

/*
 * Words 4-9 are BARs are I/O or memory decoders. Memory decoders may
 * be 64-bit decoders, in which case the next configuration word holds
 * the high-order bits (and is, thus, not a BAR itself).
 */

#define PCIE_CONF_BAR0        4U
#define PCIE_CONF_BAR1        5U
#define PCIE_CONF_BAR2        6U
#define PCIE_CONF_BAR3        7U
#define PCIE_CONF_BAR4        8U
#define PCIE_CONF_BAR5        9U

#define PCIE_CONF_BAR_IO(w)        (((w) & 0x00000001U) == 0x00000001U)
#define PCIE_CONF_BAR_MEM(w)        (((w) & 0x00000001U) != 0x00000001U)
#define PCIE_CONF_BAR_64(w)        (((w) & 0x00000006U) == 0x00000004U)
#define PCIE_CONF_BAR_ADDR(w)        ((w) & ~0xfUL)
#define PCIE_CONF_BAR_IO_ADDR(w)    ((w) & ~0x3UL)
#define PCIE_CONF_BAR_FLAGS(w)        ((w) & 0xfUL)
#define PCIE_CONF_BAR_NONE        0U

#define PCIE_CONF_BAR_INVAL        0xFFFFFFF0U
#define PCIE_CONF_BAR_INVAL64        0xFFFFFFFFFFFFFFF0UL

#define PCIE_CONF_BAR_INVAL_FLAGS(w)            \
    ((((w) & 0x00000006U) == 0x00000006U) ||    \
     (((w) & 0x00000006U) == 0x00000002U))

/*
 * Type 1 Header has files related to bus management
 */
#define PCIE_BUS_NUMBER         6U

#define PCIE_BUS_PRIMARY_NUMBER(w)      ((w) & 0xffUL)
#define PCIE_BUS_SECONDARY_NUMBER(w)    (((w) >> 8) & 0xffUL)
#define PCIE_BUS_SUBORDINATE_NUMBER(w)  (((w) >> 16) & 0xffUL)
#define PCIE_SECONDARY_LATENCY_TIMER(w) (((w) >> 24) & 0xffUL)

#define PCIE_BUS_NUMBER_VAL(prim, sec, sub, lat) \
    (((prim) & 0xffUL) |             \
     (((sec) & 0xffUL) << 8) |         \
     (((sub) & 0xffUL) << 16) |         \
     (((lat) & 0xffUL) << 24))

/*
 * Type 1 words 7 to 12 setups Bridge Memory base and limits
 */
#define PCIE_IO_SEC_STATUS      7U

#define PCIE_IO_BASE(w)         ((w) & 0xffUL)
#define PCIE_IO_LIMIT(w)        (((w) >> 8) & 0xffUL)
#define PCIE_SEC_STATUS(w)      (((w) >> 16) & 0xffffUL)

#define PCIE_IO_SEC_STATUS_VAL(iob, iol, sec_status) \
    (((iob) & 0xffUL) |                 \
     (((iol) & 0xffUL) << 8) |             \
     (((sec_status) & 0xffffUL) << 16))

#define PCIE_MEM_BASE_LIMIT     8U

#define PCIE_MEM_BASE(w)        ((w) & 0xffffUL)
#define PCIE_MEM_LIMIT(w)       (((w) >> 16) & 0xffffUL)

#define PCIE_MEM_BASE_LIMIT_VAL(memb, meml) \
    (((memb) & 0xffffUL) |            \
     (((meml) & 0xffffUL) << 16))

#define PCIE_PREFETCH_BASE_LIMIT        9U

#define PCIE_PREFETCH_BASE(w)   ((w) & 0xffffUL)
#define PCIE_PREFETCH_LIMIT(w)  (((w) >> 16) & 0xffffUL)

#define PCIE_PREFETCH_BASE_LIMIT_VAL(pmemb, pmeml) \
    (((pmemb) & 0xffffUL) |               \
     (((pmeml) & 0xffffUL) << 16))

#define PCIE_PREFETCH_BASE_UPPER        10U

#define PCIE_PREFETCH_LIMIT_UPPER       11U

#define PCIE_IO_BASE_LIMIT_UPPER        12U

#define PCIE_IO_BASE_UPPER(w)   ((w) & 0xffffUL)
#define PCIE_IO_LIMIT_UPPER(w)  (((w) >> 16) & 0xffffUL)

#define PCIE_IO_BASE_LIMIT_UPPER_VAL(iobu, iolu) \
    (((iobu) & 0xffffUL) |             \
     (((iolu) & 0xffffUL) << 16))

/*
 * Word 15 contains information related to interrupts.
 *
 * We're only interested in the low byte, which is [supposed to be] set by
 * the firmware to indicate which wire IRQ the device interrupt is routed to.
 */

#define PCIE_CONF_INTR        15U

#define PCIE_CONF_INTR_IRQ(w)    ((w) & 0xFFU)
#define PCIE_CONF_INTR_IRQ_NONE    0xFFU  /* no interrupt routed */

#define PCIE_MAX_BUS  (0xFFFFFFFFU & PCIE_BDF_BUS_MASK)
#define PCIE_MAX_DEV  (0xFFFFFFFFU & PCIE_BDF_DEV_MASK)
#define PCIE_MAX_FUNC (0xFFFFFFFFU & PCIE_BDF_FUNC_MASK)

#endif
