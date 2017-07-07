<?php

/**
 * @file
 * Github deploy menu callbacks.
 */

/**
 * Menu callback for github-deploy/pull.
 */
function github_deploy_callback_pull() {
  // Load library wrapper.
  module_load_include('inc', 'github_deploy', 'github_deploy.git');

  // Hook for deployment process before start.
  module_invoke_all('github_deploy_deployment_start');

  $origin = variable_get('github_deploy_admin_settings_origin_name');
  $result = github_deploy_git_execute_pull_deploy($origin);
  if ($result) {

    if (variable_get('github_deploy_admin_clear_cache')) {
      // Clear cache.
      drupal_flush_all_caches();
    }

    if (variable_get('github_deploy_admin_features_revert')) {
      if (module_exists('features')) {
        // Revert all features.
        features_revert();
      }
    }
    // Hook for deployment process completed.
    module_invoke_all('github_deploy_deployment_completed', $result);
  }
  else {
    // Hook for deployment process fail.
    module_invoke_all('github_deploy_deployment_failed');
  }

  watchdog('Github deploy', '<pre>Output: @Output</pre>', array(
    '@Output' => print_r($result, TRUE),
  ), WATCHDOG_NOTICE);

}
