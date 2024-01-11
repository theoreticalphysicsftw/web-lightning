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


#include "web_lightning.hpp"

#include <iostream>

int main()
{
    using namespace WL;
    using RT = RuntimeDefault;

    if (!RT::Init())
    {
        std::cerr << "Init failed!" << std::endl;
    }

    ProgressIndicator::RoundSegmented<RT>::Desc desc;
    desc.emptyColor = 0x161616FF;
    desc.filledColor = 0xF0F000FF;
    desc.radius = 0.2f;
    desc.width = 0.05f;
    desc.gapSize = 0.02f;
    desc.segments = 8;
    ProgressIndicator::RoundSegmented<RT> pi(desc);
    pi.centered = true;

    RT::Register(&pi);

    RT::Loop();

    return 0;
}
