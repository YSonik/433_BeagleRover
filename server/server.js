// Import necessary modules
const http = require('http');
const fs = require('fs');
const path = require('path');
const mime = require('mime');

// Define the port to listen on
const PORT = 8088;

// Helper function to send a 404 response
function sendNotFound(res) {
  res.writeHead(404, { 'Content-Type': 'text/plain' });
  res.end('404 Not Found');
}

// Helper function to send the requested file
function sendFileContent(res, filePath, fileData) {
  // Use mime.lookup for older versions of the mime package
  const contentType = mime.lookup(path.basename(filePath)) || 'application/octet-stream';
  res.writeHead(200, { 'Content-Type': contentType });
  res.end(fileData);
}

// Function to serve static files
function serveFile(req, res) {
  let filePath = 'public' + (req.url === '/' ? '/index.html' : req.url);
  let absolutePath = path.join(__dirname, filePath);

  fs.exists(absolutePath, exists => {
    if (!exists) {
      sendNotFound(res);
      return;
    }

    fs.readFile(absolutePath, (err, data) => {
      if (err) {
        sendNotFound(res);
      } else {
        sendFileContent(res, absolutePath, data);
      }
    });
  });
}

// Create and start the HTTP server
const server = http.createServer(serveFile);
server.listen(PORT, () => {
  console.log(`Server is running on port ${PORT}`);
});

// Placeholder for WebSocket server setup
const udpServer = require('./lib/udp_server');
udpServer.listen(server);

