/**
 * Klasse Motor für die Ansteuerung eines Motors
 */


class Motor
{
public:
    const int PWM_FREQ = 1000;
    const int PWM_RES  = 8;
    const int RAMP_TICK_PERIOD_MS = 20;
    const int RAMP_STEP_PERCENT = 10;

    
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

        setPowerDirect( 0 );
    }


    /**
     * Setzt die Motorleistung
     * @param value     -100..100; höhere Werte werden abgeschnitten
     */
    void setPowerDirect( int value )
    {
        value = saturate( value );
        m_powerCurr = value;
        
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


    void setPower( int value )
    {
        value = saturate( value );
        m_powerTarget = value;
        m_rampStartMs = 0;
    }


    void tick()
    {
        unsigned long now = millis();

        if ( now >= ( m_rampStartMs + RAMP_TICK_PERIOD_MS) )
        {
            m_rampStartMs = now;
            int diff = m_powerTarget - m_powerCurr;

            if ( diff > 0 )
            {
                // wir müssen vorwärts schneller werden
                if ( diff >= RAMP_STEP_PERCENT )
                {
                    setPowerDirect( m_powerCurr + RAMP_STEP_PERCENT );
                }
                else
                {
                    setPowerDirect( m_powerTarget );
                }
            }
            else
            {
                // wir müssen rückwärts schneller werden
                if ( -diff >= RAMP_STEP_PERCENT )
                {
                    setPowerDirect( m_powerCurr - RAMP_STEP_PERCENT );
                }
                else
                {
                    setPowerDirect( m_powerTarget );
                }
            }
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
    int m_powerTarget;
    int m_powerCurr;
    unsigned long m_rampStartMs;
 };
