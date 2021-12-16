#ifndef COMPILERCOMPATIBILITY_H
#define COMPILERCOMPATIBILITY_H

#ifndef _MSC_VER
#if !defined(__ORBIS__) && !defined(__COMMODORE__)
#define	sprintf_s(buffer, buffer_size, stringbuffer, ...) (snprintf(buffer, buffer_size, stringbuffer, ##__VA_ARGS__))
#endif
#endif

#endif // COMPILERCOMPATIBILITY_H
