(function ($) {
    $(function () {

        // Add custom ajax throbber.
        $(document).bind("ajaxStart", function () {
            $('#t3').show();
            $('#t4').show();
            $('#block-booking-calendar-booking-calendar-widget').css('pointer-events', 'none');
            $('#booking-courts-wrapper').css('pointer-events', 'none');

        }).bind("ajaxStop", function () {
            $('#t3').hide();
            $('#t4').hide();
            $('#block-booking-calendar-booking-calendar-widget').css('pointer-events', 'auto');
            $('#booking-courts-wrapper').css('pointer-events', 'auto');
        });


        /**************** CALENDAR WIDGET AJAX SELECT *****************/
        $(document).on('click', '#block-booking-calendar-booking-calendar-widget tbody td', function (event) {
            if ($(this).hasClass('av') || $(this).hasClass('na') || $(this).hasClass('opt')) {
                var year = $('#controls .prev').attr('data-year');
                var month = $('#controls .prev').attr('data-month');
                var day = $(this).text();
                $('#block-booking-calendar-booking-calendar-widget tbody td').not(this).removeClass('selected-user');
                $(this).addClass('selected-user');
                ajaxWidgetCalendarSelect(year, month, day);
            }
        });


        $(document).on('click', '#booking-courts-wrapper tbody td.value', function (event) {
            var year = $(this).attr('year');
            var month = $(this).attr('month');
            var day = $(this).attr('day');
            var hour = $(this).attr('hour');
            var name = $(this).attr('name');
            var price = $(this).text();


            var tableClient = {};
            tableClient.year = year;
            tableClient.month = month;
            tableClient.day = day;
            tableClient.hour = hour;
            tableClient.name = name;
            tableClient.price = price;

            tableClient = JSON.stringify(tableClient);

            $(this).toggleClass('selected-user');
            var flag = '';
            if ($(this).hasClass('selected-user')) {
                flag = 'select';
            }
            else {
                flag = 'unselect';
            }
            console.log(tableClient);
            console.log(tableClient);

            ajaxWidgetCalendarHourBook(tableClient, flag);
        });


    });
})(jQuery);


function ajaxWidgetCalendarSelect(year, month, day) {
    var $ = jQuery;
    var postString = 'day=' + day + '&month_number=' + month + '&year=' + year;
    var ajaxPath = '/booking/calendar/widget/calendar/select/ajax'; // This is the AjAX URL set by the custom Module.
    $.ajax({
        url: ajaxPath,
        method: "POST",
        data: postString,
        success: function (data) {
            console.log(data);
            $('#booking-courts-wrapper').html(data['hours_table']);
        }
    });
}


function ajaxWidgetCalendarHourBook(tableClient, flag) {

    var $ = jQuery;

    var postString = 'table_client=' + tableClient + '&flag=' + flag;
    var ajaxPath = '/booking/calendar/widget/calendar/table/book/ajax'; // This is the AjAX URL set by the custom Module.
    $.ajax({
        url: ajaxPath,
        method: "POST",
        data: postString,
        success: function (data) {
            // $('#booking-courts-wrapper').html(data['hours_table']);
        }
    });
}