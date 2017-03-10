(function ($) {
    $(function () {
        // Add custom ajax throbber.
        $(document).bind("ajaxStart", function () {
            $('#t3').show();
            $('#block-booking-calendar-booking-calendar-widget').css('pointer-events', 'none');

        }).bind("ajaxStop", function () {
            $('#t3').hide();
            $('#block-booking-calendar-booking-calendar-widget').css('pointer-events', 'auto');
        });

        $(document).on('click', '#block-booking-calendar-booking-calendar-widget tbody td', function (event) {

            var year = $('#controls .prev').attr('data-year');
            var month = $('#controls .prev').attr('data-month');
            var day = $(this).text();
            var widget = true;
            $('#block-booking-calendar-booking-calendar-widget tbody td').not(this).removeClass('selected-user');
            $(this).addClass('selected-user');


            ajaxWidgetCalendar(year, month, day, widget);

        });
    });
})(jQuery);

function ajaxWidgetCalendar(year, month, day, widget) {

    var $ = jQuery;

    var postString = 'day=' + day + '&month_number=' + month + '&year=' + year + '&widget=' + widget;
    console.log(postString);
    var ajaxPath = '/booking/calendar/ajax'; // This is the AjAX URL set by the custom Module.
    $.ajax({
        url: ajaxPath,
        method: "POST",
        data: postString,
        success: function (data) {
            console.log(data['hours_table']);
            $('#booking-courts-wrapper').html(data['hours_table']);
        }
    });
}