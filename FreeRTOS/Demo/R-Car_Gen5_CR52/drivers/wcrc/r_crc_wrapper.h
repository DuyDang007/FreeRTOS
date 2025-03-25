#ifndef _RENESAS_CRC_WRAPPER_H_
#define _RENESAS_CRC_WRAPPER_H_

#include "wcrc/r_wcrc_common.h"

int wcrcSetMode(wcrc_instance_ctrl_t * const p_ctrl);

void wcrcRemoveBuffer(void * p_buf);

int wcrcStart(wcrc_instance_ctrl_t * const p_ctrl);

#endif /* _RENESAS_CRC_WRAPPER_H_ */
