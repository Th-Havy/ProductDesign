var api_endpoint = "http://127.0.0.1:8000/api/trashbin/"

function initMap() {
var EPFL_CENTER_LATITUDE = 46.518349;
var EPFL_CENTER_LONGITUDE = 6.568052;
var DEFAULT_ZOOM = 18;

// To hide google maps points of interest
var mapStyles =[
    {
        featureType: "poi",
        elementType: "labels",
        stylers: [
              { visibility: "off" }
        ]
    }
];

var mapProperties= {
    center:new google.maps.LatLng(EPFL_CENTER_LATITUDE, EPFL_CENTER_LONGITUDE),
    zoom:DEFAULT_ZOOM,
    styles:mapStyles,
    // Remove street view button
    streetViewControl:false,
    // Hide satellite view button
    mapTypeControlOptions: {
      mapTypeIds: [google.maps.MapTypeId.ROADMAP]
    },
};

// Create the map on the webpage
var map=new google.maps.Map(document.getElementById("trashBinMap"),mapProperties);

// Fetch the position of the trash bins and display markers
$(document).ready(function() {
    $.ajax({
        url: api_endpoint
    }).then(function(data) {
        data.forEach(function(trashbin) {
            new google.maps.Marker({
                position: {lat: trashbin.latitude, lng: trashbin.longitude},
                map: map
            });
        });
    });
});

/*var rolex = {lat: EPFL_CENTER_LATITUDE, lng: EPFL_CENTER_LONGITUDE};

var marker1 = new google.maps.Marker({
          position: rolex,
          map: map
      });*/
}
