#ifndef __BIND_UTILITIES_HEADER_GAURD__
#define __BIND_UTILITIES_HEADER_GAURD__


#include <stdlib.h>


/** A typedef for bind_lock_t, a simple mutex */
typedef struct bind_lock_t bind_lock_t;

/** err if !b */
void bind_assert(const int b, const char* Msg);

/** Malloc, but check to see if failed */
void * bind_safe_malloc(size_t c, size_t s);

/** Realloc, but check to see if failed */
void * bind_safe_realloc(void * p, size_t c, size_t s);

/** Create a bind_lock */
bind_lock_t * make_bind_lock();

/** A lock dunction for bind_lock */
void bind_lock( bind_lock_t * const lock );

/** A lock dunction for bind_lock */
void bind_unlock( bind_lock_t * const lock );


#endif
