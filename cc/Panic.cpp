#include "Panic.h"
#include "Localization.h"

int Panic::errmsgCount(0);

bool Panic::success (true);

void Panic::panic (string message, string fileName, int lineNo, int columnNo) {
    success = false;
    printf((ErrMsg::errHeader + fileName + ", " + std::to_string(lineNo) + ":" + std::to_string(columnNo) + " - " + message + "\n").c_str());
    errmsgCount++;
    if(errmsgCount == MAX_ERR_CNT) {
        printf("Error limit exceeded.\n");
        exit(1);
    }
}
