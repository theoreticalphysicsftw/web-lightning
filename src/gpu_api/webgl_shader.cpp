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


#include <common/logging.hpp>

#include "webgl_shader.hpp"

namespace WL
{
    WebGLShader::WebGLShader(const C* source, U32 size, EShaderType type) :
        Shader(type), id(0), source(source), sourceSize(size)
    {
    }

    WebGLShader::~WebGLShader()
    {
    }
    
    auto WebGLShader::AddSource(const C* source, U32 size, EShaderType type) -> V
    {
        this->source = source;
        this->sourceSize = size;
        this->type = type;
    }

    auto WebGLShader::Compile() -> B
    {
        static const GLenum typeTable[] =
        {
            GL_VERTEX_SHADER,
            GL_FRAGMENT_SHADER
        };

        static const C* nameTable[] =
        {
            "Vertex",
            "Fragment"
        };

        id = glCreateShader(typeTable[(U32)type]);
        
        if (id == 0)
        {
            return false;
        }

        auto data = source;
        GLsizei size = sourceSize;
        glShaderSource(id, 1, &data, &size);
        glCompileShader(id);
        GLint result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);

        if (result != GL_TRUE)
        {
            GLint maxLength = 0;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
            Str errorLog;
            errorLog.resize(maxLength);
            glGetShaderInfoLog(id, maxLength, &maxLength, errorLog.data());
            LogError(nameTable[U32(type)], " shader compile error: ", errorLog);
        }

        return result == GL_TRUE;
    }
}
