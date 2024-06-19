#include <stdio.h>
#include <stddef.h>

char HeapStorage[20000];


struct meta_data
{
    size_t size;             
    int free;                
                    
    struct meta_data *next; 
};


struct meta_data *freeList = (void *)HeapStorage;


void initialize();
void split(struct meta_data *fitting_slot, size_t size);
void *Alloc_memory(size_t noOfBytes);
void merge();
void free_memory(void *ptr);

void initialize()
{
    freeList->size = 20000 - sizeof(struct meta_data);
    freeList->free = 1;
    freeList->next = NULL;
   
}


void split(struct meta_data *fitting_slot, size_t size)
{
    struct meta_data *new = (void *)(((void *)fitting_slot) + size + sizeof(struct meta_data));
    new->size = (fitting_slot->size) - size - sizeof(struct meta_data);
    new->free = 1;
    new->next = fitting_slot->next;
    fitting_slot->size = size;
    fitting_slot->free = 0;
    fitting_slot->next = new;
}


void *Alloc_memory(size_t SizeToAllocate)
{
    struct meta_data *curr, *prev; 
    void *result; 
    int count = 0;
    if (!(freeList->size))
    {
        initialize();
        printf("Memory initialized\n");
    }
    curr = freeList; 
    while (((curr->size < SizeToAllocate) || (curr->free == 0)) && (curr->next != NULL))
    {
        prev = curr;
        curr = curr->next;
        count++;
        printf(" %d Meta_data checked\n",count);
    }
    if ((curr->size) == SizeToAllocate)
    {
        curr->free = 0;
        result = (void *)(++curr);
        printf("Exact fitting meta_data allocated\n");
        return result;
    }
    else if (curr->size > (SizeToAllocate + sizeof(struct meta_data)))
    {
        split(curr, SizeToAllocate);
        result = (void *)(++curr);
        printf("Fitting meta_data allocated with memory split\n");
        return result;
    }
    
    else
    {
        result = NULL;
        printf("Sorry!! No sufficient Heap Storage to allocate\n");
        return result;
    }
}


void merge()
{
    struct meta_data *curr, *prev;
    curr = freeList;
    while ((curr->next) != NULL)
    {
        if ((curr->free) && (curr->next->free))
        {
            curr->size += (curr->next->size) + sizeof(struct meta_data);
            curr->next = curr->next->next;
        }
        prev = curr;
        curr = curr->next;
    }
}

void free_memory(void *ptr)
{
    if (((void *)HeapStorage <= ptr) && (ptr <= (void *)(HeapStorage + 20000)))
    {
        struct meta_data *curr = ptr;
        --curr;
        curr->free = 1;
        merge();
        printf(" Item freed from Heap\n");

    }
    else
        printf("\nPlease provide a valid pointer allocated by My_Alloc\n");

}
int main()
{
    int *first = (int *)Alloc_memory(150 * sizeof(int));
    char *second = (char *)Alloc_memory(200);
    int *third = (int *)Alloc_memory(1000 * sizeof(int));
    char *fourth = (char *)Alloc_memory(700);
    int *fifth = (int *)Alloc_memory(500 * sizeof(int));
    free_memory(first);
    free_memory(second);
    free_memory(third);
    int * h = (int *)Alloc_memory(5000* sizeof(int));
    free_memory(h);

    printf("\nAllocation and deallocation is done successfully!");
}