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


function updateFields()
{
    //Get ultrasonic distance reading
    //socket.emit("get_uls_reading",{eventType: "get_uls_reading"});

    //Get gyroscopr reading
    socket.emit("get_gys_reading",{eventType: "get_gys_reading"});
}

$(document).ready(()=>
{
    //Every 1000ms, update the value of the Volume, Tempo, and Process_Uptime
    window.setInterval(updateFields, 10);
})

socket.on("gyro_updated", (result)=>{
    
    document.getElementById("gys_reading").value = `${result.new_gyro}`;
})







