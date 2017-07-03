<?php

/**
 * @file
 * Github deploy menu callbacks.
 */

/**
 * Menu callback for github-deploy/pull.
 */
function github_deploy_callback_pull() {
  watchdog('Github deploy', 'Github hook executed Pull', WATCHDOG_NOTICE);
}
