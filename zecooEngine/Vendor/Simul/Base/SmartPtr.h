#ifndef SIMUL_SMART_PTR_H
#define SIMUL_SMART_PTR_H
#include "Simul/Base/Referenced.h"
namespace simul
{
	namespace base
	{
		//!  A smart pointer class that uses intrusive reference counting.
		//! The following functions must exist for the pointed-to class:
		//!
		//!      void intrusive_ptr_add_ref(T * p);
		//!
		//!      void intrusive_ptr_release(T * p);
		//!
		//!	The usual way to do this is to have the pointed-to class inherit from simul::base::Referenced.
		//!  The object is responsible for destroying itself when its ref-count goes to zero.
		/*!
	For example:
	\code
	class TheClass: public simul::base::Referenced
	{
	public:
		TheClass():simul::base::Referenced()
		{
		}
		virtual ~TheClass()
		{
		}
		void DoSomeAction()
		{
		}
	};

	...

	{
		simul::base::SmartPtr<TheClass> classInstance=new TheClass;
		// now the instance has a reference-count of 1.

		classInstance->DoSomeAction();
		...

		// when the SmartPtr goes out of scope, the reference count is decremented.
		// and as it's now zero, the class instance deletes itself.
	}
\endcode
*/
		template<class T> class SmartPtr
		{
		private:
			typedef SmartPtr ThisType;
			T * the_pointer;
		public:
			typedef T PointedToType;
			SmartPtr(): the_pointer(0)
			{
			}
			SmartPtr(T * p, bool add_ref=true): the_pointer(p)
			{
				if(the_pointer!=0 && add_ref)
					the_pointer->intrusive_ptr_add_ref();
					//intrusive_ptr_add_ref((simul::base::Referenced*)the_pointer);
			}
			template<class U> SmartPtr(SmartPtr<U> const & sp): the_pointer(sp.get())
			{
				if(the_pointer!=0)
					intrusive_ptr_add_ref((simul::base::Referenced*)the_pointer);
			}

			SmartPtr(SmartPtr const & sp): the_pointer(sp.the_pointer)
			{
				if(the_pointer!=0)
					intrusive_ptr_add_ref((simul::base::Referenced*)the_pointer);
			}

			~SmartPtr()
			{
				if(the_pointer!=0)
					intrusive_ptr_release((simul::base::Referenced*)the_pointer);
			}
			template<class U> SmartPtr & operator=(SmartPtr<U> const & sp)
			{
				ThisType(sp).swap(*this);
				return *this;
			}
			SmartPtr & operator=(SmartPtr const & sp)
			{
				ThisType(sp).swap(*this);
				return *this;
			}
			SmartPtr & operator=(T * sp)
			{
				ThisType(sp).swap(*this);
				return *this;
			}
			T * get() const
			{
				return the_pointer;
			}
			operator T*()
			{
				return the_pointer;
			}
			T & operator*() const
			{
				return *the_pointer;
			}
			T * operator->() const
			{
				return the_pointer;
			}
			typedef T * ThisType::*unspecified_bool_type;
			operator unspecified_bool_type () const
			{
				return the_pointer == 0? 0: &ThisType::the_pointer;
			}
			bool operator! () const
			{
				return the_pointer == 0;
			}
			void swap(SmartPtr &sp)
			{
				T * tmp=the_pointer;
				the_pointer=sp.the_pointer;
				sp.the_pointer=tmp;
			}
		};

		template<class T, class U> inline bool operator==(SmartPtr<T> const & a, SmartPtr<U> const & b)
		{
			return a.get() == b.get();
		}

		template<class T, class U> inline bool operator!=(SmartPtr<T> const & a, SmartPtr<U> const & b)
		{
			return a.get()!=b.get();
		}

		template<class T> inline bool operator==(SmartPtr<T> const & a, T * b)
		{
			return a.get() == b;
		}

		template<class T> inline bool operator!=(SmartPtr<T> const & a, T * b)
		{
			return a.get()!=b;
		}

		template<class T> inline bool operator==(T * a, SmartPtr<T> const & b)
		{
			return a == b.get();
		}

		template<class T> inline bool operator!=(T * a, SmartPtr<T> const & b)
		{
			return a!=b.get();
		}

		template<class T> void swap(SmartPtr<T> & lhs, SmartPtr<T> & sp)
		{
			lhs.swap(sp);
		}

		// mem_fn support

		template<class T> T * get_pointer(SmartPtr<T> const & p)
		{
			return p.get();
		}

		template<class T, class U> SmartPtr<T> static_pointer_cast(SmartPtr<U> const & p)
		{
			return static_cast<T *>(p.get());
		}

		template<class T, class U> SmartPtr<T> const_pointer_cast(SmartPtr<U> const & p)
		{
			return const_cast<T *>(p.get());
		}
	}
}

#endif  // #ifndef SIMUL_SMART_PTR_H
