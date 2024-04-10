/*
 * personTran.h
 */

#ifndef PERSONTRAN_H_
#define PERSONTRAN_H_
#include "person.h"
#include <iostream>

using namespace std;

class personTran {
public:
	bool dd;
	bool ll;
	bool ad;
	bool al;
	int name_len;
	unsigned int pos;

	// Save various personal information transformations
	char *email[3];
	char *name[8];
	char *birth[12];
	char *account[3];
	char *phone;
	char *gid[2];

	// Save the length of various transformations
	int email_len[3];
	int nname_len[8];
	int birth_len[12];
	int account_len[3];
	int phone_len;
	int gid_len[2];

	personTran();
	personTran(const person &pp);
	~personTran();
};


#endif /* PERSONTRAN_H_ */
