// EENG 348: Digital Systems - Lab 4
// Daniel Esguerra and Mawuli Akpalu

/********* SKETCH_PART1 ***********/

#include <stdbool.h>

// Include code provided by Prof. Manohar
#include "concurreny.h"

struct process_state
{
    unsigned int sp;            // stack pointer
    struct process_state* next; // pointer to next process
};

struct lock_state
{
    bool locked;
};

void p1();
void p2();

void setup()
{
    // Set the baud rate for output
    Serial.begin(9600);

    // Set the current process to NULL
    current_process = NULL;

    // Return 0 on failure to create Process 1
    if (process_create(p1, 64) < 0)
        return 0;

    // Return 0 on failure to create Process 2
    if (process_create(p1, 64) < 0)
        return 0;
}

void loop()
{
    // Begin executing processes
    process_start();

    // Stall after all processes have executed
    while (1);
}

int process_create(void (*f)(void), int n)
{
    process_t* new_process;

    // Allocate space for a new process struct
    new_process = malloc(sizeof(process_t));

    // Return error on failure to allocate space
    if (new_process == NULL)
    {
        Serial.println("ERROR 0x01: Failure to allocate memory");
        return -1;
    }

    // Initialize new process f() with an n-byte stack
    process_init(f, n);

    // Return 0 on success
    return 0;
}

void process_start(void)
{
    // Initialize concurrent data structures

    // Start the execution of the next process
    process_begin();
}

unsigned int process_select(unsigned int cursp)
{
    process_t* next_process;

    // Assign the pointer to the next process
    next_process = current_process->next;

    // Assign the next process to the current process
    current_process = next_process;                         /** DOES THIS NEED TO BE UPDATED??? **/

    // Return 0 if no process is ready to execute
    if (next_process == NULL)
        return 0;

    // Return the stack pointer for the next process
    return next_process->sp;
}

void lock_init(lock_t *l)
{

}

void lock_aquire(lock_t *l)
{
    if (l->locked)
        yield(); // block
    
    l->locked = true;
}

void lock_release(lock_t *l)
{
    l->locked = false;
}

// Process 1
void p1(void)
{
    return;
}

// Process 2
void p2(void)
{
    return;
}
