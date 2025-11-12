#ifndef AGNI_CONFIG_H
#define AGNI_CONFIG_H

// Configuration constants for Project AGNI

// Model parameters
#define MAMBA_VOCAB_SIZE 50257
#define MAMBA_HIDDEN_SIZE 768
#define MAMBA_NUM_LAYERS 12
#define MAMBA_STATE_SIZE 16
#define MAMBA_HEAD_DIM 64

// Performance targets
#define TARGET_LATENCY_MS 54
#define TARGET_MEMORY_MB 256
#define TARGET_POWER_W 7

// Queue parameters
#define MAX_QUEUE_SIZE 1000
#define MAX_WORKERS 4
#define WORKER_TIMEOUT_MS 30000

// API parameters
#define API_PORT 8080
#define API_MAX_CONNECTIONS 100
#define API_REQUEST_TIMEOUT_MS 60000
#define API_MAX_UPLOAD_SIZE (50 * 1024 * 1024)  // 50MB

#endif // AGNI_CONFIG_H
