#pragma once

#include <string>
#include <fstream>
#include <regex>
#include <map>
#include <iostream>

#include <sys/stat.h>

using namespace std;


class ConfigFile {
private:
    const string configFile;

public:
    map<string, map<string, string>> content;

    ConfigFile(const string configFile);

    bool save();

    void clear();

    bool good(string prefix = "nullempty");

    void reload();

    vector<string> explode(string data, char dividerChar);

    map<string, string>& operator[](string index);
};
