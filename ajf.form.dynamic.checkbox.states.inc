<?php
/**
 * Uses form states API to conditionally display form fields.
 *
 * Uses a checkbox field to determine which conditional fields to display.
 */
 
  // Conditionaly display quantity, units, or billed amount fields.
  $form['entry_type'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display Quantity and Unit Cost fields.'),
    '#default_value' => FALSE,
  );
  $form['quantity'] = array(
    '#type' => 'textfield',
    '#title' => t('Quantity'),
    '#default_value' => '1',
    '#maxlength' => 9,
    '#element_validate' => array('element_validate_number'),
    '#description' => t('Enter the number of units sold.'),
    '#states' => array(
      'visible' => array(
        ':input[name="entry_type"]' => array('checked' => TRUE)
      ),
    ),
  );
  $form['unit_cost_amt'] = array(
    '#type' => 'textfield',
    '#title' => t('Unit Cost'),
    '#default_value' => '0.00',
    '#maxlength' => 12,
    '#element_validate' => array('element_validate_number'),
    '#description' => t('Enter the cost for one unit.'),
    '#states' => array(
      'visible' => array(
        ':input[name="entry_type"]' => array('checked' => TRUE)
      ),
    ),
  );
  $form['billed_amt'] = array(
    '#type' => 'textfield',
    '#title' => t('Billed Amount'),
    '#default_value' => '0.00',
    '#maxlength' => 12,
    '#element_validate' => array('element_validate_number'),
    '#description' => t('Enter the total amount billed for this item.'),
    '#states' => array(
      'visible' => array(
        ':input[name="entry_type"]' => array('checked' => FALSE)
      ),
    ),
  );
  // End conditional section.
