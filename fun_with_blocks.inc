<?php

/**
 * @file
 * Provides custom blocks.
 */

/**
 * Implements hook_block_info().
 */
function my_module_block_info() {
  $blocks['footer_legal'] = array(
    // The name that will appear in the block list.
    'info' => t('Footer Legal'),
    // Default setting.
    'cache' => DRUPAL_CACHE_PER_ROLE,
    'status' => 1,
    'region' => 'footer',
  );
  return $blocks;
}

/**
 * Implements hook_block_configure().
 */
function my_module_block_configure($delta = NULL) {
  $form = [];
  switch($delta) {
    case 'footer_legal' :
      $form['text_body'] = [
      '#type' => 'text_format',
      '#title' => t('Body'),
      '#default_value' => '<p>© 2009-2017 My Company, LLC - All rights reserved.</p><p>"Some text" and "Some more text" are trademarks, and "My Company" is a registered trademark, of My Company, LLC.</p>',
      ];
      break;
  }
  return $form;
}

/**
 * Implements hook_block_save().
 */
function my_module_block_save($delta = '', $edit = []) {
  switch($delta) {
    case 'footer_legal' :
      // Saving the WYSIWYG text
      variable_set('footer_legal_text', $edit['text_body']['value']);
      break;
  }
}

/**
 * Implements hook_block_view().
 */
function my_module_block_view($delta = NULL) {
  $block = [];
  switch($delta) {
    case 'footer_legal' :
      $block['content'] = my_module_footer_legal_content();
      break;
  }
  return $block;
}

/**
 * Formats the content for the Footer Legal block.
 *
 * @return string
 *   - The block content.
 */
function my_module_footer_legal_content() {
  $default_legal_text = '<p>© 2009-2017 My Company, LLC - All rights reserved.</p><p>"Some text" and "Some more text" are trademarks, and "My Company" is a registered trademark, of My Company, LLC.</p>';
  $footer_legal_text = variable_get('footer_legal_text', $default_legal_text);

  $content = [
    'message' => [
      '#type' => 'markup',
      '#markup' => $footer_legal_text,
      '#suffix' => '',
    ],
  ];

  return $content;
}
