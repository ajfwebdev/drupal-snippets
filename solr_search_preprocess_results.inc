<?php

/**
 * @file
 * Examples of preprocessing Apachesolr search results.
 */

 /**
 * Implements hook_preprocess_search_result().
 */
function mymodule_preprocess_search_result(&$vars) {
  if (isset($vars['result']['node'])) {
    $node = $vars['result']['node'];

    // Adds created date info to the search result.
    $vars['info'] = format_date($node->created);
    $vars['datetime'] = date('c', $node->created);

    // Some nodes have no snippet because all text is entity referenced. Like when using Paragraphs text.
    if ($node->bundle == 'my_referency_page') {
      try {
        $emw = entity_metadata_wrapper('node', $node->entity_id);
        $paragraphs_array = $emw->field_paragraphs->value();
        $para_text = strip_tags($paragraphs_array[0]->field_para_text['und'][0]['safe_value']);
        $para_text = substr($para_text, 0, 300);
        $vars['snippet'] = $para_text . '...';
      }
      catch (EntityMetadataWrapperException $e) {
        watchdog(__FUNCTION__, 'Entity Metadata Wrapper Exception: %t', array('%t' => $e->getMessage()));
      }
    }
  }
  else {
    // Prevent non-node results from breaking the page markup.
    if (isset($vars['snippet'])) {
      $vars['snippet'] = NULL;
    }
  }

}
