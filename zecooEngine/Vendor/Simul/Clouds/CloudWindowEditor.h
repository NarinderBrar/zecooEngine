#pragma once
#include "Platform/Core/BaseMouseHandler.h"
#include "Platform/CrossPlatform/Quaterniond.h"
#include "Simul/Clouds/Export.h"
#include "Simul/Clouds/CloudWindow.h"
#include "Simul/Clouds/Environment.h"

namespace simul
{
	namespace clouds
	{
		enum class CloudWindowEditorMode
		{
			KEYFRAME,
			VOLUME
		};
		enum class VolumePartType
		{
			NONE=0,
			POSITION=1,
			HEIGHT,
			LOWER_RADIUS,
			UPPER_RADIUS,
			LOWER_EDGE,
			UPPER_EDGE,
			LOWER_HEIGHT,
			UPPER_HEIGHT
		};
		struct VolumePartIdentifier
		{
			sky::uid uid=0;
			VolumePartType part=VolumePartType::NONE;
			crossplatform::Quaterniond original;
		};
		//! An onscreen editor for the CloudWindow. This handles mouse behaviour, but is renderer-independent: it is passed as a pointer to the cloud window editor render funcion.
		class SIMUL_CLOUDS_EXPORT CloudWindowEditor : public simul::base::BaseMouseHandler
		{
		public:
			CloudWindowEditor();
			~CloudWindowEditor();
			void	SetEnvironment(Environment *e);
			void	mousePress(int button,int x,int y) override;
			void	mouseRelease(int button,int x,int y) override;
			void	mouseDoubleClick(int button,int x,int y) override;
			void	mouseMove(int x,int y) override;
			void	mouseWheel(int delta,int modifiers) override;
			void    KeyboardProc(unsigned int nChar, bool bKeyDown, bool bAltDown) override;
			void	SetCloudWindow(CloudWindow *w);
			void	NewSequence();
			CloudWindow *GetCloudWindow();
			void	SetMouseOrigin(crossplatform::Quaterniond m);
			CloudWindowEditorMode	GetMode() const;
			void	SetMode(CloudWindowEditorMode m);
			void	SetHighlightColour(vec4 clr);
			vec4	GetHighlightColour() const
			{
				return highlightColour;
			}
			CloudWindowEditorMode GetCloudWindowEditorMode() const
			{
				return cloudWindowEditorMode;
			}
			void SetCloudWindowEditorMode(CloudWindowEditorMode mode)
			{
				cloudWindowEditorMode = mode;
			}
			Environment *GetEnvironment()
			{
				return environment;
			}
			VolumePartIdentifier GetNearestPart(int2 pos) const;
			CloudKeyframe* ActiveKeyframe() const;
			float	GetRadiansPerPixel() const
			{
				return rads_per_pixel;
			}

			void	SetStoreForUndo(std::function<void()> fn);
			//	layerID : Layer the keyframe is located on; an optimisation, saves searching.
			SelectionIdentifier KeyframeToSelectionIdentifier(simul::sky::BaseKeyframe* keyframe) const;
			SelectionIdentifier UidToSelectionIdentifier(simul::sky::uid uid) const;

			void	SetTypeVisible(SelectionIdentifier::Type type, bool);
			SelectionIdentifier::Type	GetTypeVisibleMask() const;
			void SetInverseMatrix(mat4 invViewProj, vec3 cam_pos);
			/// Convert from window pixel position to quaternion on the sphere.
			crossplatform::Quaterniond WindowPosToQuaternion(int2 pos) const;
			crossplatform::Quaterniond GetMouseQuaternion() const;
			std::set<simul::clouds::SelectionIdentifier> &GetSelections();
			crossplatform::Quaterniond origin;
			float scaleKm;
		private:
			bool MouseMovedEnoughCheck() const;
			CloudWindowEditorMode cloudWindowEditorMode;
			std::function<void()> StoreForUndo;
			SelectionIdentifier::Type type_mask;
			crossplatform::Quaterniond mouseOrigin;
			mat4 inverseMatrix;
			vec3 cam_pos;
			CloudWindow *cloudWindow;
			Environment *environment;
			std::set<SelectionIdentifier> selections;
			int modifiers = 0;
			vec4 highlightColour = { 1.f,1.f,1.f,1.f };
			int2 startMouse;
			bool moved_enough;
			float rads_per_pixel;
			VolumePartIdentifier internalSelection;
		};
	}
}