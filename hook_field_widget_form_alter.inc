<?php

/**
 * @file
 * Example of using hook_field_widget_form_alter().
 *
 * This example shows a very specific use case, so take only what you need.
 * It's purpose is to add custom validation when users enter values in an
 * autocomplete, entity reference field that can have multiple values.
 * It checks to make sure the chosen entity is not being used by any other entity.
 */

/**
 * Implements hook_field_widget_form_alter().
 */
function mymodule_field_widget_form_alter(&$element, &$form_state, $context) {
  if (arg(0) != 'admin' && $context['field']['type'] == 'entityreference') {
    if (isset($element['target_id'])) {
      $element['target_id']['#element_validate'][] = '_mymodule_er_field_validate';
    }
    else {
      $element['#element_validate'][] = '_mymodule_er_field_validate';
    }
  }
}

/**
 * Entity reference field widget form validator.
 *
 * 'Module' here is the name of a custom a content type (not referring to ta custom module!).
 * 'Toolkit' is a content type that has an entity reference field, field_modules,
 * which references nodes of the Module content type.
 */
function _mymodule_er_field_validate($element, &$form_state, $form) {

  if ($element['#entity_type'] == 'node' && $element['#bundle'] == 'toolkit' && $element['#field_name'] == 'field_modules') {
    $valid = TRUE;
    $invalid_modules = [];

    // Is the Modules form field populated on the Toolkit edit form?
    if (isset($form_state['values']['field_modules']) && !empty($form_state['values']['field_modules'])) {

      // What modules are already included in this Toolkit?
      try {
        $node_wrapper = entity_metadata_wrapper('node', $element['#entity']);
        $toolkit_modules = $node_wrapper->field_modules->value();
      }
      catch (EntityMetadataWrapperException $e) {
        watchdog(__FUNCTION__, 'Node wrapper error: %t', array('%t' => $e->getMessage()));
      }

      $toolkit_target_ids = [];
      foreach ($toolkit_modules as $module) {
        $toolkit_target_ids[$module->nid] = $module->title;
      }

      // What modules are currently populated in the Toolkit edit form?
      foreach ($form_state['values']['field_modules']['und'] as $form_module_key => $form_module) {
        if (($form_module_key === 0) || ($form_module_key != 'add_more')) {
          $target_id_raw = $form_module['target_id'];

          // We only want the target_id number, not other strings.
          $target_id = preg_replace('/[^0-9]/', '', $target_id_raw);

          // Is this module new to this toolkit?
          if (!array_key_exists($target_id, $toolkit_target_ids)) {

            // Is the new module already in use with other toolkits?
            if (mymodule_module_in_use($target_id)) {
              $valid = FALSE;
              $invalid_modules[$target_id] = $target_id_raw;
            }
          }
        }
      }
    }
    // Set the error on the form element.
    if (!$valid) {
      $display_modules = '';
      foreach ($invalid_modules as $module_key => $module_value) {
        $display_modules .= $module_value . ' ';
      }
      form_error($form[$element['#field_name']], t('Selected modules are already in use on another Toolkit: %display_modules', array('%display_modules' => $display_modules)));
    }
  }
}

/**
 * Checks if toolkit module is used by any toolkit.
 *
 * 'Module' here is the name of a custom a content type (not referring to ta custom module!).
 *
 * @param string $module_nid
 *   - The node ID of the toolkit module.
 *
 * @return bool
 *   - True if the module is being used, else false.
 */
function mymodule_module_in_use($module_nid) {
  $module_in_use = FALSE;

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'toolkit')
    ->fieldCondition('field_modules', 'target_id', $module_nid);
  $result = $query->execute();
  if (!empty($result['node'])) {
    $module_in_use = TRUE;
  }

  return $module_in_use;
}

