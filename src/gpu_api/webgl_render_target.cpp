#include "webgl_render_target.hpp"
#include "webgl_render_target.hpp"
#include "webgl_render_target.hpp"
#include "webgl_render_target.hpp"
#include "webgl_render_target.hpp"
#include "webgl_render_target.hpp"
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


#pragma once

#include <common/error.hpp>

#include "webgl_render_target.hpp"
#include "webgl_error.hpp"

namespace WL
{
	auto WebGLRenderTarget::Init(U32 width, U32 height, U32 samples, B withDepth) -> V
	{
		this->width = width;
		this->height = height;
		this->samples = samples;

		WEBGL_VALIDATE(glGenFramebuffers(1, &frameBufferID));
		WEBGL_VALIDATE(glGenRenderbuffers(1, &colorBufferID));

		if (withDepth)
		{
			WEBGL_VALIDATE(glGenRenderbuffers(1, &depthBufferID));
		}

		GLint oldFrameBufferID;
		WEBGL_VALIDATE(glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &oldFrameBufferID));
		WEBGL_VALIDATE(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID));

		WEBGL_VALIDATE(glBindRenderbuffer(GL_RENDERBUFFER, colorBufferID));
		if (samples > 1)
		{
			WEBGL_VALIDATE(glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_RGBA8, width, height));
		}
		else
		{
			WEBGL_VALIDATE(glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height));
		}
		WEBGL_VALIDATE(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorBufferID));

		if (withDepth)
		{
			WEBGL_VALIDATE(glBindRenderbuffer(GL_RENDERBUFFER, depthBufferID));
			if (samples > 1)
			{
				WEBGL_VALIDATE(glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT16, width, height));
			}
			else
			{
				WEBGL_VALIDATE(glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height));
			}
			WEBGL_VALIDATE(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID));
		}
		
		WL_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		WEBGL_VALIDATE(glBindFramebuffer(GL_FRAMEBUFFER, oldFrameBufferID));
	}

	auto WebGLRenderTarget::Wrap(NativeID frameBufferID) -> V
	{
		this->frameBufferID = frameBufferID;
		wrapped = true;
	}

	auto WebGLRenderTarget::Bind() -> V
	{
		WEBGL_VALIDATE(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID));
	}

	auto WebGLRenderTarget::BlitTo(const WebGLRenderTarget& other) const -> V
	{
		WEBGL_VALIDATE(glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBufferID));
		WEBGL_VALIDATE(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, other.frameBufferID));
		WEBGL_VALIDATE(glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR));
	}

	auto WebGLRenderTarget::Recreate(U32 width, U32 height, U32 samples, B withDepth) -> V
	{
		Destroy();
		Init(width, height, samples, withDepth);
	}

	WebGLRenderTarget::~WebGLRenderTarget()
	{
		Destroy();
	}

	auto WebGLRenderTarget::Destroy() -> V
	{
		if (wrapped)
		{
			return;
		}
		if (colorBufferID != CInvalidID)
		{
			glDeleteRenderbuffers(1, &colorBufferID);
		}
		if (depthBufferID != CInvalidID)
		{
			glDeleteRenderbuffers(1, &depthBufferID);
		}
		if (frameBufferID != CInvalidID)
		{
			glDeleteFramebuffers(1, &frameBufferID);
		}
	}
}

