//---------------------------------------
// Servo Loop Class
// A Proportional/Derivative feedback
// loop for pan/tilt servo tracking of
// blocks.
// (Based on Pixy CMUcam5 example code)
//---------------------------------------
// Declare tracking constants
#define X_CENTER    160L
#define Y_CENTER    100L
#define RCS_MIN_POS     0L
#define RCS_MAX_POS     1000L
#define RCS_CENTER_POS	((RCS_MAX_POS-RCS_MIN_POS)/2)

class ServoLoop
{
public:
	ServoLoop(int32_t proportionalGain, int32_t derivativeGain);
 
	void update(int32_t error);
 
	int32_t m_pos;
	int32_t m_prevError;
	int32_t m_proportionalGain;
	int32_t m_derivativeGain;
};
 
// ServoLoop Constructor
ServoLoop::ServoLoop(int32_t proportionalGain, int32_t derivativeGain)
{
	m_pos = RCS_CENTER_POS;
	m_proportionalGain = proportionalGain;
	m_derivativeGain = derivativeGain;
	m_prevError = 0x80000000L;
}
 
// ServoLoop Update 
// Calculates new output based on the measured
// error and the current state.
void ServoLoop::update(int32_t error)
{
	long int velocity;
	char buf[32];
	if (m_prevError!=0x80000000)
	{	
		velocity = (error*m_proportionalGain + (error - m_prevError)*m_derivativeGain)>>10;
 
		m_pos += velocity;
		if (m_pos>RCS_MAX_POS) 
		{
			m_pos = RCS_MAX_POS; 
		}
		else if (m_pos<RCS_MIN_POS) 
		{
			m_pos = RCS_MIN_POS;
		}
	}
	m_prevError = error;
}
// End Servo Loop Class
//---------------------------------------



