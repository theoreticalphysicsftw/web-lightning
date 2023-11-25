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
#include <common/logging.hpp>
#include <common/utilities.hpp>

#include "webgl_api.hpp"

#ifdef WL_DEBUG
	#define WEBGL_VALIDATE(ARG) \
	{ \
		ARG; \
		Str WEBGL_VALIDATE_line = ToString(__LINE__); \
		Str WEBGL_VALIDATE_file = __FILE__; \
		Str WEBGL_VALIDATE_arg = #ARG; \
		auto WEBGL_VALIDATE_error = glGetError(); \
		Str WEBGL_VALIDATE_errorString; \
		if (WEBGL_VALIDATE_error != GL_NO_ERROR) \
		{ \
			WEBGL_VALIDATE_errorString += "GL Error in "; \
			WEBGL_VALIDATE_errorString += WEBGL_VALIDATE_file + "|"; \
			WEBGL_VALIDATE_errorString += WEBGL_VALIDATE_line + ": "; \
		} \
		switch(WEBGL_VALIDATE_error) \
		{ \
			case GL_NO_ERROR: \
				break; \
			case GL_INVALID_ENUM: \
				WEBGL_VALIDATE_errorString += "GL_INVALID_ENUM"; \
				break; \
			case GL_INVALID_VALUE: \
				WEBGL_VALIDATE_errorString += "GL_INVALID_VALUE"; \
				break; \
			case GL_INVALID_OPERATION: \
				WEBGL_VALIDATE_errorString += "GL_INVALID_OPERATION"; \
				break; \
			case GL_INVALID_FRAMEBUFFER_OPERATION: \
				WEBGL_VALIDATE_errorString += "GL_INVALID_FRAMEBUFFER_OPERATION"; \
				break; \
			case GL_OUT_OF_MEMORY: \
				WEBGL_VALIDATE_errorString += "GL_OUT_OF_MEMORY"; \
				break; \
			default: \
				WEBGL_VALIDATE_errorString += "UNKNOWN (" + ToString(WEBGL_VALIDATE_error) + ")"; \
				break; \
		} \
		if (!WEBGL_VALIDATE_errorString.empty()) \
		{ \
			LogError(WEBGL_VALIDATE_errorString); \
			LogError(WEBGL_VALIDATE_arg); \
			Terminate(); \
		} \
    }
	
#else
	#define WEBGL_VALIDATE(ARG) ARG
#endif
