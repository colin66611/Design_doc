#ifndef OA_TYPE_H
#define OA_TYPE_H

/* ===========================================================================
*/
//  32 bit unsigned
//  //
//  // ======================================================================== */

#ifndef _UNSIGNED_32
#define _UNSIGNED_32
typedef unsigned long int     unsigned32;
#endif


/* ===========================================================================
//
//  32 bit signed
//
// ======================================================================== */

#ifndef _SIGNED_32
#define _SIGNED_32
typedef long int     signed32;
#endif


/* ===========================================================================
//
//  16 bit unsigned
//
// ======================================================================== */

#ifndef _UNSIGNED_16
#define _UNSIGNED_16
typedef unsigned short     unsigned16;
#endif


/* ===========================================================================
//
//  16 bit signed
//
// ======================================================================== */

#ifndef _SIGNED_16
#define _SIGNED_16
typedef short int    signed16;
#endif


/* ===========================================================================
//
//  8 bit unsigned
//
// ======================================================================== */

#ifndef _UNSIGNED_8
#define _UNSIGNED_8
typedef unsigned char      unsigned8;
#endif


/* ===========================================================================
//
//  8 bit signed
//
// ======================================================================== */

#ifndef _SIGNED_8
#define _SIGNED_8
typedef signed char      signed8;
#endif


/* ===========================================================================
//
//  64 bit unsigned
//
// ======================================================================== */

#ifndef _UNSIGNED_64
#define _UNSIGNED_64
typedef unsigned long long    unsigned64;
#endif


/* ===========================================================================
//
//  64 bit signed
//
// ======================================================================== */

#ifndef _SIGNED_64
#define _SIGNED_64
typedef long long    signed64;
#endif


/* ===========================================================================
//
//  BOOLEAN types
//
// ======================================================================== */

#ifndef _BOOLEAN
#define _BOOLEAN
/* For C++  - NOTDONEYET: There is a clash of symbols in the code - resolve properly */
#ifndef __cplusplus
typedef short bool;
#endif
#endif

#ifndef true
#define true     (1)
#endif

#ifndef TRUE
#define TRUE     (1)
#endif

#ifndef  false
#define false    (0)
#endif

#ifndef  FALSE
#define FALSE    (0)
#endif


#define UNUSED(x)	((void)x)

#define PRINT_PREFIX		    "YOUR_SUPPER_COOL_NAME"

#define PRINT_DEBUG(msg...) do{printf(PRINT_PREFIX" debug:" msg);printf("\n");}while(0)
#define PRINT_TRACE(msg...) do{printf(PRINT_PREFIX" trace:" msg);printf("\n");}while(0)
#define PRINT_ERROR(msg...) do{printf(PRINT_PREFIX" error:" msg);printf("\n");}while(0)

/*Event definition for Timer*/
#define PERIODIC_TIMER_EVENT    (1 << 1)
#define REPLY_TIMEOUT_EVENT       (1 << 2)

/*Event Definition for OSEvent*/
#define EVENT_PS_START_REQ         (1 << 1)
#define EVENT_PS_REPLY_ACK         (1 << 2)
#define EVENT_PS_REPLY_COMPLETE    (1 << 3)
#define EVENT_PS_REPLY_ERROR       (1 << 4)
#define EVENT_PS_REPLY_TIMEOUT     (1 << 5)

#endif
