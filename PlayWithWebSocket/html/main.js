//document.getElementById("status").innerHTML = "WebSocket is not connected";

var websocket = new WebSocket('ws://'+location.hostname+'/');

function clickPlay(element) {
	console.log('clickPlay');
	var label = element.getAttribute('aria-label');
	console.log(label);
	if (label == 'Play') {
		element.classList.add('playing');
		element.setAttribute('aria-label', 'Pause');
		sendId("play");
	} else {
		element.classList.remove('playing');
		element.setAttribute('aria-label', 'Play');
		sendId("pause");
	}
}

function clickMute(element) {
	console.log('clickMute');
	var label = element.getAttribute('aria-label');
	console.log('lavel=' + label);
	if (label == 'Mute') {
		element.classList.add('muted');
		element.setAttribute('aria-label', 'Muted');
		sendIdValue("mute", "ON");
	} else {
		element.classList.remove('muted');
		element.setAttribute('aria-label', 'Mute');
		sendIdValue("mute", "OFF");
	}
}

function clickSkip(element) {
	console.log('clickSkip');
	var label = element.getAttribute('aria-label');
	console.log('lavel=' + label);
	if (label == "Skip Backward") {
		sendId("previous");
	} else {
		sendId("next");
	}
}

function clickLoop(element) {
	console.log('clickLoop');
	var label = element.getAttribute('aria-label');
	console.log('lavel=' + label);
	if (label == 'Loop') {
		element.classList.add('looped');
		element.setAttribute('aria-label', 'Looped');
		sendIdValue("loop", "ON");
	} else {
		element.classList.remove('looped');
		element.setAttribute('aria-label', 'Loop');
		sendIdValue("loop", "OFF");
	}
}

function changeVolume(element) {
	console.log('changeVolume');
	var value = element.value;
	console.log('value=' + value);
	sendIdValue("volume", value);
}

function sendId(name) {
	console.log('sendId name=[%s]', name);
	var data = {};
	data["id"] = name;
	console.log('data=', data);
	json_data = JSON.stringify(data);
	console.log('json_data=' + json_data);
	websocket.send(json_data);
}

function sendIdValue(name, value) {
	console.log('sendIdValue name=[%s] value=[%s]', name, value);
	var data = {};
	data["id"] = name;
	data["value"] = value;
	console.log('data=', data);
	json_data = JSON.stringify(data);
	console.log('json_data=' + json_data);
	websocket.send(json_data);
}

websocket.onopen = function(evt) {
	console.log('WebSocket connection opened');
	var data = {};
	data["id"] = "init";
	console.log('data=', data);
	json_data = JSON.stringify(data);
	console.log('json_data=' + json_data);
	websocket.send(json_data);
}

websocket.onmessage = function(evt) {
	var msg = evt.data;
	console.log("msg=" + msg);
	var values = msg.split('\4'); // \4 is EOT
	//console.log("values=" + values);
	switch(values[0]) {
		case 'LIST':
			console.log("values[1]=" + values[1]);
			var songs = parseInt(values[1], 10);
			console.log("songs=" + songs);
			var list = document.getElementById("track-list");
			console.log(list);
			for(i=0;i<songs;i++) {
				var li = document.createElement('li');
				li.innerHTML = sprintf("%04d.mp3",i);
				list.appendChild(li);
			}
			break;

		case 'CURRENT':
			console.log("values[1]=" + values[1]);
			var current = parseInt(values[1], 10);
			console.log("current=" + current);

			// Set playing-title
			var title = document.getElementById("playing-title");
			console.log(title);
			title.textContent = sprintf("%04d.mp3",current-1);

			// Clear styles from all elements
			var list = document.getElementById("track-list");
			console.log(list);
			listElement = list.getElementsByTagName('li');
			console.log("listElement.length=" + listElement.length);
			for (var j=0; j<listElement.length; j++) {
				//console.log("textContent=" + listElement[j].textContent);
				//console.log("style.color=" + listElement[j].style.color);
				//console.log("style.backgroundColor=" + listElement[j].style.backgroundColor);
				listElement[j].style = null;
			}

			// Set styles to current elements
			listElement[current-1].style.color = 'rgb(255, 0, 0)';
			break;

		case 'FINISH':
			var toggleBtn = document.getElementById("play-btn");
			toggleBtn.classList.remove('playing');
			toggleBtn.setAttribute('aria-label', 'Play');
			break;

		default:
			break;
	}
}

websocket.onclose = function(evt) {
	console.log('Websocket connection closed');
	//document.getElementById("status").innerHTML = "WebSocket closed";
}

websocket.onerror = function(evt) {
	console.log('Websocket error: ' + evt);
	//document.getElementById("status").innerHTML = "WebSocket error";
}
