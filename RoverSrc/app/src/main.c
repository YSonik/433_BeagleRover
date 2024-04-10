#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../../hal/include/hal/accelerometer.h"
#include "../../hal/include/hal/gyroscope.h"
#include <time.h>


#include "utils/time_utils.h"
#include "hal/dfrobot_pirate.h"

int main()
{
    printf("Rover project compiles correctly\n");
    Gyroscope_init();

    int16_t xAcclValue;

    while (1)
    {
        Gyroscope_getAngle(&xAcclValue);
        float xAcclValue_float = xAcclValue / 1000.0f;
        printf("xGyroValue: %.3f\n", xAcclValue_float);
    }
    

    // Accel_init();

    // int16_t xAcclValue, yAcclValue;

    // getAccelerometerValues(&xAcclValue, &yAcclValue);

    // while (1)
    // {
    //     getAccelerometerValues(&xAcclValue, &yAcclValue);
    //     float xAcclValue_float = xAcclValue / 1000.0f;
    // float yAcclValue_float = yAcclValue / 1000.0f;

    // // Print accelerometer values
    // printf("xAcclValue: %.3f\n", xAcclValue_float);
    // printf("yAcclValue: %.3f\n", yAcclValue_float);
    // }

    return 0;
}


// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h> // For memset
// #include <unistd.h> // For close()
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>

// #include "../../hal/include/hal/gyroscope.h"

// #define SERVER_PORT 41234 // The port on which to send data
// #define SERVER_IP "127.0.0.1" // Assuming the server is running on localhost

// int main() {
//     int sockfd;
//     struct sockaddr_in servaddr;

//     // Creating socket file descriptor
//     if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
//         perror("Socket creation failed");
//         exit(EXIT_FAILURE);
//     }

//     memset(&servaddr, 0, sizeof(servaddr));

//     // Filling server information
//     servaddr.sin_family = AF_INET;
//     servaddr.sin_port = htons(SERVER_PORT);
//     servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);

//     Gyroscope_init();

//     int16_t xAcclValue;

//     char buffer[1024];
//     while (1) {
//         Gyroscope_getAngle(&xAcclValue);
//         float xAcclValue_float = xAcclValue / 1000.0f;
//         snprintf(buffer, sizeof(buffer), "xGyroValue: %.3f\n", xAcclValue_float);
//         printf("%s", buffer); // Also print to the console

//         // Send the message to the server
//         sendto(sockfd, (const char *)buffer, strlen(buffer), 0, (const struct sockaddr *) &servaddr, sizeof(servaddr));
        
//         sleep(1); // Sleep for a second before sending the next value
//     }

//     close(sockfd);
//     return 0;
// }
