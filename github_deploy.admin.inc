<?php

/**
 * @file
 * Github deploy admin interface.
 */

/**
 * Admin settings form.
 *
 * @return mixed
 *    Admin settings form.
 */
function github_deploy_admin_settings() {
  $form = array();

  // Load library wrapper.
  module_load_include('inc', 'github_deploy', 'github_deploy.git');

  // Check if current site has in git repository.
  if (!github_deploy_git_is_repository()) {
    $form['information'] = array(
      '#markup' => t("You don't have any git repository, Please Initializing one."),
    );
    return $form;
  }

  $form['github_deploy'] = array(
    '#type' => 'fieldset',
    '#title' => t('Github deploy settings'),
    '#collapsible' => FALSE,
    '#weight' => 0,
  );

  // Get active branch name.
  $active_branch = github_deploy_git_get_active_branch();
  $form['github_deploy']['repository'] = array(
    '#markup' => t('Your current branch is <strong>@branch</strong>', array(
      '@branch' => $active_branch,
    )),
  );

  $form['github_deploy']['github_deploy_admin_settings_origin_name'] = array(
    '#type' => 'select',
    '#title' => t('Origin name'),
    '#description' => t('Select origin name for repository.'),
    '#options' => array(),
    '#default_value' => variable_get('github_deploy_admin_settings_origin_name'),
    '#required' => TRUE,
  );

  $form['github_deploy']['github_deploy_admin_settings_secure_method'] = array(
    '#type' => 'select',
    '#title' => t('Secure method'),
    '#description' => t('Method to secure Github deploy callback'),
    '#options' => array(
      'secret_token' => t('Secret token'),
      'whitelist_githubs_ip' => t("Whitelist GitHub's IP"),
      'both_methods' => t('Both methods'),
    ),
    '#default_value' => variable_get('github_deploy_admin_settings_secure_method'),
    '#required' => TRUE,
  );

  $form['github_deploy']['github_deploy_admin_settings_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret<span class="form-required"> *</span>'),
    '#description' => t('Secret you want to use'),
    '#default_value' => variable_get('github_deploy_admin_settings_secret', ''),
    '#element_validate' => array(0 => 'github_deploy_states_require_validate'),
    '#states' => array(
      'visible' => array(
        array(
          ':input[name="github_deploy_admin_settings_secure_method"]' => array(
            "value" => 'both_methods',
          ),
        ),
        array(
          ':input[name="github_deploy_admin_settings_secure_method"]' => array(
            "value" => 'secret_token',
          ),
        ),
      ),
    ),
  );

  return system_settings_form($form);
}

/**
 * Custom validation for form items which required and use states API.
 *
 * @param array $element
 *   Form element.
 * @param array $form_state
 *   Form state.
 */
function github_deploy_states_require_validate(array $element, array $form_state) {
  $visible_states = array();

  // Get visible states.
  foreach ($element['#states']['visible'] as $state) {
    if (is_scalar($state)) {
      $visible_states[] = $state;
    }
    else {
      $visible_states[] = array_shift(array_values($state)[0]);
    }
  }

  // Get Depended field name.
  $required_field_key = key($element['#states']['visible']);
  if (!$required_field_key) {
    $required_field_key = key($element['#states']['visible'][0]);
  }

  $required_field = explode('"', $required_field_key);

  // Check form element value if its required.
  if (in_array($form_state['values'][$required_field[1]], $visible_states)) {
    if (empty($form_state['values'][$element['#name']])) {
      form_set_error($element['#name'], $element['#title'] . ' is required.');
    }
  }
}
