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

#include <algebra/algebra.hpp>
#include <algebra/bezier.hpp>
#include <algebra/line.hpp>
#include <rendering/color.hpp>


namespace WL
{
	template <typename TF>
	struct Path2D
	{
		using Scalar = TF;
		using QuadraticBezier = QuadraticBezier<Scalar, 2>;
		using CubicBezier = CubicBezier<Scalar, 2>;
		using Line = Line<Scalar, 2>;
		using Primitive = Variant<QuadraticBezier, CubicBezier, Line>;

		ColorU32 fillColor;
		ColorU32 outlineColor;
		F32 outlineWidth;
		F32 outlineFeather = 2.f;
		F32 outlineFeatherBegin = outlineFeather;
		F32 outlineFeatherEnd = outlineFeather;
		B outlined = false;
		B filled = true;
		B closed = false;

		auto CloneWithoutPrimitives() const -> Path2D;

		Array<Primitive> primitives;
	};
}


namespace WL
{
	template <typename TF>
	inline auto Path2D<TF>::CloneWithoutPrimitives() const -> Path2D
	{
		Path2D result;
		result.fillColor = fillColor;
		result.filled = filled;
		result.closed = closed;
		result.outlineColor = outlineColor;
		result.outlined = outlined;
		result.outlineWidth = outlineWidth;
		return result;
	}
}