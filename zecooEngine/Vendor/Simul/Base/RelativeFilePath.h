#ifndef SIMUL_BASE_RELATIVEFILEPATH_H
#define SIMUL_BASE_RELATIVEFILEPATH_H
#include <string>

namespace simul
{
	namespace base
	{
		extern std::string Abs2Rel(const std::string &pcAbsPath,const std::string &pcCurrDir);
		extern std::string Rel2Abs(const std::string &pcRelPath,const std::string &pcCurrDir);
		extern std::string Rel2Rel(const std::string &pcRelPath,const std::string &pcRelCurrDir);
	}
}
#endif
