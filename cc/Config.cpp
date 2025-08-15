#include "Config.h"

#include <cstdio>
#include <map>
#include <string>
#include <regex>

using std::map;
using std::string;

map<string, string> Config::configs;

void Config::loadConfig() {
    FILE* fin = fopen("config/CONFIG.txt", "r");
    char line[1024];
    std::regex rProp { R"(^([_0-9A-Za-z]+)\s*=\s*([^; \t\r\n]+))" };
    std::cmatch cm;

    while(!feof(fin)) {
        fgets(line, 1024, fin);
        if(!std::regex_search(line, cm, rProp))
            continue;
        string key = cm[1];
        string value = cm[2];
        Config::configs[key] = value;
    }

    fclose(fin);

}

string Config::get(const string& key) {
    auto it = Config::configs.find(key);
    if (it != Config::configs.end()) {
        return it->second;
    }
    return "";
}

void Config::set(const string& key, const string& value) {
    configs[key] = value;
}
