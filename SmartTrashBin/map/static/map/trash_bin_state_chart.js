var api_endpoint = "http://127.0.0.1:8000/api/trashbin/"

// Get the canvas element from the page to draw the graph
var ctx = document.getElementById("stateChart").getContext('2d');

var fullness_levels = [];
var times = [];

// Fetch the state data
$(document).ready(function() {
    $.ajax({
        url: api_endpoint + "8/state/"
    }).then(function(data) {
        data.forEach(function(state) {
            fullness_levels.push(state.fullness);
            times.push(state.time);
        });
        console.log(times);
    });
});

//liste détat dans data
data.forEach(fonctionDeTraitement);

function fonctionDeTraitement(state) {
    static average = 0

    average += state.fullness /100;
}

var myChart = new Chart(ctx, {
    type: 'line',
    data: {
        labels: times,
        datasets: [{
            label: '#Fullness level',
            data: fullness_levels,
            /*backgroundColor: [
                'rgba(255, 99, 132, 0.2)',
                'rgba(54, 162, 235, 0.2)',
                'rgba(255, 206, 86, 0.2)',
                'rgba(75, 192, 192, 0.2)',
                'rgba(153, 102, 255, 0.2)',
                'rgba(255, 159, 64, 0.2)'
            ],
            borderColor: [
                'rgba(255,99,132,1)',
                'rgba(54, 162, 235, 1)',
                'rgba(255, 206, 86, 1)',
                'rgba(75, 192, 192, 1)',
                'rgba(153, 102, 255, 1)',
                'rgba(255, 159, 64, 1)'
            ],*/
            backgroundColor: 'rgb(255, 99, 132)',
            borderColor: 'rgb(255, 99, 132)',
            borderWidth: 1
        }]
    },
    options: {
        /*scales: {
            yAxes: [{
                ticks: {
                    beginAtZero:true
                }
            }]
        }*/
    }
});
