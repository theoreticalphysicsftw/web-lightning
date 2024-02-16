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
#include <algebra/arc_length.hpp>
#include <common/error.hpp>
#include <common/utilities.hpp>


namespace WL
{
	template <typename TF>
	struct PathTraversal
	{
		using Scalar = TF;
		using Path2D = Path2D<Scalar>;
		using Line = Line<Scalar, 2>;
		using Quadratic = QuadraticBezier<Scalar, 2>;
		using Cubic = CubicBezier<Scalar, 2>;
		using LengthReparametrization = Variant<ArcLengthReparametrization<Line>, ArcLengthReparametrization<Quadratic>, ArcLengthReparametrization<Cubic>>;

		struct ParamValue
		{
			U32 curveIdx;
			Scalar t0;
			Scalar t1;
		};

		PathTraversal(const Path2D& path, Scalar particleLength, Scalar speed, Scalar initialDelay);

		auto Update(Scalar dt) -> Span<const ParamValue>;

		Array<Scalar> cumulativeLength;
		Array<LengthReparametrization> reparametrizations;
		Scalar particleLength;
		Scalar speed;
		Scalar initialDelay;
		Scalar currentTime;
		Scalar currentStart;
		U32 currentStartCurveIdx;
		Array<ParamValue> currentState;
	#ifdef WL_DEBUG
		const Path2D* pathPtr;
	#endif
	};
}


namespace WL
{
	template<typename TF>
	inline PathTraversal<TF>::PathTraversal(const Path2D& path, Scalar particleLength, Scalar speed, Scalar initialDelay)
	{
		this->particleLength = particleLength;

	#ifdef WL_DEBUG
		pathPtr = &path;
	#endif

		for (auto& prim : path.primitives)
		{
			Visit<V>
			(
				[&](const auto& prim) -> V
				{
					reparametrizations.emplace_back(prim);
				},
				prim
			);
			
			Visit<V>
			(
				[&](const auto& prim) -> V
				{
					cumulativeLength.push_back(prim.GetMaxArcLength());
				},
				reparametrizations.back()
			);
			
		}

		for (auto i = 1; i < cumulativeLength.size(); ++i)
		{
			cumulativeLength[i] += cumulativeLength[i - 1];
		}

		this->speed = speed;
		this->initialDelay = initialDelay;
		currentTime = 0;
		currentStartCurveIdx = 0;
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

		auto totalLength = cumulativeLength.back();

		if (currentStart >= totalLength)
		{
			currentTime = 0;
			currentStartCurveIdx = 0;
			currentStart = -particleLength;
			return {};
		}

		// Most of the time the particle doesn't move far (< 2 curves forwards), so
		// use linear search instead of binary.
		for (auto i = 0; i < cumulativeLength.size(); ++i)
		{
			if (currentStart < cumulativeLength[i])
			{
				currentStartCurveIdx = i;
				break;
			}
		}

		auto currentEnd = particleLength + currentStart;

		if (currentEnd < 0)
		{
			return {};
		}

		auto currentEndCurveIdx = cumulativeLength.size() - 1;

		for (auto i = currentStartCurveIdx; i < cumulativeLength.size(); ++i)
		{
			if (currentEnd < cumulativeLength[i])
			{
				currentEndCurveIdx = i;
				break;
			}
		}

		auto curveSpan = currentEndCurveIdx - currentStartCurveIdx + 1;
		currentState.resize(curveSpan);

		auto currentStartInCurve = (currentStartCurveIdx > 0) ? currentStart - cumulativeLength[currentStartCurveIdx - 1] : currentStart;
		auto currentEndInCurve = (currentEndCurveIdx > 0) ? currentEnd - cumulativeLength[currentEndCurveIdx - 1] : currentEnd;
	
		currentState[0].curveIdx = currentStartCurveIdx;
		
		Visit<V>
		(
			[&](const auto& reparam) -> V
			{
				currentState[0].t0 = reparam.GetParameterValue(currentStartInCurve);
				currentState[0].t1 = (currentEndCurveIdx > currentStartCurveIdx) ? Scalar(1) : reparam.GetParameterValue(currentEndInCurve);
			},
			reparametrizations[currentStartCurveIdx]
		);

		// The middle ones span the whole curves.
		for (auto i = currentStartCurveIdx + 1; i < currentEndCurveIdx; ++i)
		{
			auto stateIdx = i - currentStartCurveIdx;
			currentState[stateIdx].curveIdx = i;
			currentState[stateIdx].t0 = Scalar(0);
			currentState[stateIdx].t1 = Scalar(1);
		}

		if (currentEndCurveIdx > currentStartCurveIdx)
		{
			auto stateIdx = curveSpan - 1;
			currentState[stateIdx].curveIdx = currentEndCurveIdx;
			currentState[stateIdx].t0 = Scalar(0);
			Visit<V>
			(
				[&](const auto& reparam) -> V
				{
					currentState[stateIdx].t1 = reparam.GetParameterValue(currentEndInCurve);
				},
				reparametrizations[currentEndCurveIdx]
			);
			
		}

	#ifdef WL_DEBUG
		auto sum = Scalar(0);
		for (auto i = 0; i < currentState.size(); ++i)
		{
			Visit<V>
			(
				[&](const auto& prim) -> V
				{
					sum += GetArcLength(prim, currentState[i].t0, currentState[i].t1);
				},
				pathPtr->primitives[currentState[i].curveIdx]
			);
		}
		WL_ASSERT(particleLength > sum || Abs(sum - particleLength) < Scalar(0.01 * particleLength));
	#endif

		return {currentState.data(), curveSpan};
	}
}