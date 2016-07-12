<?php

/**
 * Implements theme_menu_link().
 *
 * For when you want to exclude a menu link on a particular page.
 * Useful when you only want to have the link to the parent page on sub pages.
 * Put this function in the template.php file of your theme.
 * The 'MENUNAME' is the machine name of the menu you wish to customize.
 * Use underscores instead of dashes when including menu name in function name.
 *
 * @param array $variables
 *   Structured array data for a menu link.
 *
 * @return string
 *   Empty string to skip the link or link markup to include the link.
 */
function THEMENAME_menu_link__MENUNAME(array $variables) {
  $page_title = drupal_get_title();
  $current_path = current_path();

  // Exclude the menu link for this page if we are on the specified page.
  if ($page_title === "put your page title here" && $variables['element']['#href'] === $current_path) {
    return '';
  }
  return theme_menu_link($variables);
}
