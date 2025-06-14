#ifndef __ARCH_8086_CDEFS_H
#define __ARCH_8086_CDEFS_H
/* compiler-specific definitions for kernel and userspace */

#if __STDC__
#define	__CONCAT(x,y)	x ## y
#define	__STRING(x)     #x
#else
#define	__CONCAT(x,y)	x/**/y
#define	__STRING(x)     "x"
#endif

#define __P(x) x        /* always ANSI C */

#ifdef __GNUC__
#define noreturn        __attribute__((__noreturn__)) /* don't require <stdnoreturn.h> */
#define stdcall         __attribute__((__stdcall__))
#define restrict        __restrict
#define printfesque(n)  __attribute__((__format__(__gnu_printf__, n, n + 1)))
#define noinstrument    __attribute__((no_instrument_function))
#define CONSTRUCTOR(fn,pri) void fn(void) __attribute__((constructor(pri)))
#define DESTRUCTOR(fn,pri)  void fn(void) __attribute__((destructor(pri)))
#define __wcfar
#define __wcnear
#endif

#ifdef __WATCOMC__
#define noreturn        __declspec(aborts)
#define stdcall         __stdcall
#define restrict        __restrict
#define printfesque(n)
#define noinstrument
#define CONSTRUCTOR(fn,pri) void fn(void);                                  \
                            static struct _rt_init __based(__segname("XI")) \
                                __CONCAT(_ctor,fn) = { fn, pri, 0}
#define DESTRUCTOR(fn,pri)  void fn(void);                                  \
                            static struct _rt_init __based(__segname("YI")) \
                                __CONCAT(_dtor,fn) = { fn, pri, 0}
#define __attribute__(n)
#define __wcfar         __far
#define __wcnear        __near
#endif

#ifdef __C86__
#define __HAS_NO_FLOATS__   1
#define noreturn
#define stdcall
#define restrict
#define printfesque(n)
#define noinstrument
#define CONSTRUCTOR(fn,pri) void fn(void)
#define DESTRUCTOR(fn,pri)  void fn(void)
#define __attribute__(n)
#define __wcfar
#define __wcnear

/* register specification flags for __loadreg */
#define __F_DREG    1               /* data register (AX, BX, CX, DX) */
#define __F_AREG    2               /* address register (SI, DI) */
#define __AX        (__F_DREG)
#define __CX        (__F_DREG|1024)
#define __DX        (__F_DREG|2048)
#define __BX        (__F_DREG|4096)
#define __SI        (__F_AREG)
#define __DI        (__F_AREG|8192)
void __loadreg(int reg, int value);

#endif

#endif
