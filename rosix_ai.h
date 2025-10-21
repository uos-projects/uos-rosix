/*
 * ROSIX AI Interface
 * 
 * This file defines the AI agent interfaces for intent-driven and
 * semantic understanding capabilities in ROSIX.
 * 
 * Copyright (c) 2024 UOS Project
 * All rights reserved.
 */

#ifndef _ROSIX_AI_H
#define _ROSIX_AI_H

#include "rosix.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * AI Agent Data Structures
 * ============================================================================ */

/**
 * AI Agent structure for intelligent resource interaction
 */
typedef struct {
    char* model_uri;                   /* AI model URI or identifier */
    char* prompt;                      /* System prompt or context */
    char* output;                      /* Agent output format */
    char* capabilities;                /* JSON array of agent capabilities */
    char* version;                     /* Agent version */
    int max_tokens;                    /* Maximum tokens for responses */
    double temperature;                /* Response creativity (0.0-1.0) */
} ROSIX_Agent;

/**
 * AI Agent execution context
 */
typedef struct {
    char* session_id;                  /* Session identifier */
    char* user_id;                     /* User identifier */
    char* intent;                      /* User intent or query */
    char* context;                     /* Additional context information */
    time_t timestamp;                  /* Request timestamp */
    void* user_data;                   /* User-defined data */
} ROSIX_AgentContext;

/**
 * AI Agent execution result
 */
typedef struct {
    char* response;                    /* Agent response */
    char* action_taken;                /* Action that was taken */
    ROSIX_Result result_code;          /* Result code */
    double confidence;                 /* Confidence level (0.0-1.0) */
    char* reasoning;                   /* Reasoning explanation */
    time_t processing_time;            /* Processing time in milliseconds */
} ROSIX_AgentResult;

/**
 * AI Agent plan structure
 */
typedef struct {
    char* plan_id;                     /* Unique plan identifier */
    char* description;                 /* Plan description */
    char* steps;                       /* JSON array of plan steps */
    int priority;                      /* Plan priority */
    char* status;                      /* Plan status */
    time_t created_time;              /* Plan creation time */
    time_t estimated_duration;         /* Estimated execution duration */
} ROSIX_AgentPlan;

/**
 * AI Agent plan step
 */
typedef struct {
    char* step_id;                     /* Step identifier */
    char* action;                      /* Action to perform */
    char* target_resource;             /* Target resource URI */
    char* parameters;                  /* Step parameters */
    int order;                         /* Execution order */
    char* status;                      /* Step status */
} ROSIX_AgentPlanStep;

/* ============================================================================
 * AI Agent Management
 * ============================================================================ */

/**
 * Create a new AI agent
 * 
 * @param agent Agent configuration
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_agent_create(ROSIX_Agent* agent);

/**
 * Delete an AI agent
 * 
 * @param agent_id Agent identifier
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_agent_delete(const char* agent_id);

/**
 * Update an AI agent configuration
 * 
 * @param agent_id Agent identifier
 * @param agent Updated agent configuration
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_agent_update(const char* agent_id, ROSIX_Agent* agent);

/**
 * Get AI agent information
 * 
 * @param agent_id Agent identifier
 * @param agent Output parameter for agent information
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_agent_get_info(const char* agent_id, ROSIX_Agent* agent);

/* ============================================================================
 * AI Agent Execution
 * ============================================================================ */

/**
 * Invoke an AI agent with intent
 * 
 * @param agent_id Agent identifier
 * @param context Agent execution context
 * @param result Output parameter for execution result
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_agent_invoke(const char* agent_id,
                                const ROSIX_AgentContext* context,
                                ROSIX_AgentResult* result);

/**
 * Bind an AI agent to a resource
 * 
 * @param agent_id Agent identifier
 * @param resource_handle Resource handle to bind to
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_agent_bind(const char* agent_id, ResourceHandle resource_handle);

/**
 * Unbind an AI agent from a resource
 * 
 * @param agent_id Agent identifier
 * @param resource_handle Resource handle to unbind from
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_agent_unbind(const char* agent_id, ResourceHandle resource_handle);

/**
 * Get bound resources for an agent
 * 
 * @param agent_id Agent identifier
 * @param resources Output array for resource handles
 * @param max_resources Maximum number of resources to return
 * @return Number of resources found on success, -1 on error
 */
int rosix_agent_get_bound_resources(const char* agent_id,
                                    ResourceHandle* resources,
                                    size_t max_resources);

/* ============================================================================
 * AI Agent Planning and Reasoning
 * ============================================================================ */

/**
 * Create an AI agent plan
 * 
 * @param agent_id Agent identifier
 * @param intent User intent for planning
 * @param plan Output parameter for created plan
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_agent_create_plan(const char* agent_id,
                                     const char* intent,
                                     ROSIX_AgentPlan* plan);

/**
 * Execute an AI agent plan
 * 
 * @param plan_id Plan identifier
 * @param result Output parameter for execution result
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_agent_execute_plan(const char* plan_id,
                                      ROSIX_AgentResult* result);

/**
 * Get plan execution status
 * 
 * @param plan_id Plan identifier
 * @param status Output parameter for plan status
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_agent_get_plan_status(const char* plan_id, char** status);

/**
 * Cancel an AI agent plan
 * 
 * @param plan_id Plan identifier
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_agent_cancel_plan(const char* plan_id);

/* ============================================================================
 * AI Agent Learning and Adaptation
 * ============================================================================ */

/**
 * Train an AI agent with data
 * 
 * @param agent_id Agent identifier
 * @param training_data Training data
 * @param data_size Size of training data
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_agent_train(const char* agent_id,
                               const void* training_data,
                               size_t data_size);

/**
 * Update agent model
 * 
 * @param agent_id Agent identifier
 * @param model_uri New model URI
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_agent_update_model(const char* agent_id, const char* model_uri);

/**
 * Get agent performance metrics
 * 
 * @param agent_id Agent identifier
 * @param accuracy Output parameter for accuracy
 * @param response_time Output parameter for average response time
 * @param success_rate Output parameter for success rate
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_agent_get_metrics(const char* agent_id,
                                     double* accuracy,
                                     double* response_time,
                                     double* success_rate);

/* ============================================================================
 * AI Agent Communication and Collaboration
 * ============================================================================ */

/**
 * Enable agent-to-agent communication
 * 
 * @param agent_id Agent identifier
 * @param communication_enabled Enable/disable communication
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_agent_enable_communication(const char* agent_id,
                                              int communication_enabled);

/**
 * Send message to another agent
 * 
 * @param from_agent_id Sender agent identifier
 * @param to_agent_id Receiver agent identifier
 * @param message Message to send
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_agent_send_message(const char* from_agent_id,
                                      const char* to_agent_id,
                                      const char* message);

/**
 * Get agent messages
 * 
 * @param agent_id Agent identifier
 * @param messages Output array for messages
 * @param max_messages Maximum number of messages to return
 * @return Number of messages found on success, -1 on error
 */
int rosix_agent_get_messages(const char* agent_id,
                             char** messages,
                             size_t max_messages);

/* ============================================================================
 * AI Agent Monitoring and Debugging
 * ============================================================================ */

/**
 * Enable agent logging
 * 
 * @param agent_id Agent identifier
 * @param log_level Log level (debug, info, warn, error)
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_agent_enable_logging(const char* agent_id, const char* log_level);

/**
 * Get agent logs
 * 
 * @param agent_id Agent identifier
 * @param start_time Start time for log query
 * @param end_time End time for log query
 * @param logs Output array for log entries
 * @param max_logs Maximum number of logs to return
 * @return Number of logs found on success, -1 on error
 */
int rosix_agent_get_logs(const char* agent_id,
                         time_t start_time,
                         time_t end_time,
                         char** logs,
                         size_t max_logs);

/**
 * Set agent debugging mode
 * 
 * @param agent_id Agent identifier
 * @param debug_enabled Enable/disable debugging
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_agent_set_debug(const char* agent_id, int debug_enabled);

/* ============================================================================
 * Utility Functions
 * ============================================================================ */

/**
 * List all available AI agents
 * 
 * @param agents Output array for agent identifiers
 * @param max_agents Maximum number of agents to return
 * @return Number of agents found on success, -1 on error
 */
int rosix_agent_list(char** agents, size_t max_agents);

/**
 * Get agent capabilities
 * 
 * @param agent_id Agent identifier
 * @param capabilities Output parameter for capabilities JSON
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_agent_get_capabilities(const char* agent_id, char** capabilities);

/**
 * Free memory allocated for agent structures
 * 
 * @param agent Agent to free
 */
void rosix_agent_free(ROSIX_Agent* agent);

/**
 * Free memory allocated for agent result structures
 * 
 * @param result Agent result to free
 */
void rosix_agent_free_result(ROSIX_AgentResult* result);

/**
 * Free memory allocated for agent plan structures
 * 
 * @param plan Agent plan to free
 */
void rosix_agent_free_plan(ROSIX_AgentPlan* plan);

#ifdef __cplusplus
}
#endif

#endif /* _ROSIX_AI_H */

