#pragma once

#ifndef VICTINI_H
#define VICTINI_H

// https://agner.org/optimize/optimizing_cpp.pdf
// https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html
// https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
// https://stackoverflow.com/questions/18071209/how-to-enable-the-highest-warning-level-in-gcc-compilerboost-is-heavily-used
// https://stackoverflow.com/questions/5088460/flags-to-enable-thorough-and-verbose-g-warnings

#include <cstdio>
#include <cstdlib>
#include <cinttypes>
#include <format>

/*
DEFINE-OPTIONS:
__VI_DEBUG: Enables Victini Debug Mode
__VI_NOLOG: Disables all Victini logging
*/

typedef unsigned long long size_t;

#define RET_SUCCESS 0

#define VI_NAMESPACE_BEGIN namespace vi {
#define VI_NAMESPACE_END };

#define VI_STRINGIFY(x) #x
#define VI_TOSTRING(x) VI_STRINGIFY(x)
#define VI_UNIMPLEMENTED static_assert(false, "UNIMPLEMENTED FUNCTION AT: (" __FILE__ ", " VI_TOSTRING(__LINE__) ")")

#define VI_TEST(__test_name, __func_call, __expected) \
auto test_value = __func_call;\
if( test_value == __expected ) fputs(std::format("TEST: [{}] => SUCCESS", __test_name), stdout); \
else fputs(std::format("TEST: [{}] => FAILURE w/ value ({})", __test_name, test_value).c_str(), stderr)

#define VI_INTERFACE template<class _C>
#define VI_ICAST static_cast<_C*>(this)
#define VI_IARGS(...) __VA_ARGS__
#define VI_MAKE_IFUNC(__type, __func_name, __args, __values) __type __func_name(__args){ return VI_ICAST->__func_name(__values); }
#define VI_USE_INTERFACE(__class, __interface) class __class final: public __interface<__class>

/// GCC ONLY
//#define _VI_ATTR_(...)      __attribute__((__VA_ARGS__))
//#define _VI_PURE            pure
//#define _VI_NO_SIDE_EFFECT  const
//#define _VI_CLASS_MODULE    visibility("internal")
//#define _VI_FASTCALL        fastcall
//#define _VI_FORCE_INLINE    always_inline
//#define _VI_LIKELY_EXEC     hot
//#define _VI_UNLIKELY_EXEC   cold
//#define _VI_FLATTEN         flatten
//#define _VI_NO_INLINE       noinline
//#define _VI_WARN_UNUSED     warn_unused_result
//#define _VI_NOPADDING       packed

#define VI_INTRINSIC_FORCE_INLINE __forceinline
#define VI_INTRINSIC_STDCALL      __stdcall
#define VI_INTRINSIC_FASTCALL     __fastcall
#define VI_INTRINSIC_VECTORCALL   __vectorcall
#define VI_INTRINSIC_RESTRICT     __restrict
#define VI_INTRINSIC_CDECL        __cdecl
#define VI_INTRINSIC_CALLBACK     __callback

#define VI_FORCE_INLINE           VI_INTRINSIC_FORCE_INLINE inline
#define VI_ALIGNAS(x)             alignas(x)
#define VI_MODULE                 static inline
#define VI_UNREACHABLE            __assume(false)
#define VI_NORETURN               [[noreturn]]
#define VI_CARRIES_DEP            [[carries_dependency]]
#define VI_FALLTHROUGH            [[fallthrough]]
#define VI_MAYBE_UNUSED           [[maybe_unused]]
#define VI_LIKELY                 [[likely]]
#define VI_UNLIKELY               [[unlikely]]
#define VI_DEPRECATED             [[deprecated]]
#define VI_NO_UNIQUE_ADDRESS      [[no_unique_address]]
#define VI_DEPRECATED_CAUSE(x)    [[deprecated(x)]]
#define VI_ASSUME(expr)           [[assume(expression)]]
#define VI_API extern
#define VI_CAPI extern "C"
#define VI_DLLEXPORT VI_CAPI  __declspec(dllexport)
#define VI_DLLIMPORT __declspec(dllimport)

#ifdef _DEBUG
    #define VI_NOEXCEPT noexcept(false)
#else
    #define VI_NOEXCEPT noexcept(true)
#endif

#ifdef VI_ENABLE_KW
#define vinterface   VI_INTERFACE class
#define forceinline  VI_FORCE_INLINE
#define stdcall      VI_INTRINSIC_STDCALL
#define fastcall     VI_INTRINSIC_FASTCALL
#define vectorcall   VI_INTRINSIC_VECTORCALL
#define unreachable  VI_UNREACHABLE
#endif // VI_ENABLE_KW

#ifdef VI_NOLOG
    #define VI_LOG(stream, log_type, msg, file, function, line)
    #define VI_LOGF(stream, format, ...) 
    #define VI_LOG_INFO(msg)
    #define VI_LOG_WARNING(msg)
    #define VI_LOG_ERROR(msg)
    #define VI_LOG_FATAL(msg)
    #define VI_IF_LOG_INFO(expr, msg)
    #define VI_IF_LOG_WARNING(expr, msg)
    #define VI_IF_LOG_ERROR(expr, msg)
    #define VI_IF_LOG_FATAL(expr, msg)
#else // DO_LOGGING
    #define VI_LOG(stream, log_type, msg, file, function, line) fprintf(stream, "[%s] %-25s (FILE: [%s] : FUNCTION: [%s] : LINE: [%d])\n", log_type, msg, file, function, line)
    #define VI_LOGF(stream, format, ...)                        fprintf(stream, format "\n", __VA_ARGS__)
    #define VI_LOG_INFO(msg)                      VI_LOG(stdout, "INFO",  msg, __FILE__, __FUNCTION__, __LINE__)
    #define VI_LOG_WARNING(msg)                   VI_LOG(stdout, "WARN",  msg, __FILE__, __FUNCTION__, __LINE__)
    #define VI_LOG_ERROR(msg)                     VI_LOG(stderr, "ERRR", msg, __FILE__, __FUNCTION__, __LINE__)
    #define VI_LOG_FATAL(msg)                     VI_LOG(stderr, "FATAL", msg, __FILE__, __FUNCTION__, __LINE__); exit(1)
    #define VI_IF_LOG_INFO(expr, msg)    if(expr) VI_LOG_INFO(msg)
    #define VI_IF_LOG_WARNING(expr, msg) if(expr) VI_LOG_WARNING(msg)
    #define VI_IF_LOG_ERROR(expr, msg)   if(expr) VI_LOG_ERROR(msg)
    #define VI_IF_LOG_FATAL(expr, msg)   if(expr) VI_LOG_FATAL(msg)
#endif // VI_NOLOG

#ifdef _DEBUG 
#define VI_STATIC_ASSERT(expr, msg) static_assert(expr, msg);
#define VI_CHECK(expr, msg) \
if(! (expr) ) {\
    VI_LOG_FATAL(msg);\
    exit(EXIT_FAILURE);\
}
#else
#define VI_CHECK
#define VI_STATIC_ASSERT(expr, msg)
#endif //_DEBUG

#endif // VICTINI_H