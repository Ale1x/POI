#include "city.h"

/*In tutti i vettori che hanno dimensione "M_TRANSPORT"
 * la posizone 1=auto (a) ,2=autobus (A), 3=bicicletta (b);
 */

// Definizione della struttura di una strada
struct _road {
    struct _POI *POI_src;
    struct _POI *POI_dest;
    bool m_transport[M_TRANSPORT];
    int necessary_t[M_TRANSPORT];
};

// Definizione della struttura di un punto d'interesse
struct _POI {
    char *name;
    int ID_POI;
    int n_visit[M_TRANSPORT];
    int n_roads;
    struct _road *roads;
};

// Definizione della struttura di una città
struct _city {
    char *name;
    int n_POI;
    int max_POI;
    struct _POI *POI;
    bool **roads;
};
//
char* get_POI_name(POI currentPOI){
     return currentPOI->name;
}

// Funzione che restituisce il punto d'interessecon un determinato ID
// all'interno di una città
POI get_POI(city c, int POI_ID) {
    for (int i = 0; i < c->n_POI; ++i) {
        if (c->POI[i].ID_POI == POI_ID)
            return &(c->POI[i]);
    }
    return NULL;
}

// Funzione che restituisce l'indice di un punto d'interesse all'interno di una città
int get_POI_index(city _city, POI currentPOI) {
    for (int i = 0; i < _city->n_POI; i++) {
        if (_city->POI[i].ID_POI == currentPOI->ID_POI) {
            return i; // Restituisce l'indice se il punto d'interesse è trovato
        }
    }
    return -1; // Restituisce -1 se il punto d'interessenon è trovato
}

// Funzione per creare una nuova città
city create_city(char *city_name) {
    city c = (city)malloc(sizeof(struct _city));

    c->name = malloc(strlen(city_name) + 1);
    strcpy(c->name, city_name);
    c->n_POI = 0;
    c->max_POI = MAX_SIZE;
    c->POI = (POI)malloc(sizeof(struct _POI) * MAX_SIZE);

    c->roads = (bool **)malloc(sizeof(bool *) * MAX_SIZE);
    for (int i = 0; i < MAX_SIZE; i++) {
        c->roads[i] = (bool *)malloc(sizeof(bool) * MAX_SIZE);
        for (int j = 0; j < MAX_SIZE; j++) {
            c->roads[i][j] = false;
        }
    }
    return c;
}

// Funzione interna per creare un nuovo punto d'interesse (Modify create_POI_internal to return the created POI)
POI create_POI_internal(char *POI_name, int n_POI) {
    POI new_POI = (POI)malloc(sizeof(struct _POI));
    new_POI->name = malloc(strlen(POI_name) + 1);
    strcpy(new_POI->name, POI_name);
    new_POI->ID_POI = n_POI;

    for (int i = 0; i < M_TRANSPORT; ++i) {
        new_POI->n_visit[i] = 0;
    }

    new_POI->roads = NULL;
    new_POI->n_roads = 0;

    return new_POI;
}

// Update create_POI to use the modified create_POI_internal function
int create_POI(city _city, char *POI_name) {
    if (_city->n_POI < _city->max_POI) {
        POI new_POI = create_POI_internal(POI_name, _city->n_POI);
        _city->POI[_city->n_POI] = *new_POI;
        _city->n_POI++;
        return _city->n_POI - 1; // Restituisci l'indice effettivo del nuovo punto d'interesse
    } else {
        return -1;
    }
}

// Funzione che verifica se una strada esiste tra due punti d'interesse all'interno di una città, restituisce false
// se la strada non esiste, o se i poi non esistono, altrimenti true
bool road_exists(city _city, POI POI_src, POI POI_dest) {
    int index_src = get_POI_index(_city, POI_src);
    int index_dest = get_POI_index(_city, POI_dest);

    if (index_src == -1 || index_dest == -1) {
        return false;
    }

    return _city->roads[index_src][index_dest];
}

// Funzione interna per creare una nuova strada
void create_road_internal(city c, POI *POI_src, POI *POI_dest,char *_m_transport, int *times) {
    int id_src = (*POI_src)->ID_POI;
    int id_dest = (*POI_dest)->ID_POI;

    c->roads[id_src][id_dest] = true;

    (*POI_src)->n_roads++;
    (*POI_src)->roads = (struct _road *)realloc((*POI_src)->roads, sizeof(struct _road) * (*POI_src)->n_roads);
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
    (*POI_dest)->roads = (struct _road *)realloc((*POI_dest)->roads, sizeof(struct _road) * (*POI_dest)->n_roads);
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

// Funzione che verifica se un punto d'interesse esiste all'interno di una città
bool POI_exists(city c, POI POI_check) {
    if (POI_check == NULL) {
        return false;
    }

    return (POI_check->ID_POI >= 0 && POI_check->ID_POI < c->n_POI);
}

// Funzione per creare una nuova strada tra due punti d'interesse all'interno di una città
int create_road(city _city, POI *POI_src, POI *POI_dest, char *m_transport,int *times, bool one_way) {
    if (!POI_exists(_city, *POI_src) || !POI_exists(_city, *POI_dest)) {
        return -1;
    }

    if (!road_exists(_city, *POI_src, *POI_dest)) {
        create_road_internal(_city, POI_src, POI_dest, m_transport, times);
    }
    if (!one_way) {
        if (!road_exists(_city, *POI_dest, *POI_src)) {
            create_road_internal(_city, POI_dest, POI_src, m_transport, times);
        }
    }

    return 1;
}

//Funzione che verifica se per una data strada (POI_src e POI_dest) 
//puo passare un dato mezzo di trasporto, true se puo passare, false se non puo'
bool Mtransport_road_exists(city _city, POI POI_src, POI POI_dest, char* m_transport){
    int index_src = get_POI_index(_city, POI_src);
    int index_dest = get_POI_index(_city, POI_dest);

    if (index_src == -1 || index_dest == -1) {
        return false;
    }
    if(m_transport[0] == 'a') {
        if(POI_src->roads[index_src].m_transport[0])
            return true;
    }
    if(m_transport[1] == 'A') {
        if(POI_src->roads[index_src].m_transport[1])
            return true;
    }
    if(m_transport[2] == 'b') {
        if(POI_src->roads[index_src].m_transport[2])
            return true;
    }
    return false;
}

//La funzione viene chiamata dalla funzione principale e aggiorna i parametri di
// visita di un determinato POI
void register_trip_internal(city _city, int* path, char* m_transport, int size){
    int index_m_trasport;
    if(m_transport[0] == 'a') {
        index_m_trasport = 0;
    }
    if(m_transport[1] == 'A') {
        index_m_trasport = 1;
    }
    if(m_transport[2] == 'b') {
        index_m_trasport = 2;
    }
    for(int z = 0; z < size ; ++z){
        POI current_POI = get_POI(_city, path[z]);
        current_POI->n_visit[index_m_trasport]+1;
    }
}

//La funzione permette di registrare un viaggo, mane verifica anche la validità
char* register_trip(city _city, int* path, char m_transport[]) {
    char* res = malloc(sizeof(char) * 255); // allocate 255 characters for the result string
    int dim = 0;
    for (int i = 0; path[i] != -1; ++i) {
        dim++;
    }

    // Prima verifica che le strade esistano
    for (int j = 0; j < dim - 1; ++j) {
        POI POI_src = get_POI(_city, path[j]);
        POI POI_dest = get_POI(_city, path[j+1]);
        
        if (!POI_src || !POI_dest) {
            strcpy(res, "Uno dei punti d'interesse nel percorso non esiste!");
            return res;
        }

        if (!road_exists(_city, POI_src, POI_dest)) {
            strcpy(res, "Una delle strade tra i punti d'interesse non esiste!");
             return res;
        }

    }
    
    // Poi verifica i mezzi di trasporto
    bool isValidTransport = false;
    int index_m_transport = -1;
    
    for (int i = 0; m_transport[i] != '\0'; ++i) {
        switch (m_transport[i]) {
            case 'a':
                index_m_transport = 0;
                isValidTransport = true;
                break;
            case 'A':
                index_m_transport = 1;
                isValidTransport = true;
                break;
            case 'b':
                index_m_transport = 2;
                isValidTransport = true;
                break;
        }
    }
    
    if (!isValidTransport) {
        strcpy(res, "Mezzo di trasporto non valido!");
        return res;
    }

    // Verifica se il mezzo di trasporto è consentito sulle strade
    for (int j = 0; j < dim - 1 ; ++j) {
        POI POI_src = get_POI(_city, path[j]);
        
        bool m_transport_allowed = false;
        for (int k = 0; k < POI_src->n_roads; k++) {
            if (POI_src->roads[k].m_transport[index_m_transport]) {
                m_transport_allowed = true;
                break;
            }
        }

        if (!m_transport_allowed) {
            strcpy(res, "Il mezzo di trasporto specificato non è consentito su uno dei tratti!");
            return res;
        }

        POI_src->n_visit[index_m_transport]++;
    }

    strcpy(res, "OK");
    return res;
}






//La funzione prendendo in input la città in questione, calcola il POI in cui sono
//state più persone e lo restituisce come output
POI crowded_POI(city _city, int* nVisitMax) {
    int maxVisits = 0; // Variabile per memorizzare il numero massimo di visite
    int Nvisit1 = 0;
    int MAXid = _city->POI[0].ID_POI;

    for (int i = 0; i < M_TRANSPORT; ++i) {
        maxVisits += _city->POI[0].n_visit[i];
    }

    for (int i = 1; i < _city->n_POI; ++i) {
        Nvisit1 = 0;
        for (int j = 0; j < M_TRANSPORT; ++j) {
            Nvisit1 += _city->POI[i].n_visit[j];
        }
        if (Nvisit1 > maxVisits) {
            maxVisits = Nvisit1;
            MAXid = i;
        }
    }

    *nVisitMax = maxVisits; // Assegna il valore massimo di visite al puntatore nVisitMax

    return get_POI(_city, MAXid);
}

//La funzione scorre tutti i POI, della città, per ognuno analizza la 'road'
// associata e verifica se puo essere raggiunta con tutti i mezzi di trasporto.
//Se tutte le strade di un POI posso essere percorse con tutte le tipologie di mezzi,
// viene controllato se il numero di strade raggiungibili è maggiore di quelle trovate finora.
// Se lo è, il POI diventa il più facilmente raggiungibile trovato finora.
POI ez_reachable(city _city) {
    POI easiest_reachable_POI = NULL;
    int max_reachable_count = 0;

    for (int i = 0; i < _city->n_POI; i++) {
        POI currentPOI = &_city->POI[i];
        int reachable_count = 0;

        for (int j = 0; j < currentPOI->n_roads; j++) {
            road current_road = &currentPOI->roads[j];
            bool reachable = true;

            for (int k = 0; k < M_TRANSPORT; k++) {
                if (!current_road->m_transport[k]) {
                    reachable = false;
                    break;
                }
            }

            if (reachable) {
                // Controlla se tutti i punti d'interesse di destinazione sono tra i POI correnti
                bool all_destinations_reachable = true;
                for (int dest = 0; dest < currentPOI->n_roads; dest++) {
                    road dest_road = &currentPOI->roads[dest];
                    if (dest_road->POI_dest != currentPOI) {
                        bool dest_reachable = false;
                        for (int k = 0; k < M_TRANSPORT; k++) {
                            if (dest_road->m_transport[k]) {
                                dest_reachable = true;
                                break;
                            }
                        }
                        if (!dest_reachable) {
                            all_destinations_reachable = false;
                            break;
                        }
                    }
                }

                if (all_destinations_reachable) {
                    reachable_count++;
                }
            }
        }

        if (reachable_count > max_reachable_count) {
            max_reachable_count = reachable_count;
            easiest_reachable_POI = currentPOI;
        }
    }

    return easiest_reachable_POI;
}



//'main' di prova per testare funzioni

int main() {
    // Creazione della città
    city myCity = create_city("MyCity");

    // Aggiunta di punti d'interesse
    int POI0 = create_POI(myCity, "POI0");
    int POI1 = create_POI(myCity, "POI1");
    int POI2 = create_POI(myCity, "POI2");

    // Stampa dei punti d'interesse
    printf("Punti di interesse:\n");
    for (int i = 0; i < myCity->n_POI; i++) {
        printf("- %s (ID: %d)\n", myCity->POI[i].name, myCity->POI[i].ID_POI);
    }
    printf("\n");

    // Creazione di strade
    POI POI_src = get_POI(myCity, POI0);
    POI POI_dest = get_POI(myCity, POI1);
    char m_transport[] = {'a', 'A', 'b'};
    int times[] = {10, 20, 30};
    create_road(myCity, &POI_src, &POI_dest, m_transport, times, false);

    POI_src = get_POI(myCity, POI1);
    POI_dest = get_POI(myCity, POI2);
    create_road(myCity, &POI_src, &POI_dest, m_transport, times, false);

    // Stampa delle strade
    printf("Strade:\n");
    for (int i = 0; i < myCity->n_POI; i++) {
        for (int j = i + 1; j < myCity->n_POI; j++) {
            if (myCity->roads[i][j]) {
                printf("- Da %s (ID: %d) a %s (ID: %d)\n", myCity->POI[i].name, myCity->POI[i].ID_POI, myCity->POI[j].name, myCity->POI[j].ID_POI);
            }
        }
    }
    printf("\n");


    // Registrazione di un percorso
    int path[] = {0, 1, 2, -1};
    char m_transport_trip[] = {'l', 'A', 'n'};
    char* result = register_trip(myCity, path, m_transport_trip);
    printf("%s\n", result);

    return 0;
}