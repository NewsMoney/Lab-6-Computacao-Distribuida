#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_SIZE 100
#define NUM_PROCESSES 5

int main(int argc, char *argv[]) {
    int rank, size;

    // Inicializa o ambiente MPI
    MPI_Init(&argc, &argv);

    // Obtém o rank do processo
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Obtém o número total de processos
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Verifica se o número de processos é exatamente 5
    if (size != NUM_PROCESSES) {
        if (rank == 0) {
            printf("Erro: este programa deve ser executado com %d processos.\n", NUM_PROCESSES);
        }

        MPI_Finalize();
        return 1;
    }

    int chunk_size = DATA_SIZE / NUM_PROCESSES;

    int original_data[DATA_SIZE];
    int transformed_data[DATA_SIZE];

    int local_data[chunk_size];

    // Apenas o processo 0 inicializa o vetor original
    if (rank == 0) {
        for (int i = 0; i < DATA_SIZE; i++) {
            original_data[i] = i + 1;
        }

        // Imprime vetor original
        printf("[Processo 0] Vetor original:\n[");

        for (int i = 0; i < DATA_SIZE; i++) {
            printf("%d", original_data[i]);

            if (i < DATA_SIZE - 1) {
                printf(", ");
            }
        }

        printf("]\n\n");
    }

    // Distribui os dados entre os processos
    MPI_Scatter(
        original_data,
        chunk_size,
        MPI_INT,
        local_data,
        chunk_size,
        MPI_INT,
        0,
        MPI_COMM_WORLD
    );

    // Cada processo aplica a transformação x -> x²
    for (int i = 0; i < chunk_size; i++) {
        local_data[i] = local_data[i] * local_data[i];
    }

    // Reúne os dados transformados no processo 0
    MPI_Gather(
        local_data,
        chunk_size,
        MPI_INT,
        transformed_data,
        chunk_size,
        MPI_INT,
        0,
        MPI_COMM_WORLD
    );

    // Processo 0 imprime o vetor transformado
    if (rank == 0) {
        printf("[Processo 0] Vetor transformado:\n[");

        for (int i = 0; i < DATA_SIZE; i++) {
            printf("%d", transformed_data[i]);

            if (i < DATA_SIZE - 1) {
                printf(", ");
            }
        }

        printf("]\n");
    }

    // Finaliza o ambiente MPI
    MPI_Finalize();

    return 0;
}
