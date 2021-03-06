#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "beach_line.h"

#define MIN(x, y) ((x < y) ? x : y)
#define MAX(x, y) ((x > y) ? x : y)

vertex form( vertex p1, vertex p2 ) {

    vertex fin;
    fin.x = ( -1 * (p2.x - p1.x)) / (p2.y - p1.y);
    fin.y = ((p2.y + p1.y) / 2) - (fin.x * ((p2.x + p1.x) / 2));
    return fin;
}

float distance( vertex v1, vertex v2 ) {

    return sqrtf(powf((v2.x - v1.x), 2) + powf((v2.y - v1.y), 2));
}

/* Returns the s value that causes a break point for this arc */
vertex circumcenter( arc* local ) {

    vertex v1 = local->prev->parent->site;
    vertex v2 = local->parent->site;
    vertex v3 = local->next->parent->site;

    vertex fin;

    if ( v1.y == v2.y ) {

        fin.x = (v1.x + v2.x) / 2;
        vertex mb = form( v2, v3 );
        fin.y = (mb.x * fin.x) + mb.y;

    } else if ( v2.y == v3.y ) {

        fin.x = (v2.x + v3.x) / 2;
        vertex mb = form( v1, v2 );
        fin.y = (mb.x * fin.x) + mb.y;

    } else {

        vertex mb1 = form( v1, v2 );
        vertex mb2 = form( v2, v3 );

        fin.x = ( mb2.y - mb1.y ) / ( mb1.x - mb2.x );
        fin.y = (mb1.x * fin.x) + mb1.y;
    }

    return fin;
}

static inline float quad_x( float f, float h, float p, float k, float m, float b ) {

    return (f * sqrtf(p) * sqrtf( b + ( h * m ) - k + ( m * m * p ) )) + h + (2 * m * p);
}

/* Returns the value of the bp to the right */
float break_point( arc* left, arc* right, float s ) {

    if ( left == NULL ) {

        return INT_MIN;

    } else if ( right == NULL ) {

        return INT_MAX;

    } else {

        vertex vleft = left->parent->site;
        vertex vright = right->parent->site;

        float h = vleft.x;
        float p = ( ( vleft.y - s ) / 2 );
        float k = ( vleft.y - p );

        if ( vleft.y == vright.y ) {
            return (vright.x + vleft.x) / 2 ;      

        } else {
            float m = (-1) * ((vright.x - vleft.x) / ( vright.y - vleft.y ));
            float b = (((pow(vright.x, 2) - pow(vleft.x, 2)) + (pow(vright.y, 2) - pow(vleft.y, 2))) / (2 * (vright.y - vleft.y)));

            float x1 = quad_x(  2.0, h, p, k, m, b );
            float x2 = quad_x( -2.0, h, p, k, m, b );
            float x;

            if ( vleft.y < vright.y ) {
                x = MAX(x1, x2);
            } else {
                x = MIN(x1, x2);
            }

            return x;
        }
    }
}

static inline int no_event( vertex l, vertex c, vertex r ) {

    return (((r.x - l.x) * (c.y - l.y)) - ((r.y - l.y) * (c.x - l.x))) <= 0;
}

void recalculate_vertex_event( arc* local, vertex_list* vlist ) {

    if ( local->next == NULL ||
         local->prev == NULL || 
         no_event( local->prev->parent->site, 
                   local->parent->site, 
                   local->next->parent->site )) {
        
        null_vertex_event( vlist, &(local->pinch) );

    } else {

        vertex v = circumcenter( local );
        float event_s = v.y - distance( v, local->parent->site );
        insert_vertex_event( vlist, &(local->pinch), v.x, v.y, event_s );
    }
}

arc* search_left( arc* segment, float x, float s ) {

    if ( break_point( segment->prev, segment, s ) < x ) {
        return segment;
    } else {
        return search_left( segment->prev, x, s );
    }
}

arc* search_right( arc* segment, float x, float s ) {

    if ( break_point( segment, segment->next, s ) > x ) {
        return segment;
    } else {
        return search_right( segment->next, x, s );
    }
}

arc* create_new_arc( face* parent ) {

    arc* new_arc = (arc*) malloc( sizeof(arc) );
    new_arc->parent = parent;
    new_arc->next = NULL;
    new_arc->prev = NULL;
    new_arc->pinch = NULL;

    new_arc->reverse = create_edge( parent );
    
    new_arc->reverse->next = parent->top_edge;
    parent->top_edge->next = new_arc->reverse;

    return new_arc;
}

arc* duplicate_arc( face* parent ) {

    arc* new_arc = (arc*) malloc( sizeof(arc) );
    new_arc->parent = parent;
    new_arc->next = NULL;
    new_arc->prev = NULL;
    new_arc->pinch = NULL;

    new_arc->reverse = create_edge( parent );
    
    return new_arc;
}

void insert_face( arc* segment, face* parent, vertex_list* vlist ) {

    // split the arc
    
    arc* new_center = create_new_arc( parent );
    arc* new_left = duplicate_arc( segment->parent );

    arc* far_left = segment->prev;

    if ( far_left != NULL ) {
        far_left->next = new_left;
    }

    new_left->prev = far_left;
    new_left->next = new_center;
    new_center->prev = new_left;
    new_center->next = segment;
    segment->prev = new_center;

    // split the edge

    edge* left_rev = new_left->reverse;
    edge* center_rev = new_center->reverse;
    
    edge* new_reverse = create_edge( segment->parent );
    edge* exit = segment->reverse->next;

    segment->reverse->next = new_reverse;
    new_reverse->next = left_rev;
    left_rev->next = exit;

    new_reverse->twin = parent->top_edge;
    parent->top_edge->twin = new_reverse;

    parent->top_edge->next = center_rev;
    center_rev->next = parent->top_edge;

    // recalulate vertex_events

    recalculate_vertex_event( new_left, vlist );
    recalculate_vertex_event( segment, vlist );
}

void site_event( line* ln, face* parent, vertex_list* vlist ) {

    if ( ln->head == NULL ) {
        ln->head = create_new_arc( parent );
        return;
    }

    arc* search = ln->head;
    float s = parent->site.y;

    if ( parent->site.x < break_point( search->prev, search, s ) ) {
        
        search = search_left( search->prev, parent->site.x, s );
        insert_face( search, parent, vlist );
        ln->head = ln->head->prev;

    } else if ( parent->site.x > break_point( search, search->next, s ) ) {
        
        search = search_right( search->next, parent->site.x, s );
        insert_face( search, parent, vlist );
        ln->head = ln->head->next;

    } else {
        insert_face( search, parent, vlist );
        ln->head = ln->head->prev;
    }
}

arc* find_left( arc* this, arc* next, vertex_event* v_event, int* bias );

arc* find_right( arc* this, arc* next, vertex_event* v_event, int* bias ) {

    if ( this == NULL ) {
        return find_left( next, NULL, v_event, bias );
    
    } else if ( this->pinch == v_event ) {
        (*bias)--;
        return this;
    
    } else {
        return find_left( next, this->next, v_event, bias );
    }
}

arc* find_left( arc* this, arc* next, vertex_event* v_event, int* bias ) {

    if ( this == NULL ) {
        return find_right( next, NULL, v_event, bias );
    
    } else if ( this->pinch == v_event ) {
        (*bias)++;
        return this;

    } else {
        return find_right( next, this->prev, v_event, bias );
    }
}

void circle_event( line* ln, vertex_event* v_event, vertex_list* vlist ) { 

    arc* search = find_right( ln->head, ln->head->prev, v_event, &(ln->bias) );

    // center arc

    edge* lead = search->next->reverse->next->twin;
    lead->next = search->reverse->next;
    lead->next->origin = search->pinch->v_site;

    // center strike

    edge* strike_right = create_edge( search->next->parent );
    edge* strike_left = create_edge( search->prev->parent );

    strike_right->twin = strike_left;
    strike_left->twin = strike_right;

    // right arc

    edge* right_lead = lead->twin;
    strike_right->next = right_lead;
    search->next->reverse->next = strike_right;
    right_lead->origin = search->pinch->v_site;

    // left arc

    edge* left_lead = lead->next->twin;
    left_lead->next = strike_left;
    strike_left->next = search->prev->reverse;
    strike_left->origin = search->pinch->v_site;

    // arc removal

    arc* right = search->next;
    arc* left = search->prev;

    right->prev = left;
    left->next = right;

    // shift beach line head

    if ( search == ln->head ) {

        ln->head = right;

    } else if ( ln->bias > 1 ) {
        ln->head = right;
        ln->bias = 0;

    } else if ( ln->bias < -1 ) {
        ln->head = left;
        ln->bias = 0;
    }

    free( search->reverse );
    free( search );
    free( v_event );

    recalculate_vertex_event( left, vlist );
    recalculate_vertex_event( right, vlist );
}

line* create_line( vertex ll, vertex tr ) {

    line* beach_line = (line*) malloc( sizeof(line) );
    beach_line->head = NULL;
    beach_line->bias = 0;

    beach_line->top_right = tr;
    beach_line->lower_left = ll;

    return beach_line;
}

void destroy_line( line* ln ) {

    if ( ln->head != NULL ) {

        arc* right = ln->head->next;
        arc* left = ln->head->prev;

        while ( right != NULL ) {

            arc* tmp = right;
            right = right->next;
            free(tmp);
        }

        while ( left != NULL ) {

            arc* tmp = left;
            left = left->prev;
            free(tmp);
        }

        free(ln->head);
    }

    free( ln );
}