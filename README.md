# C collections

C doesn't have generics... or does it? When you think about it one dirty hack comes to mind - preprocessor macros.

This library is a collection of pseudo-generic collections implemented as such macros. The usage is quite simple
and documented below. You can also go straight to the examples in ```examples``` folder or ```tests```.

It is pure header library - just copy from ```include``` folder collection that interests you.

## Usage

```bash
autoreconf --install
./configure
make
make check
```

## Api

### Vector

```C
VECTOR_DEFINE_ALL(TYPE, SUFFIX)
```

Macro pasting all structures and functions for given ```TYPE```. Structures and functions display having ```SUFFIX``` in name.

```C
struct vector_context_##SUFFIX
{                               
    TYPE *array;                 
    size_t length;               
    size_t capacity;             
};                
```
This is collections state object and all operations use it.


```C
enum vector_status {
    OK,
    OUT_OF_BOUNDS,
    EMPTY,
    NOT_FOUND
};
```
Various statuses returned by the functions.

```C
struct vector_context_##SUFFIX vector_init1_##SUFFIX(size_t capacity)
    
```
Return state object for given capacity.

```C
struct vector_context_##SUFFIX vector_init0_##SUFFIX()
```
Return state object with default capacity.

```C
void vector_destroy_##SUFFIX(struct vector_context_##SUFFIX* ctx)
```

Free allocated resources.


```C
 void vector_push_last_##SUFFIX(struct vector_context_##SUFFIX* ctx, TYPE elem)
```

Append element to the end of the collection.

```C
void vector_push_first_##SUFFIX(struct vector_context_##SUFFIX* ctx, TYPE elem) 
    
```
Append element to the begining of the collection.

```C
enum vector_status vector_pop_last_##SUFFIX(struct vector_context_##SUFFIX* ctx, TYPE* elem)
```
Returns EMPTY if collection doesn't have elements, otherwise sets pointer contents to the last element in array, removes it and returns OK.

```C
enum vector_status vector_pop_first_##SUFFIX(struct vector_context_##SUFFIX* ctx, TYPE* elem)
```
Returns EMPTY if collection doesn't have elements, otherwise sets pointer contents to the first element in array, removes it and returns OK.

```C
enum vector_status vector_remove_##SUFFIX(struct vector_context_##SUFFIX* ctx, int idx, TYPE* elem)  
```

Removes element from given index and returns OK, otherwise returns OUT_OF_BOUNDS.

```C
enum vector_status vector_insert_##SUFFIX(struct vector_context_##SUFFIX* ctx, TYPE elem, int idx)
```

Inserts element at given position and returns OK, otherwise returns OUT_OF_BOUNDS.

```C
enum vector_status vector_index_of4_##SUFFIX(struct vector_context_##SUFFIX* ctx, TYPE elem, int* idx, int (*equals)(TYPE lhs, TYPE rhs))
```

Searches index of element given pointer to index to be filled and equals function and returns OK. Returns NOT_FOUND if element not present.

```C
enum vector_status vector_index_of3_##SUFFIX(struct vector_context_##SUFFIX* ctx, TYPE elem, int* idx) 
    
```

Searches index of element with memory equality.

```C
TYPE vector_reduce_##SUFFIX(struct vector_context_##SUFFIX* ctx, TYPE acc, TYPE (*reduce_op)(TYPE lhs, TYPE rhs))   
```

Given initial value in acc, reduces collection to single value with reduce operator.

```C
void vector_filter_##SUFFIX(struct vector_context_##SUFFIX* ctx, int (*pred)(TYPE elem))
```

Filters collection given the predicate.

```C
void vector_map_##SUFFIX(struct vector_context_##SUFFIX* ctx, TYPE (*map_op)(TYPE elem))  
```

Maps collection given mapping operation.

```C
struct vector_context_##SUFFIX vector_clone_##SUFFIX(struct vector_context_##SUFFIX* ctx)
```

Creates shallow clone of the collection.

```C
enum vector_status vector_get_##SUFFIX(struct vector_context_##SUFFIX* ctx, int idx, TYPE* elem)
```

```C
enum vector_status vector_set_##SUFFIX(struct vector_context_##SUFFIX* ctx, int idx, TYPE elem)
```

```C
char* vector_to_string_##SUFFIX(struct vector_context_##SUFFIX* ctx, char* (*fmt)(TYPE el)) 
```

```C
void vector_sort_##SUFFIX(struct vector_context_##SUFFIX* ctx, int (*compare)(TYPE lhs, TYPE rhs))
```

Sorts the collection given compare function.