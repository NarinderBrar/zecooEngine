#pragma once
#include "Platform/Math/Matrix4x4.h"
#include "Platform/Shaders/SL/CppSl.sl"
#include "Platform/CrossPlatform/Export.h"
#include <stack>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4251)
#endif
namespace simul
{
	namespace crossplatform
	{
		/// A simple struct encapsulating a view and a projection matrix.
		struct SIMUL_CROSSPLATFORM_EXPORT ViewStruct
		{
			ViewStruct() :
				initialized(false)
				, view_id(0)
				, depthTextureStyle(DepthTextureStyle::PROJECTION)
			{}
			ViewStruct(const ViewStruct& vs) = default;
			void PushModelMatrix(math::Matrix4x4 m);
			void PopModelMatrix();
			bool initialized;
			int view_id;			///< An id unique to each rendered view, but persistent across frames.
			math::Matrix4x4 model;	///< The model matrix
			math::Matrix4x4 view;	///< The view matrix. If considered as row-major, position information is in the 4th row.
			math::Matrix4x4 proj;	///< The projection matrix, row-major.
			// derived matrices
			math::Matrix4x4 invViewProj;
			math::Matrix4x4 invView;
			math::Matrix4x4 viewProj;
			// derived vectors.
			vec3 cam_pos, view_dir, up;
			simul::crossplatform::Frustum frustum;	///< The viewing frustum, calculated from the proj matrix and stored for convenience using simul::crossplatform::GetFrustumFromProjectionMatrix.
			DepthTextureStyle depthTextureStyle;	///< How to interpret any depth texture passed from outside.
			//! MUST be called whenever view or proj change.
			void Init();
		protected:
			std::stack<math::Matrix4x4> modelStack;
		};
	}
}
#ifdef _MSC_VER
#pragma warning(pop)
#endif