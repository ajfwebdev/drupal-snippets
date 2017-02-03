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

/**
 * Can also create an entity metadata wrapper using the entity ID instead of the object.
 * When you do this the wrapper is lazy-loaded, meaning it is not populated with data at first.
 * It is only populated/loaded when you access the field data.
 *
 * The entity name is used even when the ID belongs to a particular bundle of that entity.
 * You don't have to specify the bundle name.
 * Entity IDs are unique (within the entity at least), so it will find the correct bundle instance.
 *
 * $my_entity_wrapper (below) will be lazy-loaded. If you inspect the wrapper after this instanciation in a
 * debugger it will appear to be empty.
 */
 $my_entity_wrapper = entity_metadata_wrapper('my_entity_name', $entity_id);

 // This statement will cause the wrapper to load the field data and return it.
 $my_field = $my_entity_wrapper->field_my_field_name->value();

/**
 * If the entity has an entity reference field to another entity, you can access the referenced
 * entity's data also from the same wrapper.
 * I've heard this process is called "chaining".
 *
 * 'title' here is the title field of the referenced entity. You can get any field in the referenced
 * entity this way, though the chaining may vary based on the referenced entity's data structure.
 */
$referenced_entity_title = $my_entity_wrapper->field_my_entity_reference_field->title->value();