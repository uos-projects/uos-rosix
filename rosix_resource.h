/*
 * ROSIX Resource Space Interface
 * 
 * This file defines the ResourceSpace interfaces for spatial, temporal,
 * and semantic context management in ROSIX.
 * 
 * Copyright (c) 2024 UOS Project
 * All rights reserved.
 */

#ifndef _ROSIX_RESOURCE_H
#define _ROSIX_RESOURCE_H

#include "rosix.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Spatial Context
 * ============================================================================ */

/**
 * Spatial context representing 3D position and orientation
 */
typedef struct {
    double x, y, z;                    /* Position coordinates */
    double orientation[3];             /* Orientation vector (roll, pitch, yaw) */
    double accuracy;                   /* Position accuracy in meters */
    char* coordinate_system;           /* Coordinate system identifier */
} SpatialContext;

/* ============================================================================
 * Temporal Context
 * ============================================================================ */

/**
 * Temporal context representing time-based state information
 */
typedef struct {
    time_t timestamp;                  /* Unix timestamp */
    char* state;                       /* Current state description */
    char* trend;                       /* Trend: "increasing", "stable", "decreasing" */
    double confidence;                 /* Confidence level (0.0 to 1.0) */
    char* prediction;                  /* Future state prediction */
} TemporalContext;

/* ============================================================================
 * Semantic Profile
 * ============================================================================ */

/**
 * Semantic profile describing resource capabilities and ontology
 */
typedef struct {
    char* type;                        /* Resource type identifier */
    char* capabilities;                /* JSON array of capabilities */
    char* ontology_uri;                /* Ontology URI for semantic description */
    char* version;                     /* Resource version */
    char* manufacturer;                /* Manufacturer information */
    char* model;                       /* Model information */
} SemanticProfile;

/* ============================================================================
 * Resource Reference
 * ============================================================================ */

/**
 * Complete resource reference with all context information
 */
typedef struct {
    ResourceHandle handle;             /* Resource handle */
    SpatialContext space;              /* Spatial context */
    TemporalContext time;              /* Temporal context */
    SemanticProfile semantic;          /* Semantic profile */
    char* uri;                         /* Resource URI */
    char* status;                      /* Current status */
} ResourceRef;

/* ============================================================================
 * Resource Resolution and Context Management
 * ============================================================================ */

/**
 * Resolve a resource URI to a complete resource reference
 * 
 * @param uri Resource URI to resolve
 * @return ResourceRef structure with all context information
 */
ResourceRef rosix_resolve(const char* uri);

/**
 * Update spatial context for a resource
 * 
 * @param handle Resource handle
 * @param ctx New spatial context
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_update_spatial(ResourceHandle handle, 
                                  const SpatialContext* ctx);

/**
 * Update temporal context for a resource
 * 
 * @param handle Resource handle
 * @param ctx New temporal context
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_update_temporal(ResourceHandle handle, 
                                   const TemporalContext* ctx);

/**
 * Update semantic profile for a resource
 * 
 * @param handle Resource handle
 * @param profile New semantic profile
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_update_semantic(ResourceHandle handle, 
                                   const SemanticProfile* profile);

/* ============================================================================
 * Topology and Relationship Queries
 * ============================================================================ */

/**
 * Query topological neighbors of a resource
 * 
 * @param handle Resource handle
 * @param neighbors Array to store neighbor resource references
 * @param max Maximum number of neighbors to return
 * @return Number of neighbors found on success, -1 on error
 */
int rosix_query_topology(ResourceHandle handle, ResourceRef* neighbors, 
                         size_t max);

/**
 * Query resources within a spatial radius
 * 
 * @param center_x Center X coordinate
 * @param center_y Center Y coordinate
 * @param center_z Center Z coordinate
 * @param radius Search radius
 * @param results Array to store matching resource references
 * @param max Maximum number of results to return
 * @return Number of resources found on success, -1 on error
 */
int rosix_query_spatial_range(double center_x, double center_y, double center_z,
                              double radius, ResourceRef* results, size_t max);

/**
 * Query resources by type
 * 
 * @param type Resource type to search for
 * @param results Array to store matching resource references
 * @param max Maximum number of results to return
 * @return Number of resources found on success, -1 on error
 */
int rosix_query_by_type(const char* type, ResourceRef* results, size_t max);

/**
 * Query resources by capability
 * 
 * @param capability Capability to search for
 * @param results Array to store matching resource references
 * @param max Maximum number of results to return
 * @return Number of resources found on success, -1 on error
 */
int rosix_query_by_capability(const char* capability, ResourceRef* results, 
                              size_t max);

/* ============================================================================
 * Context History and Versioning
 * ============================================================================ */

/**
 * Get historical spatial context for a resource
 * 
 * @param handle Resource handle
 * @param start_time Start time for history query
 * @param end_time End time for history query
 * @param contexts Array to store historical contexts
 * @param max Maximum number of contexts to return
 * @return Number of contexts found on success, -1 on error
 */
int rosix_get_spatial_history(ResourceHandle handle, time_t start_time, 
                             time_t end_time, SpatialContext* contexts, 
                             size_t max);

/**
 * Get historical temporal context for a resource
 * 
 * @param handle Resource handle
 * @param start_time Start time for history query
 * @param end_time End time for history query
 * @param contexts Array to store historical contexts
 * @param max Maximum number of contexts to return
 * @return Number of contexts found on success, -1 on error
 */
int rosix_get_temporal_history(ResourceHandle handle, time_t start_time, 
                              time_t end_time, TemporalContext* contexts, 
                              size_t max);

/**
 * Create a snapshot of current resource state
 * 
 * @param handle Resource handle
 * @param snapshot_id Output parameter for snapshot identifier
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_create_snapshot(ResourceHandle handle, char** snapshot_id);

/**
 * Restore resource state from a snapshot
 * 
 * @param handle Resource handle
 * @param snapshot_id Snapshot identifier to restore
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_restore_snapshot(ResourceHandle handle, 
                                    const char* snapshot_id);

/* ============================================================================
 * Utility Functions
 * ============================================================================ */

/**
 * Calculate spatial distance between two resources
 * 
 * @param handle1 First resource handle
 * @param handle2 Second resource handle
 * @return Distance in meters, -1.0 on error
 */
double rosix_calculate_distance(ResourceHandle handle1, ResourceHandle handle2);

/**
 * Check if two resources are spatially adjacent
 * 
 * @param handle1 First resource handle
 * @param handle2 Second resource handle
 * @param threshold Adjacency threshold in meters
 * @return Non-zero if adjacent, zero if not
 */
int rosix_is_adjacent(ResourceHandle handle1, ResourceHandle handle2, 
                      double threshold);

/**
 * Free memory allocated for ResourceRef structures
 * 
 * @param ref Resource reference to free
 */
void rosix_free_resource_ref(ResourceRef* ref);

#ifdef __cplusplus
}
#endif

#endif /* _ROSIX_RESOURCE_H */
