var api_endpoint = "http://127.0.0.1:8000/api/trashbin/"

color = Chart.helpers.color;

// Get the canvas element from the page to draw the graph
var ctx = document.getElementById("stateChart").getContext('2d');

var fullness_levels = [];
var times = [];

var pagePath = window.location.pathname; // Returns path (/map/state/id)

// Extract date filter from the url
var dateFilter = new URL(window.location.href).searchParams.get("date");

if (dateFilter == null)
{
    dateFilter = "";
}
else
{
    dateFilter = "?date=" + dateFilter;
}

// Fetch the state data
$(document).ready(function() {
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
