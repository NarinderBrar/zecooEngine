#ifndef SIMUL_CLOUDS_ENVIRONMENT_H
#define SIMUL_CLOUDS_ENVIRONMENT_H
#include "Platform/Core/PropertyMacros.h"
#include "Platform/Core/ProfilingInterface.h"
#include "Simul/Clouds/CloudKeyframer.h"
#include "Simul/Clouds/Export.h"
#include "Simul/Sky/SkyKeyframer.h"
#include "Platform/CrossPlatform/TextInputOutput.h"

#include "Platform/Core/ProfilingInterface.h"
#include <iostream>

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable:4251)
#endif

namespace simul
{
	namespace clouds
	{

	/*! The main class that manages environment data. Usually, you will create a single instance of Environment, which will persist while your
		 3D game or simulation world is active.

		 If you use environment directly and not via TrueSkyRenderer, you must call the following each frame:

			SetRealTime(time_seconds);
			skyKeyframer->TimeStep(step_days);
			Update();
		 */
		SIMUL_CLOUDS_EXPORT_CLASS Environment
		{
			simul::base::MemoryInterface *memoryInterface;
		public:
			//! Constructor - initialize with an optional memory allocator.
			Environment(simul::base::MemoryInterface *mem=NULL);
			//! Destructor
			virtual ~Environment(); // RDE: added virtual
			//! Create an Environment.
			static Environment* Create( simul::base::MemoryInterface *m = nullptr);
			//! Destroy an Environment .
			static void Destroy(Environment *env);
			//! Call this to create the keyframer sub-objects. If not called before the first Update(), it will be called from there.
			void CreateSubObjects();
			//! Call this once per frame before Update() if you use real-time updates for clouds, rain, lightning, sky keyframes etc.
			void SetRealTime(double rt_sec);
			//! Call this once per-frame
			void Update();
			//! Save a sky sequence as binary (the .seq file format); this is a low-overhead method of storing and retrieving sequences, but is subject to format changes, so 
			//! sequences should generally be stored as text (see \ref SaveToText).
			void Save(simul::sky::Output &os) const;
			//! Load a sequence from the binary .seq format.
			void Load(simul::sky::Input &is);
			//! Save a sky sequence as text (the .sq file format). This is the standard means of storing sequences.
			void SaveToText(simul::crossplatform::TextOutput &output) const;
			//! Load a sequence from the text .sq format.
			void LoadFromText(simul::crossplatform::TextInput &input);
			//! Clear the sequence.
			void New();
			//! Force recalculation of any cached values.
			void Reset();
			//! Get the cloud keyframer with the specified uid.
			CloudKeyframer *GetCloudKeyframer(sky::uid uid);
			//! Delete the specified cloud layer.
			bool DeleteCloudKeyframer(sky::uid uid);
			//!Delete the specified cloud keyframe
			bool DeleteCloudKeyframe(sky::uid uid);
			//Get the first Cloud Keyframer 
			CloudKeyframer *GetDefaultCloudKeyframer();
			//! A checksum for the keyframer setup.
			unsigned GetKeyframersChecksum() const;
			//! Map of 3D Cloud Layers
			std::map<sky::uid,simul::clouds::CloudKeyframer *> cloudKeyframers;
			//Get Keyframe by unique ID
			sky::BaseKeyframe *GetCloudKeyframe(sky::uid uid) const;
			//Get Cloud Keyframer by layer index, starting at zero.
			CloudKeyframer * GetCloudKeyframerByLayerIndex(int layer) const;
			//Get Cloud Keyframer's UID by layer index, starting at zero.
			sky::uid GetCloudKeyframerUIDByLayerIndex(int layer) const;
			//! The atmosphere/sky keyframer.
			simul::sky::SkyKeyframer *skyKeyframer;
		public:
			//! Override this to create a custom derived keyframer. Valid id's are anything greater than zero.
			virtual clouds::CloudKeyframer *CreateCloudKeyframer(base::MemoryInterface *m,bool make2d,sky::uid uid=0);
		protected:
			//! Override this to create a custom derived keyframer.
			virtual sky::SkyKeyframer *CreateSkyKeyframer(base::MemoryInterface *m,int NumElevations,int NumDistances,float MaxFadeDistanceKm);
			bool created_sub_objects;
			double now_real_time;
		public:
			unsigned setupChecksum;
		};
	}
}

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

#endif
