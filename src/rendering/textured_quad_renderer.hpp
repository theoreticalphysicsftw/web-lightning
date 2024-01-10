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
#include <common/utilities.hpp>
#include <rendering/color.hpp>
#include <algebra/algebra.hpp>

#include <gpu_api/shader.hpp>
#include <gpu_api/pso.hpp>
#include <shaders/common.hpp>

#include "quad.hpp"

namespace WL
{
	template <typename TPresentSurface>
	class TexturedQuadRenderer
	{
	public:
		using PresentSurface = TPresentSurface;
		using GPUAPI = PresentSurface::GPUAPI;
		using Image = GPUAPI::Image;
		using Buffer = GPUAPI::Buffer;
		using Pso = GPUAPI::Pso;
		using QuadDesc = QuadDesc2D<GPUAPI>;
		auto Init() -> B;
		auto AccumulateQuad(const QuadDesc& desc) -> V;
		auto CommitDrawCommands() -> V;
		auto Clear() -> V;

		static constexpr U32 initialQuadInstancesCapacity = 1024;

	private:
		auto AllocateBuffers() -> B;
		auto ReallocateBuffers(U32 newCapacity) -> B;

		U32 instanceCapacity = initialQuadInstancesCapacity;
		U32 instances = 0;

		const Image* texture;
		Pso pso;
		Buffer linearTransformsBuffer;
		Buffer translationsBuffer;
		Buffer uvsBuffer;
		Buffer radiusesBuffer;

		Array<Mat2x2> linearTransformsBufferCPU;
		Array<Vec2> translationsBufferCPU;
		Array<Vec4> uvsBufferCPU;
		Array<F32> radiusesBufferCPU;
	};
}


namespace WL
{
	template<typename TPresentSurface>
	inline auto TexturedQuadRenderer<TPresentSurface>::Init() -> B
	{
		if (!AllocateBuffers())
		{
			return false;
		}

		pso.AddShader(TexturedQuadVert.data(), TexturedQuadVert.size(), EShaderType::Vertex);
		pso.AddShader(TexturedQuadFrag.data(), TexturedQuadFrag.size(), EShaderType::Fragment);

		pso.AddVBLayout({ .binding = 0, .type = EType::Float, .components = 4 });
		pso.AddVBLayout({ .binding = 1, .type = EType::Float, .components = 2 });
		pso.AddVBLayout({ .binding = 2, .type = EType::Float, .components = 4 });
		pso.AddVBLayout({ .binding = 3, .type = EType::Float, .components = 1 });
		pso.AddSimpleConstant(0, EType::Float, "uScreenDims");
		pso.AddTexture(0, "uTexture0");

		return pso.Compile();
	}


	template<typename TPresentSurface>
	inline auto TexturedQuadRenderer<TPresentSurface>::AccumulateQuad(const QuadDesc& desc) -> V
	{
		instances++;
		if (instances > instanceCapacity)
		{
			ReallocateBuffers(instanceCapacity * 2);
			instanceCapacity *= 2;
		}

		uvsBufferCPU.emplace_back(desc.u0, desc.v0, desc.u1, desc.v1);
		radiusesBufferCPU.emplace_back(desc.radius);
		linearTransformsBufferCPU.emplace_back(desc.width / 2.f, 0.f, 0.f, desc.height / 2.f);
		translationsBufferCPU.emplace_back(desc.offsetX, desc.offsetY);
		texture = desc.texture;
	}


	template<typename TPresentSurface>
	inline auto TexturedQuadRenderer<TPresentSurface>::CommitDrawCommands() -> V
	{
		if (!instances)
		{
			return;
		}

		uvsBuffer.Update(uvsBufferCPU);
		radiusesBuffer.Update(radiusesBufferCPU);
		linearTransformsBuffer.Update(linearTransformsBufferCPU);
		translationsBuffer.Update(translationsBufferCPU);

		pso.BindVB(0, linearTransformsBuffer, true);
		pso.BindVB(1, translationsBuffer, true);
		pso.BindVB(2, uvsBuffer, true);
		pso.BindVB(3, radiusesBuffer, true);

		pso.Use();
		pso.UpdateConstant(0, PresentSurface::GetDimensions());
		pso.BindTexture(*texture);

		pso.DrawInstanced(0, DefaultQuad2D::verticesCount, instances);

		Clear();
	}


	template<typename TPresentSurface>
	inline auto TexturedQuadRenderer<TPresentSurface>::AllocateBuffers() -> B
	{
		B status =
			linearTransformsBuffer.Allocate(initialQuadInstancesCapacity, false) &&
			translationsBuffer.Allocate(initialQuadInstancesCapacity, false) &&
			uvsBuffer.Allocate(initialQuadInstancesCapacity, false) &&
			radiusesBuffer.Allocate(initialQuadInstancesCapacity, false);
		return status;
	}


	template<typename TPresentSurface>
	inline auto TexturedQuadRenderer<TPresentSurface>::ReallocateBuffers(U32 newCapacity) -> B
	{
		B status =
			linearTransformsBuffer.Reallocate(newCapacity, false) &&
			translationsBuffer.Reallocate(newCapacity, false) &&
			uvsBuffer.Reallocate(newCapacity, false) &&
			radiusesBuffer.Reallocate(newCapacity, false);
		return status;
	}


	template<typename TPresentSurface>
	inline auto TexturedQuadRenderer<TPresentSurface>::Clear() -> V
	{
		instances = 0;
		uvsBufferCPU.clear();
		radiusesBufferCPU.clear();
		linearTransformsBufferCPU.clear();
		translationsBufferCPU.clear();
	}
}