/*
 * =====================================================================================
 *
 *       Filename:  nstest.c
 *
 *    Description:  A test usage of nscript.
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
#include <trie.h>
#include <nsfuncs.h>
#include <nsnamemaps.h>
#include <nsstack.h>
#include <nserror.h>

/*
 * A very hacky tried dumping thing.
 */
void trie_dump(struct trie *trie, int indent)
{
    if (trie->info.type != TY_EMPTY)
    {
        printf("\t\t-> ");
        ns_push(trie->info);
        ns_print();
        printf("\n");
        int ind = indent;
        int i;
        for (i = 1; i < 256; ++i)
            if (trie->arr[i])
            {
                while (--ind > 0)
                    putchar(' ');
                if (indent)
                    putchar('+');
                break;
            }
    }

    int i, hits = 0;
    for (i = 1; i < 256; ++i)
        if (trie->arr[i])
        {
            if (hits)
            {
                //putchar('\n');
                int ind = indent;
                while (--ind > 0)
                    putchar(' ');
                if (indent)
                    putchar('+');
            }
            printf("%c", i);
            trie_dump(trie->arr[i], indent + 1);
            ++hits;
        }
}

/*
 * A test function to execute from nscript.
 */

#include <math.h>

void my_ns_sqrt()
{
    /* Get the last object on the stack. */
    struct ns_obj obj = ns_pop();

    /* Check its type. */
    if (obj.type != TY_INT)
        ns_error("sqrt: Need an integer!");

    /* Push an object containing the sqrt result. */
    ns_push(ns_makeIntObj(sqrt(NS_INTFROMOBJ(obj))));
}

int main()
{
    ns_init();

    /* 
     * Add our sqrt function.
     */
    ns_addVariable("sqrt", ns_makeFuncObj(my_ns_sqrt));

    /*
     * Some example nscript code.
     */
    //*
    char code[] =
        "   3 $three                                            #Sets 'three' to 3                                                            \n"
        "   '\\'three\\' is ' print three print                 #Prints the value of 'three'                                                  \n"
        "   { print ', ' print } $commaPrint                    #Prints something and adds a comma after it                                   \n"

        "   { 3 + commaPrint } $add3Print                       #Prints 3 + last thing on stack                                               \n"

        "   ', and 3 plus 3, 2, 1 is ' print                                                                                                  \n"
        "   1 2 3 &add3Print 3 repeat                           #Repeats 'add3Print' over 3, 2 and 1                                          \n"

        "   '\nLambda test: ' print                                                                                                           \n"
        "   6 5 4 { 2 + print ', ' print } 1 2 add repeat       #Repeats a 'lambda' block 1+2 times over 4, 5, 6                              \n"

        "   #Print the Fibonacci sequence.                                                                                                    \n"

        "   {                                                                                                                                 \n"
        "       1 at 1 at +                                     #Add last two on stack, but keep them on stack.                               \n"
        "       dup commaPrint                                  #Print it (duplicate so it's not lost).                                       \n"

        "       dup 3 ==                                                                                                                      \n"
        "       {                                                                                                                             \n"
        "           '(that was three), ' print                                                                                                \n"
        "       } if                                            #We print '(that was three)' if the value was 3 (just for fun!)               \n"
        "   } $_fib                                                                                                                           \n"

        "   {                                                                                                                                 \n"
        "       '0, 1, ' print                                  #Print first two terms.                                                       \n"
        "       0 rot 1 rot                                     #Put first two terms, but 'rot' to keep the repeat index last.                \n"
        "       &_fib rot                                       #Put reference to _fib, but 'rot' to keep the repeat index last.              \n"
        "       repeat                                          #Runs _fib as many times as on the stack before running 'fib'.                \n"
        "   } $fib                                                                                                                            \n"

        "   '\nFirst 12 Fibonacci numbers are: ' print                                                                                        \n"
        "   10 fib                                              #Now print the first 12 numbers in the sequence.                              \n"

        "   '\n' print                                                                                                                        \n"

        "   #Factorial of a number (the stupid way :P ).                                                                                      \n"

        "   {                                                                                                                                 \n"
        "       1 rot { dup 1 + } rot                           #We put 1 and a block (which copies and increments numbers) before the count. \n"

        "       1 - dup $oneLess                                #We put count - 1 on stack, and in the 'oneLess' variable.                    \n"
        "       repeat                                          #This will cause { dup 1 + } to run starting with 1, count - 1 times, thus    \n"
        "                                                       #making  a list of numbers from 1 to count on the stack.                      \n"

        "       &* oneLess repeat                               #Now we multiply 'oneLess' times, which multiplies the list of numbers        \n"
        "                                                       #together.                                                                    \n"
        "   } $fact                                                                                                                           \n"

        "   'Factorial of 6 is: 'print                                                                                                        \n"
        "   6 fact print '\n\n' print                           #Print 6 factorial!                                                           \n"

        ;

    ns_interpret(code);
    // */

    /*
     * An nscript interactive interpretter.
     */
    //*
    char buf[1024];

    for (;;)
    {
        printf("> ");
        if (!fgets(buf, sizeof(buf), stdin))
            break;
        printf("\n");
        ns_interpret(buf);
        printf("\n\n");
    }
    // */

    /*
     * Once done, print the tries as a tree.
     */

    printf("\n\nFunctions: \n");
    trie_dump(ns_functrie, 0);

    printf("\n\nOperators: \n");
    trie_dump(ns_operatortrie, 0);

    printf("\n\nConstants: \n");
    trie_dump(ns_constanttrie, 0);

    /*
     * Some debugging stuff.
     */
    /*
    char debug[] =
        "&trieDump 1 repeat"
        ;

    ns_interpret(debug);
    // */

    return 0;
}
