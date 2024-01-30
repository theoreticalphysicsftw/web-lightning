// MIT License
// 
// Copyright (c) 2023 - 2024 Mihail Mladenov
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
#include <common/utilities.hpp>

#include <rendering/color.hpp>
#include <rendering/textured_quad_renderer.hpp>

#include <algebra/algebra.hpp>

#include <gpu_api/shader.hpp>
#include <gpu_api/pso.hpp>
#include <shaders/common.hpp>

#include "quad.hpp"

namespace WL
{
	template <typename TPresentSurface>
	class BoxRenderer
	{
	public:
		using PresentSurface = TPresentSurface;
		using GPUAPI = PresentSurface::GPUAPI;
		using TexturedQuadRenderer = TexturedQuadRenderer<PresentSurface>;
		using QuadDesc = TexturedQuadRenderer::QuadDesc;
		using Buffer = GPUAPI::Buffer;
		using Image = GPUAPI::Image;
		using Pso = GPUAPI::Pso;

		static auto Init() -> B;
		static auto AccumulateBox(const QuadDesc& desc) -> V;
		static auto CommitDrawCommands() -> V;
		static auto Clear() -> V;

		static constexpr U32 initialBoxInstancesCapacity = 1024;

	private:
		static auto AllocateBuffers() -> B;
		static auto ReallocateBuffers(U32 newCapacity) -> B;

		inline static TexturedQuadRenderer texturedQuadRenderer;

		inline static U32 instanceCapacity = initialBoxInstancesCapacity;
		inline static U32 instances = 0;

		inline static Pso pso;
		inline static Buffer positionsBuffer;
		inline static Buffer linearTransformsBuffer;
		inline static Buffer translationsBuffer;
		inline static Buffer colorsBuffer;
		inline static Buffer radiusesBuffer;

		inline static Array<Mat2x2> linearTransformsBufferCPU;
		inline static Array<Vec2> translationsBufferCPU;
		inline static Array<ColorU32> colorsBufferCPU;
		inline static Array<F32> radiusesBufferCPU;

		inline static Map<const Image*, Array<QuadDesc>> texturedDescs;
	};
}


namespace WL
{
	template<typename TPresentSurface>
	inline auto BoxRenderer<TPresentSurface>::Init() -> B
	{
		if (!(texturedQuadRenderer.Init() && AllocateBuffers()))
		{
			return false;
		}
		positionsBuffer.Update((const Byte*)DefaultQuad2D::vertices, sizeof(DefaultQuad2D::vertices));

		pso.AddShader(BoxVert.data(), BoxVert.size(), EShaderType::Vertex);
		pso.AddShader(BoxFrag.data(), BoxFrag.size(), EShaderType::Fragment);
		
		pso.AddVBLayout({ .binding = 0, .type = EType::Float, .components = 2 });
		pso.AddVBLayout({ .binding = 1, .type = EType::Float, .components = 4 });
		pso.AddVBLayout({ .binding = 2, .type = EType::Float, .components = 2 });
		pso.AddVBLayout({ .binding = 3, .type = EType::Uint, .components = 1 });
		pso.AddVBLayout({ .binding = 4, .type = EType::Float, .components = 1 });
		pso.AddSimpleConstant(0, EType::Float, "uScreenDims");

		return pso.Compile();
	}


	template<typename TPresentSurface>
	inline auto BoxRenderer<TPresentSurface>::AccumulateBox(const QuadDesc& desc) -> V
	{
		if (desc.textured)
		{
			texturedDescs[desc.texture].push_back(desc);
		}
		else
		{
			instances++;
			if (instances > instanceCapacity)
			{
				ReallocateBuffers(instanceCapacity * 2);
				instanceCapacity *= 2;
			}

			colorsBufferCPU.emplace_back(desc.color);
			radiusesBufferCPU.emplace_back(desc.radius);
			linearTransformsBufferCPU.emplace_back(desc.width / 2.f, 0.f, 0.f, desc.height / 2.f);
			translationsBufferCPU.emplace_back(desc.offsetX, desc.offsetY);
		}
	}


	template<typename TPresentSurface>
	inline auto BoxRenderer<TPresentSurface>::CommitDrawCommands() -> V
	{
		if (!instances && texturedDescs.empty())
		{
			return;
		}

		colorsBuffer.Update(colorsBufferCPU);
		radiusesBuffer.Update(radiusesBufferCPU);
		linearTransformsBuffer.Update(linearTransformsBufferCPU);
		translationsBuffer.Update(translationsBufferCPU);

		pso.Use();
		pso.BindVB(0, positionsBuffer);
		pso.BindVB(1, linearTransformsBuffer, true);
		pso.BindVB(2, translationsBuffer, true);
		pso.BindVB(3, colorsBuffer, true);
		pso.BindVB(4, radiusesBuffer, true);


		pso.UpdateConstant(0, PresentSurface::GetDimensions());

		pso.DrawInstanced(0, DefaultQuad2D::verticesCount, instances);
		// Commit textured quads with the same texture to abuse instancing
		// and reduce the number of draw calls.
		for (auto& descsArray : texturedDescs)
		{
			for (auto& desc : descsArray.second)
			{
				texturedQuadRenderer.AccumulateQuad(desc);
			}
			texturedQuadRenderer.CommitDrawCommands();
		}

		Clear();
	}


	template<typename TPresentSurface>
	inline auto BoxRenderer<TPresentSurface>::AllocateBuffers() -> B
	{
		B status =
			positionsBuffer.Allocate(DefaultQuad2D::verticesCount) &&
			linearTransformsBuffer.Allocate(initialBoxInstancesCapacity, false) &&
			translationsBuffer.Allocate(initialBoxInstancesCapacity, false) &&
			colorsBuffer.Allocate(initialBoxInstancesCapacity, false) &&
			radiusesBuffer.Allocate(initialBoxInstancesCapacity, false);
		return status;
	}


	template<typename TPresentSurface>
	inline auto BoxRenderer<TPresentSurface>::ReallocateBuffers(U32 newCapacity) -> B
	{
		B status =
			linearTransformsBuffer.Reallocate(newCapacity, false) &&
			translationsBuffer.Reallocate(newCapacity, false) &&
			colorsBuffer.Reallocate(newCapacity, false) &&
			radiusesBuffer.Reallocate(newCapacity, false);
		return status;
	}


	template<typename TPresentSurface>
	inline auto BoxRenderer<TPresentSurface>::Clear() -> V
	{
		instances = 0;
		colorsBufferCPU.clear();
		radiusesBufferCPU.clear();
		linearTransformsBufferCPU.clear();
		translationsBufferCPU.clear();
		texturedDescs.clear();
	}
}