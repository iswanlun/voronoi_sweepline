#include "fortune.h"
#include "stddef.h"
#include "limits.h"

#define VALID_EDGE(e) (e->origin.x != INT_MIN && e->origin.y != INT_MIN)



void bound_faces( face_list* list, vertex ll, vertex tr ) {

    
    // remove four excess faces and push vertices to bounds

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