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


namespace WL
{
	template <typename TPrimitive>
	struct ArcLengthReparametrization
	{
		using Primitive = TPrimitive;
		using Scalar = Primitive::Scalar;
		ArcLengthReparametrization(const Primitive& prim, U32 lutSize = 128, U32 maxSubdivisions = 1024);

		auto GetParameterValue(Scalar arcLength) -> Scalar;

		Array<Scalar> parameterLUT;
	};
}


namespace WL
{
	template<typename TPrimitive>
	inline ArcLengthReparametrization<TPrimitive>::ArcLengthReparametrization(const Primitive& prim, U32 lutSize, U32 maxSubDivisions)
	{
		Array<Scalar> cumulativeLength;

		Scalar sum = 0;
		auto scale = Scalar(1) / Scalar(maxSubDivisions);
		auto currentPoint = prim.p0;
		cumulativeLength.push_back(sum);

		for (auto i = 0u; i < maxSubDivisions; ++i)
		{
			auto nextParam = (i + 1) * scale;
			auto nextPoint = prim.EvaluateAt(nextParam);
			auto length = (nextPoint - currentPoint).Length();
			sum += length;
			cumulativeLength.push_back(sum);
			currentPoint = nextPoint;
		}

		auto maxArcLength = cumulativeLength[maxSubDivisions];

		auto lutScale = maxArcLength / (lutSize - 1);
		parameterLUT.push_back(0);
		for (auto i = 0u; i < (lutSize - 1); ++i)
		{
			auto arcLength = (i + 1) * lutScale;
			auto idx1 = LowerBound(cumulativeLength, arcLength);
			auto idx0 = idx1 - 1;
			auto t1 = idx1 * scale;
			auto t0 = idx0 * scale;
			auto s0 = cumulativeLength[idx0];
			auto s1 = cumulativeLength[idx1];
			parameterLUT.push_back(Lerp(t0, t1, ((arcLength - s0) / (s1 - s0))));
		}
	}


	template<typename TPrimitive>
	inline auto ArcLengthReparametrization<TPrimitive>::GetParameterValue(Scalar arcLength) -> Scalar
	{
		auto maxDivisions = parameterLUT.size() - 1;
		auto lutScale = parameterLUT[maxDivisions] / maxDivisions;
		auto q = arcLength / lutScale;
		auto bucket = U32(q);

		if (bucket == maxDivisions)
		{
			return Scalar(1);
		}

		return Lerp(parameterLUT[bucket], parameterLUT[bucket + 1], Frac(q));
	}
}