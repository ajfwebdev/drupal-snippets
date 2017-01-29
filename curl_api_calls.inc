<?php
/**
 * Here are examples of using Curl to make calls to a remote API.
 * In these cases the security is Basic Auth, which is just 'Basic ' plus a token string.
 */

/**
 * Get some data (GET) from a remote service.
 *
 * @param string $data_id
 *   The ID of the entity to get data for.
 *
 * @returns
 *   An array of data.
 */
function my_module_get_data($data_id) {
  // Basic auth requires a token, which you have to get from the service you're calling.
  $token = get_the_token();
  $data = NULL;

  // Get the data.
  try {
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, my_module_remote_service_url() . "/api/some/remote_api/$data_id/data");
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_HTTPHEADER, array(
      'Accept: application/json',
      'Authorization: Basic ' . $token
    ));
    curl_setopt($ch, CURLOPT_CUSTOMREQUEST, 'GET');
    $result = curl_exec($ch);
    curl_close($ch);
  }
  catch (Exception $e) {
      watchdog('my_module', $e->getMessage(), array(), WATCHDOG_ERROR);
      return FALSE;
  }

  $decoded = drupal_json_decode($result);
  if (!empty($decoded['data'])) {
    $data = $decoded['data'];
  }

  return $data;
}


/**
 * Call to create something (POST) on the remote service.
 *
 * @param string $thing_name
 *   The name of the thing to be created.
 * @param string $tok
 *   The remote access token.
 *
 * @return array $messages
 *   An array of messages for optional logging.
 */
function my_module_create_remote_thing($thing_name, $tok) {
  $messages = array();

  $post_data_array = array(
    'thing_templates' => array(
      array(
      'title' => $thing_name,
      'thing_type' => 'remote',
      'other_data_1' => FALSE,
      'other_data_2' => 50
      )
    )
  );
  $post_data = json_encode($post_data_array);
  $messages[] = 'my_module_create_remote_thing, $post_data, ' . $post_data;

  $url = remote_url() . '/api/remote/thing_templates';
  $messages[] = 'my_module_create_remote_thing, $url, ' . $url;
  $messages[] = 'my_module_create_remote_thing, $tok, ' . $tok;

  $header_array = array(
    'Content-Type: application/json',
    'Accept: application/json',
    'Authorization: Basic ' . $tok,
  );

  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, $url);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
  curl_setopt($ch, CURLOPT_POST, 1);
  curl_setopt($ch, CURLOPT_POSTFIELDS, $post_data);
  curl_setopt($ch, CURLOPT_HTTPHEADER, $header_array);
  try {
    $result = curl_exec($ch);
    if ($result) {
      $messages[] = 'my_module_create_remote_thing, $result, ' . $result;
    }
    else {
      $messages[] = 'my_module_create_remote_thing, $result, FALSE';
    }
  }
  catch (Exception $e) {
    watchdog('my_module', $e->getMessage(), array(), WATCHDOG_ERROR);
    $messages[] = 'my_module_create_remote_thing, curl_exec Error, ' . $e->getMessage();
    return $messages;
  }
  curl_close($ch);

  return $messages;
}


/**
 * Deletes something (DELETE) on a remote service.
 *
 * @param string $remote_thing_id
 *   The remote thing ID.
 */
function my_module_remote_thing_delete($remote_thing_id) {
  $tok = remote_token();

  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, remote_url() . '/api/remote/thing_templates/' . $remote_thing_id);
  curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "DELETE");
  curl_setopt($ch, CURLOPT_POSTFIELDS, '');
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
  $header_array = array(
    'Content-Type: application/json',
    'Accept: application/json',
    'Authorization: Basic ' . $tok,
  );
  curl_setopt($ch, CURLOPT_HTTPHEADER, $header_array);
  try {
    $result = curl_exec($ch);
  }
  catch (Exception $e) {
    watchdog('my_module', $e->getMessage(), array(), WATCHDOG_ERROR);
    return FALSE;
  }
  $result = json_decode($result);
  curl_close($ch);

  return $result;
}

/**
 * Updates something (PATCH) on a remote service.
 *
 * @param string $remote_thing_id
 *   The remote thing ID.
 * @param array $thing_data
 *   An associative array containing:
 *   - Field keys and new values to update with.
 *
 * @return array
 *   The remote API call result data.
 */
function my_module_remote_thing_update($remote_thing_id, $thing_array) {
  $tok = remote_token();
  $patch_data = json_encode($thing_array);

  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, remote_url() . '/api/remote/things/' . $remote_thing_id);
  curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "PATCH");
  curl_setopt($ch, CURLOPT_POSTFIELDS, $patch_data);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
  $header_array = array(
    'Content-Type: application/json',
    'Accept: application/json',
    'Authorization: Basic ' . $tok,
  );
  curl_setopt($ch, CURLOPT_HTTPHEADER, $header_array);
  try {
    $result = curl_exec($ch);
  }
  catch (Exception $e) {
    watchdog('my_module', $e->getMessage(), array(), WATCHDOG_ERROR);
    return FALSE;
  }
  $result = json_decode($result);
  curl_close($ch);

  return $result;
}
