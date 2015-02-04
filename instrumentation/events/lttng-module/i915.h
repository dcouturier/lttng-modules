#undef TRACE_SYSTEM
#define TRACE_SYSTEM i915

#if !defined(LTTNG_TRACE_I915_H) || defined(TRACE_HEADER_MULTI_READ)
#define LTTNG_TRACE_I915_H

#include "../../../probes/lttng-tracepoint-event.h"

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,7,0))
LTTNG_TRACEPOINT_EVENT(intel_gpu_freq_change,
	
	TP_PROTO(u32 freq),

	TP_ARGS(freq),

	TP_STRUCT__entry(
		__field(u32, freq)
	),

	TP_fast_assign(
		tp_assign(freq, freq)
	),

	TP_printk("new_freq=%u",__entry->freq)
)
#endif

#endif /* LTTNG_TRACE_I915_H */

/* This part must be outside protection */
#include "../../../probes/define_trace.h"
