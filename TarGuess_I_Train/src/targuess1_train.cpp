/*
 * targuess1_train.cpp
 */
#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include "timer.h"
#include "read_config.h"
#include "person.h"

using namespace std;

map<string, person> PI;

map<string, int> p_count; // Count the number of patterns
int p_total;
map<string, int> d_count[40]; // Count the quantity of D
int d_total[40];
unsigned int d_maxlen;
map<string, int> l_count[40]; // Count the quantity of L
int l_total[40];
unsigned int l_maxlen;
map<string, int> s_count[40]; // Count the quantity of S
int s_total[40];
unsigned int s_maxlen;

// Sort from small to large, and finally scan backwards 
multimap<float, string> p_sort;
multimap<float, string> d_sort[40];
multimap<float, string> l_sort[40];
multimap<float, string> s_sort[40];

//////////////////////////////////
CTimer *timer = new CTimer; // timer
person pp;
int tot = 0;
person &CTimer::pi = pp;
int &CTimer::trained_num = p_total;
int &CTimer::tot_num = tot;
float CTimer::time_cost = 0;
string CTimer::status = "\"running\"";

/////////////////////////////////// parameter
ifstream fin; // training set

ofstream fout;//output
int interval = 1000;

//////////////////////////////////

// Obtain the password pattern based on personal information
string getPattern(string email, string psw) {
	int len = psw.length();
	string pat_str = "";
	for (int i = 0; i < len; i++)
		pat_str += 'P';
	// If the user's personal information can be found, match the corresponding location on the password
	if (PI.find(email) != PI.end()) {
		person pp = PI[email];
		/// phone
		pp.processPhone(pat_str, psw);

		// account name
		pp.processAccount(pat_str, psw);

		///name
		pp.processName(pat_str, psw);

		//birth
		pp.processBirth(pat_str, psw);

		//email
		pp.processEmail(pat_str, psw);

		// gid
		pp.processGid(pat_str, psw);
	}

	for (int i = 0; i < len; i++) {
		char c = psw[i];
		if (pat_str[i] == 'P') {
			if (c >= '0' && c <= '9')
				pat_str[i] = 'D';
			else if (c >= 'a' && c <= 'z' || c <= 'Z' && c >= 'A')
				pat_str[i] = 'L';
			else
				pat_str[i] = 'S';
		}
	}

	string ptmp = pat_str;
	char last = 'P';
	pat_str = "";
	for (int i = 0; i < len; i++) {
		char c = ptmp[i];
		if (c == 'D' || c == 'L' || c == 'S')
			pat_str += c;
		else if (c != last)
			pat_str += c;

		last = c;
	}

	return pat_str;
}

// Add various structures to the map
void addDLSPattern(string psw, string pat_str) {
	int len = psw.length();
	char last = 'P';
	string seg = "";

	pat_str = "";
	for (int i = 0; i < len; i++)
		pat_str += 'P';
	for (int i = 0; i < len; i++) {
		char c = psw[i];
		if (pat_str[i] == 'P') {
			if (c >= '0' && c <= '9')
				pat_str[i] = 'D';
			else if (c >= 'a' && c <= 'z' || c <= 'Z' && c >= 'A')
				pat_str[i] = 'L';
			else
				pat_str[i] = 'S';
		}
	}

	for (int i = 0; i < len; i++) {
		char c = psw[i];
		if (i == 0 || pat_str[i] == last)
			seg += c;
		else {
			if (last == 'D') {
				d_count[seg.length()][seg]++;
				d_total[seg.length()]++;
				if (seg.length() > d_maxlen)
					d_maxlen = seg.length();
			} else if (last == 'L') {
				l_count[seg.length()][seg]++;
				l_total[seg.length()]++;
				if (seg.length() > l_maxlen)
					l_maxlen = seg.length();
			} else if (last == 'S') {
				s_count[seg.length()][seg]++;
				s_total[seg.length()]++;
				if (seg.length() > s_maxlen)
					s_maxlen = seg.length();
			}
			seg = c;
		}
		last = pat_str[i];
	}

	if (last == 'D') {
		d_count[seg.length()][seg]++;
		d_total[seg.length()]++;
		if (seg.length() > d_maxlen)
			d_maxlen = seg.length();
	} else if (last == 'L') {
		l_count[seg.length()][seg]++;
		l_total[seg.length()]++;
		if (seg.length() > l_maxlen)
			l_maxlen = seg.length();
	} else if (last == 'S') {
		s_count[seg.length()][seg]++;
		s_total[seg.length()]++;
		if (seg.length() > s_maxlen)
			s_maxlen = seg.length();
	}

}

// Sort the statistical data based on probability
void sortTable() {
	//cout << "sortTable" << endl;
	map<string, int>::iterator pit = p_count.begin();
	map<string, int>::iterator it;
	float prob;
	for (; pit != p_count.end(); ++pit) {
		prob = pit->second / (float) p_total;
		p_sort.insert(make_pair(prob, pit->first));
	}

	//cout << "afterP" << endl;
	for (unsigned int i = 1; i <= d_maxlen; i++) {
		for (it = d_count[i].begin(); it != d_count[i].end(); ++it) {
			prob = it->second / (float) d_total[i];
			d_sort[i].insert(make_pair(prob, it->first));
		}
	}
	//cout << "afterD" << endl;
	for (unsigned int i = 1; i <= l_maxlen; i++) {
		for (it = l_count[i].begin(); it != l_count[i].end(); ++it) {
			prob = it->second / (float) l_total[i];
			l_sort[i].insert(make_pair(prob, it->first));
		}
	}
	//cout << "afterL" << endl;
	for (unsigned int i = 1; i <= s_maxlen; i++) {
		for (it = s_count[i].begin(); it != s_count[i].end(); ++it) {
			prob = it->second / (float) s_total[i];
			s_sort[i].insert(make_pair(prob, it->first));
		}
	}
	//cout << "afterS" << endl;

}

int main(int argc, char * argv[]) {

	//////////////// reading config
	string config_path = "config.ini";
	if (argc == 3) {
		if (strcmp(argv[1], "-c") == 0) {
			config_path = argv[2];
		} else {
			cout << "please input the correct parameter (-c filename)" << endl;
			return 0;
		}
	}
	map<string, string> config_inf;
	bool flag = readConfig(config_path, config_inf);
	if (!flag) {
		cout << "please input the correct config file" << endl;
		return 0;
	}
	cout << "reading config information successfully" << endl;

	/////////////// finish reading

	/////////////// reading file and initial
	fin.open(config_inf["training_set_path"].c_str()); // train in pattern
	fout.open(config_inf["model_output_path"].c_str()); // output

	if (config_inf.find("print_info_interval") != config_inf.end())
		interval = atoi(config_inf["print_info_interval"].c_str());

	if (!fin.is_open()) {
		cout << "reading training file failed " << endl;
		return 0;
	}
	string line;
	ifstream ffin(config_inf["training_set_path"].c_str());
	while (getline(ffin, line)) {
		tot++;
	}
	ffin.close();
	cout << "tot psw num: " << tot << endl;
	cout << "start training" << endl;

	timer->StartTimer(interval / 1000.0); // Start timing

	// The PI assignment that does not exist is ""
	while (getline(fin, line)) {
		int len = line.length();
		string inf[100];
		int st = 0, tmp = 0;
		for (int i = 0; i < len; i++) {
			if (line[i] == '\t') {
				inf[tmp++] = line.substr(st, i - st);
				st = i + 1;
			}
		}
		inf[tmp++] = line.substr(st, len - st);
		person pp;

		// After reading the data, store it in the inf array
		pp.email = inf[0];
		pp.psw = inf[1];
		pp.name = inf[2];
		pp.gid = inf[3];
		pp.account = inf[4];
		pp.phone = inf[5];
		pp.birth = inf[6];
		for (int i = 0; i < (int) pp.name.length(); i++) {
			if (pp.name[i] == '|')
				pp.name[i] = ' ';
		}
		if (pp.name[pp.name.length() - 1] != ' ') {
			pp.name += ' ';
		}

		PI[pp.email] = pp;
		string pat = getPattern(pp.email, pp.psw); // Get the structure of the psw
		addDLSPattern(pp.psw, pat);
		p_count[pat]++; // Add this pattern
		p_total++;
		CTimer::pi = pp;
	}

	person tmp;
	CTimer::status = "\"sorting\"";
	CTimer::pi = tmp;
	//////////////// training
	sortTable();

	//////// Output the model to a file
	// Output pattern first 
	multimap<float, string>::reverse_iterator iter;
	for (iter = p_sort.rbegin(); iter != p_sort.rend(); iter++)
		fout << iter->second << '\t' << iter->first << endl;
	fout << endl;
	// Then output strings of various types and their lengths
	for (unsigned int i = 1; i <= d_maxlen; i++) {
		for (iter = d_sort[i].rbegin(); iter != d_sort[i].rend(); iter++)
			fout << 'D' << i << '\t' << iter->second << '\t' << iter->first
					<< endl;
	}
	for (unsigned int i = 1; i <= l_maxlen; i++) {
		for (iter = l_sort[i].rbegin(); iter != l_sort[i].rend(); iter++)
			fout << 'L' << i << '\t' << iter->second << '\t' << iter->first
					<< endl;
	}
	for (unsigned int i = 1; i <= s_maxlen; i++) {
		for (iter = s_sort[i].rbegin(); iter != s_sort[i].rend(); iter++)
			fout << 'S' << i << '\t' << iter->second << '\t' << iter->first
					<< endl;
	}

	delete timer;

	return 0;
}

