#undef TRACE_SYSTEM
#define TRACE_SYSTEM i915

#if !defined(_TRACE_I915_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_I915_H

#include <linux/tracepoint.h>


TRACE_EVENT(intel_gpu_freq_change,
            TP_PROTO(u32 freq),
            TP_ARGS(freq),

            TP_STRUCT__entry(
                             __field(u32, freq)
                             ),

            TP_fast_assign(
                           __entry->freq = freq;
                           ),

            TP_printk("new_freq=%u", __entry->freq)
);

#endif /* _TRACE_I915_H */

/* This part must be outside protection */
#include <trace/define_trace.h>

