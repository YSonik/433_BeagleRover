"use strict"
const socket = io()

//Drive Rover Event Emitters.
function drive_rover(direction) {
    console.log(direction);
    if (direction === 'forward') {
        socket.emit("drive_forward", () => {
            console.log("Drive Forward Event Emitted");
        });
    }
    else if (direction === 'backward') {
        socket.emit("drive_backward", () => {
            console.log("Drive Backward Event Emitted");
        });
    }
    else if (direction === 'left') {
        socket.emit("drive_left", () => {
            console.log("Drive Left Event Emitted");
        });
    }
    else if (direction === 'right') {
        socket.emit("drive_right", () => {
            console.log("Drive Right Event Emitted");
        });
    } else if (direction === 'stop') {
        socket.emit("drive_stop", () => {
            console.log("Drive Stop Event Emitted");
        });
    }
}

//Set Speed Event Emitter
function set_speed(speed) {
    console.log(speed);
    socket.emit("set_speed", speed, () => {
        console.log("Set Speed Event Emitted");
    });
}

const change_speed = () => {

    const speed = document.getElementById("speed_slider").value;

    console.log(speed);
    set_speed(speed);
}

const autodrive = () => {
    socket.emit("start_autodrive", () => {
        console.log("Autodrive Event Emitted");
    });
}

function updateFields() {
    //Get ultrasonic distance reading
    socket.emit("get_uls_reading", (result) => {
        document.getElementById("uls_reading").innerHTML = `${result}`;
    });

    //Get gyroscopr reading
    socket.emit("get_gys_reading", (result) => {
        document.getElementById("gys_reading").innerHTML = `${result}`;
    });

    //Get speed reading
    socket.emit("get_speed", (result) => {
        document.getElementById("speed_reading").innerHTML = `${result}`;
    });
}

$(document).ready(() => {
    //Every 1000ms, update the value of the Volume, Tempo, and Process_Uptime
    window.setInterval(updateFields, 1000);
})






