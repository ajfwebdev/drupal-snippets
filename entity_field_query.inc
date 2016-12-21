<?php

/**
 * This function looks up a Entities by date range for a given ID.
 *
 * @param string $id
 *   ID to get entities for.
 * @param int $start_date
 *   Start date (unix timestamp).
 * @param int $end_date
 *   End date (unix timestamp).
 *
 * @return array $entity_objects
 *   Enrollment entities.
 */
function my_module_get_entities_by_date_range($id, $start_date, $end_date) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'my_entity')
    ->entityCondition('bundle', 'my_entity_type')
    ->fieldCondition('my_target_id', 'value', $id)
    ->propertyCondition('created', array($start_date, $end_date), 'BETWEEN');
  $result = $query->execute();
  $entity_objects = array();
  if (!empty($result['my_entity'])) {
    $entity_ids = array_keys($result['my_entity']);
    $entity_objects = entity_load('my_entity', $entity_ids);
  }

  return $entity_objects;
}
