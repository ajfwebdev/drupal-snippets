<?php

/**
 * @file
 * Provides the custom blocks.
 */

/**
 * Implements hook_block_info().
 */
function my_module_blocks_block_info() {
  $blocks['footer_legal'] = array(
    // The name that will appear in the block list.
    'info' => t('Footer Legal'),
    // Default setting.
    'cache' => DRUPAL_CACHE_PER_ROLE,
    'status' => 1,
    'region' => 'footer',
    'theme' => 'my_theme',
  );
  $blocks['logo-block'] = array(
    // The name that will appear in the block list.
    'info' => t('Site Logo and Title'),
    // Default setting.
    'cache' => DRUPAL_CACHE_PER_ROLE,
    'status' => 1,
    'region' => 'header',
    'theme' => 'my_theme',
  );

  // Changed key from 'callout_block' to 'callout-block' to make it work.
  $blocks['callout-block'] = array(
    // The name that will appear in the block list.
    'info' => t('Callout Block'),
    // Default setting.
    'cache' => DRUPAL_CACHE_PER_ROLE,
    'status' => 1,
    // For block to be listed as disabled on blocks page, set region to -1.
    'region' => -1,
    'theme' => 'my_theme',
  );
  return $blocks;
}

/**
 * Implements hook_block_info_alter().
 */
function my_module_blocks_block_info_alter(&$blocks, $theme, $code_blocks) {
  if ($theme === 'my_theme') {

    // Place the Main Menu block in the header region.
    if (isset($blocks['menu']['menu-main-menu'])) {
      $blocks['menu']['menu-main-menu']['status'] = 1;
      $blocks['menu']['menu-main-menu']['cache'] = 1;
      $blocks['menu']['menu-main-menu']['region'] = 'header';
    }

    // Place the Verticals Menu block in the hero region.
    if (isset($blocks['menu']['menu-verticals'])) {
      $blocks['menu']['menu-verticals']['status'] = 1;
      $blocks['menu']['menu-verticals']['cache'] = 1;
      $blocks['menu']['menu-verticals']['region'] = 'hero';
      $blocks['menu']['menu-verticals']['title'] = '<none>';
      $blocks['menu']['menu-verticals']['visibility'] = 1;
      $blocks['menu']['menu-verticals']['pages'] = '<front>';
      $blocks['menu']['menu-verticals']['custom'] = FALSE;
    }

    // Place the Latest News view block in the Lower Full Width region.
    if (isset($blocks['views']['latest_news-block'])) {
      $blocks['views']['latest_news-block']['status'] = 1;
      $blocks['views']['latest_news-block']['cache'] = 1;
      $blocks['views']['latest_news-block']['region'] = 'lower_full';
      $blocks['views']['latest_news-block']['visibility'] = 1;
      $blocks['views']['latest_news-block']['pages'] = '<front>';
    }

    // Place the Footer Menu block in the footer region.
    if (isset($blocks['menu']['menu-footer-menu'])) {
      $blocks['menu']['menu-footer-menu']['status'] = 1;
      $blocks['menu']['menu-footer-menu']['cache'] = 1;
      $blocks['menu']['menu-footer-menu']['region'] = 'footer';
    }
  }
}

/**
 * Implements hook_block_configure().
 */
function my_module_blocks_block_configure($delta = NULL) {
  $form = [];
  switch ($delta) {
    case 'footer_legal':
      $form['text_body'] = [
        '#type' => 'text_format',
        '#title' => t('Body'),
        '#default_value' => '<p>© 2009-2017 etc. - All rights reserved.</p><p>More info...</p>',
      ];
      break;

    case 'logo-block':
      $form['site_logo'] = [
        '#name' => 'logo_image',
        '#type' => 'managed_file',
        '#title' => t('Choose a Logo Image File'),
        '#description' => t('Select a Logo Image.  Only *.gif, *.png, *.jpg, and *.jpeg images allowed.'),
        '#default_value' => variable_get('my_module_logo_image_fid', ''),
        '#upload_location' => 'public://logo_image/',
        '#upload_validators' => ['file_validate_extensions' => ['gif png jpg jpeg']],
      ];
      $form['site_title'] = [
        '#type' => 'textfield',
        '#title' => t('Site Title'),
        '#default_value' => variable_get('my_module_site_title_text', ''),
        '#description' => t('The site title to display in the header. If the title is part of the logo image, leave this field blank.'),
      ];
      break;

    case 'callout-block':
      $form['callout_body_text'] = [
        '#type' => 'text_format',
        '#title' => t('Body'),
        '#default_value' => variable_get('my_module_callout_body_text', ''),
      ];
      break;
  }
  return $form;
}

/**
 * Implements hook_block_save().
 */
function my_module_blocks_block_save($delta = '', $edit = []) {
  switch ($delta) {
    case 'footer_legal':
      variable_set('my_module_footer_legal_text', $edit['text_body']['value']);
      break;

    case 'logo-block':
      variable_set('my_module_site_title_text', $edit['site_title']);
      my_module_blocks_save_logo($edit);
      break;

    case 'callout-block':
      variable_set('my_module_callout_body_text', $edit['callout_body_text']['value']);
      break;
  }
}

/**
 * Saves the logo image file.
 *
 * @param array $edit
 *   - Contains data from the submitted block edit form.
 */
function my_module_blocks_save_logo(array $edit) {
  if (!empty($edit['site_logo'])) {
    $file = file_load($edit['site_logo']);
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    $block = block_load('my_module_blocks', 'logo-block');
    file_usage_add($file, 'my_module_blocks', 'block', $block->bid);
    variable_set('my_module_logo_image_fid', $file->fid);
  }
  else {
    variable_set('my_module_logo_image_fid', '');
  }
}

/**
 * Implements hook_block_view().
 */
function my_module_blocks_block_view($delta = NULL) {
  $block = [];
  switch ($delta) {
    case 'footer_legal':
      $block['content'] = my_module_blocks_footer_legal_content();
      break;

    case 'logo-block':
      $block['content'] = my_module_blocks_logo_block_content();
      break;

    case 'callout-block':
      $block['content'] = my_module_blocks_callout_block_content();
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
function my_module_blocks_footer_legal_content() {
  $default_legal_text = '<p>© 2009-2017 etc. - All rights reserved.</p><p>More info...</p>';
  $footer_legal_text = variable_get('my_module_footer_legal_text', $default_legal_text);

  $content = [
    'message' => [
      '#type' => 'markup',
      '#markup' => $footer_legal_text,
      '#suffix' => '',
    ],
  ];

  return $content;
}

/**
 * Formats the content for the Header logo and title.
 *
 * @return string
 *   - The block content.
 */
function my_module_blocks_logo_block_content() {
  $site_title_text = variable_get('my_module_site_title_text', '');

  // Capture the image file path and form into HTML with attributes.
  $image_file = file_load(variable_get('my_module_logo_image_fid', ''));
  $image_path = '';

  if (isset($image_file->uri)) {
    $image_path = $image_file->uri;
  }

  $image = theme_image([
    'path' => ($image_path),
    'alt' => t('Site logo image'),
    'title' => t('Site Logo'),
    'attributes' => ['class' => 'site_logo'],
  ]);

  $content = [
    'site_logo_image' => [
      '#prefix' => '',
      '#type' => 'markup',
      '#markup' => $image,
    ],
    'site_title_text' => [
      '#type' => 'markup',
      '#markup' => $site_title_text,
      '#suffix' => '',
    ],
  ];

  return $content;
}

/**
 * Formats the content for the Callout block.
 *
 * @return string
 *   - The block content.
 */
function my_module_blocks_callout_block_content() {
  $default_callout_text = '';
  $callout_text = variable_get('my_module_callout_body_text', $default_callout_text);

  $content = [
    'callout_text' => [
      '#type' => 'markup',
      '#markup' => $callout_text,
      '#suffix' => '',
    ],
  ];

  return $content;
}
