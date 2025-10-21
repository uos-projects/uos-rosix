/*
 * ROSIX Rule Engine Interface
 * 
 * This file defines the rule-based programming interfaces for
 * conditional triggering and logical constraints in ROSIX.
 * 
 * Copyright (c) 2024 UOS Project
 * All rights reserved.
 */

#ifndef _ROSIX_RULE_H
#define _ROSIX_RULE_H

#include "rosix.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Rule Data Structures
 * ============================================================================ */

/**
 * Rule structure defining condition-action pairs
 */
typedef struct {
    char* condition;                   /* Condition expression (e.g., "temperature > 28") */
    char* action;                     /* Action to execute (e.g., "fan = ON") */
    int priority;                     /* Rule priority (higher number = higher priority) */
    char* description;               /* Human-readable rule description */
    int enabled;                      /* Rule enabled flag */
} ROSIX_Rule;

/**
 * Rule set containing multiple rules
 */
typedef struct {
    char* name;                       /* Rule set name */
    ROSIX_Rule* rules;                /* Array of rules */
    size_t rule_count;                /* Number of rules in set */
    char* description;                /* Rule set description */
    int enabled;                      /* Rule set enabled flag */
} ROSIX_RuleSet;

/**
 * Rule execution context
 */
typedef struct {
    ResourceHandle source;             /* Source resource that triggered rule */
    char* event_type;                  /* Type of event that triggered rule */
    void* event_data;                 /* Event data */
    time_t trigger_time;              /* Time when rule was triggered */
    char* execution_id;               /* Unique execution identifier */
} ROSIX_RuleContext;

/**
 * Rule execution result
 */
typedef struct {
    int success;                      /* Execution success flag */
    char* message;                    /* Execution result message */
    char* action_taken;               /* Action that was taken */
    time_t execution_time;           /* Time when action was executed */
    ROSIX_Result result_code;         /* Result code */
} ROSIX_RuleResult;

/* ============================================================================
 * Rule Management
 * ============================================================================ */

/**
 * Define a new rule set
 * 
 * @param name Rule set name
 * @param rules Array of rules
 * @param count Number of rules
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_rule_define(const char* name, ROSIX_Rule* rules, size_t count);

/**
 * Enable a rule set
 * 
 * @param name Rule set name to enable
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_rule_enable(const char* name);

/**
 * Disable a rule set
 * 
 * @param name Rule set name to disable
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_rule_disable(const char* name);

/**
 * Delete a rule set
 * 
 * @param name Rule set name to delete
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_rule_delete(const char* name);

/* ============================================================================
 * Rule Execution
 * ============================================================================ */

/**
 * Execute rules for a given context
 * 
 * @param context Rule execution context
 * @param result Output parameter for execution result
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_rule_execute(const ROSIX_RuleContext* context,
                                ROSIX_RuleResult* result);

/**
 * Execute a specific rule
 * 
 * @param rule_set_name Rule set name
 * @param rule_index Index of rule to execute
 * @param context Rule execution context
 * @param result Output parameter for execution result
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_rule_execute_specific(const char* rule_set_name,
                                         int rule_index,
                                         const ROSIX_RuleContext* context,
                                         ROSIX_RuleResult* result);

/* ============================================================================
 * Rule Monitoring and Statistics
 * ============================================================================ */

/**
 * Get rule execution statistics
 * 
 * @param rule_set_name Rule set name
 * @param total_executions Output parameter for total executions
 * @param successful_executions Output parameter for successful executions
 * @param failed_executions Output parameter for failed executions
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_rule_get_stats(const char* rule_set_name,
                                  uint64_t* total_executions,
                                  uint64_t* successful_executions,
                                  uint64_t* failed_executions);

/**
 * Get rule execution history
 * 
 * @param rule_set_name Rule set name
 * @param start_time Start time for history query
 * @param end_time End time for history query
 * @param results Array to store execution results
 * @param max_results Maximum number of results to return
 * @return Number of results found on success, -1 on error
 */
int rosix_rule_get_history(const char* rule_set_name,
                           time_t start_time,
                           time_t end_time,
                           ROSIX_RuleResult* results,
                           size_t max_results);

/* ============================================================================
 * Rule Validation and Testing
 * ============================================================================ */

/**
 * Validate a rule condition
 * 
 * @param condition Condition expression to validate
 * @return ROSIX_SUCCESS if valid, error code on failure
 */
ROSIX_Result rosix_rule_validate_condition(const char* condition);

/**
 * Validate a rule action
 * 
 * @param action Action expression to validate
 * @return ROSIX_SUCCESS if valid, error code on failure
 */
ROSIX_Result rosix_rule_validate_action(const char* action);

/**
 * Test a rule with sample data
 * 
 * @param rule Rule to test
 * @param test_data Sample data for testing
 * @param result Output parameter for test result
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_rule_test(ROSIX_Rule* rule,
                             const char* test_data,
                             ROSIX_RuleResult* result);

/* ============================================================================
 * Rule Persistence and Import/Export
 * ============================================================================ */

/**
 * Save rule set to file
 * 
 * @param rule_set_name Rule set name to save
 * @param filename Output filename
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_rule_save(const char* rule_set_name, const char* filename);

/**
 * Load rule set from file
 * 
 * @param filename Input filename
 * @param rule_set_name Output parameter for rule set name
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_rule_load(const char* filename, char** rule_set_name);

/**
 * Export rule set to JSON
 * 
 * @param rule_set_name Rule set name to export
 * @param json_output Output parameter for JSON string
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_rule_export_json(const char* rule_set_name, char** json_output);

/**
 * Import rule set from JSON
 * 
 * @param json_input JSON string to import
 * @param rule_set_name Output parameter for rule set name
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_rule_import_json(const char* json_input, char** rule_set_name);

/* ============================================================================
 * Rule Dependencies and Conflicts
 * ============================================================================ */

/**
 * Check for rule conflicts
 * 
 * @param rule_set_name Rule set name to check
 * @param conflicts Output parameter for conflict descriptions
 * @param max_conflicts Maximum number of conflicts to return
 * @return Number of conflicts found on success, -1 on error
 */
int rosix_rule_check_conflicts(const char* rule_set_name,
                               char** conflicts,
                               size_t max_conflicts);

/**
 * Set rule dependencies
 * 
 * @param rule_set_name Rule set name
 * @param dependencies Array of dependency rule set names
 * @param num_dependencies Number of dependencies
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_rule_set_dependencies(const char* rule_set_name,
                                         const char** dependencies,
                                         size_t num_dependencies);

/**
 * Get rule dependencies
 * 
 * @param rule_set_name Rule set name
 * @param dependencies Output array for dependency names
 * @param max_dependencies Maximum number of dependencies to return
 * @return Number of dependencies found on success, -1 on error
 */
int rosix_rule_get_dependencies(const char* rule_set_name,
                                char** dependencies,
                                size_t max_dependencies);

/* ============================================================================
 * Utility Functions
 * ============================================================================ */

/**
 * List all available rule sets
 * 
 * @param rule_sets Output array for rule set names
 * @param max_rule_sets Maximum number of rule sets to return
 * @return Number of rule sets found on success, -1 on error
 */
int rosix_rule_list(char** rule_sets, size_t max_rule_sets);

/**
 * Get rule set information
 * 
 * @param rule_set_name Rule set name
 * @param rule_set Output parameter for rule set information
 * @return ROSIX_SUCCESS on success, error code on failure
 */
ROSIX_Result rosix_rule_get_info(const char* rule_set_name, ROSIX_RuleSet* rule_set);

/**
 * Free memory allocated for rule structures
 * 
 * @param rule_set Rule set to free
 */
void rosix_rule_free(ROSIX_RuleSet* rule_set);

#ifdef __cplusplus
}
#endif

#endif /* _ROSIX_RULE_H */

