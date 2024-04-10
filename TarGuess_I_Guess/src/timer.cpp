/*
 * timer.cpp
 */
#include "timer.h"

time_t CTimer::start_time = time(NULL);

CTimer::CTimer() :
	m_Elapse(0), m_hThread(NULL) {
}

CTimer::~CTimer() {
	EndTimer();

	log << "{" << "\"task_status\":" << "\"finish\"" << ",\"password_num\":"
			<< CTimer::psw_guess_num << ",\"time_cost\":" << CTimer::time_cost
			<< 's' << ",\"speed\":" << CTimer::psw_guess_num
			/ CTimer::time_cost << "guesses/s}" << endl;
}

void CTimer::StartTimer(float nElapse) {
	m_Elapse = nElapse;
	m_hThread
			= CreateThread(NULL, 0, ThreadFunc, (LPVOID) (&m_Elapse), 0, NULL);
	CTimer::start_time = time(NULL);
}

void CTimer::EndTimer() {
	CloseHandle(m_hThread);
}

DWORD WINAPI CTimer::ThreadFunc(LPVOID pParam) {

	time_t t1, t2;
	float Diff = 0;
	float elapse = *((float *) pParam);

	/* Obtain the current system time */
	t1 = time(NULL);
	while (1) {
		/* Obtain the current system time in seconds */
		t2 = time(NULL);
		Diff = difftime(t2, t1);

		/* Print logs with elapse intervals */
		if (Diff == elapse) {
			time_cost = difftime(t2, start_time);
			//float trained_portion=(float)trained_num/tot_num;
			log << "{\"password_num\":";
			if (predict_guess_num == -1)
				log << "disabled";
			else if (predict_guess_num == -2)
				log << "timeout";
			else
				log << predict_guess_num;
			log << ",\"maximum_guess_num\":" << max_guess_num
					<< ",\"password_guessed_num\":" << psw_guess_num
					<< ",\"start_time\":" << start_time << ",\"guess_speed\":"
					<< psw_guess_num / time_cost << "guesses/s}"
					<< ",\"time_consumed\":" << time_cost << 's'
					<< ",\"current_guess\":";
			log << "{" << "\"reg\":" << pat << ",\"password\":" << cur
					<< ",\"prob\":" << prob << "}";
			log << ",\"estimated_time\":" << time_cost / psw_guess_num
					* max_guess_num - difftime(t2,last_time)<< "s for this account" << ",\"task_status\":"
					<< status << "}" << endl;

			t1 = t2;
		}
	}
	return 0;
}
