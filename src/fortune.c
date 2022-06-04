#include "fortune.h"
#include "stddef.h"
#include "limits.h"

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

void bound_face( edge* out, edge* stop, float line, int is_x ) {

    if ( out->twin == stop ) {
        return;
    }

    edge* in = out->next;

    while ( (is_crosscut( in, line, is_x ) == 0) && in != stop ) {
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

            if ( middle->next == &(middle->home->top_edge) ) {

                edge* new_mid = &(middle->home->top_edge);
                middle->twin->twin = NULL;
                free(middle);
                out->next = new_mid;
                middle = new_mid;
            
            } else {

                edge* new_next = middle->next->next;
                middle->next->twin->twin = NULL;
                free(middle->next);
                middle->next = new_next;
            }
        }
    }

    middle->origin = out->twin->origin;

    if ( in != stop ) {
        in->origin = intersect( in->home->site, in->twin->home->site, line, is_x );
    }

    bound_face( in->twin, stop, line, is_x );
}

void smash_in_corners(  ) {
    
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
}

void bound_faces( face_list* list, vertex ll, vertex tr ) {

    // smash in four corners

    // bound the four edges
    
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