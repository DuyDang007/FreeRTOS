#include "gic.h"
//#include "cmsis_gcc.h"
#include "cmsis_rcar_gen5.h"

#define GICV3_ROUTE_AFF3_SHIFT           (8)


GICD_Type*       gic_dist;
GICR_Type*      gic_rdist;

static uint32_t gic_addr_valid = 0;
static uint32_t gic_max_rd = 0;

void R_GIC_SetICC_SRE(unsigned int value)
{
__set_CP(15, 0, value, 12, 12, 5); // Write value to ICC_SRE

__ISB();
}

uint32_t R_GIC_GetICC_SRE(void)
{
uint32_t value;

__get_CP(15, 0, value, 12, 12, 5); // Read ICC_SRE into value

return value;
}

void R_GIC_SetICC_HSRE(unsigned int value)
{
__set_CP(15, 4, value, 12, 9, 5); // Write value to ICC_HSRE

__ISB();
}

uint32_t R_GIC_GetICC_HSRE(void)
{
    uint32_t value;

    __get_CP(15, 4, value, 12, 9, 5); // Read ICC_HSRE into value

    return value;
}

void R_GIC_SetICC_MSRE(unsigned int value)
{
    __set_CP(15, 6, value, 12, 12, 5); // Write value to ICC_MSRE

    __ISB();
}

uint32_t R_GIC_GetICC_MSRE(void)
{
    uint32_t value;

    __get_CP(15, 6, value, 12, 12, 5); // Read ICC_MSRE into value

    return value;
}

void R_GIC_EnableGroup0Ints(void) 
{
    uint32_t ICC_IGRPEN0 = 1;
   
   __set_CP(15, 0, ICC_IGRPEN0, 12, 12, 6); // Write value to ICC_IGRPEN0

   __ISB();
}

void R_GIC_DisableGroup0Ints(void)
{
    uint32_t value = 0;

    __set_CP(15, 0, value, 12, 12, 6); // Write value to ICC_IGRPEN0

    __ISB();
}

void R_GIC_EnableGroup1Ints(void)
{
    uint32_t value = 1;

    __set_CP(15, 0, value, 12, 12, 7); // Write value to ICC_IGRPEN1

    __ISB();
}

void R_GIC_DisableGroup1Ints(void)
{
    uint32_t ICC_IGRPEN1 = 0;

    __set_CP(15, 0, ICC_IGRPEN1, 12, 12, 7); // Write value to ICC_IGRPEN1

    __ISB();
}

void R_GIC_EnableNSGroup1Ints(void)
{
    uint32_t value;

    __get_CP(15, 6, value, 12, 12, 7); // Read ICC_MGRPEN1 into value

    value |= 0x1;

    __set_CP(15, 6, value, 12, 12, 7); // Write value to ICC_MGRPEN1

    __ISB();
}

void R_GIC_DisableNSGroup1Ints(void)
{
    uint32_t value;

    __get_CP(15, 6, value, 12, 12, 7); // Read ICC_MGRPEN1 into value

    value &= ~0x1;

    __set_CP(15, 6, value, 12, 12, 7); // Write value to ICC_MGRPEN1

    __ISB();
}

unsigned int R_GIC_GetICC_MCTLR(void)
{
    uint32_t value;

    __get_CP(15, 6, value, 12, 12, 4); // Read ICC_MCTLR into value

    return value;
}

unsigned int R_GIC_GetICC_CTLR(void)
{
    uint32_t value;

    __get_CP(15, 0, value, 12, 12, 4); // Read ICC_CTLR into value

    return value;
}

void R_GIC_SetICC_MCTLR(unsigned int value)
{
    __set_CP(15, 0, value, 12, 12, 4); // Write value to ICC_CTLR

    __ISB();
}

unsigned int R_GIC_ReadIntAck(void)
{
    uint32_t value;

    __get_CP(15, 0, value, 12, 8, 0); // Read ICC_IAR0 into value

    return value;
}

void R_GIC_WriteEOI(unsigned int ID)
{
    __set_CP(15, 0, ID, 12, 8, 1); // Write value to ICC_EOIR0

    __ISB();
}

void R_GIC_WriteDIR(unsigned int ID)
{
    __set_CP(15, 0, ID, 12, 11, 1); // Write value to ICC_DIR

    __ISB();
}

unsigned int R_GIC_ReadAliasedIntAck(void)
{
    uint32_t value;

    __get_CP(15, 0, value, 12, 12, 0); // Read ICC_IAR1 into value

    return value;
}

void R_GIC_WriteAliasedEOI(unsigned int ID)
{
    __set_CP(15, 0, ID, 12, 12, 1); // Write value to ICC_EOIR1

    __DSB();
}

unsigned int R_GIC_GetPriorityMask()
{
    uint32_t priority_mask;

    __get_CP(15, 0, priority_mask, 4, 6, 0); // Read ICC_PMR into priority_mask

    return priority_mask ;
}

void R_GIC_SetPriorityMask(unsigned int ID)
{
    __set_CP(15, 0, ID, 4, 6, 0); // Write ID to ICC_PMR
}

unsigned int R_GIC_GetBinaryPoint(void)
{
    uint32_t value;

    __get_CP(15, 0, value, 12, 8, 3); // Read ICC_BPR0 into value

    return value;
}

void R_GIC_SetBinaryPoint(unsigned int ID)
{
    __set_CP(15, 0, ID, 12, 8, 3); // Write ID to ICC_BPR0

    __ISB();
}

unsigned int R_GIC_GetAliasedBinaryPoint(void)
{
    uint32_t value;

    __get_CP(15, 0, value, 12, 12, 3); // Read ICC_BPR1 into value

    return value;
}

void R_GIC_SetAliasedBinaryPoint(unsigned int ID)
{
    __set_CP(15, 0, ID, 12, 12, 3); // Write ID to ICC_BPR1

    __ISB();
}

uint32_t R_GIC_GetRunningPriority(void)
{
    uint32_t value;

    __get_CP(15, 0, value, 12, 11, 3); // Read ICC_RPR into value

    return value;
}

void R_GIC_SendGroup0SGI(unsigned int ID, unsigned int mode, unsigned target_list)
{
    uint64_t value;

    value = (ID | target_list) | (uint64_t)mode << 32;

    __set_CP64(15, 2, value, 12); // Write value_L to ICC_SGI0R[31:0] and value_H to ICC_SGI0R[63:32]
}

void R_GIC_SendGroup1SGI(unsigned int ID, unsigned int mode, unsigned target_list)
{
    uint64_t value;

    value = (ID | target_list) | (uint64_t)mode << 32;

    __set_CP64(15, 0, value, 12); // Write value_L to ICC_SGI1R[31:0] and value_H to ICC_SGI1R[63:32]
}

void R_GIC_sendOtherGroup1SGI(unsigned int ID, unsigned int mode, unsigned target_list)
{
    uint64_t value;

    value = (ID | target_list) | (uint64_t)mode << 32;

    __set_CP64(15, 1, value, 12); // Write value_L to ICC_ASGI1R[31:0] and value_H to ICC_ASGI1R[63:32]
}

void R_GIC_SetICH_HCR(unsigned int value)
{
    __set_CP(15, 4, value, 12, 11, 0); // Write value to ICH_HCR

    __ISB();
}

unsigned int R_GIC_GetICH_HCR(void)
{
    uint32_t value;

    __get_CP(15, 4, value, 12, 11, 0); // Read ICH_HCR into value

    return value;
}

void R_GIC_SetICH_AP0R0(unsigned int value)
{
    __set_CP(15, 4, value, 12, 8, 0); // Write value to ICH_AP0R0

    __ISB();
}

unsigned int R_GIC_GetICH_AP0R0(void)
{
    uint32_t value;

    __get_CP(15, 4, value, 12, 8, 0); // Read ICH_AP0R0 into value

    return value;
}

void R_GIC_SetICH_AP1R0(unsigned int value)
{
    __set_CP(15, 4, value, 12, 9, 0); // Write value to ICH_AP1R0

    __ISB();
}

unsigned int R_GIC_GetICH_AP1R0(void)
{
    uint32_t value;

    __get_CP(15, 4, value, 12, 9, 0); // Read ICH_AP1R0 into value

    return value;
}

void R_GIC_SetICH_LR0(unsigned int value)
{
    __set_CP(15, 4, value, 12, 12, 0); // Write value to ICH_LR0

    __ISB();
}

unsigned int R_GIC_GetICH_LR0(void)
{
    uint32_t value;

    __get_CP(15, 4, value, 12, 12, 0); // Read ICH_LR0 into value

    return value;
}

void R_GIC_SetICH_LRC0(unsigned int value)
{
    __set_CP(15, 4, value, 12, 14, 0); // Write value to ICH_LRC0

    __ISB();
}

unsigned int R_GIC_GetICH_LRC0(void)
{
    uint32_t value;

    __get_CP(15, 4, value, 12, 14, 0); // Read ICH_LRC0 into value

    return value;
}

void R_GIC_SetICH_LR1(unsigned int value)
{
    __set_CP(15, 4, value, 12, 12, 1); // Write value to ICH_LR1

    __ISB();
}

unsigned int R_GIC_GetICH_LR1(void)
{
    uint32_t value;

    __get_CP(15, 4, value, 12, 12, 1); // Read ICH_LR1 into value

    return value;
}

void R_GIC_SetICH_LRC1(unsigned int value)
{
    __set_CP(15, 4, value, 12, 14, 1); // Write value to ICH_LRC1

    __ISB();
}

unsigned int R_GIC_GetICH_LRC1(void)
{
    uint32_t value;

    __get_CP(15, 4, value, 12, 14, 1); // Read ICH_LRC1 into value

    return value;
}

void R_GIC_SetICH_LR2(unsigned int value)
{
    __set_CP(15, 4, value, 12, 12, 2); // Write value to ICH_LR2

    __ISB();
}

unsigned int R_GIC_GetICH_LR2(void)
{
    uint32_t value;

    __get_CP(15, 4, value, 12, 12, 2); // Read ICH_LR2 into value

    return value;
}

void R_GIC_SetICH_LRC2(unsigned int value)
{
    __set_CP(15, 4, value, 12, 14, 2); // Write value to ICH_LRC2

    __ISB();
}

unsigned int R_GIC_GetICH_LRC2(void)
{
    uint32_t value;

    __get_CP(15, 4, value, 12, 14, 2); // Read ICH_LRC2 into value

    return value;
}

void R_GIC_SetICH_LR3(unsigned int value)
{
    __set_CP(15, 4, value, 12, 12, 3); // Write value to ICH_LR3

    __ISB();
}

unsigned int R_GIC_GetICH_LR3(void)
{
    uint32_t value;

    __get_CP(15, 4, value, 12, 12, 3); // Read ICH_LR3 into value

    return value;
}

void R_GIC_SetICH_LRC3(unsigned int value)
{
    __set_CP(15, 4, value, 12, 14, 3); // Write value to ICH_LRC3

    __ISB();
}

unsigned int R_GIC_GetICH_LRC3(void)
{
    uint32_t value;

    __get_CP(15, 4, value, 12, 14, 3); // Read ICH_LRC3 into value

    return value;
}

void R_GIC_SetAddr(void* dist, void* rdist) {
    uint32_t index = 0;

    gic_dist = (GICD_Type *)dist;
    gic_rdist = (GICR_Type *)rdist;
    gic_addr_valid = 1;

    // Now find the maximum RD ID that I can use
    // This is used for range checking in later functions
    while((gic_rdist[index].lpis.GICR_TYPER[0] & (1<<4)) == 0) // Keep incrementing until GICR_TYPER.Last reports no more RDs in block
    {
      index++;
    }

    gic_max_rd = index;
    return;

}

uint32_t R_GIC_Enable(void) {
    uint32_t result = 1;  // Success indicator

    // Check that GIC pointers are valid
    if (gic_dist == NULL)
        return 1;

    // First set the ARE bits
    gic_dist->GICD_CTLR = (1 << 5) | (1 << 4);

    // The split here is because the register layout is different once ARE==1

    // Now set the rest of the options
    gic_dist->GICD_CTLR = 7 | (1 << 5) | (1 << 4);

    return result;
}

/* ------------------------------------------------------------
 * Redistributor Functions
 * ------------------------------------------------------------ */
uint32_t R_GIC_GetRedistID(uint32_t affinity) {
    uint32_t index = 0;

    // Check that GIC pointers are valid
    if (gic_addr_valid==0)
      return 0xFFFFFFFF;

    do
    {
      if (gic_rdist[index].lpis.GICR_TYPER[1] == affinity)
         return index;

      index++;
    }
    while(index <= gic_max_rd);

    return 0xFFFFFFFF; // return -1 to signal not RD found
}

uint32_t R_GIC_WakeUpRedist(uint32_t rd) {
    uint32_t tmp;

    // Check that GIC pointers are valid
    if (gic_addr_valid==0)
      return 1;

    // Tell the Redistributor to wake-up by clearing ProcessorSleep bit
    tmp = gic_rdist[rd].lpis.GICR_WAKER;
    tmp = tmp & ~0x2;
    gic_rdist[rd].lpis.GICR_WAKER = tmp;

    // Poll ChildrenAsleep bit until Redistributor wakes
    do
    {
      tmp = gic_rdist[rd].lpis.GICR_WAKER;
    }
    while((tmp & 0x4) != 0);

    return 0;

}

uint32_t R_GIC_EnableInt(uint32_t ID, uint32_t rd) {
    (void)ID; (void)rd;
    uint32_t result = 1;
    uint32_t bank, max_ppi, max_spi;
    uint8_t* config;

    #ifdef DEBUG
    printf("enableInt:: Enabling INTID %d on RD%d\n", ID, rd);
    #endif

    // Check that GIC pointers are valid
   if (gic_addr_valid==0)
   {
     #ifdef DEBUG
     printf("enableInt:: ERROR - GIC pointers not intialized\n");
     #endif
     return 1;
   }

    if (ID < 31)
    {
      // Check rd in range
      if (rd > gic_max_rd)
      {
         #ifdef DEBUG
         printf("enableInt:: ERROR - Invalid RD index.\n");
         #endif
         return 1;
      }

       // SGI or PPI
       ID   = ID & 0x1f;    // ... and which bit within the register
       ID   = 1 << ID;      // Move a '1' into the correct bit position

       gic_rdist[rd].sgis.GICR_ISENABLER[0] = ID;
    }
    else if (ID < 1020)
    {
      // SPI
      bank = ID/32;        // There are 32 IDs per register, need to work out which register to access
      ID   = ID & 0x1f;    // ... and which bit within the register

      ID   = 1 << ID;      // Move a '1' into the correct bit position

      gic_dist->GICD_ISENABLER[bank] = ID;
    }
    else
    {
      #ifdef DEBUG
      printf("enableInt:: ERROR - Invalid interrupt.\n");
      #endif
      return 1;
    }

    return result;
}

uint32_t R_GIC_DisableInt(uint32_t ID, uint32_t rd) {
    uint32_t bank, max_ppi, max_spi;
    uint8_t* config;

    #ifdef DEBUG
    printf("disableInt:: Disabling INTID %d on RD%d\n", ID, rd);
    #endif

    // Check that GIC pointers are valid
    if (gic_addr_valid==0)
    {
      #ifdef DEBUG
      printf("disableInt:: ERROR - GIC pointers not intialized\n");
      #endif
      return 1;
    }

    if (ID < 31)
    {
      // Check rd in range
      if (rd > gic_max_rd)
         return 1;

      // SGI or PPI
      ID   = ID & 0x1f;    // ... and which bit within the register
      ID   = 1 << ID;      // Move a '1' into the correct bit position

      gic_rdist[rd].sgis.GICR_ICENABLER[0] = ID;
    }
    else if (ID < 1020)
    {
      // SPI
      bank = ID/32;        // There are 32 IDs per register, need to work out which register to access
      ID   = ID & 0x1f;    // ... and which bit within the register

      ID   = 1 << ID;      // Move a '1' into the correct bit position

      gic_dist->GICD_ICENABLER[bank] = ID;
    }
    else
    {
      #ifdef DEBUG
      printf("disableInt:: ERROR - Invalid interrupt.\n");
      #endif
      return 1;
    }

    return 0;
}

uint32_t R_GIC_SetIntPriority(uint32_t ID, uint32_t rd, uint8_t priority) {
    uint8_t* config;
    uint32_t max_ppi, max_spi;

    #ifdef DEBUG
    printf("setIntPriority:: Setting priority of INTID %d on RD%d to 0x%x\n", ID, rd, priority);
    #endif

    // Check that GIC pointers are valid
    if (gic_addr_valid==0)
    {
      #ifdef DEBUG
      printf("setIntPriority:: ERROR - GIC pointers not intialized\n");
      #endif
      return 1;
    }

    if (ID < 31)
    {
      // Check rd in range
      if (rd > gic_max_rd)
         return 1;

      // SGI or PPI
      gic_rdist[rd].sgis.GICR_IPRIORITYR[ID] = priority;
    }
    else if (ID < 1020)
    {
      // SPI
      gic_dist->GICD_IPRIORITYR[ID] = priority;
    }
    else
    {
      #ifdef DEBUG
      printf("setIntPriority:: ERROR - Invalid interrupt.\n");
      #endif
      return 1;
    }

    return 0;
}

uint32_t R_GIC_SetIntType(uint32_t ID, uint32_t rd, uint32_t type) {
  uint8_t* config;
  uint32_t bank, tmp, conf, max_spi;

  #ifdef DEBUG
  printf("setIntType:: Setting INTID %d on RD%d as type 0x%x\n", ID, rd, type);
  #endif

  // Check that GIC pointers are valid
  if (gic_addr_valid==0)
    return 1;

  if (ID < 31)
  {
    // SGI or PPI
    // Config of SGIs is fixed
    // It is IMP DEF whether ICFG for PPIs is write-able, on Arm implementations it is fixed
    return 1;
  }
  else if (ID < 1020)
  {
    // SPI
    type = type & 0x3;            // Mask out unused bits

    bank = ID/16;                 // There are 16 IDs per register, need to work out which register to access
    ID   = ID & 0xF;              // ... and which field within the register
    ID   = ID * 2;                // Convert from which field to a bit offset (2-bits per field)

    conf = conf << ID;            // Move configuration value into correct bit position

    tmp = gic_dist->GICD_ICFGR[bank];     // Read current value
    tmp = tmp & ~(0x3 << ID);             // Clear the bits for the specified field
    tmp = tmp | conf;                     // OR in new configuration
    gic_dist->GICD_ICFGR[bank] = tmp;     // Write updated value back
  }
  else
    return 1;

  return 0;

}

uint32_t R_GIC_SetIntGroup(uint32_t ID, uint32_t rd, uint32_t security) {
    uint8_t* config;
    uint32_t bank, tmp, group, mod, max_ppi, max_spi;

    #ifdef DEBUG
    printf("setIntGroup:: Setting INTID %d on RD%d as groups 0x%x\n", ID, rd, security);
    #endif

    // Check that GIC pointers are valid
    if (gic_addr_valid==0)
      return 1;

    if (ID < 31)
    {
      // Check rd in range
      if (rd > gic_max_rd)
         return 1;

      // SGI or PPI
      ID   = ID & 0x1f;    // Find which bit within the register
      ID   = 1 << ID;      // Move a '1' into the correct bit position

      // Read current values
      group = gic_rdist[rd].sgis.GICR_IGROUPR[0];
      mod   = gic_rdist[rd].sgis.GICR_IGRPMODR[0];

      // Update required bits
      switch (security)
      {
        case GICV3_GROUP0:
          group = (group & ~ID);
          mod   = (mod   & ~ID);
          break;

        case GICV3_GROUP1_SECURE:
          group = (group & ~ID);
          mod   = (mod   | ID);
          break;

        case GICV3_GROUP1_NON_SECURE:
          group = (group | ID);
          mod   = (mod   & ~ID);
          break;

        default:
          return 1;
      }

      // Write modified version back
      gic_rdist[rd].sgis.GICR_IGROUPR[0] = group;
      gic_rdist[rd].sgis.GICR_IGRPMODR[0] = mod;
    }
    else if (ID < 1020)
    {
      // SPI
      bank = ID/32;        // There are 32 IDs per register, need to work out which register to access
      ID   = ID & 0x1f;    // ... and which bit within the register

      ID   = 1 << ID;      // Move a '1' into the correct bit position

      group = gic_dist->GICD_IGROUPR[bank];
      mod   = gic_dist->GICD_IGRPMODR[bank];

      switch (security)
      {
        case GICV3_GROUP0:
          group = (group & ~ID);
          mod   = (mod   & ~ID);
          break;

        case GICV3_GROUP1_SECURE:
          group = (group & ~ID);
          mod   = (mod   | ID);
          break;

        case GICV3_GROUP1_NON_SECURE:
          group = (group | ID);
          mod   = (mod   & ~ID);
          break;

        default:
          return 1;
      }

      gic_dist->GICD_IGROUPR[bank] = group;
      gic_dist->GICD_IGRPMODR[bank] = mod;
    }
    else
      return 1;

  return 0;
}

uint32_t R_GIC_SetIntRoute(uint32_t ID, uint32_t mode, uint32_t affinity) {
  uint64_t tmp, max_spi;

  #ifdef DEBUG
  printf("setIntRoute:: Routing INTID %d to mode=0x%x and affinity=0x%08x\n", ID, mode, affinity);
  #endif

  // Check that GIC pointers are valid
  if (gic_addr_valid==0)
    return 0xFFFFFFFF;

  // Check for SPI ranges
  // Combine routing in
  if ((ID > 31) && (ID < 1020)) {
    tmp = (uint64_t)(affinity & 0x00FFFFFF) | (((uint64_t)affinity & 0xFF000000) << GICV3_ROUTE_AFF3_SHIFT) | ((uint64_t)mode);
    gic_dist->GICD_IROUTER[ID] = tmp;
  } 
  else 
  {
      return 1;
  }

  return 0;
}

uint32_t R_GIC_SetIntPending(uint32_t ID, uint32_t rd) {
    uint8_t* config;
    uint32_t bank, tmp, max_ppi, max_spi;

    #ifdef DEBUG
    printf("setIntPending:: Setting INTID %d on RD%d as Pending\n", ID, rd);
    #endif

    // Check that GIC pointers are valid
    if (gic_addr_valid==0)
      return 0xFFFFFFFF;

    if (ID < 31)
    {
      // Check rd in range
      if (rd > gic_max_rd)
         return 1;

      ID   = ID & 0x1f;    // Find which bit within the register
      ID   = 1 << ID;      // Move a '1' into the correct bit position

      gic_rdist[rd].sgis.GICR_ISPENDR[0] = ID;
    }
    else if (ID < 1020)
    {
      // SPI
      bank = ID/32;        // There are 32 IDs per register, need to work out which register to access
      ID   = ID & 0x1f;    // ... and which bit within the register

      ID   = 1 << ID;      // Move a '1' into the correct bit position

      gic_dist->GICD_ISPENDR[bank] = ID;
    }
    else
      return 1;

    return 0;

}

uint32_t R_GIC_ClearIntPending(uint32_t ID, uint32_t rd) {
    uint8_t* config;
    uint32_t bank, tmp, max_ppi, max_spi;

    #ifdef DEBUG
    printf("clearIntPending:: Clearing pending state of INTID %d on RD%d\n", ID, rd);
    #endif

    // Check that GIC pointers are valid
    if (gic_addr_valid==0)
      return 0xFFFFFFFF;

    if (ID < 31)
    {
      // Check rd in range
      if (rd > gic_max_rd)
         return 1;

      ID   = ID & 0x1f;    // Find which bit within the register
      ID   = 1 << ID;      // Move a '1' into the correct bit position

      gic_rdist[rd].sgis.GICR_ICPENDR[0] = ID;

    }
    else if (ID < 1020)
    {
      // SPI
      bank = ID/32;        // There are 32 IDs per register, need to work out which register to access
      ID   = ID & 0x1f;    // ... and which bit within the register

      ID   = 1 << ID;      // Move a '1' into the correct bit position

      gic_dist->GICD_ICPENDR[bank] = ID;
    }
    else
      return 1;

    return 0;
}


