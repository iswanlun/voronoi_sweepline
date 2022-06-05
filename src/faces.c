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

face_list* create_face_list( vertex* sites, int size, vertex ll, vertex tr ) {

    face_list* fin = (face_list*) malloc( sizeof(face_list) );
    fin->size = (size + 4);
    fin->index = 0;

    fin->collection = (face*) calloc( sizeof(face), (size + 4) );

    for ( int i = 0; i < size; i++ ) {
        fin->collection[i].site = sites[i];
        fin->collection[i].top_edge = create_edge( &(fin->collection[i]) );
    }

    for ( int i = size; i < (size + 4); i++ ) {
        
        fin->collection[i].top_edge = create_edge( &(fin->collection[i]) );
    }

    float height = tr.y - ll.y;
    float width = tr.x - ll.x;

    // 1 - Bounding points to remove
    int index = size;
    fin->collection[index].site.x = ll.x - width; 
    fin->collection[index].site.y = ((ll.y + tr.y) / 2);

    // 2
    index++;
    fin->collection[index].site.x = ((ll.x + tr.x) / 2); 
    fin->collection[index].site.y = tr.y + height;

    // 3
    index++;
    fin->collection[index].site.x = tr.x + width; 
    fin->collection[index].site.y = ((ll.y + tr.y) / 2);

    // 4
    index++;
    fin->collection[index].site.x = ((ll.x + tr.x) / 2); 
    fin->collection[index].site.y = ll.y - height;

    for ( int i = 0; i < 4; i++ ) {
        fin->bounds[i] = &(fin->collection[index]);
        index--;
    }

    fin->collection = sort_faces( fin->collection, (size + 4) );
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

    edge* index = f->top_edge->next;

    while ( index != NULL && index != f->top_edge ) {
        edge* next = index->next;
        free(index);
        index = next;
    }

    free(f->top_edge);
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

vertex intersect( vertex f1, vertex f2, float line, int is_x ) {

    vertex fin;

    if ( f1.y == f2.y ) {

        fin.x = ( f1.x + f2.x ) / 2;
        fin.y = line;

    } else {
        
        float m = ( -1 * (f2.x - f1.x)) / (f2.y - f1.y);
        float b = ((f2.y + f1.y) / 2) - (m * ((f2.x + f1.x) / 2));
        
        if ( is_x ) {

            fin.x = line;
            fin.y = (m * line) + b;

        } else {

            fin.x = ( line - b ) / ((m == 0) ? 1 : m);
            fin.y = line;
        }

    }

    return fin;
}

int is_crosscut( edge* e, float line, int is_x ) {

    int test = 0;

    if ( is_x ) {
        test |= ((e->origin.x > line) ? 1 : 0) ^ ((e->next->origin.x > line) ? 1 : 0);

    } else {
        test |= ((e->origin.y > line) ? 1 : 0) ^ ((e->next->origin.y > line) ? 1 : 0);
    }

    return test;
}

edge* bound_face( edge* out, edge* stop, float line, int is_x ) {

    edge* in = out->next;

    while ( !is_crosscut( in, line, is_x ) && (in != stop) ) {
        in = in->next;
    }

    // check difference between in and out

    edge* middle;

    if ( out->next == in ) {
        middle = create_edge( out->home );
        out->next = middle;
        middle->next = in;

    } else {
        
        middle = out->next;

        while ( middle->next != in ) {

            edge* nn = middle->next->next;

            if ( middle->home->top_edge == middle->next ) {
                middle->home->top_edge = middle;
            }

            free(middle->next);
            middle->next = nn;
        }
    }

    middle->origin = out->twin->origin;

    if ( in != stop ) {
        in->origin = intersect( in->home->site, in->twin->home->site, line, is_x );
        return bound_face( in->twin, stop, line, is_x );
    
    } else {
        return middle;
    }
}

// smash trifecta of vertices to single point
void smash_in_corner( edge* to_set, vertex ll, vertex tr ) {
    
    for ( int i = 0; i < 3; i++ ) {
    
        if ( to_set->origin.x < ll.x ) {
            to_set->origin.x = ll.x;

        } else if ( to_set->origin.x > tr.x ) {
            to_set->origin.x = tr.x;
        }

        if ( to_set->origin.y < ll.y ) {
            to_set->origin.y = ll.y;

        } else if ( to_set->origin.y > tr.y ) {
            to_set->origin.y = tr.y;
        }

        to_set = to_set->twin->next;
    }
}

void bound_faces( face_list* list, vertex ll, vertex tr ) {

    // smash in four corners

    face* top_bound = &(list->collection[0]);

    edge* out_edge[4];
    edge* e = top_bound->top_edge;

    for ( int i = 0; i < 4; i++ ) {

        while ( e->twin == NULL ) {
            e = e->next;
        }

        smash_in_corner( e->next, ll, tr );
        out_edge[i] = e->next->twin;

        e = e->twin->next;
    }

    // bound the four edges

    out_edge[1] = bound_face( out_edge[0], out_edge[1]->next, tr.x, 1);
    out_edge[2] = bound_face( out_edge[1], out_edge[2]->next, ll.y, 0);
    out_edge[3] = bound_face( out_edge[2], out_edge[3]->next, ll.x, 1);
    bound_face( out_edge[3], out_edge[0]->next, tr.y, 0);

    // TODO: remove four boundary faces
}