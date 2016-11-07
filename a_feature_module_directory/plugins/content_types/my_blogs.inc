<?php

/**
 * @file
 * Content pane for My blog section for My Home.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('My Blogs'),
  'description' => t('My blog.'),
  'category' => t('Harvard My Home'),
  'edit form' => 'my_blogs_edit_form',
  'render callback' => 'my_blogs_render',
  'admin info' => 'my_blogs_info',
  'defaults' => array(
    'items' => 4,
  ),
);

function my_blogs_info($subtype, $conf, $contexts) {
  if (!empty($conf)) {
    $block = new stdClass();
    $block->title = $conf['override_title'] ? $conf['override_title_text'] : '';
    $block->content = t('Showning @i items from the My Blogs view', array(
      '@i' => $conf['items'],
    ));
    return $block;
  }
}

function my_blogs_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $form['items'] = array(
    '#title' => t('Items to show'),
    '#type' => 'textfield',
    '#default_value' => $conf['items'],
    '#required' => TRUE,
  );
  return $form;
}

function my_blogs_edit_form_submit($form, &$form_state) {
  $form_state['conf']['items'] = $form_state['values']['items'];
}

function my_blogs_render($subtype, $conf, $panel_args, $context = NULL) {
  $block = new stdClass();
  $block->title = '';
  $block->content = '';
  $view = views_get_view('my_blog_posts');
  if (is_object($view)) {
    $view->set_items_per_page($conf['items']);
    $view->pre_execute();
    $view->execute();
    $items = array();
    foreach ($view->result as $item) {
      $background = '';
      if (count($item->field_field_image)) {
        $image = image_style_url('500x500', $item->field_field_image[0]['raw']['uri']);
        $background = 'style="background:url(' . $image . ')"';
      }
      $author_image = file_load($item->users_node_picture);
      $author_image = image_style_url('thumbnail', $author_image->uri);
      $author_name = $item->users_node_name;
      if (count($item->field_field_first_name) && count($item->field_field_last_name)) {
        $author_name = $item->field_field_first_name[0]['raw']['value'];
        $author_name .= ' ' . $item->field_field_last_name[0]['raw']['value'];
      }

      $items[] = array(
        'title' => $item->node_title,
        'link' => l(t('Read More'), 'node/' . $item->nid),
        'author_name' => $author_name,
        'author_image' => $author_image,
        'background' => $background,
      );
    }
    $block->content = theme('my_blogs_block', array('items' => $items));
  }
  return $block;

}
