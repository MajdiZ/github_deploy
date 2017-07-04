<?php

/**
 * @file
 * Wrapper functions for git library.
 */

// Loading library.
libraries_load('gitphp');

/**
 * Check if provided path is git repository.
 *
 * @param string $path
 *    The path to check, default is DRUPAL_ROOT.
 *
 * @return bool
 *    TRUE if path is git repository otherwise FALSE.
 */
function github_deploy_git_is_repository($path = DRUPAL_ROOT) {

  try {
    Git::open($path);
    $is_repository = TRUE;
  }
  catch (Exception $exception) {
    $is_repository = FALSE;
  }

  return $is_repository;
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

  try {
    $repository = Git::open($path);
    return $repository->active_branch($keep_asterisk);
  }
  catch (Exception $exception) {
    return FALSE;
  }

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
  try {
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
  catch (Exception $exception) {
    return FALSE;
  }
}
