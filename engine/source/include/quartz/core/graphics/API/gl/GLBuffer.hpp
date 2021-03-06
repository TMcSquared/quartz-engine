// Copyright 2019 Genten Studios
// 
// Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
// following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the 
// following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the 
// following disclaimer in the documentation and/or other materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote 
// products derived from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED 
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY 
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH 
// DAMAGE.

#pragma once

#include <quartz/core/Core.hpp>
#include <quartz/core/graphics/API/IBuffer.hpp>
#include <quartz/core/graphics/API/gl/GLCommon.hpp>

namespace qz
{
	namespace gfx
	{
		namespace api
		{
			namespace gl
			{
				class QZ_API GLBuffer : public IBuffer
				{
				public:
					GLBuffer(BufferTarget target, BufferUsage usage);

					~GLBuffer();

					GLBuffer(const GLBuffer& o) = default;
					GLBuffer& operator=(const GLBuffer& o) = default;

					GLBuffer(GLBuffer&& o) noexcept;
					GLBuffer& operator=(GLBuffer&& o) noexcept;

					void bind() override;
					void unbind() override;

					void resize(unsigned int size) override;
					void setData(unsigned int size, const void* data) override;

					void releaseDataPointer() override;

				protected:
					void* retrievePointerInternal() override;

				private:
					unsigned int m_id = 0;
					unsigned int m_size = 0;

					GLenum m_target;
					GLenum m_usage;
				};
			}

		}
	}
}

