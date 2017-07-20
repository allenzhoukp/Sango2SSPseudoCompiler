int function GetObjectByHandle(handle) {
    var num = handle & 0xFFFF;
    if(num < 0x800)
        //return *POBJECT_LIST_ROOT + 0x0A8 * num;
        return GetInt(POBJECT_LIST_ROOT) + 0x0A8 * num;

    return 0; //NULL
}

int function IsFriendly(pTarget, pAttacker) {
    // return pTarget->INFO_pMother == pAttacker->INFO_pMother;
    return GetInt(pTarget + 0x40) == GetInt(pAttacker + 0x40);
}

int function IsFriendly_ByHandle(target, attacker) {
    // return IsFriendly(GetObjectByHandle(target)->SANGO_OBJECT_pMother,
    //                   GetObjectByHandle(attacker)->SANGO_OBJECT_pMother);
    return IsFriendly(GetInt(GetObjectByHandle(target) + 0x84),
                      GetInt(GetObjectByHandle(attacker) + 0x84));
}

int function IsChivary(pInfo) {
    //if(pInfo->INFO_dwFlags & 0x4)
    if(GetInt(pInfo + 0x2C) & 0x4)
        return true;
    return false;
}

int function IsChivary_ByHandle(handle) {
    //return IsChivary(GetObjectByHandle(handle)->SANGO_OBJECT_pMother);
    return IsChivary(GetInt(GetObjectByHandle(handle) + 0x84));
}

int function IsMajor(pInfo) {
    //if(pInfo->INFO_dwFlags & 0x1)
    if(GetInt(pInfo + 0x2C) & 0x1)
        return true;
    return false;
}

int function IsMajor_ByHandle(handle) {
    //return IsMajor(GetObjectByHandle(handle)->SANGO_OBJECT_pMother);
    return IsMajor(GetInt(GetObjectByHandle(handle) + 0x84));
}

// this func is supposed to be private
int function isSolAtkValid_AttackPoint(attackPoint, pTarget, pAttacker) {
    // pTarget->x1 == pTarget->x2 && pTarget->y1 == pTarget->y2
    // A Stopped Object is easier to get hit
    if(GetInt(pTarget + 0x08) == GetInt(pTarget + 0x10) &&
       GetInt(pTarget + 0x0C) == GetInt(pTarget + 0x14))
        attackPoint = attackPoint * 2 / 3;

    var dice = Rand(0, 299);
    if(dice >= attackPoint) { //not hit.
        if(IsChivary(pAttacker)) {
            attackPoint = attackPoint * 2 / 3;
            dice = Rand(0, 299);
            if(dice >= attackPoint)
                return false;
            else
                return true;
        }
        //else attacker is not chivary. not hit.
        return false;
    }
    //hit.
    return true;
}

int function IsSolAtkToSolValid(pTarget, pAttacker) {
    if(IsMajor(pTarget))
        return false;
    if(IsFriendly(pTarget, pAttacker))
        return false;
    //AttackPoint = pAttacker -> INFO_pMother -> BATTLE_USER_INFO_nSoldierAttack
    return isSolAtkValid_AttackPoint(
        GetInt(GetInt(pAttacker + 0x40) + 0x70),
        pTarget, pAttacker);
}

int function IsSolAtkToSolValid_ByHandle(target, attacker) {
    // return IsSolAtkToSolValid(GetObjectByHandle(target) -> SANGO_OBJECT_pMother,
    //                           GetObjectByHandle(attacker) -> SANGO_OBJECT_pMother);
    return IsSolAtkToSolValid(GetInt(GetObjectByHandle(target) + 0x84),
                              GetInt(GetObjectByHandle(attacker) + 0x84));
}

int function GetSolHarmToMajor(pTarget, pAttacker) {
    if(!IsMajor(pTarget))
        return 0;
    if(IsFriendly(pTarget, pAttacker))
        return 0;

    //var strength = pTarget -> MAJOR_INFO_pMother -> BATTLE_USER_INFO_pGeneral ((->)) General_str;
    var strength = GetShort(GetInt(GetInt(pTarget + 0x40) + 0x48) + OFFSET_GENERAL_STR);
    var attackPoint = 150 - strength * 20 / 100;

    //not hit, no harm.
    if(!isSolAtkValid_AttackPoint(attackPoint, pTarget, pAttacker))
        return 0;

    //var soldierAtk = pAttacker -> SOLDIER_INFO_pMother -> BATTLE_USER_INFO_nSoldierAttack;
    var soldierAtk = GetInt(GetInt(pAttacker + 0x40) + 0x70);
    if(IsChivary(pAttacker))
        soldierAtk = soldierAtk * 120 / 100;
    return soldierAtk * 4 / 192;
}

int function GetSolHarmToMajor_ByHandle(target, attacker) {
    // return GetSolHarmToMajor(GetObjectByHandle(target) -> SANGO_OBJECT_pMother,
    //                           GetObjectByHandle(attacker) -> SANGO_OBJECT_pMother);
    return GetSolHarmToMajor(GetInt(GetObjectByHandle(target) + 0x84),
                              GetInt(GetObjectByHandle(attacker) + 0x84));
}
