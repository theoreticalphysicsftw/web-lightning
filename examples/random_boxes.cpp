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


#include "web_lightning.hpp"

#include <iostream>
#include <random>
#include <chrono>

int main()
{
    using namespace WL;
    using RT = RuntimeDefault;

    if (!RT::Init())
    {
        std::cerr<<"Init failed!"<<std::endl;
    }

    auto rd = std::random_device();
    auto mt = std::mt19937(rd());
    auto dist = std::uniform_int_distribution<U32>(0, ~0u);
    auto boxesSize = 1u << 20;
    Array<Box<RT>> boxes;
    boxes.reserve(boxesSize);
    
    RT::AddPreRenderingCode
    (
        [boxesSize, &mt, &dist, &boxes](F64 dt)
        {
            static U32 currentBoxes = 0;
            if (currentBoxes < boxesSize && dt / 1000.0 < 32)
            {
                for (auto i = 0u; (i < 128 && currentBoxes < boxesSize); ++i)
                {
                    auto w = dist(mt) / F32(~0u);
                    auto h = dist(mt) / F32(~0u);
                    auto x = dist(mt) / F32(~0u);
                    auto y = dist(mt) / F32(~0u);
                    auto r = dist(mt) / F32(~0u);
                    auto c = dist(mt);
                    boxes.emplace_back(c | 0xFF, w / 8.f, h / 8.f, x, y / 2.f, r);
                    RT::Register(&boxes.back());
                    currentBoxes++;
                }
                std::cout << currentBoxes << " boxes -> " << dt / 1000.0 << "ms" << std::endl;
            }
            else
            {
                for (auto& box : boxes)
                {
                    RT::Deregister(&box);
                }
                boxes.clear();
                currentBoxes = 0;
            }
        }
    );

    RT::Loop();

    return 0;
}
