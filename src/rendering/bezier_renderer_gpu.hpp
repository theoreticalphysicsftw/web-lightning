// MIT License
// 
// Copyright (c) 2024 Mihail Mladenov
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

#include "bezier_renderer.hpp"

#include <algebra/bezier.hpp>
#include <algebra/vector.hpp>


namespace WL
{
	template <typename TPresentSurface>
	class BezierRendererGPU : public BezierRenderer<BezierRendererGPU<TPresentSurface>>
	{
	public:
		using PresentSurface = TPresentSurface;
		using GPUAPI = PresentSurface::GPUAPI;
		using Buffer = GPUAPI::Buffer;
		using Image = GPUAPI::Image;
		using Pso = GPUAPI::Pso;
		using QuadraticBezier = QuadraticBezier<F32, 2>;

		static auto Init() -> B;
		static auto AccumulateBezier(const QuadraticBezier& curve, ColorU32 color, F32 width, F32 feather = 1.0f) -> V;
		static auto CommitDrawCommands() -> V;
		static auto Clear() -> V;

		static constexpr U32 initialCurveInstancesCapacity = 1024;

	private:
		static auto AllocateBuffers() -> B;
		static auto ReallocateBuffers(U32 newCapacity) -> B;
		static auto PackPoint(Vec2 point) -> U32;

		inline static U32 instanceCapacity = initialCurveInstancesCapacity;
		inline static U32 instances = 0;

		inline static Pso pso;
		inline static Buffer packedPointsAndColorBuffer;
		inline static Buffer widthAndFeatherBuffer;

		inline static Array<UVec4> packedPointsAndColorBufferCPU;
		inline static Array<Vec2> widthAndFeatherBufferCPU;
	};
}


namespace WL
{
	template<typename TPresentSurface>
	inline auto BezierRendererGPU<TPresentSurface>::Init() -> B
	{
		if (!AllocateBuffers())
		{
			return false;
		}

		pso.AddShader(BezierApproxVert.data(), BezierApproxVert.size(), EShaderType::Vertex);
		pso.AddShader(BezierApproxFrag.data(), BezierApproxFrag.size(), EShaderType::Fragment);

		pso.AddVBLayout({ .binding = 0, .type = EType::Uint, .components = 4 });
		pso.AddVBLayout({ .binding = 1, .type = EType::Float, .components = 2 });
		pso.AddSimpleConstant(0, EType::Float, "uScreenDims");

		return pso.Compile();
	}


	template<typename TPresentSurface>
	inline auto BezierRendererGPU<TPresentSurface>::AccumulateBezier(const QuadraticBezier& curve, ColorU32 color, F32 width, F32 feather) -> V
	{
		instances++;
		if (instances > instanceCapacity)
		{
			ReallocateBuffers(instanceCapacity * 2);
			instanceCapacity *= 2;
		}

		auto p0 = PackPoint(curve.p0);
		auto p1 = PackPoint(curve.p1);
		auto p2 = PackPoint(curve.p2);


		// Make that the triangle is CCW.
		auto dir0 = curve.p2 - curve.p1;
		auto dir1 = curve.p1 - curve.p0;
		if (HodgeDualWedge(dir0, dir1) < 0)
		{
			Swap(p0, p2);
		}

		packedPointsAndColorBufferCPU.emplace_back(p0, p1, p2, color.packed);
		widthAndFeatherBufferCPU.emplace_back(width, feather);
	}


	template<typename TPresentSurface>
	inline auto BezierRendererGPU<TPresentSurface>::PackPoint(Vec2 point) -> U32
	{
		auto x = U32(Abs(point[0]) * 0x7FFFu) | (Sign(point[0]) << 15);
		auto y = U32(Abs(point[1]) * 0x7FFFu) | (Sign(point[1]) << 15);

		return x | (y << 16);
	}


	template<typename TPresentSurface>
	inline auto BezierRendererGPU<TPresentSurface>::CommitDrawCommands() -> V
	{
		if (!instances)
		{
			return;
		}

		packedPointsAndColorBuffer.Update(packedPointsAndColorBufferCPU);
		widthAndFeatherBuffer.Update(widthAndFeatherBufferCPU);

		pso.Use();
		pso.BindVB(0, packedPointsAndColorBuffer, true);
		pso.BindVB(1, widthAndFeatherBuffer, true);

		pso.UpdateConstant(0, PresentSurface::GetDimensions());

		pso.DrawInstanced(0, 3, instances);

		Clear();
	}


	template<typename TPresentSurface>
	inline auto BezierRendererGPU<TPresentSurface>::AllocateBuffers() -> B
	{
		B status =
			packedPointsAndColorBuffer.Allocate(initialCurveInstancesCapacity, false) &&
			widthAndFeatherBuffer.Allocate(initialCurveInstancesCapacity, false);
		return status;
	}


	template<typename TPresentSurface>
	inline auto BezierRendererGPU<TPresentSurface>::ReallocateBuffers(U32 newCapacity) -> B
	{
		B status =
			packedPointsAndColorBuffer.Reallocate(initialCurveInstancesCapacity, false) &&
			widthAndFeatherBuffer.Reallocate(initialCurveInstancesCapacity, false);
		return status;
	}


	template<typename TPresentSurface>
	inline auto BezierRendererGPU<TPresentSurface>::Clear() -> V
	{
		instances = 0;
		packedPointsAndColorBufferCPU.clear();
		widthAndFeatherBufferCPU.clear();
	}
}