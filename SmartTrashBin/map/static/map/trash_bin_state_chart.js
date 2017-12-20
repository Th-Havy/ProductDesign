var pagePath = window.location.pathname; // Returns path (/map/state/id)
var pageHref = window.location.href; // http://127.0.0.1:8000/map/state/8

var domain = pageHref.substring(0, pageHref.search("map"));// http://127.0.0.1:8000/ (domain)

var api_endpoint = domain + "api/trashbin/"

color = Chart.helpers.color;

// Get the canvas element from the page to draw the graph
var ctx = document.getElementById("stateChart").getContext('2d');

var fullness_levels = [];
var times = [];

dateFilter = extractDate();

var averageClicked = false;

// Fetch the state data
$(document).ready(function()
{
    // Add datepicker to filter by day
    $(function ()
    {
        $('#datepicker').datepicker({
            format:'yyyy-mm-dd',
            autoclose: true,
            clearBtn: true,
            todayBtn: true,
            todayHighlight:true,
            startDate: '2016-01-01',
            endDate: '0d',
        });

        $('#datepicker').on('changeDate', function(e)
        {
            if (averageClicked) {
                averageClicked = false;
            }
            else {
                // Reload page with a filter date
                reloadWithDateFilter(e.format());
            }
        });

        var calendarButton = document.getElementById('calendarButton');
        calendarButton.onclick = function() {
            // Replace text  "Clear" by "Average"
            $('div:contains("Clear")')[1].innerHTML = $('div:contains("Clear")')[1].innerHTML.replace("Clear", "Average");
            var averageButton = document.getElementsByClassName("clear")[0];

            // Add a callback function to the average button
            averageButton.onclick = function() {
                averageClicked = true;
                reloadWithDateFilter("average");
            }
        };
    });

    // Fetch states from database
    $.ajax({
        url: api_endpoint + pagePath.substr(11) + "/state/" + dateFilter
    }).then(function(data) {
        data.forEach(function(state) {
            fullness_levels.push(state.fullness);
            times.push(state.time);
        });
        times.reverse();
        fullness_levels.reverse();

        createChart(ctx, times, fullness_levels);

    });
});

function createChart(ctx, times, fullness_levels)
{
    var myChart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: times,
            datasets: [{
                label: 'Fullness level',
                data: fullness_levels,
                backgroundColor: color("#4286f4").alpha(0.5).rgbString(),
				borderColor: "#3e95cd",
                borderWidth: 3
            }]
        }
    });
}

// Extract date filter from the url
function extractDate()
{
    var dateFilter = new URL(window.location.href).searchParams.get("date");

    if (dateFilter == null)
    {
        dateFilter = "";
    }
    else
    {
        dateFilter = "?date=" + dateFilter;
    }

    return dateFilter;
}

function reloadWithDateFilter(dateFilter)
{
    if (dateFilter == null)
    {
        dateFilter = "";
    }
    else if (dateFilter == "average") {
        dateFilter = "?date=" + dateFilter;
    }
    else
    {
        dateFilter = "?date=" + dateFilter + "T00:00:01Z";
    }

    window.location.assign(dateFilter);
}
