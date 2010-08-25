/*
 * =====================================================================================
 *
 *       Filename:  ns.c
 *
 *    Description:  Runs nscript code from standard input, file or argument.
 *
 *        Created:  10/25/2009 08:17:38 AM
 *       Compiler:  gcc
 *
 *         Author:  Nikhilesh S (nikki)
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <unistd.h>

#include <nscript.h>

void runString(const char *str)
{
    ns_init();
    ns_interpret(str);
}

void runFile(FILE *file)
{
    struct dynarr *str;
    str = dynarr_new();

    int c;
    while ((c = getc(file)) != EOF)
        dynarr_append(str, (char) c);
    dynarr_append(str, '\0');

    runString(str->arr);
}

void showHelp()
{
    puts("ns [-f file] [-c code]");
    puts("Runs nscript code from standard input, file or argument.");
}

int main(int argc, char *argv[])
{
    int c;
    while ((c = getopt(argc, argv, "hc:f:")) != -1)
        switch (c)
        {
            case 'f':
                runFile(fopen(optarg, "r"));
                return 0;

            case 'c':
                runString(optarg);
                return 0;

            case 'h':
                showHelp();
                return 0;
        }

    runFile(stdin);

    return 0;
}
