<?php

/**
 * Implements hook_block_view().
 *
 * Displays the custom block, Hello World Block, only
 * on node pages of the hello_world_article node type.
 */
function hello_world_block_view($delta = '') {
  $block = array();
  switch ($delta) {
    case 'hello_world_block':
      $block['subject'] = '';
      $node = menu_get_object();
      if (isset($node->type) && $node->type == 'hello_world_article') {
        $block['content'] = _hello_world_block_content();
      }
      else {
        $block['content'] = '';
      }
      break;
  }
  return $block;
}

/**
 * Builds the markup for the Hello World Block.
 *
 * Conditions for node link inclusion:
 * - Nodes should be of the Hello World Article content type.
 * - Nodes are tagged with "Enabled" terms from the Sections vocabulary.
 *
 * @return string
 *   The markup for the Hello World Block.
 */
function _hello_world_block_content() {
  $output = '<div class="hello-world-block-title"><h2>' . t('Hello World!') . '</h2></div>';
  $output .= '<div class="hello-world-block-links"><ul>';
  $result = hello_world_node_links_query();
  foreach ($result as $row) {
    $output .= '<li>';
    $node_url = 'node/' . $row->nid;
    $node_title = check_plain($row->title);
    $output .= l($node_title, $node_url);
    $output .= '</li>';
  }
  $output .= '</ul></div>';
  return $output;
}

/**
 * Get data for node links from the database.
 *
 * Conditions for node inclusion:
 * - Nodes should be of the Hello World Article content type.
 * - Nodes are tagged with "Enabled" terms from the Sections vocabulary.
 *
 * @return object
 *   A DatabaseStatementInterface prepared statement
 *   object, already executed.
 */
function hello_world_node_links_query() {
  $query = 'SELECT
    n.nid
  , n.type
  , n.title
  , fe.bundle
  , fe.field_enabled_value
  FROM
  {node} n
  INNER JOIN
  {taxonomy_index} tx
  ON tx.nid = n.nid
  INNER JOIN
  {field_data_field_enabled} fe
  ON fe.entity_id = tx.tid
  AND fe.entity_type = :entity_type
  AND fe.field_enabled_value = :enabled_value
  AND fe.bundle = :bundle
  WHERE
  n.type = :content_type
  AND fe.bundle = :tx_vocabulary
  ORDER BY n.nid';
  return db_query($query, array(
    ':entity_type' => 'taxonomy_term',
    ':bundle' => 'sections',
    ':enabled_value' => 1,
    ':content_type' => 'hello_world_article',
    ':tx_vocabulary' => 'sections',
  ));
}
