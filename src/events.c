#include <stdlib.h>
#include "events.h"
#include "stddef.h"

void sort_vertex_list( vertex_list* list ) {

}

void insert_vertex_event( vertex_list* list, vertex_event** event, float v_x, float v_y, float sweep_y ) {

    if ( *event == NULL ) {
        *event = (vertex_event*) malloc( sizeof(vertex_event) );
        (*event)->next = list->head;
        list->head = (*event);
    }

    (*event)->sweep_y = sweep_y;
    (*event)->v_site.x = v_x;
    (*event)->v_site.y = v_y;
    
    sort_vertex_list( list );
}
