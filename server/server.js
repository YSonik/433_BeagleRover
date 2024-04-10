const express = require('express');
const path = require('path');
const dgram = require('dgram');
const app = express();
const udpClient = dgram.createSocket('udp4');

const UDP_PORT = 41234;
const UDP_HOST = 'localhost';
const HTTP_PORT = 3000;

app.use(express.static('public')); // Serve static files

app.get('/gyroscope', (req, res) => {
    udpClient.send('gyroscope', UDP_PORT, UDP_HOST, (error) => {
        if (error) res.sendStatus(500);
        else console.log('Requested gyroscope data');
    });

    udpClient.on('message', (message) => {
        res.json({data: message.toString()});
    });
});

app.get('/ultrasonic', (req, res) => {
    udpClient.send('ultrasonic', UDP_PORT, UDP_HOST, (error) => {
        if (error) res.sendStatus(500);
        else console.log('Requested ultrasonic data');
    });

    udpClient.on('message', (message) => {
        res.json({data: message.toString()});
    });
});

app.listen(HTTP_PORT, () => {
    console.log(`HTTP Server running on port ${HTTP_PORT}`);
});
