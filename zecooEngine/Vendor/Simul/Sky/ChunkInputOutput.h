#pragma once
#include "Platform/CrossPlatform/TextInputOutput.h"
#include "Simul/Sky/Export.h"
#include "Platform/Core/MemoryInterface.h"
#include <vector>
#include <fstream>
#ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable:4251)
#endif
namespace simul
{
	namespace sky
	{
	/// A virtual base class to load objects from binary.
		SIMUL_SKY_EXPORT_STRUCT Input
		{
			virtual void read(char *,int size) const=0;
			virtual int tellg() const=0;
			virtual void seekg(int)=0;
			virtual bool good() const=0;
		};
		/// A virtual base class to save objects to binary
		SIMUL_SKY_EXPORT_STRUCT Output
		{
			virtual void write(const char *,int size)=0;
			virtual int tellp() const=0;
			virtual void seekp(int)=0;
			virtual bool good() const=0;
			virtual void clear()=0;
		};
		SIMUL_SKY_EXPORT_STRUCT FileInput:public Input
		{
			FileInput(const char *filename,simul::base::MemoryInterface *m=NULL);
			~FileInput();
			void read(char *,int size) const;
			int tellg() const;
			void seekg(int);
			bool good() const;
		private:
			simul::base::MemoryInterface *memoryInterface;
			mutable std::ofstream ifs;

			mutable int pos;
			char *chunk;
			std::string fileName;
		};
		SIMUL_SKY_EXPORT_STRUCT FileOutput:public Output
		{
			FileOutput(const char *filename);
			~FileOutput();
			void write(const char *,int size);
			int tellp() const;
			void seekp(int);
			bool good() const;
			void clear();
			mutable std::ofstream ofs;
			std::string fileName;
		};

		//! \deprecated binary streaming is out of date.
		SIMUL_SKY_EXPORT_STRUCT ChunkInputOutput:virtual public Input,virtual public Output
		{
			ChunkInputOutput(simul::base::MemoryInterface *m=NULL);
			~ChunkInputOutput();
			void read(char *,int size) const;
			void write(const char *,int size);
			int tellg() const;
			int tellp() const;
			void seekg(int);
			void seekp(int);
			bool good() const;
			void clear();
			const char *data() const{return chunk;}
			int size() const{return num_bytes;}
		private:
			simul::base::MemoryInterface *memoryInterface;
			int num_bytes;
			int reserved;
			mutable int pos;
			char *chunk;
			void resize(int min_size);
		};
	}
}

#ifdef _MSC_VER
    #pragma warning(pop)
#endif
