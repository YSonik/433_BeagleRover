//"use strict"
const path = require('path');
var fs = require('fs')
var http = require('http')
var mime = require('mime-types')
var socketIO = require('socket.io')

const dgram = require('dgram');
const { removeListener } = require('process');
const client = dgram.createSocket('udp4')
client.bind(8090, '0.0.0.0',()=>{
        console.log("Node Server is listening to the C App\n");
})
client.setMaxListeners(4)
let event_list = {"drive_forward":false,
                  "drive_backward":false,
                  "drive_left":false,
                  "drive_right":false,}

const app_ip = '172.20.10.13' //(Supreet's Hotspot)May Change
const app_port = 12345


function sendFile(response, filepath, data)
{
    response.writeHead(200,{
        "content-type":mime.lookup(path.basename(filepath))
    });

    response.end(data);
}

function serveStatic(response, filepath)
{
    if(fs.existsSync(filepath))
    {
        fs.readFile(filepath, (err, data)=>{
    
            if(err){
                console.log("ERROR: server couldn't read file.\n")
            }
            else
            {
                sendFile(response, filepath, data)
            }
    
        })
    }
   
}

var server = http.createServer((request, response)=>{
    var filePath = false;
    if(request.url == '/')
    {
        filePath = './public/index.html'
    }
    else
    {
        filePath = './public'+request.url
    }

    serveStatic(response, filePath);
})


function addListener(socket, event, listener)
{   
    console.log(event_list[event])
    if(event_list[event] == false)
    {
        console.log("Added listener\n")
        socket.on(event, listener);
        event_list[event] = true;
    }
}

function drive_forward(socket)
{
    client.send("direction=0", app_port, app_ip, (err)=>{
        if(err){
            console.log("UDP Error\n");
        }
    })
}

function drive_backward(socket)
{
    client.send("direction=1", app_port, app_ip, (err)=>{
        if(err){
            console.log("UDP Error\n");
        }
    })
}

function drive_left(socket)
{
    client.send("direction=2", app_port, app_ip, (err)=>{
        if(err){
            console.log("UDP Error\n");
        }
    })
}

function drive_right(socket)
{
    client.send("direction=3", app_port, app_ip, (err)=>{
        if(err){
            console.log("UDP Error\n");
        }
    })
}


const io = socketIO(server)
io.on("connection", (socket)=>{
    console.log("Server connected to Frontend\n");
    //Drive Rover event listeners
    addListener(socket, "drive_forward", drive_forward)

    addListener(socket, "drive_backward", drive_backward)

    addListener(socket, "drive_left", drive_left)

    addListener(socket, "drive_right", drive_right)

});   
    
    

const server_port = 3000
const server_ip = '0.0.0.0'
server.listen(server_port,server_ip, ()=>{console.log("Server is listening on port: "+server_port)})