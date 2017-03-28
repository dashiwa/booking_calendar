(function ($) {
    $(function () {

        // Add custom ajax throbber.
        $(document).bind("ajaxStart", function () {
            $('#t3').show();
            $('#booking-calendar').css('pointer-events', 'none');

        }).bind("ajaxStop", function () {
            $('#t3').hide();
            $('#booking-calendar').css('pointer-events', 'auto');
        });

        // Change booking flag
        $(document).on('click', '.flags ul li', function (event) {
            $('.flags ul li').removeClass('this');
            $(this).addClass('this');
            $(this).attr('active');
        });

        // Change booking date
        $(document).on('click', '#booking-calendar td', function (event) {
            var chan = $('.flags ul li.this span').attr('class');
            var this_select = 'select';
            if (typeof chan != 'undefined') {

                // Remove old click class.
                $(this).removeClass(this_select);

                // Add new click class
                $(this).addClass(chan);
                $(this).addClass(this_select);

                var year = $('#controls .prev').attr('data-year');
                var month = $('#controls .prev').attr('data-month');
                var day = $(this).text();

                var week_i = $(this).attr('week_i');
                var week_j = $(this).attr('week_j');

                // No push empty cell
                if (typeof week_i != 'undefined' || typeof week_j != 'undefined') {
                    var table = {};

                    table.year = year;
                    table.month = month;
                    table.week_i = week_i;
                    table.week_j = week_j;
                    table.day = day;
                    table.chan = chan;


                    var currentSelect = {};
                    currentSelect.year = year;
                    currentSelect.month = month;
                    currentSelect.week_i = week_i;
                    currentSelect.week_j = week_j;
                    currentSelect.day = day;

                    var flag = '';
                    console.log(table);
                    table = JSON.stringify(table);
                    currentSelect = JSON.stringify(currentSelect);
                    ajaxCalendarAdminBookSettings($(this), flag, table, day, currentSelect);
                }

            }

        });
    });
})(jQuery);

function ajaxCalendarAdminBookSettings(jqueryThis, flag, table, day, currentSelect) {
    var $ = jQuery;

    var postString = 'table=' + table + '&current_select=' + currentSelect;
    var ajaxPath = '/booking/calendar/admin/settings/ajax'; // This is the AjAX URL set by the custom Module.
    $.ajax({
        url: ajaxPath,
        method: "POST",
        data: postString,
        success: function (data) {
            $('#calendar-wrapper').html(data['output']);
            $('#hours-table-wrapper').html(data['hours_table'])
        }
    });
}


/*********************************** CONTROLS WORKING !!!!!!!!!!!!!! *********************/


/***************************** CONTROLS - NEXT AND PREV FOR CALENDAR ********************/
(function ($) {
    $(function () {
        // Add custom ajax throbber.
        $(document).bind("ajaxStart", function () {
            $('#t3').show();
            $('#booking-calendar').css('pointer-events', 'none');

        }).bind("ajaxStop", function () {
            $('#t3').hide();
            $('#booking-calendar').css('pointer-events', 'auto');
        });


        // Months controls
        $(document).on('click', '#controls .prev', function (event) {
            var flag = $(this).attr('class');
            ajaxCalendarControls($(this), flag);
        });
        // NEXT MONTH.
        $(document).on('click', '#controls .next', function (event) {
            var flag = $(this).attr('class');
            ajaxCalendarControls($(this), flag);
        });
    });
})(jQuery);

function ajaxCalendarControls(jqueryThis, flag) {

    var $ = jQuery;

    var month_number = jqueryThis.attr('data-month');
    if (typeof month_number == 'undefined') {
        month_number = $('#controls .prev').attr('data-month');
    }

    var year = jqueryThis.attr('data-year');
    if (typeof year == 'undefined') {
        year = $('#controls .prev').attr('data-year');
    }

    if (flag == 'prev') {
        month_number = parseInt(month_number);
        month_number--;

        if (month_number == 0 || (month_number < 0)) {
            month_number = 12;
            year--;
        }

        if (month_number > 12)
            month_number = 1;
    }


    if (flag == 'next') {

        month_number = parseInt(month_number);
        month_number++;

        if (month_number == 0 || (month_number < 0)) {
            month_number = 12;
        }

        if (month_number > 12) {
            month_number = 1;
            year++;
        }
    }


    var postString = 'month_number=' + month_number + '&year=' + year;
    var ajaxPath = '/booking/calendar/select/month/ajax'; // This is the AjAX URL set by the custom Module.
    $.ajax({
        url: ajaxPath,
        method: "POST",
        data: postString,
        success: function (data) {
            $('#calendar-wrapper').html(data['output']);
        }
    });
}

/***************************** END CONTROLS - NEXT AND PREV FOR CALENDAR ********************/











