#ifndef AGNI_COMMON_H
#define AGNI_COMMON_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

// Error codes
#define AGNI_OK 0
#define AGNI_ERROR_NULL_POINTER -1
#define AGNI_ERROR_INVALID_INPUT -2
#define AGNI_ERROR_ALLOCATION -3
#define AGNI_ERROR_TIMEOUT -4

// Logging macros
#define LOG_INFO(fmt, ...) printf("[INFO] " fmt "\n", ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) printf("[WARN] " fmt "\n", ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) fprintf(stderr, "[ERROR] " fmt "\n", ##__VA_ARGS__)

// Memory utilities
#define SAFE_FREE(ptr) do { if (ptr) { free(ptr); ptr = NULL; } } while(0)

// Math utilities
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define CLAMP(x, min, max) MAX(min, MIN(x, max))

// Vector sizes
#define MAMBA_HIDDEN_SIZE 768
#define MAMBA_STATE_SIZE 16
#define INPUT_BUFFER_SIZE (1024 * 1024)  // 1MB

#endif // AGNI_COMMON_H
