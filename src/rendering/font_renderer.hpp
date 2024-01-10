// MIT License
// 
// Copyright (c) 2023 Mihail Mladenov
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#pragma once

#include <common/types.hpp>
#include <common/error.hpp>

#include <algebra/algebra.hpp>

#include <rendering/color.hpp>
#include <rendering/quad.hpp>

#include <shaders/common.hpp>

#include <gpu_api/shader.hpp>
#include <gpu_api/pso.hpp>

namespace WL
{
	struct FontRendererOptions
	{
		U32 fontIndex = 0;
		F32 height = 0.05f;
		Color4 color = Color4(1.f, 0.f, 0.f, 0.f);
		F32 offsetX = 0;
		F32 offestY = 0;
	};

	template <typename TPresentSurface, typename TRasterizer>
	class FontRenderer
	{
	public:
		using Rasterizer = TRasterizer;
		using PresentSurface = TPresentSurface;
		using GPUAPI = PresentSurface::GPUAPI;
		using Buffer = GPUAPI::Buffer;
		using Image = GPUAPI::Image;
		static auto Init() -> B;
		static auto AccumulateDrawState(const Array<I32> codepoints, const FontRendererOptions& options) -> V;
		static auto CommitDrawCommands() -> V;
		static auto Clear() -> V;

	private:
		static auto AllocateBuffers(U32 atlasIdx) -> B;
		static auto ReallocateBuffers(U32 atlasIdx, U32 newCapacity) -> B;


		inline static constexpr U32 initialGlyphInstancesCapacity = 1024;
		inline static U32 instanceCapacity = initialGlyphInstancesCapacity;
		inline static U32 instances = 0;

		inline static GPUAPI::Pso pso;

		struct Buffers
		{
			Image* atlas;
			Buffer linearTransformsBuffer;
			Buffer translationsBuffer;
			Buffer uvsBuffer;
			Buffer colorsBuffer;

			Array<Mat2x2> linearTransformsBufferCPU;
			Array<Vec2> translationsBufferCPU;
			Array<Vec4> uvsBufferCPU;
			Array<ColorU32> colorsBufferCPU;
		};

		inline static Map<U32, Buffers*> atlasIdxToBuffers;

		inline static Set<U32> usedAtlases;
	};
}

namespace WL
{
	template <typename TPresentSurface, typename TRasterizer>
	auto FontRenderer<TPresentSurface, TRasterizer>::Init() -> B
	{
		if (!Rasterizer::Init())
		{
			return false;
		}

		for (auto atlasIdx : Rasterizer::availableAtlases)
		{
			if (!AllocateBuffers(atlasIdx))
			{
				return false;
			}
		}

		pso.AddShader(FontAtlasTextVert.data(), FontAtlasTextVert.size(), EShaderType::Vertex);
		pso.AddShader(FontAtlasTextFrag.data(), FontAtlasTextFrag.size(), EShaderType::Fragment);

		pso.AddVBLayout({ .binding = 0, .type = EType::Float, .components = 4 });
		pso.AddVBLayout({ .binding = 1, .type = EType::Float, .components = 2 });
		pso.AddVBLayout({ .binding = 2, .type = EType::Float, .components = 4 });
		pso.AddVBLayout({ .binding = 3, .type = EType::Float, .components = 1 });

		pso.AddTexture(0, "uTexture0");

		return pso.Compile();

	}


	template <typename TPresentSurface, typename TRasterizer>
	auto FontRenderer<TPresentSurface, TRasterizer>::AccumulateDrawState(const Array<I32> codepoints, const FontRendererOptions& options) -> V
	{
		auto uHeight = PresentSurface::UnitsToPixels(options.fontIndex);
		auto atlasIdx = options.fontIndex | (uHeight << 16);


		WL_ASSERT(atlasIdxToBuffers.find(atlasIdx) != atlasIdxToBuffers.end());
		auto buffersPtr = atlasIdxToBuffers[atlasIdx];
		usedAtlases.insert(atlasIdx);

		instances++;
		if (instances > instanceCapacity)
		{
			ReallocateBuffers(atlasIdx, instanceCapacity * 2);
			instanceCapacity *= 2;
		}

		auto& rasterizedFont = Rasterizer::GetRasterizedFont();
		buffersPtr->atlas = rasterizedFont->heightToAtlas[uHeight];

		F32 xAdvance = 0.f;
		for (auto codepoint : codepoints)
		{
			auto& glyphDesc = rasterizedFont.codePointAndHeightToGlyph[U32(codepoint) | (U64(uHeight) << 32)];
			auto ar = PresentSurface::GetAspectRation();

			F32 x0 = PresentSurface::PixelsToUnits(glyphDesc.xOffset0);
			F32 y0 = PresentSurface::PixelsToUnits(glyphDesc.yOffset0) * ar;
			F32 x1 = PresentSurface::PixelsToUnits(glyphDesc.xOffset1);
			F32 y1 = PresentSurface::PixelsToUnits(glyphDesc.yOffset1) * ar;
			F32 width = x1 - x0;
			F32 height = y1 - y0;
			buffersPtr->uvsBufferCPU.emplace_back(glyphDesc.u0, glyphDesc.v0, glyphDesc.u1, glyphDesc.v1);
			buffersPtr->colorsBufferCPU.emplace_back(options.color);
			buffersPtr->linearTransformsBufferCPU.emplace_back(width, 0.f, 0.f, height);
			buffersPtr->translationsBufferCPU.emplace_back(options.offsetX + x0 * 2.f + xAdvance, options.offestY + x0 * 2.f);
			xAdvance += x0 * 2 + glyphDesc.xAdvance;
		}
	}


	template <typename TPresentSurface, typename TRasterizer>
	auto FontRenderer<TPresentSurface, TRasterizer>::CommitDrawCommands() -> V
	{
		if (!instances)
		{
			return;
		}

		for (auto atlasIdx : usedAtlases)
		{
			auto buffersPtr = atlasIdxToBuffers[atlasIdx];

			buffersPtr->uvsBuffer.Update(buffersPtr->uvsBufferCPU);
			buffersPtr->colorsBuffer.Update(buffersPtr->colorsBufferCPU);
			buffersPtr->linearTransformsBuffer.Update(buffersPtr->linearTransformsBufferCPU);
			buffersPtr->translationsBuffer.Update(buffersPtr->translationsBufferCPU);

			pso.BindVB(0, buffersPtr->linearTransformsBuffer, true);
			pso.BindVB(1, buffersPtr->translationsBuffer, true);
			pso.BindVB(2, buffersPtr->uvsBuffer, true);
			pso.BindVB(3, buffersPtr->colorsBuffer, true);

			pso.Use();
			pso.BindTexture(*buffersPtr->atlas);

			pso.DrawInstanced(0, DefaultQuad2D::verticesCount, instances);
		}

		Clear();
	}


	template <typename TPresentSurface, typename TRasterizer>
	inline auto FontRenderer<TPresentSurface, TRasterizer>::Clear() -> V
	{
		instances = 0;

		for (auto atlasIdx : usedAtlases)
		{
			auto buffersPtr = atlasIdxToBuffers[atlasIdx];
			buffersPtr->linearTransformsBufferCPU.clear();
			buffersPtr->translationsBufferCPU.clear();
			buffersPtr->uvsBufferCPU.clear();
			buffersPtr->colorsBufferCPU.clear();
		}

		usedAtlases.clear();
	}

	template <typename TPresentSurface, typename TRasterizer>
	inline auto FontRenderer<TPresentSurface, TRasterizer>::AllocateBuffers(U32 atlasIdx) -> B
	{
		auto& buffersPtrRef = atlasIdxToBuffers[atlasIdx];
		buffersPtrRef = new Buffers;
		B status =
			buffersPtrRef->linearTransformsBuffer.Allocate(initialGlyphInstancesCapacity, false) &&
			buffersPtrRef->translationsBuffer.Allocate(initialGlyphInstancesCapacity, false) &&
			buffersPtrRef->colorsBuffer.Allocate(initialGlyphInstancesCapacity, false) &&
			buffersPtrRef->uvsBuffer.Allocate(initialGlyphInstancesCapacity, false);
		return status;
	}


	template <typename TPresentSurface, typename TRasterizer>
	inline auto FontRenderer<TPresentSurface, TRasterizer>::ReallocateBuffers(U32 atlasIdx, U32 newCapacity) -> B
	{
		auto buffersPtr = atlasIdxToBuffers[atlasIdx];
		B status =
			buffersPtr->linearTransformsBuffer.Reallocate(newCapacity, false) &&
			buffersPtr->translationsBuffer.Reallocate(newCapacity, false) &&
			buffersPtr->colorsBuffer.Reallocate(newCapacity, false) &&
			buffersPtr->uvsBuffer.Reallocate(newCapacity, false);
		return status;
	}
}