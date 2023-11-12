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


#include "webgl_api.hpp"
#include "webgl_pso.hpp"


namespace WL
{
    WebGLPso::WebGLPso() : program(CInvalidId), vao(CInvalidId)
    {
    }


    WebGLPso::~WebGLPso()
    {
        if (program != CInvalidId)
        {
            glDeleteProgram(program);
            glDeleteVertexArrays(1, &vao);
        }
    }


    auto WebGLPso::Compile() -> B
    {
        B status = true;

        glGenVertexArrays(1, &vao);

        this->program = glCreateProgram();

        for (auto& shader : shaders)
        {
            status = status && shader.Compile();
            glAttachShader(program, shader.GetNativeId());
        }

        glLinkProgram(program);
        GLint linkingDone = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &linkingDone);

        return status && linkingDone;
    }


    auto WebGLPso::AddShader(const C* source, U32 size,  EShaderType type) -> V
    {
        shaders[(U32)type].AddSource(source, size, type);
    }


    auto WebGLPso::AddVBLayout(const VBLayout& layout) -> V
    {
        vbLayouts[layout.binding] = layout;
    }


    auto WebGLPso::BindVB(U32 slot, const WebGLBuffer& buffer) -> V
    {
        static constexpr GLenum typeTable[] =
        {
            GL_INT,
            GL_UNSIGNED_INT,
            GL_FLOAT
        };
        glEnableVertexAttribArray(slot);
        glBindBuffer(GL_ARRAY_BUFFER, buffer.GetNativeId());
        
        const auto& layout = vbLayouts[slot];
        if (layout.type == EType::Float)
        {
            glVertexAttribPointer(slot, layout.components, typeTable[(U32)layout.type], GL_FALSE, 0, nullptr);
        }
        else
        {
            glVertexAttribIPointer(slot, layout.components, typeTable[(U32)layout.type], 0, nullptr);
        }
    }


    auto WebGLPso::BindIB(const WebGLBuffer& buffer) -> V
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.GetNativeId());
    }
}
