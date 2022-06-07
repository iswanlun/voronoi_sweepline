#include "fortune.h"
#include "stddef.h"
#include "limits.h"

face_list* fortunes_sweep_line( vertex* sites, int size, vertex ll, vertex tr ) {

    face_list* flist = create_face_list( sites, size, ll, tr );
    line* ln = create_line( ll, tr );
    vertex_list* vlist = create_vertex_list();

    while ( peek_vertex_event( vlist ) == NULL ) {

        site_event( ln, pop_next_face( flist ), vlist );
    }

    while ( peek_next_face( flist ) != NULL ) {

        if ( peek_vertex_event( vlist ) == NULL ) {

            site_event( ln, pop_next_face( flist ), vlist );
        
        } else {

            if ( peek_next_face( flist )->site.y > peek_vertex_event( vlist )->sweep_y ) {

                site_event( ln, pop_next_face( flist ), vlist );

            } else {
                circle_event( ln, next_vertex_event( vlist ), vlist );
            }
        }
    }

    while ( peek_vertex_event( vlist ) != NULL ) {

        circle_event( ln, next_vertex_event( vlist ), vlist );
    }

    bound_faces( flist, ll, tr );

    destroy_line( ln );
    dispose_vertex_list( vlist );

    return flist;
}