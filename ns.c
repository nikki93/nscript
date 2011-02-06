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
#include <stdlib.h>

#include <nscript.h>

void runString(const char *str, const char *filename)
{
    ns_init();
    ns_interpretInChild(str, filename, ns_builtinsSpace);
}

void runFile(FILE *file, const char *filename)
{
    struct dynarr *str = dynarr_new();

    int c;
    while ((c = getc(file)) != EOF)
        dynarr_append(str, (char) c);
    dynarr_append(str, '\0');

    runString(str->arr, filename);

    dynarr_free(str);
}

void repl()
{
    char buf[1024];

    ns_init();

    struct ns_namespace *replSpace = ns_newNamespace(ns_builtinsSpace);

    for (;;)
    {
        printf("> ");
        if (!fgets(buf, sizeof(buf), stdin))
            break;
        printf("\n");
        ns_interpretInNamespace(buf, "<repl>", replSpace);
        printf("\n\n");
    }

    printf("\n");

    namespace_free(replSpace);
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
            runFile(stdin, "<stdin>");
        else if (!strcmp(argv[1], "-h"))
            showHelp(argv[0]);
        else
            runFile(fopen(argv[1], "r"), argv[1]);
    else
        repl();

    namespace_free(ns_builtinsSpace);

    struct ns_obj ret;
    int return_value = 0;

    if (ns_stackSize > 0 && (ret = ns_pop()).type == TY_INT)
        return_value = NS_INTFROMOBJ(ret);
    
    while (ns_stackSize > 0)
        ns_pop();
    free(ns_stack);

    return return_value;
}
