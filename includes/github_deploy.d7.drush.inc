<?php

/**
 * @file
 * Github deploy Drush integration.
 */

/**
 * Implements hook_drush_help().
 */
function github_deploy_drush_help($command) {
  switch ($command) {
    case 'drush:deploy-it':
      return dt('Execute github deploy command.');
  }
}

/**
 * Implements hook_drush_command().
 */
function github_deploy_drush_command() {

  $items['deploy-it'] = array(
    'description' => 'Execute deployment.',
    'aliases' => array('deployit'),
    'arguments' => array(),
    'options' => array(),
    'examples' => array(
      'drush deployit' => 'Deploy WITHOUT clear-cache or revert-features.',
    ),
  );

  return $items;
}

/**
 * Implements drush_hook_COMMAND().
 */
function drush_github_deploy_deploy_it() {
  module_load_include('inc', 'github_deploy', 'includes/github_deploy.pages');
  github_deploy_callback_pull();
}
