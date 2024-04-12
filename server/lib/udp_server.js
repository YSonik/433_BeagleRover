const dgram = require('dgram');
const udpServer = dgram.createSocket('udp4');

const PORT = 41234;
const HOST = 'localhost';

udpServer.on('message', (message, remote) => {
    console.log(`UDP Server received: ${message} from ${remote.address}:${remote.port}`);

    // Simulate fetching data based on request
    let response;
    if(message.toString() === 'gyroscope') {
        response = `Gyroscope Data: ${Math.random().toFixed(2)}`;
    } else if(message.toString() === 'ultrasonic') {
        response = `Ultrasonic Sensor Distance: ${Math.random().toFixed(2)} cm`;
    }

    udpServer.send(response, remote.port, remote.address, (error) => {
        if (error) console.error(error);
        console.log('Sent response to ' + remote.address + ':' + remote.port);
    });
});

udpServer.bind(PORT, HOST);
console.log(`UDP Server listening on ${HOST}:${PORT}`);
