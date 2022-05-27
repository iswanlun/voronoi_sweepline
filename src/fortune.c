#include "fortune.h"
#include "stddef.h"
#include "limits.h"

#define VALID_EDGE(e) (e->origin.x != INT_MIN && e->origin.y != INT_MIN)

void bound_face( edge* start, vertex ll, vertex tr );

void bound_free_face( edge* start, vertex ll, vertex tr ) {

    // start with a twinned edge

    // unbound faces will intersect two sides, find both intersections

    // create only the points needed for the first twin - null - twin set, leaving a return pathway

    edge* out = start;
    edge* index = out->next;

    while ( index->twin == NULL && index != start ) {

        index = index->next;
    }

    edge* in = index;

    if ( index == start ) { // full circle, no neighbors
        
        if ( index->twin == NULL ) { // single point no slope

        }

    } else {
        // use slope and direction to find intersection for ONE HALF only (up until next twin)

    }


    bound_face( in->twin, ll, tr );
}

void bound_face( edge* start, vertex ll, vertex tr ) {

    // find out line else find in line

    edge* out;
    edge* in;

    if ( VALID_EDGE(start) ) {

        edge* index = start;
        
        while ( VALID_EDGE(index->next) && index->next != start ) {

            index = index->next;
        }

        if ( index->next == start ) {
            return; // when whole diagram complete
        }

        out = index;

        while( !VALID_EDGE(index->next) ) {
            index = index->next;
        }

        in = index;

    } else {

        edge* index = start;
        
        while ( !VALID_EDGE(index->next) && index->next != start ) {

            index = index->next;
        }

        if ( index->next == start ) { 
            bound_free_face( start, ll, tr ); // special case, no vertex polygon
        }

        in = index;

        while( VALID_EDGE(index->next) ) {
            index = index->next;
        }

        out = index;
    }

    // create bisector from out to in

    // remove all other edges

    // set the vertex of the bisector at the bounds

    // set the vertex of in at the bounds

    bound_face( in->twin, ll, tr );
}

void bound_faces( face_list* list, vertex ll, vertex tr ) {

    // find the out line for the first face in face list

}


face_list* fortunes_sweep_line( vertex* sites, int size, vertex ll, vertex tr ) {

    face_list* list = create_face_list( sites, size );
    line* ln = create_line( ll, tr );
    vertex_list* vlist = create_vertex_list();

    vertex_event* v_event = next_vertex_event( vlist );
    face* f_event = pop_next_face( list );

    while ( v_event != NULL || f_event != NULL ) {

        if ( v_event == NULL ) {
            site_event( ln, f_event, vlist );
            f_event = pop_next_face( list );

        } else if ( f_event == NULL ) {
            circle_event( ln, v_event, vlist );
            v_event = next_vertex_event( vlist );

        } else {
            if ( f_event->site.y > v_event->sweep_y ) {
                site_event( ln, f_event, vlist );
                f_event = pop_next_face( list );

            } else {
                circle_event( ln, v_event, vlist );
                v_event = next_vertex_event( vlist );
            }
        }
    }

    bound_faces( list, ll, tr );
    // ???

    destroy_line( ln );
    dispose_vertex_list( vlist );

    return list;
}