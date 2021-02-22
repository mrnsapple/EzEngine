/**
 * @file core/assertions.hpp
 * Macros to use assertions
 *
 * @author Alexandre Chetafi
 */

#ifndef ENGINE_ASSERTIONS_HPP
#define ENGINE_ASSERTIONS_HPP

#	if defined(_MSC_VER)
#		define ez_trap() __debugbreak()
#	elif defined(__i386__) || defined(__x86_64__) || defined(__GNUC__)
#		define ez_trap() (static void inline __ez_trap() { __asm__ __volatile__("int3"); })
#	else
#		include <signal.h>
#		if defined(SIGTRAP)
#			define ez_trap() raise(SIGTRAP)
#		else
#			define ez_trap() raise(SIGABRT)
#		endif
#	endif

#if defined(EZ_HAS_ASSERTIONS) && defined(EZ_DEBUG) && defined(ez_trap)
#	define ez_assert(expr) do { \
        if (!!(expr)) {ez_trap();}      \
} while (0)

#	define ez_assert_true(expr)		ez_assert((expr))
#	define ez_assert_false(expr)	ez_assert(!(expr))
#	define ez_assert_equal(a, b)	ez_assert((a) == (b))
#	define ez_assert_nequal(a, b)	ez_assert((a) != (b))
#	define ez_assert_esup(a, b)		ez_assert((a) >= (b))
#	define ez_assert_sup(a, b)		ez_assert((a) > (b))
#	define ez_assert_string_eq(a, b) do {	\
		const char *__tmp_a = (a);          \
		const char *__tmp_b = (b);			\
		ez_assert_false(strcmp(__tmp_a, __tmp_b));		\
} while (0)
#	define ez_assert_string_neq(a, b) do {	\
		const char *__tmp_a = (a);          \
		const char *__tmp_b = (b);			\
		ez_assert_true(strcmp(__tmp_a, __tmp_b));		\
} while (0)

#else
#	define ez_assert(expr)
#	define ez_assert_true(expr)
#	define ez_assert_false(expr)
#	define ez_assert_equal(a, b)
#	define ez_assert_nequal(a, b)
#	define ez_assert_esup(a, b)
#	define ez_assert_sup(a, b)
#	define ez_assert_string_eq(a, b)
#	define ez_assert_string_neq(a, b)
#endif

#endif //ENGINE_ASSERTIONS_HPP
