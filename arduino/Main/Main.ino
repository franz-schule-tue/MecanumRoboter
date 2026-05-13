/**
 * Projekt: MecanumRoboter
 *
 * Board: ESP32 Dev Module
 */

#include <Wire.h>
#include <BluetoothSerial.h>

#include "motor.hpp"
#include "servo.hpp"
//#include "us_sensor.hpp"

// Definition der Mikrocontroller-Pins
#define PIN_MOTOR_ENABLE_FRONT_LEFT     21
#define PIN_MOTOR_ENABLE_FRONT_RIGHT    26
#define PIN_MOTOR_ENABLE_REAR_LEFT      23
#define PIN_MOTOR_ENABLE_REAR_RIGHT     33
#define PIN_MOTOR_PHASE_FRONT_LEFT      19
#define PIN_MOTOR_PHASE_FRONT_RIGHT     25
#define PIN_MOTOR_PHASE_REAR_LEFT       22
#define PIN_MOTOR_PHASE_REAR_RIGHT      32
#define PIN_LED                         2
#define PIN_SERVO_1                     13
#define PIN_SERVO_2                     12
#define PIN_SERVO_3                     14
#define PIN_SERVO_4                     15
#define PIN_SERVO_5                     4
#define PIN_SERVO_6                     5


BluetoothSerial bt;
Motor           motorFrontLeft{  PIN_MOTOR_ENABLE_FRONT_LEFT,  PIN_MOTOR_PHASE_FRONT_LEFT /*,  PWM_CHAN_FRONT_LEFT*/  };
Motor           motorFrontRight{ PIN_MOTOR_ENABLE_FRONT_RIGHT, PIN_MOTOR_PHASE_FRONT_RIGHT /*, PWM_CHAN_FRONT_RIGHT*/ };
Motor           motorRearLeft{   PIN_MOTOR_ENABLE_REAR_LEFT,   PIN_MOTOR_PHASE_REAR_LEFT /*,   PWM_CHAN_REAR_LEFT */  };
Motor           motorRearRight{  PIN_MOTOR_ENABLE_REAR_RIGHT,  PIN_MOTOR_PHASE_REAR_RIGHT /*,  PWM_CHAN_REAR_RIGHT */  };
Servo           servo1{ PIN_SERVO_1 };
Servo           servo2{ PIN_SERVO_2 };
Servo           servo3{ PIN_SERVO_3 };
Servo           servo4{ PIN_SERVO_4 };
Servo           servo5{ PIN_SERVO_5 };
Servo           servo6{ PIN_SERVO_6 };

unsigned long aliveTime = 0;
unsigned long blinkTime = 0;
bool          blinkState = false;
unsigned long blinkPeriod = 1000;


volatile bool connected = false;


void stopMotors()
{
    motorFrontLeft.setPower( 0 );
    motorFrontRight.setPower( 0 );
    motorRearLeft.setPower( 0 );
    motorRearRight.setPower( 0 );
}


void onBluetoothEvent( esp_spp_cb_event_t event, esp_spp_cb_param_t *param )
{
    if (  event == ESP_SPP_SRV_OPEN_EVT )
    {
        // verbunden
        connected = true;
    }
    else if( event == ESP_SPP_CLOSE_EVT )
    {
        // unterbrochen 
        connected = false;
        stopMotors();
    }
}


void setup()
{
    pinMode( PIN_LED, OUTPUT );
    digitalWrite( PIN_LED, LOW );             // LOW: aus, HIGH: ein

    bt.register_callback( onBluetoothEvent );
    bt.begin( "MecanumRoboter" );

    // für Debug-Ausgaben:
    Serial.begin( 115200 );
    Serial.println( "Hallo!\r\n" );
}


void loop()
{
    unsigned long now = millis();

    if ( bt.available() )
    {
        String rd = bt.readStringUntil( ';' );
        if ( rd.startsWith( "MFL" ) )
        {
            motorFrontLeft.setPower( rd.substring( 3 ).toInt() );
        }
        else if ( rd.startsWith( "MFR" ) )
        {
            motorFrontRight.setPower( rd.substring( 3 ).toInt() );
        }
        else if ( rd.startsWith( "MRL" ) )
        {
            motorRearLeft.setPower( rd.substring( 3 ).toInt() );
        }
        else if ( rd.startsWith( "MRR" ) )
        {
            motorRearRight.setPower( rd.substring( 3 ).toInt() );
        }
        // TODO: generisch mit Array aus Servos
        else if ( rd.startsWith( "S1" ) )
        {
            servo1.setDeciPercent( rd.substring( 2 ).toInt() );
        }
        else if ( rd.startsWith( "S2" ) )
        {
            servo2.setPercent( rd.substring( 2 ).toInt() );
        }
        else if ( rd.startsWith( "S3" ) )
        {
            servo3.setPercent( rd.substring( 2 ).toInt() );
        }
        else if ( rd.startsWith( "S4" ) )
        {
            servo4.setPercent( rd.substring( 2 ).toInt() );
        }
        else if ( rd.startsWith( "S5" ) )
        {
            servo5.setPercent( rd.substring( 2 ).toInt() );
        }
        else if ( rd.startsWith( "S6" ) )
        {
            servo6.setPercent( rd.substring( 2 ).toInt() );
        }
        else if ( rd.startsWith( "AL" ) )
        {
            // Alive-Timer zurücksetzen
            aliveTime = now;
        }
    }

    if ( now > aliveTime + 1000 )
    {
        // es kam zu lange kein Alive-Signal, also Motoren aus
        stopMotors();
    }

    if ( now > blinkTime + blinkPeriod )
    {
        blinkTime = now;
        blinkPeriod = connected ? 100 : 500;
        blinkState = !blinkState;
        digitalWrite( PIN_LED, blinkState ? HIGH : LOW );
    }
    
    motorFrontLeft.tick();
    motorFrontRight.tick();
    motorRearLeft.tick();
    motorRearRight.tick();

}
