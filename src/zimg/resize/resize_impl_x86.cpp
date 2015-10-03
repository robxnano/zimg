#ifdef ZIMG_X86

#include "common/cpuinfo.h"
#include "resize_impl_x86.h"

namespace zimg {;
namespace resize {;

graph::IZimgFilter *create_resize_impl_h_x86(const FilterContext &context, unsigned height, PixelType type, unsigned depth, CPUClass cpu)
{
	X86Capabilities caps = query_x86_capabilities();
	graph::IZimgFilter *ret = nullptr;

	if (cpu == CPUClass::CPU_AUTO) {
		if (!ret && caps.sse)
			ret = create_resize_impl_h_sse(context, height, type, depth);
	} else {
		if (!ret && cpu >= CPUClass::CPU_X86_SSE)
			ret = create_resize_impl_h_sse(context, height, type, depth);
	}

	return ret;
}

graph::IZimgFilter *create_resize_impl_v_x86(const FilterContext &context, unsigned width, PixelType type, unsigned depth, CPUClass cpu)
{
	X86Capabilities caps = query_x86_capabilities();
	graph::IZimgFilter *ret = nullptr;

	if (cpu == CPUClass::CPU_AUTO) {
		if (!ret && caps.sse2)
			ret = create_resize_impl_v_sse2(context, width, type, depth);
		if (!ret && caps.sse)
			ret = create_resize_impl_v_sse(context, width, type, depth);
	} else {
		if (!ret && cpu >= CPUClass::CPU_X86_SSE2)
			ret = create_resize_impl_v_sse2(context, width, type, depth);
		if (!ret && cpu >= CPUClass::CPU_X86_SSE)
			ret = create_resize_impl_v_sse(context, width, type, depth);
	}

	return ret;
}

} // namespace resize
} // namespace zimg

#endif // ZIMG_X86
