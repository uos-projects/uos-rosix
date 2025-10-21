/*
 * ROSIX Stream Interface
 * 
 * This file defines the streaming interfaces for real-time data processing
 * and event-driven computation in ROSIX.
 * 
 * Copyright (c) 2024 UOS Project
 * All rights reserved.
 */

#ifndef _ROSIX_STREAM_H
#define _ROSIX_STREAM_H

#include "rosix.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Stream Data Structures
 * ============================================================================ */

/**
 * Stream processing function type
 */
typedef void (*ROSIX_StreamProcessor)(const void* data, size_t size, void* context);

/**
 * Stream configuration structure
 */
typedef struct {
    ResourceHandle source;             /* Source resource handle */
    ROSIX_StreamProcessor process;     /* Processing function */
    void* context;                     /* User context for processing */
    size_t buffer_size;                /* Stream buffer size */
    int max_retries;                   /* Maximum retry attempts */
    int timeout_ms;                    /* Timeout in milliseconds */
} ROSIX_Stream;

/**
 * Stream statistics structure
 */
typedef struct {
    uint64_t bytes_processed;          /* Total bytes processed */
    uint64_t messages_processed;       /* Total messages processed */
    uint64_t errors;                   /* Total errors encountered */
    double avg_processing_time;        /* Average processing time in ms */
    double throughput;                 /* Throughput in bytes/second */
} ROSIX_StreamStats;

/* ============================================================================
 * Stream Operations
 * ============================================================================ */

/**
 * Open a stream from a resource source
 * 
 * @param source Source resource handle
 * @param stream Stream configuration structure
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_stream_open(ResourceHandle source, ROSIX_Stream* stream);

/**
 * Subscribe to stream events
 * 
 * @param stream Stream to subscribe to
 * @param callback Callback function for stream events
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_stream_subscribe(ROSIX_Stream* stream, ROSIX_Callback callback);

/**
 * Unsubscribe from stream events
 * 
 * @param stream Stream to unsubscribe from
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_stream_unsubscribe(ROSIX_Stream* stream);

/**
 * Close a stream and release resources
 * 
 * @param stream Stream to close
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_stream_close(ROSIX_Stream* stream);

/* ============================================================================
 * Stream Processing
 * ============================================================================ */

/**
 * Start stream processing
 * 
 * @param stream Stream to start
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_stream_start(ROSIX_Stream* stream);

/**
 * Stop stream processing
 * 
 * @param stream Stream to stop
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_stream_stop(ROSIX_Stream* stream);

/**
 * Pause stream processing
 * 
 * @param stream Stream to pause
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_stream_pause(ROSIX_Stream* stream);

/**
 * Resume stream processing
 * 
 * @param stream Stream to resume
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_stream_resume(ROSIX_Stream* stream);

/* ============================================================================
 * Stream Filtering and Transformation
 * ============================================================================ */

/**
 * Add a filter to the stream
 * 
 * @param stream Stream to add filter to
 * @param filter_function Filter function
 * @param filter_context Context for filter function
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_stream_add_filter(ROSIX_Stream* stream,
                                     ROSIX_StreamProcessor filter_function,
                                     void* filter_context);

/**
 * Add a transformation to the stream
 * 
 * @param stream Stream to add transformation to
 * @param transform_function Transformation function
 * @param transform_context Context for transformation function
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_stream_add_transform(ROSIX_Stream* stream,
                                        ROSIX_StreamProcessor transform_function,
                                        void* transform_context);

/**
 * Set stream processing rate limit
 * 
 * @param stream Stream to limit
 * @param max_rate Maximum processing rate (messages per second)
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_stream_set_rate_limit(ROSIX_Stream* stream, int max_rate);

/* ============================================================================
 * Stream Monitoring and Statistics
 * ============================================================================ */

/**
 * Get stream statistics
 * 
 * @param stream Stream to get statistics for
 * @param stats Output parameter for statistics
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_stream_get_stats(ROSIX_Stream* stream, 
                                     ROSIX_StreamStats* stats);

/**
 * Reset stream statistics
 * 
 * @param stream Stream to reset statistics for
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_stream_reset_stats(ROSIX_Stream* stream);

/**
 * Check if stream is active
 * 
 * @param stream Stream to check
 * @return Non-zero if active, zero if not
 */
int rosix_stream_is_active(ROSIX_Stream* stream);

/**
 * Get stream buffer usage
 * 
 * @param stream Stream to check
 * @return Buffer usage percentage (0-100), -1 on error
 */
int rosix_stream_get_buffer_usage(ROSIX_Stream* stream);

/* ============================================================================
 * Stream Aggregation and Batching
 * ============================================================================ */

/**
 * Create a stream aggregator
 * 
 * @param input_streams Array of input streams
 * @param num_streams Number of input streams
 * @param output_stream Output stream for aggregated data
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_stream_aggregate(ROSIX_Stream* input_streams[],
                                    size_t num_streams,
                                    ROSIX_Stream* output_stream);

/**
 * Create a stream splitter
 * 
 * @param input_stream Input stream to split
 * @param output_streams Array of output streams
 * @param num_streams Number of output streams
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_stream_split(ROSIX_Stream* input_stream,
                                ROSIX_Stream* output_streams[],
                                size_t num_streams);

/**
 * Set stream batch size
 * 
 * @param stream Stream to configure
 * @param batch_size Number of messages per batch
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_stream_set_batch_size(ROSIX_Stream* stream, size_t batch_size);

/* ============================================================================
 * Stream Persistence and Recovery
 * ============================================================================ */

/**
 * Enable stream persistence
 * 
 * @param stream Stream to persist
 * @param persistence_path Path for persistence storage
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_stream_enable_persistence(ROSIX_Stream* stream,
                                             const char* persistence_path);

/**
 * Disable stream persistence
 * 
 * @param stream Stream to stop persisting
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_stream_disable_persistence(ROSIX_Stream* stream);

/**
 * Recover stream from persistence
 * 
 * @param persistence_path Path to persistence storage
 * @param stream Output parameter for recovered stream
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_stream_recover(const char* persistence_path,
                                  ROSIX_Stream* stream);

#ifdef __cplusplus
}
#endif

#endif /* _ROSIX_STREAM_H */
