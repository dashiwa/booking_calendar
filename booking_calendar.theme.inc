<?php

/**
 * @file
 */

/**
 * @param $variables
 * @return string
 */
function theme_booking_calendar_admin_table_day_hours($variables) {

  $output = '';

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

  $courts = _default_hours_and_name_courts();

  if (!empty(variable_get('booking_calendar_names_courts', ''))) {
    $courts = variable_get('booking_calendar_names_courts', '');
  }
  //Add fist column - time.
  array_unshift($courts, array('name' => t('TIME')));

  // Add admin setting form for table hours.
  $court_form = drupal_get_form('booking_calendar_courts_form');
  $courts_form_html = render($court_form);

  $output .= $courts_form_html;


  $output .= '<table id="booking-courts"><thead><tr>';
  foreach ($courts as $court) {
    if (!empty($court['name'])) {
      $output .= '<td>' . $court['name'] . '</td>';
    }
    else {
      $output .= '<td></td>';
    }
  }
  $output .= '</tr></thead>';
  foreach ($hours as $hour) {
    $output .= '<tr>';

    $a = 0;
    foreach ($courts as $court) {
      if ($a == 0) {
        $output .= '<td>' . $hour . '</td>';
      }
      else {
        if ($court['name']) {
          $hour_key = str_replace(' : ', '_', $hour);
          if (!empty(variable_get('booking_calendar_names_courts', ''))) {
            if (isset($court['hours'][$hour]['price'])) {
              $output .= '<td>' . $court['hours'][$hour]['price'] . '€' . '</td>';
            }
            else {
              $output .= '<td>' . 0 . '€' . '</td>';
            }

          }
          else {
            $output .= '<td>' . $court['hours'][$hour]['price'] . '</td>';
          }
        }
      }
      $a++;
    }
    '</tr>';
  }
  $output .= '</table>';

  return $output;
}


/**
 *
 */
function theme_booking_calendar_admin($variables) {

  isset($variables['week']) ? $week = $variables['week'] : $week = _calendar_calculate();;
  isset($variables['month']) ? $month = $variables['month'] : $month = date('n');
  isset($variables['year']) ? $year = $variables['year'] : $year = date('Y');

  $month_name = date('F', mktime(0, 0, 0, $month, date("j"), date('Y')));

  // Translate months.
  switch ($month_name) {
    case 'January' :
      $month_name = 'Sausis';
      break;
    case 'February' :
      $month_name = 'Vasaris';
      break;
    case 'March' :
      $month_name = 'Kovas';
      break;
    case 'April' :
      $month_name = 'Balandis';
      break;
    case 'May' :
      $month_name = 'Gegužė';
      break;
    case 'June' :
      $month_name = 'Birželis';
      break;
    case 'July' :
      $month_name = 'Liepa';
      break;
    case 'August' :
      $month_name = 'Rugpjūtis';
      break;
    case 'September' :
      $month_name = 'Rugsėjis';
      break;
    case 'October' :
      $month_name = 'Spalis';
      break;
    case 'November' :
      $month_name = 'Lapkritis';
      break;
    case 'December' :
      $month_name = 'Gruodis';
      break;
  }


  // 3. Print the contents of the array $ week
  // A calendar
  // Display the table.
  $output = '<div class="year">' . $year . '</div>
             <div id="controls">
                <div data-year="' . $year . '" data-month="' . $month . '" class="prev">' . t('Ankstesnis') . '</div><div class="month">' . $month_name . '</div><div  data-year="' . $year . '" data-month="' . $month . '" class="next">' . t('Kitas') . '</div></div>';
  $output .= '<table id="booking-calendar"><thead>';

  $output .= '<thead><tr>';
  for ($i = 6; $i < 13; $i++) {

    $output .= '<td>' . date('l', mktime(0, 0, 0, date('m'), $i, $year)) . '</td>';
  }

  $output .= '</tr></thead>';
  for ($i = 0; $i < count($week); $i++) {
    $output .= "<tr>";
    for ($j = 0; $j < 7; $j++) {
      if (!empty($week[$i][$j])) {

        // If you are dealing with a Saturday and Sunday
        // highlights them.

        if ($j == 5 || $j == 6) {
          $classes = 'weekends';
          if ($week[$i][$j] == date('j')) {
            $classes = 'this weekends';
          }
        }
        elseif ($week[$i][$j] == date('j')) {
          $classes = 'this';
        }
        else {
          $classes = '';
        }
        if (isset($_SESSION['tables'])) {
          foreach ($_SESSION['tables'][$year][$month] as $key => $value) {
            if (!empty($_SESSION['tables'][$year][$month][$key])) {
              if ($j == $_SESSION['tables'][$year][$month][$key]['week_j'] && $i == $_SESSION['tables'][$year][$month][$key]['week_i']) {
                $classes = $_SESSION['tables'][$year][$month][$key]['chan'];
                if (isset($_SESSION['tables'][$year][$month][$key]['this_select'])) {

                  $classes .= ' ' . $_SESSION['tables'][$year][$month][$key]['this_select'];
                  unset($_SESSION['tables'][$year][$month][$key]['this_select']);
                }

              }
            }
          }
        }
        else {
          $booking_table = variable_get('booking_calendar_book_table', '');
          if (!empty($booking_table)) {
            foreach ($booking_table[$year][$month] as $key => $value) {
              if (!empty($booking_table[$year][$month][$key])) {
                if ($j == $booking_table[$year][$month][$key]['week_j'] && $i == $booking_table[$year][$month][$key]['week_i']) {
                  $classes = $booking_table[$year][$month][$key]['chan'];

                }
              }
            }
          }

        }
        $output .= '<td week_i="' . $i . '" week_j = "' . $j . '" class="' . $classes . '">' . $week[$i][$j] . '</td>';
      }
      else {
        $output .= "<td>&nbsp;</td>";
      }
    }
    $output .= "</tr>";
  }
  $output .= "</table>";

  return $output;
}