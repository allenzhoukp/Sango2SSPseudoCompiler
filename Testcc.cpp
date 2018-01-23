#define OBJECT_LIST_ROOT (SangoObject**) 0x4A64A0

void function Sol13LongSpcSmokeFade(int smoke) {
    PlaySound(smoke, "S013SND01", 255);
    Delay(80);

    SetObjectFlags(smoke, 262144); //OF_MIXER
    SetObjectBrightness(smoke, 16);
    for(int brightness = 16; brightness > 0; brightness--) {
        Delay(3);
        SetObjectBrightness(smoke, brightness);
    }

    FreeObjectByHandle(smoke);
}

void function Sol13LongSpcAttack (int shootRange) callsign 29013 {
    int soldier = GetScriptLinkedObject();
    int soldierDir = GetObjectDir(soldier);
    if(soldierDir == 0)
        soldierDir = 1;
    else
        soldierDir = -1;

    int soldierScreenX = GetObjectScreenX(soldier);
    int soldierScreenY = GetObjectScreenY(soldier);
    int log[5];

    for(int i = 1; i <= shootRange; i++) {
        int target = GetSoldierHandle(ScreenXToBattleX(soldierScreenX) + soldierDir * i, ScreenYToBattleY(soldierScreenY));
        if(target == 0) {
            log[i - 1] = false;
            continue;
        }

        log[i - 1] = true;

        int targetScreenX = GetObjectScreenX(target);
        int targetScreenY = GetObjectScreenY(target);
        int smoke = CreateObjectRaw(targetScreenX, targetScreenY, 0, 0, 504);

        /*
        __asm {
                PUSHSTR "Sol13LongSpcSmokeFade"
                PUSHARG smoke
                PUSH 0
                PUSH 0
                PUSH 0
                CALLBS
        }
        */

        asynccall Sol13LongSpcSmokeFade(smoke);

        break;
    }

    if(target == 0)
        return;

    int targetDir = GetObjectDir(target);
    KillSoldier(target);
    int soldierSide = GetSoldierSide(soldier);
    SangoObject** pObjectListRoot = OBJECT_LIST_ROOT;
    if (!(GetSoldierCount(soldierSide) == 200)) {
        General* pSelfGeneral = (*pObjectListRoot)[soldier & 0xFFFF].pMother->pGeneral;
        General* pEnemyGeneral = (*pObjectListRoot)[target & 0xFFFF].pMother->pGeneral;
        int selfSoldierType = pSelfGeneral->dwCurrentTroopType;
        int enemySoldierType = pEnemyGeneral->dwCurrentTroopType;

        SetIntv(GetObjectSequence(target), 20);
        SetIntv(1, 19);
        pSelfGeneral->dwCurrentTroopType = enemySoldierType;

        int newSoldier;
        //Chiinty: Requires a special CreateSoldier call
        if((*pObjectListRoot)[soldier & 0xFFFF].pMother->dwFlags & 4)
                newSoldier = CreateSoldier(soldierSide, ScreenXToBattleX(targetScreenX) + 10000, ScreenYToBattleY(targetScreenY));
        else
                newSoldier = CreateSoldier(soldierSide, ScreenXToBattleX(targetScreenX), ScreenYToBattleY(targetScreenY));
        SetObjectCoordinate(newSoldier, targetScreenX, targetScreenY, 0);
        SetObjectDir(newSoldier, targetDir);

        pSelfGeneral->dwCurrentTroopType = selfSoldierType;
        SetIntv(20, 0);
        SetIntv(19, 0);

        FreeObjectByHandle(target);
    }
}
