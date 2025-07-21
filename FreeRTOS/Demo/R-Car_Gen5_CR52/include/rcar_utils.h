#ifndef __RCAR_UTILS_H__
#define __RCAR_UTILS_H__

/**
 * @defgroup UTILS_Module RCAR UTILS Module
 * @{
 * @brief This module provides common RCAR UTILS function. 
 */

 #include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdint.h>

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** 
 * @brief Enumeration for types of memory regions. 
 */
typedef enum e_memory_type {
    PERIPHERAL,
    CMA,
    OSAL,
    SHARE_MEM 
} e_memory_type_t;

/**
 * @brief Structure to represent a memory region.
 * 
 * This structure contains the base address and size of a memory region.
 */
typedef struct st_memory {
    uint32_t base_address;
    uint32_t size;
} st_memory_t;

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/
/**
 * @brief Get memory region info.
 *
 * Returns the base address and size of a specified memory region.
 *
 * @param[in] type Memory type
 * @param[in] region_idx Region index (e.g., 0 for the first region)
 *
 * @return memory_t Struct containing base address and size
 */
st_memory_t R_UTILS_GetMemoryRegionInfo(e_memory_type_t type, uint8_t region_idx);

/**
 * @brief Get total regions of a specific memory type.
 * 
 * Return the total number of regions for a given memory type.
 * 
 * @param[in] type Memory type
 * 
 * @return uint8_t Total number of memory regions for the given type
 */
uint8_t R_UTILS_GetTotalRegionOfMemory(e_memory_type_t type);


/**
 * @brief Get counter of timer.
 *
 * Return the counter of timer.
 *
 *
 * @return uint64_t Timer counter.
 */
uint64_t R_UTILS_GetTimerCounter(void);

/**
 * @brief Get frequency of timer.
 *
 * Return the counter of timer by hz.
 *
 *
 * @return uint64_t Timer frequency.
 */
uint32_t R_UTILS_GetTimerFrequency(void);

void * aligned_malloc(size_t align, size_t size);
void aligned_free(void * ptr);


/**
 * @brief Invalidate Data cache.
 *
 * Notify CPU to invalidate data cache before reading.
 *
 * @param[in] addr Physical start address.
 *
 * @param[in] size Buffer size in byte.
 *
 */
void R_UTILS_InvalidateDCache(uint32_t addr, uint32_t size);

/**
 * @brief Invalidate All Data cache.
 *
 * Notify CPU to invalidate data cache before reading.
 *
 */
void R_UTILS_InvalidateDCacheAll();

/**
 * @brief Invalidate cache and read memory for DMA.
 * 
 * Invalidates cache for the given address range and reads memory to ensure
 * fresh data after DMA transfer.
 * 
 * @param[in] addr Address to read.
 * @param[in] size Size of memory range to invalidate.
 * 
 * @return uint32_t Value read from memory.
 */
uint32_t R_UTILS_ReadMemForDMA(void *addr, uint32_t size);

/**-----------------------------------------------------------**/

#ifdef __cplusplus
}
#endif

/** @} */ // end of UTILS_Module

#endif // __RCAR_UTILS_H__
