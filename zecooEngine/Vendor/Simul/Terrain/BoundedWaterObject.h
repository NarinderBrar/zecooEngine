#pragma once

#include "WaterWaveletsSimulator.h"
#include "Platform/CrossPlatform/Texture.h"
#include "Platform/CrossPlatform/Effect.h"
#ifdef _MSC_VER
	#pragma warning(push)  
	#pragma warning(disable : 4251)  
#endif


namespace simul
{
	namespace terrain
	{
		//! Struct for mesh objects used in water rendering
		struct WaterMeshObjectValues
		{
			int ID;
			vec3 location;
			math::Quaternion rotation;
			vec3 scale;
			int noOfVertices;
			int noOfIndices;
			vec3* vertices;
			vec3* normals;
			uint* indices;
		};

		SIMUL_TERRAIN_EXPORT_CLASS BoundedWaterObject
		{
		public:
			BoundedWaterObject();
			virtual	~BoundedWaterObject();
			//! Platform-dependent function called when initializing the water object.
			virtual	void RestoreDeviceObjects(crossplatform::RenderPlatform *r);
			//! Platform-dependent function called when uinitializing the water object.
			void	InvalidateDeviceObjects();
			//! Once per-frame update. Do this before any rendering each frame.
			void	PreRenderUpdate(crossplatform::GraphicsDeviceContext &deviceContext, float real_time_seconds);

			//! Get the Array of results from the water probes associated with this water object
			vec4*	getWaterProbeOutputs();
			//! Set the Array of results from the water probes associated with this water object
			void	setWaterProbeOutputs(vec4* outputs);
			//! Get the Array of positions of the water probes associated with this water object
			vec4*	getWaterProbePositions();
			//! Set the Array of positions of the water probes associated with this water object
			void	setWaterProbePosition(int ID, vec3 pos);
			//! Get a pointer to the probe outputs buffer
			crossplatform::StructuredBuffer<vec4>* getWaterProbeOutputsBuffer();

			//! Set the water wavelets simulator that this water object will use to generate its surface
			void								setWaterWaveletsSimulator(simul::terrain::WaterWaveletsSimulator *o);
			//! Set the global water values
			void								setWaterGlobals(simul::terrain::globalWaterValues *newWaterGlobals);
			//! Get a pointer the water values of this water object
			simul::terrain::localWaterValues*	getWaterValues();
			//! Get the wave grid that the water object is using to generate its surface
			WaveGrid* getWaveGrid();

			//! Set a given mesh object as the shape of the water object
			void setCustomWaterMesh(const WaterMeshObjectValues* newObject);
			//! Update a given mesh object
			void updateCustomWaterMesh(const WaterMeshObjectValues* Object);
			//! Get the current mesh object
			WaterMeshObjectValues* getCustomWaterMesh();
			//! Remove the water mesh
			void removeCustomWaterMesh();

			//! Update the LOD values depending on the size of the object
			void updateLODs();
			//! Update the level of detail of the surface of the water object depending on how far away the camera is
			void updateQuadDensity(vec3 camPos);

			//! Set the location of the center of the water object
			void setLocation(vec3 newLocation);
			//! Get the location of the center of the water object
			vec3 getLocation();
			//! Set the dimension of the center of the water object
			void setDimension(vec3 newDimension);
			//! Get the dimension of the center of the water object
			vec3 getDimension();
			//! Set the rotation of the center of the water object
			void setRotation(float newRotation);
			//! Get the rotation of the center of the water object
			float getRotation();
			//! Get the dimensions of how many quads the surface of the water object is using
			int2 getQuadDensity();
			//! Get the total verticies used to render the water object
			uint getTotalVerticies();
			uint ID;
			bool customMesh;
		protected:
			crossplatform::RenderPlatform			*renderPlatform;

			//Reference to Wavelets Simulator
			simul::terrain::WaterWaveletsSimulator	*waterWaveletsSimulator;

			//Foam texture
			//simul::crossplatform::Texture*	foamAlpha;

			simul::terrain::localWaterValues	waterValues;
			simul::terrain::globalWaterValues	*waterGlobals;

			WaveGrid* waveGrid;

			vec4* waterProbePositions;
			vec4* waterProbeOutputs;
			crossplatform::StructuredBuffer<vec4>* waterProbeOutputsBuffer;

			WaterMeshObjectValues customWaterObjectMesh;

			vec3 dimension;
			vec3 location;
			float rotation;
			int2 minQuadDensity;
			int2 currentQuadDensity;
			uint maxLOD;
			uint totalVerticies;
		};
	}

}

#ifdef _MSC_VER
	#pragma warning(pop)  
#endif