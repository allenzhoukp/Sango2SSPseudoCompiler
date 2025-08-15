#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <map>

using std::string;
using std::map;

class Config {
    static map<string, string> configs;
public:
    static void loadConfig();
    static string get(const string& key);
    static void set(const string& key, const string& value);
};

#endif
