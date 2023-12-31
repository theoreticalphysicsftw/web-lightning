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


#include "webgl_image.hpp"
#include "webgl_error.hpp"

namespace WL
{
	GLint gFormatToGLInternalFormat[] =
	{
		GL_R8,
		GL_RGBA8,
		GL_RGBA32UI,
		GL_RGBA32F
	};

	GLenum gFormatToGLFormat[] =
	{
		GL_RED,
		GL_RGBA,
		GL_RGBA_INTEGER,
		GL_RGBA
	};

	GLenum gFormatToGLType[] =
	{
		GL_UNSIGNED_BYTE,
		GL_UNSIGNED_BYTE,
		GL_UNSIGNED_INT,
		GL_FLOAT
	};

	auto WebGLImage::Allocate(EFormat f, U32 w, U32 h, U32 d) -> B
	{
		WEBGL_VALIDATE(glGenTextures(1, &id));
		format = f;
		width = w;
		height = h;
		depth = d;

		glBindTexture(GL_TEXTURE_2D, id);
		if (depth > 1)
		{
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		return true;
	}

	WebGLImage::WebGLImage(const RawCPUImage* img)
	{
		if (img != nullptr)
		{
			Allocate(img->format, img->width, img->height, 1);
			InitData((V*)img->data.data());
		}
	}


	auto WebGLImage::InitData(V* inData) -> V
	{

		auto glFormat = gFormatToGLFormat[U32(format)];
		auto glInternalFormat = gFormatToGLInternalFormat[U32(format)];
		auto glType = gFormatToGLType[U32(format)];
		
		if (depth > 1)
		{
			WEBGL_VALIDATE(glBindTexture(GL_TEXTURE_2D_ARRAY, id));
			WEBGL_VALIDATE(glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, glInternalFormat, width, height, depth, 0, glFormat, glType, inData));
		}
		else
		{
			WEBGL_VALIDATE(glBindTexture(GL_TEXTURE_2D, id));
			WEBGL_VALIDATE(glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, width, height, 0, glFormat, glType, inData));
		}

	}


	auto WebGLImage::UpdateData(const Extent& extent, void* inData) -> V
	{
		auto glFormat = gFormatToGLFormat[U32(format)];
		auto glType = gFormatToGLType[U32(format)];

		if (depth > 1)
		{
			WEBGL_VALIDATE(glBindTexture(GL_TEXTURE_2D_ARRAY, id));
			WEBGL_VALIDATE(
							 glTexSubImage3D(
								              GL_TEXTURE_2D_ARRAY,
								              0,
								              extent.x,
											  extent.y,
								              extent.z,
								              extent.w,
								              extent.h,
								              extent.d,
											  glFormat,
											  glType,
											  inData
						                    )
			              );
		}
		else
		{
			WEBGL_VALIDATE(glBindTexture(GL_TEXTURE_2D, id));
			WEBGL_VALIDATE(
							 glTexSubImage2D(
								              GL_TEXTURE_2D,
								              0,
								              extent.x,
											  extent.y,
								              extent.w,
								              extent.h,
											  glFormat,
											  glType,
											  inData
						                    )
			              );
		}

	}
}