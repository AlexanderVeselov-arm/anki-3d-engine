// Copyright (C) 2009-2017, Panagiotis Christopoulos Charitos and contributors.
// All rights reserved.
// Code licensed under the BSD License.
// http://www.anki3d.org/LICENSE

#pragma once

#include <anki/ui/Common.h>

namespace anki
{

/// @addtogroup ui
/// @{

/// An interface that just builds the UI.
class UiImmediateModeBuilder : public UiObject
{
public:
	virtual void build(CanvasPtr ctx) = 0;
};
/// @}

} // end namespace anki