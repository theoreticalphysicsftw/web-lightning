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

#include <rendering/vector_path.hpp>
#include <common/error.hpp>

namespace WL
{
	template <typename TF>
	struct PathTraversal
	{
		using Scalar = TF;
		using Path2D = Path2D<Scalar>;

		struct ParamValue
		{
			U32 curveIdx;
			Scalar t0;
			Scalar t1;
		};

		PathTraversal(const Path2D& path, Scalar particleLength, Scalar speed, Scalar initialDelay);

		auto Update(Scalar dt) -> Span<const ParamValue>;

		U32 curvesCount;
		Scalar particleLength;
		Scalar speed;
		Scalar initialDelay;
		Scalar currentTime;
		Scalar currentStart;
		StaticArray<ParamValue, 2> currentState;
	};
}


namespace WL
{
	template<typename TF>
	inline PathTraversal<TF>::PathTraversal(const Path2D& paths, Scalar particleLength, Scalar speed, Scalar initialDelay)
	{
		WL_ASSERT(particleLength <= 1.0);
		this->particleLength = particleLength;

		curvesCount = paths.primitives.size();
		this->speed = speed;
		this->initialDelay = initialDelay;
		currentTime = 0;
		currentStart = -particleLength;
	}

	template<typename TF>
	inline auto PathTraversal<TF>::Update(Scalar dt) -> Span<const ParamValue>
	{
		currentTime += dt;

		if (currentTime < initialDelay)
		{
			return {};
		}

		currentStart += dt * speed;

		if (currentStart < 0)
		{
			currentState[0].curveIdx = 0;
			currentState[0].t0 = 0;
			currentState[0].t1 = particleLength + currentStart;

			return Span<ParamValue>(currentState.data(), 1u);
		}

		Scalar totalParamValue = curvesCount;
		auto progress = currentStart / totalParamValue;
		auto startCurveIdx = U32(progress);

		if (startCurveIdx >= curvesCount)
		{
			currentTime = 0;
			currentStart = -particleLength;
			return {};
		}

		auto currentStartParam = Frac(progress);

		currentState[0].curveIdx = startCurveIdx;
		currentState[0].t0 = currentStartParam;
		currentState[0].t1 = Min(Scalar(1), currentStartParam + particleLength);

		auto remainder = currentStartParam + particleLength - 1;

		if (remainder > 0 && startCurveIdx < curvesCount - 1)
		{
			currentState[1].curveIdx = startCurveIdx + 1;
			currentState[1].t0 = 0;
			currentState[1].t1 = remainder;

			return Span<ParamValue>(currentState.data(), 2u);
 		}
		else
		{
			return Span<ParamValue>(currentState.data(), 1u);
		}

		return {};
	}
}