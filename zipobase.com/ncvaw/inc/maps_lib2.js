/*------------------------------------------------------------------+
 | Functions used for searchable fusion table maps                  |
 | Requires jQuery                                                  |
 +-------------------------------------------------------------------*/

var gMap;
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
var district_state_house;
var district_state_senate;

google.load('visualization', '1', {}); //used for custom SQL call to get count

function map_reset() {
	setCookie("address","");
	setCookie("dist_state_senate","");
	setCookie("dist_state_house","");
	map_init_find_address();

}
function map_init() {
	
	geocoder = new google.maps.Geocoder();
	var raleigh = new google.maps.LatLng(35.487511,-79.782715);
	var myOptions = {
		zoom: 7,
		center: raleigh,
		mapTypeId: google.maps.MapTypeId.ROADMAP
	};
	gMap = new google.maps.Map(document.getElementById("map_canvas"),myOptions);
	
}
function map_init_find_address() {
	
	map_init()
	$( "#resultCount" ).html("");

	
	searchrecords = null;
	var address=getCookie("address");
	$("#txtSearchAddress").val(address);
	$("#legislators").html("")
	doSearch();
}

function get_legislator(chamber,dist) {
$.ajax({
	  type: 'GET',
	  url: '/guide/getleg.php',
	  data: { ch: chamber, dist: dist },
	  beforeSend:function(){
			
	    // this is where we append a loading image
	    //$('#legislators').html('<div>Loading...</div>');
	  },
	  success:function(data){
	    // successful request; do something with the data
		  var text="<h3 style='margin:0;padding:0;'>Your " + (chamber=='H'? 'House' : 'Senate')+" district: "+ dist+"</h3>";
		  $('#legislators').append(text);
		  $('#legislators').append(data);
	    
	  },
	  error:function(){
	    // failed request; give feedback to user
	    $('#legislators').html('<p class="error"><strong>Oops!</strong> Try that again in a few moments.</p>');
	  }
	});
}
function map_copy() {
	var canvas=document.getElementById("map_canvas");


	 var url = canvas.toDataURL();
	 
	 var newImg = document.createElement("img");
	 newImg.src = url;
	 document.body.appendChild(newImg);	

}
function doSearch(doZoom) {
	clearSearch();
	var address = $("#txtSearchAddress").val();
	setCookie("address",address);
	$("#legislators").html("");

	searchRadius = 1; //meters
	
	var type1 = 0; //$("#cbType1").is(':checked');
	var type2 = 1; //$("#cbType2").is(':checked');
	var type3 = 1; //$("#cbType3").is(':checked');
	
	searchStr = "SELECT geometry FROM " + fusionTableId + " WHERE geometry not equal to ''";
	
	//-----filter by type-------
	//remove this if you don't have any types to filter
	
	//best way to filter results by a type is to create a 'type' column and assign each row a number (strings work as well, but numbers are faster). then we can use the 'IN' operator and return all that are selected
	var searchType = "type IN (-1,";
        if (type1) { searchType += "1,"; } // congressional
	if (type2) { searchType += "2,"; } // senate
	if (type3) { searchType += "3,"; } // house

	
	//-------end of filter by type code--------
	
	// because the geocode function does a callback, we have to handle it in both cases - when they search for and address and when they dont
	if (address != "") {
		//if (address.toLowerCase().indexOf("raleigh") == -1) { address = address + " raleigh"; }
        searchStr += " AND " + searchType.slice(0, searchType.length - 1) + ")";
		
		geocoder.geocode( { 'address': address}, function(results, status) {
		    if (status == google.maps.GeocoderStatus.OK)
		    {
		        console.log("found address: " + results[0].geometry.location.toString());
		       
		            gMap.setCenter(results[0].geometry.location);
		            gMap.setZoom(14);
		            //gMap.setZoom(9);
		          
				addrMarker = new google.maps.Marker({
					position: results[0].geometry.location,
					map: gMap,
					icon: addrMarkerImage,
					animation: google.maps.Animation.DROP
				});

				drawSearchRadiusCircle(results[0].geometry.location);
				
				//searchStr += " AND ST_INTERSECTS(geometry, LATLNG" + results[0].geometry.location.toString() +")";
				searchStr += " AND ST_INTERSECTS(geometry, CIRCLE(LATLNG" + results[0].geometry.location.toString() + "," + searchRadius + "))";
				
				//get using all filters
				console.log(searchStr);
				searchrecords = new google.maps.FusionTablesLayer(fusionTableId, { query: searchStr } );
				
				searchrecords.setMap(gMap);
				//displayCount(searchStr);
				map_setbounds(searchStr);
				drawTable(searchStr);

				var contentString = '<b>ADDRESS:</b>' + address;

				var infowindow = new google.maps.InfoWindow({
    					content: contentString
				});

				google.maps.event.addListener(addrMarker, 'mouseover', function() {
  					infowindow.open(gMap,addrMarker);
				});

			} else {
				$('#legislators').html('<p class="error"><strong>Oops!</strong> We could not find your address. Make sure it is the </p>');
			}
		});
	} else {
		//get using all filters
		console.log(searchStr);
		searchrecords = new google.maps.FusionTablesLayer(fusionTableId, { query: searchStr } );
		
		searchrecords.setMap(gMap);
		//displayCount(searchStr);
		//drawTable(searchStr);
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
		searchrecords.setMap(gMap);
	} else {
		records.setMap(gMap);
	}
}

function map_geoFindMe() {
	  var output = document.getElementById("map_find_msg");

	  if (!navigator.geolocation){
	    output.innerHTML = "<p>Geolocation is not supported by your browser</p>";
	    return;
	  }

	  function mapsFindMeSuccess(position) {
	    var latitude  = position.coords.latitude;
	    var longitude = position.coords.longitude;

	    
	    //output.innerHTML = '<p>Latitude is ' + latitude + '� <br>Longitude is ' + longitude + '�</p>';

		var foundLocation = new google.maps.LatLng(position.coords.latitude,position.coords.longitude);
		addrFromLatLng(foundLocation);	
		
	  };

	  function error() {
	    output.innerHTML = "Unable to retrieve your location";
	  };

	  output.innerHTML = "<p>Locating...</p>";

	  navigator.geolocation.getCurrentPosition(mapsFindMeSuccess, error);
	}
function mapsFindMeSuccess2(position) 
{
	var foundLocation = new google.maps.LatLng(position.coords.latitude,position.coords.longitude);
	addrFromLatLng(foundLocation);	

}
function mapsFindMeFail(err) 
{
	$('#map_find_msg').html('Cound not find address automatically. Please type address above');

}
function mapsFindMe() {
	// Try W3C Geolocation (Preferred)
	var foundLocation;
	$('#map_find_msg').html('Trying to locate you automatically. You browser may first ask you to allow this.');
	
	if(navigator.geolocation) 
	{
		 var options = {timeout:3000};
		navigator.geolocation.getCurrentPosition(mapsFindMeSuccess,mapsFindMeFail);
	}
	else
		mapsFindMeFail();
}

function addrFromLatLng(latLngPoint) 
{
	geocoder.geocode({ 'latLng' : latLngPoint}, 
		function(results, status) 
		{
			var address=0;
			if (status == google.maps.GeocoderStatus.OK)
			{
				address=results[0]; /* TODO give more generic address to not freak people out */
				if(!address)
					address=results[0];
				if (address) 
				{
					$('#txtSearchAddress').val(address.formatted_address);
					 $('.hint').focus();
				} 
			}
			if(!address)
				$('#txtSearchAddress').val('Cound not find address');
			doSearch();
			
		}
	);
}

function drawSearchRadiusCircle(point) {
	var circleOptions = {
		strokeColor: "#4b58a6",
		strokeOpacity: 0.3,
		strokeWeight: 1,
		fillColor: "#4b58a6",
		fillOpacity: 0.05,
		map: gMap,
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
/*
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
	//$( "#resultCount" ).fadeOut(function() { $( "#resultCount" ).html((numRows) + " " + name + " found"); } );
	//$( "#resultCount" ).fadeIn();
}

*/
function map_setbounds(searchStr) {
	
	
    //var query = searchStr.replace("SELECT geometry ", "SELECT Latitude,Longitude");
	
	var queryText = encodeURIComponent(searchStr);
	query = new google.visualization.Query(
			'http://www.google.com/fusiontables/gvizdata?tq=' + queryText);

	query.send(function(response) 
	{
		var numRows = response.getDataTable().getNumberOfRows();
		var bounds = new google.maps.LatLngBounds();
		// create the list of lat/long coordinates
	
		for (i = 0; i < numRows; i++) {
			var polygon = response.getDataTable().getValue(i, 0);
			var a= polygon.search('<coordinates>');
			var b=polygon.search('</coordinates>');
			var c=polygon.substr(a+13,b-a-13);
			var points=c.split(' ');
			for (j = 0; j < points.length; j++) {
				var p=points[j];
				var ll=p.split(',');
				var c=new google.maps.LatLng(Number(ll[1]),Number( ll[0]));
				
				bounds.extend(c);
				
			}
			
			
			
			
		}


		gMap.fitBounds(bounds);

	
	});
}
function drawTable(searchStr) {
    // Construct query

    var query = searchStr.replace("SELECT geometry ", "SELECT 'District' as District,'typedesc'");

    var queryText = encodeURIComponent(query);
    var gvizQuery = new google.visualization.Query(
        'http://www.google.com/fusiontables/gvizdata?tq=' + queryText);

    // Send query and draw table with data in response
    gvizQuery.send(function (response) {
        var numRows = response.getDataTable().getNumberOfRows();
        var numCols = response.getDataTable().getNumberOfColumns();
        /*
        var ftdata = ['<table><thead><tr>'];
        for (var i = 0; i < numCols; i++) {
            var columnTitle = response.getDataTable().getColumnLabel(i);
            ftdata.push('<th>' + columnTitle + '</th>');
        }
        ftdata.push('</tr></thead><tbody>');
*/
        for (var i = 0; i < numRows; i++) {
            /*
              
            ftdata.push('<tr>');
            for (var j = 0; j < numCols; j++) 
            {
                var rowValue = response.getDataTable().getValue(i, j);
                ftdata.push('<td>' + rowValue + '</td>');
            }
            ftdata.push('</tr>');
            */
            var chamber=response.getDataTable().getValue(i, 1);
            var district=response.getDataTable().getValue(i, 0);
            if(chamber=='House')
            	{
            	setCookie("dist_state_house",district);
            	chamber='H';
            	}
            else
            	{
            	setCookie("dist_state_senate",district);
            	chamber='S';
            	}
            
            get_legislator(chamber,district);
        }
        ncvaw_init_your_district_menus();
        $("#map_find_msg").html("");
       // ftdata.push('</tbody></table>');
        //document.getElementById('ft-data').innerHTML = ftdata.join('');
    });
}



function map_show_district(chamber, district) {

	

	
	searchStr = "SELECT geometry FROM " + fusionTableId + " WHERE District = "+district+" AND type =";
	
	//-----filter by type-------
	//remove this if you don't have any types to filter
	
	//best way to filter results by a type is to create a 'type' column and assign each row a number (strings work as well, but numbers are faster). then we can use the 'IN' operator and return all that are selected

	if (chamber=='S') { searchStr += "2"; } // senate
	if (chamber=='H') { searchStr += "3"; } // house

	searchrecords = new google.maps.FusionTablesLayer(fusionTableId, { query: searchStr } );
	searchrecords.setMap(gMap);
	map_setbounds(searchStr);

}
