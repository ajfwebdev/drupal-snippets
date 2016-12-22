<?php
/**
 * Examples of using entity_metadata_wrapper to get entity field data.
 *
 * Also shows how to get entity property data.
 */

// Load some user entity.
$uid = 1;
$user_obj = user_load($uid);

// Use the entity object itself to get entity property data.
$user_created_date = $user_obj->created;

// Create an entity metadata wrapper for the user object.
$user_wrapper = entity_metadata_wrapper('user', $user_obj);

// All entity metadata wrapper operations should be in try/catch blocks.
try {
  // Use the entity metadata wrapper to get entity field data.
  $effective_date = $user_wrapper->field_effective_date->value();
  // For a date field with "from" and "to" dates, access each date like so.
  $from_date = $user_wrapper->field_policy_date->value->value();
  $to_date = $user_wrapper->field_policy_date->value2->value();

}
catch (EntityMetadataWrapperException $e) {
  watchdog(__FUNCTION__, 'User Profile Exception: %t', array('%t' => $e->getMessage()));
}
