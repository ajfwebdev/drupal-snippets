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

/**
 * Example where one fieldCondition is on an Entity Reference field.
 *
 * @param string $entity_reference_id
 *   The entity reference ID to get Bundle objects for.
 * @param array $status_array
 *   One or many status values to filter on.
 *
 * @return array $my_bundle_objects
 *   An associative array containing:
 *   - Bundle objects matching the input criteria.
 */
function my_module_get_my_bundle_by_status($entity_reference_id, $status_array) {
  $my_bundle_objects = array();

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'my_entity')
    ->entityCondition('bundle', 'my_bundle')
    ->fieldCondition('field_my_entity_reference_field', 'target_id', $entity_reference_id)
    ->fieldCondition('field_my_status', 'value', $status_array, 'IN');
  $result = $query->execute();
  if (!empty($result['my_entity'])) {
    $entity_ids = array_keys($result['my_entity']);
    $my_bundle_objects = entity_load('my_entity', $entity_ids);
  }

  return $my_bundle_objects;
}
