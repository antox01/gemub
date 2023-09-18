#include "../includes/gpu.h"
#include "../includes/memory.h"
#include <stdlib.h>

struct gpu_s {
    memory_t memory;
};

gpu_t initGpu(memory_t memory) {
    gpu_t gpu = malloc(sizeof(*gpu));

    gpu->memory = memory;
    return gpu;
}

void freeGpu(gpu_t gpu) {
    if(gpu == NULL) {
        return;
    }

    gpu->memory = NULL;
    free(gpu);
}
