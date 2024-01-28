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

#include <cmath>
#include <limits>
#include <common/types.hpp>

namespace WL
{
    template <typename T>
    using Limits = std::numeric_limits<T>;

    template <typename TF>
    inline auto ArcCos(TF s) -> TF;
    template <typename TF>
    inline auto ArcSin(TF s) -> TF;
    template <typename TF>
    inline auto Sin(TF a) -> TF;
    template <typename TF>
    inline auto Cos(TF a) -> TF;
    template <typename TF>
    inline auto ArcTan2(TF y, TF x) -> TF;
    template <typename TF>
    inline auto Exp(TF n) -> TF;
    template <typename TF>
    inline auto Logarithm(TF n) -> TF;
    template <typename TF>
    inline auto Sqrt(TF n) -> TF;
    template <typename TF>
    inline auto Cbrt(TF n) -> TF;
    template <typename TF>
    inline auto Frac(TF n) -> TF;
    template <typename TF>
    inline auto Abs(TF n) -> TF;

    template <typename T>
    inline auto SmoothStep(T range0, T range1, T x) -> T;

    template <typename TF>
    inline auto IsNan(TF n) -> B;

    template <typename T>
    inline auto Clamp(T value, T range0, T range1) -> T;


    template <typename TF>
    struct Constants
    {
        static constexpr TF CPi = TF(3.14159265358979323846264338327950288419716939937510582097494459230781640628620899);
        static constexpr TF C2Pi = TF(2) * CPi;
    };
}


namespace WL
{
    template <typename TF>
    inline auto ArcCos(TF c) -> TF
    {
        return std::acos(c);
    }


    template <typename TF>
    inline auto ArcSin(TF s) -> TF
    {
        return std::asin(s);
    }


    template <typename TF>
    inline auto Sin(TF a) -> TF
    {
        return std::sin(a);
    }


    template <typename TF>
    inline auto Cos(TF a) -> TF
    {
        return std::cos(a);
    }


    template<typename TF>
    auto ArcTan2(TF y, TF x) -> TF
    {
        return std::atan2(y, x);
    }


    template<typename TF>
    auto Exp(TF n) -> TF
    {
        return std::exp(n);
    }


    template<typename TF>
    auto Logarithm(TF n) -> TF
    {
        return std::log(n);
    }


    template<typename TF>
    auto Sqrt(TF n) -> TF
    {
        return std::sqrt(n);
    }


    template<typename TF>
    auto Cbrt(TF n) -> TF
    {
        return std::cbrt(n);
    }


    template <typename TF>
    inline auto Frac(TF n) -> TF
    {
        TF integralPart;
        return std::modf(n, &integralPart);
    }


    template<typename TF>
    auto Abs(TF n) -> TF
    {
        return std::abs(n);
    }


    template<typename T>
    auto SmoothStep(T range0, T range1, T x) -> T
    {
        x = Clamp((x - range0) / (range1 - range0), T(range0), T(range1));
        return x * x * (T(3) - T(2) * x);
    }


    template<typename TF>
    auto IsNan(TF n) -> B
    {
        return std::isnan(n);
    }


    template<typename T>
    auto Clamp(T value, T range0, T range1) -> T
    {
        return Min(Max(value, range0), range1);
    }
}
