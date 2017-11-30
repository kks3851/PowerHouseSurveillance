// Variables
var ws = undefined; // websocket instance
var logs = [];
var logsLimit = 4;
var b = document.getElementById('btnWS');
var panleft = document.getElementById('panleft');
var panright = document.getElementById('panright');
var tiltup = document.getElementById('tiltup');
var tiltdown = document.getElementById('tiltdown');

// Initialize the WebSocket
function initWebSocket() {
    var ipName = window.location.hostname;
    if (wss) {
        ws.close(); // close the websocket if open.
        ws = undefined;
    }
    ws = new WebSocket('ws://' + ipName + '/stream');

    ws.onopen = function () { // when handshake is complete:
        log('WebSocket open to ZentriOS device ' + ipName);
        //*** Change the text of the button to read "Stop Webcam" ***//
        var btn = document.querySelector("#btnWS");
	btn.onclick = function () {
		var elem = document.getElementById("#btnWS");
		if (elem.innerHTML == "Start Webcam") {
				elem.innerHTMl = "Stop Webcam";
		} else {
			elem.innerHTML = "Start Webcam";
			}
		elem.title = "Click to Stop Webcam";
    };
    ws.onclose = function () { // when socket is closed:
        log('WebSocket connection to ' + ipName + ' has been closed!');
        var btn = document.querySelector("#btnWS");
	btn.onclick = function () {
		var elem = document.getElementById("#btnWS");
		if (elem.value == "Stop Webcam")
		elem.value = "Start Webcam";
		else elem.value == "Stop Webcam";
		elem.title = "Click to Start Webcam";
    };

    wss.onmessage = function (event) { // when client receives a WebSocket message:
        //*** Display a new timestamp ***//
        //var now = new Date();
        setTime();
        
        //*** Set the source of the image to the image on the WiFi chip ***//
        document.getElementById('pic').src = "../img.jpg";        
    };
    
    wss.onerror = function () { // when an error occurs
		ws.close();
		log('Websocket error');
	};
}


// Set up event listeners
//*** When the button is clicked, disable it, and depending on whether a Websocket is open or not, either run "initWebSocket()" or "ws.close()" ***//
//var b = document.getElementById('btnWs');
    b.addEventListener(
    'click', 
    function (event) {
        b.disabled=true;
        setTime();
        if (ws.readyState==3 || ws.readyState==2){
            initWebSocket();
        }
        else {
            ws.close();
        }
    });

panleft.addEventListener(
    'click', 
    function (event) {
        sendValuePanLeft(event);
    });
panright.addEventListener(
    'click', 
    function (event) {
        sendValuePanRight(event);
    });
tiltup.addEventListener(
    'click', 
    function (event) {
        sendValueTiltUp(event);
    });
tiltdown.addEventListener(
    'click', 
    function (event) {
        sendValueTiltDown(event);
    });

function sendValuePanLeft (event){
    var val = 'panleft';
        if (ws){
            ws.send(val);
            sent('Panning left');
        }
        else {
            log('Unable to pan left; Websocket not found.');
        }
}
function sendValuePanRight (event){
    var val = 'panright';
        if (ws){
            ws.send(val);
            sent('Panning right');
        }
        else {
            log('Unable to pan right; Websocket not found.');
        }
}
function sendValueTiltUp (event){
    var val = 'tiltup';
        if (ws){
            ws.send(val);
            sent('Tilting up');
        }
        else {
            log('Unable to tilt up; Websocket not found.');
        }
}
function sendValueTiltDown (event){
    var val = 'tiltdown';
        if (ws){
            ws.send(val);
            sent('Tilting down');
        }
        else {
            log('Unable to tilt down; Websocket not found.');
        }
}


 /*TIME STAMP FUNCTIONS*/
function getTime() {
    var now = new Date();
    return ((now.getMonth() + 1) + '/' + (now.getDate()) + '/' + now.getFullYear() + "  "
                    + now.getHours() + ':'
                         + ((now.getMinutes() < 10) ? ("0" + now.getMinutes()) : (now.getMinutes())) + ':' + ((now.getSeconds() < 10) ? ("0" + now
                         .getSeconds()) : (now.getSeconds())));
    }
    
function setTime() {
    document.getElementById('time').value = getTime();
    }
    


// Other functions
function log(txt) {
    logs.push({
        'content': txt,
        'type': 'log'
    });
    showLog(logs, 'log', logsLimit);
}

function sent(txt) {
    logs.push({'content':txt, 'type':'log'});
    showLog(logs, 'log',logsLimit)
}

function showLog(logArray, logId, logLimit) {
    var logContent = '';
    var logLength = logArray.length;
    var iStart = logLength - logLimit - 1;
    if (iStart < 0) {
        iStart = 0;
    }
    for (var index = iStart; index < logLength; ++index) {
        logItem = logArray[index];
        logContent += '<span class="' + logItem.type + '">' + logItem.content + '</span><br/>\n'
    }
    document.getElementById(logId).innerHTML = logContent;
}

// Define initialization function
function init() {
    initWebSocket();
}

// Open Websocket as soon as page loads
window.addEventListener("load", init, false);

