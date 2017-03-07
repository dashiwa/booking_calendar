<?php

/**
 * @file
 */


/**
 * @return mixed
 * Calculate calendar arrays.
 */
function _booking_calendar_calculate() {

  // We calculate the number of days in the current month.
  $dayofmonth = date('t');
  // Counter for the days of the month.
  $day_count = 1;

  $month = date('n');

  $week = array();

  // 1. The first week.
  $num = 0;
  for ($i = 0; $i < 7; $i++) {
    // Calculate the number of days of the week for the number of.
    $dayofweek = date('w', mktime(0, 0, 0, $month, $day_count, date('Y')));

    // Leads to a number of the format 1 - Monday, ..., 6 - Saturday.
    $dayofweek = $dayofweek - 1;
    if ($dayofweek == -1) {
      $dayofweek = 6;
    }

    if ($dayofweek == $i) {
      // If the same days of the week,
      // Fill the array $week
      // Number of months.
      $week[$num][$i] = $day_count;
      $day_count++;
    }
    else {
      $week[$num][$i] = "";
    }
  }

  // 2. The following week of the month.
  while (TRUE) {
    $num++;
    for ($i = 0; $i < 7; $i++) {
      $week[$num][$i] = $day_count;
      $day_count++;
      // If you have reached the end of the month - leave
      // The loop.
      if ($day_count > $dayofmonth) {
        break;
      }
    }
    // If you have reached the end of the month - leave
    // From the cycle.
    if ($day_count > $dayofmonth) {
      break;
    }
  }
  return $week;
}

/**
 * @return array
 * Default values for table hours
 */
function _default_hours_and_name_courts() {

  $hours = array();

  $minutes = 0;
  $interval = 30;

  $hours_settings = variable_get('booking_hours_settings', '');

  if (empty($hours_settings)) {
    $hour_end = 22;
    $hour_begin = 8;
  }
  else {
    $hour_end = $hours_settings['hour_end'];
    $hour_begin = $hours_settings['hour_begin'];
  }


  //count = (($hour_end - $hound_begin)*interval for hours)+2;
  $count = (($hour_end - $hour_begin) * 2) + 2;


  for ($i = 0; $i < $count; $i++) {
    $hours[$i] = date('H : i', mktime($hour_begin, $minutes, 0, date('n'), date('j'), date('Y')));
    $minutes += $interval;
  }

  // Define default values.
  $courts = array();

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

  $courts_final = array();

  foreach ($courts as $court) {
    foreach ($hours as $hour) {
      $hours_prices[$hour] = array(
        'hour' => $hour,
        'price' => 0,
      );
      $courts_final[$court['name']] = array(
        'name' => $court['name'],
        'hours' => $hours_prices,
      );
    }
  }
  return $courts_final;
}


/**
 * @param $value
 * @return mixed
 * Custom function for create valid form-state keys for drupal form submit function.
 */
function _form_valid_keys($value) {
  return (str_replace(' : ', '_', $value));
}

/**
 * @param $value
 * @return string
 * Conver arabic number values to roman.
 */
function _arabic_to_roman($value) {
  if ($value < 0) {
    $value = -$value;
  }
  if (!$value) {
    return "0";
  }
  $thousands = (int) ($value / 1000);
  $value -= $thousands * 1000;
  $result = str_repeat("M", $thousands);
  $table = array(
    900 => "CM",
    500 => "D",
    400 => "CD",
    100 => "C",
    90 => "XC",
    50 => "L",
    40 => "XL",
    10 => "X",
    9 => "IX",
    5 => "V",
    4 => "IV",
    1 => "I"
  );
  while ($value) {
    foreach ($table as $part => $fragment) {
      if ($part <= $value) {
        break;
      }
    }
    $amount = (int) ($value / $part);
    $value -= $part * $amount;
    $result .= str_repeat($fragment, $amount);
  }
  return $result;
}