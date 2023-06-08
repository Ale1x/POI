#include "city.h"

#define MAX_SIZE 6
#define M_TRANSPORT 3
/*In tutti i vettori che hanno dimensione "M_TRANSPORT"
 * la posizone 1=auto (a) ,2=autobus (A), 3=bicicletta (b);
 */


// Definizione della struttura di una strada
struct _road {
    struct _POI* POI_src;
    struct _POI* POI_dest;
    bool m_transport[M_TRANSPORT];
    int necessary_t[M_TRANSPORT];
};

// Definizione della struttura di un punto di interesse
struct _POI {
    char* name;
    int ID_POI;
    int n_visit[M_TRANSPORT];
    int n_roads;
    struct _road* roads;
};

// Definizione della struttura di una città
struct _city {
    char* name;
    int n_POI;
    int max_POI;
    struct _POI* POI;
    bool** roads;
};

// Funzione che restituisce il punto di interesse con un determinato ID all'interno di una città
POI get_POI(city c, int POI_ID) {
    for (int i = 0; i < c->n_POI; ++i) {
        if (c->POI[i].ID_POI == POI_ID)
            return &(c->POI[i]);
    }
    return NULL;  // Restituisce NULL se il POI con l'ID specificato non viene trovato
}

// Funzione che restituisce l'indice di un punto di interesse all'interno di una città
int get_POI_index(city _city, POI POI_item) {
    for (int i = 0; i < _city->n_POI; i++) {
        if (_city->POI[i].ID_POI == POI_item->ID_POI) {
            return i;  // Restituisce l'indice se il punto di interesse è trovato
        }
    }
    return -1;  // Restituisce -1 se il punto di interesse non è trovato
}

// Funzione per creare una nuova città
city create_city(char* city_name) {
    city c = (city) malloc(sizeof(city));

    c->name = city_name;
    c->n_POI = 0;
    c->max_POI = MAX_SIZE;
    c->POI = (POI)malloc(sizeof(POI) * MAX_SIZE);

    c->roads = (bool**) malloc(sizeof(bool*) * MAX_SIZE);
    for (int i = 0; i < MAX_SIZE; i++) {
        c->roads[i] = (bool*)malloc(sizeof(bool) * MAX_SIZE);
        for (int j = 0; j < MAX_SIZE; j++) {
            c->roads[i][j] = false;
        }
    }
    return c;
}

// Funzione interna per creare un nuovo punto di interesse
void create_POI_internal(POI* new_POI, char* POI_name, int n_POI) {
    new_POI[n_POI]->name = POI_name;
    new_POI[n_POI]->ID_POI = n_POI - 1;

    for (int i = 0; i < M_TRANSPORT; ++i) {
        new_POI[n_POI]->n_visit[i] = 0;
    }

    new_POI[n_POI]->roads = NULL;
    new_POI[n_POI]->n_roads = 0;
}

// Funzione per creare un nuovo punto di interesse all'interno di una città
int create_POI(city _city, char* POI_name) {
    if (_city->n_POI < _city->max_POI) {
        create_POI_internal(&_city->POI, POI_name, _city->n_POI);
        _city->n_POI++;
        return _city->n_POI - 1;
    } else {
        return -1;
    }
}

// Funzione che verifica se una strada esiste tra due punti di interesse all'interno di una città
bool road_exists(city _city, POI POI_src, POI POI_dest) {
    int index_src = get_POI_index(_city, POI_src);
    int index_dest = get_POI_index(_city, POI_dest);

    if (index_src == -1 || index_dest == -1) {
        return false;
    }

    return _city->roads[index_src][index_dest];
}

// Funzione interna per creare una nuova strada
void create_road_internal(city c, POI* POI_src, POI* POI_dest, char* _m_transport, int* times) {
    int id_src = (*POI_src)->ID_POI;
    int id_dest = (*POI_dest)->ID_POI;

    c->roads[id_src][id_dest] = true;

    (*POI_src)->n_roads++;
    (*POI_src)->roads = (road) malloc(sizeof(road) * (*POI_src)->n_roads);
    (*POI_src)->roads[(*POI_src)->n_roads - 1].POI_src = *POI_src;
    (*POI_src)->roads[(*POI_src)->n_roads - 1].POI_dest = *POI_dest;

    for (int i = 0; i < M_TRANSPORT; ++i) {
        switch (_m_transport[i]) {
            case 'a':
            case 'A':
            case 'b':
                (*POI_src)->roads[(*POI_src)->n_roads - 1].m_transport[i] = true;
                break;
            default:
                (*POI_src)->roads[(*POI_src)->n_roads - 1].m_transport[i] = false;
                break;
        }
    }

    for (int i = 0; i < M_TRANSPORT; ++i) {
        (*POI_src)->roads[(*POI_src)->n_roads - 1].necessary_t[i] = times[i];
    }

    (*POI_dest)->n_roads++;
    (*POI_dest)->roads = (road) malloc(sizeof(road) * (*POI_dest)->n_roads);
    (*POI_dest)->roads[(*POI_dest)->n_roads - 1].POI_src = *POI_src;
    (*POI_dest)->roads[(*POI_dest)->n_roads - 1].POI_dest = *POI_dest;

    for (int i = 0; i < M_TRANSPORT; ++i) {
        switch (_m_transport[i]) {
            case 'a':
            case 'A':
            case 'b':
                (*POI_dest)->roads[(*POI_dest)->n_roads - 1].m_transport[i] = true;
                break;
            default:
                (*POI_dest)->roads[(*POI_dest)->n_roads - 1].m_transport[i] = false;
                break;
        }
    }

    for (int i = 0; i < M_TRANSPORT; ++i) {
        (*POI_dest)->roads[(*POI_dest)->n_roads - 1].necessary_t[i] = times[i];
    }
}

// Funzione che verifica se un punto di interesse esiste all'interno di una città
bool POI_exists(city c, POI POI_check) {
    return (POI_check->ID_POI >= 0 && POI_check->ID_POI < c->n_POI);
}

// Funzione per creare una nuova strada tra due punti di interesse all'interno di una città
int create_road(city _city, POI* POI_src,  POI* POI_dest, char* m_transport, int* times, bool one_way) {
    if (!POI_exists(_city, *POI_src) || !POI_exists(_city, *POI_dest)) {
        return -1;
    } else {
        if (one_way) {
            if (!road_exists(_city, *POI_src, *POI_dest)) {
                create_road_internal(_city, POI_src, POI_dest, m_transport, times);
            }
        } else {
            if (!road_exists(_city, *POI_src, *POI_dest)) {
                create_road_internal(_city, POI_src, POI_dest, m_transport, times);
            }
            if (!road_exists(_city, *POI_dest, *POI_src)) {
                create_road_internal(_city, POI_dest, POI_src, m_transport, times);
            }
        }
        return 1;
    }
}