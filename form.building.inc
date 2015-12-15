<?php
/**
 * Builds forms for creating or editing emails.
 *
 * @param string $type
 *   Indicates the type of form to build: "create" or "edit".
 * @param string $sender
 *   Indicates which page this form was called from.
 *   Values are: 'contacts' or 'customers'.
 * @param array $cust_data
 *   An associative array containing:
 *   - All data for the parent customer (or contact) with
 *     column names as keys and the column values as values.
 * @param array $email_data
 *   An associative array containing:
 *   - All data for an existing email with column names as keys
 *     and the column values as values.
 *
 * @return array
 *   A form array
 */
function freebil_build_email_form($type, $sender, array $cust_data, $email_data = array()) {

  $form['sender'] = array(
    '#type' => 'hidden',
    '#value' => $sender,
  );

  if ($type === "edit") {
    $form['email_id_display'] = array(
      '#markup' => t('<h5>Email ID: @email_id</h5>', array('@email_id' => $email_data['email_id'])),
    );
  }

  $cust_name = '';
  if (array_key_exists("cust_name", $email_data)) {
    $cust_name = $email_data['cust_name'];
  }
  elseif (array_key_exists("cust_name", $cust_data)) {
    $cust_name = $cust_data['cust_name'];
  }

  if ($sender == 'customers') {
    $sender_display = 'Customer';
  }
  else {
    $sender_display = 'Contact';
  }
  $form['customer_name_display'] = array(
    '#markup' => '<h5>' . $sender_display . ': ' . $cust_name . '</h5>',
  );

  $cust_id = '';
  if (array_key_exists("cust_id", $email_data)) {
    $cust_id = $email_data['cust_id'];
  }
  elseif (array_key_exists("cust_id", $cust_data)) {
    $cust_id = $cust_data['cust_id'];
  }
  $form['cust_id'] = array(
    '#type' => 'hidden',
    '#value' => $cust_id,
  );

  $form['email_addr'] = array(
    '#type' => 'textfield',
    '#title' => t('Email Address'),
    '#maxlength' => 25,
    '#wysiwyg' => FALSE,
    '#description' => t('Add the full email address here.'),
  );
  $form['email_type'] = array(
    '#type' => 'select',
    '#title' => t('Email Type'),
    '#options' => array('Work' => t('Work'), 'Personal' => t('Personal')),
    '#default_value' => 'Work',
  );
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Email Description'),
    '#maxlength' => 255,
    '#wysiwyg' => FALSE,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  if ($type === "create") {
    $form['actions']['cancel'] = array(
      '#markup' => l(t('Cancel'), 'admin/freebil/' . $sender . '/' . $cust_id . '/view'),
    );
  }
  else {
    $form['actions']['cancel'] = array(
      '#markup' => l(t('Cancel'), 'admin/freebil/' . $sender . '/' . $cust_id . '/view'),
    );
  }

  // If this is the edit form, populate the values from the email table.
  if ($type === "edit") {
    // Need the email ID to update the table row on form submit.
    $form['email_id'] = array(
      '#type' => 'hidden',
      '#value' => $email_data['email_id'],
    );
    $form['email_addr']['#default_value'] = $email_data['email_addr'];
    $form['email_type']['#default_value'] = $email_data['email_type'];
    $form['description']['#default_value'] = $email_data['description'];
  }

  return $form;
}
