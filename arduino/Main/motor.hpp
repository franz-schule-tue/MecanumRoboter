/**
 * Klasse Motor für die Ansteuerung eines Motors
 */


class Motor
{
public:
    const int PWM_FREQ = 1000;
    const int PWM_RES  = 8;

    
    /**
     * Konstruktor
     */
    Motor( int pinEnable, int pinPhase /*, int pwmChannel*/ )
    {
        m_pinEnable  = pinEnable;
        m_pinPhase   = pinPhase;
        //m_pwmChannel = pwmChannel;
        
        pinMode( pinPhase, OUTPUT );
        digitalWrite( pinPhase, LOW );             // LOW: vorwärts, HIGH: rückwärts

        ledcAttach( pinEnable, PWM_FREQ, PWM_RES );

        setPower( 0 );
    }


    /**
     * Setzt die Motorleistung
     * @param value     -100..100; höhere Werte werden abgeschnitten
     */
    void setPower( int value )
    {
        value = saturate( value );
        
        if ( value < 0 )
        {
            // rückwärts
            digitalWrite( m_pinPhase, HIGH );             // LOW: vorwärts, HIGH: rückwärts
    
            // PWM andersrum. Achtung: value ist negativ
            ledcWrite( m_pinEnable, (-value) * 255 / 100 );
        }
        else
        {
            // vorwärts
            digitalWrite( m_pinPhase, LOW );             // LOW: vorwärts, HIGH: rückwärts
    
            // PWM richtigrum
            ledcWrite( m_pinEnable, value * 255 / 100 );
        }
    }


private:
    static int saturate( int value )
    {
        int res = value;
           
        if ( value < -100 )
        {
            res = -100;
        }
        else if ( value > 100 )
        {
            res = 100;
        }
            
        return res;
    }


private:

    int m_pinEnable;
    int m_pinPhase;
    //int m_pwmChannel;
 };
