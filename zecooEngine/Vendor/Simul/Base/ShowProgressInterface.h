#ifndef SIMUL_BASE_SHOWPROGRESSINTERFACE_H
#define SIMUL_BASE_SHOWPROGRESSINTERFACE_H

namespace simul
{
	namespace base
	{
//! ShowProgressInterface
		class ShowProgressInterface
		{
		public:
			virtual void SetText(const void *,const char *)=0;	//< Text on progress indicator
			virtual void SetProgress(const void *,float)=0;		//< Progress from zero to 1.0
			virtual void SetProgressComplete(const void *)=0;	//< Hide the progress indicator.
			virtual bool GetCancel(const void *caller)=0;		//< Has the user pressed "cancel"?
		};
	}
}
#endif
