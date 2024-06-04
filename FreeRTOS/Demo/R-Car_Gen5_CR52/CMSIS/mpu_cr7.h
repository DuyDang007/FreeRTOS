/******************************************************************************
 * @file     mpu_armv7.h
 * @brief    CMSIS MPU API for Armv7-M MPU
 * @version  V5.0.5
 * @date     06. September 2018
 ******************************************************************************/
/*
 * Copyright (c) 2017-2018 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
/*
 * Modified for Cortex-R for MPU code
 */


#if   defined ( __ICCARM__ )
  #pragma system_include         /* treat file as system include file for MISRA check */
#elif defined (__clang__)
  #pragma clang system_header    /* treat file as system include file */
#endif
 
#ifndef ARM_MPU_ARMV7_H
#define ARM_MPU_ARMV7_H

#define ARM_MPU_REGION_SIZE_256B     ((uint8_t)0x07U) ///!< MPU Region Size 256 Bytes
#define ARM_MPU_REGION_SIZE_512B     ((uint8_t)0x08U) ///!< MPU Region Size 512 Bytes
#define ARM_MPU_REGION_SIZE_1KB      ((uint8_t)0x09U) ///!< MPU Region Size 1 KByte
#define ARM_MPU_REGION_SIZE_2KB      ((uint8_t)0x0AU) ///!< MPU Region Size 2 KBytes
#define ARM_MPU_REGION_SIZE_4KB      ((uint8_t)0x0BU) ///!< MPU Region Size 4 KBytes
#define ARM_MPU_REGION_SIZE_8KB      ((uint8_t)0x0CU) ///!< MPU Region Size 8 KBytes
#define ARM_MPU_REGION_SIZE_16KB     ((uint8_t)0x0DU) ///!< MPU Region Size 16 KBytes
#define ARM_MPU_REGION_SIZE_32KB     ((uint8_t)0x0EU) ///!< MPU Region Size 32 KBytes
#define ARM_MPU_REGION_SIZE_64KB     ((uint8_t)0x0FU) ///!< MPU Region Size 64 KBytes
#define ARM_MPU_REGION_SIZE_128KB    ((uint8_t)0x10U) ///!< MPU Region Size 128 KBytes
#define ARM_MPU_REGION_SIZE_256KB    ((uint8_t)0x11U) ///!< MPU Region Size 256 KBytes
#define ARM_MPU_REGION_SIZE_512KB    ((uint8_t)0x12U) ///!< MPU Region Size 512 KBytes
#define ARM_MPU_REGION_SIZE_1MB      ((uint8_t)0x13U) ///!< MPU Region Size 1 MByte
#define ARM_MPU_REGION_SIZE_2MB      ((uint8_t)0x14U) ///!< MPU Region Size 2 MBytes
#define ARM_MPU_REGION_SIZE_4MB      ((uint8_t)0x15U) ///!< MPU Region Size 4 MBytes
#define ARM_MPU_REGION_SIZE_8MB      ((uint8_t)0x16U) ///!< MPU Region Size 8 MBytes
#define ARM_MPU_REGION_SIZE_16MB     ((uint8_t)0x17U) ///!< MPU Region Size 16 MBytes
#define ARM_MPU_REGION_SIZE_32MB     ((uint8_t)0x18U) ///!< MPU Region Size 32 MBytes
#define ARM_MPU_REGION_SIZE_64MB     ((uint8_t)0x19U) ///!< MPU Region Size 64 MBytes
#define ARM_MPU_REGION_SIZE_128MB    ((uint8_t)0x1AU) ///!< MPU Region Size 128 MBytes
#define ARM_MPU_REGION_SIZE_256MB    ((uint8_t)0x1BU) ///!< MPU Region Size 256 MBytes
#define ARM_MPU_REGION_SIZE_512MB    ((uint8_t)0x1CU) ///!< MPU Region Size 512 MBytes
#define ARM_MPU_REGION_SIZE_1GB      ((uint8_t)0x1DU) ///!< MPU Region Size 1 GByte
#define ARM_MPU_REGION_SIZE_2GB      ((uint8_t)0x1EU) ///!< MPU Region Size 2 GBytes
#define ARM_MPU_REGION_SIZE_4GB      ((uint8_t)0x1FU) ///!< MPU Region Size 4 GBytes

#define ARM_MPU_AP_NONE 0U ///!< MPU Access Permission no access
#define ARM_MPU_AP_PRIV 1U ///!< MPU Access Permission privileged access only
#define ARM_MPU_AP_URO  2U ///!< MPU Access Permission unprivileged access read-only
#define ARM_MPU_AP_FULL 3U ///!< MPU Access Permission full access
#define ARM_MPU_AP_PRO  5U ///!< MPU Access Permission privileged access read-only
#define ARM_MPU_AP_RO   6U ///!< MPU Access Permission read-only access

#define ARM_MPU_REGION(Region) \
  ((Region) & MPU_RBAR_REGION_Msk)

/**
* MPU Region Size Value
* 
* \param SubRegionDisable  Sub-region disable field.
* \param Size              Region size of the region to be configured, for example 4K, 8K.
*/
#define ARM_MPU_SIZE(SubRegionDisable, Size)    \
  ((((SubRegionDisable) << MPU_RASR_SRD_Pos)  & MPU_RASR_SRD_Msk)                                 | \
   (((Size)             << MPU_RASR_SIZE_Pos) & MPU_RASR_SIZE_Msk)                                | \
   (((MPU_RASR_ENABLE_Msk))))

/**
* MPU Memory Access Attributes
* 
* \param TypeExtField      Type extension field, allows you to configure memory access type, for example strongly ordered, peripheral.
* \param IsShareable       Region is shareable between multiple bus masters.
* \param IsCacheable       Region is cacheable, i.e. its value may be kept in cache.
* \param IsBufferable      Region is bufferable, i.e. using write-back caching. Cacheable but non-bufferable regions use write-through policy.
*/  
#define ARM_MPU_ACCESS_(TypeExtField, IsShareable, IsCacheable, IsBufferable)   \
  ((((TypeExtField) << MPU_RASR_TEX_Pos) & MPU_RASR_TEX_Msk)                  | \
   (((IsShareable)  << MPU_RASR_S_Pos)   & MPU_RASR_S_Msk)                    | \
   (((IsCacheable)  << MPU_RASR_C_Pos)   & MPU_RASR_C_Msk)                    | \
   (((IsBufferable) << MPU_RASR_B_Pos)   & MPU_RASR_B_Msk))

/**
* MPU Region Attribute Value
* 
* \param DisableExec       Instruction access disable bit, 1= disable instruction fetches.
* \param AccessPermission  Data access permissions, allows you to configure read/write access for User and Privileged mode.
* \param AccessAttributes  Memory access attribution, see \ref ARM_MPU_ACCESS_.
*/
#define ARM_MPU_ATTRIB_EX(DisableExec, AccessPermission, AccessAttributes)    \
  ((((DisableExec)      << MPU_RASR_XN_Pos)   & MPU_RASR_XN_Msk)                                  | \
   (((AccessPermission) << MPU_RASR_AP_Pos)   & MPU_RASR_AP_Msk)                                  | \
   (((AccessAttributes) & (MPU_RASR_TEX_Msk | MPU_RASR_S_Msk | MPU_RASR_C_Msk | MPU_RASR_B_Msk))))

/**
* MPU Region Attribute Value
* 
* \param DisableExec       Instruction access disable bit, 1= disable instruction fetches.
* \param AccessPermission  Data access permissions, allows you to configure read/write access for User and Privileged mode.
* \param TypeExtField      Type extension field, allows you to configure memory access type, for example strongly ordered, peripheral.
* \param IsShareable       Region is shareable between multiple bus masters.
* \param IsCacheable       Region is cacheable, i.e. its value may be kept in cache.
* \param IsBufferable      Region is bufferable, i.e. using write-back caching. Cacheable but non-bufferable regions use write-through policy.
*/                         
#define ARM_MPU_ATTRIB(DisableExec, AccessPermission, TypeExtField, IsShareable, IsCacheable, IsBufferable) \
  ARM_MPU_ATTRIB_EX(DisableExec, AccessPermission, ARM_MPU_ACCESS_(TypeExtField, IsShareable, IsCacheable, IsBufferable))

/**
* MPU Memory Access Attribute for strongly ordered memory.
*  - TEX: 000b
*  - Shareable
*  - Non-cacheable
*  - Non-bufferable
*/ 
#define ARM_MPU_ACCESS_ORDERED ARM_MPU_ACCESS_(0U, 1U, 0U, 0U)

/**
* MPU Memory Access Attribute for device memory.
*  - TEX: 000b (if non-shareable) or 010b (if shareable)
*  - Shareable or non-shareable
*  - Non-cacheable
*  - Bufferable (if shareable) or non-bufferable (if non-shareable)
*
* \param IsShareable Configures the device memory as shareable or non-shareable.
*/ 
#define ARM_MPU_ACCESS_DEVICE(IsShareable) ((IsShareable) ? ARM_MPU_ACCESS_(0U, 1U, 0U, 1U) : ARM_MPU_ACCESS_(2U, 0U, 0U, 0U))

/**
* MPU Memory Access Attribute for normal memory.
*  - TEX: 1BBb (reflecting outer cacheability rules)
*  - Shareable or non-shareable
*  - Cacheable or non-cacheable (reflecting inner cacheability rules)
*  - Bufferable or non-bufferable (reflecting inner cacheability rules)
*
* \param OuterCp Configures the outer cache policy.
* \param InnerCp Configures the inner cache policy.
* \param IsShareable Configures the memory as shareable or non-shareable.
*/ 
#define ARM_MPU_ACCESS_NORMAL(OuterCp, InnerCp, IsShareable) ARM_MPU_ACCESS_((4U | (OuterCp)), IsShareable, ((InnerCp) & 2U), ((InnerCp) & 1U))

/**
* MPU Memory Access Attribute non-cacheable policy.
*/
#define ARM_MPU_CACHEP_NOCACHE 0U

/**
* MPU Memory Access Attribute write-back, write and read allocate policy.
*/
#define ARM_MPU_CACHEP_WB_WRA 1U

/**
* MPU Memory Access Attribute write-through, no write allocate policy.
*/
#define ARM_MPU_CACHEP_WT_NWA 2U

/**
* MPU Memory Access Attribute write-back, no write allocate policy.
*/
#define ARM_MPU_CACHEP_WB_NWA 3U


/** Enable the MPU.
* \param MPU_Control Default access permissions for unconfigured regions.
*/
__STATIC_INLINE void ARM_MPU_Enable(void)
{
	uint32_t tmp;

	__get_CP(15, 0, tmp, 1, 0, 0);
	tmp |= SCTLR_M_Msk;
	__DSB();
	__set_CP(15, 0, tmp, 1, 0, 0);
	__ISB();
}

/** Clear and disable the given MPU region.
* \param rnr Region number to be cleared.
*/
__STATIC_INLINE void ARM_MPU_ClrRegion(uint32_t rnr)
{
	uint32_t zero = 0;

	__set_CP(15, 0, rnr, 6, 2, 0);
	__ISB();
	__set_CP(15, 0, zero, 6, 1, 2);
	__ISB();
}

/** Disable the MPU.
*/
__STATIC_INLINE void ARM_MPU_Disable(void)
{
	uint32_t tmp;

	__DSB();
	__ISB();
	__get_CP(15, 0, tmp, 1, 0, 0);
	tmp &= ~SCTLR_M_Msk;
	__DSB();
	__set_CP(15, 0, tmp, 1, 0, 0);
	__ISB();
}

/** Configure an MPU region.
* \param rnr Region number to be configured.
*/   
/* Note: The region base address must always align to the region size */
__STATIC_INLINE void ARM_MPU_SetRegionCR(uint32_t rnr, uint32_t addr, uint32_t attrib, uint32_t size)
{
	uint32_t val;

	/* RGNR (MPU Memory Region Number Register) */
	val = ARM_MPU_REGION(rnr);
	__set_CP(15, 0, val, 6, 2, 0);
	__ISB();

	/* DRBAR (Data Region Base Address Register) */
	__set_CP(15, 0, addr, 6, 1, 0);
	__ISB();

	/* DRACR (Data Region Access Control Register) */
	__set_CP(15, 0, attrib, 6, 1, 4);
	__ISB();

	/* DRSR (Data Region Size and Enable Register) */
	val = ARM_MPU_SIZE(0, size);
	__set_CP(15, 0, val, 6, 1, 2);
	__ISB();
}

/** Get the number of MPU regions that the device supports.
*/
__STATIC_INLINE uint32_t ARM_MPU_GetNrRegions(void)
{
	uint32_t tmp;

	__DSB();
	__ISB();
	__get_CP(15, 0, tmp, 0, 0, 4);

	return ((tmp >> 8) & 0xff);
}

#endif
