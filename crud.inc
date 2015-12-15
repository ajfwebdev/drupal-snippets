<?php

/**
 * The Drupal CRUD Operations
 *
 * This snippet show examples of using the Drupal Database API
 * to perform the Create, Read, Update, and Delete (or CRUD) operations.
 *
 * This code is from the Freebil Drupal module. It shows how the CRUD
 * operations are used to manage and access email address data.
 */
 
 /**
 * Email insert API.
 *
 * @param array $row
 *   An associative array containing:
 *   - The column names as keys and the column values as values for
 *     the email row to be inserted.
 *
 * @return object
 *   - InsertQuery object
 */
function freebil_email_insert(array $row) {
  $row['created_ts'] = REQUEST_TIME;
  $row['last_update_ts'] = REQUEST_TIME;
  $obj = db_insert('freebil_email')
    ->fields($row)->execute();

  return $obj;
}

 /**
 * Get emails for the customer and related contacts.
 *
 * @param string $cust_id
 *   The customer ID to get emails for.
 *
 * @return object
 *   A DatabaseStatementInterface prepared statement
 *   object, already executed.
 */
function freebil_cust_and_contact_emails_query($cust_id) {
  $query = 'SELECT
   c.cust_id
  , c.parent_cust_id
  , c.cust_name
  , e.email_id
  , e.email_addr
  , e.email_type
  , e.description
  , e.created_ts
  , e.last_update_ts
   FROM
   {freebil_customer} c
   INNER JOIN
   {freebil_email} e
   ON e.cust_id = c.cust_Id
   WHERE
   c.cust_id = :custid
   OR c.parent_cust_id = :custid
   ORDER BY c.cust_name, e.created_ts';
  return db_query($query, array(':custid' => $cust_id));
}

/**
 * Email update API.
 *
 * @param array $row
 *   An associative array containing:
 *   - The column names as keys and the column values as values for
 *     the email row to be updated.
 * @param string $email_id
 *   The email ID for the email to be updated.
 *
 * @return object
 *   An UpdateQuery object
 */
function freebil_email_update(array $row, $email_id) {
  $row['last_update_ts'] = REQUEST_TIME;
  $update_query_obj = db_update('freebil_email')
    ->fields($row)
    ->condition('email_id', $email_id, '=')
    ->execute();

  return $update_query_obj;
}

/**
 * Email delete API.
 *
 * @param string $email_id
 *   The email ID for the email to be deleted.
 *
 * @return object
 *   A DeleteQuery object
 */
function freebil_email_delete($email_id) {
  $delete_query_obj = db_delete('freebil_email')
    ->condition('email_id', $email_id)
    ->execute();
  return $delete_query_obj;
}
