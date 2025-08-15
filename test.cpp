

int g_test0;
int g_test[2];

void InitializeBattle(int a1, int a2, int a3, string a4, string a5, int a6, int a7) {
    return;
}

int main() {

    string xx = "Hello, World!";
    string yy = "Hello, Sango!";
    asynccall InitializeBattle(1, 2, 3, xx, yy, 4, 5);

    General* gen = intvLeftGeneral;
    int x = gen->chMagic[0];
    int y = gen->wHPMax;

    int b = 1;
    float a = 1f;

    SangoObject* (*GetObjectByHandle) (int handle) = 0x421A80;
    SangoObject* objptr = GetObjectByHandle(intvAttackerMajor);

    void (*RandomVoid) (int handle) = 0x421A80;
    RandomVoid = 0x421A80;
    RandomVoid(intvAttackerMajor);

    
    g_test0 = 0x12345678;
    x = g_test0;
    y = g_test[0];
    g_test[1] = 0x87654321;

    float *pF = (float *) 0x444444;
    float fop = (*pF) + 3.6;

    return 0;
}
