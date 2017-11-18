function initMap() {
var EPFL_CENTER_LATITUDE = 46.518349;
var EPFL_CENTER_LONGITUDE = 6.568052;
var DEFAULT_ZOOM = 18;

var mapProp= {
    center:new google.maps.LatLng(EPFL_CENTER_LATITUDE, EPFL_CENTER_LONGITUDE),
    zoom:DEFAULT_ZOOM,
};
var map=new google.maps.Map(document.getElementById("trashBinMap"),mapProp);

var rolex = {lat: 46.518349, lng: 6.568052};

var marker1 = new google.maps.Marker({
          position: rolex,
          map: map
        });

console.log("Ta mère")
}
