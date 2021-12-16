#pragma once
#include "Export.h"
#include "Platform/CrossPlatform/Layout.h"
#include "glad/glad.h"

namespace simul
{
	namespace opengl
	{
        //! Holds current vertex buffer layout information:
		class Layout:public crossplatform::Layout
		{
		public:
			        Layout();
			virtual ~Layout();
			void    InvalidateDeviceObjects();
			void    Apply(crossplatform::DeviceContext& deviceContext);
			void    Unapply(crossplatform::DeviceContext& deviceContext);
		};
	}
}