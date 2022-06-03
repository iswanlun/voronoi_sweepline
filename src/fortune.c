#include "fortune.h"
#include "stddef.h"
#include "limits.h"

int valid_edge( edge* e, vertex ll, vertex tr ) {

    return  ( e->origin.x >= ll.x ) && 
            ( e->origin.x <= tr.x ) && 
            ( e->origin.y >= ll.y ) &&
            ( e->origin.y <= tr.y ); 
}

void correct_to_bounds( face* f1, face* f2, edge* e, vertex ll, vertex tr ) {

}

void correct_to_edge( edge* start, edge* stop, vertex ll, vertex tr ) {

    if ( start == stop ) {
        return;
    }

    if ( start->origin.x < ll.x ) {
        start->origin.x = ll.x;

    } else if ( start->origin.x > tr.x ) {
        start->origin.x = tr.x;
    }

    if ( start->origin.y < ll.y ) {
        start->origin.y = ll.y;

    } else if ( start->origin.y > tr.y ) {
        start->origin.y = tr.y;
    }

    correct_to_edge( start->next, stop, ll, tr );
}

void bound_face( edge* start, vertex ll, vertex tr ) {

    // find out line else find in line

    edge* out;
    edge* in;

    if ( valid_edge(start, ll, tr) ) {

        edge* index = start;

        while ( valid_edge(index->next, ll, tr) && index->next != start ) {

            index = index->next;
        }

        if ( index->next == start ) {
            return; // when whole diagram complete
        }

        out = index;

        while( !valid_edge(index->next, ll, tr) ) {
            index = index->next;
        }

        in = index;

    } else {

        edge* index = start;

        while ( !valid_edge(index->next, ll, tr) && index->next != start ) {

            index = index->next;
        }

        if ( index->next == start ) {

            // bound edge for all

            // find right

            // find left


            return;
        }

        in = index;

        while( valid_edge(index->next, ll, tr) ) {
            index = index->next;
        }

        out = index;
    }

    // check difference between in and out

    if ( out->next == in ) {

        edge* middle = create_edge( out->home );

        out->next = middle;
        middle->next = in;
        middle->origin = in->origin;
    }

    correct_to_bounds( out->home, out->twin->home, out->next, ll, tr );
    correct_to_bounds( in->home, in->twin->home, in, ll, tr );

    // correct to lines

    correct_to_edge( out->next->next, in, ll, tr );


    // remove duplicates

    bound_face( in->twin, ll, tr );
}

void bound_faces( face_list* list, vertex ll, vertex tr ) {


    // find the first face that is not one of the four borders


    // set bound face on it

    
    // remove four boundary faces
}

face_list* fortunes_sweep_line( vertex* sites, int size, vertex ll, vertex tr ) {

    face_list* list = create_face_list( sites, size, ll, tr );
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