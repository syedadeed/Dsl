#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

typedef enum{
    DA_NIL,                   //When there is no error
    DA_NULL_ARRAY,            //When arr is NULL
    DA_INVALID_PARAMS,        //When parameter values are not valid
    DA_MEM_REALLOC_FALIURE,   //When memory coudnt be reallocated to a different size
} daErrorCode;

typedef struct DynamicArray DynamicArray;

//Creates a new heap allocated DynamicArray, if element_size is 0 or capacity is 0 or heap allocation faliure it returns NULL
DynamicArray* daCreate(const unsigned int capacity, const unsigned int element_size);

//Frees the DynamicArray, if arr is NULL it does nothing
void daDestroy(DynamicArray* arr);

//Returns the lenght of DynamicArray, if arr is NULL it returns 0
unsigned int daLen(DynamicArray* arr);

//Returns the capacity of DynamicArray, if arr is NULL it returns 0
unsigned int daCapacity(DynamicArray* arr);

//Returns the element size of DynamicArray, if arr is NULL it returns 0
unsigned int daElementSize(DynamicArray* arr);

//Returns the memory address of DynamicArray[pos], if arr is NULL or pos is invalid returns NULL
void* daGet(DynamicArray* arr, const unsigned int pos);

//Updates the value of DynamicArray[pos] to 'value', if arr is NULL or pos is invalid or value is NULL or value_size not equal to element_size it does nothing
void daSet(DynamicArray* arr, const unsigned int pos, const void* value, const unsigned int value_size);

//Inserts a new element to DynamicArray, if arr is NULL or pos is invalid or value is NULL or value_size not equal to element_size it does nothing
void daInsert(DynamicArray* arr, const unsigned int pos, const void* value, const unsigned int value_size);

//Removes the DynamicArray[pos] element, if arr is NULL or pos is invalid it does nothing
void daPop(DynamicArray* arr, const unsigned int pos);

//Returns the index of the first occurence of key, if key is not present it returns -1, if arr is NULL or key is NULL or key_size not equal to element_size it returns -2
int daSearch(DynamicArray* arr, const void* key, const unsigned int key_size);

//Returns the index of the first occurence of key in sorted DynamicArray, if key is not present it returns -1, if arr is NULL or key is NULL or key_size not equal to element_size it returns -2
int daBinarySearch(DynamicArray* arr, const void* key, const unsigned int key_size, int (*compare)(const void*, const void*));

//Sorts the DynamicArray based on the comparision function using qsort, if arr is NULL or compare is NULL it does nothing
void daSort(DynamicArray* arr, int (*compare)(const void*, const void*));

//Returns the error code for the last performed operation on the DynamicArray, if arr is NULL returns ERROR_CODE_NOT_FOUND
daErrorCode daGetError(const DynamicArray* arr);

//Prints a detailed explaination of the daErrorCode error
void daPrintError(const daErrorCode error);

#endif
