// Copyright (C) 2009-2023, Panagiotis Christopoulos Charitos and contributors.
// All rights reserved.
// Code licensed under the BSD License.
// http://www.anki3d.org/LICENSE

#include <AnKi/Gr/Framebuffer.h>
#include <AnKi/Gr/Vulkan/FramebufferImpl.h>
#include <AnKi/Gr/GrManager.h>

namespace anki {

Framebuffer* Framebuffer::newInstance(const FramebufferInitInfo& init)
{
	FramebufferImpl* impl = anki::newInstance<FramebufferImpl>(GrMemoryPool::getSingleton(), init.getName());
	const Error err = impl->init(init);
	if(err)
	{
		deleteInstance(GrMemoryPool::getSingleton(), impl);
		impl = nullptr;
	}
	return impl;
}

} // end namespace anki
