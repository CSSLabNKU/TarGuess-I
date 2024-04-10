/*
 * timer.h
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <Windows.h>
#include <time.h>
#include <map>
#include <iostream>
#include <fstream>
#include "person.h"

using namespace std;

class CTimer {
public:
	CTimer();
	~CTimer();
	////// print information
	static long long& predict_guess_num; // Expected maximum number of generated passes (per user)
	static int &psw_guess_num; // Number of generated appearances (for all users)
	static int & max_guess_num; // Maximum output passes (per user)
	static string& pat; //The pattern of the current guess
	static char* cur; // Current guess
	static float& prob; // The probability of the current guess
	static time_t start_time; // start time
	static time_t & last_time; // The time when the current user started generating the guess
	static float time_cost; // Time consumed
	static ofstream& log;
	static string status;
	//////
	void StartTimer(float nElapse);
	void EndTimer();

	static DWORD WINAPI ThreadFunc(LPVOID pParam);
private:
	float m_Elapse;
	HANDLE m_hThread;

};

#endif /* TIMER_H_ */
