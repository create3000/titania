#ifndef __BIND_H__
#define __BIND_H__

#include <stdint.h>


/** Only allow 64 bit */
#ifndef __x86_64__
	*** Error: Only 64-bit currently supported! ***
#endif


/*******************************************************************/
/*                                                                 */
/*                              Types                              */
/*                                                                 */
/*******************************************************************/


/** The argument type */
typedef void * arg_t;
/** The return type */
typedef void * ret_t;


/** A bindable function type for SystemV
 *  This function should return a ret_t */
typedef void * BindableSystemV;
/** A bindable function type */
typedef ret_t (* Bindable) ( arg_t * args );


/** A fully bound function type */
typedef ret_t (* FullBound) ( );
/** A partially bound function type
 *  Because this can have multiple different
 *  type signatures it is left as a void * */
typedef void * PartBound;


/*******************************************************************/
/*                                                                 */
/*                            Functions                            */
/*                                                                 */
/*******************************************************************/


/** Fully bind a function to the n_total arguments
 *  provided using the SystemV calling convention */
FullBound full_bind(BindableSystemV func, const uint64_t n_total,  ...);

/** Partially bind a function that takes n_total argument to the
 *  n_bound arguments provided using the SystemV calling convenction
 *  Note: fully binding a function via partial_bind is permitted */
PartBound partial_bind(BindableSystemV func, const uint64_t n_total, const uint64_t n_bound, ...);


/** Fully bind a function to the n_total arguments provided */
FullBound full_nonsystemv_bind(Bindable func, const uint64_t n_total,  ...);

/** Partially bind a function that takes n_total
 *  arguments to the n_bound arguments provided
 *  Note: fully binding a function via partial_bind is permitted */
PartBound partial_nonsystemv_bind(Bindable func, const uint64_t n_total, const uint64_t n_bound, ...);


/** Set the signal number that will be used by the bind library internally
 *  This is not needed in a single threaded application, but for a multi-threaded
 *  application the ensures thread-safety. The default is SIGUSR2. Select a
 *  signal your program does not use for the bind library to use */
void bind_set_signal_number(const int signo);

/** Return the signal number used by the bind library internally */
int bind_get_signal_number();


#endif
