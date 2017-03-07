<?php

/**
 * @file
 */

function booking_calendar_courts_form($form, &$form_state) {


  if (!empty(variable_get('booking_calendar_names_courts', ''))) {
    $courts = variable_get('booking_calendar_names_courts', '');

    // Get data for current click state.
    if (!empty($_SESSION['current_select'])) {
      $current_state = $_SESSION['current_select'];
      $year = $current_state['year'];
      $month = $current_state['month'];
      $day = $current_state['day'];
      if (isset($_SESSION['table_hours'][$year]) && isset($_SESSION['table_hours'][$year][$month]) && isset($_SESSION['table_hours'][$year][$month][$day])) {
        $courts = $_SESSION['table_hours'][$year][$month][$day];
      }
    }
    else {
      $year = date('Y');
      $month = date('n');
      $day = date('j');
    }
  }
  else {
    $courts = _default_hours_and_name_courts();
    $year = date('Y');
    $month = date('n');
    $day = date('j');
  }


  $form['courts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Courts settings for ') . '<strong>' . $year . '/' . $month . '/' . $day . '</strong>',
    '#weight' => 5,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['courts']['names'] = array(
    '#type' => 'fieldset',
    '#title' => t('Courts names'),
    '#weight' => 2,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );


  $form['courts']['actions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Courts actions'),
    '#weight' => 3,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['courts']['actions']['delete'] = array(
    '#type' => 'fieldset',
    '#title' => t('Courts delete'),
    '#weight' => 3,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['courts']['actions']['add'] = array(
    '#type' => 'fieldset',
    '#title' => t('Courts add'),
    '#weight' => 3,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $hours_settings = variable_get('booking_hours_settings', '');
  if (!empty($hours_settings)) {
    $hour_begin = $hours_settings['hour_begin'];
    $hour_end = $hours_settings['hour_end'];
  }
  else {
    $hour_begin = 8;
    $hour_end = 22;
  }

  $form['courts']['hour_begin'] = array(
    '#type' => 'textfield',
    '#title' => t('Hour begin'),
    '#default_value' => $hour_begin,
    '#size' => 60,
    '#maxlength' => 128,
  );
  $form['courts']['hour_end'] = array(
    '#type' => 'textfield',
    '#title' => t('Hour end'),
    '#default_value' => $hour_end,
    '#size' => 60,
    '#maxlength' => 128,
  );

  if (!empty(variable_get('booking_calendar_names_courts', ''))) {
    $flag = TRUE;
  }

  foreach ($courts as $court) {
    $form['courts']['names']['name']['name-' . $court['name']] = array(
      '#type' => 'textfield',
      '#title' => t('Name court'),
      '#default_value' => $court['name'],
      '#size' => 60,
      '#maxlength' => 128,
    );

    $form['courts']['prices']['price']['price-' . $court['name']] = array(
      '#type' => 'fieldset',
      '#title' => t('Courts prices') . ' ' . $court['name'],
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );


    $hours = array();
    $minutes = 0;
    $interval = 30;

    $hours_settings = variable_get('booking_hours_settings', '');
    if (empty($hours_settings)) {
      $hour_begin = 8;
      $hour_end = 22;
    }
    else {
      $hour_begin = $hours_settings['hour_begin'];
      $hour_end = $hours_settings['hour_end'];
    }


    // Count = (($hour_end - $hound_begin)*interval for hours)+2.
    $count = (($hour_end - $hour_begin) * 2) + 2;

    for ($i = 0; $i < $count; $i++) {
      $hours[$i] = date('H : i', mktime($hour_begin, $minutes, 0, date('n'), date('j'), date('Y')));
      $minutes += $interval;
    }


    foreach ($hours as $hour) {
      $hour_key = str_replace(' : ', '_', $hour);

      if (isset($court['hours'][$hour]['price'])) {
        $default_value = $court['hours'][$hour]['price'];
      }
      else {
        $default_value = array();
      }


      $form['courts']['prices']['price']['price-' . $court['name']][$court['name'] . '_' . $hour_key] = array(
        '#type' => 'textfield',
        '#title' => t('Price court ') . $court['name'] . '  ' . $hour,
        '#default_value' => $default_value,
        '#size' => 60,
        '#maxlength' => 128,
      );

    }


    $form['courts']['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete court'),
      '#attributes' => array('class' => array('delete-form')),
    );
    $form['courts']['actions']['add'] = array(
      '#type' => 'submit',
      '#value' => t('Add court'),
      '#attributes' => array('class' => array('add-form')),
    );
    $form['courts']['actions']['add_name_court'] = array(
      '#type' => 'textfield',
      '#title' => t('Name new court'),
      '#default_value' => 'U4',
      '#size' => 25,
      '#maxlength' => 32,
      '#attributes' => array('class' => array('add-form-name')),
    );
  }


  $form['submit_button'] = array(
    '#type' => 'submit',
    '#value' => t('Save cofiguration'),
    '#weight' => 10,
  );

  return $form;
}

function booking_calendar_courts_form_submit($form, &$form_state) {


  $hours = array();
  $minutes = 0;
  $interval = 30;


  if (!isset($form_state['values']['hour_begin']) || !isset($form_state['values']['hour_end'])) {
    $hour_begin = 8;
    $hour_end = 22;
  }
  else {
    $hour_begin = $form_state['values']['hour_begin'];
    $hour_end = $form_state['values']['hour_end'];
  }

  //count = (($hour_end - $hound_begin)*interval for hours)+2;
  $count = (($hour_end - $hour_begin) * 2) + 2;

  $hours_settings = array(
    'hour_begin' => $hour_begin,
    'hour_end' => $hour_end,
    'hour_count' => $count,
  );

  for ($i = 0; $i < $count; $i++) {
    $hours[$i] = date('H : i', mktime($hour_begin, $minutes, 0, date('n'), date('j'), date('Y')));
    $minutes += $interval;
  }


  $courts = array(
    1 => array(
      'name' => 'U1',
      'price' => 0,
    ),
    2 => array(
      'name' => 'U2',
      'price' => 0,
    ),
    3 => array(
      'name' => 'U3',
      'price' => 0,
    ),
    4 => array(
      'name' => 'U4',
      'price' => 0,
    ),
  );

  if (!empty(variable_get('booking_calendar_names_courts', ''))) {
    $courts = variable_get('booking_calendar_names_courts', '');
  }
  //dsm($courts);

  $courts_final = array();
  foreach ($courts as $court) {
    foreach ($hours as $hour) {
      $hour_key = str_replace(' : ', '_', $hour);

      $hours_prices_new_court[$hour_key] = array(
        'hour' => $hour,
        'price' => 0,
      );
      if (isset($form_state['values'][$court['name'] . '_' . $hour_key])) {
        $hours_prices[$hour] = array(
          'hour' => $hour,
          'price' => $form_state['values'][$court['name'] . '_' . $hour_key],
        );
      }
      else {
        $hours_prices = array();
      }


      $courts_final[$court['name']] = array(
        'name' => $form_state['values']['name-' . $court['name']],
        'hours' => $hours_prices,
      );
    }
  }

  //dsm($form_state);
  // dsm($courts_final);
  if ($form_state['triggering_element']['#value'] == 'Add court') {
    $new_court[$form_state['values']['add_name_court']] = array(
      'name' => $form_state['values']['add_name_court'],
      'hours' => $hours_prices_new_court,
    );
    if (count($courts_final) < 5) {
      $courts_final = array_merge($courts_final, $new_court);
    }

  }

  if ($form_state['triggering_element']['#value'] == 'Delete court') {
    if (count($courts_final) > 1) {
      array_pop($courts_final);
    }
  }


  variable_set('booking_hours_settings', $hours_settings);
  variable_set('booking_calendar_names_courts', $courts_final);

  // Save for curren click state.
  if (!empty($_SESSION['current_select'])) {
    $current_state = $_SESSION['current_select'];
    $year = $current_state['year'];
    $month = $current_state['month'];
    $day = $current_state['day'];
    $_SESSION['table_hours'][$year][$month][$day] = $courts_final;
  }


}