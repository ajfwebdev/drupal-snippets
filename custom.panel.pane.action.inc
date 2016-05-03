<?php
/**
 * @file
 * A custom content pane to use for calls to action.
 */

/**
 * Plugin array - used to describe the plugin.
 *
 * This is a Ctools content type plugin (a panels pane).
 *
 * 'title': This is the title of the plugin. It'll be used in the Panels
 *   administration area.
 * 'description': This is used as the description of the plugin, it's only
 *   used in the Panels administration area.
 * 'single': Content types have a concept called subtype.
 *   For basic content types leave this as TRUE.
 * 'content_types': This is the machine name of the plugin.
 * 'render callback': This is a callback to a function which will be used to
 *   render the content type.
 * 'required context': This tells Ctools which context is required for this
 *   content type. By using ctools_context_required(t('Node'), 'node'), this
 *   content type will only be available for node entities.
 * 'edit form': This is a callback for the edit form. Please note, this edit
 *   form must be implemented if you define a 'required context' key.
 * 'category': This allows you to add a content type into a Panels category.
 */
$plugin = array(
  'single' => TRUE,
  'content_types' => array('my_module_action_pane'),
  'title' => t('Call to Action Custom Pane'),
  'description' => t('Use to create custom call to action blocks.'),
  'category' => t('Form'),
  'edit form' => 'my_module_action_pane_edit_form',
  'render callback' => 'my_module_action_pane_render',
  'admin info' => 'my_module_action_pane_admin_info',
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'defaults' => array(
    'text' => '',
  ),
  'all contexts' => TRUE,
);


/**
 * Ctools edit form.
 */
function my_module_action_pane_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['action_pane_body'] = array(
    '#type' => 'text_format',
    '#base_type' => 'textarea',
    '#title' => t('Body'),
    '#description' => t('The body text for this call to action.'),
    '#default_value' => isset($conf['action_pane_body']['value']) ? $conf['action_pane_body']['value'] : '',
    '#format' => 'full_html',
    '#process' => array('filter_process_format', 'ctools_dependent_process'),
  );
  $form['action_pane_body_class'] = array(
    '#type' => 'textfield',
    '#title' => t('Customize the CSS class for the Body text'),
    '#description' => t('Enter the CSS class to be used to style the Body text, or leave the default class.'),
    '#default_value' => isset($conf['action_pane_body_class']) ? $conf['action_pane_body_class'] : 'action-pane-body',
  );
  $form['action_pane_link_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Link Text'),
    '#description' => t('Enter the text that should clickable as a link.'),
    '#default_value' => isset($conf['action_pane_link_text']) ? $conf['action_pane_link_text'] : '',
  );
  $form['action_pane_link_text_class'] = array(
    '#type' => 'textfield',
    '#title' => t('Customize the CSS class for the Link text'),
    '#description' => t('Enter the CSS class to be used to style the Link text, or leave the default class.'),
    '#default_value' => isset($conf['action_pane_link_text_class']) ? $conf['action_pane_link_text_class'] : 'action-pane-link-text',
  );
  $form['action_pane_url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL'),
    '#description' => t('Enter the URL for the link. For external links use the full URL (including http:// or https://). For internal links a Drupal site-relative URL can be entered.'),
    '#default_value' => isset($conf['action_pane_url']) ? $conf['action_pane_url'] : '',
  );
  $form['action_pane_url_target'] = array(
    '#type' => 'select',
    '#title' => t('URL is Internal or External?'),
    '#description' => t('Select External if you want the linked page to open in a new browser window. Select Internal to stay in the same browser window.'),
    '#options' => array('External' => t('External'), 'Internal' => t('Internal')),
    '#default_value' => isset($conf['action_pane_url_target']) ? $conf['action_pane_url_target'] : 'External',
  );
  $form['action_pane_anchor_class'] = array(
    '#type' => 'textfield',
    '#title' => t('Customize the CSS class for the Link.'),
    '#description' => t('Enter the CSS class to be used to style the Link (the anchor tag), or leave the default class.'),
    '#default_value' => isset($conf['action_pane_anchor_class']) ? $conf['action_pane_anchor_class'] : 'action-pane-link',
  );
  $form['action_pane_wrapper_class'] = array(
    '#type' => 'textfield',
    '#title' => t('Customize the CSS class for the wrapper.'),
    '#description' => t('Enter the CSS class to be used to style the wrapper of this Call to Action block content, or leave the default class.'),
    '#default_value' => isset($conf['action_pane_wrapper_class']) ? $conf['action_pane_wrapper_class'] : 'action-pane-wrapper',
  );
  return $form;
}

/**
 * Ctools edit form submit handler.
 */
function my_module_action_pane_edit_form_submit($form, &$form_state) {
  $form_fields = array(
    'action_pane_body',
    'action_pane_body_class',
    'action_pane_link_text',
    'action_pane_link_text_class',
    'action_pane_url',
    'action_pane_url_target',
    'action_pane_anchor_class',
    'action_pane_wrapper_class',
  );
  foreach ($form_fields as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Render callback function.
 */
function my_module_action_pane_render($subtype, $conf, $args, $context) {
  $block = new stdClass();
  $block->content = my_module_action_build_markup($conf);

  return $block;
}

/**
 * Builds markup for the content of our block.
 *
 * @param array $conf
 *   An associative array containing the action pane values.
 *
 * @return string $output
 *   The markup to use for the Block content.
 */
function my_module_action_build_markup($conf) {
  $output = '';
  $target = '';
  if ($conf['action_pane_url_target'] == 'External') {
    $target = '_blank';
  }

  // Build markup for the link.
  $link_text = '<span class="' . $conf['action_pane_link_text_class'] . '">';
  $link_text .= t('@submit_text', array('@submit_text' => $conf['action_pane_link_text']));
  $link_text .= '</span>';
  $options = array(
    'attributes' => array(
      'class' => array($conf['action_pane_anchor_class']),
      'target' => $target,
    ),
    'html' => TRUE,
  );

  // Build total markup and return.
  $output .= '<span class="' . $conf['action_pane_wrapper_class'] . '">';
  $output .= '<span class="' . $conf['action_pane_body_class'] . '">';
  $output .= $conf['action_pane_body']['value'];
  $output .= '</span>';
  $output .= l($link_text, $conf['action_pane_url'], $options);
  $output .= '</span>';
  return $output;
}

/**
 * Admin info callback for panel pane.
 */
function my_module_action_pane_admin_info($subtype, $conf, $context) {
  // Set the title that shows on the admin Content page to the overridden title.
  $block = new stdClass();
  $block->title = $conf['override_title_text'] ? $conf['override_title_text'] : '';
  return $block;
}
