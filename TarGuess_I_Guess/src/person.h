/*
 * person.h
 */

#ifndef PERSON_H_
#define PERSON_H_
#include <iostream>

using namespace std;
class person {
public:
	// E,N,B,A,C
	string email;
	string name;
	string birth;
	string account;
	string phone;
	string psw;
	string gid;
	// Find str in psw and replace it with label c at the corresponding position in pat_str
	void findStr(string& pat_str, string psw, string str, char c);
	// Searching for phone and its deformation information in PSW
	void processPhone(string& pat_str, string psw);
	// Search for name and its deformation information in PSW, with similar functions below
	void processName(string& pat_str, string psw);
	void processBirth(string& pat_str, string psw);
	void processEmail(string& pat_str, string psw);
	void processAccount(string& pat_str, string psw);
	void processGid(string& pat_str, string psw);
	person();
	~person();

};

class guess {
public:
	string pat;
	string psw;
	int pivot;
	float prob;
	guess() {
		psw = "";
		pivot = 0;
	}
	bool operator <(const guess &b) const{
		return (b.prob - prob > 0.000000001);
	}
};


#endif /* PERSON_H_ */
