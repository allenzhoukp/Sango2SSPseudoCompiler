int function IsSolAtkToSolValid (pTarget, pAttacker) {

	var nAttackPoint = pAttacker->pMother->nSoldierAttack;

	if(isFriendly(pTarget, pAttacker))
		return false;

	//A moving object is harder to be hit
	if(pTarget->x1 == pTarget->x2 && pTarget->y1 == pTarget->y2)
		nAttackPoint = nAttackPoint * 2 / 3;

	var dice = Rand(0, 299);
	if(dice >= nAttackPoint) {
		if(pAttacker->dwFlags & 0x4) { //IsChivary?
			nAttackPoint = nAttackPoint * 2 / 3;
			dice = Rand(0, 299);
			if(dice >= nAttackPoint)
				return false;
			else
				return true;
		}
		return false;
	}
	return true;
}
