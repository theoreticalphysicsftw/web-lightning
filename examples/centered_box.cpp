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


#include <web_lightning.hpp>
#include <iostream>




int main()
{
    using namespace WL;
    using RT = RuntimeDefault;

    if (!RT::Init())
    {
        std::cerr << "Init failed!" << std::endl;
    }

    Box<RT>::BoxDesc desc;
    desc.width = 0.1;
    desc.height = 0.1;
    desc.radius = 0.01;

    Box<RT> box(desc);
    box.centered = true;
    RT::Register(&box);


    RT::AddPreRenderingCode
    (
        [&box](F64 dt)
        {
            static F32 direction = 1.f;
            static F32 colorDirection = 1.f;
            static F32 luminance = 5.f;
            static F32 cb = 125.f;
            static F32 cr = 44.f;
            ColorU32 color;
            color.y = ClampedU8(luminance);
            color.cb = ClampedU8(cb);
            color.cr = ClampedU8(cr);
            color.a = 255u;
            static constexpr F32 colorSpeed = 255.f / (3.f * 1E6);
            static constexpr F32 sizeSpeed = 1.f / (3.f * 1E6);
            luminance += colorSpeed * colorDirection * dt;
            box.desc.color = YCbCrAToRGBA(color);
            auto sizeChange = sizeSpeed * direction * dt;
            box.desc.width += sizeChange;
            box.desc.height += sizeChange;
            direction = (box.desc.width >= 0.5f || box.desc.width <= 0.1f) ? -direction : direction;
            colorDirection = (luminance > 250.f || luminance < 5.f) ? -colorDirection : colorDirection;
        }
    );

    RT::Loop();

    return 0;
}