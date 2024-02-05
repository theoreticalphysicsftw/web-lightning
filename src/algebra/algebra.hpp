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

#include "vector.hpp"
#include "matrix.hpp"
#include "arithmetic.hpp"


namespace WL
{
    using Vec2 = Vector<F32, 2>;
    using Vec3 = Vector<F32, 3>;
    using Vec4 = Vector<F32, 4>;

    using UVec2 = Vector<U32, 2>;
    using UVec3 = Vector<U32, 3>;
    using UVec4 = Vector<U32, 4>;

    using IVec2 = Vector<I32, 2>;
    using IVec3 = Vector<I32, 3>;
    using IVec4 = Vector<I32, 4>;

    using Mat2x2 = Matrix<F32, 2, 2>;
    using Mat3x3 = Matrix<F32, 3, 3>;
    using Mat4x4 = Matrix<F32, 4, 4>;

    template<typename TF>
    auto SolveCubic(TF a, TF b, TF c, TF d) -> StaticArray<TF, 3>;
}


namespace WL
{
    template<typename TF>
    auto SolveCubic(TF a, TF b, TF c, TF d) -> StaticArray<TF, 3>
    {
        static constexpr TF tolerance = TF(1) * Limits<TF>::epsilon();
        WL_ASSERT(a > tolerance);
        auto nan = Limits<TF>::quiet_NaN();
        StaticArray<TF, 3> roots =
        {
            nan,
            nan,
            nan
        };
        b = b / a;
        c = c / a;
        d = d / a;
        a = 1.f;
        // Transform into a depressed cubic.
        auto bSq = b * b;
        auto p = c - bSq / TF(3);
        auto q = (TF(2) * b * bSq - TF(9) * b * c) / TF(27) + d;

        auto delta = p * p * p / TF(27) + q * q / TF(4);

        if (delta > tolerance)
        {
            auto minusHalfQ = q / (-2.f);
            auto sqrtDelta = Sqrt(delta);
            roots[0] = Cbrt(minusHalfQ + sqrtDelta) + Cbrt(minusHalfQ - sqrtDelta);
        }
        else if (delta > -tolerance)
        {
            if (p > -tolerance && p < tolerance)
            {
                roots[0] = TF(0);
                roots[1] = TF(0);
                roots[2] = TF(0);
            }
            else
            {
                roots[0] = TF(3) * q / p;
                roots[1] = TF(-0.5) * roots[0];
                roots[2] = roots[1];
            }
        }
        else
        {
            // Use Viete trigonometric formula.
            auto tmp0 = TF(2) * Sqrt(-p / TF(3));
            auto tmp1 = TF(3) * q / (TF(2) * p) * Sqrt(TF(-3) / p);
            PA_ASSERT(tmp1 > TF(-1) && tmp1 < TF(1));
            auto tmp2 = TF(1) / TF(3) * ArcCos(tmp1);

            roots[0] = tmp0 * Cos(tmp2 - Constants<TF>::C2Pi / TF(3) * TF(0));
            roots[1] = tmp0 * Cos(tmp2 - Constants<TF>::C2Pi / TF(3) * TF(1));
            roots[2] = tmp0 * Cos(tmp2 - Constants<TF>::C2Pi / TF(3) * TF(2));
        }

        // Now "undepress" the roots.
        roots[0] = roots[0] - b / TF(3);
        roots[1] = roots[1] - b / TF(3);
        roots[2] = roots[2] - b / TF(3);

        return roots;
    }
}