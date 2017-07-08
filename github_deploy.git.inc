<?php

/**
 * @file
 * Wrapper functions for git library.
 */

// Loading library.
libraries_load('gitphp');

/**
 * Initialize repository object.
 *
 * @param string $path
 *    Root of repository, default is DRUPAL_ROOT.
 *
 * @return bool|object
 *    return FALSE on failure or repository object.
 */
function github_deploy_git_init_object($path = DRUPAL_ROOT) {
  $bin_path = variable_get('github_deploy_admin_settings_git_path', GITHUB_DEPLOY_DEFAULT_GIT_PATH);
  Git::set_bin($bin_path);

  try {
    $repository = Git::open($path);
    return $repository;
  }
  catch (Exception $exception) {
    return FALSE;
  }
}

/**
 * Return current active branch name.
 *
 * @param string $path
 *    repository path, default is DRUPAL_ROOT.
 * @param bool $keep_asterisk
 *    Flag for keeping * on active branch name or remove it, default is FALSE.
 *
 * @return bool|string
 *    False or active branch name.
 */
function github_deploy_git_get_active_branch($path = DRUPAL_ROOT, $keep_asterisk = FALSE) {
  if ($repository = github_deploy_git_init_object($path)) {
    return $repository->active_branch($keep_asterisk);
  }
  return FALSE;
}

/**
 * Return list of origin name and urls.
 *
 * @param string $path
 *     repository path, default is DRUPAL_ROOT.
 *
 * @return array|bool
 *    Array of origins => names, or FALSE.
 */
function github_deploy_git_get_origins_name($path = DRUPAL_ROOT) {
  $origins = array();

  if ($repository = github_deploy_git_init_object($path)) {
    $repository = Git::open($path);
    $result = $repository->run('remote');
    // Always empty line at the end.
    $origins_names = explode("\n", $result);
    array_pop($origins_names);

    // Prepare array of $origins => Remotes.
    foreach ($origins_names as $name) {
      $origins[$name] = $repository->run('remote get-url ' . $name);
    }
    return $origins;
  }
  else {
    return FALSE;
  }
}

/**
 * Execute Pull deploy action.
 *
 * @param string $origin
 *    Origin name.
 * @param string $path
 *    repository path, default is DRUPAL_ROOT.
 *
 * @return array|bool
 *    array of command => result or FALSE on error.
 */
function github_deploy_git_execute_pull_deploy($origin, $path = DRUPAL_ROOT) {
  $result = array();
  $commands = array(
    'fetch' => 'fetch --all',
    'reset' => 'reset --hard ' . $origin,
    'pull' => 'pull',
  );

  if ($repository = github_deploy_git_init_object($path)) {
    $repository = Git::open($path);

    foreach ($commands as $command_name => $command) {
      $result[$command_name] = $repository->run($command);
    }
    return $result;
  }
  else {
    return FALSE;
  }
}
