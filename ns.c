/*
 * =====================================================================================
 *
 *       Filename:  ns.c
 *
 *    Description:  Runs nscript code from standard input or file.
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
#include <nsstack.h>

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

void repl()
{
    char buf[1024];

    ns_init();

    for (;;)
    {
        printf("> ");
        if (!fgets(buf, sizeof(buf), stdin))
            break;
        printf("\n");
        ns_interpret(buf);
        printf("\n\n");
    }

    printf("\n");
}

void showHelp(char *progname)
{
    printf("%s file\n", progname);
    exit(0);
}

int main(int argc, char *argv[])
{
    if (argc > 1)
        if (!strcmp(argv[1], "-"))
            runFile(stdin);
        else if (!strcmp(argv[1], "-h"))
            showHelp(argv[0]);
        else
            runFile(fopen(argv[1], "r"));
    else
        repl();

    struct ns_obj ret;
    if (ns_stackSize > 0 && (ret = ns_pop()).type == TY_INT)
        return NS_INTFROMOBJ(ret);
    return 0;
}
