/*
 * person.h
 * Define functions about personal information
 */

#ifndef PERSON_H_
#define PERSON_H_
#include <iostream>

using namespace std;
class person {
public:
	// E,N,B,A,C,G
	string email;
	string name;
	string birth;
	string account;
	string phone;
	string psw;
	string gid;
	// Search for strings related to personal information in PSW 
	// and replace them with the corresponding label at the corresponding position in pat str
	void findStr(string& pat_str, string psw, string str, char c);
	// Process the user's phone number, generate several phone number deformations and corresponding labels
	void processPhone(string& pat_str, string psw);
	// Functionally similar, processing names
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
	bool operator <(const guess &b)const {
		return (b.prob - prob > 0.000000001);
	}
};
#endif /* PERSON_H_ */