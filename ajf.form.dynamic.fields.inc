<?php

/**
 * Code to add additional fields to a form dynamically as needed. The user clicks a button and another instance of the field(s) is added.
 *
 * This is based on code shared by Sam in his blog article:
 * http://rapiddg.com/blog/use-ajax-support-multiple-item-values-custom-drupal-7-forms
 *
 * I have adapted his example to my own needs.
 */
 
function ajf_customer_create_form($form, &$form_state) {
  // Set the page breadcrumb
  $crumbs = array(
    l(t('Home'), '<front>'),
    l(t('Administration'), 'admin'),
    l(t('ajf'), 'admin/ajf'),
    l(t('Customers'), 'admin/ajf/customers'),
  );
  drupal_set_breadcrumb($crumbs);
  
  return ajf_build_form($form, $form_state, "create");
}

function ajf_customer_create_form_submit($form, &$form_state) {
  
  $custRow = array(
  'cust_name' => $form_state['values']['customer-name'],
  'cust_type' => $form_state['values']['customer-type'],
  'addr_line1' => $form_state['values']['addr-line1'],
  'addr_line2' => $form_state['values']['addr-line2'],
  'addr_line3' => $form_state['values']['addr-line3'],
  'addr_line4' => $form_state['values']['addr-line4'],
  'zipcode' => $form_state['values']['zipcode'],
  'city' => $form_state['values']['city'],
  'state' => $form_state['values']['state'],
  'country' => $form_state['values']['country'],
  'phone1' => $form_state['values']['phone1'],
  'phone1_type' => $form_state['values']['phone1-type'],
  'phone2' => $form_state['values']['phone2'],
  'phone2_type' => $form_state['values']['phone2-type'],
  'phone3' => $form_state['values']['phone3'],
  'phone3_type' => $form_state['values']['phone3-type'],
  'description' => $form_state['values']['description'],
  );
  
  $custId = ajf_customer_insert($custRow); //function definition omitted in this snippet.
  
  for ($i = 1; $i <= $form_state['storage']['email_count']; $i++) {
    if (isset($form_state['values']['email'][$i]) && (!empty($form_state['values']['email'][$i]['email_addr']))) {
      $emailRow = array(
        'cust_id' => $custId,
        'email_addr' => $form_state['values']['email'][$i]['email_addr'],
        'email_type' => $form_state['values']['email'][$i]['email_type'],
        'default_email' => $form_state['values']['email'][$i]['default_email'],
        'description' => $form_state['values']['email'][$i]['email_desc'],
      );
      $emailId = ajf_email_insert($emailRow); //function definition omitted in this snippet.
    }
  }
  
  $form_state['redirect'] = 'admin/ajf/customers';   
  drupal_set_message("Successfully saved Customer."); 
}


function ajf_build_form($form, &$form_state, $type, $custData = array()) {
  // If the form state storage field for the email address count is not set, then set it to zero.
  $form_state['storage']['email_count'] =
    isset($form_state['storage']['email_count']) ? $form_state['storage']['email_count'] : 0;

  //dpm($form_state); 
  
  // *ajf - many form fields for creating a customer are omitted here, as this is only to highlight the dynamic email fields.
  
  //Additional instances of the email form fields can be added by the user - as many as needed.
  $form['email'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
    '#prefix' => '<div id="email">',
    '#suffix' => '</div>',
  );
  
  if ($form_state['storage']['email_count']) {
    for ($i = 1; $i <= $form_state['storage']['email_count']; $i++) {
      $form['email'][$i] = array(
        '#type' => 'fieldset',
        '#tree' => TRUE,
      );
  
      $form['email'][$i]['email_addr'] = array(
        '#title' => t('Email Address'),
        '#type' => 'textfield',
        '#description' => 'Enter an email address for this customer.',
      );
      $form['email'][$i]['email_type'] = array(
        '#type' => 'select',
        '#title' => t('Email Type'),
        '#options' => array('Work' => 'Work', 'Personal' => 'Personal'),
        '#default_value' => 'Work',
      );      
      $form['email'][$i]['default_email'] = array(
        '#title' => t('Make Default'),
        '#type' => 'radios',
        '#default_value' => 'N',
        '#options' => array('Y' => 'Yes', 'N' => 'No'),
        '#description' => t('Choose Yes if you want this email to be chosen by default when sending invoices.'),
      );
      $form['email'][$i]['email_desc'] = array(
        '#type' => 'textarea',
        '#title' => t('Email Description'),
        '#maxlength' => 255,
        '#wysiwyg' => false,
      );
    }
  }

  $form['add_email'] = array(
    '#type' => 'button',
    '#name' => 'add_email',
    '#value' => t('Add an Email Address'),
    '#href' => '',
    '#ajax' => array(
      'callback' => 'ajf_ajax_add_email',
      'wrapper' => 'email',
     ),
   );
  
  $form['actions'] = array('#type' => 'actions');  // wrapper for a group of buttons
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  if ($type === "edit") {
    $form['actions']['cancel'] = array(
      '#markup' => l(t('Cancel'), 'admin/ajf/customers' .  $custData['cust_id'] . '/view'),
    );
  } else {
    $form['actions']['cancel'] = array(
      '#markup' => l(t('Cancel'), 'admin/ajf/customers'),
    );
  }
  
  // when the form is rendered, check if we added an email. If so, increment the counter.
  if (isset($form_state['triggering_element'])) {
    if ($form_state['triggering_element']['#name'] == 'add_email') {
      $form_state['storage']['email_count']++;
    }
  }else{
    $form_state['storage']['email_count']++; // the first time the user clicks the email button, 'triggering_element' isn't set, so we need this.
  }
  
  return $form; 
}

function ajf_ajax_add_email($form, $form_state) {
  return $form['email'];
}
