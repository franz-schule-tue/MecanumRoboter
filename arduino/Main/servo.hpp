/**
 * Klasse Servo für die Ansteuerung eines Servos
 */


class Servo
{
public:
    Servo( int pin, int initDeciPercent = 0 )
    {
        m_pin = pin;

        ledcAttach( pin, PWM_FREQUENCY, PWM_RESOLUTION );
        setDeciPercent( initDeciPercent );
    }


    void setPercent( int value )
    {
        ledcWrite( m_pin, saturate( value ) * ( PWM_VALUE_MAX - PWM_VALUE_MIN ) / 100 + PWM_VALUE_MIN );
    }    

    void setDeciPercent( int value )
    {
        ledcWrite( m_pin, saturateDeci( value ) * ( PWM_VALUE_MAX - PWM_VALUE_MIN ) / 1000 + PWM_VALUE_MIN );
    }    


private:
    const int PWM_VALUE_MIN     = 3277;
    const int PWM_VALUE_MAX     = 6554;
    const int PWM_FREQUENCY     = 50;
    const int PWM_RESOLUTION    = 16;      // 16 Bits, d.h. Impuls-Breite von 0 bis 65535 Bits. Zulässige Werte für Servos sind 1ms (3277) bis 2ms (6554)

    int saturate( int value )
    {
        int res = value;

        if ( value < 0 )
        {
            res = 0;
        }
        else if ( value > 100 )
        {
            res = 100;
        }

        return res;
    }

    int saturateDeci( int value )
    {
        int res = value;

        if ( value < 0 )
        {
            res = 0;
        }
        else if ( value > 1000 )
        {
            res = 1000;
        }

        return res;
    }


    int m_pin;
};
