/*
 * ROSIX Workflow Interface
 * 
 * This file defines the workflow orchestration interfaces for
 * complex multi-resource, multi-stage coordination in ROSIX.
 * 
 * Copyright (c) 2024 UOS Project
 * All rights reserved.
 */

#ifndef _ROSIX_WORKFLOW_H
#define _ROSIX_WORKFLOW_H

#include "rosix.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Workflow Data Structures
 * ============================================================================ */

/**
 * Task execution function type
 */
typedef ROSIX_Result (*ROSIX_TaskExecutor)(void* context);

/**
 * Task structure defining individual workflow tasks
 */
typedef struct {
    char* task_name;                   /* Unique task name */
    char* dependencies[8];             /* Array of dependency task names */
    ROSIX_TaskExecutor execute;        /* Task execution function */
    void* context;                     /* Task execution context */
    int timeout_seconds;               /* Task timeout in seconds */
    int retry_count;                   /* Maximum retry attempts */
    char* description;                 /* Task description */
} ROSIX_Task;

/**
 * Workflow structure containing multiple tasks
 */
typedef struct {
    char* name;                        /* Workflow name */
    ROSIX_Task* tasks;                 /* Array of tasks */
    size_t task_count;                 /* Number of tasks */
    char* description;                 /* Workflow description */
    int enabled;                       /* Workflow enabled flag */
    char* version;                     /* Workflow version */
} ROSIX_Workflow;

/**
 * Workflow execution context
 */
typedef struct {
    char* execution_id;                /* Unique execution identifier */
    char* workflow_name;               /* Workflow name */
    time_t start_time;                 /* Execution start time */
    time_t end_time;                   /* Execution end time */
    int status;                        /* Execution status */
    char* current_task;                /* Currently executing task */
    void* user_data;                   /* User-defined data */
} ROSIX_WorkflowContext;

/**
 * Task execution result
 */
typedef struct {
    char* task_name;                   /* Task name */
    ROSIX_Result result;               /* Execution result */
    char* message;                     /* Result message */
    time_t start_time;                 /* Task start time */
    time_t end_time;                   /* Task end time */
    int retry_count;                   /* Number of retries attempted */
} ROSIX_TaskResult;

/**
 * Workflow execution result
 */
typedef struct {
    char* execution_id;                /* Execution identifier */
    ROSIX_Result overall_result;       /* Overall execution result */
    ROSIX_TaskResult* task_results;    /* Array of task results */
    size_t task_result_count;          /* Number of task results */
    time_t total_duration;             /* Total execution duration */
    char* summary;                     /* Execution summary */
} ROSIX_WorkflowResult;

/* ============================================================================
 * Workflow Management
 * ============================================================================ */

/**
 * Create a new workflow
 * 
 * @param name Workflow name
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_workflow_create(const char* name);

/**
 * Add a task to a workflow
 * 
 * @param workflow_name Workflow name
 * @param task Task to add
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_workflow_add_task(const char* workflow_name, ROSIX_Task task);

/**
 * Remove a task from a workflow
 * 
 * @param workflow_name Workflow name
 * @param task_name Task name to remove
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_workflow_remove_task(const char* workflow_name, 
                                        const char* task_name);

/**
 * Update a task in a workflow
 * 
 * @param workflow_name Workflow name
 * @param task_name Task name to update
 * @param task Updated task definition
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_workflow_update_task(const char* workflow_name,
                                        const char* task_name,
                                        ROSIX_Task task);

/* ============================================================================
 * Workflow Execution
 * ============================================================================ */

/**
 * Start a workflow execution
 * 
 * @param workflow_name Workflow name to start
 * @param context Execution context
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_workflow_start(const char* workflow_name,
                                  ROSIX_WorkflowContext* context);

/**
 * Stop a workflow execution
 * 
 * @param execution_id Execution identifier
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_workflow_stop(const char* execution_id);

/**
 * Pause a workflow execution
 * 
 * @param execution_id Execution identifier
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_workflow_pause(const char* execution_id);

/**
 * Resume a workflow execution
 * 
 * @param execution_id Execution identifier
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_workflow_resume(const char* execution_id);

/* ============================================================================
 * Workflow Monitoring and Status
 * ============================================================================ */

/**
 * Get workflow execution status
 * 
 * @param execution_id Execution identifier
 * @param context Output parameter for execution context
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_workflow_get_status(const char* execution_id,
                                       ROSIX_WorkflowContext* context);

/**
 * Get workflow execution result
 * 
 * @param execution_id Execution identifier
 * @param result Output parameter for execution result
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_workflow_get_result(const char* execution_id,
                                       ROSIX_WorkflowResult* result);

/**
 * List all running workflow executions
 * 
 * @param execution_ids Output array for execution identifiers
 * @param max_executions Maximum number of executions to return
 * @return Number of executions found on success, -1 on error
 */
int rosix_workflow_list_running(char** execution_ids, size_t max_executions);

/**
 * Get workflow execution history
 * 
 * @param workflow_name Workflow name
 * @param start_time Start time for history query
 * @param end_time End time for history query
 * @param results Output array for execution results
 * @param max_results Maximum number of results to return
 * @return Number of results found on success, -1 on error
 */
int rosix_workflow_get_history(const char* workflow_name,
                               time_t start_time,
                               time_t end_time,
                               ROSIX_WorkflowResult* results,
                               size_t max_results);

/* ============================================================================
 * Workflow Dependencies and Scheduling
 * ============================================================================ */

/**
 * Validate workflow dependencies
 * 
 * @param workflow_name Workflow name to validate
 * @return ROSIX_SUCCESS if valid, error code on failure
 */
ROSIX_Result rosix_workflow_validate_dependencies(const char* workflow_name);

/**
 * Set workflow scheduling policy
 * 
 * @param workflow_name Workflow name
 * @param policy Scheduling policy (e.g., "immediate", "scheduled", "conditional")
 * @param schedule_data Policy-specific data
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_workflow_set_schedule(const char* workflow_name,
                                          const char* policy,
                                          const char* schedule_data);

/**
 * Get workflow scheduling information
 * 
 * @param workflow_name Workflow name
 * @param policy Output parameter for scheduling policy
 * @param schedule_data Output parameter for schedule data
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_workflow_get_schedule(const char* workflow_name,
                                          char** policy,
                                          char** schedule_data);

/* ============================================================================
 * Workflow Persistence and Import/Export
 * ============================================================================ */

/**
 * Save workflow to file
 * 
 * @param workflow_name Workflow name to save
 * @param filename Output filename
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_workflow_save(const char* workflow_name, const char* filename);

/**
 * Load workflow from file
 * 
 * @param filename Input filename
 * @param workflow_name Output parameter for workflow name
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_workflow_load(const char* filename, char** workflow_name);

/**
 * Export workflow to JSON
 * 
 * @param workflow_name Workflow name to export
 * @param json_output Output parameter for JSON string
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_workflow_export_json(const char* workflow_name, 
                                         char** json_output);

/**
 * Import workflow from JSON
 * 
 * @param json_input JSON string to import
 * @param workflow_name Output parameter for workflow name
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_workflow_import_json(const char* json_input, 
                                         char** workflow_name);

/* ============================================================================
 * Workflow Templates and Reusability
 * ============================================================================ */

/**
 * Create workflow template
 * 
 * @param template_name Template name
 * @param workflow_name Source workflow name
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_workflow_create_template(const char* template_name,
                                             const char* workflow_name);

/**
 * Instantiate workflow from template
 * 
 * @param template_name Template name
 * @param workflow_name New workflow name
 * @param parameters Template parameters
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_workflow_instantiate_template(const char* template_name,
                                                  const char* workflow_name,
                                                  const char* parameters);

/**
 * List available workflow templates
 * 
 * @param templates Output array for template names
 * @param max_templates Maximum number of templates to return
 * @return Number of templates found on success, -1 on error
 */
int rosix_workflow_list_templates(char** templates, size_t max_templates);

/* ============================================================================
 * Utility Functions
 * ============================================================================ */

/**
 * List all available workflows
 * 
 * @param workflows Output array for workflow names
 * @param max_workflows Maximum number of workflows to return
 * @return Number of workflows found on success, -1 on error
 */
int rosix_workflow_list(char** workflows, size_t max_workflows);

/**
 * Get workflow information
 * 
 * @param workflow_name Workflow name
 * @param workflow Output parameter for workflow information
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_workflow_get_info(const char* workflow_name, 
                                      ROSIX_Workflow* workflow);

/**
 * Delete a workflow
 * 
 * @param workflow_name Workflow name to delete
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_workflow_delete(const char* workflow_name);

/**
 * Free memory allocated for workflow structures
 * 
 * @param workflow Workflow to free
 */
void rosix_workflow_free(ROSIX_Workflow* workflow);

/**
 * Free memory allocated for workflow result structures
 * 
 * @param result Workflow result to free
 */
void rosix_workflow_free_result(ROSIX_WorkflowResult* result);

#ifdef __cplusplus
}
#endif

#endif /* _ROSIX_WORKFLOW_H */

