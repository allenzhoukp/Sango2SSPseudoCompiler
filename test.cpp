int main() {
    General* gen = intvLeftGeneral;
    int x = gen->chMagic[0];

    float a = -1.5;

    SangoObject* (*GetObjectByHandle) (int handle) = 0x421A80;
    SangoObject* objptr = GetObjectByHandle(intvAttackerMajor);

    void (*RandomVoid) (int handle) = 0x421A80;
    RandomVoid = 0x421A80;
    RandomVoid(intvAttackerMajor);
    
    return 0;
}
