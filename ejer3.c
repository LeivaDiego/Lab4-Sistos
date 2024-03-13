#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_PROCESOS 5
#define NUM_RECURSOS 3

int recursos_disponibles[NUM_RECURSOS];
int max[NUM_PROCESOS][NUM_RECURSOS];
int asignacion[NUM_PROCESOS][NUM_RECURSOS];
int necesidad[NUM_PROCESOS][NUM_RECURSOS];

bool verificarEstadoSeguro() {
    int trabajo[NUM_RECURSOS];
    bool terminado[NUM_PROCESOS];
    for (int i = 0; i < NUM_RECURSOS; i++) {
        trabajo[i] = recursos_disponibles[i];
    }
    for (int i = 0; i < NUM_PROCESOS; i++) {
        terminado[i] = false;
    }
    for (int k = 0; k < NUM_PROCESOS; k++) {
        for (int i = 0; i < NUM_PROCESOS; i++) {
            if (!terminado[i]) {
                bool puede_proceder = true;
                for (int j = 0; j < NUM_RECURSOS; j++) {
                    if (necesidad[i][j] > trabajo[j]) {
                        puede_proceder = false;
                        break;
                    }
                }
                if (puede_proceder) {
                    for (int j = 0; j < NUM_RECURSOS; j++) {
                        trabajo[j] += asignacion[i][j];
                    }
                    terminado[i] = true;
                }
            }
        }
    }
    for (int i = 0; i < NUM_PROCESOS; i++) {
        if (!terminado[i]) {
            return false;
        }
    }
    return true;
}

bool solicitarRecursos(int idProceso, int solicitud[]) {
    for (int i = 0; i < NUM_RECURSOS; i++) {
        if (solicitud[i] > necesidad[idProceso][i] || solicitud[i] > recursos_disponibles[i]) {
            return false;
        }
    }
    for (int i = 0; i < NUM_RECURSOS; i++) {
        recursos_disponibles[i] -= solicitud[i];
        asignacion[idProceso][i] += solicitud[i];
        necesidad[idProceso][i] -= solicitud[i];
    }
    if (verificarEstadoSeguro()) {
        return true;
    } else {
        for (int i = 0; i < NUM_RECURSOS; i++) {
            recursos_disponibles[i] += solicitud[i];
            asignacion[idProceso][i] -= solicitud[i];
            necesidad[idProceso][i] += solicitud[i];
        }
        return false;
    }
}

void liberarRecursos(int idProceso, int liberacion[]) {
    for (int i = 0; i < NUM_RECURSOS; i++) {
        recursos_disponibles[i] += liberacion[i];
        asignacion[idProceso][i] -= liberacion[i];
        necesidad[idProceso][i] += liberacion[i];
    }
}

int main() {
    for (int i = 0; i < NUM_PROCESOS; i++) {
        for (int j = 0; j < NUM_RECURSOS; j++) {
            necesidad[i][j] = max[i][j] - asignacion[i][j];
        }
    }
    int solicitud[NUM_RECURSOS] = {1, 0, 2};
    if (solicitarRecursos(1, solicitud)) {
        printf("La solicitud para el proceso 1 ha sido concedida.\n");
    } else {
        printf("La solicitud para el proceso 1 ha sido denegada.\n");
    }
    int liberacion[NUM_RECURSOS] = {0, 1, 1};
    liberarRecursos(1, liberacion);
    printf("El proceso 1 ha liberado algunos recursos.\n");
    if (verificarEstadoSeguro()) {
        printf("El sistema está en un estado seguro.\n");
    } else {
        printf("El sistema no está en un estado seguro.\n");
    }
    return 0;
}
