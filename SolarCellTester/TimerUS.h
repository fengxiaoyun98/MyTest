#pragma once

class TimerUS
{
    LARGE_INTEGER tim1, freq,tim2;
public:
	void Reset(void){QueryPerformanceCounter(&tim1);}
	bool IsLost(double delayus){//usΪ��λ
		delayus/=1000000;//ת����s��λ
		return GetLost()>delayus;
	}
	double GetLost(void){//usΪ��λ
		double seconds;
		QueryPerformanceCounter(&tim2);
		seconds = (double)(tim2.QuadPart-tim1.QuadPart) /(double)freq.QuadPart;
		return seconds;
	}
	TimerUS(void){QueryPerformanceFrequency(&freq);Reset();}
	~TimerUS(void){};
};
