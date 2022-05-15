#include <stdlib.h>
#include "beach_line.h"

line* create_line( vertex ll, vertex tr ) {

    line* result = (line*) malloc( sizeof(line) );

    result->bottom_left_corner = ll;
    result->top_right_corner = tr;

    // arcs

    result->lbound  = (face*) malloc( sizeof( face ) );
    result->tbound  = (face*) malloc( sizeof( face ) );
    result->rbound  = (face*) malloc( sizeof( face ) );

    // lbound

    edge* l_rev = (edge*) malloc( sizeof(edge) );
    l_rev->origin.x = ll.x;
    l_rev->origin.y = tr.y;
    l_rev->next = &result->lbound->top_edge;
    l_rev->twin = NULL;

    result->lbound->top_edge.origin = ll;
    result->lbound->top_edge.next = l_rev;
    result->lbound->top_edge.twin = NULL;

    // tbound

    edge* t_rev = (edge*) malloc( sizeof(edge) );
    t_rev->origin = tr;
    t_rev->next = &result->tbound->top_edge;
    t_rev->twin = NULL;

    result->tbound->top_edge.origin.x = ll.x;
    result->tbound->top_edge.origin.y = tr.y;
    result->tbound->top_edge.next = t_rev;
    result->tbound->top_edge.twin = NULL;

    // rbound

    edge* r_rev = (edge*) malloc( sizeof(edge) );
    r_rev->origin.x = tr.x;
    r_rev->origin.y = ll.y;
    r_rev->next = &result->rbound->top_edge;
    r_rev->twin = NULL;

    result->rbound->top_edge.origin = tr;
    result->rbound->top_edge.next = r_rev;
    result->rbound->top_edge.twin = NULL;

    // arcs

    result->head    = (arc*) malloc( sizeof(arc) );
    arc* first      = (arc*) malloc( sizeof(arc) );
    arc* second     = (arc*) malloc( sizeof(arc) );

    result->head->next = first;
    result->head->prev = NULL;
    result->head->parent = result->lbound;
    result->head->reverse = l_rev;

    first->prev = result->head;
    first->next = second;
    first->parent = result->tbound;
    first->reverse = t_rev;

    second->prev = first;
    second->next = NULL;
    second->parent = result->rbound;
    second->reverse = r_rev;

    return result;
}

vertex find_break_point( line* l, arc* left, arc* right ) {

    if ( left == NULL ) {
        return right->parent->top_edge.origin;
    } else if ( right == NULL ) {
        return left->parent->top_edge.next->origin;
    } else {

        // import math from tool

    }
}

float intersect_two_points_and_line( arc* local, arc* point, float x ) {

}

float circumcenter_event_y( vertex* v1, vertex* v2, vertex* v3 ) {

    // import math from tool

}

void recalculate_vertex_events( arc* local, line* l, vertex_list* vlist ) {

    if ( local != l->lbound && local != l->rbound ) {

        float y;

        if ( local->prev == l->lbound ) {

            if ( local == l->tbound ) {
                // distance from corner is vertical off off point
            } else {
                // intersection between line and outer bound
            }

        } else if ( local->next == l->rbound ) {

            if ( local == l->tbound ) {
                // distance from corner is vertical off off point
            } else {
                // intersection between line and outer bound
            }

        } else {
            y = circumcenter_event_y(&local->prev->parent->incident, &local->parent->incident, &local->next->parent->incident);
        }

        local->pinch = vertex_insert_event( vlist, local->pinch, y );
    }
}


void insert_segment( line* l, face* parent, vertex_list* vlist ) {

    arc* search_head = l->head;

    while ( search_head != NULL ) {

        vertex bpl = find_break_point( l, search_head->prev, search_head );
        vertex bpr = find_break_point( l, search_head, search_head->next );

        if ( bpl.x <= parent->incident.x && parent->incident.x < bpr.x ) {
            
            // split the arc
            
            arc* new_center = (arc*) malloc( sizeof(arc) );
            arc* new_left = (arc*) malloc( sizeof(arc) );
            arc* far_left = search_head->prev;

            far_left->next = new_left;
            new_left->prev = far_left;

            new_left->next = new_center;
            new_center->prev = new_left;

            new_center->next = search_head;
            search_head->prev = new_center;

            // split the edge

            new_center->parent = parent;
            new_left->parent = search_head->parent;

            edge* left_rev = (edge*) malloc( sizeof(edge) );
            edge* center_rev = (edge*) malloc( sizeof(edge) );
            
            edge* new_reverse = (edge*) malloc( sizeof(edge) );
            edge* exit = search_head->reverse->next;

            search_head->reverse->next = center_rev;
            center_rev->next = left_rev;
            left_rev->next = exit;

            center_rev->twin = &parent->top_edge;
            parent->top_edge.twin = center_rev;
            parent->top_edge.next = new_reverse;
            new_reverse->next = &parent->top_edge;

            new_center->reverse = new_reverse;

            // recalulate vertex_events

            recalculate_vertex_events( new_left, l , vlist );
            recalculate_vertex_events( new_center, l , vlist );
            recalculate_vertex_events( search_head, l , vlist );

            return;
        }

        search_head = search_head -> next;
    }
     
    // note an error

}