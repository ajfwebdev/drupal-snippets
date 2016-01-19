<?php

/**
 * Implements hook_theme().
 *
 * Instructs Drupal to use a custom node template file
 * for all nodes of the type, hello_world_article.
 */
function hello_world_theme($existing, $type, $theme, $path) {
  $theme = array();
  $theme['node__hello_world_article'] = array(
    'render element' => 'content',
    'base hook' => 'node',
    'template' => 'node--hello_world_article',
    'path' => drupal_get_path('module', 'hello_world') . '/templates',
  );
  return $theme;
}
