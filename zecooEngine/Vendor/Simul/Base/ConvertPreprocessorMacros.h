#if defined(__EXCEPTIONS) && !defined(_CPPUNWIND)
#define _CPPUNWIND __EXCEPTIONS
#elif defined(_CPPUNWIND) && !defined(__EXCEPTIONS)
#define __EXCEPTIONS _CPPUNWIND
#endif
#if defined(_CPPRTTI) && !defined(__GXX_RTTI)
#define __GXX_RTTI _CPPRTTI
#elif defined(__GXX_RTTI) && !defined(_CPPRTTI)
#define _CPPRTTI __GXX_RTTI
#endif

// We DISABLE throw if we don't have exceptions enabled.
#ifndef _CPPUNWIND
#define throw
#endif