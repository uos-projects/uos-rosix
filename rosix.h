/*
 * ROSIX - Resource-Oriented System Interface for X
 * 
 * This file defines the core ROSIX interfaces, similar to POSIX
 * for resource-oriented computing in human-machine-thing integrated systems.
 * 
 * Copyright (c) 2024 UOS Project
 * All rights reserved.
 */

#ifndef _ROSIX_H
#define _ROSIX_H

#include <stddef.h>
#include <stdint.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Basic Types and Constants
 * ============================================================================ */

/* Result codes */
typedef int ROSIX_Result;
typedef int ResourceHandle;

/* Standard result codes */
#define ROSIX_SUCCESS           0
#define ROSIX_ERROR            -1
#define ROSIX_INVALID_HANDLE   -2
#define ROSIX_PERMISSION_DENIED -3
#define ROSIX_NOT_FOUND        -4
#define ROSIX_ALREADY_EXISTS   -5
#define ROSIX_TIMEOUT          -6
#define ROSIX_INVALID_PARAM    -7
#define ROSIX_OUT_OF_MEMORY    -8
#define ROSIX_NOT_SUPPORTED    -9

/* Resource access modes */
#define ROSIX_READ_ONLY        "r"
#define ROSIX_WRITE_ONLY       "w"
#define ROSIX_READ_WRITE       "rw"
#define ROSIX_APPEND           "a"
#define ROSIX_CREATE           "c"
#define ROSIX_EXCLUSIVE        "x"

/* Event types */
#define ROSIX_EVENT_UPDATE     "update"
#define ROSIX_EVENT_DELETE     "delete"
#define ROSIX_EVENT_ERROR   "error"
#define ROSIX_EVENT_STATE_CHANGE "state_change"

/* ============================================================================
 * Core Data Structures
 * ============================================================================ */

/**
 * Resource descriptor containing metadata about a resource
 */
typedef struct {
    char* uri;          /* Unique resource identifier */
    char* type;         /* Resource type (sensor, actuator, service, human) */
    char* name;         /* Human-readable name */
    char* metadata;     /* JSON-formatted metadata */
} ResourceDescriptor;

/**
 * Callback function type for resource events
 */
typedef void (*ROSIX_Callback)(ResourceHandle handle, const char* event, void* userdata);

/* ============================================================================
 * Core Resource Operations
 * ============================================================================ */

/**
 * Open or create a resource reference
 * 
 * @param uri Resource URI (e.g., "sensor://temperature/room1")
 * @param mode Access mode (r, w, rw, a, c, x)
 * @return Resource handle on success, -1 on error
 */
ResourceHandle rosix_open(const char* uri, const char* mode);

/**
 * Close a resource handle and release associated resources
 * 
 * @param handle Resource handle to close
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_close(ResourceHandle handle);

/**
 * Read data from a resource
 * 
 * @param handle Resource handle
 * @param buffer Buffer to store read data
 * @param size Maximum number of bytes to read
 * @return Number of bytes read on success, -1 on error
 */
ssize_t rosix_read(ResourceHandle handle, void* buffer, size_t size);

/**
 * Write data to a resource
 * 
 * @param handle Resource handle
 * @param data Data to write
 * @param size Number of bytes to write
 * @return Number of bytes written on success, -1 on error
 */
ssize_t rosix_write(ResourceHandle handle, const void* data, size_t size);

/* ============================================================================
 * Resource Attributes and Control
 * ============================================================================ */

/**
 * Get a resource attribute value
 * 
 * @param handle Resource handle
 * @param key Attribute key
 * @param value Buffer to store attribute value
 * @param len Maximum length of value buffer
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_getattr(ResourceHandle handle, const char* key, 
                           char* value, size_t len);

/**
 * Set a resource attribute value
 * 
 * @param handle Resource handle
 * @param key Attribute key
 * @param value Attribute value to set
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_setattr(ResourceHandle handle, const char* key, 
                           const char* value);

/**
 * Invoke an action on a resource
 * 
 * @param handle Resource handle
 * @param action Action name to invoke
 * @param args JSON-formatted arguments
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_invoke(ResourceHandle handle, const char* action, 
                         const char* args);

/* ============================================================================
 * Event Subscription
 * ============================================================================ */

/**
 * Subscribe to resource events
 * 
 * @param handle Resource handle
 * @param event Event type to subscribe to
 * @param callback Callback function to invoke on event
 * @param userdata User data passed to callback
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_subscribe(ResourceHandle handle, const char* event,
                             ROSIX_Callback callback, void* userdata);

/**
 * Unsubscribe from resource events
 * 
 * @param handle Resource handle
 * @param event Event type to unsubscribe from
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_unsubscribe(ResourceHandle handle, const char* event);

/* ============================================================================
 * Resource Relationships
 * ============================================================================ */

/**
 * Create a hierarchical link between resources
 * 
 * @param parent Parent resource handle
 * @param child Child resource handle
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_link(ResourceHandle parent, ResourceHandle child);

/**
 * Remove a hierarchical link between resources
 * 
 * @param parent Parent resource handle
 * @param child Child resource handle
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_unlink(ResourceHandle parent, ResourceHandle child);

/* ============================================================================
 * Utility Functions
 * ============================================================================ */

/**
 * Get the last error code
 * 
 * @return Last error code
 */
ROSIX_Result rosix_get_last_error(void);

/**
 * Get error message for error code
 * 
 * @param error_code Error code
 * @return Error message string
 */
const char* rosix_strerror(ROSIX_Result error_code);

/**
 * Check if a resource handle is valid
 * 
 * @param handle Resource handle to check
 * @return Non-zero if valid, zero if invalid
 */
int rosix_is_valid_handle(ResourceHandle handle);

#ifdef __cplusplus
}
#endif

#endif /* _ROSIX_H */
