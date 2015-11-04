<?php

/**
 * Snippet to download a text file from a Drupal module.
 *
 * This does not store the file on the server. It writes
 * the text directly from PHP.
 */

/**
 * In the hook_menu() function, add an item for the download callback function.
 * It needs it's own page because it has to send HTTP headers in order to download a file.
 * I'm using %var_1 and $var_2 as wildcard path variables to pass information to
 * the function that will determine what kind of data to output for download.
 */
  $items['some/path/for/%var_1/%var_2/download/function'] = array(
    'title' => 'Text File Download',
    'description' => 'Download some data in text format.',
    'page callback' => 'ajf_data_download',
    'page arguments' => array(3, 4),
    'access arguments' => array('some permission'),
    'weight' => 0,
    'type' => MENU_CALLBACK,
  );

/**
 * Add a link that will resolve to the path you added to your hook_menu() implementation.
 * You can add the link to any page. I've added it to a page that displays a form, so
 * the link is in a form element.
 */
  $form['data_download'] = array(
    '#markup' => l(t('Download this data as a text file'), 'some/path/for/' . $var1 . '/' . $var2 . '/download/function'),
  );
  
/**
 * Menu Callback: Downloads data in text format.
 *
 * @param string $var1
 *   Data from the first wildcard parameter in the hook_menu() item path.
 * @param string $var2
 *   Data from the second wildcard parameter in the hook_menu() item path.
 */
function ajf_data_download($var1, $var2) {
  // Here I'll just use $var1 and $var2 to format the file name.
  $filename = 'invoice_' . $var1 . '_' . $var2 . '_format.txt';
  $text = 'Hello World!';

  // Now set the HTTP headers.
  header("Content-type: text/plain");
  header("Content-Disposition: attachment; filename=" . $filename);

  // Now download the text file. The 'print' function is all you need.
  print $text;
}
