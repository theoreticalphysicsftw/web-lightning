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

#include "algebra.hpp"

namespace WL
{
	template <typename TF, U32 Dim>
	struct QuadraticBezier
	{
		using Scalar = TF;
		using Vec = Vector<TF, Dim>;

		static constexpr U32 dimension = Dim;

		union
		{
			struct
			{
				Vec p0;
				Vec p1;
				Vec p2;
			};
			StaticArray<Vec, 3> points;
		};

		QuadraticBezier(const Vec& p0, const Vec& p1, const Vec& p2) : p0(p0), p1(p1), p2(p2) {}

		auto GetPolynomialCoefficients() const->StaticArray<Vec, 3>;
		auto EvaluateAt(Scalar t) const->Vec;
		auto GetCentroid() const->Vec;
		auto GetSquaredDistanceFrom(const Vec& p) const->TF;
		auto GetDistanceFrom(const Vec& p) const->TF;
	};


	template <typename TF, U32 Dim>
	struct CubicBezier
	{
		using Scalar = TF;
		using Vec = Vector<TF, Dim>;

		static constexpr U32 dimension = Dim;

		union
		{
			struct
			{
				Vec p0;
				Vec p1;
				Vec p2;
				Vec p3;
			};
			StaticArray<Vec, 4> points;
		};

		CubicBezier(const Vec& p0, const Vec& p1, const Vec& p2, const Vec& p3) : p0(p0), p1(p1), p2(p2), p3(p3) {}


		auto EvaluateAt(Scalar t) const -> Vec;
		auto GetCentroid() const -> Vec;
		// TODO: Implement.
		// auto GetSquaredDistanceFrom(const Vec& p) const->TF;
		// auto GetDistanceFrom(const Vec& p) const->TF;
		// auto GetPolynomialCoefficients() const->StaticArray<Vec, 4>;
	};
}


namespace WL
{
	template<typename TF, U32 Dim>
	inline auto QuadraticBezier<TF, Dim>::GetPolynomialCoefficients() const -> StaticArray<Vec, 3>
	{
		StaticArray<Vec, 3> result =
		{
			p0 - p1 * Scalar(2) + p2,
			(p1 - p0) * Scalar(2),
			p0
		};
		return result;
	}


	template<typename TF, U32 Dim>
	inline auto QuadraticBezier<TF, Dim>::EvaluateAt(Scalar t) const -> Vec
	{
		return (Scalar(1) - t) * (Scalar(1) - t) * p0 + Scalar(2) * (Scalar(1) - t) * t * p1 + t * t * p2;
	}


	template<typename TF, U32 Dim>
	inline auto QuadraticBezier<TF, Dim>::GetCentroid() const -> Vec
	{
		return (p0 + p1 + p2) / Scalar(3);
	}


	template<typename TF, U32 Dim>
	inline auto QuadraticBezier<TF, Dim>::GetSquaredDistanceFrom(const Vec& p) const -> TF
	{
		// Polinomial coefficients of the direction from the point
		auto coefficients = GetPolynomialCoefficients();
		coefficients[2] = coefficients[2] - p;

		// Now get the polynomial coefficients of the direction dotted with the derivative
		auto a = TF(2) * coefficients[0].Dot(coefficients[0]);
		auto b = TF(3) * coefficients[0].Dot(coefficients[1]);
		auto c = TF(2) * coefficients[0].Dot(coefficients[2]) + coefficients[1].Dot(coefficients[1]);
		auto d = coefficients[1].Dot(coefficients[2]);

		auto roots = SolveCubic(a, b, c, d);

		U32 testPointsCount = 0u;
		StaticArray<Vec, 5> testPoints;
		for (auto i = 0; i < 3; ++i)
		{
			if (roots[i] >= TF(0) && roots[i] <= TF(1))
			{
				testPoints[testPointsCount++] = EvaluateAt(roots[i]);
			}
		}
		testPoints[testPointsCount++] = p0;
		testPoints[testPointsCount++] = p2;

		auto minSqDist = SquaredDistance(testPoints[0], p);
		for (auto i = 1u; i < testPointsCount; ++i)
		{
			minSqDist = Min(minSqDist, SquaredDistance(testPoints[i], p));
		}

		return minSqDist;
	}


	template<typename TF, U32 Dim>
	inline auto QuadraticBezier<TF, Dim>::GetDistanceFrom(const Vec& p) const -> TF
	{
		return Sqrt(GetSquaredDistanceFrom(p));
	}



	template<typename TF, U32 Dim>
	inline auto CubicBezier<TF, Dim>::EvaluateAt(Scalar t) const -> Vec
	{
		auto oneMinusT = (Scalar(1) - t);
		auto oneMinusTSq = oneMinusT * oneMinusT;
		auto oneMinusTCb = oneMinusTSq * oneMinusT;
		auto tSq = t * t;
		auto tCb = tSq * t;
		return oneMinusTCb * p0 + Scalar(3) * oneMinusTSq * t * p1 + Scalar(3) * oneMinusT * tSq * p2 + tCb * p3;
	}


	template<typename TF, U32 Dim>
	inline auto CubicBezier<TF, Dim>::GetCentroid() const -> Vec
	{
		return (p0 + p1 + p2 + p3) / Scalar(4);
	}

}