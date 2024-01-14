// MIT License
// 
// Copyright (c) 2023 - 2024 Mihail Mladenov
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

#include <common/logging.hpp>
#include <common/utilities.hpp>

#include "webgl_api.hpp"
#include "webgl_pso.hpp"
#include "webgl_error.hpp"



namespace WL
{
    WebGLPso::WebGLPso() : program(CInvalidID), vao(CInvalidID)
    {
        Fill(ubSlots, -1);
        Fill(texSlots, -1);
    }


    WebGLPso::~WebGLPso()
    {
        if (program != CInvalidID)
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

        WEBGL_VALIDATE(glBindVertexArray(vao));

        return status && (linkingDone == GL_TRUE);
    }

    auto WebGLPso::Use() -> V
    {
        glUseProgram(program);
    }


    auto WebGLPso::AddShader(const C* source, U32 size,  EShaderType type) -> V
    {
        shaders[(U32)type].AddSource(source, size, type);
    }


    auto WebGLPso::DrawInstanced(U32 first, U32 count, U32 instances) -> V
    {
    #ifdef WL_DEBUG
        GLint result = 0;
        glValidateProgram(program);
        glGetProgramiv(program, GL_VALIDATE_STATUS, &result);
        if (result != GL_TRUE)
        {
            GLint length;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
            Str error;
            error.resize(length);
            glGetProgramInfoLog(program, length, &length, error.data());
            LogError("Validation error: ", error);
        }
    #endif
        glDrawArraysInstanced(GL_TRIANGLES, first, count, instances);
    }


    auto WebGLPso::GetUBLocation(U32 slot) -> V
    {
        if (ubSlots[slot] == -1)
        {
            WEBGL_VALIDATE(ubSlots[slot] = glGetUniformLocation(program, ubNames[slot].c_str()));
        }
    }


    auto WebGLPso::AddVBLayout(const VBLayout& layout) -> V
    {
        vbLayouts[layout.binding] = layout;
    }


    auto WebGLPso::AddSimpleConstant(U32 slot, EType type, const C* name) -> V
    {
        ubNames[slot] = name;
    }


    auto WebGLPso::AddTexture(U32 slot, const C* name) -> V
    {
        texNames[slot] = name;
    }


    auto WebGLPso::UpdateConstant(U32 slot, F32 v) -> V
    {
        GetUBLocation(slot);
        WEBGL_VALIDATE(glUniform1f(ubSlots[slot], v));
    }


    auto WebGLPso::UpdateConstant(U32 slot, Vec2 v) -> V
    {
        GetUBLocation(slot);
        WEBGL_VALIDATE(glUniform2f(ubSlots[slot], v[0], v[1]));
    }

    auto WebGLPso::UpdateConstant(U32 slot, Vec3 v) -> V
    {
        GetUBLocation(slot);
        WEBGL_VALIDATE(glUniform3f(ubSlots[slot], v[0], v[1], v[2]));
    }

    auto WebGLPso::UpdateConstant(U32 slot, Vec4 v) -> V
    {
        GetUBLocation(slot);
        WEBGL_VALIDATE(glUniform4f(ubSlots[slot], v[0], v[1], v[2], v[3]));
    }


    auto WebGLPso::BindVB(U32 slot, const WebGLBuffer& buffer, B perInstance) -> V
    {
        static constexpr GLenum typeTable[] =
        {
            GL_INT,
            GL_UNSIGNED_INT,
            GL_FLOAT
        };
        WEBGL_VALIDATE(glEnableVertexAttribArray(slot));
        WEBGL_VALIDATE(glBindBuffer(GL_ARRAY_BUFFER, buffer.GetNativeId()));
        
        const auto& layout = vbLayouts[slot];
        if (layout.type == EType::Float)
        {
            WEBGL_VALIDATE(glVertexAttribPointer(slot, layout.components, typeTable[(U32)layout.type], GL_FALSE, 0, nullptr));
        }
        else
        {
            WEBGL_VALIDATE(glVertexAttribIPointer(slot, layout.components, typeTable[(U32)layout.type], 0, nullptr));
        }

        if (perInstance)
        {
            WEBGL_VALIDATE(glVertexAttribDivisor(slot, 1));
        }
    }


    auto WebGLPso::BindIB(const WebGLBuffer& buffer) -> V
    {
        WEBGL_VALIDATE(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.GetNativeId()));
    }


    auto WebGLPso::BindTexture(const WebGLImage& tex, U32 slot) -> V
    {
        if (tex.depth > 1)
        {
            WEBGL_VALIDATE(glBindTexture(GL_TEXTURE_2D_ARRAY, tex.GetNativeId()));
        }
        else
        {
            WEBGL_VALIDATE(glBindTexture(GL_TEXTURE_2D, tex.GetNativeId()));
        }
        WEBGL_VALIDATE(glActiveTexture(GL_TEXTURE0 + slot));

        if (texSlots[slot] == -1)
        {
            WEBGL_VALIDATE(texSlots[slot] = glGetUniformLocation(program, texNames[slot].c_str()));
        }
        WEBGL_VALIDATE(glUniform1i(texSlots[slot], slot));
    }
}
