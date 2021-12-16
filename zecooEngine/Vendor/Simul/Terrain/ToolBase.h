#ifndef SIMUL_TERRAIN_ToolBase_H
#define SIMUL_TERRAIN_ToolBase_H
#include "Export.h"
#include <string>
#include "Platform/Math/Vector3.h"
#ifdef _MSC_VER
	
	#pragma warning(push)
	// C4251 can be ignored in Visual C++ if you are deriving from a type in the Standard C++ Library
	#pragma warning(disable:4251)
	#pragma warning(disable:4541)
#endif
namespace simul
{
	namespace terrain
	{
		SIMUL_TERRAIN_EXPORT_CLASS HeightMapInterface;
		SIMUL_TERRAIN_EXPORT_CLASS ToolBase
		{
		protected:
			virtual void ChangedToolSettings()=0;
			HeightMapInterface *heightMapInterface;
			simul::math::Vector3 view_pos;
			simul::math::Vector3 tool_dir;
			simul::math::Vector3 tool_pos;
			bool shift,alt,ctrl;
			bool mouseButtons[6];
			void ToGrid(int &X,int &Y,float x,float y) const;
			void ToMetres(float &x,float &y,int X,int Y) const;
		public:
			ToolBase();
			virtual ~ToolBase();
			//VisualDebugInterface for tools:
			unsigned GetNumLines() const;
			void GetLine(unsigned,float*,float*,int*) const;
			// Editor3DToolInterface
			virtual void UpdatePositionAndDirection(const float p[],const float d[]);
			virtual void SetShiftAltCtrlState(bool shift,bool alt,bool ctrl);
			virtual void SetMouseButtonState(int btn,bool state);
			virtual bool IsActive() const=0;

			void SetHeightMapInterface(HeightMapInterface *hmi);
		};
	}
}
#ifdef _MSC_VER
	#pragma warning(pop)
#endif
#endif // ifndef WORLD_H

