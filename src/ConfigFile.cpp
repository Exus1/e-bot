#include "includes/ConfigFile.hpp"

using namespace std;

ConfigFile::ConfigFile(const string configFile) : configFile(configFile) {
    fstream file;
    file.open(configFile, ios::in);
    if(!file.good()) {
        return;
    }

    string line;
    string section;
    regex reg("[[:s:]]*([^ \t\n\r\f]+)[[:s:]]*={1}[[:s:]]*(.*)");
    regex regSect("[[:s:]]*\\[[[:s:]]*([^ \t\n\r\f]+)[[:s:]]*\\]");
    smatch expMatch;

    while(getline(file, line)) {
        if(!line.length()) continue;
        if(line[0] == '#') continue;
        if(line[0] == ';') continue;



        if(regex_search(line, expMatch, regSect)) {
            section = expMatch[1];
            continue;
        }

        if(regex_search(line, expMatch, reg)) {
            this->content[section][expMatch[1]] = expMatch[2];
            continue;
        }
    }

    file.close();
}

bool ConfigFile::save() {
    fstream file;
    file.open(this->configFile, ios::out | ios::trunc);

    if(!file.good()) {
        file.close();
        return false;
    }

    for(map<string, map<string, string>>::iterator it = this->content.begin(); it != this->content.end(); it++) {
        file << "[" << it->first << "]" << endl;
        for(map<string,string>::iterator it1 = it->second.begin(); it1 != it->second.end(); it1++) {
            file << " " << it1->first << " = " << it1->second << endl;
        }
    }

    file.flush();
    file.close();
    return true;
}

map<string, string>& ConfigFile::operator[](string index) {
        return content[index];
}

void ConfigFile::clear() {
    this->content.clear();
}

bool ConfigFile::good(string prefix) {
    if(prefix != "nullempty") {
        if(this->content.find(prefix) == this->content.end()) return false;
        else return true;
    }else {
        if(this->content.empty()) return false;
        else return true;
    }


}

vector<string> ConfigFile::explode(string data, char dividerChar) {
  vector<string> returnedVector;
  string dataBuff;

  for(int i = 0; i < data.length(); i++) {
    if(dataBuff.empty() && (data[i] == ' ')) continue;
    if((dataBuff[i] == '#') || ((dataBuff[i] == '/') && (dataBuff[i+1] == '/'))) break;
    if(!dataBuff.empty() && (data[i] == dividerChar)) {
      returnedVector.push_back(dataBuff);
      dataBuff.clear();
      continue;
    }

    dataBuff += data[i];
  }

  if(!dataBuff.empty()) returnedVector.push_back(dataBuff);

  return returnedVector;
}

void ConfigFile::reload() {
    ConfigFile(this->configFile);
}
