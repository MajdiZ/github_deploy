<?php

/**
 * @file
 * Sample hooks demonstrating for github_deploy module.
 */

/**
 * Run after passing callback access and before starting the deployment process.
 */
function hook_github_deploy_deployment_start() {

}

/**
 * Runs after deployment process completed successfully.
 *
 * @param string $result
 *    Result of execution.
 */
function hook_github_deploy_deployment_completed($result) {

}

/**
 * Runs if deployment failed.
 */
function hook_github_deploy_deployment_failed() {

}
