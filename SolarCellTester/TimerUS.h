#pragma once

class TimerUS
{
    LARGE_INTEGER tim1, freq,tim2;
public:
	void Reset(void){QueryPerformanceCounter(&tim1);}
	bool IsLost(double delayus){//us为单位
		delayus/=1000000;//转换成s单位
		return GetLost()>delayus;
	}
	double GetLost(void){//us为单位
		double seconds;
		QueryPerformanceCounter(&tim2);
		seconds = (double)(tim2.QuadPart-tim1.QuadPart) /(double)freq.QuadPart;
		return seconds;
	}
	TimerUS(void){QueryPerformanceFrequency(&freq);Reset();}
	~TimerUS(void){};
};
