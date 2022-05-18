#include <stdlib.h>
#include <math.h>
#include "beach_line.h"

line* create_line( vertex ll, vertex tr ) {

    line* beach_line = (line*) malloc( sizeof(line) );

    beach_line->bottom_left_corner = ll;
    beach_line->top_right_corner = tr;

    arc* head = NULL;

    for ( int i = 0; i < 4; i++ ) {
        beach_line->bounds[i] = (face*) malloc( sizeof(face) );
        
        arc* place = create_arc( beach_line->bounds[i] ); 

        place->prev = head;
        head = place;
    }

    arc* last = head;

    while ( head->prev != NULL ) {
        head->prev->next = head;
        head = head->prev;
    }

    head->prev = last;
    last->next = head;

    return beach_line;
}

vertex find_break_point( line* l, arc* left, arc* right ) {


}

vertex circumcenter( arc* a1, arc* a2, arc* a3 ) {

}

inline float point_distance( vertex* p1, vertex* p2 ) {

    return sqrtf(powf((p2->x - p1->x), 2) + powf((p2->y - p2->y), 2));

}

inline float top_intersect( arc* a1, arc* a2, float line_y ) {

    float m = (( -1 * (a2->parent->site.x - a1->parent->site.x)) / (a2->parent->site.y - a1->parent->site.y) );
    float b = ((powf(a2->parent->site.x, 2) - powf(a1->parent->site.x, 2)) + (powf(a2->parent->site.y, 2) - powf(a1->parent->site.y, 2))) / (2 * (a2->parent->site.y - a1->parent->site.y));
    return ((line_y - b) / m);
}

void point_line_v_event( arc* local, line* l, vertex_list* vlist ) {

    vertex site;
    site.x = top_intersect( local, local->next, l->top_right_corner.y );
    site.y = l->top_right_corner.y;

    float y_trigger = site.y - point_distance(&site, &(local->next->parent->site));

    insert_vertex_event(vlist, &(local->pinch), site.x, site.y, y_trigger);

}

void recalculate_vertex_events( arc* local, line* l, vertex_list* vlist ) {

    
}

void insert_segment( line* l, face* parent, vertex_list* vlist ) {

    arc* search_head = l->head;

    vertex bpl = find_break_point( l, search_head->prev, search_head );

    while ( search_head != NULL ) {

        vertex bpr = find_break_point( l, search_head, search_head->next );

        if ( bpl.x <= parent->site.x && parent->site.x < bpr.x ) {
            
            // split the arc
            
            arc* new_center = create_arc( parent );
            arc* new_left = create_arc( search_head->parent );
            arc* far_left = search_head->prev;

            far_left->next = new_left;
            new_left->prev = far_left;

            new_left->next = new_center;
            new_center->prev = new_left;

            new_center->next = search_head;
            search_head->prev = new_center;

            // split the edge

            edge* left_rev = new_left->reverse;
            edge* center_rev = new_center->reverse;
            
            edge* new_reverse = (edge*) malloc( sizeof(edge) );
            edge* exit = search_head->reverse->next;

            search_head->reverse->next = new_reverse;
            new_reverse->next = left_rev;
            left_rev->next = exit;

            new_reverse->twin = &(parent->top_edge);
            parent->top_edge.twin = new_reverse;

            parent->top_edge.next = center_rev;
            center_rev->next = &(parent->top_edge);

            if ( search_head == l->head ) {
                l->head = new_left;
            }

            // recalulate vertex_events

            recalculate_vertex_events( new_left, l , vlist );
            recalculate_vertex_events( new_center, l , vlist );
            recalculate_vertex_events( search_head, l , vlist );

            return;
        }

        bpl = bpr;
        search_head = search_head -> next;
    }
     
    // note an error

}

void pinch_out_segment( line* l, vertex_event* v_event ) {

    arc* search = l->head;

    while ( search != NULL ) {

        if ( search->pinch == v_event ) {

            vertex bp = find_break_point( l, search, search->next );

            // center arc

            edge* lead = search->next->reverse->next->twin;

            lead->next = search->reverse->next;
            lead->next->origin = bp;

            // center strike

            edge* strike_right = (edge*) malloc( sizeof(edge) );
            edge* strike_left = (edge*) malloc( sizeof(edge) );

            strike_right->twin = strike_left;
            strike_left->twin = strike_right;

            // right arc

            edge* right_lead = lead->twin;
            right_lead->origin = bp;

            strike_right->next = right_lead;
            search->next->reverse->next = strike_right;

            // left arc

            edge* left_lead = lead->next->twin;
            left_lead->next = strike_left;
            strike_left->origin = bp;
            strike_left->next = search->prev->reverse;

            // arc removal

            arc* right = search->next;
            arc* left = search->prev;

            right->prev = left;
            left->next = right;

            free(search->reverse);
            free(search);

            return;
        }

        search = search -> next;
    } 

    // note an error
}

arc* create_arc( face* parent ) {

    arc* new_arc = (arc*) malloc( sizeof(arc) );
    new_arc->parent = parent;
    new_arc->next = NULL;
    new_arc->prev = NULL;
    new_arc->pinch = NULL;

    new_arc->reverse = (edge*) malloc( sizeof(edge) );
    new_arc->reverse->next = NULL;
    new_arc->reverse->twin = NULL;

    return new_arc;
}

float arc_heading( arc* self, float x, float s, float y) {

}

float arc_wall( arc* self, float x, float s, float y) {
    
}