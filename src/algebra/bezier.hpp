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

#include <common/utilities.hpp>

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

		auto GetPolynomialCoefficients() const -> StaticArray<Vec, 3>;
		auto EvaluateAt(Scalar t) const -> Vec;
		auto Split(Scalar t) const -> Pair<QuadraticBezier, QuadraticBezier>;
		auto GetCentroid() const -> Vec;
		auto GetSquaredDistanceFrom(const Vec& p) const -> TF;
		auto GetDistanceFrom(const Vec& p) const -> TF;
	
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
		auto Split(Scalar t) const -> Pair<CubicBezier, CubicBezier>;
		// TODO: Implement.
		// auto GetSquaredDistanceFrom(const Vec& p) const->TF;
		// auto GetDistanceFrom(const Vec& p) const->TF;
		// auto GetPolynomialCoefficients() const->StaticArray<Vec, 4>;
	};

	template <typename TF>
	auto ApproximateByQuadratics(const CubicBezier<TF, 2>& cubic, TF tolerance = 0.001) -> Array<QuadraticBezier<TF, 2>>;
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
		auto tmp0 = Lerp(p0, p1, t);
		auto tmp1 = Lerp(p1, p2, t);
		return Lerp(tmp0, tmp1, t);
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
		auto tmp0 = Lerp(p0, p1, t);
		auto tmp1 = Lerp(p1, p2, t);
		auto tmp2 = Lerp(p2, p3, t);

		auto tmp3 = Lerp(tmp0, tmp1, t);
		auto tmp4 = Lerp(tmp1, tmp2, t);

		return Lerp(tmp3, tmp4, t);
	}
	

	template<typename TF, U32 Dim>
	inline auto QuadraticBezier<TF, Dim>::Split(Scalar t) const -> Pair<QuadraticBezier, QuadraticBezier>
	{
		Vec b[3][3];

		for (auto i = 0u; i < 3; ++i)
		{
			b[0][i] = points[i];
		}

		for (auto i = 1u; i < 3u; ++i)
		{
			for (auto j = 0u; j < (3 - i); ++j)
			{
				b[i][j] = (1 - t) * b[i - 1][j] + t * b[i - 1][j + 1];
			}
		}

		return MakePair(QuadraticBezier(b[0][0], b[1][0], b[2][0]), QuadraticBezier(b[2][0], b[1][1], b[0][2]));
	}


	template<typename TF, U32 Dim>
	inline auto CubicBezier<TF, Dim>::GetCentroid() const -> Vec
	{
		return (p0 + p1 + p2 + p3) / Scalar(4);
	}


	template<typename TF, U32 Dim>
	inline auto CubicBezier<TF, Dim>::Split(Scalar t) const -> Pair<CubicBezier, CubicBezier>
	{
		Vec b[4][4];

		for (auto i = 0u; i < 4; ++i)
		{
			b[0][i] = points[i];
		}

		for (auto i = 1u; i < 4u; ++i)
		{
			for (auto j = 0u; j < (4 - i); ++j)
			{
				b[i][j] = (1 - t) * b[i - 1][j] + t * b[i - 1][j + 1];
			}
		}

		return MakePair(CubicBezier(b[0][0], b[1][0], b[2][0], b[3][0]), CubicBezier(b[3][0], b[2][1], b[1][2], b[0][3]));
	}


	template<typename TF>
	inline auto ApproximateByQuadratics(const CubicBezier<TF, 2>& cubic, TF tolerance) -> Array<QuadraticBezier<TF, 2>>
	{
		Array<QuadraticBezier<TF, 2>> result;
		Array<CubicBezier<TF, 2>> stack;
		stack.push_back(cubic);

		while (!stack.empty())
		{
			auto c = stack.back();
			stack.pop_back();

			// Find the tangent intersection.
			auto t0 = c.p1 - c.p0;
			auto t1 = c.p3 - c.p2;
			auto b = c.p3 - c.p0;

			// We need to solve the equation t0 * x0 - t1 * x1 = b or in matrix form
			// Mat2x2(t0[0], -t1[0], t0[1], -t1[1]) * Vec2(x0, x1) = b
			// Use Cramer's rule.
			auto det = -t0[0] * t1[1] + t1[0] * t0[1];
			auto x0 = (-b[0] * t1[1] + t1[0] * b[1]) / det;
			auto intersection = c.p0 + x0 * t0;

			QuadraticBezier<TF, 2> approx(c.p0, intersection, c.p3);

			auto halfValueCubic = c.EvaluateAt(TF(0.5));
			auto halfValueQuadratic = approx.EvaluateAt(TF(0.5));

			if ((halfValueCubic - halfValueQuadratic).Length() < tolerance)
			{
				// Make that the triangle is CCW.
				auto dir0 = approx.p1 - approx.p0;
				auto dir1 = approx.p2 - approx.p1;
				if (HodgeDualWedge(dir0, dir1) < 0)
				{
					Swap(approx.p0, approx.p2);
				}
				result.push_back(approx);
			}
			else
			{
				auto split = c.Split(TF(0.5));
				stack.push_back(split.second);
				stack.push_back(split.first);
			}
		}

		return result;
	}
}