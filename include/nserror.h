/*
 * =====================================================================================
 *
 *       Filename:  nserror.h
 *
 *    Description:  Error handling.
 *
 *        Created:  10/25/2009 09:09:42 AM
 *       Compiler:  gcc
 *
 *         Author:  Nikhilesh S (nikki)
 *
 * =====================================================================================
 */

#ifndef __NSERROR_H__
#define __NSERROR_H__

#include <stdio.h>

/*
 * Shows an error message.
 */
#define ns_error(fmt, ...)                                                                     \
do                                                                                             \
{                                                                                              \
    fprintf(stderr, fmt, ##__VA_ARGS__);                                                       \
    putchar('\n');                                                                             \
    exit(1);                                                                                   \
} while (0)

#endif
