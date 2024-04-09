"use strict";
// Client-side interactions with the browser.

// Make connection to server when web page is fully loaded.
var socket = io.connect();
$(document).ready(function () {

	$('#modeNone').click(function () {
		sendCommandViaUDP("None");
	});
	$('#modeRock1').click(function () {
		sendCommandViaUDP("Rock1");
	});
	$('#modeRock2').click(function () {
		sendCommandViaUDP("Rock2");
	});
	$('#volumeDown').click(function () {
		sendCommandViaUDP("volumeDown");
	});
	$('#volumeUp').click(function () {
		sendCommandViaUDP("volumeUp");
	});
	$('#tempoDown').click(function () {
		sendCommandViaUDP("tempoDown");
	});
	$('#tempoUp').click(function () {
		sendCommandViaUDP("tempoUp");
	});
	$('#hihat').click(function () {
		sendCommandViaUDP("HiHat");
	});
	$('#snare').click(function () {
		sendCommandViaUDP("Snare");
	});
	$('#base').click(function () {
		sendCommandViaUDP("Base");
	});
	$('#stop').click(function () {
		sendCommandViaUDP("terminate");
	});
	sendCommandViaUDP('volumeValue');

	sendCommandViaUDP('bpmValue');

	setInterval(() => {
		sendCommandViaUDP("uptime");
	}, 1000);
	setInterval(() => {
		sendCommandViaUDP("mode");
	}, 1000);



});

function sendCommandViaUDP(message) {
	socket.emit('fcn', message);
};
