<?php

/**
 * @file
 * Admin functions.
 */


/**
 * Defines form callback for the admin/config/availability-calendar/settings
 * page.
 */
function booking_calendar_admin_settings() {

  drupal_add_js(drupal_get_path('module', 'bookinf_calendar') . '/js/booking-calendar.js');
  drupal_add_css(drupal_get_path('module', 'bookinf_calendar') . '/css/booking-calendar.css');

  $output = '';
  $output .= '<div id="t3"></div>';
  $output .= '<div class="flags">
                <ul>
                    <li><span class="av">' . t('Available') . '</span></li>
                    <li><span class="na">' . t('Fully booked') . '</span></li>
                    <li><span class="opt">' . t('Provisionally booked') . '</span></li>
                </ul>
            </div>';

  $output .= '<div id="calendar-wrapper">';
  $output .= theme('booking_calendar', array('week' => _booking_calendar_calculate()));
  $output .= '</div>';

  return $output;
}

/**
 * Implements hook_theme().
 */
function booking_calendar_theme($existing, $type, $theme, $path) {
  return array(
    'booking_calendar' => array(
      'variables' => array(
        'week' => NULL,
        'month' => NULL,
        'year' => NULL,
      ),
    ),
  );
}


/**
 *
 * Themes the availability calendar field.
 *
 */
function theme_booking_calendar($variables) {

  isset($variables['week']) ? $week = $variables['week'] : $week = _calendar_calculate();;
  isset($variables['month']) ? $month = $variables['month'] : $month = date('n');
  isset($variables['year']) ? $year = $variables['year'] : $year = date('Y');

  // 3. Print the contents of the array $ week
  // A calendar
  // Display the table.
  $output = '<div class="year">' . $year . '</div>
             <div id="controls">
                <div data-year="' . $year . '" data-month="' . $month . '" class="prev">' . t('PREV') . '</div><div class="month">' . date('F', mktime(0, 0, 0, $month, date("j"), date('Y'))) . '</div><div  data-year="' . $year . '" data-month="' . $month . '" class="next">' . t('NEXT') . '</div></div>';
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