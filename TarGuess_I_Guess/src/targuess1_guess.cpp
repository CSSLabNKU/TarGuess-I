/*
 * targuess1_guess.cpp
 */

#include <iostream>
#include <fstream>
#include <queue>
#include <set>
#include <map>
#include "timer.h"
#include "read_config.h"
#include "person.h"
#include "personTran.h"
using namespace std;

int total_guess = 0;
int cur_guess_num = 0;
long long predict_num = -1;
string g_pat, g_psw;
char g_cur[50]; // Add the following characters and fill in the guess
float g_prob;
time_t last_fin_time;

////////////////////////////// parameter
ifstream pin;//personal info
ifstream model_in;//model
ofstream fout;//output file
ofstream log_out;//log file
bool is_output_to_file = true;

// The maximum number of passwords that can be printed out is set to negative, with no limit
int maximum_guess_num = 100000000;
// The minimum probability threshold for allowing output passwords, set to negative to have no limit
double minimum_prob = 0.0000000000001;
// Maximum password length, set to negative to have no limit
int password_max_len = 999; 
// The shortest length of the password, set to negative, there is no limit
int password_min_len = 0;
// The time interval for printing summary information in log, in milliseconds
int print_info_interval = 1000;
// detailed information
bool verbose = false;
// Is it possible to pre calculate the number of passwords that can be generated
bool predict_password_num = true;
// The maximum time required to calculate the number of passwords that can be generated, 
// if set to negative, there is no limit
int predict_timeout = 40000;

// Whether to remove duplicates
bool check_duplicate = false;
// Do you want to output the guess to the console
bool output_to_console = true;
//////////////////////////////////

CTimer *timer = new CTimer;
long long & CTimer::predict_guess_num = predict_num;
int& CTimer::psw_guess_num = total_guess;
int & CTimer::max_guess_num = maximum_guess_num;
float CTimer::time_cost = 0;
time_t &CTimer::last_time = last_fin_time;
string& CTimer::pat = g_pat;
char* CTimer::cur = g_cur;
float & CTimer::prob = g_prob;
string CTimer::status = "\"running\"";
ofstream & CTimer::log = log_out;

///////////////////////////
/// Sort from small to large
multimap<float, string> p_sort;
multimap<float, string> d_sort[40];
multimap<float, string> l_sort[40];
multimap<float, string> s_sort[40];

map<string, multimap<float, string>::iterator> iter_map;

priority_queue<guess> G;
map<string, person> PI;
set<string> exist; // Desaving


personTran * pt; // The transformation of personal information

// Fill in the pattern based on personal information
bool genPersonalGuess() {
	int len = g_pat.length();
	int pat_st = 0, cur_st = 0;
	bool has_person_label = 0;
	for (; pat_st < len; ++pat_st) {
		char c = g_pat[pat_st];
		if (c == 'L' || c == 'D' || c == 'S') {
			g_cur[cur_st++] = g_psw[pat_st];
		} else {
			has_person_label = 1;
			if (c == 'E' || c == 's' || c == 't') {
				if ((unsigned int) pt->pos == string::npos)
					break;
				if (c == 'E') { // email prefix
					strcpy(g_cur + cur_st, pt->email[0]);
					cur_st += pt->pos;
					continue;
				} else if (c == 's') { // The first string of digits in an email
					if (pt->dd == 1) {
						strcpy(g_cur + cur_st, pt->email[1]);
						cur_st += pt->email_len[1];
						continue;
					}
				} else if (c == 't') { // The first string of letters in an email
					if (pt->ll == 1) {
						strcpy(g_cur + cur_st, pt->email[2]);
						cur_st += pt->email_len[2];
						continue;
					}
				}

			} else if (c == 'A' || c == 'u' || c == 'v') {
				if (strlen(pt->account[0]) != 0) {
					if (c == 'A') { // account
						strcpy(g_cur + cur_st, pt->account[0]);
						cur_st += pt->account_len[0];
						continue;
					} else if (c == 'u') { // The first string of digits in an account
						if (pt->ad == 1) {
							strcpy(g_cur + cur_st, pt->account[1]);
							cur_st += pt->account_len[1];
							continue;
						}
					} else if (c == 'v') { // The first string of letters in an account
						if (pt->al == 1) {
							strcpy(g_cur + cur_st, pt->account[2]);
							cur_st += pt->account_len[2];
							continue;
						}
					}

				}
			} else if (c == 'C') { // phone
				if (pt->phone_len != 0) {
					strcpy(g_cur + cur_st, pt->phone);
					cur_st += pt->phone_len;
					continue;
				}

			} else if (c == 'G' || c == 'w') { // gid
				if (pt->gid_len[0] == 18) {
					if (c == 'G') {
						strcpy(g_cur + cur_st, pt->gid[0]);
						cur_st += pt->gid_len[0];
						continue;
					} else if (c == 'w') {
						strcpy(g_cur + cur_st, pt->gid[1]);
						cur_st += pt->gid_len[1];
						continue;
					}
				}

			} else if (c == 'B' || 'h' <= c && c <= 'r') { // birth
				if (pt->birth_len[0] == 8) {
					if (c == 'B') {
						strcpy(g_cur + cur_st, pt->birth[0]);
						cur_st += 8; // 19930508
						continue;
					} else if (c == 'h') {
						if (pt->birth[0][4] == '0') {
							strcpy(g_cur + cur_st, pt->birth[1]);
							cur_st += 7; // 1993508
							continue;
						}
					} else if (c == 'i') {
						if (pt->birth[0][6] == '0') {
							strcpy(g_cur + cur_st, pt->birth[2]);
							cur_st += 7; // 1993058
							continue;
						}
					} else if (c == 'k') {
						strcpy(g_cur + cur_st, pt->birth[3]);
						cur_st += 6; // 199305
						continue;
					} else if (c == 'l') {
						strcpy(g_cur + cur_st, pt->birth[4]);
						cur_st += 6; // 930508
						continue;
					} else if (c == 'j') {
						if (pt->birth[0][4] == '0' && pt->birth[0][6] == '0') {
							strcpy(g_cur + cur_st, pt->birth[5]);
							cur_st += 6; // 199358
							continue;
						}
					} else if (c == 'm') {
						strcpy(g_cur + cur_st, pt->birth[6]);
						cur_st += 4; // 1993
						continue;
					} else if (c == 'o') {
						strcpy(g_cur + cur_st, pt->birth[7]);
						cur_st += 4; // 9305
						continue;
					} else if (c == 'n') {
						strcpy(g_cur + cur_st, pt->birth[8]);
						cur_st += 4; // 0507
						continue;
					} else if (c == 'p') {
						strcpy(g_cur + cur_st, pt->birth[9]);
						cur_st += 8; // 05071993
						continue;
					} else if (c == 'q') {
						strcpy(g_cur + cur_st, pt->birth[10]);
						cur_st += 6; // 051993
						continue;
					} else if (c == 'r') {
						strcpy(g_cur + cur_st, pt->birth[11]);
						cur_st += 6; // 050793
						continue;
					}
				}
			} else if (c == 'N' || 'a' <= c && c <= 'g') { // name
				if (pt->name_len > 0) {
					// zhangsanchuan, sanchuanzhang
					if (c == 'N') {
						strcpy(g_cur + cur_st, pt->name[0]);
						cur_st += pt->nname_len[0];
						continue;
					} else if (c == 'a') {
						strcpy(g_cur + cur_st, pt->name[1]);
						cur_st += pt->nname_len[1];
						continue;
					}
					// zsc
					else if (c == 'b') {
						strcpy(g_cur + cur_st, pt->name[2]);
						cur_st += pt->nname_len[2];
						continue;
					}
					// sanchuan
					else if (c == 'c') {
						strcpy(g_cur + cur_st, pt->name[3]);
						cur_st += pt->nname_len[3];
						continue;
					}
					// zhang, Zhang
					else if (c == 'e') {
						strcpy(g_cur + cur_st, pt->name[4]);
						cur_st += pt->nname_len[4];
						continue;
					} else if (c == 'd') {
						strcpy(g_cur + cur_st, pt->name[5]);
						cur_st += pt->nname_len[5];
						continue;
					}
					// sczhang, zhangsc
					else if (c == 'f') {
						strcpy(g_cur + cur_st, pt->name[6]);
						cur_st += pt->nname_len[6];
						continue;
					} else if (c == 'g') {
						strcpy(g_cur + cur_st, pt->name[7]);
						cur_st += pt->nname_len[7];
						continue;
					}
				}

			}
			return 0;
		}
	}
	if ((int) strlen(g_cur) >= password_min_len && (int) strlen(g_cur)
			<= password_max_len) {
		if (check_duplicate) {
			if (has_person_label) {
				if (exist.find(g_cur) == exist.end()) {
					exist.insert(g_cur);
				} else
					return 1;
			}
		}
		if (is_output_to_file) { // Output the content of the console to a file
			fout << g_cur;
			if (verbose)
				fout << '\t' << g_prob;
			fout << endl;
		}
		if (output_to_console) { // Output the guess to the console
			printf("%s",g_cur);
			//cout << g_cur;
			if (verbose)
				printf("\t%e",g_prob);
				//cout << '\t' << g_prob;
			printf("\n");
			//cout << endl;
		}

		cur_guess_num++;
		total_guess++;
		return 1;
	}
	return 0;
}

void genNext(guess top) { // Generate the next set of appearances
	string pat = top.pat;
	string psw = top.psw;
	float prob = top.prob;
	int pivot = top.pivot;

	int pat_len = pat.length();
	int pat_num = 0;
	string seg = "";
	multimap<float, string>::iterator iter;

	for (int i = 0; i <= pat_len; ++i) {
		char c = 'P';
		if (i != pat_len)
			c = pat[i];
		if (i != pat_len && (i == 0 || c == seg[0] && (c == 'D' || c == 'L'
				|| c == 'S')))
			seg += c;
		else {
			if (pat_num >= pivot) {
				int seg_len = seg.length();
				float newprob = prob;
				string newpsw = psw;
				guess tmp;
				if (seg[0] == 'D') {
					string sub = psw.substr(i - seg_len, seg_len);
					iter = iter_map[sub];
					newprob /= iter->first;
					if (iter != d_sort[seg_len].begin()) {
						--iter;
						newprob *= iter->first;
						if (newprob > minimum_prob) {
							tmp.prob = newprob;
							tmp.pat = pat;
							tmp.pivot = pat_num;
							// Replace the corresponding string
							tmp.psw = newpsw.replace(i - seg_len, seg_len, iter->second);
							G.push(tmp);
						}
					}
				} else if (seg[0] == 'L') {
					string sub = psw.substr(i - seg_len, seg_len);
					iter = iter_map[sub];
					newprob /= iter->first;
					if (iter != l_sort[seg_len].begin()) {
						--iter;
						newprob *= iter->first;
						if (newprob > minimum_prob) {
							tmp.prob = newprob;
							tmp.pat = pat;
							tmp.pivot = pat_num;
							// Replace the corresponding string
							tmp.psw = newpsw.replace(i - seg_len, seg_len, iter->second);
							G.push(tmp);
						}
					}
				} else if (seg[0] == 'S') {
					string sub = psw.substr(i - seg_len, seg_len);
					iter = iter_map[sub];
					newprob /= iter->first;
					if (iter != s_sort[seg_len].begin()) {
						--iter;
						newprob *= iter->first;
						if (newprob > minimum_prob) {
							tmp.prob = newprob;
							tmp.pat = pat;
							tmp.pivot = pat_num;
							// Replace the corresponding string
							tmp.psw = newpsw.replace(i - seg_len, seg_len, iter->second);
							G.push(tmp);
						}
					}
				}
			}
			seg = c;
			pat_num++;
		}
	}
}

void genGuess(person & pp) {
	multimap<float, string>::reverse_iterator pit;
	// Fill each pose with the pattern with the highest probability and insert it into the priority queue
	for (pit = p_sort.rbegin(); pit != p_sort.rend(); ++pit) {
		float prob = pit->first;
		string pat = pit->second;
		string psw = "";
		int len = pat.length();
		string seg;
		guess tmp;
		bool suc = 1;
		for (int i = 0; i <= len; ++i) {
			char c = 'P';
			if (i != len)
				c = pat[i];
			if (i != len && (i == 0 || c == seg[0]))
				seg += c;
			else {
				int seg_len = seg.length();
				if (seg[0] == 'D') {
					if (d_sort[seg_len].empty()) {
						suc = 0;
						break;
					}

					psw += d_sort[seg_len].rbegin()->second;
					prob *= d_sort[seg_len].rbegin()->first;
				} else if (seg[0] == 'L') {
					if (l_sort[seg_len].empty()) {
						suc = 0;
						break;
					}
					psw += l_sort[seg_len].rbegin()->second;
					prob *= l_sort[seg_len].rbegin()->first;
				} else if (seg[0] == 'S') {
					if (s_sort[seg_len].empty()) {
						suc = 0;
						break;
					}
					psw += s_sort[seg_len].rbegin()->second;
					prob *= s_sort[seg_len].rbegin()->first;
				} else
					psw += seg;
				seg = c;
			}
		}
		if (suc) {
			tmp.pat = pat;
			tmp.psw = psw;
			tmp.prob = prob;
			tmp.pivot = 0;
			G.push(tmp); // Insert into priority queue
		}
	}

	pt = new personTran(pp); // Transforming personal information
	cur_guess_num = 0;
	while (!G.empty()) {
		guess top = G.top(); // Pop up the guide at the top of the heap
		G.pop();
		if (maximum_guess_num >= 0 && cur_guess_num >= maximum_guess_num
				|| top.prob < minimum_prob) {
			//cout << cur_guess_num << ' ' << maximum_guess_num << endl;
			break;
		}

		g_pat = top.pat;
		g_psw = top.psw;
		g_prob = top.prob;
		memset(g_cur, 0, sizeof(g_cur));
		bool flag = genPersonalGuess(); // Fill in personal information
		if (!flag)
			continue;
		genNext(top); // Generate the next set of appearances
	}
	G = priority_queue<guess> ();// clear G
	//	while(!G.empty())
	//		G.pop();

	if (check_duplicate)
		exist.clear();
	delete pt;

}

// Calculate the maximum number of appearances that can be generated, 
// enumerate each pattern, fill in the DLS in the pattern, 
// and estimate the number of probabilities greater than the threshold after filling
long long calMaxGuessNum() {
	time_t st = time(NULL);
	multimap<float, string>::reverse_iterator pit = p_sort.rbegin();
	multimap<float, string>::reverse_iterator it;
	long long max_guess = 0;
	for (; pit != p_sort.rend(); ++pit) {
		if (predict_timeout > 0) {
			time_t ed = time(NULL);
			if (difftime(ed, st) > predict_timeout) // timeout
				return -2;
		}
		if (pit->first < minimum_prob)
			break;
		string pat = pit->second;
		int len = pat.length();
		if (len > password_max_len)
			continue;

		char c = pat[0];
		int seg_len = 1;
		double max_pro = pit->first;
		bool flag = false;
		long long guess_num = 1;

		// Calculate the maximum probability of filling the structure first
		for (int i = 1; i < len; i++) {
			if (c == 'D' || c == 'L' || c == 'S') {
				if (pat[i] == c)
					seg_len++;
				else {
					if (c == 'D') {
						max_pro *= d_sort[seg_len].rbegin()->first;
						if (max_pro < minimum_prob) {
							flag = true;
							break;
						}
					} else if (c == 'L') {
						max_pro *= l_sort[seg_len].rbegin()->first;
						if (max_pro < minimum_prob) {
							flag = true;
							break;
						}
					} else if (c == 'S') {
						max_pro *= s_sort[seg_len].rbegin()->first;
						if (max_pro < minimum_prob) {
							flag = true;
							break;
						}
					}
					seg_len = 1;
				}
			} else
				seg_len = 1;
			c = pat[i];
		}
		if (flag)
			continue;
		if (c == 'D') {
			max_pro *= d_sort[seg_len].rbegin()->first;
			if (max_pro < minimum_prob)
				continue;
		} else if (c == 'L') {
			max_pro *= l_sort[seg_len].rbegin()->first;
			if (max_pro < minimum_prob)
				continue;
		} else if (c == 'S') {
			max_pro *= s_sort[seg_len].rbegin()->first;
			if (max_pro < minimum_prob)
				continue;
		}

		c = pat[0];
		seg_len = 1;
		flag = false;
		// The smallest remaining probability that can be filled
		double remain_pro = minimum_prob / max_pro;
		for (int i = 1; i < len; i++) {
			if (c == 'D' || c == 'L' || c == 'S') {
				if (pat[i] == c)
					seg_len++;
				else {
					if (c == 'D') {
						double rpro = remain_pro
								* d_sort[seg_len].rbegin()->first;
						if (d_sort[seg_len].begin()->first >= rpro)
							guess_num *= d_sort[seg_len].size();
						else {
							int num = 0;
							// Traverse from back to front and count the quantity that can be filled
							for (it = d_sort[seg_len].rbegin(); it != d_sort[seg_len].rend(); it++) {
								if (it->first < rpro)
									break;
								num++;
							}
							guess_num *= num;
						}
					} else if (c == 'L') {
						double rpro = remain_pro
								* l_sort[seg_len].rbegin()->first;
						if (l_sort[seg_len].begin()->first >= rpro)
							guess_num *= l_sort[seg_len].size();
						else {
							int num = 0;
							for (it = l_sort[seg_len].rbegin(); it
									!= l_sort[seg_len].rend(); it++) {
								if (it->first < rpro)
									break;
								num++;
							}
							guess_num *= num;
						}
					} else if (c == 'S') {
						double rpro = remain_pro
								* s_sort[seg_len].rbegin()->first;
						if (s_sort[seg_len].begin()->first >= rpro)
							guess_num *= s_sort[seg_len].size();
						else {
							int num = 0;
							for (it = s_sort[seg_len].rbegin(); it
									!= s_sort[seg_len].rend(); it++) {
								if (it->first < rpro)
									break;
								num++;
							}
							guess_num *= num;
						}
					}
					seg_len = 1;
				}
			} else
				seg_len = 1;
			c = pat[i];
		}
		if (c == 'D') {
			double rpro = remain_pro * d_sort[seg_len].rbegin()->first;
			if (d_sort[seg_len].begin()->first >= rpro)
				guess_num *= d_sort[seg_len].size();
			else {
				int num = 0;
				for (it = d_sort[seg_len].rbegin(); it
						!= d_sort[seg_len].rend(); it++) {
					if (it->first < rpro)
						break;
					num++;
				}
				guess_num *= num;
			}
		} else if (c == 'L') {
			double rpro = remain_pro * l_sort[seg_len].rbegin()->first;
			if (l_sort[seg_len].begin()->first >= rpro)
				guess_num *= l_sort[seg_len].size();
			else {
				int num = 0;
				for (it = l_sort[seg_len].rbegin(); it
						!= l_sort[seg_len].rend(); it++) {
					if (it->first < rpro)
						break;
					num++;
				}
				guess_num *= num;
			}
		} else if (c == 'S') {
			double rpro = remain_pro * s_sort[seg_len].rbegin()->first;
			if (s_sort[seg_len].begin()->first >= rpro)
				guess_num *= s_sort[seg_len].size();
			else {
				int num = 0;
				for (it = s_sort[seg_len].rbegin(); it
						!= s_sort[seg_len].rend(); it++) {
					if (it->first < rpro)
						break;
					num++;
				}
				guess_num *= num;
			}
		}

		if (guess_num < 0 || max_guess < 0)
			return -1;
		max_guess += guess_num;
	}

	return max_guess;

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
	model_in.open(config_inf["model_path"].c_str());//train in pattern
	pin.open(config_inf["personal_info_path"].c_str());
	log_out.open(config_inf["log_path"].c_str());
	if (config_inf["output_to_file"] != "")
		fout.open(config_inf["output_to_file"].c_str());//output
	else
		is_output_to_file = false;

	if (!model_in.is_open()) {
		cout << "reading model file failed " << endl;
		return 0;
	}
	if (!pin.is_open()) {
		cout << "reading personal info failed " << endl;
		return 0;
	}

	// The maximum number of passwords that can be printed out is set to negative, with no limit
	maximum_guess_num = atoi(config_inf["maximum_guess_num"].c_str());
	// The minimum probability threshold for allowing output passwords, set to negative to have no limit
	minimum_prob = atof(config_inf["minimum_prob"].c_str());
	// Maximum password length, set to negative to have no limit
	password_max_len = atoi(config_inf["password_max_len"].c_str());
	if (password_max_len < 0)
		password_max_len = 999;
	// The shortest length of the password, set to negative, there is no limit
	password_min_len = atoi(config_inf["password_min_len"].c_str());
	// The time interval for printing summary information in log, in milliseconds
	print_info_interval = atoi(config_inf["print_info_interval"].c_str());
	if (config_inf["verbose"] == "false")
		verbose = false;
	else
		verbose = true;
	if (config_inf["predict_password_num"] == "false")
		predict_password_num = false;
	else
		predict_password_num = true;

	if (config_inf["check_duplicate"] == "false")
		check_duplicate = false;
	else
		check_duplicate = true;

	if (config_inf["output_to_console"] == "false")
		output_to_console = false;
	else
		output_to_console = true;

	// The maximum time required to calculate the number of passwords that can be generated, 
	// if set to negative, there is no limit
	predict_timeout = atoi(config_inf["predict_timeout"].c_str());
	predict_timeout /= 1000;
	////////////////////////////////////////////


	string line;
	int cnt = 0;
	cout << "reading model" << endl;
	// Read in prediction data in the format of pattern \t pro
	while (getline(model_in, line)) {
		int len = line.length();
		if (line == "")
			break;
		string inf[10];
		int st = 0, tmp = 0;
		for (int i = 0; i < len; i++) {
			if (line[i] == '\t') {
				inf[tmp++] = line.substr(st, i - st);
				st = i + 1;
			}
		}
		inf[tmp++] = line.substr(st, len - st);

		p_sort.insert(make_pair(atof(inf[1].c_str()), inf[0]));
	}
	// Read the strings and probabilities of various structures in pcfg, format: type len\tstr\tpro
	while (getline(model_in, line)) {
		int len = line.length();
		string inf[10];
		int st = 0, tmp = 0;
		for (int i = 0; i < len; i++) {
			if (line[i] == '\t') {
				inf[tmp++] = line.substr(st, i - st);
				st = i + 1;
			}
		}
		inf[tmp++] = line.substr(st, len - st);
		// Store in inf array

		int num = atoi(inf[0].substr(1, inf[0].length() - 1).c_str()); // String length
		if (inf[0][0] == 'D') {
			// Insert into map
			iter_map[inf[1]] = d_sort[num].insert(make_pair(atof(inf[2].c_str()), inf[1]));
		} else if (inf[0][0] == 'L') {
			iter_map[inf[1]] = l_sort[num].insert(make_pair(atof(inf[2].c_str()), inf[1]));
		} else if (inf[0][0] == 'S') {
			iter_map[inf[1]] = s_sort[num].insert(make_pair(atof(inf[2].c_str()), inf[1]));
		}
	}
	// Estimating the number of Guess generated
	if (predict_password_num) {
		predict_num = calMaxGuessNum();
		if (predict_num >= 0 && maximum_guess_num > predict_num)
			maximum_guess_num = predict_num;
	}

	cout << "start generating" << endl;
	// Start timing
	timer->StartTimer(print_info_interval / 1000.0);
	last_fin_time = CTimer::start_time;

	while (getline(pin, line)) {
		cout << cnt++ << endl;
		int len = line.length();
		string inf[1000];
		int st = 0, tmp = 0;
		for (int i = 0; i < len; i++) {
			if (line[i] == '\t') {
				inf[tmp++] = line.substr(st, i - st);
				st = i + 1;
			}
		}
		inf[tmp++] = line.substr(st, len - st);
		person pp;

		// Read personal information and store it in the inf array
		pp.email = inf[0];
		pp.psw = inf[1];
		pp.name = inf[2];
		pp.gid = inf[3];
		pp.account = inf[4];
		pp.phone = inf[5];
		pp.birth = inf[6];
		string name_tmp = "";
		string ppname = "";
		for (int i = 0; i < (int) pp.name.length(); i++) {
			if (pp.name[i] == '|') {
				ppname += name_tmp + ' ';
				name_tmp = "";
			} else
				name_tmp += pp.name[i];
		}
		pp.name = ppname;

		genGuess(pp); // Generate a guess based on personal information
		last_fin_time = time(NULL); // Reset time to current time
	}

	return 0;
}

