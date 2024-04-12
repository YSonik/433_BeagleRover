
const socketio = require('socket.io');
const dgram = require('dgram');

// Info for connecting to the local process via UDP
var PORT = 12345;
var HOST = '127.0.0.1';

const drive_forward = async (callback) => {
    console.log("Drive Forward\n");

    const client = dgram.createSocket('udp4');
    client.send("direction=0", PORT, HOST, (err) => {
        if (err) {
            console.log("UDP Error\n");
        } else {
            callback();
        }
    })
}

const drive_backward = async (callback) => {
    console.log("Drive Backward\n");

    const client = dgram.createSocket('udp4');
    client.send("direction=1", PORT, HOST, (err) => {
        if (err) {
            console.log("UDP Error\n");
        }
        else {
            callback();
        }
    })
}

const drive_left = async (callback) => {

    console.log("Drive Left\n");

    const client = dgram.createSocket('udp4');
    client.send("direction=2", PORT, HOST, (err) => {
        if (err) {
            console.log("UDP Error\n");
        } else {
            callback();
        }
    })
}

const drive_right = async (callback) => {

    console.log("Drive Right");

    const client = dgram.createSocket('udp4');
    client.send("direction=3", PORT, HOST, (err) => {
        if (err) {
            console.log("UDP Error\n");
        } else {
            callback();
        }
    })
}

const drive_stop = async (callback) => {

    console.log("Drive None");

    const client = dgram.createSocket('udp4');
    client.send("direction=4", PORT, HOST, (err) => {
        if (err) {
            console.log("UDP Error\n");
        } else {
            callback();
        }
    });
}

const set_speed = async (speed, callback) => {

    const client = dgram.createSocket('udp4');
    client.send(`speed=${speed}`, PORT, HOST, (err) => {
        if (err) {
            console.log("UDP Error\n");
        } else {
            console.log("Speed set to: " + speed);
            callback();
        }
    })
}

exports.listen = (server) => {
    const io = socketio(server);

    const read_gyro = async (callback) => {
        const client = dgram.createSocket('udp4');

        client.send("gyro", PORT, HOST, (err) => {
            if (err) {
                console.log("UDP Error\n");
            }
            else {
                client.on('message', (msg) => {
                    console.log(msg.toString());
                    // io.emit("gyro_updated", { new_gyro: parseInt(msg.toString()) });

                    callback(parseInt(msg.toString()));
                })
            }
        })
    }

    const read_uls = async (callback) => {
        const client = dgram.createSocket('udp4');
        client.send("distance", PORT, HOST, (err) => {
            if (err) {
                console.log("UDP Error\n");
            }
            else {
                client.on('message', (msg) => {
                    console.log("Distance: " + msg.toString());

                    callback(parseInt(msg.toString()));
                    // io.emit("uls_updated", { new_uls: parseInt(msg.toString()) });

                })
            }
        })
    }

    const read_speed = async (callback) => {
        const client = dgram.createSocket('udp4');
        client.send("speed", PORT, HOST, (err) => {
            if (err) {
                console.log("UDP Error\n");
            }
            else {
                client.on('message', (msg) => {
                    console.log(msg.toString());
                    callback(parseInt(msg.toString()));
                    // io.emit("speed", { new_speed: parseInt(msg.toString()) });
                })
            }
        })
    }

    const start_autodrive = async (callback) => {
        const client = dgram.createSocket('udp4');
        client.send("autodrive", PORT, HOST, (err) => {
            if (err) {
                console.log("UDP Error\n");
            }
            else {
                callback();
            }
        })
    }

    const stop_autodrive = async (callback) => {
        const client = dgram.createSocket('udp4');
        client.send("manualdrive", PORT, HOST, (err) => {
            if (err) {
                console.log("UDP Error\n");
            }
            else {
                callback();
            }
        })
    }

    io.on('connection', (socket) => {
        console.log("Server connected to Frontend\n");

        //Drive Rover event listeners
        // addListener(socket, "drive_forward", drive_forward);
        // addListener(socket, "drive_backward", drive_backward);
        // addListener(socket, "drive_left", drive_left);
        // addListener(socket, "drive_right", drive_right);

        // addListener(socket, "get_gys_reading", read_gyro);
        // addListener(socket, "get_uls_reading", read_uls);

        // addListener(socket, "get_speed", read_speed);
        // addListener(socket, "set_speed", set_speed);

        socket.on("drive_forward", drive_forward);
        socket.on("drive_backward", drive_backward);
        socket.on("drive_left", drive_left);
        socket.on("drive_right", drive_right);
        socket.on("drive_stop", drive_stop);

        socket.on("get_gys_reading", read_gyro);
        socket.on("get_uls_reading", read_uls);

        socket.on("get_speed", read_speed);
        socket.on("set_speed", set_speed);

        socket.on("start_autodrive", start_autodrive);
        socket.on("stop_autodrive", stop_autodrive);
    });
};