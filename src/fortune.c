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

edge* bound_face( edge* out, edge* stop, float line, int is_x ) {

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

// test face to detirmine if one of four
int is_bound( face* f, vertex* list ) {

    if ( f == NULL ) {
        return 0;
    }

    int test = 0;

    for ( int i = 0; i < 4; i++ ) {

        test |= ((f->site.x == list[i].x) && (f->site.y == list[i].y));
    }

    return test;
}

void bound_faces( face_list* list, vertex ll, vertex tr ) {

    // smash in four corners

    face* top_bound = &(list->collection[0]);

    edge* out_edge[4];
    edge* e = &(top_bound->top_edge);

    for ( int i = 0; i < 4; i++ ) {

        while ( 0 == is_bound(e->twin, list->bounds) ) {
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