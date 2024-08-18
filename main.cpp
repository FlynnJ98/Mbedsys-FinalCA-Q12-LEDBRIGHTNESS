/* 
"Write a program to control the LED brightness using the 3 axis accelerometer. Moving the board in x-axis changes the brightness on LED1, y-axis changes the brightness on LED2 
and z-axis changes the brightness on LED3."

The program starts by defining the PWM period as well as max and min values for the led's. 
Logic was then applied to ensure the accelerometer data was within the range set. 
The program starts with all values set to zero, and gradually increases the brightness of the LED when the board is moved. The ACC data from each access as well as the brightness from 
0.00 to 1.00 is printed on the serial monitor to allow the user to see the changes. 
*/

#include "mbed.h"
#include "LSM6DSLSensor.h"

// Define PWM period and Brightness range 
#define PWM_PERIOD 0.020f // 20ms period
#define MIN_PWM 0.0f // Min PWM value (LED off)
#define MAX_PWM 1.0f // Max PWM value (LED on)

static DevI2C devI2c(PB_11, PB_10);
static LSM6DSLSensor acc_gyro(&devI2c, 0xD4, D4, D5); 

PwmOut led1(PA_5); // Defining LED pins as PWMout
PwmOut led2(PB_14);
PwmOut led3(PC_9);

const float ACC_MIN = 0.00f; // setting the range for the accelerometer
const float ACC_MAX = 999.00f;

float ACC_PWM(int acc_gyro) {

    if (acc_gyro < ACC_MIN) acc_gyro = ACC_MIN; // apply the accelerometer value within that range 
        if (acc_gyro > ACC_MAX) acc_gyro = ACC_MAX; // if below the range, set to min and if above set to max

float PWM_Value = (acc_gyro - ACC_MIN) / (ACC_MAX - ACC_MIN); //Normalises the value to a range between 0-1
    PWM_Value = MIN_PWM + PWM_Value * (MAX_PWM - MIN_PWM); // Pulse width Modulation value = 0.0 + PWM value x 1.0 - 0.0

    if (PWM_Value < MIN_PWM) PWM_Value = MIN_PWM; // logic to see if PWM value is less than min. if so set to min
        if (PWM_Value > MAX_PWM) PWM_Value = MAX_PWM; // logic to see if PWM value is greater than max. if so set to max.

    return PWM_Value; // returns the value to control the brightness
}

int main() {
    int32_t axes[3];
    uint8_t id;
    acc_gyro.init(NULL); // initialising the gyroscope. 
    acc_gyro.enable_x();
    acc_gyro.enable_g();

 // Set PWM period
    led1.period(PWM_PERIOD); // set value in the defined section at the start of this code 
    led2.period(PWM_PERIOD);
    led3.period(PWM_PERIOD);

    printf("LED brightness control based on accelerometer data:\n"); // Print this statement on start of the program. 
    while (1) { // while value is 1, execute what is within this loop

        acc_gyro.get_x_axes(axes); // gets the gyroscope to find the axes values

// LED brightness equals the acc_PWM x axes[]
        float brightness_led1 = ACC_PWM(axes[0]);
        float brightness_led2 = ACC_PWM(axes[1]);
        float brightness_led3 = ACC_PWM(axes[1]);
// write a high or low value to the led. This will be based on brightness_led() above.
        led1.write(brightness_led1);
        led2.write(brightness_led2);
        led3.write(brightness_led3);

// Print values for acc data and the intensity of brightness on the serial monitor 
        printf("AccX: %6d, AccY: %6d, AccZ: %6d\r\n", axes[0], axes[1], axes[2]);
        printf("B-LED1: %f, B-LED2: %f, B-LED3: %f,\r\n", brightness_led1, brightness_led2, brightness_led3); //B-LED stands for Brightness LED
        ThisThread::sleep_for(500ms);
}
}