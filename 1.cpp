
void test () callsign 899 {
    string s = "³\»u";
    
    Prompt(3, 0, s + "%k%%e%");
    Delay(1);

    Prompt(3, 0, GetGeneralName(intvRightGeneral)+ "%k%%e%");
    Delay(1);

    Prompt(3, 0, (string) (s == GetGeneralName(intvRightGeneral)) + "%k%%e%");
    Delay(1);
}
#include "magic.cpp"