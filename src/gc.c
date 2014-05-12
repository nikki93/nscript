#include <nscript.h>
#include <stdlib.h>

struct dynarr** garbage = NULL;
int garbage_size = 0;
int garbage_alloc = 1;

void gc_push(struct dynarr* ptr)
{
        garbage_size++;
        garbage_alloc++;
        garbage = (struct dynarr **)realloc(garbage, garbage_alloc * sizeof(struct dynarr *));
        garbage[garbage_size] = ptr;
}

void gc_collect()
{
	int i;

	for(i = garbage_size; i > 0; i--)
	{
		if(garbage[i]->arr != NULL)
		{
			free(garbage[i]->arr);
			garbage[i]->arr = NULL;
		}
	}

	for(i = garbage_size; i > 0; i--)
	{
		if(garbage[i] != NULL)
		{
			free(garbage[i]);
			garbage[i] = NULL;
		}
	}

	free(garbage);  
}
