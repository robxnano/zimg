#pragma once

#ifndef ZIMG_DEPTH_DITHER2_H_
#define ZIMG_DEPTH_DITHER2_H_

#include <tuple>
#include "Common/align.h"
#include "Common/zfilter.h"

namespace zimg {;

enum class CPUClass;

struct PixelFormat;

namespace depth {;

enum class DitherType;

class OrderedDitherBase : public ZimgFilter {
public:
	typedef void (*func_type)(const float *dither, unsigned dither_offset, unsigned dither_len, const void *src, void *dst, float scale, float offset, unsigned bits, unsigned width);
	typedef void (*f16c_func_type)(const void *src, void *dst, unsigned width);
protected:
	AlignedVector<float> m_dither;

	func_type m_func;
	f16c_func_type m_f16c;

	PixelType m_pixel_in;
	PixelType m_pixel_out;
	float m_scale;
	float m_offset;
	unsigned m_depth;

	OrderedDitherBase(const PixelFormat &pixel_in, const PixelFormat &pixel_out, CPUClass cpu);

	virtual std::tuple<unsigned, unsigned, unsigned> get_dither_params(unsigned i, unsigned left) const = 0;
public:
	zimg_filter_flags get_flags() const override;

	size_t get_tmp_size(unsigned left, unsigned right) const override;

	void process(void *ctx, const zimg_image_buffer *src, const zimg_image_buffer *dst, void *tmp, unsigned i, unsigned left, unsigned right) const override;
};

class NoneDither final : public OrderedDitherBase {
	std::tuple<unsigned, unsigned, unsigned> get_dither_params(unsigned i, unsigned left) const override;
public:
	NoneDither(const PixelFormat &pixel_in, const PixelFormat &pixel_out, CPUClass cpu);
};

class BayerDither final : public OrderedDitherBase {
	std::tuple<unsigned, unsigned, unsigned> get_dither_params(unsigned i, unsigned left) const override;
public:
	BayerDither(const PixelFormat &pixel_in, const PixelFormat &pixel_out, CPUClass cpu);
};

class RandomDither final : public OrderedDitherBase {
	static const unsigned RAND_NUM = 1 << 14;

	std::tuple<unsigned, unsigned, unsigned> get_dither_params(unsigned i, unsigned left) const override;
public:
	RandomDither(const PixelFormat &pixel_in, const PixelFormat &pixel_out, CPUClass cpu);
};

class ErrorDiffusion final : public ZimgFilter {
public:
	typedef void(*func_type)(const void *src, void *dst, void *error_top, void *error_cur, float scale, float offset, unsigned bits, unsigned width);
	typedef void(*f16c_func_type)(const void *src, void *dst, unsigned width);
private:
	func_type m_func;
	f16c_func_type m_f16c;
	PixelType m_pixel_in;
	PixelType m_pixel_out;
	float m_scale;
	float m_offset;
	unsigned m_depth;
	unsigned m_width;

	size_t get_context_width() const;
public:
	ErrorDiffusion(unsigned width, const PixelFormat &pixel_in, const PixelFormat &pixel_out, CPUClass cpu);

	zimg_filter_flags get_flags() const override;

	size_t get_context_size() const override;

	void init_context(void *ctx) const override;

	void process(void *ctx, const zimg_image_buffer *src, const zimg_image_buffer *dst, void *tmp, unsigned i, unsigned left, unsigned right) const override;
};

IZimgFilter *create_dither_convert2(DitherType type, unsigned width, const PixelFormat &pixel_in, const PixelFormat &pixel_out, CPUClass cpu);

} // namespace depth
} // namespace zimg

#endif // ZIMG_DEPTH_DITHER2_H_
