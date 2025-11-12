#ifndef MAMBA_BLOCK_H_R6
#define MAMBA_BLOCK_H_R6

#include "rvv_1024.h"
#include <stddef.h>
#include <stdint.h>

typedef struct {
    double* A;           // [hidden_size, state_size]
    double* B;           // [seq_len, state_size]
    double* C;           // [seq_len, state_size]
    double* h;           // [batch, hidden_size, state_size]
    double* u;           // [batch, seq_len, hidden_size]
} MambaBlockParams;

typedef struct {
    size_t hidden_size;
    size_t state_size;
    size_t seq_len;
    double* A;
    double* B;
    double* C;
    double* D;
    double* dt;
} MambaState;

int mamba_block_init(MambaState* state, size_t hidden_size,
                     size_t state_size, size_t seq_len);

void mamba_block_forward(MambaState* state, const double* x, double* y);

void mamba_block_free(MambaState* state);

#endif // MAMBA_BLOCK_H_R6
