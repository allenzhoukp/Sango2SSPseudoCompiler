// 突石
void function ConvexStone(level) {
	var func_callsign = 10002;
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\010\\*");
	DisablePlayMagic();
	DownBrightness(5, 12);
	SetOverhelming?(GetINV(INTV_ATTACKER_MAJOR), true);
	var x = GetObjectScreenX(INTV_ATTACKER_MAJOR);
	var y = GetObjectScreenY(INTV_ATTACKER_MAJOR);
	SetViewCamera(x, y - 120);
	AddAttackCounter(GetINV(INTV_ATTACKER_MAJOR), 2);
	Delay(20);

	SetObjectAnimate(GetINV(INTV_ATTACKER_MAJOR), 0x10000); //OAF_SPEEL2
	SetOverhelming?(GetINV(INTV_ATTACKER_MAJOR), false);
	Delay(30);

	var dir;
	if(GetINV(INTV_IS_LEFT))
		dir = 0;
	else
		dir = 128;

	x = GetObjectScreenX(INTV_DEFENDER_MAJOR);
	y = GetObjectScreenY(INTV_DEFENDER_MAJOR);
	MoveCamera(40, y - 120, x);
	Delay(15);

	x = GetObjectScreenX(INTV_DEFENDER_MAJOR);
	y = GetObjectScreenY(INTV_DEFENDER_MAJOR);
	var a1;
	if(level == 0)
		a1 = 1;
	else if(level == 1)
		a1 = 3;
	else {
		a1 = 5;
		func_callsign = 10003; //else it is 10002. See the 1st line of func.
	}
	var a7 = 0;
	var a8 = 0;
	var a11 = a7;
	var a12 = a8;
	do {
		//?, func_callsign, direction, object sequence, y, x.
		//-2, -3,           -4,        -5,             -6,-7.
		asynccall ProduceSomethingXY(1, func_callsign, dir, 20001 + (level == 2), y + a8, x + a7);
		asynccall ProduceSomethingXY(0, func_callsign, dir, 20003 + (level == 2), y + a8, x + a7);
		asynccall LockTargetXY(OF_ENEMYGENERAL | OF_ENEMYFORCE,
							   0, 0, 10001, 8, dir, 2501, y + a8, x + a7);
		for(var a2 = 1; a2 < 6; a2 = a2 + 1) {
			var a14 = y + a8 - 72 * 3 / 2 + a2 * 72 / 2; //some y coord
			for(var a3 = 1; a3 < 6; a3 = a3 + 1) {
				var a13 = x + a7 - 96 * 3 / 2 + a3 * 96 / 2; //some x coord
				asynccall LockTargetXY(OF_ENEMYFORCE,
									   0, 0, 10001, 8, dir, 2501, a14, a13);
			}
		}
		Delay(Rand(4, 12));

		while(true) {
			a7 = Rand(-240, 240);
			if(a7 > a11 + 28 || a7 < a11 - 28)
				break;
		}
		while(true) {
			a8 = Rand(-110, 340);
			if(a8 > a12 + 36 || a8 < a12 - 36)
				break;
		}

		a11 = a7;
		a12 = a8;
		a1--;
	} while (a1 > 0);

	Wait("CreateEightWay");
	Delay(60);

	RaiseBrightness(5, 16);
	EnablePlayMagic();

}
