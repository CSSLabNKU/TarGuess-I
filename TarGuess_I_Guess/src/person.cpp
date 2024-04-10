/*
 * person.cpp
 */

#include "person.h"
#include <iostream>

using namespace std;

person::person(){
}

person::~person(){
}

// example name: zhang sanchuan 19940231
void person::findStr(string& pat_str, string psw, string str, char c) {
	unsigned int pos = psw.find(str);
	int len = str.length();
	string flag = "";
	for (int i = 0; i < len; i++)
		flag += c;
	if (pos != string::npos) {
		for (unsigned int i = pos; i < pos + len; i++) {
			if (pat_str[i] != 'P')
				return;
		}
		pat_str.replace(pos, len, flag);
	}
}

void person::processPhone(string& pat_str, string psw) {
	if (phone == "")
		return;
	findStr(pat_str, psw, phone, 'C');
}

void person::processName(string& pat_str, string psw) {
	if (name == "")
		return;
	string py = name; // zhang san chuan
	string name_str[20]; //zhang/san/chuan
	int name_len = 0;
	int py_len = py.length(), st = 0;
	for (int i = 0; i < py_len; i++) {
		if (py[i] == ' ') {
			if (py[i - 1] != '?')
				name_str[name_len++] = py.substr(st, i - st);
			st = i + 1;
		}
	}

	string tmp[5];
	for (int i = 0; i < name_len; i++) {
		tmp[0] += name_str[i][0];
		tmp[1] += name_str[i];
		if (i != 0)
			tmp[2] += name_str[i];
	}
	tmp[3] = name_str[0];
	tmp[3][0] -= 32;
	tmp[4] = tmp[0].substr(1, tmp[0].length() - 1); // sc

	findStr(pat_str, psw, tmp[1], 'N'); // zhangsanchuan
	findStr(pat_str, psw, tmp[2] + name_str[0], 'a'); // sanchuanzhang
	findStr(pat_str, psw, tmp[0], 'b'); // zsc
	findStr(pat_str, psw, tmp[2], 'c'); // sanchuan
	findStr(pat_str, psw, name_str[0], 'd'); // zhang
	findStr(pat_str, psw, tmp[3], 'e'); // Zhang

	findStr(pat_str, psw, tmp[4] + name_str[0], 'f'); // sczhang
	findStr(pat_str, psw, name_str[0] + tmp[4], 'g'); // zhangsc
}

void person::processBirth(string& pat_str, string psw) {
	if (birth == "" || birth.length() != 8)
		return;
	findStr(pat_str, psw, birth, 'B'); // 19940231
	if (birth[4] == '0')
		findStr(pat_str, psw, birth.substr(0, 4) + birth.substr(5, 3), 'h'); // 1994231
	if (birth[6] == '0')
		findStr(pat_str, psw, birth.substr(0, 6) + birth[7], 'i'); // 1994021
	if (birth[4] == '0' && birth[6] == '0')
		findStr(pat_str, psw, birth.substr(0, 4) + birth[5] + birth[7], 'j'); // 199421
	findStr(pat_str, psw, birth.substr(0, 6), 'k'); // 199402
	findStr(pat_str, psw, birth.substr(2, 6), 'l'); // 940231
	findStr(pat_str, psw, birth.substr(0, 4), 'm'); // 1994
	findStr(pat_str, psw, birth.substr(4, 4), 'n'); // 0231
	findStr(pat_str, psw, birth.substr(2, 4), 'o'); // 9402

	findStr(pat_str, psw, birth.substr(4, 4) + birth.substr(0, 4), 'p'); // 02311994
	findStr(pat_str, psw, birth.substr(4, 2) + birth.substr(0, 4), 'q'); // 021994
	findStr(pat_str, psw, birth.substr(4, 4) + birth.substr(2, 2), 'r'); // 023194
}

void person::processEmail(string& pat_str, string psw) {
	unsigned int pos = email.find('@');
	if (email == "" || pos == string::npos)
		return;
	findStr(pat_str, psw, email.substr(0, pos), 'E'); // email prefix
	string email_d = "", email_l = "", email_pre = email.substr(0, pos);
	bool dd = 0, ll = 0;
	for (int i = 0; i < (int)pos; i++) {
		if (email_pre[i] >= '0' && email_pre[i] <= '9') {
			email_d += email_pre[i];
			dd = 1;
		} else {
			if (dd == 1)
				break;
		}
	}
	if (dd == 1 && email_d != email_pre)
		findStr(pat_str, psw, email_d, 's'); // email prefix first D string 

	for (int i = 0; i < (int)pos; i++) {
		if (email_pre[i] >= 'a' && email_pre[i] <= 'z' || email_pre[i] >= 'A'
				&& email_pre[i] <= 'Z') {
			email_l += email_pre[i];
			ll = 1;
		} else {
			if (ll == 1)
				break;
		}
	}
	if (ll == 1 && email_l != email_pre)
		findStr(pat_str, psw, email_l, 't'); // email prefix first L string 
}

void person::processAccount(string& pat_str, string psw) {
	if (account == "")
		return;
	findStr(pat_str, psw, account, 'A');
	string acc_d = "", acc_l = "", acc = account;
	int acclen = account.length();
	bool ad = 0, al = 0;
	for (int i = 0; i < acclen; i++) {
		if (acc[i] >= '0' && acc[i] <= '9') {
			acc_d += acc[i];
			ad = 1;
		} else {
			if (ad == 1)
				break;
		}
	}
	if (ad == 1 && acc_d != acc)
		findStr(pat_str, psw, acc_d, 'u'); // username prefix first D string 

	for (int i = 0; i < acclen; i++) {
		if (acc[i] >= 'a' && acc[i] <= 'z' || acc[i] >= 'A' && acc[i] <= 'Z') {
			acc_l += acc[i];
			al = 1;
		} else {
			if (al == 1)
				break;
		}
	}
	if (al == 1 && acc_l != acc)
		findStr(pat_str, psw, acc_l, 'v'); // username prefix first L string 
}
void person::processGid(string& pat_str, string psw) {
	if (gid == "")
		return;
	int len = gid.length();
	findStr(pat_str, psw, gid, 'G');
	findStr(pat_str, psw, gid.substr(len - 4, 4), 'w'); // last four chars of gid
}