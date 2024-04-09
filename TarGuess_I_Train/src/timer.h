/*
 * timer.h
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <Windows.h>
#include <time.h>
#include <map>
#include <iostream>
#include "person.h"

using namespace std;

class CTimer {
public:
    CTimer();
    ~CTimer();
    /* Print Information */
    static person & pi; // Personal Identifiable Information
    static int &trained_num; // Number of trained passwords
    static int &tot_num; // Total Number
    static time_t start_time;
    static float time_cost;
    static string status;
    /* Print Information */
    
    void StartTimer(float nElapse);
    void EndTimer();
    
    static DWORD WINAPI ThreadFunc(LPVOID pParam);

private:
    float m_Elapse;
    HANDLE m_hThread;
};

#endif /* TIMER_H_ */
