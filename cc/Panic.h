#ifndef PANIC_H
#define PANIC_H

#include <cstdio>
#include <cstdlib>
#include <string>

using std::string;

class Panic {
private:
    static int errmsgCount;
    static const int MAX_ERR_CNT = 20;

public:
    static void panic (string message, string fileName, int lineNo, int columnNo);

};

#endif
