<?php

/**
 * @file
 * Using hook_preprocess_node() in a module file.
 *   - To first add theme hook suggestions to use for a new template file for a node teaser display mode.
 *   - Then to make some new variables available to that new template file.
 */

/**
 * Implements hook_preprocess_node().
 */
function mymodule_preprocess_node(&$vars) {

  // Creating theme suggestions for custom templates for node view modes.
  $vars['theme_hook_suggestions'][] = "node__" . $vars['type'] . "__" . $vars['view_mode'];
  $vars['theme_hook_suggestions'][] = "node__" . $vars['view_mode'];

  // Add the node view mode to the class array for styling purposes.
  $vars['classes_array'][] = 'node-' . $vars['view_mode'];

  if ($vars['type'] == 'article' && $vars['view_mode'] == 'teaser') {
    // Make some custom data variables available to the new node view mode template file.
    $created_date = $vars['created'];
    $date = format_date($created_date, 'custom', 'm/d/Y');
    $vars['date'] = $date;
    $vars['content']['links']['node']['#links']['node-readmore']['title'] = 'Continue Reading >><span class="element-invisible"> about ' . $vars['title'] . '</span>';
    $vars['name'] = '';
  }
}
