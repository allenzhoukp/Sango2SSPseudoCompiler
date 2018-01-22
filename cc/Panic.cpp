#include "Panic.h"

int Panic::errmsgCount(0);

void Panic::panic (string message, string fileName, int lineNo, int columnNo) {
    printf((fileName + ", " + std::to_string(lineNo) + ":" + std::to_string(columnNo) + " - " + message + "\n").c_str());
    errmsgCount++;
    if(errmsgCount == MAX_ERR_CNT) {
        printf("Error limit exceeded.\n");
        exit(1);
    }
}
