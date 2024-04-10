/*
 * read_config.h
 */

#ifndef READ_CONFIG_H_
#define READ_CONFIG_H_

#include <iostream>
#include <map>
#include <fstream>
using namespace std;

bool readConfig(string path,map<string,string> & rtn){
	ifstream config_in(path.c_str());
	if (!config_in.is_open()) {
		cout<<"please input the corect config path"<<endl;
		return false;
	}

	string line;

	while(getline(config_in,line)) {
		if (line=="[basic_config]" || line=="")
			continue;
		int len=line.length();
		string parameter_name="";
		string parameter_value="";
		int pos=line.find('=');
		if (pos==-1) {
			cout<<"line: "<<line<<" miss ="<<endl;
			return false;
		}
		parameter_name=line.substr(0,pos);
		parameter_value=line.substr(pos+1,len-pos-1);
		rtn[parameter_name]=parameter_value;
		cout<<parameter_name<<": "<<parameter_value<<endl;
	}
	return true;
}

#endif /* READ_CONFIG_H_ */
