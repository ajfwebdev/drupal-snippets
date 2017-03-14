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

// Gets the entity ID of the referenced entity via the entity reference field.
$referenced_entity_id = $my_entity_wrapper->field_my_entity_reference_field->id->value();

// You can create another wrapper for the referenced entity if that's more convenient.
$referenced_entity_wrapper = entity_metadata_wrapper('my_referenced_entity_name', $referenced_entity_id);

/**
 * Field Collection Data.
 * Example of getting data from a multi-instance field collection field on a node type.
 *
 * This code came from a cTools plugin, but the techniques can be used in any module.
 */

   $node_wrapper = entity_metadata_wrapper('node', $node_id);

  try {
    // First we get data from 2 regular fields.
    $facts_title_small = $node_wrapper->field_facts_title_small->value();
    $facts_title_large = $node_wrapper->field_facts_title_large->value();

    // field_facts_statistic is a field collection that can have multiple instances on the node.
    $statistic_collection = $node_wrapper->field_facts_statistic->value();

    foreach ($statistic_collection as $id => $instance) {
      // We create a wrapper for each instance of the field collection.
      $collection_wrapper = entity_metadata_wrapper('field_collection_item', $instance);
      // Using the collection wrapper we can access the individual fields.
      $statistic_value = $collection_wrapper->field_facts_statistic_value->value();
      $statistic_desc = $collection_wrapper->field_facts_statistic_descriptio->value();
      $facts_items[] = array(
        'fact_stat' => $statistic_value,
        'fact_desc' => $statistic_desc,
      );
    }
  }
  catch (EntityMetadataWrapperException $e) {
    watchdog(__FUNCTION__, 'User Profile Exception: %t', array('%t' => $e->getMessage()));
  }

/**
 * EMW for a link field with access to URL and Title field items.
 */
  $node_wrapper = entity_metadata_wrapper('node', $nid);

  $link_url = check_url($node_wrapper->field_cta_link->url->value());
  $link_title = check_plain($node_wrapper->field_cta_link->title->value());

/**
 * EMW for an image field.
 */
  $node_wrapper = entity_metadata_wrapper('node', $nid);
  // Accessing the field value returns an array of the image field items.
  $cta_image_array = $node_wrapper->field_cta_hero_image->value();

  // The image url is not available, only the uri which starts with //public/.
  $image_uri = $cta_image_array['uri'];
  // Use file_create_url to convert the uri to a usable url.
  $image_url = file_create_url($image_uri);

  // Getting the alt text field item.
  $image_alt = check_plain($cta_image_array['alt']);

/**
 * EMW for a text area field like a Body field.
 */
  $node_wrapper = entity_metadata_wrapper('node', $my_node);
  // Regular value() method returns an array of value, safe_value, etc.
  $long_msg = $node_wrapper->field_long_msg->value();

  // Returns filtered value.
  $long_msg = $node_wrapper->field_long_msg->value->value();

  // Unfiltered value.
  $long_msg = $node_wrapper->field_long_msg->value->raw();

  // The selected text format.
  $long_msg = $node_wrapper->field_long_msg->format->value();
