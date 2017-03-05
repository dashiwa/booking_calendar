<?php

/**
 * @file
 */


/**
 * Ajax callback.
 */
function booking_calendar_month_ajax() {
  if (count($_POST)) {
    // We calculate the number of days in the current month.
    $dayofmonth = date('t');
    // Counter for the days of the month.
    $day_count = 1;

    isset($_POST['day']) ? $day = $_POST['day'] : $day = date('j');

    isset($_POST['month_number']) ? $month = $_POST['month_number'] : $month = date('n');

    isset($_POST['year']) ? $year = $_POST['year'] : $year = date('Y');

    isset($_POST['table']) ? $table = json_decode($_POST['table']) : $table = array();

    $table = (array) $table;


    // Save values in session
    $_SESSION['tables'][$year][$month][$day] = $table;
    $booking_tables = $_SESSION['tables'];

    // If session is expired save value in database
    variable_set('booking_calendar_book_table', $booking_tables);

    // 1. The first week.
    $num = 0;
    for ($i = 0; $i < 7; $i++) {
      // Calculate the number of days of the week for the number of.
      $dayofweek = date('w', mktime(0, 0, 0, $month, $day_count, $year));

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

    $output = theme('booking_calendar_admin', array(
      'week' => $week,
      'month' => $month,
      'year' => $year,
    ));
    $response = array(
      'output' => $output,
    );

    return drupal_json_output($response);
  }
  return drupal_not_found();
}