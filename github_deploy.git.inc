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
    $result = $repository->run('remote --verbose');
    // Always empty line at the end.
    $origins_remote = explode("\n", $result);
    array_pop($origins_remote);

    // Prepare array of $origins => Remotes.
    foreach ($origins_remote as $origin) {
      $origin = preg_split('/\s+/', $origin);

      if ($origin[2] == '(fetch)') {
        $origins[$origin[0]] = $origin[0] . ' - ' . $origin[1];
      }

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

/**
 * Check setup status.
 *
 * @param string $path
 *    repository path, default is DRUPAL_ROOT.
 *
 * @return bool
 *    TRUE if check completed otherwise FALSE.
 */
function github_deploy_git_check_setup($path = DRUPAL_ROOT) {
  if ($repository = github_deploy_git_init_object($path)) {
    // Repository exist.
    if ($repository->test_git()) {
      // Git installed.
      if (github_deploy_git_get_active_branch($path)) {
        // Active branch found.
        if (github_deploy_git_get_origins_name($path)) {
          // Origin found.
          // Let's clean up the messages.
          drupal_get_messages('warning');
          return TRUE;
        }
        else {
          drupal_set_message(t("You need to have at least 1 remote origin."), 'warning');
        }
      }
      else {
        drupal_set_message(t("You need to have at least 1 branch."), 'warning');
      }
    }
    else {
      drupal_set_message(t("Git is not installed or not exported to path."), 'warning');
    }
  }
  else {
    drupal_set_message(t("Can't find repository in project root, initialize one."), 'warning');
  }
  return FALSE;
}
