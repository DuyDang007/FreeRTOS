#include <openamp/remoteproc.h>

/* Implementation of remoteproc device for X5H */


/* Remote processor operations from r52 to a720. It defines
 * notification operation and remote processor managementi operations. */
const struct remoteproc_ops x5h_r_a_proc_ops = {
	.init   = NULL,
	.remove = NULL,
	.mmap   = NULL,
	.notify = NULL,
    // Do not implement since CR can boot independently
	.start  = NULL,
	.stop   = NULL,
	.shutdown = NULL,
};
