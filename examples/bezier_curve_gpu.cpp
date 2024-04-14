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

    Path2D<F32> path;
    path.outlined = true;
    path.outlineColor = 0xFFFFFFFF;
    path.outlineWidth = 0.002f;
    //path.primitives.emplace_back(CubicBezier<F32, 2>(Vec2(0, 0), Vec2(0.25, 0.25), Vec2(0.5, 0.25), Vec2(1, 0)));
    path.primitives.emplace_back
    (
        CubicBezier<F32, 2>
        (
            WL::Vec2(0.87097974, 1.00000000),
            WL::Vec2(0.84091690, 0.94828285),
            WL::Vec2(0.80847345, 0.89250221),
            WL::Vec2(0.79261792, 0.83596743)
        )
    );
    path.primitives.emplace_back(QuadraticBezier<F32, 2>(Vec2(1.0, 0.5), Vec2(0.5, 1.0),Vec2(0.0, 0.5)));
    VectorPaths<RT> vectorPaths({ &path, 1u }, 0.6, 0.5, 0.2, 0);
    
    RT::Register(&vectorPaths);
    RT::Loop();

    return 0;
}