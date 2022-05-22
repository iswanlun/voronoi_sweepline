#include <stdlib.h>
#include <math.h>
#include "beach_line.h"

#define DIR(v1, v2) (((v2.x > v1.x) ? 0xF0 : 0x0) & ((v2.y > v2.x) ? 0xF : 0x0))

#define TOLERANCE 0.5

vertex find_break_point( arc* left, float sweep_y ) {

    return left->solve( left, sweep_y );
}

static inline float point_distance( vertex* p1, vertex* p2 ) {

    return sqrtf(powf((p2->x - p1->x), 2) + powf((p2->y - p2->y), 2));
}

float vertex_event_to( arc* local, char prev_direction, float prev_s, float delta_0, float delta_1 ) {

    vertex v1 = find_break_point(local->prev, prev_s + delta_1);
    vertex v2 = find_break_point(local, prev_s + delta_1);

    if (point_distance(&v1, &v2) < TOLERANCE) {
        return prev_s + delta_1;

    } else {

        char new_direction = DIR(v1, v2);

        if (new_direction != prev_direction) { // reversal
            return vertex_event_to( local, new_direction, prev_s + delta_1, 0, (delta_1 / -2) );

        } else {
            return vertex_event_to( local, new_direction, prev_s + delta_1, delta_1, delta_0 + delta_1 );
        }
    }
}

void recalculate_vertex_event( arc* local, vertex_list* vlist, float current_s ) {

    if ( local->next->parent == local->prev->parent || 
         (local->prev->parent->site.y >= local->parent->site.y && local->next->parent->site.y >= local->parent->site.y) ) {
        
        null_vertex_event( vlist, &(local->pinch) );

    } else {

        vertex v1 = find_break_point(local->prev, current_s);
        vertex v2 = find_break_point(local, current_s);

        float adjusted_s = vertex_event_to( local, DIR(v1, v2), current_s, 0, -1 );

        vertex v3 = find_break_point( local, adjusted_s );
    
        insert_vertex_event( vlist, &(local->pinch), v3.x, v3.y, adjusted_s );
    }
}

void insert_segment( line* l, face* parent, vertex_list* vlist, float s ) { // TODO: refactor to recusive search

    arc* search_head = l->head;

    vertex bpl = find_break_point( search_head->prev, s );

    while ( search_head != NULL ) {

        vertex bpr = find_break_point( search_head, s );

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

            new_left->solve = search_head->solve; // correct function pointers
            new_left->eval = search_head->eval;
            new_left->diff = search_head->diff;

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

            recalculate_vertex_event( new_left, vlist, s );
            recalculate_vertex_event( new_center, vlist, s );
            recalculate_vertex_event( search_head, vlist, s );

            return;
        }

        bpl = bpr;
        search_head = search_head -> next;
    }
     
    // note an error
}

void pinch_out_segment( line* l, vertex_event* v_event ) { // TODO: refactor to recursive

    arc* search = l->head;

    while ( search != NULL ) {

        if ( search->pinch == v_event ) {

            // center arc

            edge* lead = search->next->reverse->next->twin;

            lead->next = search->reverse->next;
            lead->next->origin = search->pinch->v_site;

            // center strike

            edge* strike_right = (edge*) malloc( sizeof(edge) );
            edge* strike_left = (edge*) malloc( sizeof(edge) );

            strike_right->twin = strike_left;
            strike_left->twin = strike_right;

            // right arc

            edge* right_lead = lead->twin;
            right_lead->origin = search->pinch->v_site;

            strike_right->next = right_lead;
            search->next->reverse->next = strike_right;

            // left arc

            edge* left_lead = lead->next->twin;
            left_lead->next = strike_left;
            strike_left->origin = search->pinch->v_site;
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

float arc_rec( arc* self, float s, float x, float prev_diff, float delta_0, float delta_1 ) {

    if ( prev_diff > TOLERANCE ) {

        float this_x = x + delta_1;
        float self_y = self->eval( self, s, this_x );
        float this_diff = self->next->diff( self->next, this_x, s, self_y );

        if ( this_diff < 0 ) {
            return arc_rec( self, s, this_x, this_diff, 0, (delta_1 / 2) );
        } else {
            return arc_rec( self, s, this_x, this_diff, delta_1, delta_0 + delta_1 );
        }

    } else if ( prev_diff < -TOLERANCE ) {

        float this_x = x - delta_1;
        float self_y = self->eval( self, s, this_x );
        float this_diff = self->next->diff( self->next, this_x, s, self_y );

        if ( this_diff > 0 ) {
            return arc_rec( self, s, this_x, this_diff, 0, (delta_1 / 2) );
        } else {
            return arc_rec( self, s, this_x, this_diff, delta_1, delta_0 + delta_1 );
        }

    } else {
        return x;
    }
}

float eval_cap( arc* self, float s, float x ) {

    return self->parent->site.y;
}

float diff_cap( arc* self, float x, float s, float y ) {

    return self->parent->site.y - y;
}

vertex solve_cap( arc* self, float s ) {

    vertex fin;
    float x = self->next->parent->site.x;
    fin.y = self->parent->site.y;
    fin.x = arc_rec( self, s, x, self->next->diff(self->next, x, s, fin.y), 0, 1 );
    
    return fin;
}

float eval_wall( arc* self, float s, float x ) { // this should never be used

    return 0;
}

float diff_wall( arc* self, float x, float s, float y ) {

    if ( x < ( self->parent->site.x - TOLERANCE ) ) {
        return 1;

    } else if ( x > ( self->parent->site.x + TOLERANCE )) {
        return -1;

    } else {
        return 0;
    }
}

float eval_arc( arc* self, float s, float x ) {

    float p = ((self->parent->site.y - s) / 2);
    return (powf((x - self->parent->site.x), 2) / (4 * p)) + (self->parent->site.y - p);
}

float diff_arc( arc* self, float x, float s, float y ) {

    return eval_arc( self, s, x ) - y;
}

vertex solve_arc( arc* self, float s ) {

    vertex fin;
    float x;
    if ( self->parent->site.y < self->next->parent->site.y ) {
        x = self->parent->site.x;
    } else {
        x = self->next->parent->site.x;
    }

    fin.y = self->eval( self, s, x );
    fin.x = arc_rec( self, s, x, self->next->diff(self->next, x, s, fin.y), 0, 1 );
    fin.y = self->eval( self, s, fin.x );

    return fin;
}

vertex solve_wall( arc* self, float s ) {

    vertex fin;
    fin.x = self->parent->site.x;
    fin.y = self->next->eval( self->next, s, fin.x );
    return fin;
}

arc* create_arc( face* parent ) {

    arc* new_arc = (arc*) malloc( sizeof(arc) );
    new_arc->parent = parent;
    new_arc->next = NULL;
    new_arc->prev = NULL;
    new_arc->pinch = NULL;

    new_arc->reverse = (edge*) malloc( sizeof(edge) );
    new_arc->reverse->next = &(parent->top_edge);
    new_arc->reverse->twin = NULL;

    parent->top_edge.next = new_arc->reverse;

    new_arc->solve = &(solve_arc);
    new_arc->eval = &(eval_arc);
    new_arc->diff = &(diff_arc);

    return new_arc;
}

line* create_line( vertex ll, vertex tr ) {

    line* beach_line = (line*) malloc( sizeof(line) );
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

    beach_line->head = head;

    // top
    face* util = head->parent;

    util->site.x = ll.x;
    util->site.y = tr.y;

    head->solve = &(solve_cap);
    head->eval = &(eval_cap);
    head->diff = &(diff_cap);

    head = head->next;
    
    // right wall
    util = head->parent;

    util->site.x = tr.x;
    util->site.y = tr.y;

    head->solve = &(solve_wall);
    head->eval = &(eval_wall);
    head->diff = &(diff_wall);
    
    head = head->next;

    // bottom
    util = head->parent;

    util->site.x = tr.x;
    util->site.y = ll.y;

    head->solve = &(solve_cap);
    head->eval = &(eval_cap);
    head->diff = &(diff_cap);
    
    head = head->next;

    // left wall
    util = head->parent;

    util->site.x = ll.x;
    util->site.y = ll.y;

    head->solve = &(solve_wall);
    head->eval = &(eval_wall);
    head->diff = &(diff_wall);

    return beach_line;
}

void destroy_line( line* ln ) {

    arc* first = ln->head;
    arc* index = ln->head->next;

    while( index != NULL && index != first ) {
        arc* next = index->next;
        free(index);
        index = next;
    }

    free(first);

    for ( int i = 0; i < 4; i++ ) {

        remove_face_edges( ln->bounds[i] );
        free( ln->bounds[i] );
    }
}