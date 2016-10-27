<?php
/**
 * @file
 * For hero images on the admitted students pages.
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
  'content_types' => array('my_module_hero_image'),
  'title' => t('Admitted Students Hero Image'),
  'description' => t('Use to add hero image and title for the current node with special markup.'),
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

  $help_text = t('This pane will add the Hero Image from the current node, overlaid with a title that you specify on this form.');
  $form['form_help'] = array(
    '#markup' => '<div><strong>' . $help_text . '</strong></div>',
  );
  $form['title_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Hero Title'),
    '#description' => t('Enter the title to overlay the hero image.'),
    '#default_value' => isset($conf['title_text']) ? $conf['title_text'] : '',
  );
  $form['title_heading_options'] = array(
    '#type' => 'value',
    '#value' => array(
      'h1' => 'h1',
      'h2' => 'h2',
      'h3' => 'h3',
    ),
  );
  $form['title_heading'] = array(
    '#title' => t('Title Heading Level'),
    '#type' => 'select',
    '#description' => t('Select the heading level to be used for this title.') ,
    '#default_value' => isset($conf['title_heading']) ? $conf['title_heading'] : 'h1',
    '#options' => $form['title_heading_options']['#value'],
  );
  return $form;
}

/**
 * Ctools edit form submit handler.
 */
function my_module_hero_image_edit_form_submit($form, &$form_state) {
  $form_fields = array(
    'title_text',
    'title_heading',
  );
  foreach ($form_fields as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }

}

/**
 * Render callback function.
 */
function my_module_hero_image_render($subtype, $conf, $args, $context) {
  $block = new stdClass();
  $block->content = my_module_hero_image_build_markup($conf, $context);
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
function my_module_hero_image_build_markup($conf, $context) {
  $output = '';

  // Get the url for the hero image file.
  if (!empty($context)) {
    $hero_image_url = ctools_context_keyword_substitute('%node:field_hero_image', array(), $context);
    $hero_image_alt = ctools_context_keyword_substitute('%node:field_image_alt_text', array(), $context);
  }

  // Build markup and return.
  $output .= '<div class="hero-image--image">';
  $output .= '<div class="hero-image__container">';
  $output .= '<img src="' . $hero_image_url . '" alt="' . $hero_image_alt . '"/>';
  $output .= '</div>';

  $output .= '<div class="page-title__wrapper">';
  $output .= '<' . $conf['title_heading'] . ' class="page-title">' . $conf['title_text'] . '</' . $conf['title_heading'] . '>';
  $output .= '</div>';
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
