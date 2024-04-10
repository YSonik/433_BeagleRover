$(document).ready(function () {
    // Function to fetch and update gyroscope data
    function updateGyroscopeReading() {
        $.get('/gyroscope', function(data) {
            $('#gyroscope-reading').text(data.data);
        }).fail(function() {
            $('#gyroscope-reading').text('Error fetching data');
        });
    }

    // Function to fetch and update ultrasonic sensor data
    function updateUltrasonicSensorReading() {
        $.get('/ultrasonic', function(data) {
            $('#ultrasonic-sensor-reading').text(data.data);
        }).fail(function() {
            $('#ultrasonic-sensor-reading').text('Error fetching data');
        });
    }

    // Update sensor data every second
    setInterval(updateGyroscopeReading, 1000);
    setInterval(updateUltrasonicSensorReading, 1000);
});
