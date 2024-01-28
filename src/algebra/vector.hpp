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
#include <common/concepts.hpp>

#include "arithmetic.hpp"
#include "common.hpp"

namespace WL
{
    template <typename T, U32 N>
    struct Vector
    {
        StaticArray<T, N> data;

        T& operator[](U32 n) { return data[n]; }
        const T& operator[](U32 n) const { return data[n]; }

        template <typename... Ts>
            requires CAllAreConstructibleFrom<T, Ts...>
        Vector(Ts... elements) : data{ static_cast<T>(elements)... } {};
        Vector(T fill) { for (auto i = 0; i < N; ++i) data[i] = fill; }

        T Dot(const Vector& other) const
        {
            T sum = T(0);
            for (auto i = 0u; i < N; ++i)
            {
                sum += data[i] * other.data[i];
            }
            return sum;
        }

        T Length() const
        {
            return Sqrt(this->Dot(*this));
        }

        WL_DEFINE_COMPONENT_WISE_OPERATOR(Vector, N, +);
        WL_DEFINE_COMPONENT_WISE_OPERATOR(Vector, N, -);
        WL_DEFINE_COMPONENT_WISE_OPERATOR(Vector, N, *);
        WL_DEFINE_COMPONENT_WISE_OPERATOR(Vector, N, /);
        WL_DEFINE_COMPONENT_WISE_OPERATOR(Vector, N, %);
        WL_DEFINE_COMPONENT_WISE_OPERATOR_SCALAR(Vector, N, +);
        WL_DEFINE_COMPONENT_WISE_OPERATOR_SCALAR(Vector, N, -);
        WL_DEFINE_COMPONENT_WISE_OPERATOR_SCALAR(Vector, N, *);
        WL_DEFINE_COMPONENT_WISE_OPERATOR_SCALAR(Vector, N, /);
        WL_DEFINE_COMPONENT_WISE_OPERATOR_SCALAR(Vector, N, %);
    };
    
    #define WL_DEFINE_VECTOR_COMPONENT_WISE_FUNCTION(FUNCTION_NAME) \
        template <typename T, U32 N> \
        Vector<T, N> FUNCTION_NAME (const Vector<T, N>& v) \
        { \
            Vector<T, N> result; \
            for (auto i = 0; i < N; ++i) \
            { \
                result.data[i] = FUNCTION_NAME(v.data[i]); \
            } \
            return result; \
        }

    WL_DEFINE_VECTOR_COMPONENT_WISE_FUNCTION(Sin);
    WL_DEFINE_VECTOR_COMPONENT_WISE_FUNCTION(Cos);
    WL_DEFINE_VECTOR_COMPONENT_WISE_FUNCTION(ArcSin);
    WL_DEFINE_VECTOR_COMPONENT_WISE_FUNCTION(ArcCos);

    #undef WL_DEFINE_VECTOR_COMPONENT_WISE_FUNCTION

    #define WL_DEFINE_VECTOR_LEFT_SCALAR_OPERATOR(OP) \
        template <typename T, U32 N> \
        Vector<T, N> operator OP (T scalar, const Vector<T, N>& v) \
        { \
            Vector<T, N> result; \
            for (auto i = 0; i < N; ++i) \
            { \
                result.data[i] = scalar OP v.data[i]; \
            } \
            return result; \
        }
    
    WL_DEFINE_VECTOR_LEFT_SCALAR_OPERATOR(+);
    WL_DEFINE_VECTOR_LEFT_SCALAR_OPERATOR(-);
    WL_DEFINE_VECTOR_LEFT_SCALAR_OPERATOR(*);
    WL_DEFINE_VECTOR_LEFT_SCALAR_OPERATOR(/);
    WL_DEFINE_VECTOR_LEFT_SCALAR_OPERATOR(%);

    #undef WL_DEFINE_VECTOR_LEFT_SCALAR_OPERATOR

    template <typename TF, U32 N>
    auto SquaredDistance(const Vector<TF, N>& v0, const Vector<TF, N>& v1) -> TF
    {
        auto dir = v0 - v1;
        return dir.Dot(dir);
    }

    template <typename TF, U32 N>
    auto Distance(const Vector<TF, N>& v0, const Vector<TF, N>& v1) -> TF
    {
        return Sqrt(SquaredDistance(v0, v1));
    }

    template <typename TF, U32 N>
    auto Max(const Vector<TF, N>& v0, const Vector<TF, N>& v1) -> Vector<TF, N>
    {
        Vector<TF, N> result;
        for (auto i = 0u; i < N; ++i)
        {
            result[i] = Max(v0[i], v1[i]);
        }
        return result;
    }

    template <typename TF, U32 N>
    auto Min(const Vector<TF, N>& v0, const Vector<TF, N>& v1) -> Vector<TF, N>
    {
        Vector<TF, N> result;
        for (auto i = 0u; i < N; ++i)
        {
            result[i] = Min(v0[i], v1[i]);
        }
        return result;
    }

    template <typename TF, U32 N>
    auto operator==(const Vector<TF, N>& v0, const Vector<TF, N>& v1) -> B
    {
        return v0.data == v1.data;
    }
}
