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

#include <common/types.hpp>
#include <algebra/algebra.hpp>

#include "property_transform.hpp"
#include "functions.hpp"

namespace WL::Animation
{
	template <typename TRuntime>
	class Simple
	{
	public:
		using TransformsArray = Array<PropertyTransform<PropertyTransformStorage>>;
		using TransformFunction = Function<V(TransformsArray&, F32 t, F32 dt, F32 duration)>;

		Simple(TransformsArray&& transforms = TransformsArray(), const TransformFunction& tf = TransformFunction(), F32 duration = Limits<F32>::infinity());

		auto Execute(F32 dt) -> V;
		auto Done() -> B;

	private:
		TransformsArray propertyTransforms;
		TransformFunction function;
		F32 currentTime;
		F32 duration;
	};
}


namespace WL::Animation
{
	template<typename TRuntime>
	inline Simple<TRuntime>::Simple(TransformsArray&& transforms, const TransformFunction& tf, F32 duration) :
		propertyTransforms(Forward<TransformsArray>(transforms)), function(tf), duration(duration), currentTime(0)
	{
	}


	template<typename TRuntime>
	inline auto Simple<TRuntime>::Execute(F32 dt) -> V
	{
		function(propertyTransforms, currentTime, dt, duration);
		currentTime += dt;
	}


	template<typename TRuntime>
	inline auto Simple<TRuntime>::Done() -> B
	{
		return currentTime >= duration;
	}
}