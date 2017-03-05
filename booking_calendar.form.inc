<?php

/**
 * @file
 */

function booking_calendar_courts_form($form, &$form_state) {

  $courts = _default_hours_and_name_courts();

  if (!empty(variable_get('booking_calendar_names_courts', ''))) {
    $courts = variable_get('booking_calendar_names_courts', '');
  }


  $form['courts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Courts settings for ') . '<strong>' . date('Y F j') . '</strong>',
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

  $form['courts']['hour_begin'] = array(
    '#type' => 'textfield',
    '#title' => t('Hours begin'),
    '#default_value' => $hours_settings['hour_begin'],
    '#size' => 60,
    '#maxlength' => 128,
  );
  $form['courts']['hour_end'] = array(
    '#type' => 'textfield',
    '#title' => t('Hours begin'),
    '#default_value' => $hours_settings['hour_end'],
    '#size' => 60,
    '#maxlength' => 128,
  );

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
      $hour_begin = 22;
    }
    else {
      $hour_begin = $hours_settings['hour_begin'];
      $hour_end = $hours_settings['hour_end'];
    }

    //count = (($hour_end - $hound_begin)*interval for hours)+2;
    $count_booking_hours = (($hour_end - $hour_begin) * 2) + 2;

    for ($i = 0; $i < $count_booking_hours; $i++) {
      $hours[$i] = date('H : i', mktime($hour_begin, $minutes, 0, date('n'), date('j'), date('Y')));
      $minutes += $interval;
    }
    //dsm($court);
    foreach ($hours as $hour) {
      $hour_key = str_replace(' : ', '_', $hour);


      !empty(variable_get('booking_calendar_names_courts', '')) ? $default_value = $court['hours'][$hour_key]['price'] : $default_value = $court['hours'][$hour]['price'];

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
    '#value' => t('Click Here!'),
  );

  return $form;
}

function booking_calendar_courts_form_submit($form, &$form_state) {


  if (empty(variable_get('booking_calendar_names_courts', ''))) {
    $courts_default = _default_hours_and_name_courts();
  }
  else {
    $courts_default = variable_get('booking_calendar_names_courts', '');
  }

  $hours = array();
  $minutes = 0;
  $interval = 30;

  $hours_settings = variable_get('booking_hours_settings', '');
  if (empty($hours_settings)) {
    $hour_begin = 8;
    $hour_begin = 22;
  }
  else {
    $hour_begin = $hours_settings['hour_begin'];
    $hour_end = $hours_settings['hour_end'];
  }

  //count = (($hour_end - $hound_begin)*interval for hours)+2;
  $count_booking_hours = (($hour_end - $hour_begin) * 2) + 2;

  for ($i = 0; $i < $count_booking_hours; $i++) {
    $hours[$i] = date('H : i', mktime($hour_begin, $minutes, 0, date('n'), date('j'), date('Y')));
    $minutes += $interval;
  }


  $courts_final = array();

  foreach ($courts_default as $court_default) {
    //dsm($court_default['hours']);
    foreach ($court_default['hours'] as $hour) {


      $hour_key = str_replace(' : ', '_', $hour['hour']);

      $hours_prices_new_court[$hour_key] = array(
        'hour' => $hour['hour'],
        'price' => 0,
      );

      $hours_prices[$hour_key] = array(
        'hour' => $hour['hour'],
        'price' => $form_state['values'][$court_default['name'] . '_' . $hour_key],
      );
      $courts_final[$court_default['name']] = array(
        'name' => $form_state['values']['name-' . $court_default['name']],
        'hours' => $hours_prices,
      );
    }
  }


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


  if (!isset($form_state['values']['hour_begin']) || !isset($form_state['values']['hour_end'])) {
    $hour_begin = 8;
    $hour_end = 22;
  }
  else {
    $hour_begin = $form_state['values']['hour_begin'];
    $hour_end = $form_state['values']['hour_end'];
  }

  //count = (($hour_end - $hound_begin)*interval for hours)+2;
  $count_booking_hours = (($hour_end - $hour_begin) * 2) + 2;

  $hours_settings = array(
    'hour_begin' => $hour_begin,
    'hour_end' => $hour_end,
    'hour_count' => $count_booking_hours,
  );

  variable_set('booking_hours_settings', $hours_settings);
  variable_set('booking_calendar_names_courts', $courts_final);


}