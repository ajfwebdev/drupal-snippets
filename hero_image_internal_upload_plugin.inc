<?php
/**
 * An attempt at a cTools panels plugin that would include an image uploader
 * in the edit form, that would upload image as a managed file.
 * This almost worked but ran into strange issues.
 */

/**
 * This function would go in the my_module.module file, not in the plugin file.
 */

function my_module_fix_ajax_upload($element, &$form_state, $form) {

  // process $element as normal
  $element = file_managed_file_process($element, $form_state, $form);

  // remove path, add callback
  unset($element['upload_button']['#ajax']['path']);
  $element['upload_button']['#ajax']['callback'] = 'file_ajax_upload_callback';

  return $element;

}

/**
 * The following code is from the plugin file.
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

form_load_include($form_state, 'inc', 'my_module','plugins/content_types/hero_image');

$plugin = array(
  'single' => TRUE,
  'content_types' => array('my_module_hero_image'),
  'title' => t('Admitted Students Hero Image'),
  'description' => t('Use to add hero images to the admitted students pages.'),
  'category' => t('Custom Panes'),
  'edit form' => 'my_module_hero_image_edit_form',
  'render callback' => 'my_module_hero_image_render',
  'admin info' => 'my_module_hero_image_admin_info',
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'defaults' => array(
    'text' => '',
  ),
  'all contexts' => TRUE,
);

/**
 * Ctools edit form.
 */
function my_module_hero_image_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  // Use the #managed_file FAPI element to upload an image file.
  $form['hero_image'] = array(
    '#title' => t('Image'),
    '#type' => 'managed_file',
    '#description' => t('Upload an image.'),
    '#default_value' => isset($conf['hero_image']) ? $conf['hero_image'] : '',
    '#upload_location' => 'public://',
    '#process' => array('my_module_fix_ajax_upload'),
  );
  $form['title_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('Enter the title to overlay the hero image.'),
    '#default_value' => isset($conf['title_text']) ? $conf['title_text'] : '',
  );
  $form['title_heading_options'] = array(
    '#type' => 'value',
    '#value' => array(
      'h1' => 'h1',
      'h2' => 'h2',
      'h3' => 'h3',
      'skip-this-title' => t('Skip this title'),
    ),
  );
  $form['title_heading'] = array(
    '#title' => t('Title Heading Level'),
    '#type' => 'select',
    '#description' => t('Select the heading level to be used for this title, or "Skip this title" for no title.') ,
    '#default_value' => isset($conf['title_heading']) ? $conf['title_heading'] : 'h2',
    '#options' => $form['title_heading_options']['#value'],
  );
  $form['teaser_url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL'),
    '#description' => t('Enter the URL for the link. For external links use the full URL (including http:// or https://). For internal links a Drupal site-relative URL can be entered.'),
    '#default_value' => isset($conf['teaser_url']) ? $conf['teaser_url'] : '',
  );
  $form['button_text_options'] = array(
    '#type' => 'value',
    '#value' => array(
      'Learn More' => t('Learn More'),
      'Watch Video' => t('Watch Video'),
      'skip-this-button' => t('Skip this button'),
    ),
  );
  $form['button_text'] = array(
    '#title' => t('Button Link Text'),
    '#type' => 'select',
    '#description' => t('Select the text to be used on the button link, or "Skip this button" for no button.'),
    '#default_value' => isset($conf['button_text']) ? $conf['button_text'] : t('Learn More'),
    '#options' => $form['button_text_options']['#value'],
  );
  $form['color_options'] = array(
    '#type' => 'value',
    '#value' => array(
      'crimson' => t('Crimson'),
      'blue' => t('Blue'),
      'gold' => t('Gold'),
    ),
  );
  $form['color'] = array(
    '#title' => t('Teaser Color'),
    '#type' => 'select',
    '#description' => t('Select the accent color to be used in this teaser.'),
    '#default_value' => isset($conf['color']) ? $conf['color'] : t('Crimson'),
    '#options' => $form['color_options']['#value'],
  );
  return $form;
}

/**
 * Ctools edit form submit handler.
 */
function my_module_hero_image_edit_form_submit($form, &$form_state) {
  $form_fields = array(
    'hero_image',
    'title_text',
    'title_heading',
    'teaser_url',
    'button_text',
    'color',
  );
  foreach ($form_fields as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }

  // Save the image file permanently.
  if (isset($conf['hero_image'])) {
    $file = file_load($conf['hero_image']);

    $file->status = FILE_STATUS_PERMANENT;

    $file_saved = file_save($file);

    // Record that the module is using the file.
    file_usage_add($file_saved, 'my_module_hero_image_edit_form', 'hero_image', $file_saved->fid);
  }
}

/**
 * Render callback function.
 */
function my_module_hero_image_render($subtype, $conf, $args, $context) {
  $block = new stdClass();
  $block->content = my_module_hero_image_build_markup($conf);
  return $block;
}

/**
 * Builds markup for the content of our block.
 *
 * @param array $conf
 *   An associative array containing the pane values.
 *
 * @return string $output
 *   The markup to use for the Block content.
 */
function my_module_hero_image_build_markup($conf) {
  $output = '';
  global $base_url;

  // See if link url needs formatting.
  if (strpos($conf['teaser_url'], '://')) {
    $teaser_url = $conf['teaser_url'];
  }
  else {
    $conf['teaser_url'] = ltrim($conf['teaser_url'], '/');
    $teaser_url = $base_url . '/' . $conf['teaser_url'];
  }

  // Load the hero image file.
  $file = file_load($conf['hero_image']);
  $hero_image = image_load($file->uri);
  $hero_image_content = array(
    'file' => array(
      '#theme' => 'image_style',
      '#style_name' => 'large',
      '#path' => $hero_image->source,
      '#width' => $hero_image->info['width'],
      '#height' => $hero_image->info['height'],
    ),
  );

  // Build markup and return.
  $output .= '<div class="hero-image--image">';
  $output .= '<div class="hero-image__container">';
  $output .= drupal_render($hero_image_content);
  $output .= '</div>';

  if (!($conf['title_heading'] === 'skip-this-title')) {
    $output .= '<div class="page-title__wrapper">';
    $output .= '<' . $conf['title_heading'] . ' class="page-title">' . $conf['title_text'] . '</' . $conf['title_heading'] . '>';
    $output .= '</div>';
  }
  if (!($conf['button_text'] === 'skip-this-button')) {
    $output .= '<a class="btn learn-more btn--' . $conf['color'] . '" href="' . $teaser_url . '">' . $conf['button_text'] . '</a>';
  }
  $output .= '</div></div>';

  return $output;
}

/**
 * Admin info callback for panel pane.
 */
function my_module_hero_image_admin_info($subtype, $conf, $context) {
  // Set the title that shows on the admin Content page to the overridden title.
  $block = new stdClass();
  if (!empty($conf['title_text'])) {
    $conf_title = $conf['title_text'];
  }
  else {
    $conf_title = 'No Title';
  }
  $block->title = $conf['override_title_text'] ? $conf['override_title_text'] : $conf_title;
  return $block;
}

