<?php

/**
 * Exmple of calling the _my_module_display_date function.
 *
 * $start_date would be a date in UNIX format, as a number of seconds since UNIX epoch.
 * Example: 1481979234 (ISO 8601: 2016-12-17T12:53:54Z)
 */
$timezone = "America/New_York";
$start_date = 1481979234;
_my_module_display_date($start_date, $timezone, "This is the Start Date");

/**
 * Utility function to format and display a date with a message.
 *
 * @param string $unix_date
 *   Unix format, numeric date.
 * @param string $timezone
 *   Human-readable timezone string.
 * @param string $message
 *   The message to display along with the date.
 */
function _my_module_display_date($unix_date, $timezone, $message) {
  date_default_timezone_set($timezone);
  $date_format = date('Y-m-d', $unix_date);
  // This will display as a Drupal notice on the page currently being rendered.
  drupal_set_message("$message $date_format");
}
