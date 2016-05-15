<?php

/**
 * Apply formatting rules to certain user-supplied values.
 *
 * This function is an example of calling the array_splice_assoc function.
 *
 * @param array $web_data
 *   A reference to the array of submitted values.
 */
function _my_module_format_submitted_values(&$web_data) {

  // Height format (4' 0"), Weight format (180 lbs).
  $new_elements = array(
    'height' => $web_data['height_feet'] . "' " . $web_data['height_inches'] . '"',
    'weight' => $web_data['weight_new'] . ' lbs',
  );
  // Replace elements starting with key 'height_feet' and ending at key 'Gender' (the 'Gender' element remains afterwards).
  _my_module_array_splice_assoc($web_data, 'height_feet', 'Gender', $new_elements);

}

/**
 * Inserts new elements into an associative array with replacement option.
 *
 * Works like array_splice() but uses associative array keys instead of
 * numeric offsets.
 *
 * This code is from royanee at yahoo dot com, found in PHP documentation,
 * http://php.net/manual/en/function.array-splice.php
 *
 * @param array $input
 *   The original array to be modified.
 * @param string $offset
 *   The key of the input array element to begin the replacement.
 * @param string $length
 *   The key of the input array element to stop replacement.
 * @param array $replacement
 *   The new array element or elements to insert into the original array.
 */
function _my_module_array_splice_assoc(&$input, $offset, $length, $replacement) {
  $replacement = (array) $replacement;
  $key_indices = array_flip(array_keys($input));
  if (isset($input[$offset]) && is_string($offset)) {
    $offset = $key_indices[$offset];
  }
  if (isset($input[$length]) && is_string($length)) {
    $length = $key_indices[$length] - $offset;
  }

  $input = array_slice($input, 0, $offset, TRUE)
    + $replacement
    + array_slice($input, $offset + $length, NULL, TRUE);
}
