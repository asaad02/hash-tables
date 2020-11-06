/**
 * @file hash.c
 * @author Abdullah Saad | 1043850 | asaad02@uoguelph.ca
 * @date november 2th 2020
 * @brief File containing the function prototypes of a  hash Table  
 **/


/* This structure represents a hash table */
struct HashTable
{
    unsigned int capacity;      // number of elements in the table
    unsigned int nel;           //number of elements currently in the table 
    void **data;                // an array of capacity number of pointers to the data accessed by the hash table
    int (*hash)(void *, int );  //function pointer to the hashing function used to decide where to store the data
    int (*compar)(const void *, const void *); //function which returns a value of 0 if the data stored at the two pointer arguments match.
};

/* structure to measure the performance of the code and Algorithms in tree.*/
struct Performance
{
    unsigned int reads;     /*  how many reads operations    */
    unsigned int writes;    /*  how many writes operations   */
    unsigned int mallocs;   /*  haw many maloocs operation   */
    unsigned int frees;     /*  how many frees operations    */
};

/* this function allocate memory for the performance struct and return the address of the the structure */
struct Performance *newPerformance();

/* Allocate the memory for a HashTable and initialize the parameters.*/
struct HashTable *createTable( struct Performance *performance,unsigned int capacity,int (*hash)( void *, int ),int (*compar)(const void *, const void *) );

/* Add an element to the HashTable with linear probing when a collision occurs.*/
void addElement( struct Performance *performance, struct HashTable *table,void *src );

/* Find an element in the HashTable, return its index */
int getIdx( struct Performance *performance, struct HashTable *table,void *src );

/* Free the HashTable */
void freeTable( struct Performance *performance, struct HashTable *table );

/* Find an element in the HashTable, return its pointer */
void *getElement( struct Performance *performance, struct HashTable *table,void *src );

/* Remove an element from the HashTable */
void removeElement( struct Performance *performance, struct HashTable *table,void *target );

/* Compute the hash accuracy of the contents of the HashTable. */
int hashAccuracy( struct HashTable *table );

/* Move the pointers in the HashTable to get a better hashAccuracy */
void rehash( struct HashTable *table );

