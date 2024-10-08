#ifndef _PCIE_H_
#define _PCIE_H_

/*******************************************************************************
  Includes
*******************************************************************************/
#include "drivers/pcie/common.h"
#include "drivers/pcie/debug_common.h"

/*******************************************************************************
  Functions
*******************************************************************************/
uint32_t Pcie_SimUT(void);
uint32_t Pcie_loopback_test_fromCh1(void);
uint32_t Pcie_TestMain(void);

/*******************************************************************************
  Test ID define
*******************************************************************************/
typedef enum {
	PCIE_SIM_UT,
	PCIE_EXIT,
} PCIE_TESTID;

/*******************************************************************************
  Test Menu Table
*******************************************************************************/
/* Table index = Test ID */
static const struct testMenu pcie_TestTbl[] = {
	{ PCIE_SIM_UT,			"PCIE Sim UT",		Pcie_SimUT, },
	{ PCIE_EXIT,			"EXIT",						0, },
};

#endif /* _PCIE_H_ */
