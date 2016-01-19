<?php

/**
 * Implements hook_block_info().
 *
 * Creates a custom block, Hello World Block, and
 * assigns it to the Sidebar Second region.
 */
function hello_world_block_info() {
  $blocks = array();
  $blocks['hello_world_block'] = array(
    'info' => t('Hello World Block'),
    'status' => TRUE,
    'region' => 'sidebar_second',
  );
  return $blocks;
}
