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

#define DEFINE_SHADER(SHADER_NAME) extern Str SHADER_NAME;

namespace WL
{
    DEFINE_SHADER(FontAtlasTextVert);
    DEFINE_SHADER(FontAtlasTextFrag);
    DEFINE_SHADER(TexturedQuadVert);
    DEFINE_SHADER(TexturedQuadFrag);
    DEFINE_SHADER(BoxVert);
    DEFINE_SHADER(BoxFrag);
    DEFINE_SHADER(LineVert);
    DEFINE_SHADER(LineFrag);
    DEFINE_SHADER(CircularArcVert);
    DEFINE_SHADER(CircularArcFrag);
    DEFINE_SHADER(BezierApproxVert);
    DEFINE_SHADER(BezierApproxFrag);
}