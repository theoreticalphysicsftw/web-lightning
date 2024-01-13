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
#include <common/concepts.hpp>
#include <algebra/algebra.hpp>


namespace WL
{
	enum class EPropertyTransformType
	{
		U32,
		F32,
		Vec2,
		Vec3,
		Vec4,
		Void
	};

	template <typename T>
	struct ToPropertyTransformType { static constexpr EPropertyTransformType value = EPropertyTransformType::Void; };
	template <>
	struct ToPropertyTransformType<U32> { static constexpr EPropertyTransformType value = EPropertyTransformType::U32; };
	template <>
	struct ToPropertyTransformType<F32> { static constexpr EPropertyTransformType value = EPropertyTransformType::F32; };
	template <>
	struct ToPropertyTransformType<Vec2> { static constexpr EPropertyTransformType value = EPropertyTransformType::Vec2; };
	template <>
	struct ToPropertyTransformType<Vec3> { static constexpr EPropertyTransformType value = EPropertyTransformType::Vec3; };
	template <>
	struct ToPropertyTransformType<Vec4> { static constexpr EPropertyTransformType value = EPropertyTransformType::Vec4; };

	template <EPropertyTransformType value>
	struct FromPropertyTransformType { using Type = V; };
	template <>
	struct FromPropertyTransformType<EPropertyTransformType::U32> { using Type = U32; };
	template <>
	struct FromPropertyTransformType<EPropertyTransformType::F32> { using Type = F32; };
	template <>
	struct FromPropertyTransformType<EPropertyTransformType::Vec2> { using Type = Vec2; };
	template <>
	struct FromPropertyTransformType<EPropertyTransformType::Vec3> { using Type = Vec3; };
	template <>
	struct FromPropertyTransformType<EPropertyTransformType::Vec4> { using Type = Vec4; };


	using PropertyTransformStorage = StorageType<sizeof(Vec4)>;

	template <typename T>
		requires CSizeAtMost<T, PropertyTransformStorage>
	class PropertyTransform
	{
	public:
		using EType = EPropertyTransformType;
		using Storage = PropertyTransformStorage;

		template <typename U>
		PropertyTransform(U* ptr = nullptr, const U& startValue = U(), const U& endValue = U());

		template <typename U>
		auto SetProperty(U v) -> V;

	private:
		EType type;
		Storage* propertyPtr;
		Storage startValue;
		Storage endValue;
	};

	template <typename T>
		requires CSizeAtMost<T, PropertyTransformStorage>
	auto CreatePropertyTransform(T* ptr, const T& startValue, const T& endValue) -> PropertyTransform<T>;
}


namespace WL
{
	template<typename T>
		requires CSizeAtMost<T, PropertyTransformStorage>
	template<typename U>
	inline PropertyTransform<T>::PropertyTransform(U* ptr, const U& startValue, const U& endValue)
	{
		type = ToPropertyTransformType<U>::value;
		propertyPtr = (T*)ptr;
		*((U*)&this->startValue) = startValue;
		*((U*)&this->endValue) = endValue;
	}

	template<typename T>
		requires CSizeAtMost<T, PropertyTransformStorage>
	template<typename U>
	inline auto PropertyTransform<T>::SetProperty(U v) -> V
	{
		*((U*)propertyPtr) = v;
	}

	template<typename T>
		requires CSizeAtMost<T, PropertyTransformStorage>
	auto CreatePropertyTransform(T* ptr, const T& startValue, const T& endValue) -> PropertyTransform<T>
	{
		return PropertyTransform<T>(ptr, startValue, endValue);
	}
}