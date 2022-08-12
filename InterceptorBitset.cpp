
int GetBitSet(int location, int index) {
    return (GetGlobal(location + (index >> 5)) & (1 << (index & 0x1F))) != 0;
}

void SetBitSet(int location, int index, int value) {
    int i = index >> 5;
    if (value)
        SetGlobal(location + i, GetGlobal(location + i) | (value << (index & 0x1F)));
    else 
        SetGlobal(location + i, GetGlobal(location + i) & (~(value << (index & 0x1F))));
}

void InterceptorCallback(int a0, int a1) callsign 41001  {
    int v1;
    int v2;
    int v3;
    asynccall Blood(a1, 16);
    if (!((a1 == intvDefenderMajor))) {
        Hurt(a0, a1, 0);
    } else {
        if (GetGlobal(54)) {
            v3 = (intvMagicAttackValue / GetGlobal(54));
        }
        Hurt(a0, a1, v3);
        if ((v3 > 0)) {
            SetGlobal(54, (GetGlobal(54) * 2));
        }
    }
    v1 = GetObjectContext(a0, 0);
    v1--;
    if (!((v1 <= 0))) {
        SetObjectContext(a0, 0, v1);
    } else {
        v2 = GetObjectContext(a0, 1);
        FreeObject(a0);
        SetBitSet(55, v2, 0);
    }
    return;
}
void CreateInterceptor(int a0, int a1, int a2, int a3)  {
    int v1;
    if (!((GetBitSet(55, a3) != 0))) {
        a0 = BattleXToScreenX(a0);
        a1 = BattleXToScreenX(a1);
        v1 = CreateObjectRaw(a0, a1, 0, a2, 51001);
        SetObjectFlags(v1, (OF_ENEMYFORCE + OF_ENEMYGENERAL));
        SetCallbackProcedure(v1, 41001);
        SetObjectContext(v1, 0, 3);
        SetObjectContext(v1, 1, a3);
        SetBitSet(55, a3, 1);
        return;
    } else {
        return;
    }
}
