/**
 * @file tree.c
 * @author Abdullah Saad | 1043850 | asaad02@uoguelph.ca
 * @date november 2th 2020
 * @brief File containing the function definitions and implementations of a hash table 
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

/* This function allocate memory for performance structure and set function elements to zero */
struct Performance *newPerformance()
{
    // allocate memory for a performance struct 

    struct Performance *newPerformance = (struct Performance *) malloc( sizeof (struct Performance));

    //print an error message to the standard error stream and exit if the malloc function fails.
    if(newPerformance == NULL){
        fprintf(stderr,"%s","Memory allocation for structure performance has failed\n");
        exit(0);
    }
    
    // set zero to reads, writes, malloc , free 
    newPerformance -> reads = 0;
    newPerformance -> writes = 0;
    newPerformance -> mallocs = 0;
    newPerformance -> frees = 0;

    // return the address of the new function.
    return newPerformance;
}

/* Allocate the memory for a HashTable and initialize the parameters.*/
struct HashTable *createTable( struct Performance *performance,unsigned int capacity,int (*hash)( void *, int ),int (*compar)(const void *, const void *) ){
    
    /* allocate memory for the HashTable structure  */
    struct HashTable *createTable = malloc( sizeof ( struct HashTable ));

    //print an error message to the standard error stream and exit if the malloc function fails.
    if(createTable == NULL){
        fprintf(stderr,"%s","Memory allocation for newHashTable structure has failed\n");
        exit(0);
    }

    /*copy the value of capacity,hash, and compare in the HashTable structure*/

    createTable -> capacity = capacity; //copy capacity to the struct
    createTable -> hash = *hash ;        //copy hash to the struct
    createTable -> compar = *compar ;     //copy compar to the struct
    createTable -> nel = 0 ;            // set nel to zero

    /* allocated large enough memory for data to store capacity many pointer */
    createTable->data = malloc(sizeof (void*) * capacity);

    //print an error message to the standard error stream and exit if the malloc function fails.
    if(createTable -> data == NULL){
        fprintf(stderr,"%s","Memory allocation for data in the createTable has failed\n");
        exit(0);
    }
    /* for loop to set the value of each pointer in the data array equal to null */
    for (int i = 0; i < capacity; i++)
    {
        // increment reads at performance struct 
        performance -> reads++;
        createTable -> data[i]= NULL;
    }

    // increment mallocs at performance struct 
    performance -> mallocs++;

    return createTable ;

}


/* Add an element to the HashTable with linear probing when a collision occurs.*/
void addElement( struct Performance *performance, struct HashTable *table,void *src ){

    
    /* if nel equal to capacity that indicated the hash table full of elements */
    if (table->nel == table->capacity)
    {
        fprintf(stderr,"%s","Hash table is full of Elements\n");
        exit(0);
    }else{
    
        //generate number between zero to capacity
        int index = table->hash(src,table->capacity); //using the hash function 
        
        //increment the index until the null found 
            while (table->data[index] != NULL)
            {
                performance -> reads++; // increment reads at performance struct 
                index++; //increment index 

                if (index == table->capacity) // if index reaches capacity , index set to zero 
                {
                    index = 0;
                }
            
            }

            if(table->data[index] == NULL){
            // once a null pointer found will copy the value of src to the index of the array 
            table->data[index]= src ;

            }
    
            // increment the nel in the table 
            table->nel++;

            // increment writes at performance struct 
            performance -> writes++;
    }    

}

/* Find an element in the HashTable, return its index */
int getIdx( struct Performance *performance, struct HashTable *table, void *src ){

    // calculate the index in the pointer array
    int index = table->hash(src,table->capacity);
    int i = 0;

    // beging from the index and using the comapre function 
    for (i = index ;i < table->capacity; i++)
    {
        
        // compare the src pointer’s data matches the pointer’s data
        if (table->data[i] !=NULL && table->compar(src,table->data[i]) == 0)
        {
            // increment reads at performance struct 
            performance -> reads++;
            
            return i ; // return the index
        }
        else if(table->data[i] == NULL){
            // increment reads at performance struct 
            performance -> reads++;
        }
        
    }
    for(i = 0 ; i < index; i++) // when index set to zero contain search from zero to the orgine index 
    {
        
        // compare the src pointer’s data matches the pointer’s data
        if (table->data[i] !=NULL && table->compar(src,table->data[i]) == 0 )
        {
            // increment reads at performance struct 
            performance -> reads++;
            return i ; // return the index
        }
        else if(table->data[i] == NULL){
            // increment reads at performance struct 
            performance -> reads++;
        }
        
    }

    return -1;

}

/* Free the HashTable */
void freeTable( struct Performance *performance, struct HashTable *table ){

    // free the data pointer in table 
    free(table->data);
    //free the table itself
    free(table);
    //increment performance by 1
    performance ->frees++;
}

/* Find an element in the HashTable, return its pointer */
void *getElement( struct Performance *performance, struct HashTable *table,void *src ){
    // calculated the index by getidx function
    int i = getIdx(performance, table,src );
    // if it's -1 return Null
    if(i == -1 ){
        return NULL;
    }
    // return the poointer in the data array of the table
    return  table->data[i];
}

/* Remove an element from the HashTable */
void removeElement( struct Performance *performance, struct HashTable *table,void *target ){
    // calculated the index by getidx function
    int i = getIdx(performance, table,target );

    if (i != -1)
    {
        // set the pointer in the data array of the table to NUll
        table->data[i] = NULL;

        table->nel --;
        
        // increment writes at performance struct 
            performance -> writes++;
    }

    


}
/* Compute the hash accuracy of the contents of the HashTable. */
int hashAccuracy( struct HashTable *table ){


    int i = 0 ;
    int hash = 0 ;
    int Accuracy = 0;
    int index =0 ;

    while ( i < table->capacity)
    {
        if (table->data[i] != NULL)
        {
            index = i ;
            // calculate the index in the pointer array
            hash= table->hash(table->data[i],table->capacity);

            if (index < hash )
            {
                Accuracy = Accuracy +  table->capacity - hash + index ;
            
            }
            else if( index > hash){
                Accuracy = Accuracy + index - hash;
                
            }

            

        }
    
    
        i++ ;   
    }
    return Accuracy ;
}

/* Move the pointers in the HashTable to get a better hashAccuracy */
void rehash( struct HashTable *table ){

    int hash = 0;
    int total = 0;
 
    for (int i = 0;i < table->capacity; i++)
    {
        if (table->data[i] != NULL)
        {
           int index = i;
           // calculate the index in the pointer array
            hash= table->hash(table->data[i],table->capacity);

            if (i < hash)
            {
                total = total + (table->capacity - hash) + index ;
                if( table->data[total] == NULL){
                    
                        table->data[total] = table->data[i] ;
                        table->data[i] = NULL;
                        
                }
            }
            if( i > hash){
                total = total + (index - hash);;
                if( table->data[total] == NULL){
                    table->data[total] = table->data[i] ;
                    table->data[i] = NULL;  
                    
                }
            
            }
        }

    
    }
    
}





