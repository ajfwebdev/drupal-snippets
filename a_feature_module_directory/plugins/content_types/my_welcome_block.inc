<?php

/**
 * @file
 * Content pane for Welcome Block for Admitted Students Home.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Welcome Block'),
  'description' => t('Admitted Welcome Block only for users with Admitted Student Role.'),
  'category' => t('Harvard Admitted Students Home'),
  'admin info' => 'huae_welcome_block_info',
  'edit form' => 'huae_welcome_block_edit_form',
  'render callback' => 'huae_welcome_block_render',
  'defaults' => array(
    'message' => 'Welcome to Harvard’s incredibly diverse community. Meet your future classmates and explore all we offer in and out of the classroom. We hope you will join us.',
  ),
);

/**
 * Displays admin info for content block.
 *
 * @param string $subtype
 *   Unused.
 * @param array $conf
 *   Configuration.
 * @param mixed $contexts
 *   Unused
 *
 * @return mixed
 *   Block of info.
 */
function huae_welcome_block_info($subtype, $conf, $contexts) {
  if (!empty($conf)) {
    $block = new stdClass();
    $block->title = 'Welcome Message';
    $block->content = t('%t', array(
      '%t' => $conf['message'],
    ));
    return $block;
  }
}

/**
 * Admin edit for block.
 *
 * @param array $form
 *   Form data.
 * @param $form_state
 *   Form State data.
 *
 * @return mixed
 *   Form.
 */
function huae_welcome_block_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $form['message'] = array(
    '#title' => t('Welcome Message'),
    '#type' => 'textarea',
    '#default_value' => $conf['message'],
    '#required' => TRUE,
  );
  return $form;
}

/**
 * Submit action for edit form.
 *
 * @param array $form
 *   Form data.
 * @param $form_state
 *   Form State data.
 */
function huae_welcome_block_edit_form_submit($form, &$form_state) {
  $form_state['conf']['message'] = $form_state['values']['message'];
}

/**
 * Render function for block.
 *
 * @param string $subtype
 *   Unused.
 * @param array $conf
 *   Configuration.
 * @param array $panel_args
 *   Unused.
 * @param null $context
 *   Unused.
 *
 * @return mixed
 *   Rendered block output.
 */
function huae_welcome_block_render($subtype, $conf, $panel_args, $context = NULL) {
  global $user;
  $block = new stdClass();
  $block->title = '';
  $block->content = '';
  if (in_array('admitted student', $user->roles)) {
    $amw = entity_metadata_wrapper('user', $user);
    $name = $user->name;
    if (isset($amw->field_first_name)) {
      $name = $amw->field_first_name->value();
    }
    $out = array();
    $out[] = '<div class="message-banner home">';
    $out[] = '<h2 class="message-banner__message-title">';
    $out[] = t('Congratulations on your admission, <span>!u</span>!', array(
      '!u' => $name,
    ));
    $out[] = '</h2>';
    $out[] = '<p class="message-banner__content">';
    $out[] = $conf['message'];
    $out[] =  '</p>';
    $out[] = '</div>';
    $block->content = implode('', $out);
  }
  return $block;

}
