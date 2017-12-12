var api_endpoint = "http://127.0.0.1:8000/api/trashbin/"

function initMap() {
var EPFL_CENTER_LATITUDE = 46.518349;
var EPFL_CENTER_LONGITUDE = 6.568052;
var DEFAULT_ZOOM = 18;

// Variable containing the trash bin positions grouped by building
var buildingClusters = {};

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

// Define the properties of the displayed map
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

var infoWindowContent = document.getElementById("infoWindow").innerHTML;

var infoWindow = new google.maps.InfoWindow(
{
    content: infoWindowContent
});


// Fetch the position of the trash bins and display markers
$(document).ready(function()
{
    $.ajax(
    {
        url: api_endpoint
    }).then(function(data)
    {
        populateMap(map, data, buildingClusters, infoWindow, infoWindowContent);
    });
});
}

function populateMap(map, data, buildingClusters, infoWindow, infoWindowContent)
{
    data.forEach(function(trashbin)
    {
        addTrashbinMarker(map, buildingClusters, trashbin, infoWindow, infoWindowContent);
    });

    updateBuildingClusters(map, buildingClusters);
}

function addTrashbinMarker(map, buildingClusters, trashbin, infoWindow, infoWindowContent)
{
    var color = "";

    if (trashbin.state.fullness > 70)
        color = "http://maps.google.com/mapfiles/ms/icons/red-dot.png";
    else if (trashbin.state.fullness > 30)
        color = "http://maps.google.com/mapfiles/ms/icons/orange-dot.png";
    else
        color = "http://maps.google.com/mapfiles/ms/icons/green-dot.png";

    // Create a marker for each trashbin
    var marker = new google.maps.Marker(
    {
        position: {lat: trashbin.latitude, lng: trashbin.longitude},
        map: map,
        icon: color
    });

    // Pop up window with informations about the trashbin
    marker.addListener('click', function()
    {
        openInformationPopUp(map, marker, trashbin, infoWindow, infoWindowContent);
    });

    addTrashbinToBuildingClusters(map, buildingClusters, trashbin, marker);
}

function addTrashbinToBuildingClusters(map, buildingClusters, trashbin, marker)
{
    if (buildingClusters[trashbin.building] == undefined)
    {
        buildingClusters[trashbin.building] = [];
    }

    buildingClusters[trashbin.building].push(marker);
}

// Group trash bins by building
function updateBuildingClusters(map, buildingClusters)
{
    for(var building in buildingClusters)
    {
       var markerCluster = new MarkerClusterer(map, buildingClusters[building],
           {imagePath: 'https://developers.google.com/maps/documentation/javascript/examples/markerclusterer/m',
            averageCenter: true,
            gridSize: 100});
    }
}

function openInformationPopUp(map, marker, trashbin, infoWindow, infoWindowContent)
{
    var content = infoWindowContent;
    content = content.replace("__building__", trashbin.building);
    content = content.replace("__floor__", "floor " + String(trashbin.floor));
    content = content.replace("__waste__", trashbin.wasteType);
    content = content.replace("__fullness__", trashbin.state.fullness);
    content = content.replace("__stateUrl__", window.location.pathname + "state/" + String(trashbin.pk));
    content = content.replace("__editUrl__", "/admin/map/trashbin/" + String(trashbin.pk) + "/change/");
    http://127.0.0.1:8000/admin/map/trashbin/8/change/
    infoWindow.setContent(content);
    infoWindow.open(map, marker);
}
