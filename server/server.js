//"use strict"
const path = require('path');
var fs = require('fs')
var http = require('http')
var mime = require('mime-types')

const SERVER_PORT = 3000

function sendFile(response, filepath, data) {
    response.writeHead(200, {
        "content-type": mime.lookup(path.basename(filepath))
    });

    response.end(data);
}

function serveStatic(response, filepath) {
    if (fs.existsSync(filepath)) {
        fs.readFile(filepath, (err, data) => {

            if (err) {
                console.log("ERROR: server couldn't read file.\n")
            }
            else {
                sendFile(response, filepath, data)
            }

        })
    }
}

var server = http.createServer((request, response) => {
    var filePath = false;
    if (request.url == '/') {
        filePath = './public/index.html'
    }
    else {
        filePath = './public' + request.url
    }

    serveStatic(response, filePath);
})

server.listen(SERVER_PORT, () => console.log("Server is listening on port: " + SERVER_PORT))

/*
 * Create the Proc-access server to listen for the websocket
 */
var procServer = require('./lib/udp_server');
procServer.listen(server);