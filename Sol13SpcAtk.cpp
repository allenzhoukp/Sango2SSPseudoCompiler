#include "sg2io.cpp"
#include "magic.cpp"

//TODO Convert old magic_lib.asm to magic_lib.cpp

void function Sol13LongSpcSmokeFade(smoke) {
    PlaySound(smoke, "S013SND01", 255);
    Delay(80);

    SetObjectFlags(smoke, 262144); //OF_MIXER
    SetObjectBrightness(smoke, 16);
    for(var brightness = 16; brightness > 0; brightness = brightness - 1) {
        Delay(3);
        SetObjectBrightness(smoke, brightness);
    }

    FreeObjectByHandle(smoke);
}

void function Sol13LongSpcAttack () callsign 29013 {
  var soldier = GetScriptLinkedObject();
  var soldierDir = GetObjectDir(soldier);
  if(soldierDir == 0)
    soldierDir = 1;
  else
    soldierDir = -1;

  var soldierScreenX = GetObjectScreenX(soldier);
  var soldierScreenY = GetObjectScreenY(soldier);
  var log[5];

  var i = 1;
  while(i <= 5) {
    var target = GetSoldierHandle(ScreenXToBattleX(soldierScreenX) + soldierDir * i, ScreenYToBattleY(soldierScreenY));
    if(target == 0) {
        i += 1;
        log[i - 1] = false;
        continue;
    }

    log[i - 1] = true;

    var targetScreenX = GetObjectScreenX(target);
    var targetScreenY = GetObjectScreenY(target);
    var smoke = CreateObjectRaw(targetScreenX, targetScreenY, 0, 0, 504);
    __asm {
        PUSHSTR "Sol13LongSpcSmokeFade"
        PUSHARG smoke
        PUSH 0
        PUSH 0
        PUSH 0
        CALLBS
    }
    // asynccall Sol13LongSpcSmokeFade(smoke);

    if(IsSolAtkToSolValid_ByHandle(target, soldier))
      break;
    else
      return;
  }
  if(target == 0)
    goto end;
  var targetDir = GetObjectDir(target);
  KillSoldier(target);
  var soldierSide = GetSoldierSide(soldier);
  if (!(GetSoldierCount(soldierSide) == 200)) {
    var pSelfGeneral = GetInt(GetInt(GetInt(GetObjectByHandle(soldier)+0x84)+0x40)+0x48);
    var pEnemyGeneral = GetInt(GetInt(GetInt(GetObjectByHandle(target)+0x84)+0x40)+0x48);
    var selfSoldierType = GetInt(pSelfGeneral + 0x8B);
    var enemySoldierType = GetInt(pEnemyGeneral + 0x8B);

    SetINV(GetObjectSequence(target), 20);
    SetINV(1, 19);
    SetInt(enemySoldierType, pSelfGeneral + 0x8B);

    var newSoldier;
    //Chivary: Requires a special CreateSoldier call
    if(GetInt(GetInt(GetObjectByHandle(target) + 0x84) + 0x2C) & 4)
        newSoldier = CreateSoldier(soldierSide, ScreenXToBattleX(targetScreenX) + 10000, ScreenYToBattleY(targetScreenY));
    else
        newSoldier = CreateSoldier(soldierSide, ScreenXToBattleX(targetScreenX), ScreenYToBattleY(targetScreenY));
    SetObjectCoordinate(newSoldier, targetScreenX, targetScreenY, 0);
    SetObjectDir(newSoldier, targetDir);

    SetInt(selfSoldierType, pSelfGeneral + 0x8B);
    SetINV(0, 20);
    SetINV(0, 19);

    FreeObjectByHandle(target);
  }
  end:;
}
