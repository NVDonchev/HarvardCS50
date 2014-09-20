/**
 * service.js
 *
 * Computer Science 50
 * Problem Set 8
 *
 * Implements a shuttle service.
 */

// default height
var HEIGHT = 0.8;

// default latitude
var LATITUDE = 42.3745615030193;

// default longitude
var LONGITUDE = -71.11803936751632;

// default heading
var HEADING = 1.757197490907891;

// default number of seats
var SEATS = 10;

// default velocity
var VELOCITY = 50;

// default max velocity
var MAX_VELOCITY = 200;

// default min velocity
var MIN_VELOCITY = 5;

// global reference to shuttle's marker on 2D map
var bus = null;

// global reference to 3D Earth
var earth = null;

// global reference to 2D map
var map = null;

// global reference to shuttle
var shuttle = null;

// global driver points counter
var driverPoints = 0;

// load version 1 of the Google Earth API
google.load("earth", "1");

// load version 3 of the Google Maps API
google.load("maps", "3", {other_params: "sensor=false"});

// once the window has loaded
$(window).load(function() {
    // listen for keydown anywhere in body
    $(document.body).keydown(function(event) {
        return keystroke(event, true);
    });

    // listen for keyup anywhere in body
    $(document.body).keyup(function(event) {
        return keystroke(event, false);
    });
    
    // listen for keypress anywhere in body
    $(document.body).keypress(function(event) {
        return keystroke(event, true);
    });

    // listen for click on Drop Off button
    $("#dropoff").click(function(event) {
        dropoff();
    });

    // listen for click on Pick Up button
    $("#pickup").click(function(event) {
        pickup();
    });
    
    // load application
    load();
    
    // show initial speed indicator
    updateSpeedIndicator();
});

// unload application
$(window).unload(function() {
    unload();
});

/**
 * Renders seating chart.
 */
function chart()
{
    // manage empty seats count
    var emptySeatsCounter = 0;
    for (var i = 0; i < shuttle.seats.length; i++)
    {
        if (shuttle.seats[i] == null)
        {
            emptySeatsCounter++;
        }
    }
    var html = "<p>Empty Seats: " + emptySeatsCounter + "</p>";
    
    // if there are passengers, take unique house names
    var uniqueHouseNames = GetUniqueHouseNames(shuttle.seats);
    if (uniqueHouseNames != undefined)
    {
        // sort house names
        uniqueHouseNames.sort();
        
        // show passengers list
        for (var i = 0; i < uniqueHouseNames.length; i++)
        {
            var startOfList = true;
            for (var j = 0; j < shuttle.seats.length; j++)
            {
                if (shuttle.seats[j] != null)
                {
                    if (uniqueHouseNames[i] == shuttle.seats[j].house)
                    {
                        // if first passenger with this house name
                        if (startOfList)
                        {
                            // show house name and start new list
                            html += "<div><b>" + uniqueHouseNames[i] +"</b></div>";
                            html += "<ul>";
                            
                            // reset variable
                            startOfList = false;
                        }
                        
                        // show a passenger name in the list
                        html += "<li>" + shuttle.seats[j].name + "</li>";
                    }
                }
            }
            
            // close list
            html += "</ul>";
        }
    }
    
    // insert the html
    $("#chart").html(html);
    
    // scroll to top smoothly when list changes
    $("#chart").animate({ scrollTop: 0 }, "slow");
}

/**
 * Drops up passengers if their stop is nearby.
 */
function dropoff()
{
    // prepare booleans
    var houseInRange = false;
    var passengerInShuttle = false;
    
    // iterate through all possible passengers
    for (var i = 0; i < shuttle.seats.length; i++)
    {
        if (shuttle.seats[i] != null)
        {
            // indicate that there is at least one passenger on board
            passengerInShuttle = true;
            
            // get the position of the current passenger's house
            var house = HOUSES[shuttle.seats[i].house];
            
            // get the shuttle distance from the house
            var distance = shuttle.distance(house.lat, house.lng);
            
            // check if passenger's house is nearby
            if (distance <= 30.0)
            {
                // indicate that there is a passenger house nearby
                houseInRange = true;
                
                // free the seat
                shuttle.seats[i] = null;
                
                // update driver points
                driverPoints++;
                
                // show announcement
                updateAnnouncement("passenger/s dropped off; driver points: " + driverPoints);
                
                // update chart
                chart();
            }
        }
    }
    
    // inform user
    if (passengerInShuttle && !houseInRange)
    {
        // show announcement
        updateAnnouncement("no passenger houses nearby");
    }
    else if (!passengerInShuttle)
    {
        // show announcement
        updateAnnouncement("no passengers on board");
    }
}

/**
 * Called if Google Earth fails to load.
 */
function failureCB(errorCode) 
{
    // report error unless plugin simply isn't installed
    if (errorCode != ERR_CREATE_PLUGIN)
    {
        alert(errorCode);
    }
}

/**
 * Handler for Earth's frameend event.
 */
function frameend() 
{
    shuttle.update();
}

/**
 * Called once Google Earth has loaded.
 */
function initCB(instance) 
{
    // retain reference to GEPlugin instance
    earth = instance;

    // specify the speed at which the camera moves
    earth.getOptions().setFlyToSpeed(100);

    // show buildings
    earth.getLayerRoot().enableLayerById(earth.LAYER_BUILDINGS, true);

    // disable terrain (so that Earth is flat)
    earth.getLayerRoot().enableLayerById(earth.LAYER_TERRAIN, false);

    // prevent mouse navigation in the plugin
    earth.getOptions().setMouseNavigationEnabled(false);

    // instantiate shuttle
    shuttle = new Shuttle({
        heading: HEADING,
        height: HEIGHT,
        latitude: LATITUDE,
        longitude: LONGITUDE,
        planet: earth,
        seats: SEATS,
        velocity: VELOCITY
    });

    // synchronize camera with Earth
    google.earth.addEventListener(earth, "frameend", frameend);

    // synchronize map with Earth
    google.earth.addEventListener(earth.getView(), "viewchange", viewchange);

    // update shuttle's camera
    shuttle.updateCamera();

    // show Earth
    earth.getWindow().setVisibility(true);

    // render seating chart
    chart();

    // populate Earth with passengers and houses
    populate();
}

/**
 * Handles keystrokes.
 */
function keystroke(event, state)
{
    // ensure we have event
    if (!event)
    {
        event = window.event;
    }

    // left arrow
    if (event.keyCode == 37)
    {
        shuttle.states.turningLeftward = state;
        return false;
    }

    // up arrow
    else if (event.keyCode == 38)
    {
        shuttle.states.tiltingUpward = state;
        return false;
    }

    // right arrow
    else if (event.keyCode == 39)
    {
        shuttle.states.turningRightward = state;
        return false;
    }

    // down arrow
    else if (event.keyCode == 40)
    {
        shuttle.states.tiltingDownward = state;
        return false;
    }

    // A, a
    else if (event.keyCode == 65 || event.keyCode == 97)
    {
        shuttle.states.slidingLeftward = state;
        return false;
    }

    // D, d
    else if (event.keyCode == 68 || event.keyCode == 100)
    {
        shuttle.states.slidingRightward = state;
        return false;
    }
  
    // S, s
    else if (event.keyCode == 83 || event.keyCode == 115)
    {
        shuttle.states.movingBackward = state;     
        return false;
    }

    // W, w
    else if (event.keyCode == 87 || event.keyCode == 119)
    {
        shuttle.states.movingForward = state;
        return false;
    }
    
    // +, =
    else if (event.keyCode == 43 || event.keyCode == 61)
    {
        regulateVelocity(event);
        updateSpeedIndicator();
        return false;
    }
    
    // -, _
    else if (event.keyCode == 45 || event.keyCode == 95)
    {
        regulateVelocity(event)
        updateSpeedIndicator(event);
        return false;
    }
  
    return true;
}

/**
 * Loads application.
 */
function load()
{
    // embed 2D map in DOM
    var latlng = new google.maps.LatLng(LATITUDE, LONGITUDE);
    map = new google.maps.Map($("#map").get(0), {
        center: latlng,
        disableDefaultUI: true,
        mapTypeId: google.maps.MapTypeId.ROADMAP,
        scrollwheel: false,
        zoom: 17,
        zoomControl: true
    });

    // prepare shuttle's icon for map
    bus = new google.maps.Marker({
        icon: "https://maps.gstatic.com/intl/en_us/mapfiles/ms/micons/bus.png",
        map: map,
        title: "you are here"
    });

    // embed 3D Earth in DOM
    google.earth.createInstance("earth", initCB, failureCB);
}

/**
 * Picks up nearby passengers.
 */
function pickup()
{
    // prepare booleans
    var passengerInRange = false;
    var freeSeat = false;
    
    // iterate through all possible passengers
    for (var i = 0; i < PASSENGERS.length; i++)
    {
        // check if passenger lives in the campus and if already picked
        if (PASSENGERS[i].house in HOUSES && PASSENGERS[i].placemark != null)
        {
            // get the position of the current passenger
            var placemarkPos =
                { 
                    "lat": PASSENGERS[i].placemark.getGeometry().getLatitude(),
                    "lng": PASSENGERS[i].placemark.getGeometry().getLongitude()
                }
                
            // get the distance from the shuttle
            var distance = shuttle.distance(placemarkPos.lat, placemarkPos.lng);
            
            // check if current passenger is close enogh to be picked
            if (distance <= 15.0)
            {
                passengerInRange = true;
                
                // itreate through the seats of the shuttle
                for (var j = 0; j < shuttle.seats.length; j++)
                {
                    // if there is a free seat for the passenger - pick up
                    if (shuttle.seats[j] == null)
                    {
                        // found a free seat
                        freeSeat = true;
                        
                        // remove placemark from 3D map
                        var features = earth.getFeatures();
                        features.removeChild(PASSENGERS[i].placemark);
                        
                        // remove parker from 2D map
                        PASSENGERS[i].marker.setMap(null);
                        
                        // add passenger to shuttle seat
                        shuttle.seats[j] = PASSENGERS[i];
                        
                        // show announcements
                        updateAnnouncement("passenger/s picked; driver points: " + driverPoints);
                        
                        // update seat chart
                        chart();
                        
                        // remove placemark from the passenger's object
                        PASSENGERS[i].placemark = null;
                                            
                        // break the loop after the passenger is happy in the seat
                        break;
                    }
                }
            }
        }
    }
    
    // inform user
    if (!passengerInRange)
    {
        // show announcement
        updateAnnouncement("no passengers to pickup nearby");
    }
    else if (!freeSeat)
    {
        // show announcement
        updateAnnouncement("no free seat to pickup more passengers");
    }
}

/**
 * Populates Earth with passengers and houses.
 */
function populate()
{
    // mark houses
    for (var house in HOUSES)
    {
        // plant house on map
        new google.maps.Marker({
            icon: "https://google-maps-icons.googlecode.com/files/home.png",
            map: map,
            position: new google.maps.LatLng(HOUSES[house].lat, HOUSES[house].lng),
            title: house
        });
    }

    // get current URL, sans any filename
    var url = window.location.href.substring(0, (window.location.href.lastIndexOf("/")) + 1);

    // scatter passengers
    for (var i = 0; i < PASSENGERS.length; i++)
    {
        // pick a random building
        var building = BUILDINGS[Math.floor(Math.random() * BUILDINGS.length)];

        // prepare placemark
        var placemark = earth.createPlacemark("");
        placemark.setName(PASSENGERS[i].name + " to " + PASSENGERS[i].house);

        // prepare icon
        var icon = earth.createIcon("");
        icon.setHref(url + "/img/" + PASSENGERS[i].username + ".jpg");

        // prepare style
        var style = earth.createStyle("");
        style.getIconStyle().setIcon(icon);
        style.getIconStyle().setScale(4.0);

        // prepare stylemap
        var styleMap = earth.createStyleMap("");
        styleMap.setNormalStyle(style);
        styleMap.setHighlightStyle(style);

        // associate stylemap with placemark
        placemark.setStyleSelector(styleMap);

        // prepare point
        var point = earth.createPoint("");
        point.setAltitudeMode(earth.ALTITUDE_RELATIVE_TO_GROUND);
        point.setLatitude(building.lat);
        point.setLongitude(building.lng);
        point.setAltitude(0.0);

        // associate placemark with point
        placemark.setGeometry(point);

        // add placemark to Earth
        earth.getFeatures().appendChild(placemark);

        // add marker to map
        var marker = new google.maps.Marker({
            icon: "https://maps.gstatic.com/intl/en_us/mapfiles/ms/micons/man.png",
            map: map,
            position: new google.maps.LatLng(building.lat, building.lng),
            title: PASSENGERS[i].name + " at " + building.name
        });

        // remember passenger's placemark and marker for pick-up's sake
        PASSENGERS[i].placemark = placemark;
        PASSENGERS[i].marker = marker;
    }
}

/**
 * Handler for Earth's viewchange event.
 */
function viewchange() 
{
    // keep map centered on shuttle's marker
    var latlng = new google.maps.LatLng(shuttle.position.latitude, shuttle.position.longitude);
    map.setCenter(latlng);
    bus.setPosition(latlng);
}

/**
 * Unloads Earth.
 */
function unload()
{
    google.earth.removeEventListener(earth.getView(), "viewchange", viewchange);
    google.earth.removeEventListener(earth, "frameend", frameend);
}

/**
 * Deals with announcement messages.
 */
function updateAnnouncement(message)
{
    // show a new message
    $("#announcements").html(message);

    // check moving keystrokes and change the message to the default
    $(document.body).keydown(function(event) {
        if (event.keyCode == 87 || event.keyCode == 119 ||
            event.keyCode == 65 || event.keyCode == 97 ||
            event.keyCode == 83 || event.keyCode == 115 ||
            event.keyCode == 68 || event.keyCode == 100)
        {
            $("#announcements").html("no announcements at this time");
        }
    });
}

/**
 * Deals with speed messages.
 */
function updateSpeedIndicator()
{
    // prepare message
    if (shuttle != null)
    {
        message = "Shuttle's max speed: " + shuttle.velocity + "km/h";
    }
    else
    {
        message = "Shuttle's max speed: " + VELOCITY + "km/h";
    }

    // show message
    $("#speed").html(message);
}

/**
 * Regulates shutter velocity.
 */
function regulateVelocity(event)
{
    // if + pressed
    if (event.keyCode == 43 || event.keyCode == 61)
    {
        if (shuttle.velocity < MAX_VELOCITY)
        {
            shuttle.velocity += 5;
        }
    }
    // if - pressed
    else if (event.keyCode == 45 || event.keyCode == 95)
    {
        if (shuttle.velocity > MIN_VELOCITY)
        {
            shuttle.velocity -= 5;
        }        
    }
}

/**
 * Gets the unique values from an array.
 */
function GetUniqueHouseNames(inputArray)
{
    // initialize array variable
    var outputArray = [];
    
    for (var i = 0; i < inputArray.length; i++)
    {
        // check if input array is null
        if (inputArray[i] != null)
        {
            // if item is now in the new array - add it
            if ((jQuery.inArray(inputArray[i].house, outputArray)) == -1)
            {
                outputArray.push(inputArray[i].house);
            }
        }
    }
    
    // return the new array
    return outputArray;
}
