/*
 * personTran.cpp
 */
#include "personTran.h"

personTran::personTran() {
	ad = 0;
	al = 0;
	dd = 0;
	ll = 0;
	name_len = 0;
	pos = string::npos;
	for (int i = 0; i < 3; i++) {
		email[i] = new char[35];
		account[i] = new char[35];
	}
	for (int i = 0; i < 12; i++)
		birth[i] = new char[35];
	for (int i = 0; i < 8; i++)
		name[i] = new char[35];
	for (int i = 0; i < 2; i++)
		gid[i] = new char[35];
	phone = new char[35];
}

personTran::~personTran() {
	for (int i = 0; i < 3; i++) {
		delete[] email[i];
		delete[] account[i];
	}
	for (int i = 0; i < 12; i++)
		delete[] birth[i];
	for (int i = 0; i < 8; i++)
		delete[] name[i];
	for (int i = 0; i < 2; i++)
		delete[] gid[i];
	delete[] phone;
}

personTran::personTran(const person & pp) {
	ad = 0;
	al = 0;
	dd = 0;
	ll = 0;
	name_len = 0;
	pos = string::npos;
	for (int i = 0; i < 3; i++) {
		email[i] = new char[35];
		account[i] = new char[35];
	}
	for (int i = 0; i < 12; i++)
		birth[i] = new char[35];
	for (int i = 0; i < 8; i++)
		name[i] = new char[35];
	for (int i = 0; i < 2; i++)
		gid[i] = new char[35];
	phone = new char[35];

	// Email three transformation types
	pos = pp.email.find('@');
	if (pos != string::npos) {
		string email_pre = pp.email.substr(0, pos); // email prefix
		strcpy(email[0], email_pre.c_str());
		string email_d = "";
		for (int j = 0; j < (int) pos; j++) {
			if (email_pre[j] >= '0' && email_pre[j] <= '9') {
				email_d += email_pre[j];
				dd = 1;
			} else if (dd == 1)
				break;
		}
		if (dd == 1) {
			strcpy(email[1], email_d.c_str()); // The first string of digits in the email prefix
		}
		string email_l = "";
		for (int j = 0; j < (int) pos; j++) {
			if (email_pre[j] >= 'a' && email_pre[j] <= 'z' || email_pre[j]
					>= 'A' && email_pre[j] <= 'Z') {
				email_l += email_pre[j];
				ll = 1;
			} else {
				if (ll == 1)
					break;
			}
		}
		if (ll == 1)
			strcpy(email[2], email_l.c_str());// The first letter of the email prefix
	}
	/// Account three transformation types
	strcpy(account[0], pp.account.c_str());
	if (pp.account != "") {
		string acc = pp.account;
		int acclen = acc.length();
		string acc_d = "";
		for (int j = 0; j < acclen; j++) {
			if (acc[j] >= '0' && acc[j] <= '9') {
				acc_d += acc[j];
				ad = 1;
			} else {
				if (ad == 1)
					break;
			}
		}
		if (ad == 1)
			strcpy(account[1], acc_d.c_str()); // The first string of digits in account
		string acc_l = "";
		for (int j = 0; j < acclen; j++) {
			if (acc[j] >= 'a' && acc[j] <= 'z' || acc[j] >= 'A' && acc[j]
					<= 'Z') {
				acc_l += acc[j];
				al = 1;
			} else {
				if (al == 1)
					break;
			}
		}
		if (al == 1)
			strcpy(account[2], acc_l.c_str()); // The first letter of the account string
	}
	// phone one type
	if (pp.phone != "")
		strcpy(phone, pp.phone.c_str());
	// gid two types
	if (pp.gid != "") {
		strcpy(gid[0], pp.gid.c_str());
		strcpy(gid[1], pp.gid.substr(pp.gid.length() - 4, 4).c_str()); // the last four chars of gid
	}

	// birth 12 types 
	strcpy(birth[0], pp.birth.c_str());
	if (pp.birth.length() == 8) {
		string pbirth = pp.birth; // 19940231
		strcpy(birth[1], (pbirth.substr(0, 4) + pbirth.substr(5, 3)).c_str()); // 1994231
		strcpy(birth[2], (pbirth.substr(0, 6) + pbirth[7]).c_str()); // 1994021
		strcpy(birth[3], pbirth.substr(0, 6).c_str()); // 199402
		strcpy(birth[4], pbirth.substr(2, 6).c_str()); // 940231
		strcpy(birth[5], (pbirth.substr(0, 4) + pbirth[5] + pbirth[7]).c_str()); // 199421
		strcpy(birth[6], pbirth.substr(0, 4).c_str()); // 1994
		strcpy(birth[7], pbirth.substr(2, 4).c_str()); // 9402
		strcpy(birth[8], pbirth.substr(4, 4).c_str()); // 0231
		strcpy(birth[9], (pbirth.substr(4, 4) + pbirth.substr(0, 4)).c_str()); // 02311994
		strcpy(birth[10], (pbirth.substr(4, 2) + pbirth.substr(0, 4)).c_str()); // 021994
		strcpy(birth[11], (pbirth.substr(4, 4) + pbirth.substr(2, 2)).c_str()); // 023194
	}

	// name 8 types
	if (pp.name != "") {
		string py = pp.name; // zhang san chuan
		string pname[20]; // zhang/san/chuan
		int py_len = py.length(), nst = 0;
		for (int j = 0; j < py_len; j++) {
			if (py[j] == ' ') {
				if (py[j - 1] != '?')
					pname[name_len++] = py.substr(nst, j - nst);
				nst = j + 1;
			}
		}
		if (name_len > 0) {
			string name_tmp[5];
			for (int j = 0; j < name_len; j++) {
				name_tmp[0] += pname[j][0];
				name_tmp[1] += pname[j];
				if (j != 0)
					name_tmp[2] += pname[j];
			}
			name_tmp[3] = pname[0];
			name_tmp[3][0] -= 32;
			name_tmp[4] = name_tmp[0].substr(1, name_tmp[0].length() - 1); // sc
			strcpy(name[0], name_tmp[1].c_str()); //zhangsanchuan
			strcpy(name[1], (name_tmp[2] + pname[0]).c_str()); // sanchuanzhang
			strcpy(name[2], name_tmp[0].c_str()); // zsc
			strcpy(name[3], name_tmp[2].c_str()); // sanchuan
			strcpy(name[4], name_tmp[3].c_str()); // Zhang
			strcpy(name[5], pname[0].c_str()); // zhang
			strcpy(name[6], (name_tmp[4] + pname[0]).c_str()); // sczhang
			strcpy(name[7], (pname[0] + name_tmp[4]).c_str()); // zhangsc
		}
	}
	for (int i = 0; i < 3; i++) {
		email_len[i] = strlen(email[i]);
		account_len[i] = strlen(account[i]);
	}
	for (int i = 0; i < 12; i++)
		birth_len[i] = strlen(birth[i]);
	for (int i = 0; i < 8; i++)
		nname_len[i] = strlen(name[i]);
	for (int i = 0; i < 2; i++)
		gid_len[i] = strlen(gid[i]);
	phone_len = strlen(phone);
}
