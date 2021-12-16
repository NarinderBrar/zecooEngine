#pragma once
#include "Platform/Core/MemoryInterface.h"
#include "Simul/Base/Export.h"
#include <map>
#include <string>
#include <iostream>

#ifdef _MSC_VER
	#pragma warning(push)  
	#pragma warning(disable : 4251)  
#endif

namespace simul
{
	namespace base
	{
		/// A pseudo allocator that tracks video memory but does not actually allocate it.
		class SIMUL_BASE_EXPORT TrackingAllocator :
			public simul::base::MemoryInterface
		{
			size_t maxVideoAllocated;
			size_t totalVideoAllocated;
			size_t totalVideoFreed;
			std::map<void*,size_t> memBlocks;
			std::map<void*, size_t> gpuMemBlocks;
			std::map<std::string, size_t> memoryTracks;
			std::map<void *,std::string> allocationNames;
			bool show_output;
			bool active;
		public:
			TrackingAllocator();
			virtual ~TrackingAllocator();
			//! Allocate nbytes bytes of memory, aligned to align and return a pointer to them.
			virtual void* AllocateTracked(size_t nbytes,size_t align,const char *fn) override;
			//! De-allocate the memory at \param address (requires that this memory was allocated with Allocate()).
			virtual void Deallocate(void* ptr) override;
			//! Track (but don't allocate) nbytes bytes of memory.
			virtual void TrackVideoMemory(void* ptr,size_t nbytes,const char *fn) override;
			//! Free the pointer from video memory tracking.
			virtual void UntrackVideoMemory(void* ptr) override;
			 
			const char *GetNameAtIndex(int index) const override;
			size_t GetBytesAllocated(const char *name) const override;
			size_t GetTotalBytesAllocated() const override;
			virtual size_t GetTotalVideoBytesAllocated() const override;
			virtual size_t GetTotalVideoBytesFreed() const override;
			virtual size_t GetCurrentVideoBytesAllocated() const override;

			//! Shut down and report any leaks.
			void Shutdown();
		};
	}
}

#ifdef _MSC_VER
	#pragma warning(pop)  
#endif
