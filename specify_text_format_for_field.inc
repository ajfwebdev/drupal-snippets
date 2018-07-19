<?php

/**
 * This code goes in a custom module. It will set the default text format
 * for data entry on a specific field. For when you have a wysiwyg field
 * that you want users to use a specific text format with.
 */


/**
 * Implements hook_element_info_alter().
 */
function my_module_element_info_alter(&$type) {
  if (isset($type['text_format']['#process'])) {
    foreach ($type['text_format']['#process'] as &$callback) {
      if ($callback === 'filter_process_format') {
        $callback = 'my_module_set_text_format';
      }
    }
  }
}

/**
 * Set a specific text format for content entry on certain fields.
 */
function my_module_set_text_format($element) {
  $element = filter_process_format($element);
  if (empty($element['#bundle']) || empty($element['#field_name'])) {
    return $element;
  }

  if ($element['#bundle'] == 'page' && $element['#field_name'] == 'field_my_long_text') {
    $element['format']['format']['#default_value'] = 'my_text_format_name';
  }

  return $element;
}
