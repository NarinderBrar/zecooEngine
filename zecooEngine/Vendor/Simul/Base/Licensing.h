#pragma once
#include <string>
#include "Simul/Base/Export.h"
namespace simul
{
	namespace base
	{
		/// The type of licence.
		enum LicenceResult{INVALID,EXPIRED_ALPHA,EXPIRED_FULL,EVAL_ALPHA,EVAL_FULL,PERMANENT_ALPHA,PERMANENT_FULL};
		struct LicenceInfo
		{
			LicenceResult licenceResult;
			int year,month,day;
		};
		/// The feature-level of this trueSKY version.
		enum FeatureLevel
		{
			DISABLED=0,STANDARD=2,EXPERIMENTAL=4
		};
		/// The current feature level, which determines whether this is the feature-limited trueSKY alpha, or full trueSKY.
		extern SIMUL_BASE_EXPORT FeatureLevel GetFeatureLevel();
		/// The maximum feature level (see GetFeatureLevel), determined by the current licence.
		extern SIMUL_BASE_EXPORT FeatureLevel GetMaximumFeatureLevel();
		/// Retrieve information on the specified licence key.
		extern SIMUL_BASE_EXPORT LicenceInfo GetLicenceInfo(const char *lic);
		/// Change the feature level. It is only possible to change to a lower feature level than the maximum one (see GetMaximumFeatureLevel).
		extern SIMUL_BASE_EXPORT void SetFeatureLevel(FeatureLevel f);
		extern SIMUL_BASE_EXPORT const char *GetLicence();
		//! Get information on the status of the given licence
		extern SIMUL_BASE_EXPORT const char *GetLicenceValidityText(const char *lic);
		extern SIMUL_BASE_EXPORT void SetLicence(const char *txt);
	}
}
