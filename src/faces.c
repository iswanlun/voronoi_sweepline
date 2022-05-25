#include "faces.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>

face* merge_faces( face* left, int lsize, face* right, int rsize ) {

    face arr[lsize + rsize];
    int index = 0, l = 0, r = 0;

    while ( l < lsize && r < rsize ) {

        if ( left[l].site.y > right[r].site.y ) {

            arr[index++] = left[l++];

        } else {
            arr[index++] = right[r++];
        }
    }

    if ( l < lsize ) {
        while (l < lsize ) {
            arr[index++] = left[l++];
        }

    } else {
        while( r < rsize ) {
            arr[index++] = right[r++];
        }
    }

    memcpy( left, arr, ((lsize + rsize) * sizeof(face)) );
    return left;
}

face* sort_faces( face* list, int size ) {

    if ( size < 2 ) {
        return list;

    } else {

        int len = (size/2);

        face* left = sort_faces( list, len );
        face* right = sort_faces( &(list[len]), size-len );

        return merge_faces( left, len, right, size-len );
    }
}

face_list* create_face_list( vertex* sites, int size ) {

    face_list* fin = (face_list*) malloc( sizeof(face_list) );
    fin->size = size;
    fin->index = 0;

    fin->collection = (face*) calloc( sizeof(face), size );

    for ( int i = 0; i < size; i++ ) {
        fin->collection[i].site = sites[i];
        fin->collection[i].top_edge.next = NULL;
        fin->collection[i].top_edge.home = &(fin->collection[i]);
    }

    fin->collection = sort_faces( fin->collection, size );
    return fin;
}

face* peek_next_face( face_list* list ) {

    if ( list->index < list->size ) {
        return &(list->collection[list->index]);
    } else {
        return NULL;
    }
}

face* pop_next_face( face_list* list ) {

    if ( list->index < list->size ) {
        
        face* next = &(list->collection[list->index]);
        list->index++;
        return next;

    } else {
        return NULL;
    }
}

void destroy_face_list( face_list* list ) {

    for ( int i = 0; i < list->size; i++ ) {
        remove_face_edges( &(list->collection[i]) );
    }

    free(list->collection);
    free(list);
}

void remove_face_edges( face* f ) {

    edge* index = f->top_edge.next;

    while ( index != NULL && index != &(f->top_edge) ) {
        edge* next = index->next;
        free(index);
        index = next;
    }
}

edge* create_edge( face* parent ) {

    edge* fin = (edge*) malloc( sizeof(edge) );

    fin->next = NULL;
    fin->twin = NULL;
    fin->home = parent;
    
    fin->origin.x = INT_MIN;
    fin->origin.y = INT_MIN;

    return fin;
}