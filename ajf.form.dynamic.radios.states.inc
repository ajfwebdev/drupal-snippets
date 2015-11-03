<?php
/**
 * Uses radios to determine which form fields to display to the user.
 *
 * This does a built-in ajax call based on the '#states' and 'visible' array elements
 * on the dynamic fields.
 */

  // Conditionaly display quantity and units fields.
  $form['entry_type'] = array(
    '#type' => 'radios',
    '#title' => t('Entry Type'),
    '#options' => drupal_map_assoc(
      array(
        t('Billed Amount'),
        t('Quantity'),
      )
    ),
    '#default_value' => 'Billed Amount',
    '#description' => t('Select Quantity if have multiple of the same item to charge for. Use Billed Amount if you only have one amount to charge.'),
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
        ':input[name="entry_type"]' => array('value' => t('Quantity'))
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
        ':input[name="entry_type"]' => array('value' => t('Quantity'))
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
        ':input[name="entry_type"]' => array('value' => t('Billed Amount'))
      ),
    ),
  );
  // End conditional section.