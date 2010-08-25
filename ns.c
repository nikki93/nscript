/*
 * =====================================================================================
 *
 *       Filename:  ns.c
 *
 *    Description:  Interprets nscript code from standard input.
 *
 *        Created:  10/25/2009 08:17:38 AM
 *       Compiler:  gcc
 *
 *         Author:  Nikhilesh S (nikki)
 *
 * =====================================================================================
 */

#include <stdio.h>

#include <nscript.h>

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        printf("%s [-h]\nInterprets nscript code from standard input.\n", argv[0]);
        return 0;
    }

    ns_init();

    struct dynarr *f;
    f = dynarr_new();

    int c;
    while ((c = getchar()) != EOF)
        dynarr_append(f, (char) c);
    dynarr_append(f, '\0');

    ns_interpret(f->arr);

    return 0;
}
