<?php

/**
 * @file
 * This is an example usage of theme_menu_link() hook.
 */

/**
 * Implements theme_menu_link().
 *
 * In the verticals menu section, we are getting attributes from $element['#localized_options']
 * Which is made possible by installing the menu attributes contrib module and adding some
 * custom attributes to each menu link in the menu link UI.
 */
function mytheme_menu_link(&$variables) {
  $element = $variables['element'];
  $sub_menu = '';

  // If link is part of main menu.
  if ($element['#theme'] == 'menu_link__main_menu') {

    // If link includes sublinks, insert button before output of sublinks.
    if ($element['#below']) {
      $sub_menu = '<button class="main-menu__sublinks-toggle">';
      $sub_menu .= '<svg xmlns="http://www.w3.org/2000/svg" width="30" height="30" viewBox="0 0 30 30" class="icon-double-arrow-down" aria-labelledby="icon-double-arrow-down-title icon-double-arrow-down-desc" role="presentation"><title id="icon-double-arrow-down-title">Double Arrow Down</title><description id="icon-double-arrow-down-desc">Icon of two arrows pointing down.</description><g fill="#0073EF"><path d="M14.4 17.1L0 1.7 1.6 0l12.8 13.8L28.5 0 30 1.8"/><path d="M14.4 30L0 14.6l1.6-1.7 12.8 13.8 14.1-13.8 1.5 1.7"/></g></svg>';
      $sub_menu .= '<span class="main-menu__sublinks-toggle-text">Sublinks</span>';
      $sub_menu .= '</button>';
      $sub_menu .= drupal_render($element['#below']);
    }
  }

  // If link is part of the verticals menu.
  if ($element['#theme'] == 'menu_link__menu_verticals') {
    $title_markup = '<span class="verticals-menu__link-title">';
    $title_markup .= check_plain($element['#title']);
    $title_markup .= '</span>';

    $subtitle_markup = '';
    if (isset($element['#localized_options']['attributes']['name'])) {
      $subtitle_markup = '<span class="verticals-menu__link-subtitle">';
      $subtitle_markup .= check_plain($element['#localized_options']['attributes']['name']);
      $subtitle_markup .= '</span>';
    }

    $output = '<a href="' . check_plain(url($element['#href'])) . '">';
    $output .= $title_markup;
    $output .= $subtitle_markup;
    $output .= '</a>';
  }
  else {
    $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  }
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}
