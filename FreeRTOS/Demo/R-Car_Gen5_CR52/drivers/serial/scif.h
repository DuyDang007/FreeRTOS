#ifndef __SCIF_H__
#define __SCIF_H__

#include <stdint.h>

/// Initialize scif .
/// \param[in]     port          port number
/// \return 1 on success, 0 on error.
uint32_t console_init(uint32_t port);

void console_putc(char c);

#endif	/* __SCIF_H__ */
