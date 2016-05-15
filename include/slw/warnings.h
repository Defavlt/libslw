
/*
 * Usage:
 * DISABLE_WARNING( "-Wgcc-warning", "-Wclang-warning", 4000 ) // 4000=msvc warning
 * ENABLE_WARNING( "-Wgcc-warning", "-Wclang-warning", 4000 ) // 4000=msvc warning
 *
 * SEE: http://stackoverflow.com/questions/3378560/how-to-disable-gcc-warnings-for-a-few-lines-of-code
*/

#define DIAG_STR(s) #s
#define DIAG_JOINSTR(x,y) DIAG_STR(x ## y)

#ifdef _MSC_VER
# define DIAG_DO_PRAGMA(x) __pragma (x)
# define DIAG_PRAGMA(compiler,x) DIAG_DO_PRAGMA(warning(x))
#else
# define DIAG_DO_PRAGMA(x) _Pragma (#x)
# define DIAG_PRAGMA(compiler,x) DIAG_DO_PRAGMA(compiler diagnostic x)
#endif

#if defined(__clang__)
# define DISABLE_WARNING(gcc_unused,clang_option,msvc_unused) DIAG_PRAGMA(clang,push) DIAG_PRAGMA(GCC,ignored "-Wunknown-pragmas") DIAG_PRAGMA(clang,ignored clang_option)//DIAG_JOINSTR(-W,clang_option))
# define ENABLE_WARNING(gcc_unused,clang_option,msvc_unused) DIAG_PRAGMA(clang,pop)
#elif defined(_MSC_VER)
# define DISABLE_WARNING(gcc_unused,clang_unused,msvc_errorcode) DIAG_PRAGMA(msvc,push) DIAG_DO_PRAGMA(warning(disable:##msvc_errorcode))
# define ENABLE_WARNING(gcc_unused,clang_unused,msvc_errorcode) DIAG_PRAGMA(msvc,pop)
#elif defined(__GNUC__)
# if ((__GNUC__ * 100) + __GNUC_MINOR__) >= 406
#  define DISABLE_WARNING(gcc_option,clang_unused,msvc_unused) DIAG_PRAGMA(GCC,push) DIAG_PRAGMA(GCC,ignored "-Wunknown-pragmas") DIAG_PRAGMA(GCC,ignored DIAG_JOINSTR(-W,gcc_option))
#  define ENABLE_WARNING(gcc_option,clang_unused,msvc_unused) DIAG_PRAGMA(GCC,pop)
# else
#  define DISABLE_WARNING(gcc_option,clang_unused,msvc_unused) DIAG_PRAGMA(GCC,ignored DIAG_JOINSTR(-W,gcc_option))
#  define ENABLE_WARNING(gcc_option,clang_option,msvc_unused) DIAG_PRAGMA(GCC,warning DIAG_JOINSTR(-W,gcc_option))
# endif
#endif