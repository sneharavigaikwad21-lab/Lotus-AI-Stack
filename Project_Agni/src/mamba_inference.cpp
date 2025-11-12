#include "mamba_block.h"
#include "common.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

int mamba_block_init(MambaState* state, size_t hidden_size,
                     size_t state_size, size_t seq_len) {
    if (!state) return AGNI_ERROR_NULL_POINTER;

    state->hidden_size = hidden_size;
    state->state_size = state_size;
    state->seq_len = seq_len;

    // Allocate matrices
    size_t A_size = hidden_size * state_size;
    size_t B_size = seq_len * state_size;
    size_t C_size = seq_len * state_size;

    state->A = (double*)malloc(A_size * sizeof(double));
    state->B = (double*)malloc(B_size * sizeof(double));
    state->C = (double*)malloc(C_size * sizeof(double));
    state->D = (double*)malloc(hidden_size * sizeof(double));
    state->dt = (double*)malloc(seq_len * sizeof(double));

    if (!state->A || !state->B || !state->C || !state->D || !state->dt) {
        LOG_ERROR("Memory allocation failed");
        return AGNI_ERROR_ALLOCATION;
    }

    // Initialize with zeros
    memset(state->A, 0, A_size * sizeof(double));
    memset(state->B, 0, B_size * sizeof(double));
    memset(state->C, 0, C_size * sizeof(double));
    memset(state->D, 0, hidden_size * sizeof(double));
    memset(state->dt, 0, seq_len * sizeof(double));

    LOG_INFO("Mamba block initialized: hidden_size=%zu, state_size=%zu",
             hidden_size, state_size);

    return AGNI_OK;
}

void mamba_block_forward(MambaState* state, const double* x, double* y) {
    if (!state || !x || !y) return;

    // Simplified Mamba forward pass
    // Full implementation would include state transitions

    for (size_t i = 0; i < state->seq_len; i++) {
        for (size_t j = 0; j < state->hidden_size; j++) {
            double sum = 0.0;
            for (size_t k = 0; k < state->state_size; k++) {
                size_t a_idx = j * state->state_size + k;
                size_t b_idx = i * state->state_size + k;
                sum += state->A[a_idx] * state->B[b_idx];
            }
            y[i * state->hidden_size + j] = sum;
        }
    }
}

void mamba_block_free(MambaState* state) {
    if (!state) return;

    SAFE_FREE(state->A);
    SAFE_FREE(state->B);
    SAFE_FREE(state->C);
    SAFE_FREE(state->D);
    SAFE_FREE(state->dt);

    LOG_INFO("Mamba block freed");
}
