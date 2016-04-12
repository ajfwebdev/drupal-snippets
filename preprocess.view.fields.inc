<?php

/**
 * Implements template_preprocess_views_view_fields.
 *
 * I was developing this code to change the format of start and end dates
 * for event nodes displayed by a view. It turned out there is a Drupal
 * module that provides this functionality, which is better than changing
 * the HTML markup on the fly like this.
 * This code depends on markup and CSS class names remaining static which
 * may not be the case over the long term.
 * Despite the drawbacks, I wanted to preserve the code because it uses things
 * like DOMDocument which I've never used before.
 *
 * I had this code in the template.php file for the active theme of the D7 site.
 */
function TEMPLATE_preprocess_views_view_fields(&$vars) {
  if ($vars['view']->name == 'events') {
    // First remove 'to' from start and end date markup.
    if (strpos($vars['fields']['field_event_date']->content, 'to') !== FALSE) {
      $vars['fields']['field_event_date']->content = str_replace('to', '', $vars['fields']['field_event_date']->content);
    }
    $dom = new DOMDocument();
    $dom->loadHTML($vars['fields']['field_event_date']->content);
    $spans = $dom->getElementsByTagName('span');
    $date_info = array();
    foreach ($spans as $span) {
      $date_info[$span->getAttribute('class')] = array(
        'span' => $dom->saveHTML($span),
        'raw_date' => $span->getAttribute('content'),
        'value' => $span->nodeValue,
      );
    }
    if (array_key_exists('date-display-single', $date_info)) {
      $single_date = TEMPLATE_process_single_event_date($date_info['date-display-single']['value']);
      $dom->getElementsByTagName('span')->item(0)->nodeValue = $single_date;
      $vars['fields']['field_event_date']->content = $dom->saveHTML();
    }
    elseif (array_key_exists('date-display-start', $date_info) && array_key_exists('date-display-end', $date_info)) {
      $dates_array = TEMPLATE_process_start_end_event_dates($date_info['date-display-start']['value'], $date_info['date-display-end']['value']);
      $dom->getElementsByTagName('span')->item(0)->nodeValue = $dates_array['start_date'];
      $dom->getElementsByTagName('span')->item(1)->nodeValue = $dates_array['end_date'];
      $vars['fields']['field_event_date']->content = $dom->saveHTML();
    }
  }
}

/**
 * Format date output for a single date.
 *
 * @param string $date
 *   The input date string.
 *
 * @return string $date
 *   A formatted date string.
 */
function TEMPLATE_process_single_event_date($date) {
  if (($timestamp = strtotime($date)) != FALSE) {
    $date = date('M d Y', $timestamp);
  }
  return $date;
}

/**
 * Format date output for start and end dates.
 *
 * @param string $start_date
 *   The input start date string.
 * @param string $end_date
 *   The input end date string.
 *
 * @return array
 *   An associative array containing the formatted start and end dates.
 */
function TEMPLATE_process_start_end_event_dates($start_date, $end_date) {
  $start_date_fmt = '';
  $end_date_fmt = '';
  if (($start_timestamp = strtotime($start_date)) != FALSE) {
    if (($end_timestamp = strtotime($end_date)) != FALSE) {
      $start_month = date('M', $start_timestamp);
      $end_month = date('M', $end_timestamp);
      if ($start_month == $end_month) {
        $start_date_fmt = date('M d', $start_timestamp);
        $end_date_fmt .= date(' - d Y', $end_timestamp);
      }
      else {
        $start_date_fmt = date('M d', $start_timestamp);
        $end_date_fmt .= date(' - M d Y', $end_timestamp);
      }
    }
  }
  return array(
    'start_date' => $start_date_fmt,
    'end_date' => $end_date_fmt,
  );
}
