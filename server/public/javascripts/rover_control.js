"use strict"
const socket = io()
    
//Drive Rover Event Emitters.
function drive_rover(direction)
{
    console.log(direction);
    if(direction === 'forward')
    {
        socket.emit("drive_forward",{eventType: "drive_forward"});
    }
    else if(direction === 'backward')
    {
        socket.emit("drive_backward",{eventType: "drive_backward"});
    }
    else if(direction === 'left')
    {
        socket.emit("drive_left",{eventType: "drive_left"});
    }
    else if(direction === 'right')
    {
        socket.emit("drive_right",{eventType: "drive_right"});
    }
}









