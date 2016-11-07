<?php
/**
 * @file
 *
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
  'content_types' => array('my_module_plugin'),
  'title' => t('Current node field collection fields with custom markup'),
  'description' => t('Shows ways of pulling different fields from a field collection.'),
  'category' => t('Custom Panes'),
  'edit form' => 'my_module_plugin_edit_form',
  'render callback' => 'my_module_plugin_render',
  'admin info' => 'my_module_plugin_admin_info',
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'all contexts' => TRUE,
);

/**
 * Implements hook_form().
 *
 * Required to be able to add the content pane in Panels.
 */
function my_module_plugin_edit_form($form, &$form_state) {
  return $form;
}

/**
 * Render callback function.
 */
function my_module_plugin_render($subtype, $conf, $args, $context) {
  $block = new stdClass();
  $block->content = my_module_plugin_build_markup($conf, $context);
  return $block;
}

/**
 * Builds markup for the pane content.
 *
 * @param array $conf
 *   An associative array containing the pane values.
 *
 * @return string $output
 *   The markup to use for the Block content.
 */
function my_module_plugin_build_markup($conf, $context) {
  $output = '';
  $planning_array = array();

  // Get data from the node in this context.
  if (!empty($context)) {
    $node = node_load($context['panelizer']->data->nid);

    // Build an array of field collection values.
    $statistic_collection_ids = $node->field_my_field_collection_field['und'];
    foreach ($statistic_collection_ids as $id_array) {
      $collection_id = $id_array['value'];
      $collection_values = entity_load('field_collection_item', array($collection_id));
      $planning_array[$collection_id] = array(
        'image_url' => file_create_url($collection_values[$collection_id]->field_background_image['und'][0]['uri']),
        'image_alt_text' => check_plain($collection_values[$collection_id]->field_background_image['und'][0]['alt']),
        'icon_name' => check_plain($collection_values[$collection_id]->field_icon['und'][0]['value']),
        'title_text' => $collection_values[$collection_id]->field_title_text['und'][0]['safe_value'],
        'body_text_markup' => $collection_values[$collection_id]->field_body_text['und'][0]['value'],
      );
    }
  }

  // Build markup and return.
  $output .= '<div class="my-section">';
  foreach ($planning_array as $id => $field_values) {
    $output .= '<div class="background-image--image icon-' . $field_values['icon_name'] . '">';
    $output .= '<img src="' . $field_values['image_url'] . '" alt="' . $field_values['image_alt_text'] . '"/></div>';
    $output .= '<h2 class="my-title">' . $field_values['title_text'] . '</h2>';
    $output .= $field_values['body_text_markup'];
  }
  $output .= '</div>';

  return $output;
}

/**
 * Admin info callback for panel pane.
 */
function my_module_plugin_admin_info($subtype, $conf, $context) {
  // Set the title that shows on the admin Content page to the overridden title.
  $block = new stdClass();
  $title_text = "My Special Page Section";

  if (!empty($title_text)) {
    $admin_title = $title_text;
  }
  else {
    $admin_title = 'No Title';
  }
  $block->title = $conf['override_title_text'] ? $conf['override_title_text'] : $admin_title;
  return $block;
}
