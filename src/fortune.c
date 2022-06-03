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

float distance( vertex v1, vertex v2 ) {

    return sqrtf(powf((v2.x - v1.x), 2) + powf((v2.y - v1.y), 2));
}

// result < 0 -> clockwise; result > 0 -> counter-clockwise
float clockwise( vertex a, vertex b, vertex c ) {

    return ((b.x * c.y) + (a.x * b.y) + (a.y * c.x)) - ((a.y * b.x) + (b.y * c.x) + (a.x * c.y));
}

void correct_to_edge( edge* curr, edge* out, edge* in, vertex ll, vertex tr ) {
    
    if ( curr == in ) {
        return;
    }

    if ( curr->origin.x < ll.x ) {
        curr->origin.x = ll.x;

    } else if ( curr->origin.x > tr.x ) {
        curr->origin.x = tr.x;
    }

    if ( curr->origin.y < ll.y ) {
        curr->origin.y = ll.y;

    } else if ( curr->origin.y > tr.y ) {
        curr->origin.y = tr.y;
    }

    if ( (out->origin.x == curr->origin.x && out->origin.y != curr->origin.y) ^ 
         (out->origin.y == curr->origin.y && out->origin.x != curr->origin.x) ) {

        if ( clockwise( out->home->site, out->origin, curr->origin ) > 0 ) { // 

            curr->origin = out->origin;  
        } 
    }

    if ( (in->origin.x == curr->origin.x && in->origin.y != curr->origin.y) ^ 
         (in->origin.y == curr->origin.y && in->origin.x != curr->origin.x) ) {

        if ( clockwise( in->home->site, in->origin, curr->origin ) < 0 ) {

            curr->origin = in->origin;
        }
    }

    correct_to_edge( curr->next, out, in, ll, tr );
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

    correct_to_edge( out->next->next, out->next, in, ll, tr );

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