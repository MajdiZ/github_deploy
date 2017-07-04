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

  $origin = variable_get('github_deploy_admin_settings_origin_name');
  $result = github_deploy_git_execute_pull_deploy($origin);

  watchdog('Github deploy', '<pre>Output: @Output</pre>', array(
    '@Output' => print_r($result, TRUE),
  ), WATCHDOG_NOTICE);
  
}
