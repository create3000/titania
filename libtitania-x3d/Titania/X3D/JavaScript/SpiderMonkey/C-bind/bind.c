#include "bind.h"
#include "bind_utilities.h"

#include <signal.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <syscall.h>
#include <sys/mman.h>


/** A boolean */
typedef enum { false = 0, true } bool;


/***************************************************************/
/*                                                             */
/*      A struct to store the required values to-be-bound      */
/*                                                             */
/***************************************************************/


/** A structure that holds all data needed to bind a function */
typedef struct bound_internals_t {
	/** A data array to store the bound arguments */
	arg_t * args;
	/** The function being bound */
	Bindable fn;
	/** The total number of arguments */
	uint64_t n_total;
	/** The number of arguments currently bound */
	uint64_t n_bound;
	/** True if systemv calls should be used */
	bool systemv;
} bound_internals_t;


/***************************************************************/
/*                                                             */
/*    A small vector class used to store bound_internals_ts    */
/*                                                             */
/***************************************************************/


/** A thread-safe vector of bound_internals */
typedef struct bind_vec_t {
	/** The data array */
	bound_internals_t ** arr;
	/** The size of the vector */
	uint64_t size;
	/** The max size of the vector allocated */
	uint64_t msize;
	/** An obscurred thread-safe lock */
	bind_lock_t * lock;
} bind_vec;


/** Create a bind_vec */
bind_vec * make_bind_vec() {
	bind_vec * ret = bind_safe_malloc(1, sizeof(bind_vec));
	ret->msize = 8;
	ret->arr = bind_safe_malloc(ret->msize, sizeof(bound_internals_t*));
	ret->size = 0;
	ret->lock = make_bind_lock();
	return ret;
}

/** Consume a bound_internals and append it to the vector */
uint64_t bv_consume_add_blank(bind_vec * const bv, bound_internals_t * const add) {
	bind_lock(bv->lock);
	if (bv->size == bv->msize) {
		bv->msize *= 2;
		bv->arr = bind_safe_realloc(bv->arr, bv->msize, sizeof(bound_internals_t*));
	}
	bv->arr[bv->size] = add;
	const uint64_t ret = bv->size;
	bv->size += 1;
	bind_unlock(bv->lock);
	return ret;
}

/** The get fuction for a bind_vec_t */
bound_internals_t * bv_get(bind_vec * const bv, const uint64_t index) {
	bind_lock(bv->lock);
	bind_assert(index < bv->size, "accessing illegal bv index");
	bound_internals_t * ret = bv->arr[index];
	bind_unlock(bv->lock);
	return ret;
}


/***************************************************************/
/*                                                             */
/*             The binding / inoking internal logic            */
/*                                                             */
/***************************************************************/


// Globals


/** A global bind vec */
bind_vec * global_bv = NULL;
/** A global bind_lock for full systemv inokes' args*/
bind_lock_t * full_systemv_arg_lock = NULL;
/** The full_system_lock arg value protected by a lock */
bound_internals_t * full_systemv_arg_global;
/** A global bind_lock for full systemv inokes' return */
bind_lock_t * full_systemv_ret_lock = NULL;
/** The full_system_lock ret value protected by a lock */
ret_t full_systemv_ret_global;
/** An int representing the signal number used by the bind library internals
 *  This variable must be protected by all locks that invoke this signal */
int systemv_invoke_sig = SIGUSR2;


// Functions


/** Ensure FN(ARGS) returns 0 */
#define ASSERT_ZERO(FN, ...) bind_assert(FN(__VA_ARGS__) == 0, ""#FN "() failed.");

/** Setup the global locks
 *  This will automatically be called before main */
__attribute__((constructor))
void bind_setup() {
	global_bv = make_bind_vec();
	full_systemv_arg_lock = make_bind_lock();
	full_systemv_ret_lock = make_bind_lock();
}

/** A macro used to set a register argument, used for consistency */
#define SET_ARG(REG)                   \
	"mov (%%rax), %%" # REG "\n\t"     \
	"add $8, %%rax\n\t"                \
	"dec %%r12\n\t"                    \
	"test %%r12, %%r12\n\t"            \
	"jz invoke_sym%=\n\t"

/** The signal handler used to invoke the systemv call */
__attribute__((optimize("-O0")))
void systemv_invoke_helper(int signo) {

	// Retrieve the stored bound_internals
	register volatile bound_internals_t * bb asm("r15");
	bb = full_systemv_arg_global;
	bind_unlock(full_systemv_arg_lock);

	// Setup the registers and stack needed to
	// invoke the function with the desired arguments
	volatile ret_t ret;
	asm volatile(
		"mov %[argsv], %%rax\n\t" /* Args */
		"mov %[func], %%r11\n\t"  /* Func */
		"mov %[n], %%r12\n\t"     /* n */
		"mov %%rsp, %%r14\n\t"    /* Store rsp */

		// Set first 6 args if they exist
		SET_ARG(rdi)
		SET_ARG(rsi)
		SET_ARG(rdx)
		SET_ARG(rcx)
		SET_ARG(r8)
		SET_ARG(r9)

		// Prep to set the rest of the args
		"mov %%r12, %%r13\n\t"
		"shl $3, %%r13\n\t"
		"sub $8, %%r13\n\t"
		"add %%r13, %%rax\n\t"

		// For the 7+ args, push them onto the stack in reverse
		"loop_sym%=:\n\t"
		"mov (%%rax), %%r13\n\t"
		"push %%r13\n\t"
		"sub $8, %%rax\n\t"
		"dec %%r12\n\t"
		"test %%r12, %%r12\n\t"
		"jnz loop_sym%=\n\t"

		// Invoke the function
		"invoke_sym%=:\n\t"
		"call *%%r11\n\t"

		// Restore rsp and set the return value
		"mov %%r14, %%rsp\n\t"
		"mov %%rax, %[retv]"

		// Variables
		: [retv] "=r" (ret)
		: [argsv] "r" (bb->args),
		  [func] "r" (bb->fn),
		  [n] "r" (bb->n_total)
		:
	);

	// Return the value
	bind_lock(full_systemv_ret_lock);
	full_systemv_ret_global = ret;
	(void) signo;
}

/** gettid syscall */
static long gettid() {
	return syscall(SYS_gettid);
}

/** tkill syscall */
static long tkill(int tid, int sig) {
	return syscall(SYS_tkill, tid, sig);
}

/** Invoke a systemv bound_internals_t on its store arguments */
__attribute__ ((noinline))
ret_t systemv_invoke(bound_internals_t * bb) {

	// Get the r11'th bound_internals then invoke it
	volatile uint64_t index;
	asm volatile("mov %%r11, %0" : "=rm" (index));

	// Create a new sigaction
	struct sigaction new, old;
	ASSERT_ZERO(sigemptyset, & (new.sa_mask));
	ASSERT_ZERO(sigaddset, &(new.sa_mask), systemv_invoke_sig);
	new.sa_handler = & systemv_invoke_helper;
	new.sa_flags = 0;

	// Invoke the helper via a signal then restore the signal handler
	ASSERT_ZERO(sigaction, systemv_invoke_sig, &new, &old);
	bind_lock(full_systemv_arg_lock);
	full_systemv_arg_global = bb;
	ASSERT_ZERO(tkill, gettid(), systemv_invoke_sig);
	ASSERT_ZERO(sigaction, systemv_invoke_sig, &old, NULL);

	// Release the ret lock and return the return value
	ret_t ret = full_systemv_ret_global;
	bind_unlock(full_systemv_ret_lock);
	return ret;
}

/** Decide the method of invocation based on if systemv is true */
static ret_t invoke(bound_internals_t * const bb) {
	return bb->systemv ? systemv_invoke(bb) : bb->fn(bb->args);
}

/** Invoke a fully bound function
 *  The function index is stored in r11 */
__attribute__ ((used, noinline))
ret_t invoke_full_bound() {

	// Get the r11'th bound_internals then invoke it
	volatile uint64_t index;
	asm volatile("mov %%r11, %0" : "=rm" (index) );
	bound_internals_t * const bb = bv_get(global_bv, index);
	return invoke(bb);
}

/** Invoke a partially bound function with the additional args of a1, ...
 *  The function index is stored in r11 */
__attribute__ ((used, noinline))
ret_t invoke_partial_bound( arg_t a1, ... ) {

	// Get the r11'th bound_internals
	volatile uint64_t index;
	asm volatile("mov %%r11, %0" : "=rm" (index) );
	bound_internals_t * const bb = bv_get(global_bv, index);

	// Copy in the newly provided args then invoke the function
	bb->args[bb->n_bound] = a1;
	va_list args;
	va_start(args, a1);
	for ( uint64_t i = bb->n_bound + 1; i < bb->n_total; ++i ) {
		bb->args[i] = va_arg(args, arg_t);
	}
	va_end(args);
	return invoke(bb);
}

/** Generate a stub for a bound function
 *  Use invoker as the invoke function */
void * gen_stub(const uint64_t index, void * const invoker) {

	/* Generate as code the following (intex syntax) amd64 assembly:
	 *  mov r11, invoker
	 *  push r11
	 *  mov r11, index
	 *  ret
	*/
	const uint64_t size = 0x1000;
	char * const func = mmap(0, size, PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	bind_assert(func != MAP_FAILED, "mmap() failed.");
	const char stub[] = "\x49\xBB" "AAAAAAAA" "\x41\x53\x49\xBB" "BBBBBBBB" "\xC3";
	memcpy(func, stub, sizeof(stub));
	memcpy(strstr(func, "BBBBBBBB"), & index, 8 );
	memcpy(strstr(func, "AAAAAAAA"), & invoker, 8 );
	ASSERT_ZERO(mprotect, func, size, PROT_READ | PROT_EXEC);
	return func;
}

/** Generate a stub for a fully bound function */
FullBound gen_stub_full(const uint64_t index) {
	return gen_stub(index, invoke_full_bound);
}

/** Generate a stub for a partially bound function */
PartBound gen_stub_partial(const uint64_t index) {
	return gen_stub(index, invoke_partial_bound);
}

/** A macro to set up a bound_internals
 *  This macro exists to ensure consistency */
#define BIND_AND_STORE_FN(LAST_ARG, IS_SYSTEMV)                               \
	                                                                                \
	/** Initalize a bound_internals */                                              \
	bound_internals_t * const bb = bind_safe_malloc(1, sizeof(bound_internals_t));  \
	bb->args = bind_safe_malloc(n_total, sizeof(arg_t));                            \
	bb->systemv = IS_SYSTEMV;                                                       \
	bb->n_total = n_total;                                                          \
	bb->n_bound = n_bound;                                                          \
	bb->fn = func;                                                                  \
	                                                                                \
	/** Copy in the passed arguments */                                             \
	va_list args;                                                                   \
	va_start(args, LAST_ARG);                                                       \
	for ( uint64_t i = 0; i < n_bound; ++i ) {                                      \
		bb->args[i] = va_arg(args, arg_t);                                          \
	}                                                                               \
	va_end(args);                                                                   \
	                                                                                \
	/** Add the bound_internals to the global bind_vec */                           \
	const uint64_t index = bv_consume_add_blank(global_bv, bb);


/***************************************************************/
/*                                                             */
/*                        API functions                        */
/*                                                             */
/***************************************************************/


// Fully bind a function to the n_total arguments
FullBound full_nonsystemv_bind(Bindable func, const uint64_t n_total,  ...) {
	bind_assert(n_total > 0, "full_bind() called improperly");
	const uint64_t n_bound = n_total;
	BIND_AND_STORE_FN(n_total, false);
	return gen_stub_full(index);
}

// Partially bind a function
PartBound partial_nonsystemv_bind(Bindable func, const uint64_t n_total, const uint64_t n_bound, ...) {
	bind_assert(n_bound > 0, "partial_bind() called improperly");
	bind_assert(n_total > n_bound, "partial_bind() called improperly");
	BIND_AND_STORE_FN(n_bound, false);
	return gen_stub_partial(index);
}

// Fully bind a SystemV function
FullBound full_bind(BindableSystemV func, const uint64_t n_total,  ...) {
	bind_assert(n_total > 0, "full_systemv_bind() called improperly");
	const uint64_t n_bound = n_total;
	BIND_AND_STORE_FN(n_total, true);
	return gen_stub_full(index);
}

// Partially bind a SystemV function
PartBound partial_bind(BindableSystemV func, const uint64_t n_total, const uint64_t n_bound, ...) {
	bind_assert(n_bound > 0, "partial_bind() called improperly");
	bind_assert(n_total > n_bound, "partial_bind() called improperly");
	BIND_AND_STORE_FN(n_bound, true);
	return gen_stub_partial(index);
}


// Set the signal number that will be used by the bind library internally
void bind_set_signal_number(const int signo) {
	bind_assert(signo >= 0, "signals numbers cannot be negative");
	bind_lock(full_systemv_arg_lock);
	systemv_invoke_sig = signo;
	bind_unlock(full_systemv_arg_lock);
}

// Return the signal number used by the bind library internally
int bind_get_signal_number() {
	bind_lock(full_systemv_arg_lock);
	const int ret = systemv_invoke_sig;
	bind_unlock(full_systemv_arg_lock);
	return ret;
}
