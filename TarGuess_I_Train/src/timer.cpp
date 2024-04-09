#include "timer.h"

time_t CTimer::start_time=time(NULL);

CTimer::CTimer(): m_Elapse(0), m_hThread(NULL) {}

CTimer::~CTimer() {
    EndTimer();
    
    cout << "{" << 
            "\"task_status\":" << "\"finish\"" <<
            ",\"record_num\":" << CTimer::tot_num <<
            ",\"time_cost\":" << CTimer::time_cost << 's' <<
            ",\"speed\":" << CTimer::tot_num / CTimer::time_cost << "psws/s" <<
            "}" << endl;
}

void CTimer::StartTimer(float nElapse) {
    m_Elapse = nElapse;
    m_hThread = CreateThread(NULL, 0, ThreadFunc, (LPVOID) (&m_Elapse), 0, NULL);
    CTimer::start_time = time(NULL);
}

void CTimer::EndTimer() {
    CloseHandle(m_hThread);
}

DWORD WINAPI CTimer::ThreadFunc(LPVOID pParam) {
    time_t t1, t2;
    float Diff = 0;
    float elapse = *((float *) pParam);
    
    /* Get current system time. */
    t1 = time(NULL);
    while (1) {
        /* Get the current system time in seconds. */
        t2 = time(NULL);
        Diff = difftime(t2, t1);

		/* Print log at intervals of elapsed time. */
        if (Diff == elapse) {
            time_cost = difftime(t2,start_time);
            float trained_portion = (float)trained_num / tot_num;
            cout << "{\"start_time\":" << start_time <<
                    ",\"training_speed\":" << trained_num / time_cost << " psws/s" <<
                    ",\"training_progress\":" << trained_portion * 100 << '%' <<
                    ",\"time_consumed\":" << time_cost << 's' <<
                    ",\"current_record\":";
            
            cout << "{" << "\"name\":" << pi.name <<
                    ",\"birthday\":" << pi.birth <<
                    ",\"phone_number\":" << pi.phone <<
                    ",\"id_card\":" << pi.gid <<
                    ",\"username\":" << pi.account <<
                    ",\"email\":" << pi.email <<
                    ",\"psw\":" << pi.psw <<
                    "}";

			cout << ",\"estimated_time\":" <<time_cost / trained_portion << 's' <<
                    ",\"task_status\":" << status <<
                    "}" << endl;
            t1 = t2;
        }
    }
    return 0;
}