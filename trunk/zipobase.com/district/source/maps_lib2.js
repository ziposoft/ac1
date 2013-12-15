/*------------------------------------------------------------------+
 | Functions used for searchable fusion table maps                  |
 | Requires jQuery                                                  |
 +-------------------------------------------------------------------*/

var map;
var geocoder;
var addrMarker;
var addrMarkerImage = 'blue-pushpin.png';

var fusionTableId = 2492262; //replace this with the ID of your fusion table
//https://www.googleapis.com/fusiontables/tables/2492262/columns
var searchRadius = 1; //in meters ~ 1 mile
var recordName = "result";
var recordNamePlural = "results";
var searchrecords;
var records = new google.maps.FusionTablesLayer(fusionTableId);

var goZoom = new Boolean(1);
var setzoom = 0;
var searchStr;
var searchRadiusCircle;

google.load('visualization', '1', {}); //used for custom SQL call to get count

function initialize() {
	$( "#resultCount" ).html("");
	
	geocoder = new google.maps.Geocoder();
	var raleigh = new google.maps.LatLng(35.487511,-79.782715);
	var myOptions = {
		zoom: 7,
		center: raleigh,
		mapTypeId: google.maps.MapTypeId.ROADMAP
	};
	map = new google.maps.Map(document.getElementById("map_canvas"),myOptions);
	
	$("#ddlRadius").val("3220");
	
	$("#cbType1").attr("checked", "checked");
	$("#cbType2").attr("checked", "checked");
	$("#cbType3").attr("checked", "checked");
	
	searchrecords = null;
	//$("#txtSearchAddress").val("");
	doSearch();
}

function doSearch(doZoom) {
	clearSearch();
	var address = $("#txtSearchAddress").val();

	searchRadius = 1;//$("#ddlRadius").val();
	
	var type1 = $("#cbType1").is(':checked');
	var type2 = $("#cbType2").is(':checked');
	var type3 = $("#cbType3").is(':checked');
	
	searchStr = "SELECT geometry FROM " + fusionTableId + " WHERE geometry not equal to ''";
	
	//-----filter by type-------
	//remove this if you don't have any types to filter
	
	//best way to filter results by a type is to create a 'type' column and assign each row a number (strings work as well, but numbers are faster). then we can use the 'IN' operator and return all that are selected
	var searchType = "type IN (-1,";
        if (type1) { searchType += "1,"; } // congressional
	if (type2) { searchType += "2,"; } // senate
	if (type3) { searchType += "3,"; } // house

	
        searchStr += " AND " + searchType.slice(0, searchType.length - 1) + ")";

	
	//-------end of filter by type code--------
	
	// because the geocode function does a callback, we have to handle it in both cases - when they search for and address and when they dont
	if (address != "") {
		//if (address.toLowerCase().indexOf("raleigh") == -1) { address = address + " raleigh"; }
		
		geocoder.geocode( { 'address': address}, function(results, status) {
		    if (status == google.maps.GeocoderStatus.OK)
		    {
		        console.log("found address: " + results[0].geometry.location.toString());
		       
		            map.setCenter(results[0].geometry.location);
		            map.setZoom(14);
		            if (doZoom) {
		                switch (searchRadius) {
		                    case "200":
		                        map.setZoom(17);
		                        break;
		                    case "805":
		                        map.setZoom(15);
		                        break;
		                    case "1610":
		                        map.setZoom(14);
		                        break;
		                    case "3220":
		                        map.setZoom(13);
		                        break;
		                    case "8050":
		                        map.setZoom(12);
		                        break;
		                    case "16100":
		                        map.setZoom(11);
		                        break;
		                    default:
		                        map.setZoom(14);
		                }
		            }
		        

				addrMarker = new google.maps.Marker({
					position: results[0].geometry.location,
					map: map,
					icon: addrMarkerImage,
					animation: google.maps.Animation.DROP
				});

				drawSearchRadiusCircle(results[0].geometry.location);
				
				//searchStr += " AND ST_INTERSECTS(geometry, LATLNG" + results[0].geometry.location.toString() +")";
				searchStr += " AND ST_INTERSECTS(geometry, CIRCLE(LATLNG" + results[0].geometry.location.toString() + "," + searchRadius + "))";
				
				//get using all filters
				console.log(searchStr);
				searchrecords = new google.maps.FusionTablesLayer(fusionTableId, { query: searchStr } );
				
				searchrecords.setMap(map);
				displayCount(searchStr);
				drawTable(searchStr);

				var contentString = '<b>ADDRESS:</b><br>' + address;

				var infowindow = new google.maps.InfoWindow({
    					content: contentString
				});

				google.maps.event.addListener(addrMarker, 'mouseover', function() {
  					infowindow.open(map,addrMarker);
				});

			} else {
				alert("We could not find your address: " + status);
			}
		});
	} else {
		//get using all filters
		console.log(searchStr);
		searchrecords = new google.maps.FusionTablesLayer(fusionTableId, { query: searchStr } );
		
		searchrecords.setMap(map);
		displayCount(searchStr);
		drawTable(searchStr);
	}
}

function clearSearch() {
	if (searchrecords != null) { searchrecords.setMap(null); }
	if (addrMarker != null) { addrMarker.setMap(null); }
	if (searchRadiusCircle != null) { searchRadiusCircle.setMap(null); }
	records.setMap(null);
}

function refreshrecords() {
	if (searchrecords != null) {
		searchrecords.setMap(map);
	} else {
		records.setMap(map);
	}
}

function findMe() {
	// Try W3C Geolocation (Preferred)
	var foundLocation;
	
	if(navigator.geolocation) {
		navigator.geolocation.getCurrentPosition(function(position) {
			foundLocation = new google.maps.LatLng(position.coords.latitude,position.coords.longitude);
			addrFromLatLng(foundLocation);
		}, null);
	} else {
		alert("Sorry, we could not find your location.");
	}
}

function addrFromLatLng(latLngPoint) {
	geocoder.geocode({'latLng': latLngPoint}, function(results, status) {
		if (status == google.maps.GeocoderStatus.OK) {
			if (results[1]) {
				$('#txtSearchAddress').val(results[1].formatted_address);
				$('.hint').focus();
				doSearch();
			}
		} else {
			alert("Geocoder failed due to: " + status);
		}
	});
}

function drawSearchRadiusCircle(point) {
	var circleOptions = {
		strokeColor: "#4b58a6",
		strokeOpacity: 0.3,
		strokeWeight: 1,
		fillColor: "#4b58a6",
		fillOpacity: 0.05,
		map: map,
		center: point,
		radius: parseInt(searchRadius)
	};
	searchRadiusCircle = new google.maps.Circle(circleOptions);
}

function getFTQuery(sql) {
    var queryText = encodeURIComponent(sql);
    var q = 'http://www.google.com/fusiontables/gvizdata?tq=' + queryText;
    console.log(q);
	return new google.visualization.Query('http://www.google.com/fusiontables/gvizdata?tq='  + queryText);
}

function displayCount(searchStr) {
	//set the query using the parameter
	searchStr = searchStr.replace("SELECT geometry ","SELECT Count() ");
	
	//set the callback function
	getFTQuery(searchStr).send(displaySearchCount);
}

function displaySearchCount(response) {
	var numRows = 0;
	if (response.getDataTable().getNumberOfRows() > 0) { numRows = parseInt(response.getDataTable().getValue(0, 0)); }
	var name = recordNamePlural;
	if (numRows == 1) { name = recordName; }
	$( "#resultCount" ).fadeOut(function() { $( "#resultCount" ).html(addCommas(numRows) + " " + name + " found"); } );
	$( "#resultCount" ).fadeIn();
}

function addCommas(nStr) {
	nStr += '';
	x = nStr.split('.');
	x1 = x[0];
	x2 = x.length > 1 ? '.' + x[1] : '';
	var rgx = /(\d+)(\d{3})/;
	while (rgx.test(x1)) {
		x1 = x1.replace(rgx, '$1' + ',' + '$2');
	}
	return x1 + x2;
}


// Poygon getBounds extension - google-maps-extensions
// http://code.google.com/p/google-maps-extensions/source/browse/google.maps.Polygon.getBounds.js
if (!google.maps.Polygon.prototype.getBounds) {
    google.maps.Polygon.prototype.getBounds = function (latLng) {
        var bounds = new google.maps.LatLngBounds();
        var paths = this.getPaths();
        var path;

        for (var p = 0; p < paths.getLength() ; p++) {
            path = paths.getAt(p);
            for (var i = 0; i < path.getLength() ; i++) {
                bounds.extend(path.getAt(i));
            }
        }

        return bounds;
    }
}

// Polygon containsLatLng - method to determine if a latLng is within a polygon
google.maps.Polygon.prototype.containsLatLng = function (latLng) {
    // Exclude points outside of bounds as there is no way they are in the poly

    var lat, lng;

    //arguments are a pair of lat, lng variables
    if (arguments.length == 2) {
        if (typeof arguments[0] == "number" && typeof arguments[1] == "number") {
            lat = arguments[0];
            lng = arguments[1];
        }
    } else if (arguments.length == 1) {
        var bounds = this.getBounds();

        if (bounds != null && !bounds.contains(latLng)) {
            return false;
        }
        lat = latLng.lat();
        lng = latLng.lng();
    } else {
        console.log("Wrong number of inputs in google.maps.Polygon.prototype.contains.LatLng");
    }

    // Raycast point in polygon method
    var inPoly = false;

    var numPaths = this.getPaths().getLength();
    for (var p = 0; p < numPaths; p++) {
        var path = this.getPaths().getAt(p);
        var numPoints = path.getLength();
        var j = numPoints - 1;

        for (var i = 0; i < numPoints; i++) {
            var vertex1 = path.getAt(i);
            var vertex2 = path.getAt(j);

            if (vertex1.lng() < lng && vertex2.lng() >= lng || vertex2.lng() < lng && vertex1.lng() >= lng) {
                if (vertex1.lat() + (lng - vertex1.lng()) / (vertex2.lng() - vertex1.lng()) * (vertex2.lat() - vertex1.lat()) < lat) {
                    inPoly = !inPoly;
                }
            }

            j = i;
        }
    }

    return inPoly;
}

function drawTable(searchStr) {
    // Construct query

    query = searchStr.replace("SELECT geometry ", "SELECT 'District' as District,'typedesc'");

   // var query = "SELECT 'District' as District,'typedesc' FROM 1D2YQGiGiPPHfcIAAV6CxedB6eTP_Njqbrgq0z1E   WHERE geometry not equal to ''		AND ST_INTERSECTS(geometry, CIRCLE(LATLNG(35.824523, -78.780505),200))";



 
    /*
    var team = document.getElementById('team').value;
    if (team) {
        query += " WHERE 'Scoring Team' = '" + team + "'";
    }	*/
    var queryText = encodeURIComponent(query);
    var gvizQuery = new google.visualization.Query(
        'http://www.google.com/fusiontables/gvizdata?tq=' + queryText);

    // Send query and draw table with data in response
    gvizQuery.send(function (response) {
        var numRows = response.getDataTable().getNumberOfRows();
        var numCols = response.getDataTable().getNumberOfColumns();

        var ftdata = ['<table><thead><tr>'];
        for (var i = 0; i < numCols; i++) {
            var columnTitle = response.getDataTable().getColumnLabel(i);
            ftdata.push('<th>' + columnTitle + '</th>');
        }
        ftdata.push('</tr></thead><tbody>');

        for (var i = 0; i < numRows; i++) {
            ftdata.push('<tr>');
            for (var j = 0; j < numCols; j++) {
                var rowValue = response.getDataTable().getValue(i, j);
                ftdata.push('<td>' + rowValue + '</td>');
            }
            ftdata.push('</tr>');
        }
        ftdata.push('</tbody></table>');
        document.getElementById('ft-data').innerHTML = ftdata.join('');
    });
}