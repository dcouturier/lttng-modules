#undef TRACE_SYSTEM
#define TRACE_SYSTEM i915

#if !defined(LTTNG_TRACE_I915_H) || defined(TRACE_HEADER_MULTI_READ)
#define LTTNG_TRACE_I915_H

#include "../../../probes/lttng-tracepoint-event.h"

//#include "intel_drv.h"

#include <linux/async.h>
#include <linux/i2c.h>
#include <linux/hdmi.h>
#include <drm/i915_drm.h>
#include <drm/drm_crtc.h>
#include <drm/drm_crtc_helper.h>
#include <drm/drm_fb_helper.h>
#include <drm/drm_dp_mst_helper.h>

#include <uapi/drm/i915_drm.h>

#include "i915_reg.h"
#include "intel_bios.h"
#include "intel_ringbuffer.h"
#include "intel_lrc.h"
#include "i915_gem_gtt.h"
#include "i915_gem_render_state.h"
#include <linux/io-mapping.h>
#include <linux/i2c.h>
#include <linux/i2c-algo-bit.h>
#include <drm/intel-gtt.h>
#include <drm/drm_legacy.h> // for struct drm_dma_handle
#include <drm/drm_gem.h>
#include <linux/backlight.h>
#include <linux/hashtable.h>
#include <linux/intel-iommu.h>
#include <linux/kref.h>
#include <linux/pm_qos.h>

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,7,0))

/* object tracking */

LTTNG_TRACEPOINT_EVENT(i915_gem_object_create,
	    TP_PROTO(struct drm_i915_gem_object *obj),
	    TP_ARGS(obj),

	    TP_STRUCT__entry(
			     __field(struct drm_i915_gem_object *, obj)
			    // __field(u32, size)
			     ),

	    TP_fast_assign(
			   tp_assign(obj, obj)
			  // tp_assign(size, obj->base.size)
			   ),

	    TP_printk("obj=%p"
	    		//", size=%u"
	    		, __entry->obj
				//, __entry->size
				)
)

LTTNG_TRACEPOINT_EVENT(i915_vma_bind,
	    TP_PROTO(struct i915_vma *vma, unsigned flags),
	    TP_ARGS(vma, flags),

	    TP_STRUCT__entry(
			     __field(struct drm_i915_gem_object *, obj)
			     __field(struct i915_address_space *, vm)
			     __field(u32, offset)
			     __field(u32, size)
			     __field(unsigned, flags)
			     ),

	    TP_fast_assign(
			   tp_assign(obj, vma->obj)
			   tp_assign(vm, vma->vm)
			   tp_assign(offset, vma->node.start)
			   tp_assign(size, vma->node.size)
			   tp_assign(flags, flags)
			   ),

	    TP_printk("obj=%p, offset=%08x size=%x%s vm=%p",
		      __entry->obj, __entry->offset, __entry->size,
		      __entry->flags & PIN_MAPPABLE ? ", mappable" : "",
		      __entry->vm)
)

LTTNG_TRACEPOINT_EVENT(i915_vma_unbind,
	    TP_PROTO(struct i915_vma *vma),
	    TP_ARGS(vma),

	    TP_STRUCT__entry(
			     __field(struct drm_i915_gem_object *, obj)
			     __field(struct i915_address_space *, vm)
			     __field(u32, offset)
			     __field(u32, size)
			     ),

	    TP_fast_assign(
			   tp_assign(obj, vma->obj)
			   tp_assign(vm, vma->vm)
			   tp_assign(offset, vma->node.start)
			   tp_assign(size, vma->node.size)
			   ),

	    TP_printk("obj=%p, offset=%08x size=%x vm=%p",
		      __entry->obj, __entry->offset, __entry->size, __entry->vm)
)

LTTNG_TRACEPOINT_EVENT(i915_gem_object_change_domain,
	    TP_PROTO(struct drm_i915_gem_object *obj, u32 old_read, u32 old_write),
	    TP_ARGS(obj, old_read, old_write),

	    TP_STRUCT__entry(
			     __field(struct drm_i915_gem_object *, obj)
			     ),

	    TP_fast_assign(
			   tp_assign(obj, obj)
			   ),

	    TP_printk("obj=%p",
		      __entry->obj)
)

LTTNG_TRACEPOINT_EVENT(i915_gem_object_pwrite,
	    TP_PROTO(struct drm_i915_gem_object *obj, u32 offset, u32 len),
	    TP_ARGS(obj, offset, len),

	    TP_STRUCT__entry(
			     __field(struct drm_i915_gem_object *, obj)
			     __field(u32, offset)
			     __field(u32, len)
			     ),

	    TP_fast_assign(
			   tp_assign(obj, obj)
			   tp_assign(offset, offset)
			   tp_assign(len, len)
			   ),

	    TP_printk("obj=%p, offset=%u, len=%u",
		      __entry->obj, __entry->offset, __entry->len)
)

LTTNG_TRACEPOINT_EVENT(i915_gem_object_pread,
	    TP_PROTO(struct drm_i915_gem_object *obj, u32 offset, u32 len),
	    TP_ARGS(obj, offset, len),

	    TP_STRUCT__entry(
			     __field(struct drm_i915_gem_object *, obj)
			     __field(u32, offset)
			     __field(u32, len)
			     ),

	    TP_fast_assign(
			   tp_assign(obj, obj)
			   tp_assign(offset, offset)
			   tp_assign(len, len)
			   ),

	    TP_printk("obj=%p, offset=%u, len=%u",
		      __entry->obj, __entry->offset, __entry->len)
)

LTTNG_TRACEPOINT_EVENT(i915_gem_object_fault,
	    TP_PROTO(struct drm_i915_gem_object *obj, u32 index, bool gtt, bool write),
	    TP_ARGS(obj, index, gtt, write),

	    TP_STRUCT__entry(
			     __field(struct drm_i915_gem_object *, obj)
			     __field(u32, index)
			     __field(bool, gtt)
			     __field(bool, write)
			     ),

	    TP_fast_assign(
			   tp_assign(obj, obj)
			   tp_assign(index, index)
			   tp_assign(gtt, gtt)
			   tp_assign(write, write)
			   ),

	    TP_printk("obj=%p, %s index=%u %s",
		      __entry->obj,
		      __entry->gtt ? "GTT" : "CPU",
		      __entry->index,
		      __entry->write ? ", writable" : "")
)

LTTNG_TRACEPOINT_EVENT(i915_gem_evict,
	    TP_PROTO(struct drm_device *dev, u32 size, u32 align, unsigned flags),
	    TP_ARGS(dev, size, align, flags),

	    TP_STRUCT__entry(
			     __field(u32, dev)
			     __field(u32, size)
			     __field(u32, align)
			     __field(unsigned, flags)
			    ),

	    TP_fast_assign(
			   tp_assign(dev, dev->primary->index)
			   tp_assign(size, size)
			   tp_assign(align, align)
			   tp_assign(flags, flags)
			  ),

	    TP_printk("dev=%d, size=%d, align=%d %s",
		      __entry->dev, __entry->size, __entry->align,
		      __entry->flags & PIN_MAPPABLE ? ", mappable" : "")
)

LTTNG_TRACEPOINT_EVENT(i915_gem_evict_everything,
	    TP_PROTO(struct drm_device *dev),
	    TP_ARGS(dev),

	    TP_STRUCT__entry(
			     __field(u32, dev)
			    ),

	    TP_fast_assign(
			   tp_assign(dev, dev->primary->index)
			  ),

	    TP_printk("dev=%d", __entry->dev)
)

LTTNG_TRACEPOINT_EVENT(i915_gem_evict_vm,
	    TP_PROTO(struct i915_address_space *vm),
	    TP_ARGS(vm),

	    TP_STRUCT__entry(
			     __field(u32, dev)
			     __field(struct i915_address_space *, vm)
			    ),

	    TP_fast_assign(
			   tp_assign(dev, vm->dev->primary->index)
			   tp_assign(vm, vm)
			  ),

	    TP_printk("dev=%d, vm=%p", __entry->dev, __entry->vm)
)

LTTNG_TRACEPOINT_EVENT(i915_gem_ring_sync_to,
	    TP_PROTO(struct intel_engine_cs *from,
		     struct intel_engine_cs *to,
		     u32 seqno),
	    TP_ARGS(from, to, seqno),

	    TP_STRUCT__entry(
			     __field(u32, dev)
			     __field(u32, sync_from)
			     __field(u32, sync_to)
			     __field(u32, seqno)
			     ),

	    TP_fast_assign(
			   tp_assign(dev, from->dev->primary->index)
			   tp_assign(sync_from, from->id)
			   tp_assign(sync_to, to->id)
			   tp_assign(seqno, seqno)
			   ),

	    TP_printk("dev=%u, sync-from=%u, sync-to=%u, seqno=%u",
		      __entry->dev,
		      __entry->sync_from, __entry->sync_to,
		      __entry->seqno)
)

LTTNG_TRACEPOINT_EVENT(i915_gem_ring_dispatch,
	    TP_PROTO(struct intel_engine_cs *ring, u32 seqno, u32 flags),
	    TP_ARGS(ring, seqno, flags),

	    TP_STRUCT__entry(
			     __field(u32, dev)
			     __field(u32, ring)
			     __field(u32, seqno)
			     __field(u32, flags)
			     ),

	    TP_fast_assign(
			   tp_assign(dev, ring->dev->primary->index)
			   tp_assign(ring, ring->id)
			   tp_assign(seqno, seqno)
			   tp_assign(flags, flags)
			   i915_trace_irq_get(ring, seqno);
			   ),

	    TP_printk("dev=%u, ring=%u, seqno=%u, flags=%x",
		      __entry->dev, __entry->ring, __entry->seqno, __entry->flags)
)

LTTNG_TRACEPOINT_EVENT(i915_gem_ring_flush,
	    TP_PROTO(struct intel_engine_cs *ring, u32 invalidate, u32 flush),
	    TP_ARGS(ring, invalidate, flush),

	    TP_STRUCT__entry(
			     __field(u32, dev)
			     __field(u32, ring)
			     __field(u32, invalidate)
			     __field(u32, flush)
			     ),

	    TP_fast_assign(
			   tp_assign(dev, ring->dev->primary->index)
			   tp_assign(ring, ring->id)
			   tp_assign(invalidate, invalidate)
			   tp_assign(flush, flush)
			   ),

	    TP_printk("dev=%u, ring=%x, invalidate=%04x, flush=%04x",
		      __entry->dev, __entry->ring,
		      __entry->invalidate, __entry->flush)
)

LTTNG_TRACEPOINT_EVENT(i915_gem_request_complete,
	    TP_PROTO(struct intel_engine_cs *ring),
	    TP_ARGS(ring),

	    TP_STRUCT__entry(
			     __field(u32, dev)
			     __field(u32, ring)
			     __field(u32, seqno)
			     ),

	    TP_fast_assign(
			   tp_assign(dev, ring->dev->primary->index)
			   tp_assign(ring, ring->id)
			   tp_assign(seqno, ring->get_seqno(ring, false))
			   ),

	    TP_printk("dev=%u, ring=%u, seqno=%u",
		      __entry->dev, __entry->ring, __entry->seqno)
)

LTTNG_TRACEPOINT_EVENT(i915_gem_request_wait_begin,
	    TP_PROTO(struct intel_engine_cs *ring, u32 seqno),
	    TP_ARGS(ring, seqno),

	    TP_STRUCT__entry(
			     __field(u32, dev)
			     __field(u32, ring)
			     __field(u32, seqno)
			     __field(bool, blocking)
			     ),

//	     * NB: the blocking information is racy since mutex_is_locked
//	     * doesn't check that the current thread holds the lock. The only
//	     * other option would be to pass the boolean information of whether
//	     * or not the class was blocking down through the stack which is
//	     * less desirable.

	    TP_fast_assign(
			   tp_assign(dev, ring->dev->primary->index)
			   tp_assign(ring, ring->id)
			   tp_assign(seqno, seqno)
			   tp_assign(blocking, mutex_is_locked(&ring->dev->struct_mutex))
			   ),

	    TP_printk("dev=%u, ring=%u, seqno=%u, blocking=%s",
		      __entry->dev, __entry->ring, __entry->seqno,
		      __entry->blocking ?  "yes (NB)" : "no")
)

LTTNG_TRACEPOINT_EVENT(i915_flip_request,
		TP_PROTO(int plane, struct drm_i915_gem_object *obj),

		TP_ARGS(plane, obj),

		TP_STRUCT__entry(
				__field(int, plane)
				__field(struct drm_i915_gem_object *, obj)
		),

		TP_fast_assign(
				tp_assign(plane, plane)
				tp_assign(obj, obj)
		),

		TP_printk("plane=%d, obj=%p", __entry->plane, __entry->obj)
)

LTTNG_TRACEPOINT_EVENT(i915_flip_complete,
		TP_PROTO(int plane, struct drm_i915_gem_object *obj),

		TP_ARGS(plane, obj),

		TP_STRUCT__entry(
				__field(int, plane)
				__field(struct drm_i915_gem_object *, obj)
		),

		TP_fast_assign(
				tp_assign(plane,plane)
				tp_assign(obj, obj)
		),

		TP_printk("plane=%d, obj=%p", __entry->plane, __entry->obj)
)

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
