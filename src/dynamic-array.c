#include "../include/dynamic-array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct DynamicArray{
    void* data;                //Pointer to the underlying array storing the data
    unsigned int element_size; //The number of bytes taken by a single element
    unsigned int length;       //Current number of elements in the array
    unsigned int capacity;     //Maximum number of elements that can be stored in underlying array
    daErrorCode error;         //Every func must update its value irrespective of whether err or not
};

static daErrorCode daAdjustCapacity(DynamicArray* arr){
    if(arr == NULL){
        return DA_NULL_ARRAY;
    }
    if(arr->length >= arr->capacity){
        void* tmp = realloc(arr->data, ((arr->capacity * 2) + 1) * arr->element_size);
        if(tmp == NULL){
            tmp = realloc(arr->data, (arr->capacity + 1) * arr->element_size);
            if(tmp == NULL){
                return DA_MEM_REALLOC_FALIURE;
            }
        }
        arr->data = tmp;
        arr->capacity = (arr->capacity * 2) + 1;
        return DA_NIL;
    }
    if(arr->length < arr->capacity / 2){
        void* tmp = realloc(arr->data, (arr->capacity / 2) * arr->element_size);
        if(tmp == NULL){
            return DA_MEM_REALLOC_FALIURE;
        }
        arr->data = tmp;
        arr->capacity /= 2;
        return DA_NIL;
    }
    return DA_NIL;
}

static daErrorCode daSetError(DynamicArray* arr, daErrorCode error){
    if(arr == NULL){
        return DA_NULL_ARRAY;
    }
    arr->error = error;
    return DA_NIL;
}

DynamicArray* daCreate(const unsigned int capacity, const unsigned int element_size){
    if(element_size == 0 || capacity == 0){
        return NULL;
    }
    DynamicArray* arr = calloc(1, sizeof(DynamicArray));
    if(arr == NULL){
        return NULL;
    }
    arr->data = calloc(capacity, element_size);
    if(arr->data == NULL){
        free(arr);
        return NULL;
    }
    arr->capacity = capacity;
    arr->element_size = element_size;
    arr->length = 0;
    daSetError(arr, DA_NIL);
    return arr;
}

void daDestroy(DynamicArray* arr){
    if(arr == NULL){
        return;
    }
    free(arr->data);
    free(arr);
}

unsigned int daLen(DynamicArray* arr){
    if(arr == NULL){
        return 0;
    }
    daSetError(arr, DA_NIL);
    return arr->length;
}

unsigned int daCapacity(DynamicArray* arr){
    if(arr == NULL){
        return 0;
    }
    daSetError(arr, DA_NIL);
    return arr->capacity;
}

unsigned int daElementSize(DynamicArray* arr){
    if(arr == NULL){
        return 0;
    }
    daSetError(arr, DA_NIL);
    return arr->element_size;
}

void* daGet(DynamicArray* arr, const unsigned int pos){
    if(arr == NULL){
        return NULL;
    }
    if(pos >= daLen(arr)){
        daSetError(arr, DA_INVALID_PARAMS);
        return NULL;
    }
    daSetError(arr, DA_NIL);
    return (void*)((unsigned char*)arr->data + (pos * daElementSize(arr)));
}

void daSet(DynamicArray* arr, const unsigned int pos, const void* value, const unsigned int value_size){
    if(arr == NULL){
        return;
    }
    if(pos >= daLen(arr) || value == NULL || value_size != daElementSize(arr)){
        daSetError(arr, DA_INVALID_PARAMS);
        return;
    }
    memcpy((unsigned char*)arr->data + (pos * daElementSize(arr)), value, daElementSize(arr));
    daSetError(arr, DA_NIL);
}

void daInsert(DynamicArray* arr, const unsigned int pos, const void* value, const unsigned int value_size){
    if(arr == NULL){
        return;
    }
    if(pos > daLen(arr) || value == NULL || value_size != daElementSize(arr)){
        daSetError(arr, DA_INVALID_PARAMS);
        return;
    }
    if(arr->length == arr->capacity){
        daErrorCode err = daAdjustCapacity(arr);
        if(err != DA_NIL){
            daSetError(arr, err);
            return;
        }
    }
    arr->length++;
    for(unsigned int i = arr->length - 1; i != pos; i--){
        daSet(arr, i, daGet(arr, i - 1), arr->element_size);
    }
    daSet(arr, pos, value, value_size);
    daSetError(arr, DA_NIL);
}

void daPop(DynamicArray* arr, const unsigned int pos){
    if(arr == NULL){
        return;
    }
    if(pos >= daLen(arr)){
        daSetError(arr, DA_INVALID_PARAMS);
        return;
    }
    for(unsigned int i = pos; i != arr->length - 1; i++){
        daSet(arr, i, daGet(arr, i + 1), arr->element_size);
    }
    arr->length--;
    daErrorCode err = daAdjustCapacity(arr);
    if(err != DA_NIL){
        daSetError(arr, err);
        return;
    }
    daSetError(arr, DA_NIL);
}

int daSearch(DynamicArray* arr, const void* key, const unsigned int key_size){
    if(arr == NULL){
        return -2;
    }
    if(key == NULL || key_size != daElementSize(arr)){
        daSetError(arr, DA_INVALID_PARAMS);
        return -2;
    }
    int index = -1;
    for(unsigned int i = 0; i != arr->length; i++){
        if(memcmp(daGet(arr, i), key, arr->element_size) == 0){
            index = i;
            break;
        }
    }
    daSetError(arr, DA_NIL);
    return index;
}

int daBinarySearch(DynamicArray* arr, const void* key, const unsigned int key_size, int (*compare)(const void*, const void*)){
    if(arr == NULL){
        return -2;
    }
    if(key == NULL || key_size != daElementSize(arr)){
        daSetError(arr, DA_INVALID_PARAMS);
        return -2;
    }
    void* element = bsearch(key, arr->data, arr->length, daElementSize(arr), compare);
    int index = -1;
    if(element != NULL){
        index = ((unsigned char*)element - (unsigned char*)arr->data) / daElementSize(arr);
    }
    daSetError(arr, DA_NIL);
    return index;
}

void daSort(DynamicArray* arr, int (*compare)(const void*, const void*)){
    if(arr == NULL){
        return;
    }
    qsort(arr->data, arr->length, arr->element_size, compare);
    daSetError(arr, DA_NIL);
}

daErrorCode daGetError(const DynamicArray* arr){
    if(arr == NULL){
        return DA_NULL_ARRAY;
    }
    return arr->error;
}

void daPrintError(const daErrorCode error){
    switch (error){
        case DA_NIL:
            printf("No Error\n");
            break;
        default:
            printf("IDK :(\n");
    }
}
