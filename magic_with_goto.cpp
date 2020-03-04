void DelayAmbientSound(string a0, int a1, int a2) {
LOC_40:
	SetGlobal(1, 1);
	PlaySound1(a0, a1);
	Delay(a2);
	SetGlobal(1, 0);
	return;
}

int GetSoldierMaxX2(int a0) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
LOC_90:
	v1 = GetSoldierCount((a0 ^ 1));
	if (!(v1 > 0)) goto LOC_234;
	v2 = ((v1 / 2) + 1);
	v4 = GetRandomSoldierFromAlive((a0 ^ 1));
	v3 = GetObjectScreenX(v4);
LOC_148:
	v4 = GetRandomSoldierFromAlive((a0 ^ 1));
	v5 = GetObjectScreenX(v4);
	if (!(a0 == 1)) goto LOC_1DC;
	if (!(v3 > v5)) goto LOC_1D4;
	v3 = v5;
LOC_1D4:
	goto LOC_208;
LOC_1DC:
	if (!(v3 < v5)) goto LOC_208;
	v3 = v5;
LOC_208:
	v2--;
	if ((v2 > 0)) goto LOC_148;
	goto LOC_248;
LOC_234:
	return (-1);
LOC_248:
	v3 = ScreenXToBattleX(v3);
	v3 = BattleXToScreenX(v3);
	return v3;
}

int GetSoldierMaxX(int a0) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
LOC_290:
	v1 = GetSoldierCount((a0 ^ 1));
	if (!(v1 > 0)) goto LOC_434;
	v2 = ((v1 / 2) + 1);
	v4 = GetRandomSoldierFromAlive((a0 ^ 1));
	v3 = GetObjectScreenX(v4);
LOC_348:
	v4 = GetRandomSoldierFromAlive((a0 ^ 1));
	v5 = GetObjectScreenX(v4);
	if (!(a0 == 1)) goto LOC_3DC;
	if (!(v3 > v5)) goto LOC_3D4;
	v3 = v5;
LOC_3D4:
	goto LOC_408;
LOC_3DC:
	if (!(v3 < v5)) goto LOC_408;
	v3 = v5;
LOC_408:
	v2--;
	if ((v2 > 0)) goto LOC_348;
	goto LOC_4C8;
LOC_434:
	if (!(a0 == 1)) goto LOC_490;
	v5 = GetObjectScreenX(intvDefenderMajor);
	v3 = (v5 - 320);
	goto LOC_4C8;
LOC_490:
	v5 = GetObjectScreenX(intvDefenderMajor);
	v3 = (v5 + 320);
LOC_4C8:
	return v3;
}

void HoldAllDelay(int a0) {
	SetAttackCounterTo9999();
	Delay(a0);
	RestoreAttackCounter();
	return;
}

void ProduceShadowTime(int a0, int a1) {
	int v1;
	v1 = a1;
LOC_51C:
	CreateShadowObject(a0, 12, 1);
	if (!(!IsObjectExist(a0))) goto LOC_56C;
	goto LOC_59C;
LOC_56C:
	Delay(1);
	v1--;
	if ((v1 >= 0)) goto LOC_51C;
LOC_59C:
	return;
}

void MovingShadow(int a0, int a1, int a2, int a3, int a4) {
	int v1;
	int v2;
	v2 = CreateObjectByReference(a0, a2, a3, 0);
	SetObjectFlags(v2, 33554432);
	SetCallbackProcedure(v2, a4);
	v1 = 1;
LOC_628:
	CreateShadowObject1(v2, 4, 1, 8192);
	SetCoordinateByReference_Cylind(v2, a0, a3, (v1 * 16), 0);
	Delay(1);
	v1++;
	if ((v1 <= a1)) goto LOC_628;
	SetObjectFadeOut(v2, 16, 1);
	return;
}

void LockByCenter(int a0, int a1, int a2, int a3, int a4, int a5, int a6) {
	int v2;
LOC_6F4:
	v2 = CreateObjectByReference(a0, a1, a2, 0);
	SetCoordinateByReference_Cylind(v2, a0, a2, a3, 0);
	SetObjectFlags(v2, a5);
	SetCallbackProcedure(v2, a6);
	Delay(a4);
	FreeObjectByHandle(v2);
	return;
}

void LockTargetXY(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8) {
	int v1;
	Delay(Rand(a6, a7));
	v1 = CreateObjectByReference(intvAttackerMajor, a2, a3, 0);
	SetObjectCoordinate(v1, a0, a1, 0);
	SetObjectFlags(v1, a8);
	SetCallbackProcedure(v1, a5);
	Delay(a4);
	SetObjectFadeOut(v1, 16, 1);
	return;
}

void LockTargetTime2(int a0, int a1, int a2) {
	int v1;
	int v2;
	int v3;
	int v4;
	if (!((a0 == intvDefenderMajor) || (a0 == intvAttackerMajor))) goto LOC_95C;
	if (!(GetMajorHP(GetSoldierSide(a0)) != 0)) goto LOC_944;
	v4 = 40;
	goto LOC_954;
LOC_944:
	v4 = 0;
LOC_954:
	goto LOC_96C;
LOC_95C:
	v4 = 0;
LOC_96C:
	v3 = GetObjectDir(a0);
	v2 = CreateObjectByReference(a0, a1, v3, 0);
	v1 = a2;
LOC_9CC:
	if (!(!IsObjectExist(a0))) goto LOC_9F4;
	goto LOC_A58;
LOC_9F4:
	SetCoordinateByReference_Cylind(v2, a0, 0, 0, v4);
	Delay(1);
	v1--;
	if ((v1 >= 0)) goto LOC_9CC;
LOC_A58:
	SetObjectFadeOut(v2, 16, 1);
	return;
}

void Hurt(int a0, int a1, int a2) {
LOC_A84:
	if (!((a1 == intvAttackerMajor) || (a1 == intvDefenderMajor))) goto LOC_B30;
	ClearObjectFlags(a0, (67108864 + 16777216));
	if (!(a2 <= 0)) goto LOC_B04;
	return;
LOC_B04:
	DoHarmToMajor(a1, 0, a2);
	goto LOC_B54;
LOC_B30:
	PlaySound(a1, "Hit01", 255);
LOC_B54:
	return;
}

void MoveCamera(int a0, int a1, int a2) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
LOC_B5C:
	v2 = (GetCameraX() * 1000);
	v3 = (GetCameraY() * 1000);
	a0 = (a0 * 1000);
	a1 = (a1 * 1000);
	v4 = (a0 - v2);
	v4 = (v4 / a2);
	v5 = (a1 - v3);
	v5 = (v5 / a2);
	v1 = a2;
LOC_C5C:
	if (!(v1 > 0)) goto LOC_D74;
	v2 = (v2 + v4);
	v3 = (v3 + v5);
	SetViewCamera((v2 / 1000), (v3 / 1000));
	if (!(((v2 / 100) == (a0 / 100)) && ((v3 / 100) == (a1 / 100)))) goto LOC_D58;
	v1 = 1;
LOC_D58:
	v1--;
	Delay(1);
	goto LOC_C5C;
LOC_D74:
	return;
}

void LockCameraSimple(int a0, int a1) {
	int v1;
	int v2;
	v2 = GetCameraX();
LOC_D98:
	if (!IsObjectExist(a0)) goto LOC_E78;
	v1 = GetObjectScreenX(a0);
	if (!(((v1 - v2) > 40) || ((v1 - v2) < 40))) goto LOC_E50;
	SetViewCamera(v1, (GetObjectScreenY(a0) + a1));
LOC_E50:
	Delay(1);
	v2 = GetCameraX();
	goto LOC_D98;
LOC_E78:
	return;
}

void LockCameraSimple2(int a0, int a1) {
	int v1;
	int v2;
LOC_E80:
	v2 = GetCameraX();
LOC_E9C:
	if (!IsObjectExist(a0)) goto LOC_F7C;
	v1 = GetObjectScreenX(a0);
	if (!(((v1 - v2) > 10) || ((v1 - v2) < 10))) goto LOC_F54;
	SetViewCamera(v1, (GetObjectScreenY(a0) + a1));
LOC_F54:
	Delay(1);
	v2 = GetCameraX();
	goto LOC_E9C;
LOC_F7C:
	return;
}

void LockCameraLine(int a0, int a1, int a2, int a3) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	v5 = 0;
	v2 = GetCameraX();
	v3 = GetCameraY();
	if (!(a2 == 1)) goto LOC_FF8;
	v4 = 100;
	goto LOC_100C;
LOC_FF8:
	v4 = (-100);
LOC_100C:
	v1 = GetObjectScreenX(a0);
LOC_1028:
	if (!1) goto LOC_11C0;
	if (!(a2 == 1)) goto LOC_10AC;
	v1 = (v1 + 8);
	if (!(v1 > (GetBattleWidthInScreenX() - 270))) goto LOC_10A4;
	goto LOC_11C0;
LOC_10A4:
	goto LOC_10EC;
LOC_10AC:
	v1 = (v1 - 8);
	if (!(v1 < 270)) goto LOC_10EC;
	goto LOC_11C0;
LOC_10EC:
	if (!(((v1 - v2) > 40) || ((v1 - v2) < 40))) goto LOC_116C;
	SetViewCamera((v1 + v4), (v3 + a1));
LOC_116C:
	Delay(1);
	v2 = GetCameraX();
	v5++;
	if (!(v5 > a3)) goto LOC_11B8;
	goto LOC_11C0;
LOC_11B8:
	goto LOC_1028;
LOC_11C0:
	return;
}

void LockCameraLineDelay(int a0, int a1, int a2, int a3) {
	int v1;
	int v2;
	int v3;
	int v4;
	v2 = GetCameraX();
	v3 = GetCameraY();
	if (!(a2 == 1)) goto LOC_122C;
	v4 = 100;
	goto LOC_1240;
LOC_122C:
	v4 = (-100);
LOC_1240:
	if (!IsObjectExist(a0)) goto LOC_13D0;
	v1 = GetObjectScreenX(a0);
	if (!(a2 == 1)) goto LOC_12CC;
	if (!(v1 > GetObjectScreenX(intvDefenderMajor))) goto LOC_12C4;
	goto LOC_13D0;
LOC_12C4:
	goto LOC_12FC;
LOC_12CC:
	if (!(v1 < GetObjectScreenX(intvDefenderMajor))) goto LOC_12FC;
	goto LOC_13D0;
LOC_12FC:
	if (!(((v1 - v2) > 40) || ((v1 - v2) < 40))) goto LOC_137C;
	SetViewCamera((v1 + v4), (v3 + a1));
LOC_137C:
	Delay(1);
	a3--;
	if (!(a3 <= 0)) goto LOC_13B4;
	goto LOC_13D0;
LOC_13B4:
	v2 = GetCameraX();
	goto LOC_1240;
LOC_13D0:
	return;
}

void LockCamera(int a0, int a1, int a2, int a3) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	v6 = 0;
	v4 = GetObjectScreenX(a0);
	v1 = 0;
	v3 = IsObjectExist(a0);
LOC_1438:
	if (!(v3 == 1)) goto LOC_16D0;
	v2 = GetObjectScreenX(a0);
	v4 = GetCameraX();
	if (!(a3 == 0)) goto LOC_1520;
	if (!((v4 - v2) >= a1)) goto LOC_1508;
	v5 = (v2 + a1);
	v6 = (v6 - 1);
	goto LOC_1518;
LOC_1508:
	v5 = v4;
LOC_1518:
	goto LOC_1598;
LOC_1520:
	if (!((v2 - v4) >= a1)) goto LOC_1588;
	v5 = (v2 - a1);
	v6 = (v6 + 1);
	goto LOC_1598;
LOC_1588:
	v5 = v4;
LOC_1598:
	SetViewCamera((v5 + v6), (GetObjectScreenY(a0) + a2));
	Delay(1);
	v3 = IsObjectExist(a0);
	if (!(v1 == 1)) goto LOC_1634;
	v3 = 0;
	goto LOC_16C8;
LOC_1634:
	if (!(a3 == 0)) goto LOC_1690;
	if (!(v4 <= GetObjectScreenX(intvDefenderMajor))) goto LOC_1688;
	v3 = 0;
LOC_1688:
	goto LOC_16C8;
LOC_1690:
	if (!(v4 >= GetObjectScreenX(intvDefenderMajor))) goto LOC_16C8;
	v3 = 0;
LOC_16C8:
	goto LOC_1438;
LOC_16D0:
	if (!(v1 == 0)) goto LOC_1780;
	if (!(a3 == 0)) goto LOC_1720;
	a0 = intvDefenderMajor;
	goto LOC_1730;
LOC_1720:
	a0 = intvDefenderMajor;
LOC_1730:
	asynccall MoveCamera(GetObjectScreenX(a0), (GetObjectScreenY(a0) - 120), 30);
LOC_1780:
	return;
}

void LockCamera2(int a0, int a1, int a2, int a3, int a4) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	int v7;
	int v8;
	v6 = 0;
	v4 = GetObjectScreenX(a0);
	v1 = 0;
	v3 = IsObjectExist(a0);
LOC_17E8:
	if (!(v3 == 1)) goto LOC_1A80;
	v2 = GetObjectScreenX(a0);
	v4 = GetCameraX();
	if (!(a3 == 0)) goto LOC_18D0;
	if (!((v4 - v2) >= a1)) goto LOC_18B8;
	v5 = (v2 + a1);
	v6 = (v6 - 1);
	goto LOC_18C8;
LOC_18B8:
	v5 = v4;
LOC_18C8:
	goto LOC_1948;
LOC_18D0:
	if (!((v2 - v4) >= a1)) goto LOC_1938;
	v5 = (v2 - a1);
	v6 = (v6 + 1);
	goto LOC_1948;
LOC_1938:
	v5 = v4;
LOC_1948:
	SetViewCamera((v5 + v6), (GetObjectScreenY(a0) + a2));
	Delay(1);
	v3 = IsObjectExist(a0);
	if (!(v1 == 1)) goto LOC_19E4;
	v3 = 0;
	goto LOC_1A78;
LOC_19E4:
	if (!(a3 == 0)) goto LOC_1A40;
	if (!(v4 <= GetObjectScreenX(intvDefenderMajor))) goto LOC_1A38;
	v3 = 0;
LOC_1A38:
	goto LOC_1A78;
LOC_1A40:
	if (!(v4 >= GetObjectScreenX(intvDefenderMajor))) goto LOC_1A78;
	v3 = 0;
LOC_1A78:
	goto LOC_17E8;
LOC_1A80:
	v7 = GetCameraX();
	v8 = GetCameraY();
LOC_1AA8:
	if (!IsObjectExist(a4)) goto LOC_1B78;
	v2 = GetObjectScreenX(a4);
	if (!(a3 == 1)) goto LOC_1B34;
	if (!(v2 >= (v7 - 40))) goto LOC_1B2C;
	goto LOC_1B78;
LOC_1B2C:
	goto LOC_1B64;
LOC_1B34:
	if (!(v2 <= (v7 + 40))) goto LOC_1B64;
	goto LOC_1B78;
LOC_1B64:
	Delay(2);
	goto LOC_1AA8;
LOC_1B78:
	v8 = (v8 - GetObjectScreenY(a4));
	if (!IsObjectExist(a4)) goto LOC_1BD4;
	LockCameraSimple2(a4, v8);
LOC_1BD4:
	return;
}

void FireMan(int a0, int a1, int a2) {
	int v1;
	int v2;
	int v3;
	v2 = GetObjectDir(a0);
	v3 = CreateObjectByReference(a0, a1, v2, 0);
	v1 = 0;
LOC_1C44:
	if (!(v1 < a2)) goto LOC_1CE0;
	SetCoordinateByReference_Cylind(v3, a0, 0, 0, 0);
	Delay(1);
	if (!(IsObjectExist(a0) == 0)) goto LOC_1CD0;
	goto LOC_1CE0;
LOC_1CD0:
	v1++;
	goto LOC_1C44;
LOC_1CE0:
	SetObjectFadeOut(v3, 16, 1);
	return;
}

void CreateBlood(int a0, int a1, int a2) {
	int v1;
	v1 = CreateObjectByReference(a0, 10012, 0, a1);
	SetObjectScale(v1, 32768, 32768);
	SetObjectSpeed_Sphere(v1, a2, Rand(24, 40), 7);
	Delay(2);
	SetObjectFadeOut(v1, 8, 1);
	return;
}

void Blood(int a0, int a1) {
	int v1;
	int v2;
	v2 = (128 * Rand(0, 1));
	v1 = 0;
LOC_1E2C:
	if (!(v1 < 32)) goto LOC_1EA4;
	asynccall CreateBlood(a0, a1, v2);
	Delay(Rand(0, 1));
	v1++;
	goto LOC_1E2C;
LOC_1EA4:
	return;
}

void SmallFireBall(int a0, int a1, int a2, int a3) {
	int v1;
	int v2;
	PlaySound(a1, "hit01", 255);
	v2 = 0;
LOC_1EE8:
	if (!(v2 < a2)) goto LOC_202C;
	v1 = CreateObjectByReference(a1, 10003, Rand(0, 255), a3);
	SetObjectScale(v1, 12288, 12288);
	SetObjectSpeed_Cylind(v1, Rand(4, 12), Rand((-2), 8));
	SetObjectFriction1(v1, 8192);
	SetObjectFadeOut(v1, 32, 1);
	SetObjectScaleShrink(v1, 256);
	v2++;
	goto LOC_1EE8;
LOC_202C:
	return;
}

void SmallFireBall2(int a0, int a1, int a2, int a3) {
	int v1;
	int v2;
	int v3;
LOC_2034:
	PlaySound(a1, "hit01", 255);
	v2 = 0;
LOC_2070:
	if (!(v2 < a2)) goto LOC_21D8;
	v1 = CreateObjectByReference(a1, 10003, Rand(0, 255), a3);
	v3 = Rand(8192, 12288);
	SetObjectScale(v1, v3, v3);
	SetObjectSpeed_Cylind(v1, Rand(3, 7), Rand((-1), 5));
	SetObjectFriction1(v1, 8192);
	SetObjectFadeOut(v1, 32, 1);
	SetObjectScaleShrink(v1, 256);
	v2++;
	goto LOC_2070;
LOC_21D8:
	return;
}

void SmallFireBall3(int a0, int a1, int a2, int a3) {
	int v1;
	int v2;
	PlaySound(a1, "hit01", 255);
	v2 = 0;
LOC_221C:
	if (!(v2 < a2)) goto LOC_2360;
	v1 = CreateObjectByReference(a1, 10013, Rand(0, 255), a3);
	SetObjectScale(v1, 12288, 12288);
	SetObjectSpeed_Cylind(v1, Rand(4, 12), Rand((-2), 8));
	SetObjectFriction1(v1, 8192);
	SetObjectFadeOut(v1, 32, 1);
	SetObjectScaleShrink(v1, 256);
	v2++;
	goto LOC_221C;
LOC_2360:
	return;
}

void SmallFireBall4(int a0, int a1, int a2, int a3) {
	int v1;
	int v2;
	PlaySound(a1, "hit01", 255);
	v2 = 0;
LOC_23A4:
	if (!(v2 < a2)) goto LOC_24E8;
	v1 = CreateObjectByReference(a1, 10014, Rand(0, 255), a3);
	SetObjectScale(v1, 12288, 12288);
	SetObjectSpeed_Cylind(v1, Rand(4, 12), Rand((-2), 8));
	SetObjectFriction1(v1, 8192);
	SetObjectFadeOut(v1, 32, 1);
	SetObjectScaleShrink(v1, 256);
	v2++;
	goto LOC_23A4;
LOC_24E8:
	return;
}

void DownBrightness(int a0, int a1) {
	int v1;
LOC_24F0:
	v1 = GetBackgroundBrightness();
LOC_250C:
	if (!(v1 > a0)) goto LOC_2558;
	Delay(a1);
	v1--;
	SetBackgroundBrightness(v1);
	goto LOC_250C;
LOC_2558:
	SetBackgroundBrightness(16);
	LockBrightness(a0);
	SetGlobal(2, a0);
	return;
}

void RaiseBrightness(int a0, int a1) {
	int v1;
LOC_2598:
	UnlockBrightness();
	v1 = GetGlobal(2);
	SetBackgroundBrightness(v1);
LOC_25D0:
	if (!(v1 < a0)) goto LOC_261C;
	Delay(a1);
	v1++;
	SetBackgroundBrightness(v1);
	goto LOC_25D0;
LOC_261C:
	SetBackgroundBrightness(a0);
	return;
}

void CreateDot(int a0) {
	int v1;
	int v2;
	int v3;
	int v4;
	v2 = GetObjectScreenX(a0);
	v3 = GetObjectScreenY(a0);
	v4 = GetObjectScreenZ(a0);
	Delay(Rand(2, 8));
	v1 = CreateObjectByReference(a0, 10004, 0, 0);
	SetObjectCoordinate(v1, (v2 + Rand((-60), 60)), (v3 + Rand((-60), 60)), (v4 + Rand((-60), 60)));
	ApproachObjectTowards(v1, a0, Rand(3, 5));
	SetObjectFadeOut(v1, 16, 1);
	return;
}

void CreateDots(int a0) {
	int v1;
	int v2;
	v2 = 0;
LOC_27FC:
	if (!(v2 < 10)) goto LOC_28B0;
	v1 = 0;
LOC_2828:
	if (!(v1 < 4)) goto LOC_28A0;
	asynccall CreateDot(a0);
	Delay(Rand(2, 4));
	v1++;
	goto LOC_2828;
LOC_28A0:
	v2++;
	goto LOC_27FC;
LOC_28B0:
	return;
}

void CreateLine(int a0, int a1) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	v2 = GetObjectScreenX(a0);
	v3 = GetObjectScreenY(a0);
	v4 = GetObjectScreenZ(a0);
	Delay(Rand(2, 8));
	switch (a1) { 
		case 0: goto LOC_2B18;
		case 1: goto LOC_2B80;
		case 2: goto LOC_2C04;
		case 3: goto LOC_2C6C;
		case 4: goto LOC_2CF0;
		case 5: goto LOC_2D58;
		case 6: goto LOC_2DDC;
		case 7: goto LOC_2E44;
	}
	goto LOC_2EC8;
LOC_2B18:
	v6 = 10005;
	a1 = 128;
	v5 = Rand(51, 72);
	v2 = (v2 + v5);
	goto LOC_2EC8;
LOC_2B80:
	v6 = 10006;
	a1 = 128;
	v5 = Rand(52, 77);
	v2 = (v2 + v5);
	v4 = (v4 + v5);
	goto LOC_2EC8;
LOC_2C04:
	v6 = 10007;
	a1 = 0;
	v5 = Rand(50, 71);
	v4 = (v4 + v5);
	goto LOC_2EC8;
LOC_2C6C:
	v6 = 10006;
	a1 = 0;
	v5 = Rand(50, 70);
	v2 = (v2 - v5);
	v4 = (v4 + v5);
	goto LOC_2EC8;
LOC_2CF0:
	v6 = 10005;
	a1 = 0;
	v5 = Rand(52, 70);
	v2 = (v2 - v5);
	goto LOC_2EC8;
LOC_2D58:
	v6 = 10009;
	a1 = 0;
	v5 = Rand(52, 73);
	v2 = (v2 - v5);
	v4 = (v4 - v5);
	goto LOC_2EC8;
LOC_2DDC:
	v6 = 10008;
	a1 = 0;
	v5 = Rand(51, 77);
	v4 = (v4 - v5);
	goto LOC_2EC8;
LOC_2E44:
	v6 = 10009;
	a1 = 128;
	v5 = Rand(50, 70);
	v2 = (v2 + v5);
	v4 = (v4 - v5);
	goto LOC_2EC8;
LOC_2EC8:
	v1 = CreateObjectByReference(a0, v6, a1, 0);
	SetObjectCoordinate(v1, v2, v3, v4);
	ApproachObjectTowards(v1, a0, Rand(3, 5));
	SetObjectFadeOut(v1, 16, 1);
	return;
}

void CreateLines(int a0) {
	int v1;
	int v2[20];
	v1 = 0;
LOC_28D0:
	if (!(v1 < 20)) goto LOC_2990;
	v2[v1] = (Rand(0, 1023) & 7);
	if (!(v1 > 0)) goto LOC_2980;
	if (!(v2[v1] == v2[(v1 - 1)])) goto LOC_2980;
	v1--;
LOC_2980:
	v1++;
	goto LOC_28D0;
LOC_2990:
	v1 = 0;
LOC_29A0:
	if (!(v1 < 20)) goto LOC_2A20;
	asynccall CreateLine(a0, v2[v1]);
	Delay(Rand(6, 8));
	v1++;
	goto LOC_29A0;
LOC_2A20:
	return;
}

void PowerExplode(int a0, int a1) {
	int v1;
	v1 = CreateObjectByReference(a0, 10011, 0, 0);
	SetObjectSpeed_Sphere(v1, 0, (a1 * 32), 2);
	CreateShadowObject(v1, 4, 2);
	Delay(5);
	SetObjectFadeOut(v1, 16, 1);
	return;
}

void KeepPower(int a0, int a1) {
	int v1;
	int v2;
	if (!(a1 == 1)) goto LOC_309C;
	a1 = 1;
	goto LOC_30B0;
LOC_309C:
	a1 = (-1);
LOC_30B0:
	v1 = CreateObjectByReference(a0, 10010, 0, 0);
	a1 = (a1 * 47);
	SetCoordinateByReference_Cylind(v1, a0, 0, a1, 110);
	SetObjectOpacity(v1, 12);
	asynccall CreateDots(v1);
	Delay(10);
	asynccall CreateLines(v1);
	Delay(160);
	v2 = 11;
LOC_31D0:
	if (!(v2 >= 0)) goto LOC_3224;
	SetObjectOpacity(v1, v2);
	Delay(1);
	v2--;
	goto LOC_31D0;
LOC_3224:
	return;
}

void StepShow(int a0, int a1) {
	int v1;
	SetObjectFlags(a0, 262144);
	SetObjectOpacity(a0, 0);
	Delay(20);
	v1 = 1;
LOC_3288:
	if (!(v1 <= 16)) goto LOC_32DC;
	Delay(a1);
	SetObjectOpacity(a0, v1);
	v1++;
	goto LOC_3288;
LOC_32DC:
	ClearObjectFlags(a0, 262144);
	return;
}

void HitGeneral(int a0, int a1, int a2, int a3, int a4, int a5) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
LOC_3300:
	if (!GetGlobal(3)) goto LOC_3320;
	return;
LOC_3320:
	SetGlobal(3, 1);
	v2 = CreateObjectByReference(a1, 2501, 0, 0);
	v3 = GetObjectScreenX(a1);
	v4 = GetObjectScreenY(a1);
	SetObjectCoordinate(v2, v3, v4, a4);
	v1 = 0;
LOC_33D8:
	if (!(v1 < 7)) goto LOC_35AC;
	v5 = CreateObjectByReference(v2, a2, 0, 0);
	SetObjectScale(v5, 49152, 49152);
	MoveObject(v5, Rand((-48), 48), 0, Rand((-48), 48));
	switch (a3) { 
		case 1: goto LOC_34E0;
		case 2: goto LOC_3514;
		case 3: goto LOC_3548;
	}
	goto LOC_357C;
LOC_34E0:
	asynccall SmallFireBall(a0, a1, 8, a4);
	goto LOC_357C;
LOC_3514:
	asynccall SmallFireBall3(a0, a1, 8, a4);
	goto LOC_357C;
LOC_3548:
	asynccall SmallFireBall4(a0, a1, 8, a4);
	goto LOC_357C;
LOC_357C:
	Delay(Rand(3, 7));
	v1++;
	goto LOC_33D8;
LOC_35AC:
	FreeObjectByHandle(v2);
	Delay(a5);
	SetGlobal(3, 0);
	return;
}

void DisableAttack(int a0, int a1, int a2) {
	ClearObjectFlags(a0, 251658240);
	Delay(a2);
	SetObjectFlags(a0, a1);
	return;
}

void NoMoreSoldierCallback(int a0, int a1) callsign 1000 {
	int v1;
	int v2;
	PlaySound1("m000snd01", 255);
	ClearObjectFlags(a0, 67108864);
	SetObjectAnimate(a1, 16);
	v2 = GetObjectDir(a0);
	v1 = CreateObjectByReference(a0, 10018, v2, 16);
	SetObjectGravity(v1, 10752);
	if (!(intvIsLeft == 1)) goto LOC_372C;
	v2 = 45;
	goto LOC_373C;
LOC_372C:
	v2 = 83;
LOC_373C:
	SetObjectSpeed_Sphere(v1, 0, v2, 5);
	SetObjectFadeOut(v1, 16, 3);
	FreeObjectByHandle(a0);
	return;
}

void CastFail(int a0, int a1) {
	int v1;
LOC_37A8:
	v1 = CreateObjectByReference(a0, 10017, a1, 256);
	SetObjectFlags(v1, 67108864);
	SetCallbackProcedure(v1, 1000);
	SetObjectAnimate(a0, 32768);
	SetOverwhelming(a0, 0);
	Delay(60);
	return;
}

void HalfMoonCallback(int a0, int a1) callsign 1001 {
	Hurt(a0, a1, intvMagicAttackValue);
	if (!(a1 == intvDefenderMajor)) goto LOC_3AB0;
	SetObjectFadeOut(a0, 2, 1);
	HitGeneral(a0, a1, 11002, 1, 48, 0);
	goto LOC_3B08;
LOC_3AB0:
	asynccall FireMan(a1, 10015, 60);
	asynccall SmallFireBall(a0, a1, 8, 0);
LOC_3B08:
	return;
}

void CreateHalfMoonSmoke(int a0, int a1, int a2, int a3, int a4) {
	int v1;
	if (!(intvIsLeft == 1)) goto LOC_3D70;
	v1 = CreateObjectByReference(a0, 11003, 0, 0);
	goto LOC_3DA4;
LOC_3D70:
	v1 = CreateObjectByReference(a0, 11003, 128, 0);
LOC_3DA4:
	MoveObject(v1, a1, a2, a3);
	SetObjectScale(v1, 32768, 20480);
	SetObjectSpeed_Cylind(v1, 14, Rand(1, 2));
	Delay(1);
	SetObjectFadeOut(v1, 16, 1);
	SetObjectScaleShrink(v1, ((-2048) - a4));
LOC_3E88:
	if (!IsObjectExist(v1)) goto LOC_3EFC;
	Delay(1);
	if (!(IsObjectExist(a0) == 0)) goto LOC_3EF4;
	FreeObjectByHandle(v1);
	goto LOC_3EFC;
LOC_3EF4:
	goto LOC_3E88;
LOC_3EFC:
	return;
}

void ShowHalfMoonSmoke(int a0) {
	int v1;
	v1 = 0;
LOC_3B28:
	if (!IsObjectExist(a0)) goto LOC_3D08;
	if (!(intvIsLeft == 1)) goto LOC_3C14;
	asynccall CreateHalfMoonSmoke(a0, ((7 * Rand((-8), (-6))) - ((v1 * 2) * Rand(2, 4))), 0, (-2), (256 * v1));
	goto LOC_3CB8;
LOC_3C14:
	asynccall CreateHalfMoonSmoke(a0, ((7 * Rand(8, 6)) + ((v1 * 2) * Rand(2, 4))), 0, (-2), (256 * v1));
LOC_3CB8:
	Delay(Rand(1, 1));
	v1 = ((v1 + 1) % 12);
	goto LOC_3B28;
LOC_3D08:
	return;
}

void HalfMoonAll(int a0, int a1, int a2, int a3, int a4) {
	int v1;
	int v2;
	v1 = CreateObjectByReference(a0, 11001, a1, 0);
	SetCoordinateByReference_Cylind(v1, a0, a3, a4, 1);
	SetObjectSpeed_Sphere(v1, a1, 0, a2);
	SetObjectFlags(v1, 33554432);
	SetCallbackProcedure(v1, 1001);
	asynccall ShowHalfMoonSmoke(v1);
	v2 = 600;
LOC_3978:
	if (!(!IsObjectExist(a0))) goto LOC_39A8;
	v2 = 0;
LOC_39A8:
	Delay(1);
	v2--;
	if ((v2 >= 0)) goto LOC_3978;
	Delay(120);
	SetObjectFadeOut(v1, 16, 1);
	return;
}

void HalfMoon(int a0) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	int v7;
	int v8;
	int v9;
	int v10;
	int v11;
	int v12;
LOC_3F04:
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\001\\*");
	DisablePlayMagic();
	DownBrightness(12, 5);
	v1 = intvAttackerMajor;
	v2 = intvDefenderMajor;
	v3 = intvIsLeft;
	v11 = 0;
	if (!(Rand(0, 3) == 3)) goto LOC_3FEC;
	v11 = Rand(0, 90);
LOC_3FEC:
	v6 = 18;
	if (!(v3 == 1)) goto LOC_4044;
	v5 = 0;
	v12 = (-1);
	goto LOC_4064;
LOC_4044:
	v5 = 128;
	v12 = 1;
LOC_4064:
	switch (a0) { 
		case 0: goto LOC_409C;
		case 1: goto LOC_40C0;
		case 2: goto LOC_40E4;
	}
	goto LOC_4108;
LOC_409C:
	v9 = (120 - v11);
	goto LOC_4108;
LOC_40C0:
	v9 = (210 - v11);
	goto LOC_4108;
LOC_40E4:
	v9 = (330 - v11);
	goto LOC_4108;
LOC_4108:
	SetOverwhelming(v1, 1);
	v7 = GetObjectScreenX(v1);
	v8 = GetObjectScreenY(v1);
	SetViewCamera((v7 - (v12 * 180)), (v8 - v9));
	AddAttackCounter(v1, 2);
	Delay(15);
	SetObjectAnimate(v1, 4096);
	Delay(20);
	SetOverwhelming(v1, 0);
	PlaySound(v1, "m001snd01", 255);
	v4 = CreateObjectByReference(v1, 11001, v5, 0);
	SetCoordinateByReference_Cylind(v4, v1, v5, 64, 1);
	SetObjectSpeed_Sphere(v4, v5, 0, v6);
	SetObjectFlags(v4, (16777216 | 33554432));
	SetCallbackProcedure(v4, 1001);
	asynccall ShowHalfMoonSmoke(v4);
	switch (a0) { 
		case 0: goto LOC_4368;
		case 1: goto LOC_43A0;
		case 2: goto LOC_4468;
	}
	goto LOC_45C0;
LOC_4368:
	asynccall LockCamera(v4, 180, (-v9), v3);
	goto LOC_45C0;
LOC_43A0:
	asynccall HalfMoonAll(v4, v5, v6, 48, (v12 * 140));
	asynccall HalfMoonAll(v4, v5, v6, 208, (v12 * 140));
	asynccall LockCamera(v4, 180, (-v9), v3);
	goto LOC_45C0;
LOC_4468:
	asynccall HalfMoonAll(v4, v5, v6, 48, (140 * v12));
	asynccall HalfMoonAll(v4, v5, v6, 48, (280 * v12));
	asynccall HalfMoonAll(v4, v5, v6, 208, (140 * v12));
	asynccall HalfMoonAll(v4, v5, v6, 208, (280 * v12));
	asynccall LockCamera(v4, 180, (-v9), v3);
	goto LOC_45C0;
LOC_45C0:
	v10 = IsObjectExist(v4);
	if (!(v10 == 1)) goto LOC_4660;
	v7 = GetObjectScreenX(v4);
	if (!((v7 <= 0) || (v7 >= GetBattleWidthInScreenX()))) goto LOC_4660;
	FreeObjectByHandle(v4);
LOC_4660:
	Delay(1);
	if ((v10 != 0)) goto LOC_45C0;
	Delay(70);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void ProduceSoldier(int a0, int a1) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	int v7;
	v4 = 0;
	v1 = 0;
	v6 = GetBattleWidth();
	v7 = GetBattleHeight();
LOC_4710:
	v2 = (v7 / 2);
LOC_472C:
	if (!(a0 == 1)) goto LOC_4774;
	v3 = GetForceHandleByXY(v1, v2);
	goto LOC_47B0;
LOC_4774:
	v3 = GetForceHandleByXY(((v6 - 1) - v1), v2);
LOC_47B0:
	if (!(v3 == 0)) goto LOC_48E4;
	if (!(a0 == 1)) goto LOC_4848;
	v5 = CreateSoldier(a0, v1, v2);
	asynccall StepShow(v5, 10);
	goto LOC_48B8;
LOC_4848:
	v5 = CreateSoldier(a0, ((v6 - 1) - v1), v2);
	asynccall StepShow(v5, 10);
LOC_48B8:
	v4++;
	if (!(v4 >= a1)) goto LOC_48E4;
	return;
LOC_48E4:
	if (!(intvIsLeft == 1)) goto LOC_4944;
	v3 = GetForceHandleByXY(v1, ((v7 - 1) - v2));
	goto LOC_4998;
LOC_4944:
	v3 = GetForceHandleByXY(((v6 - 1) - v1), ((v7 - 1) - v2));
LOC_4998:
	if (!(v3 == 0)) goto LOC_4AFC;
	if (!(a0 == 1)) goto LOC_4A48;
	v5 = CreateSoldier(a0, v1, ((v7 - 1) - v2));
	asynccall StepShow(v5, 10);
	goto LOC_4AD0;
LOC_4A48:
	v5 = CreateSoldier(a0, ((v6 - 1) - v1), ((v7 - 1) - v2));
	asynccall StepShow(v5, 10);
LOC_4AD0:
	v4++;
	if (!(v4 >= a1)) goto LOC_4AFC;
	return;
LOC_4AFC:
	v2--;
	if ((v2 > 0)) goto LOC_472C;
	v1++;
	if ((v1 <= v6)) goto LOC_4710;
	return;
}

void MoreSoldier(int a0, int a1) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	int v7;
LOC_4B4C:
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\002\\*");
	DisablePlayMagic();
	DownBrightness(8, 5);
	v1 = intvAttackerMajor;
	v3 = intvIsLeft;
	SetOverwhelming(intvAttackerMajor, 1);
	v4 = GetObjectScreenX(v1);
	v5 = GetObjectScreenY(v1);
	SetViewCamera(v4, (v5 - 120));
	AddAttackCounter(v1, 2);
	Delay(20);
	if (!(v3 == 1)) goto LOC_4C98;
	v2 = 0;
	goto LOC_4CA8;
LOC_4C98:
	v2 = 128;
LOC_4CA8:
	v6 = GetMajorLevel(v3);
	if (!(v6 > 40)) goto LOC_4CF0;
	v6 = 40;
LOC_4CF0:
	v7 = ((v6 * 5) - GetSoldierCount(v3));
	if (!(v7 < 0)) goto LOC_4D50;
	v7 = 0;
LOC_4D50:
	if (!(v7 > a0)) goto LOC_4D7C;
	v7 = a0;
LOC_4D7C:
	if (!(v7 != 0)) goto LOC_4F44;
	asynccall ProduceSoldier(v3, v7);
	SetObjectAnimate(v1, 32768);
	Delay(24);
	SetObjectAnimate(v1, 65536);
	SetOverwhelming(v1, 0);
	Delay(6);
	PlaySound1("m002snd01", 255);
	PlaySound1("m002snd03", 255);
	if (!(intvIsLeft == 0)) goto LOC_4EAC;
	MoveCamera(6576, 0, 40);
	goto LOC_4ECC;
LOC_4EAC:
	MoveCamera(240, 0, 40);
LOC_4ECC:
	Delay(100);
	v4 = GetObjectScreenX(intvAttackerMajor);
	v5 = GetObjectScreenY(intvAttackerMajor);
	MoveCamera(v4, (v5 - 120), 20);
	goto LOC_4F5C;
LOC_4F44:
	CastFail(v1, v2);
LOC_4F5C:
	Delay(8);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void BombLight(int a0, int a1) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	v2 = GetObjectDir(a0);
	switch (a1) { 
		case 0: goto LOC_4FFC;
		case 1: goto LOC_50A8;
		case 2: goto LOC_5138;
		case 3: goto LOC_51C8;
	}
	goto LOC_5468;
LOC_4FFC:
	v1 = CreateObjectByReference(a0, 7019, v2, 50);
	SetObjectOpacity(v1, 6);
	SetObjectScale(v1, 131072, 131072);
	Delay(10);
	SetObjectFadeOut(v1, 16, 1);
	goto LOC_5468;
LOC_50A8:
	v1 = CreateObjectByReference(a0, 13012, v2, 50);
	SetObjectScale(v1, 40960, 40960);
	Delay(20);
	SetObjectFadeOut(v1, 16, 1);
	goto LOC_5468;
LOC_5138:
	v1 = CreateObjectByReference(a0, 7021, v2, 50);
	SetObjectScale(v1, 131072, 131072);
	Delay(20);
	SetObjectFadeOut(v1, 16, 1);
	goto LOC_5468;
LOC_51C8:
	v1 = CreateObjectByReference(a0, 7046, v2, 80);
	SetObjectScale(v1, 32768, 32768);
	Delay(4);
	SetObjectFadeOut(v1, 16, 1);
	Delay(4);
	v1 = CreateObjectByReference(a0, 7047, v2, 80);
	if (!(v2 == 0)) goto LOC_52E0;
	MoveObject(v1, 16, 0, 0);
	goto LOC_5310;
LOC_52E0:
	MoveObject(v1, (-16), 0, 0);
LOC_5310:
	v4 = 32768;
	v5 = 65536;
	SetObjectScale(v1, v4, v5);
	SetObjectFadeOut(v1, 32, 1);
	v3 = 0;
LOC_5388:
	if (!(v3 < 16)) goto LOC_5460;
	SetObjectScale(v1, v4, v5);
	Delay(1);
	v4 = (v4 + 3072);
	v5 = (v5 - 3840);
	if (!(v3 == 4)) goto LOC_5450;
	SetObjectSpeed_Cylind(v1, (-1), 0);
LOC_5450:
	v3++;
	goto LOC_5388;
LOC_5460:
	goto LOC_5468;
LOC_5468:
	return;
}

void ShootObjectCallback(int a0, int a1) callsign 3001 {
	int v8;
	int v9;
	int v10;
	if (!(a1 == intvDefenderMajor)) goto LOC_578C;
	v8 = GetObjectContext(a0, 0);
	v9 = GetObjectContext(a0, 1);
	v10 = GetObjectDir(a0);
	if (!(v8 == 1)) goto LOC_5530;
	DecreaseGatherTick(a1, 50);
LOC_5530:
	Hurt(a0, a1, intvMagicAttackValue);
	if (!(v8 == 2)) goto LOC_5598;
	SetObjectFadeOut(a0, 16, 8);
	goto LOC_55BC;
LOC_5598:
	SetObjectFadeOut(a0, 2, 1);
LOC_55BC:
	if (!((v8 == 0) && (v9 == 0))) goto LOC_5630;
	HitGeneral(a0, a1, 13009, 1, 60, 0);
	goto LOC_5784;
LOC_5630:
	if (!((v8 == 1) && (v9 == 0))) goto LOC_56A4;
	HitGeneral(a0, a1, 13009, 1, 60, 0);
	goto LOC_5784;
LOC_56A4:
	if (!((v8 == 2) && (v9 == 0))) goto LOC_5718;
	HitGeneral(a0, a1, 13009, 1, 60, 0);
	goto LOC_5784;
LOC_5718:
	if (!((v8 == 0) && (v9 == 1))) goto LOC_5784;
	HitGeneral(a0, a1, 13008, 2, 60, 0);
LOC_5784:
	goto LOC_57D8;
LOC_578C:
	Hurt(a0, a1, 0);
	asynccall Blood(a1, 16);
LOC_57D8:
	return;
}

void ZoomFreq(int a0, int a1) {
	int v1;
	v1 = 0;
LOC_57F8:
	if (!IsObjectExist(a0)) goto LOC_58B4;
	if (!(v1 == 0)) goto LOC_586C;
	SetObjectScale(a0, 40960, 40960);
	v1 = 1;
	goto LOC_58A0;
LOC_586C:
	SetObjectScale(a0, 61440, 61440);
	v1 = 0;
LOC_58A0:
	Delay(a1);
	goto LOC_57F8;
LOC_58B4:
	return;
}

void CosZoomY(int a0, int a1) {
	int v1;
	int v2;
	v2 = 0;
LOC_58D4:
	if (!IsObjectExist(a0)) goto LOC_5984;
	v1 = CalcCos(v2, a1);
	SetObjectScale(a0, 32768, v1);
	Delay(1);
	v2 = (v2 + 16);
	v2 = (v2 & 255);
	goto LOC_58D4;
LOC_5984:
	return;
}

void ScaleShadow(int a0, int a1, int a2, int a3, int a4, int a5) {
	int v1;
	if (!(a5 == 0)) goto LOC_59B8;
	return;
LOC_59B8:
	v1 = CreateObjectByReference(a0, a1, a2, 0);
	if (!(a2 == 0)) goto LOC_5A40;
	MoveObject(v1, (-8), 0, 0);
	goto LOC_5A6C;
LOC_5A40:
	MoveObject(v1, 8, 0, 0);
LOC_5A6C:
	SetObjectSpeed_Sphere(v1, a2, 0, a3);
	SetObjectFlags(v1, 262144);
	SetObjectOpacity(v1, a5);
	SetObjectScale(v1, a4, a4);
	Delay(2);
	SetObjectFadeOut(v1, 60, 1);
	SetObjectScaleShrink(v1, (-(8192 / 12)));
LOC_5B50:
	if (!IsObjectExist(a0)) goto LOC_5BB0;
	if (!(IsObjectExist(v1) == 0)) goto LOC_5B9C;
	return;
LOC_5B9C:
	Delay(4);
	goto LOC_5B50;
LOC_5BB0:
	FreeObjectByHandle(v1);
	return;
}

void FireTail(int a0, int a1, int a2) {
	int v1;
	v1 = CreateObjectByReference(a0, 2501, a1, 0);
	if (!(a1 == 0)) goto LOC_5C5C;
	MoveObject(v1, (-16), 0, 0);
	goto LOC_5C88;
LOC_5C5C:
	MoveObject(v1, 16, 0, 0);
LOC_5C88:
	SetObjectSpeed_Sphere(v1, a1, 0, a2);
LOC_5CB4:
	if (!IsObjectExist(a0)) goto LOC_5D34;
	asynccall ScaleShadow(v1, 13010, a1, (a2 - 2), 20480, 14);
	Delay(10);
	goto LOC_5CB4;
LOC_5D34:
	FreeObjectByHandle(v1);
	return;
}

void FlyLight(int a0, int a1, int a2, int a3, int a4) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	v5 = 0;
	v3 = GetObjectDir(a0);
	v2 = CreateObjectByReference(a0, a1, v3, 0);
	SetCoordinateByReference_Cylind(v2, a0, v3, 0, 0);
	SetObjectSpeed_Sphere(v2, v3, 0, 16);
	if (!(a3 == 0)) goto LOC_5ECC;
	if (!(a4 == 0)) goto LOC_5EA8;
	SetObjectScale(v2, 49152, 49152);
	asynccall FireTail(v2, v3, 16);
	goto LOC_5EC4;
LOC_5EA8:
	if (!(a4 == 2)) goto LOC_5EC4;
LOC_5EC4:
	goto LOC_6028;
LOC_5ECC:
	if (!(a4 == 0)) goto LOC_6028;
	v5 = CreateObjectByReference(v2, 13011, v3, 0);
	SetCoordinateByReference_Cylind(v5, v2, v3, 0, 0);
	SetObjectSpeed_Sphere(v5, v3, 0, 16);
	if (!(intvIsLeft == 1)) goto LOC_5FCC;
	MoveObject(v5, 9, 0, 0);
	goto LOC_5FFC;
LOC_5FCC:
	MoveObject(v5, (-9), 0, 0);
LOC_5FFC:
	asynccall CosZoomY(v2, 81920);
LOC_6028:
	v1 = a2;
LOC_6038:
	v4 = IsObjectExist(a0);
	if (!(!v4)) goto LOC_6078;
	v1 = 0;
LOC_6078:
	Delay(1);
	v1--;
	if ((v1 >= 0)) goto LOC_6038;
	if (!(a4 == 2)) goto LOC_6124;
	SetObjectFadeOut(v2, 16, 1);
	if (!v5) goto LOC_611C;
	SetObjectFadeOut(v5, 16, 1);
LOC_611C:
	goto LOC_615C;
LOC_6124:
	FreeObjectByHandle(v2);
	if (!v5) goto LOC_615C;
	FreeObjectByHandle(v5);
LOC_615C:
	return;
}

void ShootObject(int a0, int a1) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	int v7;
	int v8;
	int v9;
	int v10;
LOC_6164:
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\003\\*");
	DisablePlayMagic();
	DownBrightness(8, 5);
	v1 = intvAttackerMajor;
	v2 = intvDefenderMajor;
	v3 = intvIsLeft;
	v10 = 0;
	if (!(Rand(0, 3) == 3)) goto LOC_624C;
	v10 = Rand(0, 90);
LOC_624C:
	if (!(v3 == 1)) goto LOC_6294;
	v5 = 0;
	v9 = (-1);
	goto LOC_62B4;
LOC_6294:
	v5 = 128;
	v9 = 1;
LOC_62B4:
	SetOverwhelming(v1, 1);
	v7 = GetObjectScreenX(v1);
	v8 = GetObjectScreenY(v1);
	SetViewCamera((v7 - (180 * v9)), ((v8 - 120) + v10));
	Delay(15);
	switch (a0) { 
		case 0: goto LOC_638C;
		case 1: goto LOC_63FC;
	}
	goto LOC_646C;
LOC_638C:
	SetObjectAnimate(v1, 131072);
	SetOverwhelming(v1, 0);
	Delay(15);
	PlaySound(v1, "m003snd01", 255);
	goto LOC_64D4;
LOC_63FC:
	SetObjectAnimate(v1, 262144);
	SetOverwhelming(v1, 0);
	Delay(15);
	PlaySound(v1, "m003snd02", 255);
	goto LOC_64D4;
LOC_646C:
	SetObjectAnimate(v1, 262144);
	SetOverwhelming(v1, 0);
	Delay(15);
	PlaySound(v1, "m003snd03", 255);
LOC_64D4:
	v4 = CreateObjectByReference(v1, (13001 + (a0 * 2)), v5, 0);
	SetObjectContext(v4, 0, a0);
	SetObjectContext(v4, 1, a1);
	if (!(a0 != 2)) goto LOC_65C0;
	SetCoordinateByReference_Cylind(v4, v1, v5, 80, 80);
	goto LOC_6600;
LOC_65C0:
	SetCoordinateByReference_Cylind(v4, v1, v5, 36, (80 - 11));
LOC_6600:
	SetObjectSpeed_Sphere(v4, v5, 0, 16);
	SetObjectFlags(v4, (16777216 + (33554432 * (a0 == 2))));
	SetCallbackProcedure(v4, 3001);
	if (!(a1 == 0)) goto LOC_6700;
	asynccall FlyLight(v4, (13002 + (a0 * 2)), 9999, a1, a0);
	goto LOC_673C;
LOC_6700:
	asynccall FlyLight(v4, 13007, 9999, a1, a0);
LOC_673C:
	asynccall LockCamera(v4, 180, ((-120) + v10), v3);
LOC_6778:
	v6 = IsObjectExist(v4);
	v7 = GetObjectScreenX(v4);
	if (!((v7 <= 0) || (v7 >= GetBattleWidthInScreenX()))) goto LOC_6804;
	FreeObjectByHandle(v4);
	goto LOC_682C;
LOC_6804:
	Delay(1);
	if ((v6 == 1)) goto LOC_6778;
LOC_682C:
	Delay(60);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void RushcartCallback(int a0, int a1) callsign 4001 {
	Hurt(a0, a1, intvMagicAttackValue);
	asynccall Blood(a1, 16);
	return;
}

void CreateRushCart(int a0, int a1, int a2, int a3, int a4) {
	int v1;
	Delay(Rand(0, 15));
	v1 = CreateObjectByReference(intvAttackerMajor, a2, a3, 0);
	SetObjectCoordinate(v1, a0, a1, 0);
	SetObjectFlags(v1, 33554432);
	SetCallbackProcedure(v1, 4001);
	SetObjectSpeed_Sphere(v1, a3, 0, 8);
	SetObjectScale(v1, 98304, 98304);
	if (!a4) goto LOC_6A1C;
	asynccall LockCameraLineDelay(v1, ((2 - a4) * 80), intvIsLeft, 280);
LOC_6A1C:
	Delay(400);
	SetObjectFadeOut(v1, 16, 1);
	return;
}

void RushCart(int a0) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v7;
	int v8;
	int v9;
	int v11;
LOC_6A54:
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\004\\*");
	DisablePlayMagic();
	DownBrightness(12, 5);
	v1 = intvAttackerMajor;
	v2 = intvDefenderMajor;
	v3 = intvIsLeft;
	SetOverwhelming(v1, 1);
	v7 = GetObjectScreenX(v1);
	v8 = GetObjectScreenY(v1);
	SetViewCamera(v7, (v8 - 120));
	AddAttackCounter(v1, 2);
	Delay(20);
	SetObjectAnimate(v1, 65536);
	SetOverwhelming(v1, 0);
	Delay(30);
	if (!(intvIsLeft == 1)) goto LOC_6C08;
	v11 = (-1);
	v4 = 0;
	goto LOC_6C28;
LOC_6C08:
	v11 = 1;
	v4 = 128;
LOC_6C28:
	v9 = GetSoldierMaxX(intvIsLeft);
	if (!(v3 == 1)) goto LOC_6C8C;
	SetViewCamera((v9 - 320), 0);
	goto LOC_6CB4;
LOC_6C8C:
	SetViewCamera((v9 + 320), 0);
LOC_6CB4:
	Delay(15);
	if (!(a0 == 0)) goto LOC_6E58;
	asynccall CreateRushCart((v9 + (1200 * v11)), (252 - 72), 2501, v4, 0);
	asynccall CreateRushCart((v9 + (1200 * v11)), 252, 14001, v4, 0);
	asynccall CreateRushCart((v9 + (1200 * v11)), (828 - 72), 2501, v4, 0);
	asynccall CreateRushCart((v9 + (1200 * v11)), 828, 14001, v4, (a0 + 1));
	goto LOC_7134;
LOC_6E58:
	asynccall CreateRushCart((v9 + (1200 * v11)), (108 - 72), 2501, v4, 0);
	asynccall CreateRushCart((v9 + (1200 * v11)), 108, 14001, v4, 0);
	asynccall CreateRushCart((v9 + (1200 * v11)), (252 - 72), 2501, v4, 0);
	asynccall CreateRushCart((v9 + (1200 * v11)), 252, 14001, v4, 0);
	asynccall CreateRushCart((v9 + (1200 * v11)), (828 - 72), 2501, v4, 0);
	asynccall CreateRushCart((v9 + (1200 * v11)), 828, 14001, v4, 0);
	asynccall CreateRushCart((v9 + (1200 * v11)), (972 - 72), 2501, v4, 0);
	asynccall CreateRushCart((v9 + (1200 * v11)), 972, 14001, v4, (a0 + 1));
LOC_7134:
	Wait("CreateRushCart");
	Delay(40);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void LockTargetTime3(int a0, int a1, int a2, int a3, int a4) {
	int v1;
	int v2;
	int v3;
	v3 = GetObjectDir(a0);
	v2 = CreateObjectByReference(a0, a1, v3, 0);
	SetObjectFlags(v2, a3);
	SetCallbackProcedure(v2, a4);
	v1 = a2;
LOC_7218:
	SetCoordinateByReference_Cylind(v2, a0, 0, 0, 0);
	Delay(1);
	v1--;
	if ((v1 >= 0)) goto LOC_7218;
	SetObjectFadeOut(v2, 16, 1);
	return;
}

void LockGeneral(int a0, int a1) {
	int v1;
	int v2;
	ClearObjectFlags(a0, 251658240);
	v1 = CreateObjectByReference(a1, 2501, 0, 0);
	if (!((a1 == intvAttackerMajor) || (a1 == intvDefenderMajor))) goto LOC_73D8;
	v2 = GetSoldierSide(a1);
	if (!(GetMajorHP(v2) == 0)) goto LOC_73AC;
	MoveObject(v1, 0, 0, 0);
	goto LOC_73D8;
LOC_73AC:
	MoveObject(v1, 0, 0, 48);
LOC_73D8:
	asynccall LockTargetTime2(v1, 15007, 8);
	asynccall SmallFireBall3(a0, v1, 20, 0);
	Delay(8);
	FreeObjectByHandle(v1);
	return;
}

void BigThunderCallback(int a0, int a1) callsign 5002 {
	int v1;
	int v2;
	Hurt(a0, a1, (intvMagicAttackValue / 2));
	ClearObjectFlags(a0, 251658240);
	v1 = CreateObjectByReference(a1, 2501, 0, 0);
	if (!((a1 == intvAttackerMajor) || (a1 == intvDefenderMajor))) goto LOC_75B4;
	v2 = GetSoldierSide(a1);
	if (!(GetMajorHP(v2) == 0)) goto LOC_7588;
	MoveObject(v1, 0, 0, 0);
	goto LOC_75B4;
LOC_7588:
	MoveObject(v1, 0, 0, 48);
LOC_75B4:
	asynccall LockTargetTime2(v1, 15007, 8);
	asynccall SmallFireBall3(a0, a1, 16, 0);
	asynccall SmallFireBall3(a0, a1, 12, 0);
	Delay(8);
	asynccall SmallFireBall3(a0, a1, 14, 0);
	asynccall SmallFireBall3(a0, a1, 10, 0);
	Delay(8);
	asynccall SmallFireBall3(a0, a1, 12, 0);
	asynccall SmallFireBall3(a0, a1, 8, 0);
	Delay(8);
	FreeObjectByHandle(v1);
	return;
}

void ThunderAttachAttack(int a0, int a1, int a2) {
	int v1;
	int v2;
	int v3;
	int v4;
	AddAttackCounter(a0, 2);
	v1 = GetObjectScreenX(a0);
	v2 = GetObjectScreenY(a0);
	v3 = CreateObjectRaw(((a1 + v1) / 2), ((a2 + v2) / 2), Rand(20, 60), (Rand(0, 1) * 128), 15005);
	SetObjectScale(v3, 131072, 81920);
	SetObjectFadeOut(v3, Rand(16, 32), 1);
	v3 = CreateObjectRaw(((a1 + v1) / 2), ((a2 + v2) / 2), Rand(15, 44), (Rand(0, 1) * 128), 15005);
	SetObjectScale(v3, 131072, 81920);
	SetObjectFadeOut(v3, Rand(16, 32), 1);
	v3 = CreateObjectRaw(((a1 + v1) / 2), ((a2 + v2) / 2), Rand(25, 50), (Rand(0, 1) * 128), 15005);
	SetObjectScale(v3, 131072, 81920);
	SetObjectFadeOut(v3, 32, 1);
	Delay(24);
	v4 = 0;
LOC_7A80:
	if (!(v4 < 12)) goto LOC_7BE8;
	v3 = CreateObjectByReference(a0, 15006, (Rand(0, 1) * 128), 0);
	MoveObject(v3, Rand((-12), 12), 1, Rand(16, 42));
	SetObjectScale(v3, Rand(131072, 196608), 98304);
	SetObjectFadeOut(v3, Rand(16, 32), 1);
	Delay(Rand(1, 2));
	v4++;
	goto LOC_7A80;
LOC_7BE8:
	Delay(20);
	LockByCenter(a0, 2501, 0, 0, 2, 33554432, 0);
	return;
}

void ThunderAttach(int a0) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	v1 = GetObjectXY(a0);
	v2 = GetObjectBattleY(a0);
	v5 = GetObjectScreenX(a0);
	v6 = GetObjectScreenY(a0);
	v3 = GetForceHandleByXY((v1 + 1), v2);
	v4 = GetForceHandleByXY((v1 - 1), v2);
	if (!v3) goto LOC_7DA4;
	if (!(GetObjectContext(v3, 0) != 14409)) goto LOC_7DA4;
	SetObjectContext(v3, 0, 14409);
	asynccall ThunderAttachAttack(v3, v5, v6);
LOC_7DA4:
	if (!v4) goto LOC_7E34;
	if (!(GetObjectContext(v4, 0) != 14409)) goto LOC_7E34;
	SetObjectContext(v4, 0, 14409);
	asynccall ThunderAttachAttack(v4, v5, v6);
LOC_7E34:
	return;
}

void ThunderCallback(int a0, int a1) callsign 5001 {
	ClearObjectFlags(a0, 251658240);
	if (!(GetGlobal(4) == 1)) goto LOC_7EA0;
	asynccall ThunderAttach(a1);
LOC_7EA0:
	if (!(GetGlobal(5) == 0)) goto LOC_7EEC;
	SetGlobal(5, 1);
	Hurt(a0, a1, intvMagicAttackValue);
LOC_7EEC:
	if (!((a1 == intvAttackerMajor) || (a1 == intvDefenderMajor))) goto LOC_7F54;
	asynccall LockGeneral(a0, a1);
	goto LOC_7FAC;
LOC_7F54:
	asynccall LockTargetTime2(a1, 15007, 8);
	asynccall SmallFireBall3(a0, a1, 15, 0);
LOC_7FAC:
	return;
}

void LockTargetTime(int a0, int a1, int a2) {
	int v1;
	int v2;
	int v3;
	Delay(Rand(40, 70));
	v3 = GetObjectDir(a0);
	v2 = CreateObjectByReference(a0, a1, v3, 0);
	SetObjectFlags(v2, 33554432);
	SetCallbackProcedure(v2, 5001);
	SetObjectScale(v2, 98304, 65536);
	v1 = a2;
LOC_8098:
	SetCoordinateByReference_Cylind(v2, a0, 0, 0, 0);
	Delay(1);
	v1--;
	if ((v1 >= 0)) goto LOC_8098;
	SetObjectFadeOut(v2, 16, 1);
	return;
}

void CreateThunderSparkle(int a0, int a1, int a2) {
	int v1;
	int v2;
	int v3;
	v3 = CreateObjectRaw(a0, a1, 0, 0, 2501);
	v1 = 8;
LOC_817C:
	if (!v1) goto LOC_82B4;
	v2 = CreateObjectByReference(v3, 10013, Rand(0, 255), a2);
	SetObjectScale(v2, 12288, 12288);
	SetObjectSpeed_Cylind(v2, Rand(4, 12), Rand((-2), 8));
	SetObjectFriction1(v2, 8192);
	SetObjectFadeOut(v2, 32, 1);
	SetObjectScaleShrink(v2, 256);
	v1--;
	goto LOC_817C;
LOC_82B4:
	FreeObjectByHandle(v3);
	return;
}

void CreateThunderSmoke(int a0, int a1) {
	int v1;
	Delay(2);
	v1 = CreateObjectRaw(a0, a1, 0, (128 * Rand(0, 1)), 15004);
	SetObjectLayer(v1, 2);
	SetObjectScale(v1, 8192, 8192);
	asynccall CreateThunderSparkle(a0, a1, 0);
	SetObjectScaleShrink(v1, (-2048));
	SetObjectFadeOut(v1, 30, 1);
	return;
}

void LockThunder(int a0, int a1, int a2) {
	int v1;
	int v2;
	Delay(Rand(40, 70));
	if (!(Rand(1, 3) == 1)) goto LOC_847C;
	asynccall CreateThunderSmoke(a0, a1);
LOC_847C:
	if (!GetGlobal(4)) goto LOC_84A4;
	v2 = 15010;
	goto LOC_84C8;
LOC_84A4:
	v2 = Rand(15001, 15003);
LOC_84C8:
	v1 = CreateObjectByReference(intvAttackerMajor, v2, a2, 0);
	SetObjectCoordinate(v1, a0, a1, 0);
	SetObjectFlags(v1, 33554432);
	SetCallbackProcedure(v1, 5001);
	Delay(4);
	SetObjectFadeOut(v1, 16, 1);
	return;
}

void Thunder(int a0, int a1) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	int v7;
	int v8;
	int v9;
	int v10;
	int v11;
	int v12;
	int v13;
	int v14;
	int v15;
	int v16;
	int v17;
	int v18;
	int v19;
	int v20;
	int v21;
LOC_8598:
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\005\\*");
	SetGlobal(4, a1);
	SetGlobal(5, 0);
	DisablePlayMagic();
	DownBrightness(8, 5);
	v1 = intvAttackerMajor;
	v2 = intvDefenderMajor;
	v3 = intvIsLeft;
	SetOverwhelming(v1, 1);
	v9 = GetObjectScreenX(v1);
	v10 = GetObjectScreenY(v1);
	SetViewCamera(v9, (v10 - 120));
	AddAttackCounter(v1, 2);
	Delay(20);
	SetObjectAnimate(v1, 32768);
	SetOverwhelming(v1, 0);
	Delay(30);
	if (!(intvIsLeft == 1)) goto LOC_8758;
	v4 = 0;
	goto LOC_8768;
LOC_8758:
	v4 = 128;
LOC_8768:
	v17 = GetSoldierCount((intvIsLeft ^ 1));
	v18 = 0;
	if (!(v17 < a0)) goto LOC_87D8;
	v18 = (a0 - v17);
LOC_87D8:
	if (!((a0 - v18) > 0)) goto LOC_8C48;
	v8 = 0;
	v7 = (a0 - v18);
LOC_882C:
	if (!(v8 == 0)) goto LOC_8924;
	v6 = GetMostImportantSoldier((intvIsLeft ^ 1));
	if (!(v6 == 0)) goto LOC_889C;
	v6 = intvDefenderMajor;
LOC_889C:
	v12 = GetObjectXY(v6);
	v13 = GetObjectBattleY(v6);
	v5 = v6;
	v15 = GetObjectScreenX(v5);
	v16 = GetObjectScreenY(v5);
	goto LOC_8A84;
LOC_8924:
	if (!(v8 > 8)) goto LOC_8970;
	v5 = GetRandomSoldierFromAlive((intvIsLeft ^ 1));
	goto LOC_8A84;
LOC_8970:
	v14 = GetSoldierCountInRect((intvIsLeft ^ 1), v12, v13, 3, 4);
	v5 = GetNthSoldierInRect((intvIsLeft ^ 1), v12, v13, 3, 4, Rand(0, (v14 - 1)));
	if (!((v5 == 0) || (v5 == intvDefenderMajor))) goto LOC_8A84;
	v5 = GetRandomSoldierFromAlive((intvIsLeft ^ 1));
LOC_8A84:
	v9 = GetObjectScreenX(v5);
	v10 = GetObjectScreenY(v5);
	if (!(v8 == 0)) goto LOC_8B20;
	v8 = 1;
	v19 = v9;
	v20 = v9;
	v21 = v10;
	goto LOC_8BA4;
LOC_8B20:
	if (!(v19 < v9)) goto LOC_8B4C;
	v19 = v9;
LOC_8B4C:
	if (!(v20 > v9)) goto LOC_8B78;
	v20 = v9;
LOC_8B78:
	if (!(v21 > v10)) goto LOC_8BA4;
	v21 = v10;
LOC_8BA4:
	if (!GetGlobal(4)) goto LOC_8BCC;
	v11 = 15010;
	goto LOC_8BF0;
LOC_8BCC:
	v11 = Rand(15001, 15003);
LOC_8BF0:
	asynccall LockTargetTime(v5, v11, 4);
	v7--;
	if ((v7 > 0)) goto LOC_882C;
	goto LOC_8C90;
LOC_8C48:
	v19 = GetObjectScreenX(v2);
	v20 = v19;
	v21 = GetObjectScreenY(v1);
LOC_8C90:
	if (!(v19 == v20)) goto LOC_8CE4;
	v19 = (v19 + 320);
	v20 = (v20 - 320);
LOC_8CE4:
	v7 = v18;
	if (!(v7 > 20)) goto LOC_8D20;
	v7 = 20;
LOC_8D20:
	v9 = Rand(v20, v19);
	v10 = Rand(150, 650);
	asynccall LockThunder(v9, v10, (128 * Rand(0, 1)));
	v7--;
	if ((v7 > 0)) goto LOC_8D20;
	v21 = (v21 - 120);
	if (!(v17 == 0)) goto LOC_8E60;
	v9 = ((v19 + v20) / 2);
	MoveCamera(v9, v21, 40);
	goto LOC_8E8C;
LOC_8E60:
	MoveCamera(v15, (v16 - 140), 40);
LOC_8E8C:
	Delay(60);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void BigThunder() {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
LOC_8EC4:
	DisablePlayMagic();
	DownBrightness(8, 5);
	v1 = intvAttackerMajor;
	v2 = intvDefenderMajor;
	v3 = intvIsLeft;
	SetOverwhelming(v1, 1);
	v5 = GetObjectScreenX(v1);
	v6 = GetObjectScreenY(v1);
	SetViewCamera(v5, (v6 - 120));
	AddAttackCounter(v1, 2);
	Delay(20);
	SetObjectAnimate(v1, 32768);
	SetOverwhelming(v1, 0);
	Delay(20);
	if (!(v3 == 1)) goto LOC_903C;
	v4 = 0;
	goto LOC_904C;
LOC_903C:
	v4 = 128;
LOC_904C:
	v5 = GetObjectScreenX(v2);
	v6 = GetObjectScreenY(v2);
	PlaySound1("m005snd01", 255);
	MoveCamera(v5, (v6 - 120), 40);
	asynccall LockTargetTime3(v2, 15008, 4, 16777216, 5002);
	Delay(4);
	asynccall LockTargetTime3(v2, 15009, 4, 16777216, 5002);
	Delay(60);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void HideArrow(int a0, int a1) {
	Delay(Rand(30, 40));
	if (!a1) goto LOC_91DC;
	SetObjectFadeOut(a1, 32, 1);
LOC_91DC:
	SetObjectFadeOut(a0, 32, 1);
	return;
}

void TraceArrow(int a0, int a1) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	int v7;
	v7 = 0;
LOC_9220:
	if (!IsObjectExist(a0)) goto LOC_94AC;
	v3 = GetObjectScreenZ(a0);
	if (!(v3 == 0)) goto LOC_9498;
	v1 = GetObjectScreenX(a0);
	v2 = GetObjectScreenY(a0);
	v5 = GetObjectDir(a0);
	if (!(GetGlobal(7) > 0)) goto LOC_9318;
	v6 = Rand(16012, 16018);
	SetGlobal(7, GetGlobal(7) + 1);
	goto LOC_933C;
LOC_9318:
	v6 = Rand(16012, 16013);
LOC_933C:
	v4 = CreateObjectByReference(a0, v6, v5, 0);
	if (!((a1 == 1) && ((Rand(1, 256) & 1) == 0))) goto LOC_9450;
	if (!((v6 == 7027) || (v6 == 7028))) goto LOC_9450;
	v7 = CreateObjectByReference(a0, 7045, v5, 0);
	SetObjectScale(v7, 32768, 49152);
LOC_9450:
	asynccall HideArrow(v4, v7);
	FreeObjectByHandle(a0);
	goto LOC_94AC;
LOC_9498:
	Delay(1);
	goto LOC_9220;
LOC_94AC:
	return;
}

void ArrowCallback1(int a0, int a1) callsign 6001 {
	int v1;
	if (!(a1 == intvDefenderMajor)) goto LOC_9564;
	if (!GetGlobal(8)) goto LOC_9504;
	v1 = (intvMagicAttackValue / GetGlobal(8));
LOC_9504:
	Hurt(a0, a1, v1);
	if (!(v1 > 0)) goto LOC_955C;
	SetGlobal(8, (GetGlobal(8) * 2));
LOC_955C:
	goto LOC_9584;
LOC_9564:
	Hurt(a0, a1, 0);
LOC_9584:
	return;
}

void ArrowCallback2(int a0, int a1) callsign 6002 {
	int v3;
	if (!(a1 == intvDefenderMajor)) goto LOC_9668;
	if (!GetGlobal(8)) goto LOC_95DC;
	v3 = (intvMagicAttackValue / GetGlobal(8));
LOC_95DC:
	Hurt(a0, a1, v3);
	if (!(v3 > 0)) goto LOC_9660;
	SetGlobal(8, (GetGlobal(8) * 2));
	asynccall LockTargetTime2(a1, 10015, 60);
LOC_9660:
	goto LOC_96B4;
LOC_9668:
	Hurt(a0, a1, 0);
	asynccall FireMan(a1, 10015, 60);
LOC_96B4:
	return;
}

void FlyArrowLU() callsign 6003 {
	int v1;
	int v2;
	v2 = GetScriptLinkedObject();
	v1 = CreateObjectByReference(v2, 16003, 128, 0);
	SetCoordinateByReference_Cylind(v1, v2, 128, 64, 104);
	SetObjectSpeed_Sphere(v1, 128, 32, 32);
	PlaySound(v1, "arrow", 255);
	if (!(GetGlobal(6) == 1)) goto LOC_9840;
	v1 = CreateObjectByReference(v2, 16009, 128, 0);
	SetCoordinateByReference_Cylind(v1, v2, 128, 64, 104);
	SetObjectSpeed_Sphere(v1, 128, 32, 32);
LOC_9840:
	return;
}

void FlyArrowRU() callsign 6004 {
	int v1;
	int v2;
	v2 = GetScriptLinkedObject();
	v1 = CreateObjectByReference(intvAttackerMajor, 16003, 0, 0);
	SetCoordinateByReference_Cylind(v1, v2, 0, 64, 104);
	SetObjectSpeed_Sphere(v1, 128, 96, 32);
	PlaySound(v1, "arrow", 255);
	if (!(GetGlobal(6) == 1)) goto LOC_99CC;
	v1 = CreateObjectByReference(v2, 16009, 0, 0);
	SetCoordinateByReference_Cylind(v1, v2, 0, 64, 104);
	SetObjectSpeed_Sphere(v1, 128, 96, 32);
LOC_99CC:
	return;
}

void FlyArrowLB2(int a0, int a1, int a2, int a3, int a4) {
	int v1;
	Delay(a2);
	v1 = CreateObjectByReference(intvAttackerMajor, 16004, 128, 0);
	SetObjectCoordinate(v1, (a0 + 260), a1, 260);
	SetObjectSpeed_Sphere(v1, 128, (-32), 32);
	if (!a4) goto LOC_9AB8;
	SetCallbackProcedure(v1, 6001);
	goto LOC_9B00;
LOC_9AB8:
	ClearObjectFlags(v1, 251658240);
	asynccall TraceArrow(v1, a3);
LOC_9B00:
	if (!(a3 == 1)) goto LOC_9BE4;
	if (!a4) goto LOC_9B48;
	SetCallbackProcedure(v1, 6002);
LOC_9B48:
	v1 = CreateObjectByReference(intvAttackerMajor, 16010, 128, 0);
	SetObjectCoordinate(v1, (a0 + 260), a1, 260);
	SetObjectSpeed_Sphere(v1, 128, (-32), 32);
LOC_9BE4:
	return;
}

void FlyArrowRB2(int a0, int a1, int a2, int a3, int a4) {
	int v1;
	Delay(a2);
	v1 = CreateObjectByReference(intvAttackerMajor, 16007, 0, 0);
	SetObjectCoordinate(v1, (a0 - 260), a1, 260);
	SetObjectSpeed_Sphere(v1, 0, (-32), 32);
	if (!a4) goto LOC_9CD0;
	SetCallbackProcedure(v1, 6001);
	goto LOC_9D18;
LOC_9CD0:
	ClearObjectFlags(v1, 251658240);
	asynccall TraceArrow(v1, a3);
LOC_9D18:
	if (!(a3 == 1)) goto LOC_9DFC;
	if (!a4) goto LOC_9D60;
	SetCallbackProcedure(v1, 6002);
LOC_9D60:
	v1 = CreateObjectByReference(intvAttackerMajor, 16010, 0, 0);
	SetObjectCoordinate(v1, (a0 - 260), a1, 260);
	SetObjectSpeed_Sphere(v1, 0, (-32), 32);
LOC_9DFC:
	return;
}

void FlyArrowLB1(int a0, int a1, int a2, int a3) {
	int v1;
	Delay(a2);
	v1 = CreateObjectByReference(intvAttackerMajor, 16004, 128, 0);
	SetCoordinateByReference_Cylind(v1, a0, 0, 0, 0);
	MoveObject(v1, (260 + 24), 0, 260);
	SetObjectSpeed_Sphere(v1, 128, (-32), 32);
	SetCallbackProcedure(v1, 6001);
	if (!(a3 == 1)) goto LOC_A00C;
	SetCallbackProcedure(v1, 6002);
	v1 = CreateObjectByReference(intvAttackerMajor, 16010, 128, 0);
	SetCoordinateByReference_Cylind(v1, a0, 0, 0, 0);
	MoveObject(v1, (260 + 24), 0, 260);
	SetObjectSpeed_Sphere(v1, 128, (-32), 32);
LOC_A00C:
	return;
}

void FlyArrowRB1(int a0, int a1, int a2, int a3) {
	int v1;
	Delay(a2);
	v1 = CreateObjectByReference(intvAttackerMajor, 16007, 0, 0);
	SetCoordinateByReference_Cylind(v1, a0, 0, 0, 0);
	MoveObject(v1, ((-260) - 24), 0, 260);
	SetObjectSpeed_Sphere(v1, 0, (-32), 32);
	SetCallbackProcedure(v1, 6001);
	if (!(a3 == 1)) goto LOC_A224;
	SetCallbackProcedure(v1, 6002);
	v1 = CreateObjectByReference(intvAttackerMajor, 16010, 0, 0);
	SetCoordinateByReference_Cylind(v1, a0, 0, 0, 0);
	MoveObject(v1, ((-260) - 24), 0, 260);
	SetObjectSpeed_Sphere(v1, 0, (-32), 32);
LOC_A224:
	return;
}

void ChangeRightFirePos(int a0, int a1) {
	Delay((38 + a1));
	MoveObject(a0, (-14), 0, 31);
	Delay(8);
	MoveObject(a0, 10, 0, 30);
	Delay(8);
	FreeObjectByHandle(a0);
	return;
}

void ChangeLeftFirePos(int a0, int a1) {
	Delay((38 + a1));
	MoveObject(a0, 14, 0, 31);
	Delay(8);
	MoveObject(a0, (-10), 0, 30);
	Delay(8);
	FreeObjectByHandle(a0);
	return;
}

void AddSoldierRight(int a0, int a1, int a2) {
	int v1;
	Delay(a2);
	v1 = CreateObjectByReference(intvAttackerMajor, 16001, 128, 0);
	SetObjectCoordinate(v1, a0, a1, 0);
	return;
}

void AddSoldierLeft(int a0, int a1, int a2) {
	int v1;
	Delay(a2);
	v1 = CreateObjectByReference(intvAttackerMajor, 16002, 0, 0);
	SetObjectCoordinate(v1, a0, a1, 0);
	return;
}

void ProduceArrowSoldier(int a0, int a1, int a2) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
LOC_A474:
	v2 = a1;
	v4 = 0;
	v6 = ((GetBattleWidth() * 96) - 240);
LOC_A4C8:
	v1 = 9;
LOC_A4D8:
	if (!(a0 == 1)) goto LOC_A738;
	v5 = CreateObjectByReference(intvAttackerMajor, 16006, 0, 0);
	SetObjectCoordinate(v5, ((240 - (96 * 7)) - (v4 * 96)), (180 + (v1 * 72)), 0);
	v3 = Rand(1, 30);
	asynccall AddSoldierLeft(((240 - (96 * 7)) - (v4 * 96)), (180 + (v1 * 72)), v3);
	if (!(a2 == 1)) goto LOC_A730;
	v5 = CreateObjectByReference(intvAttackerMajor, 16008, 0, 0);
	SetObjectCoordinate(v5, (((240 - (96 * 7)) + 27) - (v4 * 96)), (180 + (v1 * 72)), 14);
	asynccall ChangeLeftFirePos(v5, v3);
LOC_A730:
	goto LOC_A974;
LOC_A738:
	v5 = CreateObjectByReference(intvAttackerMajor, 16005, 128, 0);
	SetObjectCoordinate(v5, ((v6 + (96 * 7)) + (v4 * 96)), (180 + (v1 * 72)), 0);
	v3 = Rand(1, 30);
	asynccall AddSoldierRight(((v6 + (96 * 7)) + (v4 * 96)), (180 + (v1 * 72)), v3);
	if (!(a2 == 1)) goto LOC_A974;
	v5 = CreateObjectByReference(intvAttackerMajor, 16008, 128, 0);
	SetObjectCoordinate(v5, (((v6 + (96 * 7)) - 27) + (v4 * 96)), (180 + (v1 * 72)), 14);
	asynccall ChangeRightFirePos(v5, v3);
LOC_A974:
	v2--;
	if (!(v2 <= 0)) goto LOC_A9A8;
	v1 = 0;
LOC_A9A8:
	v1--;
	if ((v1 >= 0)) goto LOC_A4D8;
	v4++;
	if ((v2 > 0)) goto LOC_A4C8;
	return;
}

void ArrowSupport(int a0, int a1) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	int v7;
	int v8;
	int v9;
	int v10;
	int v11;
	int v12;
LOC_A9F8:
	SetGlobal(8, 1);
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\006\\*");
	DisablePlayMagic();
	DownBrightness(12, 5);
	SetGlobal(6, a1);
	v6 = ((GetBattleWidth() * 96) - 240);
	SetOverwhelming(intvAttackerMajor, 1);
	v4 = GetObjectScreenX(intvAttackerMajor);
	v5 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v4, (v5 - 120));
	AddAttackCounter(intvAttackerMajor, 2);
	Delay(20);
	SetObjectAnimate(intvAttackerMajor, 65536);
	SetOverwhelming(intvAttackerMajor, 0);
	Delay(30);
	ProduceArrowSoldier(intvIsLeft, a0, a1);
	if (!(intvIsLeft == 1)) goto LOC_AC08;
	asynccall MoveCamera((240 - (96 * 4)), 0, 40);
	goto LOC_AC58;
LOC_AC08:
	asynccall MoveCamera(((v6 + (96 * 4)) - 1), 0, 40);
LOC_AC58:
	Wait("MoveCamera");
	Delay(60);
	v9 = GetSoldierCount((intvIsLeft ^ 1));
	v10 = 0;
	if (!(v9 < a0)) goto LOC_ACE0;
	v10 = (a0 - v9);
LOC_ACE0:
	if (!((a0 - v10) > 0)) goto LOC_AF30;
	v2 = 0;
	v1 = (a0 - v10);
LOC_AD34:
	if (!(v1 > 0)) goto LOC_AF28;
	v3 = GetRandomSoldierFromAlive((intvIsLeft ^ 1));
	v4 = GetObjectScreenX(v3);
	v5 = GetObjectScreenY(v3);
	if (!(v2 == 0)) goto LOC_AE04;
	v2 = 1;
	v11 = v4;
	v12 = v4;
	goto LOC_AE5C;
LOC_AE04:
	if (!(v11 < v4)) goto LOC_AE30;
	v11 = v4;
LOC_AE30:
	if (!(v12 > v4)) goto LOC_AE5C;
	v12 = v4;
LOC_AE5C:
	if (!(intvIsLeft == 1)) goto LOC_AECC;
	asynccall FlyArrowRB1(v3, v5, (80 + Rand(1, 30)), a1);
	goto LOC_AF18;
LOC_AECC:
	asynccall FlyArrowLB1(v3, v5, (80 + Rand(1, 30)), a1);
LOC_AF18:
	v1--;
	goto LOC_AD34;
LOC_AF28:
	goto LOC_AF5C;
LOC_AF30:
	v11 = GetObjectScreenX(intvDefenderMajor);
	v12 = v11;
LOC_AF5C:
	if (!((v11 - v12) >= 650)) goto LOC_AFB8;
	if (!(intvIsLeft == 1)) goto LOC_AFB0;
	v11 = v12;
LOC_AFB0:
	goto LOC_AFE0;
LOC_AFB8:
	v11 = ((v11 + v12) / 2);
LOC_AFE0:
	v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
	if (!(v3 == 0)) goto LOC_B034;
	v3 = intvDefenderMajor;
LOC_B034:
	v11 = GetObjectScreenX(v3);
	v1 = v10;
LOC_B060:
	v4 = (v11 + Rand((-400), 400));
	v5 = Rand(150, 650);
	if (!(intvIsLeft == 1)) goto LOC_B138;
	asynccall FlyArrowRB2(v4, v5, (80 + Rand(1, 30)), a1, 1);
	goto LOC_B194;
LOC_B138:
	asynccall FlyArrowLB2(v4, v5, (80 + Rand(1, 30)), a1, 1);
LOC_B194:
	v1--;
	if ((v1 > 0)) goto LOC_B060;
	v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
	if (!(v3 == 0)) goto LOC_B20C;
	v3 = intvDefenderMajor;
LOC_B20C:
	v11 = GetObjectScreenX(v3);
	v1 = (((a0 / 10) * 5) + ((Rand(32, 50) - a0) / 8));
	SetGlobal(7, (v1 / 4));
LOC_B2A4:
	v4 = (v11 + Rand((-400), 400));
	v5 = Rand(200, 600);
	v7 = ScreenXToBattleX(v4);
	v8 = ScreenYToBattleY(v5);
	if (!GetForceHandleByXY(v7, v8)) goto LOC_B418;
	v8 = BattleXToScreenX(v8);
	if (!Rand(0, 1)) goto LOC_B3DC;
	v5 = (v8 - (8 * Rand(1, 2)));
	goto LOC_B418;
LOC_B3DC:
	v5 = (v8 + (8 * Rand(1, 2)));
LOC_B418:
	if (!(intvIsLeft == 1)) goto LOC_B498;
	asynccall FlyArrowRB2(v4, v5, (80 + Rand(1, 30)), a1, 0);
	goto LOC_B4F4;
LOC_B498:
	asynccall FlyArrowLB2(v4, v5, (80 + Rand(1, 30)), a1, 0);
LOC_B4F4:
	v1--;
	if ((v1 > 0)) goto LOC_B2A4;
	v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
	if (!(v3 == 0)) goto LOC_B56C;
	v3 = intvDefenderMajor;
LOC_B56C:
	v4 = GetObjectScreenX(v3);
	v5 = GetObjectScreenY(v3);
	MoveCamera(v4, 0, 80);
	Wait("HideArrow");
	Wait("TraceArrow");
	Delay(110);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void StoneEmitterCallback1(int a0, int a1) callsign 7001 {
	int v1;
	if (!(a1 == intvDefenderMajor)) goto LOC_B6C4;
	if (!GetGlobal(10)) goto LOC_B664;
	v1 = (intvMagicAttackValue / GetGlobal(10));
LOC_B664:
	Hurt(a0, a1, v1);
	if (!(v1 > 0)) goto LOC_B6BC;
	SetGlobal(10, (GetGlobal(10) * 2));
LOC_B6BC:
	goto LOC_B6E4;
LOC_B6C4:
	Hurt(a0, a1, 0);
LOC_B6E4:
	return;
}

void StoneEmitterCallback2(int a0, int a1) callsign 7002 {
	int v1;
	if (!(a1 == intvDefenderMajor)) goto LOC_B7C8;
	if (!GetGlobal(10)) goto LOC_B73C;
	v1 = (intvMagicAttackValue / GetGlobal(10));
LOC_B73C:
	Hurt(a0, a1, v1);
	if (!(v1 > 0)) goto LOC_B7C0;
	SetGlobal(10, (GetGlobal(10) * 2));
	asynccall LockTargetTime2(a1, 10015, 60);
LOC_B7C0:
	goto LOC_B814;
LOC_B7C8:
	Hurt(a0, a1, 0);
	asynccall FireMan(a1, 10015, 60);
LOC_B814:
	return;
}

void StoneEmitterCmd3() callsign 7005 {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	v3 = GetScriptLinkedObject();
	v4 = GetObjectDir(v3);
	v1 = CreateObjectByReference(v3, 17003, v4, 0);
	SetCoordinateByReference_Cylind(v1, v3, 0, 0, 117);
	SetObjectSpeed_Cylind(v1, 30, 30);
	v5 = GetObjectContext(v3, 0);
	if (!v5) goto LOC_B980;
	FreeObjectByHandle(v5);
	v5 = CreateObjectByReference(v1, 17022, v4, 0);
	SetObjectSpeed_Cylind(v5, 30, 30);
LOC_B980:
	Delay(2);
	v2 = CreateObjectByReference(v3, 17004, v4, 0);
	SetCoordinateByReference_Cylind(v2, v3, 0, 0, 0);
	SetObjectSpeed_Cylind(v2, 30, 0);
	SetObjectFadeOut(v2, 32, 1);
	SetObjectScaleShrink(v2, 1024);
	Delay(32);
	FreeObjectByHandle(v1);
	if (!v5) goto LOC_BA9C;
	FreeObjectByHandle(v5);
LOC_BA9C:
	return;
}

void StoneExplode(int a0) {
	int v1;
	int v2;
	PlaySound1("m007snd03", 200);
	v2 = GetObjectDir(a0);
	v1 = CreateObjectByReference(a0, 2501, v2, 0);
	SetObjectFlags(v1, 33554432);
	if (!(GetGlobal(9) == 0)) goto LOC_BB74;
	SetCallbackProcedure(v1, 7001);
	goto LOC_BB90;
LOC_BB74:
	SetCallbackProcedure(v1, 7002);
LOC_BB90:
	SetObjectLife(v1, 2);
	return;
}

void Func7005(int a0) {
	int v1;
	int v2;
	v1 = 65536;
	SetObjectSpeed_Cylind(a0, Rand(4, 6), Rand(8, 12));
	SetObjectGravity(a0, 24576);
	SetObjectFriction1(a0, 8192);
	v2 = Rand(20, 30);
LOC_BE1C:
	v2--;
	if (!(v2 > 0)) goto LOC_BECC;
	SetObjectScale(a0, v1, v1);
	v1 = (v1 - 256);
	if (!(v2 == 15)) goto LOC_BEB8;
	ClearObjectFlags(a0, 251658240);
LOC_BEB8:
	Delay(1);
	goto LOC_BE1C;
LOC_BECC:
	FreeObjectByHandle(a0);
	return;
}

void FallExplode1() callsign 7003 {
	int v1;
	int v2;
	int v3;
	v3 = GetScriptLinkedObject();
	SetScriptLinkedObject(intvAttackerMajor);
	asynccall StoneExplode(v3);
	v2 = 0;
LOC_BC20:
	if (!(v2 < 6)) goto LOC_BD54;
	v1 = CreateObjectByReference(v3, Rand(17007, 17011), Rand(0, 255), 0);
	if (!(GetGlobal(9) == 0)) goto LOC_BCD8;
	SetCallbackProcedure(v1, 7001);
	goto LOC_BCF4;
LOC_BCD8:
	SetCallbackProcedure(v1, 7002);
LOC_BCF4:
	SetObjectScale(v1, 65536, 65536);
	asynccall Func7005(v1);
	v2++;
	goto LOC_BC20;
LOC_BD54:
	return;
}

void FireStoneExplode(int a0) {
	int v1;
	int v2;
	if (!(GetGlobal(11) == 0)) goto LOC_BF38;
	PlaySound1("m007snd02", 255);
	SetGlobal(11, 1);
LOC_BF38:
	v2 = GetObjectDir(a0);
	v1 = CreateObjectByReference(a0, 17012, v2, 0);
	SetObjectScale(v1, 57344, 57344);
	SetObjectFlags(v1, 33554432);
	ClearObjectFlags(v1, (16777216 + 67108864));
	if (!(GetGlobal(9) == 0)) goto LOC_C030;
	SetCallbackProcedure(v1, 7001);
	goto LOC_C04C;
LOC_C030:
	SetCallbackProcedure(v1, 7002);
LOC_C04C:
	return;
}

void Func7006(int a0) {
	int v1;
	int v2;
	v1 = 65536;
	SetObjectSpeed_Cylind(a0, Rand(4, 12), Rand((-2), 8));
	SetObjectFriction1(a0, 8192);
	v2 = Rand(30, 40);
LOC_C310:
	v2--;
	if (!(v2 > 0)) goto LOC_C388;
	SetObjectScale(a0, v1, v1);
	v1 = (v1 - 256);
	Delay(1);
	goto LOC_C310;
LOC_C388:
	FreeObjectByHandle(a0);
	return;
}

void FallExplode2() callsign 7004 {
	int v1;
	int v2;
	int v3;
	v3 = GetScriptLinkedObject();
	SetScriptLinkedObject(intvAttackerMajor);
	asynccall FireStoneExplode(v3);
	v2 = 0;
LOC_C0C0:
	if (!(v2 < 10)) goto LOC_C260;
	v1 = CreateObjectByReference(v3, Rand(17016, 17020), Rand(0, 255), 0);
	if (!((v2 % 3) == 0)) goto LOC_C200;
	SetObjectFlags(v1, 33554432);
	ClearObjectFlags(v1, (16777216 + 67108864));
	if (!(GetGlobal(9) == 0)) goto LOC_C1E4;
	SetCallbackProcedure(v1, 7001);
	goto LOC_C200;
LOC_C1E4:
	SetCallbackProcedure(v1, 7002);
LOC_C200:
	SetObjectScale(v1, 65536, 65536);
	asynccall Func7006(v1);
	v2++;
	goto LOC_C0C0;
LOC_C260:
	return;
}

void ProduceStoneEmitter(int a0, int a1, int a2, int a3, int a4) {
	int v1;
	int v2;
	Delay(a2);
	if (!(intvIsLeft == 1)) goto LOC_C410;
	v1 = CreateObjectByReference(intvAttackerMajor, 17023, a3, 0);
	goto LOC_C444;
LOC_C410:
	v1 = CreateObjectByReference(intvAttackerMajor, 17002, a3, 0);
LOC_C444:
	SetObjectCoordinate(v1, a0, a1, 0);
	SetObjectContext(v1, 0, 0);
	if (!(a4 == 1)) goto LOC_C5AC;
	v2 = CreateObjectByReference(v1, 17021, a3, 0);
	SetObjectScale(v2, 81920, 81920);
	SetObjectContext(v1, 0, v2);
	if (!(intvIsLeft == 1)) goto LOC_C580;
	MoveObject(v2, (-63), 0, 42);
	goto LOC_C5AC;
LOC_C580:
	MoveObject(v2, 63, 0, 42);
LOC_C5AC:
	return;
}

void TraceStoneShadow(int a0, int a1, int a2, int a3) {
	int v1;
LOC_C5BC:
	if (!IsObjectExist(a1)) goto LOC_C6C8;
	v1 = GetObjectScreenZ(a1);
	if (!(v1 <= 0)) goto LOC_C618;
	goto LOC_C6C8;
LOC_C618:
	Delay(1);
	if (!(a2 < 65536)) goto LOC_C680;
	a2 = (a2 + 8192);
	SetObjectScale(a0, a2, a2);
LOC_C680:
	if (!(a3 < 16)) goto LOC_C6C0;
	a3++;
	SetObjectOpacity(a0, a3);
LOC_C6C0:
	goto LOC_C5BC;
LOC_C6C8:
	FreeObjectByHandle(a0);
	return;
}

void FlyStoneLB1(int a0, int a1, int a2, int a3, int a4) {
	int v1;
	int v2;
	Delay(a2);
	if (!(a3 == 0)) goto LOC_C750;
	v1 = CreateObjectByReference(intvAttackerMajor, 17005, 128, 0);
	goto LOC_C784;
LOC_C750:
	v1 = CreateObjectByReference(intvAttackerMajor, 17014, 128, 0);
LOC_C784:
	SetObjectCoordinate(v1, (a0 + 260), a1, 260);
	SetObjectSpeed_Sphere(v1, 128, (-32), 32);
	SetObjectFlags(v1, 33554432);
	SetCallbackProcedure(v1, a4);
	v2 = CreateObjectByReference(intvAttackerMajor, 17004, 128, 0);
	SetObjectCoordinate(v2, (a0 + 264), a1, 0);
	SetObjectSpeed_Cylind(v2, 23, 0);
	SetObjectScale(v2, 32768, 32768);
	SetObjectFlags(v2, 262144);
	SetObjectOpacity(v2, 4);
	asynccall TraceStoneShadow(v2, v1, 32768, 4);
	if (!(a3 == 1)) goto LOC_CA18;
	v1 = CreateObjectByReference(intvAttackerMajor, 17013, 128, 0);
	SetObjectCoordinate(v1, (a0 + 260), a1, 260);
	SetObjectSpeed_Sphere(v1, 128, (-32), 32);
	SetObjectScale(v1, 98304, 98304);
LOC_CA18:
	return;
}

void FlyStoneRB1(int a0, int a1, int a2, int a3, int a4) {
	int v1;
	int v2;
	Delay(a2);
	if (!(a3 == 0)) goto LOC_CA8C;
	v1 = CreateObjectByReference(intvAttackerMajor, 17006, 128, 0);
	goto LOC_CAC0;
LOC_CA8C:
	v1 = CreateObjectByReference(intvAttackerMajor, 17015, 128, 0);
LOC_CAC0:
	SetObjectCoordinate(v1, (a0 - 260), a1, 260);
	SetObjectSpeed_Sphere(v1, 0, (-32), 32);
	SetObjectFlags(v1, 33554432);
	SetCallbackProcedure(v1, a4);
	v2 = CreateObjectByReference(intvAttackerMajor, 17004, 0, 0);
	SetObjectCoordinate(v2, (a0 - 264), a1, 0);
	SetObjectSpeed_Cylind(v2, 23, 0);
	SetObjectScale(v2, 32768, 32768);
	SetObjectFlags(v2, 262144);
	SetObjectOpacity(v2, 4);
	asynccall TraceStoneShadow(v2, v1, 32768, 4);
	if (!(a3 == 1)) goto LOC_CD54;
	v1 = CreateObjectByReference(intvAttackerMajor, 17013, 0, 0);
	SetObjectCoordinate(v1, (a0 - 260), a1, 260);
	SetObjectSpeed_Sphere(v1, 0, (-32), 32);
	SetObjectScale(v1, 98304, 98304);
LOC_CD54:
	return;
}

void FlyStoneLB2(int a0, int a1, int a2, int a3, int a4) {
	int v1;
	int v2;
	int v3;
	v2 = GetObjectScreenX(a0);
	a1 = GetObjectScreenY(a0);
	Delay(a2);
	if (!(a3 == 0)) goto LOC_CE00;
	v1 = CreateObjectByReference(intvAttackerMajor, 17005, 128, 0);
	goto LOC_CE34;
LOC_CE00:
	v1 = CreateObjectByReference(intvAttackerMajor, 17014, 128, 0);
LOC_CE34:
	v3 = CreateObjectByReference(intvAttackerMajor, 17004, 128, 0);
	if (!(IsObjectExist(a0) == 0)) goto LOC_CF20;
	SetObjectCoordinate(v1, ((v2 + 260) + 24), a1, 260);
	SetObjectCoordinate(v3, ((v2 + 264) + 24), a1, 0);
	goto LOC_CFF8;
LOC_CF20:
	SetCoordinateByReference_Cylind(v1, a0, 0, 0, 0);
	MoveObject(v1, (260 + 24), 0, 260);
	SetCoordinateByReference_Cylind(v3, a0, 0, 0, 0);
	SetObjectCoordinate(v3, (264 + 24), 0, 0);
LOC_CFF8:
	SetObjectSpeed_Sphere(v1, 128, (-32), 32);
	SetObjectFlags(v1, 33554432);
	SetCallbackProcedure(v1, a4);
	SetObjectSpeed_Cylind(v3, 23, 0);
	SetObjectScale(v3, 32768, 32768);
	SetObjectFlags(v3, 262144);
	SetObjectOpacity(v3, 4);
	asynccall TraceStoneShadow(v3, v1, 32768, 4);
	if (!(a3 == 1)) goto LOC_D21C;
	v1 = CreateObjectByReference(intvAttackerMajor, 17013, 128, 0);
	SetCoordinateByReference_Cylind(v1, a0, 0, 0, 0);
	MoveObject(v1, (260 + 24), 0, 260);
	SetObjectSpeed_Sphere(v1, 128, (-32), 32);
	SetObjectScale(v1, 98304, 98304);
LOC_D21C:
	return;
}

void FlyStoneRB2(int a0, int a1, int a2, int a3, int a4) {
	int v1;
	int v2;
	int v3;
	v2 = GetObjectScreenX(a0);
	a1 = GetObjectScreenY(a0);
	Delay(a2);
	if (!(a3 == 0)) goto LOC_D2C8;
	v1 = CreateObjectByReference(intvAttackerMajor, 17006, 128, 0);
	goto LOC_D2FC;
LOC_D2C8:
	v1 = CreateObjectByReference(intvAttackerMajor, 17015, 128, 0);
LOC_D2FC:
	v3 = CreateObjectByReference(intvAttackerMajor, 17004, 0, 0);
	if (!(IsObjectExist(a0) == 0)) goto LOC_D3E8;
	SetObjectCoordinate(v1, ((v2 - 260) - 24), a1, 260);
	SetObjectCoordinate(v3, ((v2 - 264) - 24), a1, 0);
	goto LOC_D4C8;
LOC_D3E8:
	SetCoordinateByReference_Cylind(v1, a0, 0, 0, 0);
	MoveObject(v1, ((-260) - 24), 0, 260);
	SetCoordinateByReference_Cylind(v3, a0, 0, 0, 0);
	SetObjectCoordinate(v3, ((-264) - 24), 0, 0);
LOC_D4C8:
	SetObjectSpeed_Sphere(v1, 0, (-32), 32);
	SetObjectFlags(v1, 33554432);
	SetCallbackProcedure(v1, a4);
	SetObjectSpeed_Cylind(v3, 23, 0);
	SetObjectScale(v3, 32768, 32768);
	SetObjectFlags(v3, 262144);
	SetObjectOpacity(v3, 4);
	asynccall TraceStoneShadow(v3, v1, 32768, 4);
	if (!(a3 == 1)) goto LOC_D6F0;
	v1 = CreateObjectByReference(intvAttackerMajor, 17013, 0, 0);
	SetCoordinateByReference_Cylind(v1, a0, 0, 0, 0);
	MoveObject(v1, ((-260) - 24), 0, 260);
	SetObjectSpeed_Sphere(v1, 0, (-32), 32);
	SetObjectScale(v1, 98304, 98304);
LOC_D6F0:
	return;
}

void StoneEmitter(int a0, int a1, int a2) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	int v7;
	int v8;
	int v9;
	int v10;
	int v11;
	int v12;
	int v13;
LOC_D6F8:
	SetGlobal(9, a1);
	SetGlobal(10, 1);
	SetGlobal(11, 0);
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\007\\*");
	DisablePlayMagic();
	DownBrightness(12, 5);
	SetOverwhelming(intvAttackerMajor, 1);
	v4 = GetObjectScreenX(intvAttackerMajor);
	v5 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v4, (v5 - 120));
	AddAttackCounter(intvAttackerMajor, 2);
	Delay(20);
	SetObjectAnimate(intvAttackerMajor, 65536);
	SetOverwhelming(intvAttackerMajor, 0);
	Delay(30);
	if (!(a1 == 0)) goto LOC_D898;
	v7 = 7001;
	goto LOC_D8A8;
LOC_D898:
	v7 = 7002;
LOC_D8A8:
	if (!(intvIsLeft == 1)) goto LOC_D8DC;
	v6 = 0;
	goto LOC_D8EC;
LOC_D8DC:
	v6 = 128;
LOC_D8EC:
	if (!(a0 == 0)) goto LOC_D930;
	v8 = 2;
	v1 = 4;
	goto LOC_D950;
LOC_D930:
	v8 = 1;
	v1 = 6;
LOC_D950:
	v13 = Rand(1, 30);
	asynccall ProduceStoneEmitter((((6576 + (96 * 7)) * (intvIsLeft == 0)) + ((240 - (96 * 7)) * (intvIsLeft != 0))), ((180 + ((v1 * 72) * 2)) - (72 * 2)), v13, v6, a1);
	v1 = (v1 - v8);
	if ((v1 >= 0)) goto LOC_D950;
	if (!(a0 == 2)) goto LOC_DC3C;
	v8 = 1;
	v1 = 5;
LOC_DACC:
	v13 = Rand(1, 30);
	asynccall ProduceStoneEmitter(((((6576 + (96 * 7)) + (96 * 2)) * (intvIsLeft == 0)) + (((240 - (96 * 7)) - (96 * 2)) * (intvIsLeft != 0))), ((180 + ((v1 * 72) * 2)) - (72 * 1)), v13, v6, a1);
	v1 = (v1 - v8);
	if ((v1 > 0)) goto LOC_DACC;
LOC_DC3C:
	if (!(intvIsLeft == 1)) goto LOC_DCB0;
	asynccall MoveCamera(((240 - (96 * 4)) - 96), 0, 40);
	goto LOC_DD0C;
LOC_DCB0:
	asynccall MoveCamera((((6576 + (96 * 4)) - 1) + 96), 0, 40);
LOC_DD0C:
	Wait("MoveCamera");
	Delay(92);
	v9 = GetSoldierCount((intvIsLeft ^ 1));
	v10 = 0;
	if (!(v9 < a2)) goto LOC_DD94;
	v10 = (a2 - v9);
LOC_DD94:
	if (!((a2 - v10) > 0)) goto LOC_DFFC;
	v2 = 0;
	v1 = (a2 - v10);
LOC_DDE8:
	v3 = GetRandomSoldierFromAlive((intvIsLeft ^ 1));
	v4 = GetObjectScreenX(v3);
	v5 = GetObjectScreenY(v3);
	if (!(v2 == 0)) goto LOC_DE9C;
	v2 = 1;
	v11 = v4;
	v12 = v4;
	goto LOC_DEF4;
LOC_DE9C:
	if (!(v11 < v4)) goto LOC_DEC8;
	v11 = v4;
LOC_DEC8:
	if (!(v12 > v4)) goto LOC_DEF4;
	v12 = v4;
LOC_DEF4:
	if (!(intvIsLeft == 1)) goto LOC_DF74;
	asynccall FlyStoneRB2(v3, v5, (80 + Rand(1, 30)), a1, v7);
	goto LOC_DFD0;
LOC_DF74:
	asynccall FlyStoneLB2(v3, v5, (80 + Rand(1, 30)), a1, v7);
LOC_DFD0:
	v1--;
	if ((v1 > 0)) goto LOC_DDE8;
	goto LOC_E028;
LOC_DFFC:
	v11 = GetObjectScreenX(intvDefenderMajor);
	v12 = v11;
LOC_E028:
	if (!(v11 == v12)) goto LOC_E07C;
	v11 = (v11 + 512);
	v12 = (v12 - 512);
LOC_E07C:
	v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
	if (!(v3 == 0)) goto LOC_E0D0;
	v3 = intvDefenderMajor;
LOC_E0D0:
	v11 = GetObjectScreenX(v3);
	v1 = v10;
LOC_E0FC:
	v4 = (v11 + Rand((-320), 320));
	v5 = Rand(150, 650);
	if (!(intvIsLeft == 1)) goto LOC_E1D4;
	asynccall FlyStoneRB1(v4, v5, (80 + Rand(1, 30)), a1, v7);
	goto LOC_E230;
LOC_E1D4:
	asynccall FlyStoneLB1(v4, v5, (80 + Rand(1, 30)), a1, v7);
LOC_E230:
	v1--;
	if ((v1 > 0)) goto LOC_E0FC;
	v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
	if (!(v3 == 0)) goto LOC_E2A8;
	v3 = intvDefenderMajor;
LOC_E2A8:
	v4 = GetObjectScreenX(v3);
	v5 = GetObjectScreenY(v3);
	MoveCamera(v4, 0, 80);
	Wait("FallExplode1");
	Wait("Func7005");
	Delay(60);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void ExplodeRoundCallback(int a0, int a1) callsign 8001 {
	int v1;
	if (!(a1 == intvDefenderMajor)) goto LOC_E42C;
	if (!GetGlobal(12)) goto LOC_E3A0;
	v1 = (intvMagicAttackValue / GetGlobal(12));
LOC_E3A0:
	Hurt(a0, a1, v1);
	if (!(v1 > 0)) goto LOC_E424;
	SetGlobal(12, (GetGlobal(12) * 2));
	asynccall LockTargetTime2(a1, 10016, 60);
LOC_E424:
	goto LOC_E478;
LOC_E42C:
	Hurt(a0, a1, 0);
	asynccall FireMan(a1, 10016, 60);
LOC_E478:
	return;
}

void ProduceAirCircle(int a0, int a1, int a2) {
	int v1;
	int v2;
LOC_E480:
	v1 = CreateObjectByReference(a0, a1, 0, 0);
	SetCoordinateByReference_Cylind(v1, a0, 0, 0, 0);
	SetObjectOpacity(v1, 8);
	SetObjectLayer(v1, 2);
	v2 = 1;
LOC_E538:
	if (!(v2 <= 40)) goto LOC_E634;
	Delay(1);
	if (!(IsObjectExist(v1) == 0)) goto LOC_E590;
	goto LOC_E634;
LOC_E590:
	SetObjectScale(v1, (65536 + (12288 * v2)), (65536 + (12288 * v2)));
	if (!(v2 == a2)) goto LOC_E624;
	SetObjectFadeOut(v1, 16, 1);
LOC_E624:
	v2++;
	goto LOC_E538;
LOC_E634:
	return;
}

void CreateExplodeRound(int a0, int a1) {
	int v1;
	Delay(a1);
	v1 = CreateObjectByReference(a0, 18004, 0, 0);
	MoveObject(v1, Rand((-4), 4), Rand(0, 8), 0);
	SetObjectScale(v1, 65536, 65536);
	SetObjectSpeed_Cylind(v1, 0, Rand(4, 5));
	SetObjectScaleShrink(v1, 2176);
	SetObjectFadeOut(v1, 32, 1);
	return;
}

void ProduceRound(int a0, int a1, int a2, int a3) {
	int v1;
	int v2;
	Delay(Rand(1, 30));
	v2 = Rand(0, 255);
	v1 = CreateObjectByReference(a0, a1, v2, 0);
	SetCallbackProcedure(v1, a3);
	SetObjectFlags(v1, 50331648);
	SetCoordinateByReference_Cylind(v1, a0, v2, a2, 0);
	asynccall CreateExplodeRound(v1, Rand(0, 1));
	asynccall CreateExplodeRound(v1, Rand(2, 3));
	asynccall CreateExplodeRound(v1, Rand(4, 5));
	asynccall CreateExplodeRound(v1, Rand(5, 6));
	asynccall CreateExplodeRound(v1, Rand(7, 8));
	asynccall CreateExplodeRound(v1, Rand(9, 10));
	return;
}

void ExplodeRound(int a0, int a1, int a2) {
	int v1;
	int v3;
	int v4;
LOC_E9F4:
	SetGlobal(12, 1);
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\008\\*");
	DisablePlayMagic();
	DownBrightness(8, 5);
	SetOverwhelming(intvAttackerMajor, 1);
	v3 = GetObjectScreenX(intvAttackerMajor);
	v4 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v3, (v4 - 120));
	AddAttackCounter(intvAttackerMajor, 2);
	Delay(10);
	SetObjectAnimate(intvAttackerMajor, 32768);
	v3 = GetObjectScreenX(intvAttackerMajor);
	v4 = GetObjectScreenY(intvAttackerMajor);
	Delay(10);
	ProduceAirCircle(intvAttackerMajor, 18002, ((a0 * 3) / 18));
	SetOverwhelming(intvAttackerMajor, 0);
	Delay(20);
	v1 = 0;
LOC_EBCC:
	if (!(v1 < a0)) goto LOC_EC90;
	asynccall ProduceRound(intvAttackerMajor, a2, Rand(72, a1), 8001);
	Delay(Rand(1, 3));
	if (!(v1 == 0)) goto LOC_EC80;
	PlaySound1("m008snd01", 255);
LOC_EC80:
	v1++;
	goto LOC_EBCC;
LOC_EC90:
	Wait("ProduceRound");
	Wait("CreateExplodeRound");
	Delay(30);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void EightWayFireCallback(int a0, int a1) callsign 9001 {
	int v1;
	asynccall LockTargetTime2(a1, 10015, 60);
	if (!(a1 == intvDefenderMajor)) goto LOC_EDE8;
	if (!GetGlobal(13)) goto LOC_ED5C;
	v1 = (intvMagicAttackValue / GetGlobal(13));
LOC_ED5C:
	Hurt(a0, a1, v1);
	if (!(v1 > 0)) goto LOC_EDE0;
	SetGlobal(13, (GetGlobal(13) * 2));
	asynccall LockTargetTime2(a1, 10015, 60);
LOC_EDE0:
	goto LOC_EE34;
LOC_EDE8:
	Hurt(a0, a1, 0);
	asynccall FireMan(a1, 10015, 60);
LOC_EE34:
	return;
}

void CreateEightWay(int a0, int a1, int a2, int a3, int a4) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	v3 = 32768;
	v4 = 13;
	v5 = 20;
	v6 = 1;
	v1 = CreateObjectByReference(a0, a2, a3, 0);
	SetCallbackProcedure(v1, 9001);
	SetObjectFlags(v1, 262144);
	v2 = 0;
LOC_EF00:
	if (!(v2 < (a1 * 2))) goto LOC_F0C0;
	SetObjectOpacity(v1, v4);
	SetObjectScale(v1, 98304, v3);
	SetCoordinateByReference_Cylind(v1, a0, a3, ((v2 + 1) * v5), 0);
	Delay(2);
	v3 = (v3 + 2048);
	v4 = (v4 + v6);
	if (!(v4 >= 16)) goto LOC_F030;
	v6 = (-3);
	goto LOC_F054;
LOC_F030:
	if (!(v4 <= 0)) goto LOC_F054;
	goto LOC_F0C0;
LOC_F054:
	if (!a4) goto LOC_F0B0;
	a3 = ((a3 + 8) & 255);
	if (!(a1 & 1)) goto LOC_F0B0;
	v5--;
LOC_F0B0:
	v2++;
	goto LOC_EF00;
LOC_F0C0:
	FreeObjectByHandle(v1);
	return;
}

void EightWayFireMotion(int a0, int a1, int a2, int a3, int a4) {
	int v1;
	v1 = 0;
LOC_F0F4:
	if (!(v1 < (a1 - 8))) goto LOC_F174;
	asynccall CreateEightWay(a0, a1, a2, a3, a4);
	Delay(4);
	v1++;
	goto LOC_F0F4;
LOC_F174:
	return;
}

void EightWayFire(int a0, int a1, int a2) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
LOC_F17C:
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\009\\*");
	DisablePlayMagic();
	DownBrightness(12, 5);
	SetOverwhelming(intvAttackerMajor, 1);
	v3 = GetObjectScreenX(intvAttackerMajor);
	v4 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v3, (v4 - 120));
	AddAttackCounter(intvAttackerMajor, 2);
	Delay(20);
	SetObjectAnimate(intvAttackerMajor, 32768);
	SetOverwhelming(intvAttackerMajor, 0);
	SetGlobal(13, 1);
	v3 = GetObjectScreenX(intvAttackerMajor);
	v4 = GetObjectScreenY(intvAttackerMajor);
	if (!(a0 == 0)) goto LOC_F348;
	v1 = 4;
	v2 = 4;
	v5 = 8;
	goto LOC_F3CC;
LOC_F348:
	if (!(a0 == 1)) goto LOC_F39C;
	v1 = 6;
	v2 = 2;
	v5 = 10;
	goto LOC_F3CC;
LOC_F39C:
	v1 = 7;
	v2 = 1;
	v5 = 14;
LOC_F3CC:
	asynccall EightWayFireMotion(intvAttackerMajor, v5, a2, (v1 * 32), a1);
	v1 = (v1 - v2);
	if ((v1 >= 0)) goto LOC_F3CC;
	PlaySound1("m009snd01", 255);
	Wait("CreateEightWay");
	Delay(30);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void ConvexStoneCallback(int a0, int a1) callsign 10001 {
	Hurt(a0, a1, intvMagicAttackValue);
	return;
}

void Func10002(int a0, int a1) callsign 10002 {
	int v1;
	int v2;
	v2 = 0;
LOC_F4EC:
	if (!(v2 < 10)) goto LOC_F668;
	v1 = CreateObjectByReference(a0, Rand(20005, 20010), a1, 0);
	ClearObjectFlags(v1, 65536);
	SetObjectGravity(v1, 10752);
	SetObjectScale(v1, 98304, 98304);
	SetObjectSpeed_Sphere(v1, Rand(0, 256), Rand((64 - 32), (64 + 32)), 6);
	SetObjectFadeOut(v1, 32, 1);
	SetObjectScaleShrink(v1, 512);
	v2++;
	goto LOC_F4EC;
LOC_F668:
	return;
}

void Func10003(int a0, int a1) callsign 10003 {
	int v1;
	int v2;
	v2 = 0;
LOC_F688:
	if (!(v2 < 10)) goto LOC_F804;
	v1 = CreateObjectByReference(a0, Rand(20005, 20010), a1, 0);
	ClearObjectFlags(v1, 65536);
	SetObjectGravity(v1, 10752);
	SetObjectScale(v1, 98304, 98304);
	SetObjectSpeed_Sphere(v1, Rand(0, 256), Rand((64 - 32), (64 + 32)), 7);
	SetObjectFadeOut(v1, 32, 1);
	SetObjectScaleShrink(v1, 512);
	v2++;
	goto LOC_F688;
LOC_F804:
	return;
}

void TraceStone(int a0, int a1) {
	int v1;
	int v2;
	v1 = 0;
LOC_F824:
	if (!IsObjectExist(a0)) goto LOC_FB10;
	v2 = GetObjectScreenZ(a0);
	if (!(v2 <= 0)) goto LOC_FAFC;
	switch (v1) { 
		case 0: goto LOC_F8B0;
		case 1: goto LOC_F974;
		case 2: goto LOC_FA38;
	}
	goto LOC_FAFC;
LOC_F8B0:
	SetObjectFlags(a0, 65536);
	SetObjectSpeed_Sphere(a0, a1, Rand(46, 54), Rand(6, 8));
	Delay(3);
	ClearObjectFlags(a0, 65536);
	SetObjectGravity(a0, 49152);
	v1++;
	goto LOC_FAFC;
LOC_F974:
	SetObjectFlags(a0, 65536);
	SetObjectSpeed_Sphere(a0, a1, Rand(42, 50), Rand(4, 6));
	Delay(2);
	ClearObjectFlags(a0, 65536);
	SetObjectGravity(a0, 32768);
	v1++;
	goto LOC_FAFC;
LOC_FA38:
	SetObjectFlags(a0, 65536);
	SetObjectSpeed_Sphere(a0, a1, Rand(40, 44), Rand(2, 4));
	Delay(1);
	ClearObjectFlags(a0, 65536);
	SetObjectGravity(a0, 16384);
	v1++;
	goto LOC_FAFC;
LOC_FAFC:
	Delay(1);
	goto LOC_F824;
LOC_FB10:
	return;
}

void BrokenStone(int a0, int a1, int a2, int a3, int a4, int a5) {
	int v1;
	int v2;
	v1 = CreateObjectByReference(a0, a1, v2, a3);
	if (!(a2 > 0)) goto LOC_FB88;
	v2 = 0;
	goto LOC_FBAC;
LOC_FB88:
	a2 = (-a2);
	v2 = 128;
LOC_FBAC:
	SetCoordinateByReference_Cylind(v1, a0, v2, a2, a3);
	SetObjectScale(v1, Rand(43008, 73728), Rand(61440, 73728));
	SetObjectScaleShrink(v1, 512);
	Delay(2);
	SetObjectSpeed_Sphere(v1, a5, Rand(52, 60), Rand(6, 8));
	asynccall TraceStone(v1, a5);
	SetObjectFadeOut(v1, 48, 1);
	return;
}

void __dummy_function(){
	return;
}

void function ProduceSomethingXY(int a0, int a1, int a2, int a3, int a4, int a5) {
	int v1;
	v1 = CreateObjectByReference(intvAttackerMajor, a2, a3, 0);
	if (!a4) goto LOC_FD78;
	DoScript(a4, v1, a3, 0);
LOC_FD78:
	SetObjectCoordinate(v1, a0, a1, 0);
	SetObjectSpeed_Sphere(v1, a3, 0, 0);
	SetObjectScale(v1, 98304, 98304);
	if (!a5) goto LOC_104FC;
	Delay(12);
	if (!(a2 == 20002)) goto LOC_101CC;
	asynccall BrokenStone(v1, 20011, Rand((-7), (-9)), 147, 0, 128);
	asynccall BrokenStone(v1, 20012, Rand(6, 8), 134, 0, 0);
	asynccall BrokenStone(v1, 20011, Rand((-7), (-9)), 120, 0, 128);
	asynccall BrokenStone(v1, 20012, Rand(6, 8), 107, 0, 0);
	asynccall BrokenStone(v1, 20014, Rand((-11), (-12)), 99, 0, 128);
	asynccall BrokenStone(v1, 20013, Rand(10, 12), 85, 0, 0);
	asynccall BrokenStone(v1, 20011, Rand((-9), (-10)), 70, 0, 128);
	asynccall BrokenStone(v1, 20014, Rand(11, 13), 67, 0, 0);
	asynccall BrokenStone(v1, 20012, Rand((-10), (-11)), 52, 0, 128);
	asynccall BrokenStone(v1, 20013, Rand(11, 13), 49, 0, 0);
	goto LOC_104AC;
LOC_101CC:
	asynccall BrokenStone(v1, 20011, Rand((-7), (-9)), 120, 0, 128);
	asynccall BrokenStone(v1, 20012, Rand(6, 8), 107, 0, 0);
	asynccall BrokenStone(v1, 20014, Rand((-8), (-9)), 99, 0, 128);
	asynccall BrokenStone(v1, 20013, Rand(7, 9), 85, 0, 0);
	asynccall BrokenStone(v1, 20011, Rand((-6), (-7)), 70, 0, 128);
	asynccall BrokenStone(v1, 20014, Rand(8, 10), 67, 0, 0);
	asynccall BrokenStone(v1, 20012, Rand((-6), (-7)), 52, 0, 128);
	asynccall BrokenStone(v1, 20013, Rand(7, 9), 49, 0, 0);
LOC_104AC:
	Delay(2);
	SetObjectFlags(v1, 128);
	Wait("BrokenStone");
	FreeObjectByHandle(v1);
	goto LOC_1052C;
LOC_104FC:
	Delay(8);
	SetObjectFadeOut(v1, 16, 1);
LOC_1052C:
	return;
}

void ConvexStone(int a0) {
	int v1;
	int v2;
	int v3;
	int v5;
	int v6;
	int v7;
	int v8;
	int v9;
	int v10;
	int v11;
	int v12;
	int v13;
	int v14;
LOC_10534:
	v9 = 10002;
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\010\\*");
	DisablePlayMagic();
	DownBrightness(12, 5);
	SetOverwhelming(intvAttackerMajor, 1);
	v5 = GetObjectScreenX(intvAttackerMajor);
	v6 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v5, (v6 - 120));
	AddAttackCounter(intvAttackerMajor, 2);
	Delay(20);
	SetObjectAnimate(intvAttackerMajor, 65536);
	SetOverwhelming(intvAttackerMajor, 0);
	Delay(30);
	if (!(intvIsLeft == 1)) goto LOC_106B4;
	v10 = 0;
	goto LOC_106C4;
LOC_106B4:
	v10 = 128;
LOC_106C4:
	v5 = GetObjectScreenX(intvDefenderMajor);
	v6 = GetObjectScreenY(intvDefenderMajor);
	MoveCamera(v5, (v6 - 120), 40);
	Delay(15);
	v5 = GetObjectScreenX(intvDefenderMajor);
	v6 = GetObjectScreenY(intvDefenderMajor);
	if (!(a0 == 0)) goto LOC_107A0;
	v1 = 1;
	goto LOC_107F4;
LOC_107A0:
	if (!(a0 == 1)) goto LOC_107D4;
	v1 = 3;
	goto LOC_107F4;
LOC_107D4:
	v1 = 5;
	v9 = 10003;
LOC_107F4:
	v7 = 0;
	v8 = 0;
	v11 = v7;
	v12 = v8;
LOC_10834:
	asynccall ProduceSomethingXY((v5 + v7), (v6 + v8), (20001 + (a0 == 2)), v10, v9, 1);
	asynccall ProduceSomethingXY((v5 + v7), (v6 + v8), (20003 + (a0 == 2)), v10, v9, 0);
	asynccall LockTargetXY((v5 + v7), (v6 + v8), 2501, v10, 8, 10001, 0, 0, (33554432 + 16777216));
	v2 = 1;
LOC_109AC:
	if (!(v2 < 6)) goto LOC_10B38;
	v14 = (((v6 + v8) - ((72 * 3) / 2)) + ((v2 * 72) / 2));
	v3 = 1;
LOC_10A3C:
	if (!(v3 < 6)) goto LOC_10B28;
	v13 = (((v5 + v7) - ((96 * 3) / 2)) + ((v3 * 96) / 2));
	asynccall LockTargetXY(v13, v14, 2501, v10, 8, 10001, 0, 0, 33554432);
	v3++;
	goto LOC_10A3C;
LOC_10B28:
	v2++;
	goto LOC_109AC;
LOC_10B38:
	Delay(Rand(4, 12));
LOC_10B58:
	if (!1) goto LOC_10BEC;
	v7 = Rand((-240), 240);
	if (!((v7 > (v11 + 28)) || (v7 < (v11 - 28)))) goto LOC_10BE4;
	goto LOC_10BEC;
LOC_10BE4:
	goto LOC_10B58;
LOC_10BEC:
	if (!1) goto LOC_10C80;
	v8 = Rand((-110), 340);
	if (!((v8 > (v12 + 36)) || (v8 < (v12 - 36)))) goto LOC_10C78;
	goto LOC_10C80;
LOC_10C78:
	goto LOC_10BEC;
LOC_10C80:
	v11 = v7;
	v12 = v8;
	v1--;
	if ((v1 > 0)) goto LOC_10834;
	Wait("CreateEightWay");
	Delay(60);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void TornadorCallback(int a0, int a1) callsign 11001 {
	int v1;
	int v2;
	int v3;
	int v4;
	SetObjectFlags(a1, 65536);
	SetObjectSpeed_Cylind(a1, 0, Rand(6, 10));
	v1 = Rand(0, 255);
	v4 = Rand(0, 18);
LOC_10DAC:
	v3 = GetObjectScreenZ(a0);
	v2 = GetObjectScreenZ(a1);
	SetCoordinateByReference_Cylind(a1, a0, v1, v4, (v2 - v3));
	Delay(1);
	v1 = ((v1 + 16) & 255);
	if (!(v2 > 240)) goto LOC_10E7C;
	goto LOC_10EC0;
LOC_10E7C:
	if (!(!IsObjectExist(a1))) goto LOC_10EA4;
	goto LOC_10EC0;
LOC_10EA4:
	if (IsObjectExist(a0)) goto LOC_10DAC;
LOC_10EC0:
	FreeObjectByHandle(a1);
	return;
}

void TornadoMotion(int a0, int a1, int a2, int a3, int a4, int a5, int a6) {
	int v1;
	int v2;
	v1 = a4;
LOC_10EF4:
	if (!IsObjectExist(a1)) goto LOC_10FE0;
	if (!a6) goto LOC_10F4C;
	v2 = CalcSin(v1, a3);
	goto LOC_10F70;
LOC_10F4C:
	v2 = CalcCos(v1, a3);
LOC_10F70:
	SetCoordinateByReference_Cylind(a1, a0, 0, v2, a2);
	Delay(1);
	v1 = ((v1 + 4) & 255);
	goto LOC_10EF4;
LOC_10FE0:
	return;
}

void TornadoStoneMotion(int a0, int a1) {
	int v1;
	int v2;
	int v3;
	int v4;
	SetObjectFlags(a1, 65536);
	SetObjectSpeed_Cylind(a1, 0, Rand(4, 12));
	v1 = Rand(0, 255);
	v4 = Rand(40, 80);
LOC_1108C:
	v3 = GetObjectScreenZ(a0);
	v2 = GetObjectScreenZ(a1);
	SetCoordinateByReference_Cylind(a1, a0, v1, v4, (v2 - v3));
	Delay(1);
	v1 = ((v1 + 4) & 255);
	if (!(v2 > 240)) goto LOC_1115C;
	goto LOC_111A0;
LOC_1115C:
	if (!(!IsObjectExist(a1))) goto LOC_11184;
	goto LOC_111A0;
LOC_11184:
	if (IsObjectExist(a0)) goto LOC_1108C;
LOC_111A0:
	FreeObjectByHandle(a1);
	return;
}

void TornadoStone(int a0) {
	int v1;
	Delay(Rand(10, 18));
	v1 = CreateObjectByReference(a0, 21003, 0, 0);
	ClearObjectFlags(v1, 524288);
	SetObjectScale(v1, Rand(16384, 49152), Rand(16384, 49152));
	asynccall TornadoStoneMotion(a0, v1);
	return;
}

void BottomSmoke(int a0) {
	int v1;
	v1 = CreateObjectByReference(a0, 7050, 0, 0);
	SetCoordinateByReference_Cylind(v1, a0, Rand(128, 255), Rand(16, 32), 0);
	SetObjectScale(v1, 16384, 18432);
	SetObjectSpeed_Cylind(v1, 0, Rand(4, 12));
	SetObjectFadeOut(v1, 12, 1);
	SetObjectScaleShrink(v1, (-512));
	return;
}

void BottomMotion(int a0, int a1) {
	int v1;
	v1 = 0;
LOC_1140C:
	if (!IsObjectExist(a1)) goto LOC_114CC;
	SetCoordinateByReference_Cylind(a1, a0, 0, 0, 0);
	if (!((v1 % 5) == 0)) goto LOC_114B0;
	asynccall TornadoStone(a1);
LOC_114B0:
	Delay(1);
	v1++;
	goto LOC_1140C;
LOC_114CC:
	return;
}

void ProduceTornado(int a0) {
	int v1;
	int v2[12];
	int v14;
	int v15;
	v15 = (Rand(0, 2134) & 1);
	v1 = CreateObjectByReference(a0, 21002, 0, 0);
	asynccall BottomMotion(a0, v1);
	v14 = 0;
LOC_1157C:
	if (!(v14 < 12)) goto LOC_116F0;
	v2[v14] = CreateObjectByReference(a0, 21001, 128, 0);
	SetObjectScale(v2[v14], (16384 + ((81920 / 12) * v14)), (65536 + ((98304 / 12) * v14)));
	asynccall TornadoMotion(a0, v2[v14], ((v14 + 1) * 16), Rand(8, 16), (32 * (v14 / 2)), v14, v15);
	v14++;
	goto LOC_1157C;
LOC_116F0:
	if (!IsObjectExist(a0)) goto LOC_11720;
	Delay(2);
	goto LOC_116F0;
LOC_11720:
	SetObjectFadeOut(v1, 16, 1);
	v14 = 0;
LOC_11754:
	if (!(v14 < 12)) goto LOC_117B8;
	Delay(2);
	SetObjectFadeOut(v2[v14], 16, 1);
	v14++;
	goto LOC_11754;
LOC_117B8:
	return;
}

void DummyMotion(int a0, int a1, int a2, int a3) {
	int v1;
	int v2;
	int v3;
	int v4;
	v1 = Rand(3, 4);
	v4 = GetSoldierCount((intvIsLeft ^ 1));
	if (!(v4 == 0)) goto LOC_1184C;
	v2 = (-1);
	goto LOC_118C4;
LOC_1184C:
	if (!(v4 < 3)) goto LOC_1189C;
	v2 = Rand((-2), (-3));
	goto LOC_118C4;
LOC_1189C:
	v2 = Rand((-2), 1);
LOC_118C4:
	v3 = Rand(1, 3);
LOC_118E8:
	if (!IsObjectExist(a1)) goto LOC_119B8;
	SetCoordinateByReference_Cylind(a0, a1, a2, a3, 0);
	Delay(1);
	a2 = ((a2 + v3) & 255);
	if (!((a2 % v1) == 0)) goto LOC_119B0;
	a3 = (a3 + v2);
LOC_119B0:
	goto LOC_118E8;
LOC_119B8:
	FreeObjectByHandle(a0);
	return;
}

int CreateDummyRef(int a0, int a1, int a2) {
	int v1;
LOC_119D4:
	v1 = CreateObjectByReference(a0, 2501, 0, 0);
	SetCoordinateByReference_Cylind(v1, a0, a1, a2, 0);
	SetObjectFlags(v1, (33554432 + 134217728));
	SetCallbackProcedure(v1, 11001);
	return v1;
}

void WaitTarget(int a0, int a1, int a2) {
	int v1;
	int v2;
	int v3 = 0;
LOC_11AA0:
	if (!IsObjectExist(a0)) goto LOC_11C00;
	v1 = GetObjectScreenX(a0);
	v2 = GetObjectScreenY(a0);
	if (!(((v1 < (a1 + (96 / 4))) && (v1 > (a1 - (96 / 4)))) && ((v2 < (a2 + (72 / 4))) && (v2 > (a2 - (72 / 4)))))) goto LOC_11BC0;
	goto LOC_11C00;
LOC_11BC0:
	Delay(1);
	v3++;
	if (!(v3 > 60)) goto LOC_11BF8;
	goto LOC_11C00;
LOC_11BF8:
	goto LOC_11AA0;
LOC_11C00:
	return;
}

int GetSpeed(int a0, int a1, int a2, int a3) {
	int v1;
LOC_11C08:
	return 10;
	a2 = (a2 - a0);
	a3 = (a3 - a1);
	if (!((a2 > 180) || (a3 > 160))) goto LOC_11CA4;
	v1 = 15;
	goto LOC_11D00;
LOC_11CA4:
	if (!((a2 > 100) || (a3 > 85))) goto LOC_11CF0;
	v1 = 10;
	goto LOC_11D00;
LOC_11CF0:
	v1 = 5;
LOC_11D00:
	return v1;
}

int GetDir(int a0, int a1, int a2, int a3) {
	float v1;
	float v2;
LOC_11D10:
	a2 = (a2 - a0);
	a3 = (a3 - a1);
	if (!(a2 == 0)) goto LOC_11DB8;
	if (!(a3 >= 0)) goto LOC_11DA0;
	return 192;
	goto LOC_11DB0;
LOC_11DA0:
	return 64;
LOC_11DB0:
	goto LOC_11E18;
LOC_11DB8:
	if (!(a3 == 0)) goto LOC_11E18;
	if (!(a2 >= 0)) goto LOC_11E08;
	return 128;
	goto LOC_11E18;
LOC_11E08:
	return 0;
LOC_11E18:
	if (!(a2 >= 0)) goto LOC_11E50;
	v1 = ((float)a2);
	goto LOC_11E68;
LOC_11E50:
	v1 = ((float)(-a2));
LOC_11E68:
	if (!(a3 >= 0)) goto LOC_11EA0;
	v2 = ((float)a3);
	goto LOC_11EB8;
LOC_11EA0:
	v2 = ((float)(-a3));
LOC_11EB8:
	v2 = (v2 / v1);
	if (!((a2 > 0) && (a3 > 0))) goto LOC_11F88;
	if (!(v2 < 1058261487)) goto LOC_11F3C;
	return 128;
	goto LOC_11F80;
LOC_11F3C:
	if (!(v2 >= 1071493677)) goto LOC_11F70;
	return 192;
	goto LOC_11F80;
LOC_11F70:
	return 160;
LOC_11F80:
	goto LOC_12168;
LOC_11F88:
	if (!((a2 < 0) && (a3 > 0))) goto LOC_1203C;
	if (!(v2 < 1058261487)) goto LOC_11FF0;
	return 0;
	goto LOC_12034;
LOC_11FF0:
	if (!(v2 >= 1071493677)) goto LOC_12024;
	return 192;
	goto LOC_12034;
LOC_12024:
	return 224;
LOC_12034:
	goto LOC_12168;
LOC_1203C:
	if (!((a2 < 0) && (a3 < 0))) goto LOC_120F0;
	if (!(v2 < 1058261487)) goto LOC_120A4;
	return 0;
	goto LOC_120E8;
LOC_120A4:
	if (!(v2 >= 1071493677)) goto LOC_120D8;
	return 64;
	goto LOC_120E8;
LOC_120D8:
	return 32;
LOC_120E8:
	goto LOC_12168;
LOC_120F0:
	if (!(v2 < 1058261487)) goto LOC_12124;
	return 128;
	goto LOC_12168;
LOC_12124:
	if (!(v2 >= 1071493677)) goto LOC_12158;
	return 0;
	goto LOC_12168;
LOC_12158:
	return 96;
LOC_12168:
	return 0;
}

void Tornado(int a0) {
	int v1;
	int v2;
	int v3;
	int v4[3];
	int v7;
	int v8;
	int v9;
	int v10;
	int v11;
	int v12;
	int v13;
	int v14;
	int v15;
	int v16;
LOC_12178:
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\011\\*");
	DisablePlayMagic();
	DownBrightness(12, 5);
	SetOverwhelming(intvAttackerMajor, 1);
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v1, (v2 - 120));
	AddAttackCounter(intvAttackerMajor, 4);
	Delay(20);
	Delay(2);
	SetObjectAnimate(intvAttackerMajor, 65536);
	SetOverwhelming(intvAttackerMajor, 0);
	if (!(intvIsLeft == 1)) goto LOC_122E8;
	v3 = 0;
	goto LOC_122F8;
LOC_122E8:
	v3 = 128;
LOC_122F8:
	v8 = GetMostImportantSoldier((intvIsLeft ^ 1));
	if (!(v8 == 0)) goto LOC_12390;
	v8 = GetRandomSoldierFromAlive((intvIsLeft ^ 1));
	if (!(v8 == 0)) goto LOC_12390;
	v8 = intvDefenderMajor;
LOC_12390:
	v9 = CreateObjectByReference(v8, 2501, 0, 0);
	SetObjectLife(v9, ((a0 * 30) + Rand(160, 200)));
	PlaySound1("m011snd01", 255);
	asynccall LockCameraSimple(v9, (-320));
	v7 = 0;
LOC_12468:
	if (!(v7 <= a0)) goto LOC_125B8;
	v11 = (((v7 + 1) * 85) + Rand((-10), 10));
	v13 = ((v7 * 83) + Rand((-10), 10));
	v4[v7] = CreateDummyRef(v9, v13, v11);
	asynccall ProduceTornado(v4[v7]);
	asynccall DummyMotion(v4[v7], v9, v13, v11);
	v7++;
	goto LOC_12468;
LOC_125B8:
	if (!IsObjectExist(v9)) goto LOC_12984;
	if (!(IsObjectExist(v8) == 0)) goto LOC_12624;
	v8 = GetMostImportantSoldier((intvIsLeft ^ 1));
LOC_12624:
	if (!v8) goto LOC_127E0;
	v1 = GetObjectXY(v8);
	v2 = GetObjectBattleY(v8);
	v11 = Rand(4, 8);
	v10 = GetSoldierCountInRect((intvIsLeft ^ 1), v1, v2, v11, v11);
	v12 = 0;
	if (!(v10 > 1)) goto LOC_12768;
	v12 = GetNthSoldierInRect((intvIsLeft ^ 1), v1, v2, v11, v11, Rand(1, v10));
LOC_12768:
	if (!(v12 == 0)) goto LOC_127D8;
	v12 = GetRandomSoldierFromAlive((intvIsLeft ^ 1));
	if (!(v12 == 0)) goto LOC_127D8;
	v12 = intvDefenderMajor;
LOC_127D8:
	goto LOC_12834;
LOC_127E0:
	v12 = GetRandomSoldierFromAlive((intvIsLeft ^ 1));
	if (!(v12 == 0)) goto LOC_12834;
	v12 = intvDefenderMajor;
LOC_12834:
	v14 = GetObjectScreenX(v9);
	v15 = GetObjectScreenY(v9);
	v1 = GetObjectScreenX(v12);
	v2 = GetObjectScreenY(v12);
	v16 = GetSpeed(v14, v15, v1, v2);
	SetGlobal(14, (v16 * 2));
	SetGlobal(15, GetDir(v14, v15, v1, v2));
	ApproachObjectTowards(v9, v12, v16);
	asynccall WaitTarget(v9, v1, v2);
	Wait("WaitTarget");
	goto LOC_125B8;
LOC_12984:
	Wait("ProduceTornado");
	Delay(60);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void DuplicatorCallback(int a0, int a1) callsign 12001 {
	Hurt(a0, a1, intvMagicAttackValue);
	if (!((a1 == intvAttackerMajor) | (a1 == intvDefenderMajor))) goto LOC_12B38;
	asynccall SmallFireBall(a0, a1, 12, 0);
	Delay(4);
	asynccall SmallFireBall(a0, a1, 12, 0);
	Delay(4);
	asynccall SmallFireBall(a0, a1, 12, 0);
	Delay(4);
	asynccall SmallFireBall(a0, a1, 12, 0);
	Delay(4);
	asynccall SmallFireBall(a0, a1, 12, 0);
	goto LOC_12B64;
LOC_12B38:
	asynccall Blood(a1, 16);
LOC_12B64:
	return;
}

void TraceDuplicator(int a0, int a1) {
	int v1;
	int v2;
	int v3;
	int v4;
	v4 = 0;
LOC_12B84:
	if (!IsObjectExist(a0)) goto LOC_12D30;
	Delay(1);
	if (!(IsObjectExist(a1) == 0)) goto LOC_12BDC;
	goto LOC_12D30;
LOC_12BDC:
	v1 = GetObjectScreenX(a0);
	v2 = GetObjectScreenX(a1);
	v3 = (v2 - v1);
	if (!(v3 < 0)) goto LOC_12C60;
	v3 = (-v3);
LOC_12C60:
	if (!((v3 < 360) && (v4 < 2))) goto LOC_12CC8;
	SetObjectAnimate(a0, 4);
	v4 = 2;
	goto LOC_12D28;
LOC_12CC8:
	if (!((v3 < 1080) && (v4 < 1))) goto LOC_12D28;
	SetObjectAnimate(a0, 2);
	v4 = 1;
LOC_12D28:
	goto LOC_12B84;
LOC_12D30:
	return;
}

void MoveDuplicator(int a0, int a1, int a2) {
	int v1;
	int v2;
	int v3;
	v3 = GetObjectDir(a0);
	v2 = CreateObjectByReference(a0, a1, v3, 0);
	SetCoordinateByReference_Cylind(v2, a0, v3, (48 + 64), 0);
	v1 = a2;
LOC_12DE0:
	SetCoordinateByReference_Cylind(v2, a0, v3, (48 + 64), 0);
	Delay(1);
	if (!(!IsObjectExist(a0))) goto LOC_12E54;
	goto LOC_12E78;
LOC_12E54:
	v1--;
	if ((v1 >= 0)) goto LOC_12DE0;
LOC_12E78:
	FreeObjectByHandle(v2);
	return;
}

void DuplicatorShadow(int a0, int a1, int a2, int a3, int a4) {
	int v1;
	if (!(a4 == 0)) goto LOC_12F40;
	return;
LOC_12F40:
	v1 = CreateObjectByReference(a0, a1, a2, 0);
	if (!(a2 == 0)) goto LOC_12FC8;
	MoveObject(v1, (-8), 0, 0);
	goto LOC_12FF4;
LOC_12FC8:
	MoveObject(v1, 8, 0, 0);
LOC_12FF4:
	SetObjectSpeed_Sphere(v1, a2, 0, a3);
	SetObjectFlags(v1, 262144);
	SetObjectOpacity(v1, a4);
	Delay(2);
	SetObjectFadeOut(v1, 20, 1);
LOC_13088:
	if (!IsObjectExist(a0)) goto LOC_130E8;
	if (!(IsObjectExist(v1) == 0)) goto LOC_130D4;
	return;
LOC_130D4:
	Delay(1);
	goto LOC_13088;
LOC_130E8:
	FreeObjectByHandle(v1);
	return;
}

void ProduceDuplicator(int a0, int a1, int a2, int a3) {
LOC_12E94:
	if (!IsObjectExist(a0)) goto LOC_12F0C;
	asynccall DuplicatorShadow(a0, a1, a2, (a3 - 8), 14);
	Delay(8);
	goto LOC_12E94;
LOC_12F0C:
	return;
}

void Duplicator(int a0) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	int v7;
	int v8;
	int v9;
	int v10;
LOC_13104:
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\012\\*");
	DisablePlayMagic();
	DownBrightness(12, 5);
	v10 = 0;
	if (!(Rand(0, 3) == 3)) goto LOC_131BC;
	v10 = Rand(0, 90);
LOC_131BC:
	v9 = (1 - (2 * (intvIsLeft != 0)));
	if (!(intvIsLeft == 1)) goto LOC_13224;
	v8 = 0;
	goto LOC_13234;
LOC_13224:
	v8 = 128;
LOC_13234:
	SetOverwhelming(intvAttackerMajor, 1);
	v5 = GetObjectScreenX(intvAttackerMajor);
	v7 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera((v5 - (180 * v9)), ((v7 - 120) + v10));
	Delay(15);
	SetObjectAnimate(intvAttackerMajor, 16384);
	PlaySound(intvAttackerMajor, "att07", 255);
	SetOverwhelming(intvAttackerMajor, 0);
	Delay(10);
	v2 = CreateObjectByReference(intvAttackerMajor, 22002, v8, 0);
	SetObjectSpeed_Sphere(v2, v8, 0, 16);
	SetObjectFlags(v2, (16777216 + 33554432));
	SetCallbackProcedure(v2, 12001);
	asynccall ProduceShadowTime(v2, 9999);
	asynccall MoveDuplicator(v2, 22001, 9999);
	asynccall LockCamera(v2, 180, ((-120) + v10), intvIsLeft);
	v5 = 16;
	v4 = 1;
	v3 = 0;
	asynccall TraceDuplicator(v2, intvDefenderMajor);
LOC_134DC:
	SetObjectSpeed_Sphere(v2, v8, 0, (v5 / 16));
	if (!(v5 < (24 * 16))) goto LOC_13574;
	v5 = (v5 + v4);
	v4 = (v4 + 1);
LOC_13574:
	if (!((v3 & 1) == 0)) goto LOC_13608;
	if (!(v5 < 255)) goto LOC_135E4;
	PlaySound(v2, "m012snd01", 255);
	goto LOC_13608;
LOC_135E4:
	PlaySound(v2, "m012snd01", 255);
LOC_13608:
	v3++;
	Delay(1);
	v1 = IsObjectExist(v2);
	v6 = GetObjectScreenX(v2);
	if (!((v6 <= 0) || (v6 >= GetBattleWidthInScreenX()))) goto LOC_136A0;
	FreeObjectByHandle(v2);
LOC_136A0:
	if (v1) goto LOC_134DC;
	Delay(60);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void StopAllSoldier(int a0, int a1) {
	int v1;
	int v2;
	int v3;
	int v4;
	v1 = 0;
LOC_13700:
	if (!(v1 < 71)) goto LOC_13808;
	v2 = 0;
LOC_1372C:
	if (!(v2 < 15)) goto LOC_137F8;
	v3 = GetForceHandleByXY(v1, v2);
	if (!(v3 && (v3 != intvDefenderMajor))) goto LOC_137E8;
	v4 = GetSoldierSide(v3);
	if (!(a0 == v4)) goto LOC_137E8;
	AddAttackCounter(v3, a1);
LOC_137E8:
	v2++;
	goto LOC_1372C;
LOC_137F8:
	v1++;
	goto LOC_13700;
LOC_13808:
	return;
}

void Func13001(int a0) {
	int v1;
	MoveObject(a0, 0, 0, 256);
	Delay(2);
	SetObjectScale(a0, 65536, 65536);
	SetObjectSpeed_Sphere(a0, 128, 192, 3);
	Delay(64);
	SetObjectSpeed_Sphere(a0, 128, 192, 0);
	Delay(64);
	v1 = 0;
LOC_138F4:
	if (!(v1 < 10)) goto LOC_13968;
	SetObjectScale(a0, 0, (65536 + (v1 * 65536)));
	Delay(1);
	v1++;
	goto LOC_138F4;
LOC_13968:
	if (!(v1 < 30)) goto LOC_13A08;
	SetObjectScale(a0, 0, (65536 + (v1 * 65536)));
	MoveObject(a0, 0, 0, 30);
	Delay(1);
	v1++;
	goto LOC_13968;
LOC_13A08:
	FreeObjectByHandle(a0);
	return;
}

void ProduceSomethingXY2(int a0, int a1, int a2, int a3) {
	int v1;
	v1 = CreateObjectByReference(intvAttackerMajor, a2, 128, 0);
	SetObjectCoordinate(v1, a0, a1, 0);
	asynccall Func13001(v1);
	Wait("Func13001");
	SetObjectFadeOut(v1, 16, 1);
	return;
}

void Frozen() {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	int v7;
LOC_13AF0:
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\013\\*");
	DisablePlayMagic();
	DownBrightness(12, 5);
	SetOverwhelming(intvAttackerMajor, 1);
	v3 = GetObjectScreenX(intvAttackerMajor);
	v4 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v3, (v4 - 120));
	AddAttackCounter(intvAttackerMajor, 2);
	Delay(20);
	SetObjectAnimate(intvAttackerMajor, 65536);
	SetOverwhelming(intvAttackerMajor, 0);
	Delay(30);
	if (!(intvIsLeft == 1)) goto LOC_13C60;
	v6 = 0;
	goto LOC_13C70;
LOC_13C60:
	v6 = 128;
LOC_13C70:
	v7 = GetSoldierCount((intvIsLeft ^ 1));
	if (!(v7 > 0)) goto LOC_13E0C;
	v1 = ((v7 / 2) + 1);
	v2 = GetRandomSoldierFromAlive((intvIsLeft ^ 1));
	v5 = GetObjectScreenX(v2);
LOC_13D20:
	v2 = GetRandomSoldierFromAlive((intvIsLeft ^ 1));
	v3 = GetObjectScreenX(v2);
	if (!(intvIsLeft == 1)) goto LOC_13DB4;
	if (!(v5 > v3)) goto LOC_13DAC;
	v5 = v3;
LOC_13DAC:
	goto LOC_13DE0;
LOC_13DB4:
	if (!(v5 < v3)) goto LOC_13DE0;
	v5 = v3;
LOC_13DE0:
	v1--;
	if ((v1 > 0)) goto LOC_13D20;
	goto LOC_13EA0;
LOC_13E0C:
	if (!(intvIsLeft == 1)) goto LOC_13E68;
	v3 = GetObjectScreenX(intvDefenderMajor);
	v5 = (v3 - 320);
	goto LOC_13EA0;
LOC_13E68:
	v3 = GetObjectScreenX(intvDefenderMajor);
	v5 = (v3 + 320);
LOC_13EA0:
	if (!(intvIsLeft == 1)) goto LOC_13EFC;
	v5 = (v5 + 320);
	SetViewCamera(v5, 0);
	goto LOC_13F34;
LOC_13EFC:
	v5 = (v5 - 320);
	SetViewCamera(v5, 0);
LOC_13F34:
	Delay(15);
	PlaySound1("m013snd01", 255);
	if (!(intvIsLeft == 1)) goto LOC_141A0;
	asynccall ProduceSomethingXY2((v5 - 240), (468 + 240), 23009, v6);
	asynccall ProduceSomethingXY2((v5 - 80), (468 + 240), 23010, v6);
	asynccall ProduceSomethingXY2((v5 + 80), (468 + 240), 23011, v6);
	asynccall ProduceSomethingXY2((v5 + 240), (468 + 240), 23012, v6);
	asynccall ProduceSomethingXY2((v5 - 240), (468 - 240), 23013, v6);
	asynccall ProduceSomethingXY2((v5 - 80), (468 - 240), 23014, v6);
	asynccall ProduceSomethingXY2((v5 + 80), (468 - 240), 23015, v6);
	asynccall ProduceSomethingXY2((v5 + 240), (468 - 240), 23016, v6);
	goto LOC_143C0;
LOC_141A0:
	asynccall ProduceSomethingXY2((v5 - 240), (468 + 240), 23001, v6);
	asynccall ProduceSomethingXY2((v5 - 80), (468 + 240), 23002, v6);
	asynccall ProduceSomethingXY2((v5 + 80), (468 + 240), 23003, v6);
	asynccall ProduceSomethingXY2((v5 + 240), (468 + 240), 23004, v6);
	asynccall ProduceSomethingXY2((v5 - 240), (468 - 240), 23005, v6);
	asynccall ProduceSomethingXY2((v5 - 80), (468 - 240), 23006, v6);
	asynccall ProduceSomethingXY2((v5 + 80), (468 - 240), 23007, v6);
	asynccall ProduceSomethingXY2((v5 + 240), (468 - 240), 23008, v6);
LOC_143C0:
	asynccall StopAllSoldier((intvIsLeft ^ 1), 25);
	Wait("ProduceSomethingXY2");
	Delay(60);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void DemonDancingCallback(int a0, int a1) callsign 14001 {
	Hurt(a0, a1, GetGlobal(16));
	if (!(GetGlobal(16) == 20)) goto LOC_14488;
	SetGlobal(16, 0);
LOC_14488:
	return;
}

void CreateLastAttack(int a0, int a1) {
	int v1;
	int v2;
LOC_14490:
	v1 = CreateObjectByReference(a0, 24005, a1, 0);
	SetObjectFlags(v1, (16777216 + 33554432));
	v2 = 0;
LOC_14504:
	if (!(v2 <= 20)) goto LOC_14588;
	SetCoordinateByReference_Cylind(v1, a0, a1, (48 + (v2 * 4)), 64);
	Delay(1);
	v2++;
	goto LOC_14504;
LOC_14588:
	SetObjectFadeOut(v1, 16, 1);
	return;
}

void DemonDancing() {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	int v7;
LOC_145B4:
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\014\\*");
	DisablePlayMagic();
	DownBrightness(12, 5);
	SetOverwhelming(intvAttackerMajor, 1);
	v4 = GetObjectScreenX(intvAttackerMajor);
	v5 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v4, (v5 - 120));
	if (!(intvIsLeft == 1)) goto LOC_146F0;
	v2 = GetForceHandleByXY(((v4 / 96) + 1), (v5 / 72));
	goto LOC_14738;
LOC_146F0:
	v2 = GetForceHandleByXY(((v4 / 96) - 1), (v5 / 72));
LOC_14738:
	if (!(v2 == intvDefenderMajor)) goto LOC_1476C;
	v6 = 1;
	goto LOC_1477C;
LOC_1476C:
	v6 = 0;
LOC_1477C:
	if (!(intvIsLeft == 1)) goto LOC_147B0;
	v7 = 0;
	goto LOC_147C0;
LOC_147B0:
	v7 = 128;
LOC_147C0:
	v4 = GetObjectScreenX(intvAttackerMajor);
	v5 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v4, (v5 - 120));
	if (!(intvIsLeft == 0)) goto LOC_1488C;
	v2 = GetForceHandleByXY(((v4 / 96) - 1), (v5 / 72));
	goto LOC_148D4;
LOC_1488C:
	v2 = GetForceHandleByXY(((v4 / 96) + 1), (v5 / 72));
LOC_148D4:
	if (!(v2 == intvDefenderMajor)) goto LOC_14E08;
	AddAttackCounter(intvAttackerMajor, 4);
	AddAttackCounter(intvDefenderMajor, 4);
	SetObjectAnimate(intvAttackerMajor, 8192);
	SetOverwhelming(intvAttackerMajor, 0);
	Delay(10);
	PlaySound1("att07", 255);
	PlaySound(intvAttackerMajor, "yell01", 255);
	v2 = CreateObjectByReference(intvAttackerMajor, 24001, v7, 0);
	SetCoordinateByReference_Cylind(v2, intvAttackerMajor, v7, 58, 30);
	Delay(20);
	SetObjectFadeOut(v2, 16, 1);
	v1 = 20;
LOC_14A54:
	SetGlobal(16, 2);
	if (!(intvIsLeft == 1)) goto LOC_14AFC;
	asynccall LockTargetXY((v4 + 96), v5, 2501, v7, 2, 14001, 0, 0, (33554432 + 16777216));
	goto LOC_14B70;
LOC_14AFC:
	asynccall LockTargetXY((v4 - 96), v5, 2501, v7, 2, 14001, 0, 0, (33554432 + 16777216));
LOC_14B70:
	v3 = CreateObjectByReference(intvDefenderMajor, Rand(24002, 24004), v7, Rand(64, (48 + 64)));
	MoveObject(v3, Rand((-32), 32), 0, 0);
	SetObjectFadeOut(v3, 16, 1);
	asynccall SmallFireBall(v3, v3, 10, 0);
	Delay(5);
	v1--;
	if ((v1 > 0)) goto LOC_14A54;
	Delay(10);
	PlaySound(intvAttackerMajor, "att07", 255);
	CreateLastAttack(intvAttackerMajor, v7);
	SetGlobal(16, 20);
	if (!(intvIsLeft == 1)) goto LOC_14D8C;
	asynccall LockTargetXY((v4 + 96), v5, 2501, v7, 2, 14001, 0, 0, (33554432 + 16777216));
	goto LOC_14E00;
LOC_14D8C:
	asynccall LockTargetXY((v4 - 96), v5, 2501, v7, 2, 14001, 0, 0, (33554432 + 16777216));
LOC_14E00:
	goto LOC_1505C;
LOC_14E08:
	AddAttackCounter(intvAttackerMajor, 1);
	SetObjectAnimate(intvAttackerMajor, 16384);
	SetOverwhelming(intvAttackerMajor, 0);
	Delay(10);
	PlaySound1("att07", 255);
	PlaySound(intvAttackerMajor, "yell01", 255);
	v2 = CreateObjectByReference(intvAttackerMajor, 24001, v7, 0);
	SetCoordinateByReference_Cylind(v2, intvAttackerMajor, v7, 58, 30);
	Delay(20);
	SetObjectFadeOut(v2, 16, 1);
	SetGlobal(16, 2);
	if (!(intvIsLeft == 1)) goto LOC_14FE8;
	asynccall LockTargetXY((v4 + 96), v5, 2501, v7, 2, 14001, 0, 0, (33554432 + 16777216));
	goto LOC_1505C;
LOC_14FE8:
	asynccall LockTargetXY((v4 - 96), v5, 2501, v7, 2, 14001, 0, 0, (33554432 + 16777216));
LOC_1505C:
	Delay(90);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void FlyingSwordCallback(int a0, int a1) callsign 15001 {
	int v1;
	if (!(a1 == intvDefenderMajor)) goto LOC_15144;
	if (!GetGlobal(17)) goto LOC_150E4;
	v1 = (intvMagicAttackValue / GetGlobal(17));
LOC_150E4:
	Hurt(a0, a1, v1);
	if (!(v1 > 0)) goto LOC_1513C;
	SetGlobal(17, (GetGlobal(17) * 2));
LOC_1513C:
	goto LOC_15164;
LOC_15144:
	Hurt(a0, a1, 0);
LOC_15164:
	return;
}

void Func15010(int a0, int a1) {
	int v1;
	int v2;
	v2 = 0;
LOC_15184:
	if (!(v2 < 15)) goto LOC_152EC;
	v1 = CreateObjectByReference(a1, Rand(25001, 25002), Rand(0, 255), 0);
	SetObjectGravity(v1, 10752);
	SetObjectScale(v1, 32768, 32768);
	SetObjectSpeed_Sphere(v1, Rand(0, 256), Rand(16, (128 - 16)), 4);
	SetObjectFadeOut(v1, 64, 1);
	SetObjectScaleShrink(v1, 512);
	v2++;
	goto LOC_15184;
LOC_152EC:
	return;
}

void Func15009(int a0, int a1, int a2) {
	int v1;
	v1 = CreateObjectByReference(intvAttackerMajor, 2501, 0, 0);
	SetObjectCoordinate(v1, a0, a1, a2);
	asynccall LockTargetTime2(v1, 25008, 8);
	asynccall Func15010(v1, v1);
	Delay(8);
	FreeObjectByHandle(v1);
	return;
}

void TraceSmallSword(int a0) {
	int v1;
	int v2;
	int v3;
	int v4;
LOC_153E4:
	if (!IsObjectExist(a0)) goto LOC_15604;
	if (!(GetObjectScreenZ(a0) <= 4)) goto LOC_155F0;
	v1 = GetObjectScreenX(a0);
	v2 = GetObjectScreenY(a0);
	FreeObjectByHandle(a0);
	v3 = 0;
LOC_15484:
	if (!(v3 < 10)) goto LOC_155E8;
	v4 = CreateObjectRaw(v1, v2, 0, Rand(0, 255), Rand(25001, 25002));
	SetObjectGravity(v4, 10752);
	SetObjectScale(v4, 32768, 32768);
	SetObjectSpeed_Sphere(v4, Rand(0, 256), Rand(16, 32), 4);
	SetObjectFadeOut(v4, 16, 1);
	SetObjectScaleShrink(v4, 512);
	v3++;
	goto LOC_15484;
LOC_155E8:
	goto LOC_15604;
LOC_155F0:
	Delay(1);
	goto LOC_153E4;
LOC_15604:
	return;
}

void CreateSmallSwords(int a0, int a1, int a2) {
	int v1;
	int v2;
	int v4;
	int v5;
	int v6;
	int v7;
	int v8;
	v6 = 0;
	v4 = GetObjectXY(a0);
	v5 = GetObjectBattleY(a0);
	v7 = GetSoldierCountInRect((intvIsLeft ^ 1), v4, v5, 5, 4);
	v2 = 0;
LOC_156B4:
	if (!(v2 < v7)) goto LOC_15894;
	v8 = GetNthSoldierInRect((intvIsLeft ^ 1), v4, v5, 5, 4, Rand(0, (v7 - 1)));
	if (!v8) goto LOC_15884;
	Delay(Rand(0, 4));
	v1 = CreateObjectByReference(v8, 25005, a2, 280);
	SetObjectSpeed_Sphere(v1, 0, 192, 60);
	SetObjectFlags(v1, 33554432);
	SetCallbackProcedure(v1, 15001);
	PlaySound(v1, "att04", 128);
	asynccall TraceSmallSword(v1);
	v6++;
	if (!(v6 >= 16)) goto LOC_15884;
	goto LOC_15894;
LOC_15884:
	v2++;
	goto LOC_156B4;
LOC_15894:
	v2 = v6;
LOC_158A4:
	if (!(v2 < 24)) goto LOC_159FC;
	Delay(Rand(2, 6));
	v1 = CreateObjectByReference(a0, 25005, a2, 0);
	SetCoordinateByReference_Cylind(v1, a0, Rand(0, 255), Rand(64, 130), 280);
	SetObjectSpeed_Sphere(v1, 0, 192, 16);
	PlaySound(v1, "att04", 128);
	asynccall TraceSmallSword(v1);
	v2++;
	goto LOC_158A4;
LOC_159FC:
	return;
}

void SwordBomb(int a0, int a1, int a2) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	asynccall LockTargetXY((a0 + 96), a1, 2501, a2, 8, 15001, 0, 1, (16777216 + 33554432));
	asynccall LockTargetXY((a0 - 96), a1, 2501, a2, 8, 15001, 0, 1, (16777216 + 33554432));
	asynccall LockTargetXY(a0, (a1 + 72), 2501, a2, 8, 15001, 0, 1, (16777216 + 33554432));
	asynccall LockTargetXY(a0, (a1 - 72), 2501, a2, 8, 15001, 0, 1, (16777216 + 33554432));
	asynccall LockTargetXY((a0 + 96), (a1 + 72), 2501, a2, 8, 15001, 0, 1, (16777216 + 33554432));
	asynccall LockTargetXY((a0 + 96), (a1 - 72), 2501, a2, 8, 15001, 0, 1, (16777216 + 33554432));
	asynccall LockTargetXY((a0 - 96), (a1 + 72), 2501, a2, 8, 15001, 0, 1, (16777216 + 33554432));
	asynccall LockTargetXY((a0 - 96), (a1 - 72), 2501, a2, 8, 15001, 0, 1, (16777216 + 33554432));
	PlaySound1("m015snd02", 128);
	asynccall Func15009(a0, a1, 0);
	Delay(10);
	PlaySound1("m015snd02", 128);
	asynccall Func15009(a0, a1, 16);
	Delay(8);
	PlaySound1("m015snd02", 128);
	asynccall Func15009(a0, a1, 32);
	Delay(6);
	PlaySound1("m015snd02", 128);
	asynccall Func15009(a0, a1, 48);
	Delay(6);
	PlaySound1("m015snd02", 128);
	asynccall Func15009(a0, a1, 64);
	Delay(6);
	PlaySound1("m015snd02", 128);
	asynccall Func15009(a0, a1, 80);
	v5 = 0;
	v1 = 0;
LOC_15FE8:
	if (!(v1 < 16)) goto LOC_1626C;
	v2 = 0;
LOC_16014:
	if (!(v2 < Rand(1, 2))) goto LOC_16108;
	v3 = (a0 + Rand((-48), 48));
	v4 = (a1 + Rand((-16), 16));
	CreateObjectRaw(v3, v4, v5, 0, Rand(25007, 25008));
	v2++;
	goto LOC_16014;
LOC_16108:
	if (!(Rand(0, 3) == 0)) goto LOC_16250;
	v3 = (a0 + Rand((-48), 48));
	v4 = (a1 + Rand((-16), 16));
	v6 = CreateObjectRaw(v3, v4, v5, Rand(0, 255), Rand(25009, 25010));
	SetObjectSpeed_Cylind(v6, Rand(10, 16), Rand(10, 16));
LOC_16250:
	Delay(1);
	v1++;
	goto LOC_15FE8;
LOC_1626C:
	return;
}

void CreateBigSword(int a0, int a1) {
	int v1;
	int v2;
	int v3;
	int v4;
	v3 = CreateObjectByReference(a0, 25006, a1, 0);
	MoveObject(v3, 0, 0, 300);
	SetObjectScale(v3, 65536, 65536);
	SetObjectSpeed_Sphere(v3, 0, 192, 40);
	SetObjectFlags(v3, (16777216 + 33554432));
	SetCallbackProcedure(v3, 15001);
	v4 = 1;
LOC_16380:
	if (!(v4 < 10)) goto LOC_1640C;
	SetObjectScale(v3, (98304 + (v4 * 8192)), (106496 + (v4 * 8192)));
	Delay(1);
	v4++;
	goto LOC_16380;
LOC_1640C:
	Delay(10);
	v1 = GetObjectScreenX(v3);
	v2 = GetObjectScreenY(v3);
	asynccall SwordBomb(v1, v2, a1);
	SetObjectFadeOut(v3, 16, 2);
	return;
}

void SwordAttack(int a0, int a1) {
	int v2;
	int v3;
	int v4;
	int v5;
	v5 = GetSoldierCount((intvIsLeft ^ 1));
	v2 = 0;
	if (!(v5 > 0)) goto LOC_16588;
	if (!a1) goto LOC_1653C;
	v2 = GetMostImportantSoldier((intvIsLeft ^ 1));
LOC_1653C:
	if (!(v2 == 0)) goto LOC_16580;
	v2 = GetRandomSoldierFromAlive((intvIsLeft ^ 1));
LOC_16580:
	goto LOC_16598;
LOC_16588:
	v2 = intvDefenderMajor;
LOC_16598:
	v3 = GetObjectScreenX(v2);
	v4 = GetObjectScreenY(v2);
	MoveCamera(v3, (v4 - 240), 20);
	asynccall CreateBigSword(v2, a0);
	Delay(10);
	asynccall CreateSmallSwords(v2, Rand(0, 63), a0);
	Delay(90);
	return;
}

void ProduceSwordShadow(int a0, int a1, int a2) {
LOC_16688:
	if (!IsObjectExist(a0)) goto LOC_166E0;
	CreateShadowObject(a0, a1, a2);
	Delay(3);
	goto LOC_16688;
LOC_166E0:
	return;
}

void LargerSword(int a0, int a1, int a2, int a3) {
	int v1;
	int v3;
	int v4;
	int v5;
	v3 = 256;
	v4 = 256;
	v1 = 0;
LOC_16720:
	if (!(v1 < 32)) goto LOC_167EC;
	SetObjectScale(a0, v3, v4);
	Delay(1);
	if (!(v3 < a1)) goto LOC_167A4;
	v3 = (v3 + 2048);
LOC_167A4:
	if (!(v4 < a2)) goto LOC_167DC;
	v4 = (v4 + 2048);
LOC_167DC:
	v1++;
	goto LOC_16720;
LOC_167EC:
	Delay(30);
	v5 = CreateObjectByReference(a0, 25004, a3, 0);
	FreeObjectByHandle(a0);
	SetObjectSpeed_Sphere(v5, 0, 64, (-1));
	Delay(25);
	SetObjectSpeed_Sphere(v5, 0, 64, 16);
	asynccall ProduceSwordShadow(v5, 16, 1);
	SetObjectLife(v5, 60);
	return;
}

void PrepareSword(int a0, int a1, int a2) {
	int v1;
	v1 = CreateObjectByReference(a0, 25003, a2, 0);
	SetCoordinateByReference_Cylind(v1, a0, a1, 128, 64);
	SetObjectSpeed_Sphere(v1, 0, 64, 1);
	SetObjectScale(v1, 256, 256);
	asynccall LargerSword(v1, 65536, 65536, a2);
	return;
}

void FlyingSword(int a0) {
	int v1;
	int v3;
	int v4;
	int v5;
	int v6;
	int v7;
	int v8;
	int v9;
LOC_169EC:
	SetGlobal(17, 1);
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\015\\*");
	DisablePlayMagic();
	DownBrightness(12, 5);
	SetOverwhelming(intvAttackerMajor, 1);
	v3 = GetObjectScreenX(intvAttackerMajor);
	v4 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v3, (v4 - 120));
	AddAttackCounter(intvAttackerMajor, 2);
	Delay(20);
	SetObjectAnimate(intvAttackerMajor, 32768);
	SetOverwhelming(intvAttackerMajor, 0);
	if (!(intvIsLeft == 1)) goto LOC_16B60;
	v7 = 0;
	goto LOC_16B70;
LOC_16B60:
	v7 = 128;
LOC_16B70:
	v5 = GetObjectScreenX(intvAttackerMajor);
	GetObjectScreenY(intvAttackerMajor);
	Delay(15);
	switch (a0) { 
		case 0: goto LOC_16BE8;
		case 1: goto LOC_16C1C;
		case 2: goto LOC_16C50;
	}
	goto LOC_16C84;
LOC_16BE8:
	v8 = 4;
	PlaySound1("m015snd01", 255);
	goto LOC_16C84;
LOC_16C1C:
	v8 = 8;
	PlaySound1("m015snd01", 255);
	goto LOC_16C84;
LOC_16C50:
	v8 = 16;
	PlaySound1("m015snd03", 255);
	goto LOC_16C84;
LOC_16C84:
	v6 = (256 / v8);
	v9 = Rand(0, 63);
	v1 = 0;
LOC_16CD4:
	if (!(v1 < v8)) goto LOC_16D50;
	asynccall PrepareSword(intvAttackerMajor, (v9 + (v1 * v6)), v7);
	Delay(1);
	v1++;
	goto LOC_16CD4;
LOC_16D50:
	Delay(135);
	v1 = 0;
LOC_16D6C:
	if (!(v1 <= a0)) goto LOC_16DE8;
	asynccall SwordAttack(v7, ((2 - v1) / 2));
	Delay(100);
	v1++;
	goto LOC_16D6C;
LOC_16DE8:
	Wait("SwordBomb");
	Wait("TraceSmallSword");
	Delay(60);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void CreateHolyBall(int a0, int a1, int a2) {
	int v1;
	int v2;
	int v3;
	Delay(Rand(1, 8));
	v1 = CreateObjectByReference(a0, a2, 0, a1);
	v2 = Rand(49152, 81920);
	SetObjectScale(v1, v2, v2);
	SetCoordinateByReference_Cylind(v1, a0, Rand(0, 255), Rand(8, 38), a1);
	SetObjectFlags(v1, 262144);
	SetObjectOpacity(v1, 0);
	SetObjectSpeed_Cylind(v1, 0, Rand(1, 7));
	v3 = 1;
LOC_16FB8:
	if (!(v3 <= 8)) goto LOC_1700C;
	Delay(1);
	SetObjectOpacity(v1, v3);
	v3++;
	goto LOC_16FB8;
LOC_1700C:
	v3 = Rand(16, 32);
	SetObjectScaleShrink(v1, 256);
	SetObjectFadeOut(v1, v3, 1);
	Delay(v3);
	return;
}

void RoundHolyBall(int a0, int a1, int a2, int a3) {
	int v1;
	v1 = CreateObjectByReference(a0, a3, 0, a1);
	SetObjectScale(v1, 81920, 81920);
	SetCoordinateByReference_Cylind(v1, a0, 192, 16, a1);
	MoveObject(v1, a2, 0, 0);
	Delay(2);
	SetObjectFadeOut(v1, 8, 1);
	return;
}

void IncreaseHP() {
	int v1;
	int v2;
	v1 = GetMajorHP(intvIsLeft);
	v2 = 1;
LOC_171B0:
	if (!(v2 <= intvMagicAttackValue)) goto LOC_17210;
	SetMajorHP(intvIsLeft, (v1 + v2));
	Delay(2);
	v2++;
	goto LOC_171B0;
LOC_17210:
	return;
}

void TraceHolyLight(int a0, int a1, int a2, int a3) {
	int v1;
	int v2;
	int v3;
	v3 = (a2 - a1);
	v1 = 1;
LOC_17614:
	if (!(v1 <= a3)) goto LOC_176A4;
	v2 = (a1 + ((v3 / a3) * v1));
	SetObjectScale(a0, v2, 65536);
	Delay(1);
	v1++;
	goto LOC_17614;
LOC_176A4:
	Delay(60);
	SetObjectScaleShrink(a0, 1024);
	SetObjectFadeOut(a0, 48, 1);
	Delay(48);
	return;
}

void CreateHolyLight(int a0, int a1) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
LOC_17218:
	switch (a1) { 
		case 0: goto LOC_17258;
		case 1: goto LOC_17280;
		case 2: goto LOC_172A8;
	}
	goto LOC_172D0;
LOC_17258:
	v4 = 26001;
	v6 = 26004;
	goto LOC_172D0;
LOC_17280:
	v4 = 26002;
	v6 = 26005;
	goto LOC_172D0;
LOC_172A8:
	v4 = 26003;
	v6 = 26006;
	goto LOC_172D0;
LOC_172D0:
	if (!(intvIsLeft == 1)) goto LOC_17304;
	v5 = 0;
	goto LOC_17318;
LOC_17304:
	v5 = (-6);
LOC_17318:
	v1 = CreateObjectByReference(a0, v4, 0, 220);
	SetObjectScale(v1, 2048, 65536);
	SetCoordinateByReference_Cylind(v1, a0, 192, 16, 240);
	MoveObject(v1, v5, 0, 0);
	SetObjectGravity(v1, 98304);
	v2 = 0;
LOC_173FC:
	if (!(v2 < 30)) goto LOC_17478;
	asynccall RoundHolyBall(a0, (240 - (v2 * 8)), v5, v6);
	Delay(1);
	v2++;
	goto LOC_173FC;
LOC_17478:
	Delay(40);
	asynccall TraceHolyLight(v1, 2048, 69632, 32);
	asynccall IncreaseHP();
	v3 = 0;
LOC_174EC:
	if (!(v3 < 8)) goto LOC_175CC;
	v2 = 0;
LOC_17518:
	if (!(v2 < 10)) goto LOC_1759C;
	asynccall CreateHolyBall(a0, Rand(5, (40 + (v3 * 2))), v6);
	v2++;
	goto LOC_17518;
LOC_1759C:
	Delay(Rand(8, 12));
	v3++;
	goto LOC_174EC;
LOC_175CC:
	Wait("TraceHolyLight");
	return;
}

void Heal(int a0) {
	int v1;
	int v2;
LOC_17704:
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\016\\*");
	DisablePlayMagic();
	DownBrightness(8, 5);
	SetOverwhelming(intvAttackerMajor, 1);
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v1, (v2 - 120));
	AddAttackCounter(intvAttackerMajor, 5);
	Delay(20);
	SetObjectAnimate(intvAttackerMajor, 32768);
	Delay(10);
	PlaySound1("m016snd01", 255);
	CreateHolyLight(intvAttackerMajor, a0);
	Delay(10);
	SetOverwhelming(intvAttackerMajor, 0);
	Delay(60);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void FireDragonCallback(int a0, int a1) callsign 17001 {
	if (!(a1 == intvDefenderMajor)) goto LOC_17938;
	Hurt(a0, a1, GetGlobal(18));
	SetGlobal(18, 2);
	asynccall LockTargetTime2(a1, 10015, 60);
	goto LOC_17984;
LOC_17938:
	Hurt(a0, a1, 0);
	asynccall FireMan(a1, 10015, 60);
LOC_17984:
	return;
}

void CreateStart(int a0, int a1) {
	int v1;
	int v2;
	v1 = CreateObjectByReference(a0, 27010, a1, 64);
	SetObjectScale(v1, 131072, 114688);
	SetObjectFlags(v1, 262144);
	v2 = 0;
LOC_17A18:
	if (!(v2 < 16)) goto LOC_17A6C;
	SetObjectOpacity(v1, v2);
	Delay(1);
	v2++;
	goto LOC_17A18;
LOC_17A6C:
	ClearObjectFlags(v1, 262144);
	Delay(64);
	SetObjectFadeOut(v1, 16, 2);
	return;
}

void sc4502(int a0, int a1, int a2) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	v1 = CreateObjectByReference(a0, (27005 + ((GetGlobal(19) / 4) & 1)), a1, 0);
	v3 = 0;
	v4 = 0;
LOC_18130:
	if (!(v3 > 3)) goto LOC_185F8;
	SetObjectScale(v1, 65536, 65536);
	v3 = 0;
	v4++;
	if (!(v4 == 7)) goto LOC_181B4;
	v3 = 1;
LOC_181B4:
	if (!(v4 == 8)) goto LOC_185B0;
	v2 = CreateObjectByReference(v1, 27007, a1, 0);
	v5 = ((a2 & 255) - 128);
	if (!(v5 < 0)) goto LOC_18264;
	v5 = (0 - v5);
LOC_18264:
	if (!(v5 < 16)) goto LOC_182A4;
	SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017c50001");
	goto LOC_18400;
LOC_182A4:
	if (!(v5 < 32)) goto LOC_182E4;
	SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017c30001");
	goto LOC_18400;
LOC_182E4:
	if (!(v5 < 48)) goto LOC_18324;
	SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017c30001");
	goto LOC_18400;
LOC_18324:
	if (!(v5 < 80)) goto LOC_18364;
	SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017c10001");
	goto LOC_18400;
LOC_18364:
	if (!(v5 < 96)) goto LOC_183A4;
	SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017c10001");
	goto LOC_18400;
LOC_183A4:
	if (!(v5 < 112)) goto LOC_183E4;
	SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017c20001");
	goto LOC_18400;
LOC_183E4:
	SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017c40001");
LOC_18400:
	v5 = (a1 & 255);
	if (!((v5 >= 192) && (v5 < 224))) goto LOC_1846C;
	SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017c50001");
LOC_1846C:
	if (!((v5 > 170) && (v5 < 192))) goto LOC_184BC;
	SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017c70001");
LOC_184BC:
	SetObjectScale(v2, (65536 * (1 - (2 * (((a1 - 64) & 255) > 128)))), 65536);
	SetObjectScale(v1, (49152 * (1 - (2 * (((a1 - 64) & 255) > 128)))), 49152);
	SetObjectLife(v2, 1);
LOC_185B0:
	if (!(v4 == 9)) goto LOC_185F0;
	SetObjectScale(v1, 256, 256);
LOC_185F0:
	goto LOC_1861C;
LOC_185F8:
	SetObjectScale(v1, 256, 256);
LOC_1861C:
	Delay(1);
	v3++;
	if (!(v4 == 9)) goto LOC_18654;
	goto LOC_1867C;
LOC_18654:
	if ((IsObjectExist(a0) != 0)) goto LOC_18130;
LOC_1867C:
	FreeObjectByHandle(v1);
	return;
}

void sc4501(int a0, int a1) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	int v7;
	int v8;
	v4 = 0;
	v5 = 0;
	asynccall CreateStart(a0, a1);
	Delay(32);
	v2 = CreateObjectByReference(a0, 27002, a1, 64);
	SetObjectFlags(v2, (33554432 + 16777216));
	SetCallbackProcedure(v2, 17001);
	v1 = (60 * 6);
	v4 = 128;
	v5 = 0;
	PlaySound1("m017snd01", 255);
LOC_17BF0:
	SetObjectSpeed_Sphere(v2, v4, 0, 7);
	v6 = GetObjectScreenX(v2);
	v7 = GetObjectScreenY(v2);
	v8 = CalcSin(v5, 32);
	SetObjectCoordinate(v2, v6, v7, (64 + v8));
	SetObjectScale(v2, (65536 * (1 - (2 * (((v4 - 64) & 255) > 128)))), 65536);
	v3 = ((v5 & 255) - 128);
	if (!(v3 < 0)) goto LOC_17D7C;
	v3 = (0 - v3);
LOC_17D7C:
	if (!(v3 < 16)) goto LOC_17DBC;
	SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017a40001");
	goto LOC_17F18;
LOC_17DBC:
	if (!(v3 < 32)) goto LOC_17DFC;
	SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017a20001");
	goto LOC_17F18;
LOC_17DFC:
	if (!(v3 < 48)) goto LOC_17E3C;
	SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017a20001");
	goto LOC_17F18;
LOC_17E3C:
	if (!(v3 < 80)) goto LOC_17E7C;
	SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017a10001");
	goto LOC_17F18;
LOC_17E7C:
	if (!(v3 < 96)) goto LOC_17EBC;
	SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017a10001");
	goto LOC_17F18;
LOC_17EBC:
	if (!(v3 < 112)) goto LOC_17EFC;
	SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017a30001");
	goto LOC_17F18;
LOC_17EFC:
	SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017a30001");
LOC_17F18:
	v3 = (v4 & 255);
	if (!((v3 >= 192) && (v3 < 224))) goto LOC_17F84;
	SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017a40001");
LOC_17F84:
	if (!((v3 > 170) && (v3 < 192))) goto LOC_17FD4;
	SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017a60001");
LOC_17FD4:
	asynccall sc4502(v2, v4, v5);
	Delay(1);
	SetGlobal(19, GetGlobal(19) + 1);
	v4 = (v4 + 2);
	v5 = (v5 + 4);
	v1--;
	if ((v1 > 0)) goto LOC_17BF0;
	SetObjectFadeOut(v2, 16, 1);
	Delay(16);
	SetGlobal(20, GetGlobal(20) + 1);
	return;
}

void FireDragon(int a0) {
	int v2;
	int v3;
	int v4;
	int v6;
	int v7;
	int v8;
	int v9;
	int v10;
	int v11;
LOC_18698:
	SetGlobal(18, intvMagicAttackValue);
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\017\\*");
	DisablePlayMagic();
	DownBrightness(8, 5);
	SetOverwhelming(intvAttackerMajor, 1);
	v3 = GetObjectScreenX(intvAttackerMajor);
	v4 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v3, (v4 - 120));
	AddAttackCounter(intvAttackerMajor, 2);
	Delay(20);
	SetObjectAnimate(intvAttackerMajor, 65536);
	Delay(20);
	SetOverwhelming(intvAttackerMajor, 0);
	SetGlobal(19, 0);
	SetGlobal(20, 0);
	if (!(intvIsLeft == 1)) goto LOC_18838;
	v11 = 128;
	goto LOC_18848;
LOC_18838:
	v11 = 128;
LOC_18848:
	v2 = GetMostImportantSoldier((intvIsLeft ^ 1));
	if (!(v2 == 0)) goto LOC_1889C;
	v2 = intvDefenderMajor;
LOC_1889C:
	v3 = GetObjectScreenX(v2);
	v4 = GetObjectScreenY(v2);
	MoveCamera(v3, (v4 - 220), 20);
	v2 = CreateObjectByReference(v2, 2501, v11, 0);
	PlaySound1("m017snd02", 255);
	SetObjectCoordinate(v2, (v3 + 120), (v4 + 120), 0);
	asynccall sc4501(v2, v11);
	Delay(50);
	if (!(a0 > 0)) goto LOC_18B50;
	v6 = CreateObjectByReference(v2, 2501, v11, 0);
	v8 = GetMostImportantSoldier((intvIsLeft ^ 1));
	if (!(v8 == 0)) goto LOC_18A70;
	v8 = intvDefenderMajor;
LOC_18A70:
	v3 = GetObjectScreenX(v8);
	v4 = GetObjectScreenY(v8);
	MoveCamera(v3, (v4 - 220), 20);
	SetObjectCoordinate(v6, (v3 + 120), (v4 + 120), 0);
	asynccall sc4501(v6, v11);
	Delay(50);
LOC_18B50:
	if (!(a0 > 1)) goto LOC_18CD4;
	v7 = CreateObjectByReference(v2, 2501, v11, 0);
	v8 = GetMostImportantSoldier((intvIsLeft ^ 1));
	if (!(v8 == 0)) goto LOC_18BF4;
	v8 = intvDefenderMajor;
LOC_18BF4:
	v3 = GetObjectScreenX(v8);
	v4 = GetObjectScreenY(v8);
	MoveCamera(v3, (v4 - 220), 20);
	SetObjectCoordinate(v7, (v3 + 120), (v4 + 120), 0);
	asynccall sc4501(v7, v11);
	Delay(50);
LOC_18CD4:
	if (!(a0 > 2)) goto LOC_18E58;
	v9 = CreateObjectByReference(v2, 2501, v11, 0);
	v8 = GetRandomSoldierFromAlive((intvIsLeft ^ 1));
	if (!(v8 == 0)) goto LOC_18D78;
	v8 = intvDefenderMajor;
LOC_18D78:
	v3 = GetObjectScreenX(v8);
	v4 = GetObjectScreenY(v8);
	MoveCamera(v3, (v4 - 220), 20);
	SetObjectCoordinate(v9, (v3 + 120), (v4 + 120), 0);
	asynccall sc4501(v9, v11);
	Delay(50);
LOC_18E58:
	if (!(a0 > 3)) goto LOC_18FD0;
	v10 = CreateObjectByReference(v2, 2501, v11, 0);
	v8 = GetRandomSoldierFromAlive((intvIsLeft ^ 1));
	if (!(v8 == 0)) goto LOC_18EFC;
	v8 = intvDefenderMajor;
LOC_18EFC:
	v3 = GetObjectScreenX(v8);
	v4 = GetObjectScreenY(v8);
	MoveCamera(v3, (v4 - 220), 20);
	SetObjectCoordinate(v10, (v3 + 120), (v4 + 120), 0);
	asynccall sc4501(v10, v11);
LOC_18FD0:
	Delay(1);
	if ((GetGlobal(20) <= a0)) goto LOC_18FD0;
	FreeObjectByHandle(v2);
	FreeObjectByHandle(v6);
	FreeObjectByHandle(v7);
	FreeObjectByHandle(v9);
	FreeObjectByHandle(v10);
	Delay(60);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void CallDragonCallback(int a0, int a1) callsign 18001 {
	if (!((a1 == intvDefenderMajor) && (GetGlobal(22) == 0))) goto LOC_1912C;
	Hurt(a0, a1, intvMagicAttackValue);
	SetGlobal(22, 1);
	asynccall LockTargetTime2(a1, 10015, 60);
	goto LOC_19178;
LOC_1912C:
	Hurt(a0, a1, 0);
	asynccall FireMan(a1, 10015, 60);
LOC_19178:
	return;
}

void sc3602(int a0, int a1, int a2, int a3) {
	int v1;
	int v2;
	int v3;
	int v5;
	int v6;
	switch (a0) { 
		case 0: goto LOC_19434;
		case 1: goto LOC_1944C;
	}
	goto LOC_19464;
LOC_19434:
	v5 = 10;
	goto LOC_19474;
LOC_1944C:
	v5 = 20;
	goto LOC_19474;
LOC_19464:
	v5 = 20;
LOC_19474:
	v1 = 0;
LOC_19484:
	if (!(v1 < v5)) goto LOC_195E8;
	v2 = (a1 + Rand((-48), 48));
	v3 = (a2 + Rand((-16), 16));
	v6 = CreateObjectRaw(v2, v3, (a3 + 20), 0, Rand(28002, 28004));
	SetObjectSpeed_Cylind(v6, 0, Rand(10, 30));
	PlaySound1("m016a", 255);
	Delay(Rand(0, 5));
	v1++;
	goto LOC_19484;
LOC_195E8:
	Delay(15);
	return;
}

void sc3601(int a0, int a1, int a2, int a3, int a4) {
	int v1;
	int v2;
	Delay(Rand(10, 50));
	v1 = CreateObjectRaw(a2, a3, a4, a1, 28001);
	v2 = 60;
LOC_191F4:
	if (!(v2 > 0)) goto LOC_19260;
	a4++;
	SetObjectCoordinate(v1, a2, a3, a4);
	v2--;
	Delay(1);
	goto LOC_191F4;
LOC_19260:
	Delay(30);
	asynccall sc3602(a0, a2, a3, a4);
	v2 = 0;
LOC_192A8:
	if (!(v2 < 10)) goto LOC_1931C;
	SetObjectScale(v1, 0, (65536 + (v2 * 65536)));
	Delay(1);
	v2++;
	goto LOC_192A8;
LOC_1931C:
	if (!(v2 < 30)) goto LOC_193E4;
	a4 = (a4 + (v2 * 10));
	SetObjectScale(v1, 0, (65536 + (v2 * 65536)));
	SetObjectCoordinate(v1, a2, a3, a4);
	Delay(1);
	v2++;
	goto LOC_1931C;
LOC_193E4:
	FreeObjectByHandle(v1);
	return;
}

void sc3603(int a0, int a1, int a2, int a3) {
	int v1;
	int v2;
	int v3;
	CreateObjectRaw(a1, a2, Rand(320, 360), 0, 28005);
	Delay(Rand(4, 10));
	v1 = (2 + (3 * a0));
LOC_19698:
	v1--;
	if (!(v1 > 0)) goto LOC_19798;
	v2 = (a1 + Rand((-320), 320));
	v3 = (a2 + Rand((-100), 300));
	CreateObjectRaw(v2, v3, Rand(320, 360), 0, 28005);
	Delay(Rand(4, 10));
	goto LOC_19698;
LOC_19798:
	return;
}

void sc2512(int a0) {
	int v1;
	int v2;
	v1 = 16384;
	SetObjectSpeed_Cylind(a0, Rand(4, 12), Rand((-2), 8));
	SetObjectFriction1(a0, 8192);
	v2 = Rand(30, 40);
LOC_19848:
	v2--;
	if (!(v2 > 0)) goto LOC_198C0;
	SetObjectScale(a0, v1, v1);
	v1 = (v1 - 256);
	Delay(1);
	goto LOC_19848;
LOC_198C0:
	FreeObjectByHandle(a0);
	return;
}

void sc3604() callsign 18002 {
	int v1;
	int v2;
	int v3;
	v3 = GetScriptLinkedObject();
	SetScriptLinkedObject(intvAttackerMajor);
	asynccall MovingShadow(v3, GetGlobal(21), 28011, 0, 18001);
	asynccall MovingShadow(v3, GetGlobal(21), 28011, 128, 18001);
	asynccall MovingShadow(v3, GetGlobal(21), 28011, 64, 18001);
	asynccall MovingShadow(v3, GetGlobal(21), 28011, 192, 18001);
	SetObjectSpeed_Cylind(v3, 0, 0);
	SetObjectHasGravity(v3, 0);
	v2 = 0;
LOC_19A4C:
	if (!(v2 < 20)) goto LOC_19B10;
	v1 = CreateObjectByReference(v3, 10003, Rand(0, 255), 0);
	SetObjectScale(v1, 16384, 16384);
	asynccall sc2512(v1);
	v2++;
	goto LOC_19A4C;
LOC_19B10:
	PlaySound(v1, "m018snd03", 255);
	return;
}

void CallDragon(int a0) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v7;
	int v8;
	int v9;
	int v10;
	int v11;
	int v12;
	int v13;
LOC_19B3C:
	SetGlobal(22, 0);
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\018\\*");
	DisablePlayMagic();
	DownBrightness(12, 5);
	SetOverwhelming(intvAttackerMajor, 1);
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v1, (v2 - 120));
	AddAttackCounter(intvAttackerMajor, 2);
	Delay(15);
	SetObjectAnimate(intvAttackerMajor, 32768);
	Delay(20);
	SetOverwhelming(intvAttackerMajor, 0);
	if (!(intvIsLeft == 1)) goto LOC_19CBC;
	v10 = 0;
	goto LOC_19CCC;
LOC_19CBC:
	v10 = 128;
LOC_19CCC:
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	v3 = GetObjectScreenZ(intvAttackerMajor);
	PlaySound1("m018snd01", 255);
	switch (a0) { 
		case 0: goto LOC_19D68;
		case 1: goto LOC_19DBC;
	}
	goto LOC_19E10;
LOC_19D68:
	asynccall sc3602(a0, v1, v2, v3);
	v11 = 50;
	SetGlobal(21, 110);
	goto LOC_19E78;
LOC_19DBC:
	asynccall sc3602(a0, v1, v2, v3);
	v11 = 100;
	SetGlobal(21, 220);
	goto LOC_19E78;
LOC_19E10:
	asynccall sc3601(a0, v10, v1, v2, v3);
	v11 = (100 + 95);
	SetGlobal(21, 330);
LOC_19E78:
	v4 = 0;
LOC_19E88:
	if (!(v4 < v11)) goto LOC_1A294;
	v12 = Rand(1, 2);
	v5 = 0;
LOC_19ED8:
	if (!(v5 < v12)) goto LOC_1A08C;
	v7 = (v1 + Rand((-48), 48));
	v8 = (v2 + Rand((-16), 16));
	if (!(a0 == 2)) goto LOC_1A030;
	CreateObjectRaw((v7 - 48), v8, v9, 0, Rand(28006, 28007));
	CreateObjectRaw((v7 + 48), v8, v9, 0, Rand(28006, 28007));
	goto LOC_1A07C;
LOC_1A030:
	CreateObjectRaw(v7, v8, v9, 0, Rand(28006, 28007));
LOC_1A07C:
	v5++;
	goto LOC_19ED8;
LOC_1A08C:
	if (!(Rand(0, 3) == 0)) goto LOC_1A1D4;
	v7 = (v1 + Rand((-48), 48));
	v8 = (v2 + Rand((-16), 16));
	v13 = CreateObjectRaw(v7, v8, v9, Rand(0, 255), Rand(28008, 28009));
	SetObjectSpeed_Cylind(v13, Rand(10, 16), Rand(10, 16));
LOC_1A1D4:
	Delay(1);
	switch (v4) { 
		case 49: goto LOC_1A218;
		case 99: goto LOC_1A23C;
		case 149: goto LOC_1A260;
	}
	goto LOC_1A284;
LOC_1A218:
	PlaySound1("m018snd02", 255);
	goto LOC_1A284;
LOC_1A23C:
	PlaySound1("m018snd01", 255);
	goto LOC_1A284;
LOC_1A260:
	PlaySound1("m018snd02", 255);
	goto LOC_1A284;
LOC_1A284:
	v4++;
	goto LOC_19E88;
LOC_1A294:
	if (!(a0 > 1)) goto LOC_1A2C4;
	Wait("sc3601");
	goto LOC_1A2D0;
LOC_1A2C4:
	Wait("sc3602");
LOC_1A2D0:
	v13 = GetMostImportantSoldier((intvIsLeft ^ 1));
	if (!(v13 == 0)) goto LOC_1A324;
	v13 = intvDefenderMajor;
LOC_1A324:
	v1 = GetObjectScreenX(v13);
	v2 = GetObjectScreenY(v13);
	v3 = GetObjectScreenZ(v13);
	if (!(v2 > 240)) goto LOC_1A3D4;
	asynccall MoveCamera(v1, (v2 - 240), 20);
	goto LOC_1A400;
LOC_1A3D4:
	asynccall MoveCamera(v1, 0, 20);
LOC_1A400:
	Wait("MoveCamera");
	if (!(IsObjectExist(v13) == 0)) goto LOC_1A488;
	v13 = GetMostImportantSoldier((intvIsLeft ^ 1));
	if (!(v13 == 0)) goto LOC_1A488;
	v13 = intvDefenderMajor;
LOC_1A488:
	v1 = GetObjectScreenX(v13);
	v2 = GetObjectScreenY(v13);
	v3 = GetObjectScreenZ(v13);
	asynccall sc3603(a0, v1, v2, v3);
	Wait("sc3603");
	Delay(65);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void RunningBowCallback(int a0, int a1) callsign 19001 {
	int v1;
	SetObjectFadeOut(a0, 2, 1);
	if (!(a1 == intvDefenderMajor)) goto LOC_1A698;
	SetGlobal(23, GetGlobal(23) + 1);
	if (!(GetGlobal(23) == 0)) goto LOC_1A638;
	v1 = (intvMagicAttackValue - (5 + ((GetGlobal(24) * 2) * 2)));
	HitGeneral(a0, a1, 11002, 1, 48, 0);
	goto LOC_1A670;
LOC_1A638:
	v1 = 2;
	SmallFireBall2(a0, a1, 12, 48);
LOC_1A670:
	Hurt(a0, a1, v1);
	goto LOC_1A70C;
LOC_1A698:
	Hurt(a0, a1, 0);
	asynccall FireMan(a1, 10015, 60);
	SmallFireBall2(a0, a1, 15, 35);
LOC_1A70C:
	return;
}

void CreateBowLight(int a0, int a1, int a2, int a3) {
	int v1;
	v1 = CreateObjectByReference(a0, a1, a2, 0);
	SetObjectSpeed_Sphere(v1, a2, 0, a3);
	SetObjectScale(v1, 24576, 16384);
LOC_1A7A0:
	if (!IsObjectExist(a0)) goto LOC_1A7D0;
	Delay(1);
	goto LOC_1A7A0;
LOC_1A7D0:
	SetObjectFadeOut(v1, 4, 1);
	return;
}

int CreateBow(int a0, int a1) {
	int v1;
LOC_1A7FC:
	v1 = CreateObjectByReference(a0, 29001, a1, 0);
	SetObjectFlags(v1, 262144);
	SetObjectOpacity(v1, 0);
	SetObjectFlags(v1, (16777216 + 33554432));
	SetCallbackProcedure(v1, 19001);
	SetCoordinateByReference_Cylind(v1, a0, a1, 80, 80);
	return v1;
}

void TraceBow(int a0, int a1) {
	int v1;
	ClearObjectFlags(a0, 262144);
	SetObjectSpeed_Sphere(a0, a1, 0, 16);
	asynccall CreateBowLight(a0, 29002, a1, 16);
LOC_1A974:
	if (!IsObjectExist(a0)) goto LOC_1AA14;
	v1 = GetObjectScreenX(a0);
	if (!((v1 <= 0) || (v1 >= GetBattleWidthInScreenX()))) goto LOC_1AA00;
	FreeObjectByHandle(a0);
	goto LOC_1AA14;
LOC_1AA00:
	Delay(1);
	goto LOC_1A974;
LOC_1AA14:
	return;
}

void RunningBow(int a0) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	int v7[9];
	int v16;
LOC_1AA1C:
	SetGlobal(23, 0);
	SetGlobal(24, a0);
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\019\\*");
	DisablePlayMagic();
	DownBrightness(12, 5);
	if (!(Rand(0, 3) == 3)) goto LOC_1AAEC;
	v5 = Rand(0, 90);
	goto LOC_1AAFC;
LOC_1AAEC:
	v5 = 0;
LOC_1AAFC:
	if (!(intvIsLeft == 1)) goto LOC_1AB44;
	v4 = 0;
	v3 = (-1);
	goto LOC_1AB64;
LOC_1AB44:
	v4 = 128;
	v3 = 1;
LOC_1AB64:
	switch (a0) { 
		case 0: goto LOC_1AB90;
		case 1: goto LOC_1ABB8;
	}
	goto LOC_1ABE0;
LOC_1AB90:
	v16 = 5;
	SetGlobal(23, 5);
	goto LOC_1AC00;
LOC_1ABB8:
	v16 = 7;
	SetGlobal(23, 7);
	goto LOC_1AC00;
LOC_1ABE0:
	v16 = 9;
	SetGlobal(23, 9);
LOC_1AC00:
	SetOverwhelming(intvAttackerMajor, 1);
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera((v1 - (v3 * 180)), ((v2 - 140) + v5));
	Delay(15);
	SetObjectAnimate(intvAttackerMajor, 524288);
	Delay(15);
	PlaySound(intvAttackerMajor, "m019snd01", 255);
	SetOverwhelming(intvAttackerMajor, 0);
	v6 = 0;
LOC_1AD24:
	if (!(v6 < v16)) goto LOC_1AD78;
	v7[v6] = CreateBow(intvAttackerMajor, v4);
	v6++;
	goto LOC_1AD24;
LOC_1AD78:
	v6 = 0;
LOC_1AD88:
	if (!(v6 < v16)) goto LOC_1AE30;
	asynccall TraceBow(v7[v6], v4);
	if (!(v6 < 4)) goto LOC_1AE14;
	Delay((3 + v6));
	goto LOC_1AE20;
LOC_1AE14:
	Delay(5);
LOC_1AE20:
	v6++;
	goto LOC_1AD88;
LOC_1AE30:
	asynccall LockCamera2(v7[0], 180, ((-160) + v5), intvIsLeft, v7[(v16 - 1)]);
	Wait("TraceBow");
	Delay(60);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void BlackHoleCallback(int a0, int a1) {
	int v1;
	int v2;
	v1 = GetObjectScreenY(a0);
	ApproachObjectTowards(a1, a0, Rand(16, 25));
LOC_1AF38:
	if (!IsObjectExist(a0)) goto LOC_1AFE0;
	Delay(1);
	MoveObject(a1, 0, 0, 2);
	v2 = GetObjectScreenY(a1);
	if (!(v2 >= (v1 - 10))) goto LOC_1AFD8;
	goto LOC_1AFE0;
LOC_1AFD8:
	goto LOC_1AF38;
LOC_1AFE0:
	FreeObjectByHandle(a1);
	return;
}

void KillMan(int a0, int a1) {
	int v2;
	SetObjectContext(a0, 0, 123);
	v2 = 0;
LOC_1B038:
	if (!(v2 < 20)) goto LOC_1B0A8;
	ApproachObjectTowards(a0, a1, Rand(4, 5));
	Delay(1);
	v2++;
	goto LOC_1B038;
LOC_1B0A8:
	v2 = 0;
LOC_1B0B8:
	if (!(v2 < 30)) goto LOC_1B128;
	ApproachObjectTowards(a0, a1, Rand(2, 3));
	Delay(1);
	v2++;
	goto LOC_1B0B8;
LOC_1B128:
	KillSoldier(a0);
	asynccall BlackHoleCallback(a1, a0);
	return;
}

void Engulf(int a0, int a1, int a2, int a3) {
	int v1[231];
	int v232;
	int v233;
	int v234;
	int v236;
	int v237;
	int v239;
	int v240;
	int v241;
	int v242;
	int v243;
	int v244;
	int v245;
	v244 = 0;
	v236 = ScreenXToBattleX(a1);
	v237 = ScreenXToBattleX(a2);
	if (!(a3 == 0)) goto LOC_1B218;
	v241 = 3;
	v242 = 9;
	v243 = 8;
	goto LOC_1B248;
LOC_1B218:
	v241 = 4;
	v242 = 16;
	v243 = 10;
LOC_1B248:
	v234 = 0;
LOC_1B258:
	if (!(v234 < v243)) goto LOC_1B5B8;
	v232 = GetSoldierCountInRect((intvIsLeft ^ 1), v236, v237, v241, v242);
	if (!(v232 > 0)) goto LOC_1B5A8;
	v233 = 0;
LOC_1B2E8:
	if (!(v233 < v232)) goto LOC_1B36C;
	v1[v233] = GetNthSoldierInRect((intvIsLeft ^ 1), v236, v237, v241, v242, v233);
	v233++;
	goto LOC_1B2E8;
LOC_1B36C:
	v233 = 0;
LOC_1B37C:
	if (!(v233 < v232)) goto LOC_1B428;
	v239 = Rand(0, (v232 - 1));
	v240 = v1[v233];
	v1[v233] = v1[v239];
	v1[v239] = v240;
	v233++;
	goto LOC_1B37C;
LOC_1B428:
	if (!(a3 == 0)) goto LOC_1B470;
	v232 = Rand(4, 7);
	goto LOC_1B494;
LOC_1B470:
	v232 = Rand(6, 9);
LOC_1B494:
	v233 = 0;
LOC_1B4A4:
	if (!(v233 < v232)) goto LOC_1B5A8;
	if (!((GetObjectContext(v1[v233], 0) != 123) && (v1[v233] != intvDefenderMajor))) goto LOC_1B54C;
	asynccall KillMan(v1[v233], a0);
LOC_1B54C:
	v245 = Rand(3, 4);
	Delay(v245);
	v244 = (v244 + v245);
	v233++;
	goto LOC_1B4A4;
LOC_1B5A8:
	v234++;
	goto LOC_1B258;
LOC_1B5B8:
	if (!((a3 == 0) && (v244 < 160))) goto LOC_1B60C;
	Delay((160 - v244));
	goto LOC_1B658;
LOC_1B60C:
	if (!((a3 != 0) && (v244 < 350))) goto LOC_1B658;
	Delay((350 - v244));
LOC_1B658:
	return;
}

void CreateBlackHoleBall(int a0) {
	int v1;
LOC_1B668:
	if (!GetGlobal(25)) goto LOC_1B7A4;
	v1 = CreateObjectByReference(a0, 30013, 0, 0);
	SetCoordinateByReference_Cylind(v1, a0, Rand(152, 232), Rand(200, 300), Rand((-80), 80));
	ApproachObjectTowards(v1, a0, Rand(7, 12));
	SetObjectFadeOut(v1, 16, 1);
	Delay(Rand(10, 18));
	goto LOC_1B668;
LOC_1B7A4:
	return;
}

void CreateBlackHoleStar(int a0, int a1, int a2, int a3, int a4) {
	int v1;
	v1 = CreateObjectByReference(a0, a1, 0, 0);
	SetObjectScale(v1, 131072, 131072);
	SetCoordinateByReference_Sphere(v1, a0, 0, a3, a2, 0);
	ApproachObjectTowards(v1, a0, a4);
	SetObjectFadeOut(v1, (a2 / a4), 1);
LOC_1BA20:
	if (!IsObjectExist(v1)) goto LOC_1BAA8;
	CreateShadowObject(v1, 4, 1);
	Delay(1);
	if (!(GetGlobal(25) == 0)) goto LOC_1BAA0;
	FreeObjectByHandle(v1);
LOC_1BAA0:
	goto LOC_1BA20;
LOC_1BAA8:
	return;
}

void CreateBlackHoleStars(int a0) {
	asynccall CreateBlackHoleBall(a0);
LOC_1B7D8:
	if (!GetGlobal(25)) goto LOC_1B928;
	asynccall CreateBlackHoleStar(a0, Rand(30003, 30011), Rand(30, 130), Rand(0, 127), Rand(3, 8));
	asynccall CreateBlackHoleStar(a0, Rand(30003, 30011), Rand(30, 130), Rand(128, 255), Rand(3, 8));
	Delay(Rand(0, 4));
	goto LOC_1B7D8;
LOC_1B928:
	return;
}

void CreateBlackHole(int a0, int a1, int a2) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	int v7;
	int v9;
	int v10;
	int v11;
LOC_1BAB0:
	v2 = GetObjectScreenX(a0);
	v4 = GetObjectScreenY(a0);
	v3 = GetObjectScreenX(a1);
	v5 = GetObjectScreenY(a1);
	v11 = GetSoldierMaxX2(intvIsLeft);
	if (!(v11 == (-1))) goto LOC_1BB90;
	v2 = ((v2 + v3) / 2);
	goto LOC_1BBEC;
LOC_1BB90:
	if (!(intvIsLeft == 1)) goto LOC_1BBD0;
	v2 = (v11 + 200);
	goto LOC_1BBEC;
LOC_1BBD0:
	v2 = (v11 - 200);
LOC_1BBEC:
	v4 = ((v4 + v5) / 2);
	MoveCamera(v2, v4, 10);
	if (!(a2 == 0)) goto LOC_1BCA0;
	v9 = CreateObjectRaw(v2, (v4 + 700), 100, 128, 30012);
	goto LOC_1BCE8;
LOC_1BCA0:
	v9 = CreateObjectRaw(v2, (v4 + 700), 100, 128, 30014);
LOC_1BCE8:
	v6 = 0;
LOC_1BCF8:
	if (!(v6 < 106496)) goto LOC_1BEEC;
	if (!(v6 < 24576)) goto LOC_1BD44;
	Delay(6);
	goto LOC_1BD50;
LOC_1BD44:
	Delay(1);
LOC_1BD50:
	if (!(v6 == 24576)) goto LOC_1BE20;
	SetObjectFadeOut(v9, 16, 2);
	v7 = CreateObjectRaw(v2, (v4 + 716), 100, 128, 30002);
	v1 = CreateObjectRaw(v2, (v4 + 700), 100, 128, 30001);
LOC_1BE20:
	if (!(v6 >= 24576)) goto LOC_1BE8C;
	SetObjectScale(v7, v6, v6);
	SetObjectScale(v1, v6, v6);
	goto LOC_1BEC8;
LOC_1BE8C:
	SetObjectScale(v9, (v6 * 12), (v6 * 12));
LOC_1BEC8:
	v6 = (v6 + 4096);
	goto LOC_1BCF8;
LOC_1BEEC:
	SetGlobal(25, 1);
	if (!(a2 > 0)) goto LOC_1BF84;
	v10 = CreateObjectRaw(v2, (v4 + 700), 100, 128, 30015);
	SetObjectScale(v10, 94208, 94208);
LOC_1BF84:
	asynccall CreateBlackHoleStars(v7);
	asynccall Engulf(v1, v2, v4, a2);
	Wait("Engulf");
	Delay(150);
	if (!(a2 > 0)) goto LOC_1C024;
	FreeObjectByHandle(v10);
LOC_1C024:
	SetGlobal(25, 0);
	v6 = 131072;
LOC_1C044:
	if (!(v6 >= 0)) goto LOC_1C0D8;
	SetObjectScale(v1, v6, v6);
	SetObjectScale(v7, v6, v6);
	Delay(1);
	v6 = (v6 - 10240);
	goto LOC_1C044;
LOC_1C0D8:
	SetObjectFadeOut(v1, 2, 1);
	SetObjectFadeOut(v7, 2, 1);
	return;
}

void BlackHole(int a0) {
	int v1;
	int v2;
LOC_1C128:
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\020\\*");
	DisablePlayMagic();
	DownBrightness(8, 5);
	SetOverwhelming(intvAttackerMajor, 1);
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v1, (v2 - 140));
	AddAttackCounter(intvAttackerMajor, 2);
	Delay(15);
	SetObjectAnimate(intvAttackerMajor, 65536);
	Delay(40);
	SetOverwhelming(intvAttackerMajor, 0);
	if (!(a0 == 0)) goto LOC_1C2A4;
	PlaySound1("m020snd01", 180);
	goto LOC_1C2C0;
LOC_1C2A4:
	PlaySound1("m020snd02", 210);
LOC_1C2C0:
	CreateBlackHole(intvAttackerMajor, intvDefenderMajor, a0);
	Delay(40);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void FireRingCallback(int a0, int a1) callsign 20001 {
	int v1;
	int v2;
	if (!(a1 == intvDefenderMajor)) goto LOC_1C450;
	v2 = GetObjectContext(a0, 0);
	if (!GetGlobal(26 + v2)) goto LOC_1C39C;
	v1 = (GetGlobal(30) / GetGlobal(26 + v2));
LOC_1C39C:
	Hurt(a0, a1, v1);
	if (!(v1 > 0)) goto LOC_1C448;
	SetGlobal(26 + v2, (GetGlobal(26 + v2) * 2));
	asynccall HitGeneral(a0, a1, 11002, 1, 48, 60);
LOC_1C448:
	goto LOC_1C49C;
LOC_1C450:
	Hurt(a0, a1, 0);
	asynccall FireMan(a1, 10015, 60);
LOC_1C49C:
	return;
}

void CreateFire(int a0) {
	int v1;
	v1 = CreateObjectByReference(a0, 31001, 0, 0);
	SetObjectScale(v1, 65536, 81920);
	asynccall StepShow(v1, 8);
LOC_1C530:
	if (!IsObjectExist(a0)) goto LOC_1C594;
	Delay(1);
	SetCoordinateByReference_Cylind(v1, a0, 192, 8, 0);
	goto LOC_1C530;
LOC_1C594:
	SetObjectFadeOut(v1, 8, 1);
	return;
}

void FireRingMotion2(int a0, int a1, int a2, int a3, int a4) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	v1 = 0;
	v2 = 0;
	v3 = 1;
	v4 = a3;
	v5 = 5;
LOC_1C618:
	if (!IsObjectExist(a0)) goto LOC_1C7DC;
	SetCoordinateByReference_Cylind(a0, a1, a2, v4, a4);
	Delay(1);
	if (!(a4 >= 80)) goto LOC_1C6AC;
	v3 = (-1);
	goto LOC_1C6D8;
LOC_1C6AC:
	if (!(a4 <= 50)) goto LOC_1C6D8;
	v3 = 1;
LOC_1C6D8:
	a4 = (a4 + v3);
	a2 = ((a2 + 2) & 255);
	v2++;
	if (!((v2 & 3) == 0)) goto LOC_1C754;
	v1++;
LOC_1C754:
	if (!(v4 >= 240)) goto LOC_1C78C;
	v5 = (-5);
	goto LOC_1C7B8;
LOC_1C78C:
	if (!(v4 <= a3)) goto LOC_1C7B8;
	v5 = 5;
LOC_1C7B8:
	v4 = (v4 + v5);
	goto LOC_1C618;
LOC_1C7DC:
	return;
}

void FireRingMotion(int a0, int a1, int a2, int a3, int a4, int a5) {
	int v1;
	int v2;
	int v3;
	v1 = 0;
	v2 = 0;
	v3 = 1;
LOC_1C81C:
	if (!IsObjectExist(a0)) goto LOC_1C960;
	SetCoordinateByReference_Cylind(a0, a1, a2, a3, a4);
	Delay(1);
	if (!(a4 >= 80)) goto LOC_1C8B0;
	v3 = (-1);
	goto LOC_1C8DC;
LOC_1C8B0:
	if (!(a4 <= 50)) goto LOC_1C8DC;
	v3 = 1;
LOC_1C8DC:
	a4 = (a4 + v3);
	a2 = ((a2 + a5) & 255);
	v2++;
	if (!((v2 & 3) == 0)) goto LOC_1C958;
	v1++;
LOC_1C958:
	goto LOC_1C81C;
LOC_1C960:
	return;
}

void CreateFireRing(int a0, int a1, int a2, int a3, int a4, int a5, int a6) {
	int v1;
	int v2;
	v2 = 0;
	a1 = (a1 & 255);
	v1 = CreateObjectByReference(a0, 31002, 0, 0);
	SetCoordinateByReference_Cylind(v1, a0, a1, a2, a3);
	SetCallbackProcedure(v1, 20001);
	SetObjectFlags(v1, (33554432 + 16777216));
	if (!(intvIsLeft == 1)) goto LOC_1CA90;
	SetObjectContext(v1, 0, 0);
	goto LOC_1CAB4;
LOC_1CA90:
	SetObjectContext(v1, 0, 1);
LOC_1CAB4:
	asynccall StepShow(v1, 8);
	asynccall CreateFire(v1);
	if (!(a5 == 0)) goto LOC_1CC48;
	asynccall FireRingMotion(v1, a0, a1, a2, a3, a4);
LOC_1CB6C:
	if (!IsObjectExist(a0)) goto LOC_1CBC8;
	Delay(1);
	v2++;
	if (!(v2 > 400)) goto LOC_1CBC0;
	goto LOC_1CBC8;
LOC_1CBC0:
	goto LOC_1CB6C;
LOC_1CBC8:
	SetObjectFadeOut(v1, 8, 1);
	if (!(a6 == 1)) goto LOC_1CC28;
	SetGlobal(28 + 0, 0);
	goto LOC_1CC40;
LOC_1CC28:
	SetGlobal(28 + 1, 0);
LOC_1CC40:
	goto LOC_1CD58;
LOC_1CC48:
	asynccall FireRingMotion2(v1, a0, a1, a2, a3);
LOC_1CC84:
	if (!IsObjectExist(a0)) goto LOC_1CCE0;
	Delay(1);
	v2++;
	if (!(v2 > 400)) goto LOC_1CCD8;
	goto LOC_1CCE0;
LOC_1CCD8:
	goto LOC_1CC84;
LOC_1CCE0:
	SetObjectFadeOut(v1, 8, 1);
	if (!(a6 == 1)) goto LOC_1CD40;
	SetGlobal(28 + 0, 0);
	goto LOC_1CD58;
LOC_1CD40:
	SetGlobal(28 + 1, 0);
LOC_1CD58:
	return;
}

void FireRing(int a0, int a1) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
LOC_1CD60:
	v6 = 0;
	if (!(intvIsLeft == 1)) goto LOC_1CE08;
	if (!(GetGlobal(28 + 0) == 1)) goto LOC_1CDD0;
	v6 = 1;
	goto LOC_1CE00;
LOC_1CDD0:
	SetGlobal(28 + 0, 1);
	SetGlobal(26 + 0, 1);
LOC_1CE00:
	goto LOC_1CE90;
LOC_1CE08:
	if (!(intvIsLeft == 0)) goto LOC_1CE90;
	if (!(GetGlobal(28 + 1) == 1)) goto LOC_1CE60;
	v6 = 1;
	goto LOC_1CE90;
LOC_1CE60:
	SetGlobal(28 + 1, 1);
	SetGlobal(26 + 1, 1);
LOC_1CE90:
	SetGlobal(30, intvMagicAttackValue);
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\021\\*");
	DisablePlayMagic();
	DownBrightness(12, 5);
	SetOverwhelming(intvAttackerMajor, 1);
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v1, (v2 - 120));
	AddAttackCounter(intvAttackerMajor, 2);
	Delay(15);
	if (!(v6 == 0)) goto LOC_1D28C;
	SetObjectAnimate(intvAttackerMajor, 32768);
	PlaySound1("m021snd01", 200);
	Delay(20);
	SetOverwhelming(intvAttackerMajor, 0);
	v5 = 4;
	v4 = (((80 - 50) - 10) / v5);
	v3 = 0;
LOC_1D060:
	if (!(v3 < v5)) goto LOC_1D12C;
	asynccall CreateFireRing(intvAttackerMajor, ((v3 * 256) / v5), 60, ((50 + 5) + (v3 * v4)), (4 + (a0 * 2)), a1, intvIsLeft);
	v3++;
	goto LOC_1D060;
LOC_1D12C:
	if (!(a0 > 0)) goto LOC_1D284;
	if (!(a1 > 0)) goto LOC_1D170;
	Delay(20);
LOC_1D170:
	v5 = 6;
	v4 = (((80 - 50) - 10) / v5);
	v3 = 0;
LOC_1D1C4:
	if (!(v3 < v5)) goto LOC_1D284;
	asynccall CreateFireRing(intvAttackerMajor, (32 + ((v3 * 256) / v5)), 120, ((50 + 5) + (v3 * v4)), 2, a1, intvIsLeft);
	v3++;
	goto LOC_1D1C4;
LOC_1D284:
	goto LOC_1D2E0;
LOC_1D28C:
	if (!(intvIsLeft == 1)) goto LOC_1D2C8;
	CastFail(intvAttackerMajor, 0);
	goto LOC_1D2E0;
LOC_1D2C8:
	CastFail(intvAttackerMajor, 128);
LOC_1D2E0:
	Delay(60);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void IceCallback(int a0, int a1) callsign 22001 {
	if (!(a1 == intvDefenderMajor)) goto LOC_1D3C4;
	if (!(GetGlobal(31) < 4)) goto LOC_1D3A0;
	Hurt(a0, a1, (intvMagicAttackValue / GetGlobal(31)));
	SetGlobal(31, (GetGlobal(31) * 2));
	goto LOC_1D3BC;
LOC_1D3A0:
	ClearObjectFlags(a0, 16777216);
LOC_1D3BC:
	goto LOC_1D3E4;
LOC_1D3C4:
	Hurt(a0, a1, 0);
LOC_1D3E4:
	return;
}

void CreateIceShatter(int a0, int a1, int a2, int a3) {
	int v1;
	Delay(Rand(0, 3));
	v1 = CreateObjectByReference(a0, Rand(32003, 32005), 0, 0);
	SetObjectScale(v1, Rand(12288, 20480), Rand(12288, 20480));
	SetCoordinateByReference_Cylind(v1, a0, Rand(128, 255), 30, a1);
	SetObjectSpeed_Sphere(v1, 0, a2, a3);
	Delay((a1 / 20));
	SetObjectFadeOut(v1, 8, 1);
	return;
}

void CreateFogSpeed(int a0, int a1, int a2, int a3, int a4) {
	int v1;
	Delay(Rand(1, 6));
	v1 = CreateObjectRaw(a0, a1, 0, 0, 32001);
	SetObjectScale(v1, 32768, 32768);
	SetObjectSpeed_Sphere(v1, 0, a2, a3);
	SetObjectScaleShrink(v1, (-1024));
	Delay(a4);
	SetObjectFadeOut(v1, 16, 1);
	return;
}

void CreateIceFog(int a0, int a1, int a2) {
	asynccall CreateFogSpeed((a1 + Rand((-2), 2)), (a2 + Rand(4, 12)), Rand(48, 60), Rand(3, 5), Rand(12, 14));
	asynccall CreateFogSpeed(((a1 + 32) + Rand((-2), 2)), (a2 + Rand(4, 12)), Rand(48, 64), Rand(3, 5), Rand(12, 14));
	asynccall CreateFogSpeed(((a1 - 32) + Rand((-2), 2)), (a2 + Rand(4, 12)), Rand(48, 64), Rand(3, 5), Rand(12, 14));
	Delay(16);
	asynccall CreateFogSpeed((a1 + Rand((-4), 4)), (a2 + Rand(4, 12)), Rand(48, 64), Rand(3, 4), Rand(12, 17));
	asynccall CreateFogSpeed(((a1 + 30) + Rand((-6), 6)), (a2 + Rand(4, 12)), Rand(48, 64), Rand(3, 4), Rand(12, 17));
	asynccall CreateFogSpeed(((a1 - 30) + Rand((-6), 6)), (a2 + Rand(4, 12)), Rand(48, 64), Rand(3, 4), Rand(12, 17));
	Delay(18);
	asynccall CreateFogSpeed((a1 + Rand((-8), 8)), (a2 + Rand(4, 12)), Rand(48, 64), Rand(2, 4), Rand(12, 20));
	asynccall CreateFogSpeed(((a1 + 28) + Rand((-8), 8)), (a2 + Rand(4, 12)), Rand(48, 64), Rand(2, 4), Rand(12, 20));
	asynccall CreateFogSpeed(((a1 - 28) + Rand((-8), 8)), (a2 + Rand(4, 12)), Rand(48, 64), Rand(2, 4), Rand(12, 20));
	return;
}

void CreateIce(int a0, int a1, int a2, int a3, int a4, int a5) {
	int v1;
	int v2;
	Delay(a4);
	v1 = CreateObjectRaw(a0, a1, 0, 128, 32002);
	SetObjectScale(v1, a2, a3);
	if (!a5) goto LOC_1DD2C;
	SetObjectFlags(v1, (33554432 + 16777216));
	SetCallbackProcedure(v1, 22001);
LOC_1DD2C:
	asynccall CreateFogSpeed((a0 + Rand((-2), 2)), (a1 + Rand(4, 12)), Rand(48, 60), Rand(2, 4), Rand(12, 20));
	asynccall CreateFogSpeed(((a0 + 32) + Rand((-2), 2)), (a1 + Rand(4, 12)), Rand(48, 64), Rand(2, 4), Rand(12, 20));
	asynccall CreateFogSpeed(((a0 - 32) + Rand((-2), 2)), (a1 + Rand(4, 12)), Rand(48, 64), Rand(2, 4), Rand(12, 20));
	v2 = 0;
LOC_1DF88:
	if (!(v2 < 8)) goto LOC_1E070;
	asynccall CreateIceShatter(v1, (((a3 * (v2 + 1)) / 65536) * 20), Rand(26, 36), Rand(2, 5));
	Delay(1);
	MoveObject(v1, 2, 0, 0);
	v2++;
	goto LOC_1DF88;
LOC_1E070:
	asynccall LockTargetXY((a0 + 54), a1, 2501, 0, 4, 22001, 0, 0, 33554432);
	asynccall LockTargetXY((a0 - 54), a1, 2501, 0, 4, 22001, 0, 0, 33554432);
	asynccall LockTargetXY(a0, (a1 + 45), 2501, 0, 4, 22001, 0, 0, 33554432);
	asynccall LockTargetXY(a0, (a1 + 45), 2501, 0, 4, 22001, 0, 0, 33554432);
	asynccall LockTargetXY((a0 + 54), (a1 + 45), 2501, 0, 4, 22001, 0, 0, 33554432);
	asynccall LockTargetXY((a0 + 54), (a1 - 45), 2501, 0, 4, 22001, 0, 0, 33554432);
	asynccall LockTargetXY((a0 - 54), (a1 + 45), 2501, 0, 4, 22001, 0, 0, 33554432);
	asynccall LockTargetXY((a0 - 54), (a1 - 45), 2501, 0, 4, 22001, 0, 0, 33554432);
	Delay(35);
	SetObjectFadeOut(v1, 16, 1);
	return;
}

void Ice(int a0) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	int v7;
	int v8;
	int v9;
LOC_1E524:
	SetGlobal(31, 1);
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\022\\*");
	DisablePlayMagic();
	DownBrightness(12, 5);
	SetOverwhelming(intvAttackerMajor, 1);
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v1, (v2 - 120));
	AddAttackCounter(intvAttackerMajor, 2);
	Delay(15);
	SetObjectAnimate(intvAttackerMajor, 32768);
	Delay(20);
	SetOverwhelming(intvAttackerMajor, 0);
	v4 = 0;
LOC_1E680:
	if (!(v4 < (a0 + 1))) goto LOC_1F198;
	if (!(v4 == a0)) goto LOC_1E6F4;
	v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
	goto LOC_1E71C;
LOC_1E6F4:
	v3 = GetRandomSoldierFromAlive((intvIsLeft ^ 1));
LOC_1E71C:
	if (!(v3 == 0)) goto LOC_1E748;
	v3 = intvDefenderMajor;
LOC_1E748:
	v1 = GetObjectScreenX(v3);
	v2 = GetObjectScreenY(v3);
	MoveCamera(v1, (v2 - 140), 20);
	PlaySound1("m022snd01", 180);
	asynccall CreateIce(v1, v2, 65536, 65536, 0, 1);
	asynccall CreateIce((v1 + 44), (v2 + 12), 49152, 45056, Rand(3, 4), 0);
	asynccall CreateIce((v1 - 44), (v2 + 10), 53248, 49152, Rand(4, 5), 0);
	asynccall CreateIce((v1 + Rand((-4), 4)), (v2 - 16), 49152, 36864, Rand(6, 7), 0);
	Delay(Rand(4, 12));
	v5 = (v1 + Rand(180, 260));
	if (!Rand(0, 1)) goto LOC_1EA20;
	v6 = (v2 + Rand(180, 240));
	goto LOC_1EA50;
LOC_1EA20:
	v6 = (v2 - Rand(180, 230));
LOC_1EA50:
	v8 = ScreenXToBattleX(v5);
	v9 = ScreenXToBattleX(v6);
	v7 = GetSoldierCountInRect((intvIsLeft ^ 1), v8, v9, 5, 4);
	if (!(v7 > 0)) goto LOC_1EBB0;
	v3 = GetNthSoldierInRect((intvIsLeft ^ 1), v8, v9, 5, 4, Rand(0, (v7 - 1)));
	if (!(v3 != intvDefenderMajor)) goto LOC_1EBB0;
	v5 = GetObjectScreenX(v3);
	v6 = GetObjectScreenY(v3);
LOC_1EBB0:
	PlaySound1("m022snd01", 200);
	asynccall CreateIce(v5, v6, 65536, 65536, 0, 1);
	asynccall CreateIce((v5 + 44), (v6 + 12), 49152, 45056, Rand(3, 4), 0);
	asynccall CreateIce((v5 - 44), (v6 + 10), 53248, 49152, Rand(4, 5), 0);
	asynccall CreateIce((v5 + Rand((-4), 4)), (v6 - 16), 49152, 36864, Rand(6, 7), 0);
	Delay(Rand(4, 12));
	v5 = (v1 - Rand(170, 260));
	if (!Rand(0, 1)) goto LOC_1EE24;
	v6 = (v2 + Rand(190, 230));
	goto LOC_1EE54;
LOC_1EE24:
	v6 = (v2 - Rand(190, 240));
LOC_1EE54:
	v8 = ScreenXToBattleX(v5);
	v9 = ScreenXToBattleX(v6);
	v7 = GetSoldierCountInRect((intvIsLeft ^ 1), v8, v9, 5, 4);
	if (!(v7 > 0)) goto LOC_1EFB4;
	v3 = GetNthSoldierInRect((intvIsLeft ^ 1), v8, v9, 5, 4, Rand(0, (v7 - 1)));
	if (!(v3 != intvDefenderMajor)) goto LOC_1EFB4;
	v5 = GetObjectScreenX(v3);
	v6 = GetObjectScreenY(v3);
LOC_1EFB4:
	PlaySound1("m022snd01", 220);
	asynccall CreateIce(v5, v6, 65536, 65536, 0, 1);
	asynccall CreateIce((v5 + 44), (v6 + 12), 49152, 45056, Rand(3, 4), 0);
	asynccall CreateIce((v5 - 44), (v6 + 10), 53248, 49152, Rand(4, 5), 0);
	asynccall CreateIce((v5 + Rand((-4), 4)), (v6 - 16), 49152, 36864, Rand(6, 7), 0);
	Delay(30);
	v4++;
	goto LOC_1E680;
LOC_1F198:
	Wait("CreateIce");
	Delay(60);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void OnFireCallback(int a0, int a1) callsign 23001 {
	int v1;
	int v2;
	if (!((a1 == intvDefenderMajor) || (a1 == intvAttackerMajor))) goto LOC_1F314;
	v2 = GetObjectContext(a0, 0);
	if (!GetGlobal(32 + v2)) goto LOC_1F278;
	v1 = (GetGlobal(34) / GetGlobal(32 + v2));
LOC_1F278:
	Hurt(a0, a1, v1);
	if (!(v1 > 0)) goto LOC_1F30C;
	SetGlobal(32 + v2, (GetGlobal(32 + v2) * 2));
	asynccall LockTargetTime2(a1, 10015, 60);
LOC_1F30C:
	goto LOC_1F360;
LOC_1F314:
	asynccall FireMan(a1, 10015, 60);
	Hurt(a0, a1, 0);
LOC_1F360:
	return;
}

void CreateOnFireSmoke(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7) {
	int v1;
	v1 = CreateObjectRaw(a0, a1, a2, 0, 33002);
	if (!(a7 == 0)) goto LOC_1F3F4;
	SetObjectScale(v1, 8192, 16384);
	goto LOC_1F418;
LOC_1F3F4:
	SetObjectScale(v1, 53248, 53248);
LOC_1F418:
	SetObjectSpeed_Sphere(v1, 0, a5, a4);
	Delay(2);
	SetObjectScaleShrink(v1, (-a6));
	if (!(a7 == 0)) goto LOC_1F4A0;
	Delay(12);
	goto LOC_1F4AC;
LOC_1F4A0:
	Delay(4);
LOC_1F4AC:
	SetObjectFadeOut(v1, (a3 - 12), 1);
	return;
}

void CreateOnFireFire(int a0, int a1, int a2) {
	int v1;
	int v2;
	v1 = CreateObjectRaw(a0, a1, 0, (Rand(0, 1) * 128), 33001);
	SetObjectFlags(v1, 251658240);
	SetCallbackProcedure(v1, 23001);
	if (!(intvIsLeft == 1)) goto LOC_1F5C8;
	SetObjectContext(v1, 0, 0);
	goto LOC_1F5EC;
LOC_1F5C8:
	SetObjectContext(v1, 0, 1);
LOC_1F5EC:
	v2 = 4096;
LOC_1F5FC:
	if (!(v2 <= 81920)) goto LOC_1F66C;
	SetObjectScale(v1, v2, v2);
	Delay(2);
	v2 = (v2 + 4096);
	goto LOC_1F5FC;
LOC_1F66C:
	Delay(150);
	PlaySound1("m023snd01", 128);
	Delay(250);
	SetObjectFadeOut(v1, 16, 1);
	SetObjectFadeOut(a2, 16, 1);
	return;
}

void CreateOnFire(int a0, int a1) {
	int v1;
	int v3;
	int v4;
	int v5;
	PlaySound1("m023snd01", 224);
	v1 = 0;
LOC_1F724:
	if (!(v1 < 80)) goto LOC_1F85C;
	asynccall CreateOnFireSmoke((a0 + Rand((-4), 4)), (a1 + Rand((-4), 4)), 0, Rand(64, 80), Rand(1, 3), Rand(50, 60), Rand(512, 768), 0);
	Delay(Rand(1, 4));
	v1++;
	goto LOC_1F724;
LOC_1F85C:
	PlaySound1("m023snd02", 224);
	v3 = CreateObjectRaw(0, 0, 0, 0, 2501);
	asynccall CreateOnFireFire(a0, a1, v3);
	v1 = 0;
LOC_1F8F0:
	if (!IsObjectExist(v3)) goto LOC_1FADC;
	if (!((v1 % 12) == 0)) goto LOC_1FAC0;
	v4 = Rand((-24), 24);
	if (!(v4 > 0)) goto LOC_1F9B8;
	v5 = ((((-5) * v4) / 2) + 80);
	goto LOC_1F9EC;
LOC_1F9B8:
	v5 = (((5 * v4) / 2) + 80);
LOC_1F9EC:
	asynccall CreateOnFireSmoke((a0 + v4), (a1 + Rand((-4), 4)), v5, Rand(48, 60), Rand(1, 2), Rand(54, 74), Rand(768, 1024), 1);
LOC_1FAC0:
	Delay(1);
	v1++;
	goto LOC_1F8F0;
LOC_1FADC:
	return;
}

void TraceOnFire(int a0) {
	Wait("CreateOnFire");
	if (!(a0 == 1)) goto LOC_1FB2C;
	SetGlobal(35 + 0, 0);
	goto LOC_1FB44;
LOC_1FB2C:
	SetGlobal(35 + 1, 0);
LOC_1FB44:
	return;
}

void OnFire(int a0) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	int v7;
	int v8;
	int v9;
LOC_1FB4C:
	v3 = intvAttackerMajor;
	v4 = intvDefenderMajor;
	v5 = intvIsLeft;
	v9 = 0;
	if (!(intvIsLeft == 1)) goto LOC_1FC24;
	if (!(GetGlobal(35 + 0) == 1)) goto LOC_1FBEC;
	v9 = 1;
	goto LOC_1FC1C;
LOC_1FBEC:
	SetGlobal(35 + 0, 1);
	SetGlobal(32 + 0, 1);
LOC_1FC1C:
	goto LOC_1FCAC;
LOC_1FC24:
	if (!(intvIsLeft == 0)) goto LOC_1FCAC;
	if (!(GetGlobal(35 + 1) == 1)) goto LOC_1FC7C;
	v9 = 1;
	goto LOC_1FCAC;
LOC_1FC7C:
	SetGlobal(35 + 1, 1);
	SetGlobal(32 + 1, 1);
LOC_1FCAC:
	SetGlobal(34, intvMagicAttackValue);
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\023\\*");
	switch (a0) { 
		case 0: goto LOC_1FD10;
		case 1: goto LOC_1FD28;
	}
	goto LOC_1FD40;
LOC_1FD10:
	v6 = 5;
	goto LOC_1FD50;
LOC_1FD28:
	v6 = 9;
	goto LOC_1FD50;
LOC_1FD40:
	v6 = 13;
LOC_1FD50:
	DisablePlayMagic();
	DownBrightness(12, 5);
	SetOverwhelming(v3, 1);
	v1 = GetObjectScreenX(v3);
	v2 = GetObjectScreenY(v3);
	SetViewCamera(v1, (v2 - 120));
	AddAttackCounter(v3, 2);
	Delay(15);
	if (!(v9 == 0)) goto LOC_20144;
	SetObjectAnimate(v3, 32768);
	Delay(20);
	SetOverwhelming(v3, 0);
	v8 = 0;
LOC_1FE88:
	if (!(v8 < v6)) goto LOC_20110;
	v7 = GetRandomSoldierFromAlive((v5 ^ 1));
	if (!(v7 == 0)) goto LOC_20040;
	v1 = GetObjectScreenX(v4);
	v2 = GetObjectScreenY(v4);
	if (!Rand(0, 1)) goto LOC_1FF7C;
	v1 = (v1 + Rand(40, 400));
	goto LOC_1FFAC;
LOC_1FF7C:
	v1 = (v1 - Rand(40, 400));
LOC_1FFAC:
	if (!Rand(0, 1)) goto LOC_20008;
	v2 = (v2 + Rand(30, 300));
	goto LOC_20038;
LOC_20008:
	v2 = (v2 - Rand(30, 300));
LOC_20038:
	goto LOC_20078;
LOC_20040:
	v1 = GetObjectScreenX(v7);
	v2 = GetObjectScreenY(v7);
LOC_20078:
	if (!(v8 == 0)) goto LOC_200B4;
	MoveCamera(v1, 200, 30);
LOC_200B4:
	asynccall CreateOnFire(v1, v2);
	Delay(Rand(0, 2));
	v8++;
	goto LOC_1FE88;
LOC_20110:
	asynccall TraceOnFire(intvIsLeft);
	goto LOC_20198;
LOC_20144:
	if (!(intvIsLeft == 1)) goto LOC_20180;
	CastFail(intvAttackerMajor, 0);
	goto LOC_20198;
LOC_20180:
	CastFail(intvAttackerMajor, 128);
LOC_20198:
	Delay(60);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void ProduceBackSoldier(int a0, int a1) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	int v7;
	v4 = 0;
	v1 = 0;
	v6 = GetBattleWidth();
	v7 = GetBattleHeight();
LOC_20220:
	v2 = (v7 / 2);
LOC_2023C:
	if (!(a0 == 1)) goto LOC_2029C;
	v3 = GetForceHandleByXY(((v6 - 1) - v1), v2);
	goto LOC_202C0;
LOC_2029C:
	v3 = GetForceHandleByXY(v1, v2);
LOC_202C0:
	if (!(v3 == 0)) goto LOC_203F4;
	if (!(a0 == 1)) goto LOC_20370;
	v5 = CreateSoldier(a0, ((v6 - 1) - v1), v2);
	asynccall StepShow(v5, 10);
	goto LOC_203C8;
LOC_20370:
	v5 = CreateSoldier(a0, v1, v2);
	asynccall StepShow(v5, 10);
LOC_203C8:
	v4++;
	if (!(v4 >= a1)) goto LOC_203F4;
	return;
LOC_203F4:
	if (!(intvIsLeft == 1)) goto LOC_2046C;
	v3 = GetForceHandleByXY(((v6 - 1) - v1), ((v7 - 1) - v2));
	goto LOC_204A8;
LOC_2046C:
	v3 = GetForceHandleByXY(v1, ((v7 - 1) - v2));
LOC_204A8:
	if (!(v3 == 0)) goto LOC_2060C;
	if (!(a0 == 1)) goto LOC_20570;
	v5 = CreateSoldier(a0, ((v6 - 1) - v1), ((v7 - 1) - v2));
	asynccall StepShow(v5, 10);
	goto LOC_205E0;
LOC_20570:
	v5 = CreateSoldier(a0, v1, ((v7 - 1) - v2));
	asynccall StepShow(v5, 10);
LOC_205E0:
	v4++;
	if (!(v4 >= a1)) goto LOC_2060C;
	return;
LOC_2060C:
	v2--;
	if ((v2 > 0)) goto LOC_2023C;
	v1++;
	if ((v1 <= v6)) goto LOC_20220;
	return;
}

void BackSoldier(int a0, int a1) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	int v7;
LOC_2065C:
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\024\\*");
	DisablePlayMagic();
	DownBrightness(8, 5);
	v1 = intvAttackerMajor;
	v3 = intvIsLeft;
	SetOverwhelming(intvAttackerMajor, 1);
	v4 = GetObjectScreenX(v1);
	v5 = GetObjectScreenY(v1);
	SetViewCamera(v4, (v5 - 120));
	AddAttackCounter(v1, 2);
	Delay(20);
	if (!(v3 == 1)) goto LOC_207A8;
	v2 = 0;
	goto LOC_207B8;
LOC_207A8:
	v2 = 128;
LOC_207B8:
	v6 = GetMajorLevel(v3);
	if (!(v6 > 40)) goto LOC_20800;
	v6 = 40;
LOC_20800:
	v7 = ((v6 * 5) - GetSoldierCount(v3));
	if (!(v7 < 0)) goto LOC_20860;
	v7 = 0;
LOC_20860:
	if (!(v7 > a0)) goto LOC_2088C;
	v7 = a0;
LOC_2088C:
	if (!(v7 != 0)) goto LOC_20A78;
	asynccall ProduceBackSoldier(v3, v7);
	SetObjectAnimate(v1, 32768);
	Delay(24);
	SetObjectAnimate(v1, 65536);
	SetOverwhelming(v1, 0);
	Delay(6);
	PlaySound1("m002snd01", 255);
	PlaySound1("m002snd03", 255);
	if (!(intvIsLeft == 1)) goto LOC_209C8;
	asynccall MoveCamera(6576, 0, 40);
	goto LOC_209F4;
LOC_209C8:
	asynccall MoveCamera(240, 0, 40);
LOC_209F4:
	Wait("MoveCamera");
	Delay(100);
	v4 = GetObjectScreenX(intvAttackerMajor);
	v5 = GetObjectScreenY(intvAttackerMajor);
	MoveCamera(v4, (v5 - 120), 20);
	goto LOC_20A90;
LOC_20A78:
	CastFail(v1, v2);
LOC_20A90:
	Delay(8);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void TaiChiCallback(int a0, int a1) callsign 25001 {
	if (!(a1 == intvDefenderMajor)) goto LOC_20B74;
	if (!(GetGlobal(37) < 4)) goto LOC_20B50;
	Hurt(a0, a1, (intvMagicAttackValue / GetGlobal(37)));
	SetGlobal(37, (GetGlobal(37) * 2));
	goto LOC_20B6C;
LOC_20B50:
	ClearObjectFlags(a0, 16777216);
LOC_20B6C:
	goto LOC_20B94;
LOC_20B74:
	Hurt(a0, a1, 0);
LOC_20B94:
	return;
}

void CreateTaiChiBomb(int a0, int a1) {
	int v1;
	int v2;
	int v3;
	Delay(8);
	v1 = CreateObjectByReference(a0, 35002, 0, 0);
	SetObjectScale(v1, 4096, 4096);
	if (!a1) goto LOC_20C5C;
	SetCallbackProcedure(v1, 25001);
	SetObjectFlags(v1, (33554432 + 16777216));
LOC_20C5C:
	SetObjectScaleShrink(v1, (-4096));
	Delay(20);
	v2 = GetObjectScreenX(v1);
	v3 = GetObjectScreenY(v1);
	asynccall LockTargetXY((v2 + 64), v3, 2501, 0, 20, 25001, 0, 0, 33554432);
	asynccall LockTargetXY((v2 - 64), v3, 2501, 0, 20, 25001, 0, 0, 33554432);
	asynccall LockTargetXY(v2, (v3 + 50), 2501, 0, 20, 25001, 0, 0, 33554432);
	asynccall LockTargetXY(v2, (v3 - 50), 2501, 0, 20, 25001, 0, 0, 33554432);
	asynccall LockTargetXY((v2 + 64), (v3 + 50), 2501, 0, 20, 25001, 0, 0, 33554432);
	asynccall LockTargetXY((v2 + 64), (v3 - 50), 2501, 0, 20, 25001, 0, 0, 33554432);
	asynccall LockTargetXY((v2 - 64), (v3 + 50), 2501, 0, 20, 25001, 0, 0, 33554432);
	asynccall LockTargetXY((v2 - 64), (v3 - 50), 2501, 0, 20, 25001, 0, 0, 33554432);
	asynccall LockTargetXY((v2 + 128), v3, 2501, 0, 20, 25001, 0, 0, 33554432);
	asynccall LockTargetXY((v2 - 128), v3, 2501, 0, 20, 25001, 0, 0, 33554432);
	asynccall LockTargetXY(v2, (v3 + 100), 2501, 0, 20, 25001, 0, 0, 33554432);
	asynccall LockTargetXY(v2, (v3 - 100), 2501, 0, 20, 25001, 0, 0, 33554432);
	asynccall LockTargetXY((v2 + 128), (v3 + 100), 2501, 0, 20, 25001, 0, 0, 33554432);
	asynccall LockTargetXY((v2 + 128), (v3 - 100), 2501, 0, 20, 25001, 0, 0, 33554432);
	asynccall LockTargetXY((v2 - 128), (v3 + 100), 2501, 0, 20, 25001, 0, 0, 33554432);
	asynccall LockTargetXY((v2 - 128), (v3 - 100), 2501, 0, 20, 25001, 0, 0, 33554432);
	asynccall LockTargetXY(v2, (v3 + 150), 2501, 0, 20, 25001, 0, 0, 33554432);
	Delay(20);
	SetObjectFadeOut(v1, 16, 1);
	return;
}

void CreateTaichi(int a0, int a1) {
	int v1;
	int v2;
	v1 = CreateObjectRaw(a0, a1, 0, 0, 35001);
	SetObjectLayer(v1, 2);
	SetObjectScale(v1, 98304, 98304);
	v2 = 0;
LOC_214D4:
	if (!(v2 <= 16)) goto LOC_21528;
	SetObjectOpacity(v1, v2);
	Delay(1);
	v2++;
	goto LOC_214D4;
LOC_21528:
	PlaySound1("m025snd01", 220);
	v2 = 0;
LOC_21554:
	if (!(v2 < 3)) goto LOC_215D0;
	asynccall CreateTaiChiBomb(v1, (v2 == 0));
	Delay((10 - v2));
	v2++;
	goto LOC_21554;
LOC_215D0:
	Delay(30);
	v2 = 16;
LOC_215EC:
	if (!(v2 >= 0)) goto LOC_21640;
	SetObjectOpacity(v1, v2);
	Delay(1);
	v2--;
	goto LOC_215EC;
LOC_21640:
	FreeObjectByHandle(v1);
	return;
}

void TaiChi(int a0) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	int v7;
	int v8;
	int v9;
	int v10;
	int v11;
	int v12;
	int v13;
	int v14;
	int v15;
LOC_2165C:
	SetGlobal(37, 1);
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\025\\*");
	DisablePlayMagic();
	DownBrightness(12, 5);
	SetOverwhelming(intvAttackerMajor, 1);
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v1, (v2 - 120));
	AddAttackCounter(intvAttackerMajor, 2);
	Delay(15);
	SetObjectAnimate(intvAttackerMajor, 32768);
	Delay(20);
	SetOverwhelming(intvAttackerMajor, 0);
	v4 = 0;
LOC_217B8:
	if (!(v4 < (a0 + 1))) goto LOC_21FBC;
	v5 = 0;
LOC_217F0:
	if (!(v5 < 3)) goto LOC_21FA0;
	if (!(v5 == 0)) goto LOC_21908;
	v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
	if (!(v3 == 0)) goto LOC_2187C;
	v3 = intvDefenderMajor;
LOC_2187C:
	v6 = GetObjectScreenX(v3);
	v7 = GetObjectScreenY(v3);
	v1 = v6;
	v2 = v7;
	MoveCamera(v6, (v7 - 300), 40);
	goto LOC_21F44;
LOC_21908:
	if (!(v5 == 1)) goto LOC_21C38;
	v1 = (v6 + Rand(200, 260));
	if (!Rand(0, 1)) goto LOC_219B0;
	v2 = (v7 + Rand(220, 270));
	goto LOC_219E0;
LOC_219B0:
	v2 = (v7 - Rand(220, 270));
LOC_219E0:
	v9 = ScreenXToBattleX(v1);
	v10 = ScreenXToBattleX(v2);
	v8 = GetSoldierCountInRect((intvIsLeft ^ 1), v9, v10, 3, 3);
	if (!(v8 > 0)) goto LOC_21C30;
	v12 = 0;
LOC_21A8C:
	if (!(v12 < (v8 / 2))) goto LOC_21C30;
	v11 = GetNthSoldierInRect((intvIsLeft ^ 1), v9, v10, 3, 3, Rand(0, (v8 - 1)));
	if (!(v11 != intvDefenderMajor)) goto LOC_21C20;
	v13 = GetObjectScreenX(v11);
	v14 = GetObjectScreenY(v11);
	v15 = (((v13 - v6) * (v13 - v6)) + ((v14 - v7) * (v14 - v7)));
	if (!(v15 >= 72200)) goto LOC_21C20;
	v1 = v13;
	v2 = v14;
	goto LOC_21C30;
LOC_21C20:
	v12++;
	goto LOC_21A8C;
LOC_21C30:
	goto LOC_21F44;
LOC_21C38:
	v1 = (v6 - Rand(200, 260));
	if (!Rand(0, 1)) goto LOC_21CC4;
	v2 = (v7 + Rand(220, 270));
	goto LOC_21CF4;
LOC_21CC4:
	v2 = (v7 - Rand(220, 270));
LOC_21CF4:
	v9 = ScreenXToBattleX(v1);
	v10 = ScreenXToBattleX(v2);
	v8 = GetSoldierCountInRect((intvIsLeft ^ 1), v9, v10, 3, 3);
	if (!(v8 > 0)) goto LOC_21F44;
	v12 = 0;
LOC_21DA0:
	if (!(v12 < (v8 / 2))) goto LOC_21F44;
	v11 = GetNthSoldierInRect((intvIsLeft ^ 1), v9, v10, 3, 3, Rand(0, (v8 - 1)));
	if (!(v11 != intvDefenderMajor)) goto LOC_21F34;
	v13 = GetObjectScreenX(v11);
	v14 = GetObjectScreenY(v11);
	v15 = (((v13 - v6) * (v13 - v6)) + ((v14 - v7) * (v14 - v7)));
	if (!(v15 >= 72200)) goto LOC_21F34;
	v1 = v13;
	v2 = v14;
	goto LOC_21F44;
LOC_21F34:
	v12++;
	goto LOC_21DA0;
LOC_21F44:
	asynccall CreateTaichi(v1, v2);
	Delay(Rand(8, 12));
	v5++;
	goto LOC_217F0;
LOC_21FA0:
	Delay(45);
	v4++;
	goto LOC_217B8;
LOC_21FBC:
	Wait("CreateTaichi");
	Delay(60);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void SpoutCallback(int a0) callsign 26001 {
	int v1;
	int v2;
	int v3;
	if (!(a0 == intvDefenderMajor)) goto LOC_22090;
	if (!(GetGlobal(38) < 4)) goto LOC_22088;
	Hurt(0, a0, (intvMagicAttackValue / GetGlobal(38)));
	SetGlobal(38, (GetGlobal(38) * 2));
LOC_22088:
	return;
LOC_22090:
	v2 = GetObjectScreenX(a0);
	v3 = (GetObjectScreenY(a0) - 16);
	KillSoldier(a0);
	SetObjectFlags(a0, 65536);
	SetObjectSpeed_Cylind(a0, 0, 8);
LOC_2212C:
	v1 = GetObjectScreenZ(a0);
	SetObjectCoordinate(a0, (v2 + Rand((-12), 12)), (v3 + Rand((-9), 2)), (v1 + Rand((-3), 3)));
	Delay(1);
	if (!(v1 > 240)) goto LOC_22210;
	goto LOC_22248;
LOC_22210:
	if (!(!IsObjectExist(a0))) goto LOC_22238;
	goto LOC_22248;
LOC_22238:
	if (1) goto LOC_2212C;
LOC_22248:
	FreeObjectByHandle(a0);
	return;
}

void SpoutKillEdge(int a0, int a1) {
	int v1[4];
	int v5;
	a0 = ScreenXToBattleX(a0);
	a1 = ScreenXToBattleX(a1);
	v1[0] = GetForceHandleByXY((a0 + 1), a1);
	v1[1] = GetForceHandleByXY(a0, (a1 + 1));
	v1[2] = GetForceHandleByXY((a0 - 1), a1);
	v1[3] = GetForceHandleByXY(a0, (a1 - 1));
	v5 = 0;
LOC_22394:
	if (!(v5 < 4)) goto LOC_22484;
	if (!v1[v5]) goto LOC_22474;
	if (!((v1[v5] == intvDefenderMajor) || (v1[v5] == intvAttackerMajor))) goto LOC_2242C;
	v1[v5] = 0;
	goto LOC_22474;
LOC_2242C:
	if (!(GetSoldierSide(v1[v5]) == intvIsLeft)) goto LOC_22474;
	v1[v5] = 0;
LOC_22474:
	v5++;
	goto LOC_22394;
LOC_22484:
	v5 = 0;
LOC_22494:
	if (!(v5 < 4)) goto LOC_22538;
	if (!(v1[v5] != 0)) goto LOC_22528;
	SetObjectSpeed_Sphere(v1[v5], (v5 * 64), Rand(10, 36), 8);
LOC_22528:
	v5++;
	goto LOC_22494;
LOC_22538:
	Delay(2);
	v5 = 0;
LOC_22554:
	if (!(v5 < 4)) goto LOC_225C4;
	if (!(v1[v5] != 0)) goto LOC_225B4;
	KillSoldier(v1[v5]);
LOC_225B4:
	v5++;
	goto LOC_22554;
LOC_225C4:
	return;
}

void TraceStoneBomb(int a0) {
	Delay(4);
	SetObjectGravity(a0, 16384);
	Delay(6);
	SetObjectFadeOut(a0, 16, 1);
	return;
}

void CreateStoneBomb(int a0, int a1) {
	int v1;
	int v2;
	int v3;
	int v4;
	v2 = 0;
LOC_22644:
	if (!(v2 < 10)) goto LOC_2281C;
	v1 = CreateObjectRaw((a0 + Rand((-8), 8)), (a1 + Rand((-8), 8)), 0, 0, Rand(36002, 36003));
	SetObjectGravity(v1, 10240);
	v3 = Rand(0, 255);
	v4 = Rand(32768, 49152);
	SetObjectScale(v1, v4, v4);
	SetObjectSpeed_Sphere(v1, v3, Rand(32, 50), 6);
	asynccall TraceStoneBomb(v1);
	Delay(Rand(0, 2));
	v2++;
	goto LOC_22644;
LOC_2281C:
	return;
}

void CreateSpout(int a0, int a1, int a2) {
	int v1;
	int v2;
	int v5;
	int v6;
	asynccall CreateStoneBomb(a0, a1);
	Delay(4);
	v5 = CreateObjectRaw(a0, a1, 0, 0, 36012);
	SetObjectScale(v5, 65536, 49152);
	v6 = CreateObjectRaw(a0, a1, 0, 0, 36013);
	SetObjectScale(v6, 65536, 49152);
	if (!a2) goto LOC_2296C;
	asynccall MoveCamera(a0, (a1 - 240), 5);
LOC_2296C:
	v2 = 0;
LOC_2297C:
	if (!(v2 < 4)) goto LOC_22A64;
	MoveObject(v6, 0, 0, 56);
	v1 = CreateObjectRaw(a0, a1, 0, 0, (36005 + (v2 * 2)));
	if (!(v2 < 3)) goto LOC_22A54;
	Delay(1);
	FreeObjectByHandle(v1);
LOC_22A54:
	v2++;
	goto LOC_2297C;
LOC_22A64:
	v2 = 73728;
LOC_22A74:
	if (!(v2 <= 131072)) goto LOC_22B10;
	MoveObject(v6, 0, 0, 28);
	SetObjectScale(v1, 65536, v2);
	Delay(1);
	v2 = (v2 + 8192);
	goto LOC_22A74;
LOC_22B10:
	Delay(24);
	SetObjectFadeOut(v1, 16, 1);
	SetObjectScaleShrink(v1, 4096);
	SetObjectFadeOut(v5, 16, 1);
	SetObjectScaleShrink(v5, 4096);
	SetObjectFadeOut(v6, 16, 1);
	SetObjectScaleShrink(v6, 4096);
	return;
}

void CreateSpoutAround(int a0, int a1, int a2) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	int v7[45];
	int v52;
LOC_22BE4:
	PlaySound1("m026snd01", 200);
	v1 = ScreenXToBattleX(a0);
	v2 = ScreenXToBattleX(a1);
	v6 = GetSoldierCountInRect((intvIsLeft ^ 1), v1, v2, 4, 2);
	v5 = 0;
	v4 = 0;
LOC_22CA8:
	if (!(v4 < v6)) goto LOC_22D58;
	v7[v5] = GetNthSoldierInRect((intvIsLeft ^ 1), v1, v2, 4, 2, v4);
	if (!(v7[v4] != intvDefenderMajor)) goto LOC_22D48;
	v5++;
LOC_22D48:
	v4++;
	goto LOC_22CA8;
LOC_22D58:
	v6 = v5;
	v4 = 0;
LOC_22D78:
	if (!(v4 < v6)) goto LOC_22E24;
	v52 = Rand(0, (v6 - 1));
	v3 = v7[v4];
	v7[v4] = v7[v52];
	v7[v52] = v3;
	v4++;
	goto LOC_22D78;
LOC_22E24:
	v4 = 0;
LOC_22E34:
	if (!(v4 < a2)) goto LOC_23030;
	Delay(Rand(4, 8));
	if (!(v6 > 0)) goto LOC_22F8C;
	v1 = GetObjectScreenX(v7[v4]);
	v2 = GetObjectScreenY(v7[v4]);
	asynccall SpoutKillEdge(v1, v2);
	asynccall SpoutCallback(v7[v4]);
	PlaySound1("m026snd01", 230);
	asynccall CreateSpout(v1, v2, 0);
	v6--;
	goto LOC_23020;
LOC_22F8C:
	v1 = (a0 + Rand((-432), 432));
	v2 = (a1 + Rand((-187), 187));
	asynccall CreateSpout(v1, v2, 0);
LOC_23020:
	v4++;
	goto LOC_22E34;
LOC_23030:
	return;
}

void Spout(int a0) {
	int v1;
	int v2;
	int v3;
	int v4;
LOC_23038:
	SetGlobal(38, 1);
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\026\\*");
	DisablePlayMagic();
	DownBrightness(12, 5);
	SetOverwhelming(intvAttackerMajor, 1);
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v1, (v2 - 120));
	AddAttackCounter(intvAttackerMajor, 2);
	Delay(15);
	SetObjectAnimate(intvAttackerMajor, 32768);
	Delay(20);
	SetOverwhelming(intvAttackerMajor, 0);
	v3 = 0;
LOC_23194:
	if (!(v3 < 3)) goto LOC_23374;
	v4 = GetRandomSoldierFromAlive((intvIsLeft ^ 1));
	if (!v4) goto LOC_23280;
	v1 = GetObjectScreenX(v4);
	v2 = GetObjectScreenY(v4);
	asynccall SpoutKillEdge(v1, v2);
	asynccall SpoutCallback(v4);
	goto LOC_23300;
LOC_23280:
	v1 = (GetObjectScreenX(intvDefenderMajor) + Rand((-200), 200));
	v2 = (GetObjectScreenY(intvDefenderMajor) + Rand((-150), 150));
LOC_23300:
	asynccall CreateSpout(v1, v2, 1);
	CreateSpoutAround(v1, v2, (a0 * 2));
	Delay(40);
	v3++;
	goto LOC_23194;
LOC_23374:
	Wait("CreateSpout");
	Delay(60);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void FireCowCallback(int a0, int a1) callsign 27001 {
	int v1;
	int v2;
	int v3;
	if (!(a1 == intvDefenderMajor)) goto LOC_234FC;
	SetObjectFadeOut(a0, 2, 1);
	v3 = GetObjectContext(a0, 1);
	if (!GetGlobal(39 + v3)) goto LOC_23460;
	v2 = (intvMagicAttackValue / GetGlobal(39 + v3));
LOC_23460:
	Hurt(a0, a1, v2);
	if (!(v2 > 0)) goto LOC_234F4;
	SetGlobal(39 + v3, (GetGlobal(39 + v3) * 2));
	asynccall LockTargetTime2(a1, 10015, 60);
LOC_234F4:
	goto LOC_23574;
LOC_234FC:
	Hurt(a0, a1, 0);
	asynccall FireMan(a1, 10015, 60);
	asynccall Blood(a1, 16);
LOC_23574:
	v1 = GetObjectContext(a0, 0);
	v1--;
	if (!(v1 <= 0)) goto LOC_235E8;
	SetObjectFadeOut(a0, 2, 1);
	goto LOC_2360C;
LOC_235E8:
	SetObjectContext(a0, 0, v1);
LOC_2360C:
	return;
}

void AttachFireToCow(int a0, int a1) {
	int v1;
	int v2;
	v1 = CreateObjectByReference(a0, 37002, a1, 50);
	SetObjectScale(v1, 131072, 131072);
	SetObjectFlags(v1, 262144);
	SetObjectOpacity(v1, 12);
	v2 = CreateObjectByReference(a0, 37002, a1, 50);
	SetObjectScale(v2, 131072, 131072);
	SetObjectFlags(v2, 262144);
	SetObjectOpacity(v2, 12);
LOC_2373C:
	if (!IsObjectExist(a0)) goto LOC_23898;
	SetCoordinateByReference_Cylind(v1, a0, a1, 24, 50);
	MoveObject(v1, Rand((-8), 8), 0, Rand((-8), 8));
	SetCoordinateByReference_Cylind(v2, a0, (a1 - 128), 20, 50);
	MoveObject(v2, Rand((-8), 8), 0, Rand((-8), 8));
	Delay(1);
	goto LOC_2373C;
LOC_23898:
	FreeObjectByHandle(v1);
	FreeObjectByHandle(v2);
	return;
}

void CreateFireCow(int a0, int a1, int a2, int a3, int a4) {
	int v1;
	int v2;
	int v3;
	v1 = CreateObjectRaw((a0 + (8 * Rand((-6), 6))), (a1 + (8 * Rand((-8), 8))), 0, a2, 37001);
	SetObjectSpeed_Sphere(v1, a2, 0, (8 + Rand((-2), 2)));
	SetCallbackProcedure(v1, 27001);
	SetObjectFlags(v1, (33554432 + 16777216));
	SetObjectContext(v1, 0, (8 + Rand((-2), 4)));
	if (!(a3 == 1)) goto LOC_23A90;
	SetObjectContext(v1, 1, 0);
	goto LOC_23AB4;
LOC_23A90:
	SetObjectContext(v1, 1, 1);
LOC_23AB4:
	asynccall AttachFireToCow(v1, (a2 + 128));
	if (!a4) goto LOC_23B40;
	asynccall LockCameraLine(v1, ((3 - a4) * 80), a3, 280);
LOC_23B40:
	v3 = 1;
LOC_23B50:
	if (!IsObjectExist(v1)) goto LOC_23CD4;
	v2 = GetObjectScreenX(v1);
	if (!(a3 == 1)) goto LOC_23BF8;
	if (!(v2 >= GetBattleWidthInScreenX())) goto LOC_23BF0;
	SetObjectFadeOut(v1, 16, 2);
	goto LOC_23CD4;
LOC_23BF0:
	goto LOC_23C40;
LOC_23BF8:
	if (!(v2 <= 0)) goto LOC_23C40;
	SetObjectFadeOut(v1, 16, 2);
	goto LOC_23CD4;
LOC_23C40:
	Delay(1);
	v3++;
	if (!((v3 & 7) == 0)) goto LOC_23CCC;
	MoveObject(v1, 0, (8 * Rand((-3), 3)), 0);
LOC_23CCC:
	goto LOC_23B50;
LOC_23CD4:
	return;
}

void FireCow(int a0) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
LOC_23CDC:
	if (!(intvIsLeft == 1)) goto LOC_23D20;
	SetGlobal(39 + 0, 1);
	goto LOC_23D38;
LOC_23D20:
	SetGlobal(39 + 1, 1);
LOC_23D38:
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\027\\*");
	DisablePlayMagic();
	DownBrightness(12, 5);
	SetOverwhelming(intvAttackerMajor, 1);
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v1, (v2 - 120));
	AddAttackCounter(intvAttackerMajor, 2);
	Delay(15);
	SetObjectAnimate(intvAttackerMajor, 32768);
	Delay(20);
	SetOverwhelming(intvAttackerMajor, 0);
	Delay(10);
	if (!(intvIsLeft == 1)) goto LOC_23EC0;
	v4 = (-1);
	v3 = 0;
	goto LOC_23EE0;
LOC_23EC0:
	v4 = 1;
	v3 = 128;
LOC_23EE0:
	v5 = GetSoldierMaxX(intvIsLeft);
	if (!(intvIsLeft == 1)) goto LOC_23F44;
	SetViewCamera((v5 - 320), 0);
	goto LOC_23F6C;
LOC_23F44:
	SetViewCamera((v5 + 320), 0);
LOC_23F6C:
	Delay(15);
	PlaySound1("m027snd01", 180);
	PlaySound1("m027snd02", 180);
	if (!(a0 == 0)) goto LOC_24160;
	asynccall CreateFireCow((v5 + (1200 * v4)), (562 + 75), v3, intvIsLeft, 0);
	asynccall CreateFireCow((v5 + (1200 * v4)), (562 - 75), v3, intvIsLeft, 0);
	asynccall CreateFireCow((v5 + (1200 * v4)), (562 + 225), v3, intvIsLeft, (a0 + 1));
	asynccall CreateFireCow((v5 + (1200 * v4)), (562 - 225), v3, intvIsLeft, 0);
	goto LOC_24784;
LOC_24160:
	if (!(a0 == 1)) goto LOC_24424;
	asynccall CreateFireCow((v5 + (1200 * v4)), (562 + 350), v3, intvIsLeft, 0);
	asynccall CreateFireCow((v5 + (1200 * v4)), (562 + 250), v3, intvIsLeft, 0);
	asynccall CreateFireCow((v5 + (1200 * v4)), (562 + 150), v3, intvIsLeft, 0);
	asynccall CreateFireCow((v5 + (1200 * v4)), 562, v3, intvIsLeft, (a0 + 1));
	asynccall CreateFireCow((v5 + (1200 * v4)), (562 - 150), v3, intvIsLeft, 0);
	asynccall CreateFireCow((v5 + (1200 * v4)), (562 - 250), v3, intvIsLeft, 0);
	asynccall CreateFireCow((v5 + (1200 * v4)), (562 - 350), v3, intvIsLeft, 0);
	goto LOC_24784;
LOC_24424:
	asynccall CreateFireCow((v5 + (1200 * v4)), (562 + 400), v3, intvIsLeft, 0);
	asynccall CreateFireCow((v5 + (1200 * v4)), (562 + 300), v3, intvIsLeft, 0);
	asynccall CreateFireCow((v5 + (1200 * v4)), (562 + 200), v3, intvIsLeft, 0);
	asynccall CreateFireCow((v5 + (1200 * v4)), (562 + 100), v3, intvIsLeft, 0);
	asynccall CreateFireCow((v5 + (1200 * v4)), 562, v3, intvIsLeft, (a0 + 1));
	asynccall CreateFireCow((v5 + (1200 * v4)), (562 - 100), v3, intvIsLeft, 0);
	asynccall CreateFireCow((v5 + (1200 * v4)), (562 - 200), v3, intvIsLeft, 0);
	asynccall CreateFireCow((v5 + (1200 * v4)), (562 - 300), v3, intvIsLeft, 0);
	asynccall CreateFireCow((v5 + (1200 * v4)), (562 - 400), v3, intvIsLeft, 0);
LOC_24784:
	Delay(Rand(110, 160));
	PlaySound1("m027snd02", 210);
	Delay(10);
	PlaySound1("m027snd01", 160);
	Wait("LockCameraLine");
	Delay(20);
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	MoveCamera(v1, (v2 - 120), 20);
	Delay(60);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void CreateRollStoneBreak(int a0, int a1) {
	int v1;
	int v2;
	v2 = Rand(0, 255);
	v1 = CreateObjectByReference(a0, Rand(38002, 38008), v2, a1);
	SetObjectGravity(v1, 18432);
	SetCoordinateByReference_Cylind(v1, a0, v2, 12, a1);
	SetObjectSpeed_Sphere(v1, v2, Rand(38, 60), Rand(6, 10));
	Delay(8);
	SetObjectFadeOut(v1, 16, 2);
	return;
}

void RollDownCallback(int a0, int a1) callsign 28001 {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	int v7;
	int v8;
	int v9;
	v2 = GetObjectContext(a0, 1);
	v3 = GetObjectContext(a0, 2);
	if (!(a1 == intvDefenderMajor)) goto LOC_24BAC;
	if (!GetGlobal(41)) goto LOC_24934;
	v9 = (intvMagicAttackValue / GetGlobal(41));
LOC_24934:
	Hurt(a0, a1, v9);
	if (!(v9 > 0)) goto LOC_2498C;
	SetGlobal(41, (GetGlobal(41) * 2));
LOC_2498C:
	if (!(v2 == 0)) goto LOC_249CC;
	a0 = GetObjectContext(a0, 0);
LOC_249CC:
	if (!(GetGlobal(1) == 0)) goto LOC_24A08;
	DelayAmbientSound("m028snd02", 255, 10);
LOC_24A08:
	v8 = 0;
LOC_24A18:
	if (!(v8 < 3)) goto LOC_24B44;
	asynccall CreateRollStoneBreak(a0, Rand(10, 20));
	asynccall CreateRollStoneBreak(a0, Rand(25, 35));
	asynccall CreateRollStoneBreak(a0, Rand(40, 50));
	asynccall CreateRollStoneBreak(a0, Rand(55, 65));
	v8++;
	goto LOC_24A18;
LOC_24B44:
	SetObjectFlags(a0, 128);
	ClearObjectFlags(a0, 251658240);
	Delay(4);
	FreeObjectByHandle(a0);
	return;
	goto LOC_24BCC;
LOC_24BAC:
	Hurt(a0, a1, 0);
LOC_24BCC:
	if (!(v2 != 0)) goto LOC_24E68;
	SetObjectFriction(a0, 1);
	SetObjectFriction(v2, 1);
	SetObjectFriction(v3, 1);
	v7 = Rand((-4), 4);
	v4 = (GetObjectScreenX(a0) + v7);
	v5 = GetObjectScreenY(a0);
	v6 = GetObjectScreenZ(a0);
	SetObjectCoordinate(a0, v4, v5, v6);
	v4 = (GetObjectScreenX(v2) + v7);
	v5 = GetObjectScreenY(v2);
	v6 = GetObjectScreenZ(v2);
	SetObjectCoordinate(v2, v4, v5, v6);
	v4 = (GetObjectScreenX(v3) + v7);
	v5 = GetObjectScreenY(v3);
	v6 = GetObjectScreenZ(v3);
	SetObjectCoordinate(v3, v4, v5, v6);
	Delay(1);
	SetObjectFriction(a0, 0);
	SetObjectFriction(v2, 0);
	SetObjectFriction(v3, 0);
LOC_24E68:
	if (!(v2 == 0)) goto LOC_24EA8;
	a0 = GetObjectContext(a0, 0);
LOC_24EA8:
	v1 = GetObjectContext(a0, 0);
	v1--;
	if (!(v1 <= 0)) goto LOC_250C8;
	if (!(GetGlobal(1) == 0)) goto LOC_24F2C;
	DelayAmbientSound("m028snd02", 255, 10);
LOC_24F2C:
	v8 = 0;
LOC_24F3C:
	if (!(v8 < 3)) goto LOC_25068;
	asynccall CreateRollStoneBreak(a0, Rand(10, 20));
	asynccall CreateRollStoneBreak(a0, Rand(25, 35));
	asynccall CreateRollStoneBreak(a0, Rand(40, 50));
	asynccall CreateRollStoneBreak(a0, Rand(55, 65));
	v8++;
	goto LOC_24F3C;
LOC_25068:
	SetObjectFlags(a0, 128);
	ClearObjectFlags(a0, 251658240);
	Delay(4);
	FreeObjectByHandle(a0);
	goto LOC_250EC;
LOC_250C8:
	SetObjectContext(a0, 0, v1);
LOC_250EC:
	return;
}

void CreateRollStone(int a0, int a1, int a2) {
	int v1;
	int v2;
	int v3;
	v3 = CreateObjectRaw((a0 + 96), (a1 + 8), 0, 0, 2501);
	SetObjectSpeed_Sphere(v3, 192, 0, a2);
	SetObjectFlags(v3, 50331648);
	SetCallbackProcedure(v3, 28001);
	SetObjectContext(v3, 0, 0);
	SetObjectContext(v3, 1, 0);
	SetObjectContext(v3, 2, 0);
	v2 = CreateObjectRaw((a0 - 96), (a1 + 8), 0, 0, 2501);
	SetObjectSpeed_Sphere(v2, 192, 0, a2);
	SetObjectFlags(v2, 50331648);
	SetCallbackProcedure(v2, 28001);
	SetObjectContext(v2, 0, 0);
	SetObjectContext(v2, 1, 0);
	SetObjectContext(v2, 2, 0);
	v1 = CreateObjectRaw(a0, a1, 0, 0, 38001);
	SetObjectSpeed_Sphere(v1, 192, 0, a2);
	SetObjectScale(v1, 49152, 49152);
	SetObjectFlags(v1, 50331648);
	SetCallbackProcedure(v1, 28001);
	SetObjectContext(v1, 0, 8);
	SetObjectContext(v1, 1, v2);
	SetObjectContext(v1, 2, v3);
	SetObjectContext(v2, 0, v1);
	SetObjectContext(v3, 0, v1);
LOC_2560C:
	if (!(GetObjectScreenY(v1) > 0)) goto LOC_25648;
	Delay(2);
	goto LOC_2560C;
LOC_25648:
	FreeObjectByHandle(v2);
	FreeObjectByHandle(v3);
	SetObjectFadeOut(v1, 16, 1);
	return;
}

void TracingCamera(int a0, int a1, int a2, int a3, int a4) {
	int v1;
	int v2;
	int v3;
	int v4;
	Delay(a3);
	v1 = CreateObjectRaw(a0, (GetBattleHeightInScreenY() - 100), 0, 0, 2501);
	v2 = CreateObjectRaw(a1, 400, 0, 0, 2501);
	ApproachObjectTowards(v1, v2, a2);
	if (!(a4 == 1)) goto LOC_25808;
LOC_25778:
	v3 = GetObjectScreenX(v1);
	v4 = GetObjectScreenY(v1);
	Delay(2);
	SetViewCamera(v3, (v4 - 240));
	if ((v3 < a1)) goto LOC_25778;
	goto LOC_25890;
LOC_25808:
	v3 = GetObjectScreenX(v1);
	v4 = GetObjectScreenY(v1);
	Delay(2);
	SetViewCamera(v3, (v4 - 240));
	if ((v3 > a1)) goto LOC_25808;
LOC_25890:
	FreeObjectByHandle(v1);
	FreeObjectByHandle(v2);
	return;
}

void RollDown(int a0) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	int v7;
	int v8;
	int v9;
LOC_258C0:
	SetGlobal(1, 0);
	SetGlobal(41, 1);
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\028\\*");
	DisablePlayMagic();
	DownBrightness(12, 5);
	SetOverwhelming(intvAttackerMajor, 1);
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v1, (v2 - 120));
	AddAttackCounter(intvAttackerMajor, 2);
	Delay(15);
	SetObjectAnimate(intvAttackerMajor, 32768);
	Delay(20);
	SetOverwhelming(intvAttackerMajor, 0);
	switch (a0) { 
		case 0: goto LOC_25A48;
		case 1: goto LOC_25A70;
	}
	goto LOC_25A98;
LOC_25A48:
	v8 = 12;
	v9 = 8;
	goto LOC_25AB8;
LOC_25A70:
	v8 = 14;
	v9 = 8;
	goto LOC_25AB8;
LOC_25A98:
	v8 = 16;
	v9 = 8;
LOC_25AB8:
	v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
	if (!(v3 == 0)) goto LOC_25B0C;
	v3 = intvDefenderMajor;
LOC_25B0C:
	if (!(intvIsLeft == 1)) goto LOC_25BD0;
	v4 = ((((GetObjectScreenX(v3) + ((a0 + 1) * 250)) + (Rand((-25), 25) * 4)) - 100) + Rand((-20), 20));
	goto LOC_25C70;
LOC_25BD0:
	v4 = ((((GetObjectScreenX(v3) - ((a0 + 1) * 250)) + (Rand((-25), 25) * 4)) + 100) + Rand((-20), 20));
LOC_25C70:
	v5 = GetObjectScreenY(v3);
	v7 = (((a0 + 1) * 2) + 1);
	if (!(intvIsLeft == 1)) goto LOC_25D5C;
	asynccall TracingCamera((v4 - (v7 * 250)), (v4 - ((a0 + 1) * 250)), v8, 0, intvIsLeft);
	goto LOC_25DD4;
LOC_25D5C:
	asynccall TracingCamera((v4 + (v7 * 250)), (v4 + ((a0 + 1) * 250)), v8, 0, intvIsLeft);
LOC_25DD4:
	v6 = 0;
LOC_25DE4:
	if (!(v6 < v7)) goto LOC_25ED0;
	PlaySound1("m028snd01", 255);
	asynccall CreateRollStone(v4, (GetBattleHeightInScreenY() + 200), v9);
	Delay(10);
	if (!(intvIsLeft == 1)) goto LOC_25EA4;
	v4 = (v4 - 250);
	goto LOC_25EC0;
LOC_25EA4:
	v4 = (v4 + 250);
LOC_25EC0:
	v6++;
	goto LOC_25DE4;
LOC_25ED0:
	Wait("TracingCamera");
	Wait("CreateRollStone");
	Delay(60);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void PaCallback(int a0, int a1) callsign 29001 {
	int v1;
	if (!(a1 == intvDefenderMajor)) goto LOC_25FD0;
	if (!GetGlobal(42)) goto LOC_25F70;
	v1 = (intvMagicAttackValue / GetGlobal(42));
LOC_25F70:
	Hurt(a0, a1, v1);
	if (!(v1 > 0)) goto LOC_25FC8;
	SetGlobal(42, (GetGlobal(42) * 2));
LOC_25FC8:
	goto LOC_25FF0;
LOC_25FD0:
	Hurt(a0, a1, 0);
LOC_25FF0:
	return;
}

void CreatePaBomb(int a0, int a1, int a2) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	Delay((Rand(0, 10) * 3));
	v4 = CreateObjectByReference(a0, 39011, 0, 220);
	SetCoordinateByReference_Cylind(v4, a0, a2, a1, 220);
	SetObjectScale(v4, 4096, 65536);
	SetObjectSpeed_Cylind(v4, 0, (-20));
	v5 = 1;
LOC_260F0:
	if (!(v5 <= 10)) goto LOC_26170;
	Delay(2);
	SetObjectScale(v4, (4096 + ((12288 / 10) * v5)), 65536);
	v5++;
	goto LOC_260F0;
LOC_26170:
	SetObjectFadeOut(v4, 8, 1);
	v1 = CreateObjectByReference(a0, 39002, 0, 0);
	SetCoordinateByReference_Cylind(v1, a0, a2, a1, 0);
	SetObjectScale(v1, 4096, 4096);
	SetCallbackProcedure(v1, 29001);
	SetObjectFlags(v1, (33554432 + 16777216));
	SetObjectScaleShrink(v1, (-4096));
	Delay(10);
	v2 = GetObjectScreenX(v1);
	v3 = GetObjectScreenY(v1);
	asynccall LockTargetXY((v2 + 20), v3, 2501, 0, 8, 29001, 0, 0, (33554432 + 16777216));
	asynccall LockTargetXY(v2, (v3 + 20), 2501, 0, 8, 29001, 0, 0, (33554432 + 16777216));
	asynccall LockTargetXY((v2 - 20), v3, 2501, 0, 8, 29001, 0, 0, (33554432 + 16777216));
	asynccall LockTargetXY(v2, (v3 - 20), 2501, 0, 8, 29001, 0, 0, (33554432 + 16777216));
	Delay(10);
	SetObjectFadeOut(v1, 16, 1);
	return;
}

void BigBombCircle(int a0, int a1) {
	int v1;
	int v2;
	v1 = CreateObjectByReference(a0, a1, 0, 0);
	v2 = 1;
LOC_2651C:
	if (!(v2 < 32)) goto LOC_265A8;
	SetObjectScale(v1, (65536 + (6144 * v2)), (65536 + (6144 * v2)));
	Delay(1);
	v2++;
	goto LOC_2651C;
LOC_265A8:
	SetObjectFadeOut(v1, 16, 1);
	return;
}

void BigPaBombHurt(int a0) {
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	v4 = GetObjectScreenX(a0);
	v5 = GetObjectScreenY(a0);
	v3 = 0;
	v6 = Rand(0, 255);
	v2 = 0;
LOC_26658:
	if (!(v2 < 16)) goto LOC_26764;
	asynccall LockByCenter(a0, 16018, v6, v3, 4, (33554432 + 16777216), 29001);
	v6 = ((v6 + 64) & 255);
	if (!((v2 % 4) == 3)) goto LOC_26754;
	v3 = (v3 + 40);
	v6 = (v6 + 32);
LOC_26754:
	v2++;
	goto LOC_26658;
LOC_26764:
	return;
}

void CreateBigPaBomb(int a0) {
	int v1;
	int v2;
LOC_2676C:
	PlaySound1("m029snd02", 255);
	v1 = CreateObjectByReference(a0, 39012, 0, 220);
	SetObjectSpeed_Cylind(v1, 0, (-20));
LOC_267EC:
	Delay(1);
	v2 = GetObjectScreenZ(v1);
	if ((v2 > 0)) goto LOC_267EC;
	asynccall BigPaBombHurt(v1);
	SetObjectFadeOut(v1, 8, 1);
	asynccall BigBombCircle(a0, 39013);
	Delay(10);
	asynccall BigBombCircle(a0, 39014);
	Delay(10);
	asynccall BigBombCircle(a0, 39013);
	return;
}

void CreateRotateLight(int a0, int a1, int a2, int a3) {
	int v1;
	int v2;
	int v3;
LOC_26924:
	v1 = CreateObjectByReference(a0, 39016, 0, 0);
	SetCoordinateByReference_Cylind(v1, a0, a1, a2, 0);
	SetObjectFlags(v1, (33554432 + 16777216));
	SetObjectScale(v1, 49152, 32768);
	v3 = CreateObjectByReference(a0, 39017, 0, 0);
	SetCoordinateByReference_Cylind(v3, a0, a1, a2, 0);
	SetObjectScale(v3, 49152, 20480);
	v2 = 0;
LOC_26A7C:
	if (!(v2 < 128)) goto LOC_26BBC;
	a1 = ((a1 + 8) & 255);
	if (!((v2 % 4) == 0)) goto LOC_26B04;
	a2 = (a2 + 2);
LOC_26B04:
	Delay(1);
	SetCoordinateByReference_Cylind(v1, a0, a1, a2, 0);
	SetCoordinateByReference_Cylind(v3, a0, a1, a2, 0);
	SetCoordinateByReference_Cylind(a3, a0, a1, a2, 0);
	v2++;
	goto LOC_26A7C;
LOC_26BBC:
	SetObjectFadeOut(v1, 8, 1);
	SetObjectFadeOut(v3, 8, 1);
	return;
}

void CreateRotateBomb(int a0, int a1, int a2) {
	int v1;
	int v2;
	v1 = CreateObjectByReference(a0, 39015, 0, 220);
	SetCoordinateByReference_Cylind(v1, a0, a1, a2, 220);
	SetObjectScale(v1, 49152, 65536);
	SetObjectSpeed_Cylind(v1, 0, (-20));
LOC_26CC8:
	Delay(1);
	v2 = GetObjectScreenZ(v1);
	if ((v2 > 0)) goto LOC_26CC8;
	CreateRotateLight(a0, a1, a2, v1);
	SetObjectFadeOut(v1, 16, 1);
	return;
}

void CreatePaLight(int a0, int a1, int a2, int a3) {
	int v1;
	v1 = CreateObjectByReference(a0, a2, 128, 0);
	FreeObjectByHandle(a3);
	Delay(9);
	FreeObjectByHandle(v1);
	v1 = CreateObjectByReference(a0, a1, 128, 45);
	SetObjectSpeed_Cylind(v1, 0, 16);
	asynccall ProduceShadowTime(v1, 9999);
	Delay(18);
	FreeObjectByHandle(v1);
	return;
}

void CreatePa(int a0, int a1) {
	int v1;
	int v2;
	int v3;
	int v4[7];
	PlaySound1("m029snd01", 255);
	v1 = CreateObjectRaw(a0, a1, 0, 0, 39001);
	SetObjectLayer(v1, 2);
	SetObjectFlags(v1, 262144);
	v2 = 0;
LOC_26F24:
	if (!(v2 <= 16)) goto LOC_26F78;
	SetObjectOpacity(v1, v2);
	Delay(1);
	v2++;
	goto LOC_26F24;
LOC_26F78:
	ClearObjectFlags(v1, 262144);
	SetObjectFlags(v1, 128);
	v2 = 0;
LOC_26FC0:
	if (!(v2 < 8)) goto LOC_27034;
	v4[v2] = CreateObjectByReference(v1, (39003 + v2), 128, 0);
	v2++;
	goto LOC_26FC0;
LOC_27034:
	Delay(10);
	v2 = 0;
LOC_27050:
	if (!(v2 < 4)) goto LOC_27144;
	asynccall CreatePaLight(v1, (39003 + v2), (39015 + v2), v4[v2]);
	asynccall CreatePaLight(v1, ((39003 + v2) + 4), ((39015 + 4) + v2), v4[(v2 + 4)]);
	Delay(4);
	v2++;
	goto LOC_27050;
LOC_27144:
	Wait("CreatePaLight");
	Delay(20);
	CreateBigPaBomb(v1);
	PlaySound1("m029snd03", 255);
	v3 = 0;
LOC_27198:
	if (!(v3 < 6)) goto LOC_27290;
	v2 = 0;
LOC_271C4:
	if (!(v2 < 6)) goto LOC_27260;
	asynccall CreatePaBomb(v1, (130 + (60 * v3)), (Rand(0, 128) + (40 * v2)));
	v2++;
	goto LOC_271C4;
LOC_27260:
	Delay(Rand(1, 8));
	v3++;
	goto LOC_27198;
LOC_27290:
	Wait("CreatePaBomb");
	Delay(10);
	v2 = 8;
LOC_272B8:
	if (!(v2 >= 0)) goto LOC_2730C;
	SetObjectOpacity(v1, v2);
	Delay(2);
	v2--;
	goto LOC_272B8;
LOC_2730C:
	return;
}

void Pa(int a0) {
	int v1;
	int v2;
	int v3;
LOC_27314:
	SetGlobal(42, 1);
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\029\\*");
	DisablePlayMagic();
	DownBrightness(12, 5);
	SetOverwhelming(intvAttackerMajor, 1);
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v1, (v2 - 120));
	AddAttackCounter(intvAttackerMajor, 2);
	Delay(15);
	SetObjectAnimate(intvAttackerMajor, 32768);
	Delay(20);
	SetOverwhelming(intvAttackerMajor, 0);
	v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
	if (!(v3 == 0)) goto LOC_274B4;
	v3 = intvDefenderMajor;
LOC_274B4:
	v1 = GetObjectScreenX(v3);
	v2 = GetObjectScreenY(v3);
	asynccall HoldAllDelay((8 * Rand(6, 12)));
	MoveCamera(v1, (v2 - 160), 40);
	asynccall CreatePa(v1, v2);
	Wait("CreatePa");
	Delay(60);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void HalfMoonNewCallback(int a0, int a1) callsign 30001 {
	int v1;
	if (!(a1 == intvDefenderMajor)) goto LOC_276D0;
	FreeObjectByHandle(a0);
	if (!GetGlobal(43)) goto LOC_27638;
	v1 = (intvMagicAttackValue / GetGlobal(43));
LOC_27638:
	Hurt(a0, a1, v1);
	if (!(v1 > 0)) goto LOC_27690;
	SetGlobal(43, (GetGlobal(43) * 2));
LOC_27690:
	HitGeneral(a0, a1, 40003, 2, 48, 0);
	goto LOC_2771C;
LOC_276D0:
	asynccall FireMan(a1, 10016, 60);
	Hurt(a0, a1, 0);
LOC_2771C:
	return;
}

void CheckHalfMoonNew(int a0, int a1) {
	int v1;
LOC_2772C:
	if (!IsObjectExist(a0)) goto LOC_2781C;
	Delay(1);
	v1 = GetObjectScreenX(a0);
	if (!(intvIsLeft == 1)) goto LOC_277DC;
	if (!(v1 > (GetBattleWidthInScreenX() - 100))) goto LOC_277D4;
	FreeObjectByHandle(a0);
	goto LOC_2781C;
LOC_277D4:
	goto LOC_27814;
LOC_277DC:
	if (!(v1 < 100)) goto LOC_27814;
	FreeObjectByHandle(a0);
	goto LOC_2781C;
LOC_27814:
	goto LOC_2772C;
LOC_2781C:
	FreeObjectByHandle(a1);
	return;
}

void HalfMoonNewMotion(int a0, int a1, int a2, int a3) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	int v7;
	int v8;
	int v9;
	int v10;
	float v12;
	v1 = GetObjectScreenX(a0);
	v2 = GetObjectScreenY(a0);
	v7 = GetObjectScreenZ(a0);
	v3 = GetObjectScreenX(intvDefenderMajor);
	v4 = GetObjectScreenY(intvDefenderMajor);
	v9 = (v3 - v1);
	if (!(v9 < 0)) goto LOC_27958;
	v9 = (-v9);
	if (!(v9 < 512)) goto LOC_27950;
	v3 = (v1 - 512);
LOC_27950:
	goto LOC_27990;
LOC_27958:
	if (!(v9 < 512)) goto LOC_27990;
	v3 = (v1 + 512);
LOC_27990:
	v12 = ((float)v9);
	v12 = ((v12 * 1124073472) / 1167261696);
	v10 = ((int)v12);
	v8 = 0;
LOC_279F0:
	if (!(v8 < v10)) goto LOC_27B7C;
	v6 = (v1 + (((v3 - v1) * v8) / v10));
	if (!(a3 == 0)) goto LOC_27AB8;
	v5 = (v4 + CalcSin(((v8 * 128) / v10), a2));
	goto LOC_27B00;
LOC_27AB8:
	v5 = (v4 - CalcSin(((v8 * 128) / v10), a2));
LOC_27B00:
	SetObjectCoordinate(a0, v6, v5, v7);
	SetCoordinateByReference_Cylind(a1, a0, 0, 0, 0);
	Delay(1);
	v8++;
	goto LOC_279F0;
LOC_27B7C:
	if (!(((float)v10) != v12)) goto LOC_27BFC;
	SetObjectCoordinate(a0, v3, v4, v7);
	SetCoordinateByReference_Cylind(a1, a0, 0, 0, 0);
LOC_27BFC:
	if (!IsObjectExist(a0)) goto LOC_27DA8;
	SetObjectFadeOut(a0, 16, 1);
	v8 = 128;
LOC_27C4C:
	if (!IsObjectExist(a0)) goto LOC_27DA8;
	v6 = (v1 + (((v3 - v1) * v8) / 128));
	if (!(a3 == 0)) goto LOC_27CFC;
	v5 = (v4 + CalcSin(v8, a2));
	goto LOC_27D2C;
LOC_27CFC:
	v5 = (v4 - CalcSin(v8, a2));
LOC_27D2C:
	SetObjectCoordinate(a0, v6, v5, v7);
	SetCoordinateByReference_Cylind(a1, a0, 0, 0, 0);
	Delay(1);
	v8++;
	goto LOC_27C4C;
LOC_27DA8:
	return;
}

int GetGeneralWidth() {
	int v1;
LOC_27DB0:
	v1 = (GetObjectScreenX(intvAttackerMajor) - GetObjectScreenX(intvDefenderMajor));
	if (!(v1 < 0)) goto LOC_27E1C;
	v1 = (-v1);
LOC_27E1C:
	return v1;
}

void CreateHalfMoonNew(int a0, int a1, int a2, int a3, int a4, int a5) {
	int v1;
	int v2;
	int v4;
	v1 = CreateObjectByReference(a0, 40001, a1, 0);
	SetCoordinateByReference_Cylind(v1, a0, a2, a3, 0);
	SetObjectFlags(v1, (33554432 + 16777216));
	SetCallbackProcedure(v1, 30001);
	SetObjectScale(v1, 49152, 49152);
	v2 = CreateObjectByReference(v1, 40002, a1, 0);
	SetCoordinateByReference_Cylind(v2, v1, 0, 0, 0);
	v4 = GetGeneralWidth();
	asynccall CheckHalfMoonNew(v1, v2);
	if (!a4) goto LOC_27FE8;
	asynccall LockCameraSimple(v1, (-330));
LOC_27FE8:
	if (!(v4 < 1200)) goto LOC_28054;
	ApproachObjectTowards(v1, intvDefenderMajor, 30);
	ApproachObjectTowards(v2, intvDefenderMajor, 30);
	goto LOC_28080;
LOC_28054:
	asynccall HalfMoonNewMotion(v1, v2, a3, a5);
LOC_28080:
	return;
}

void HalfMoonNew(int a0) {
	int v1;
	int v2;
	int v3;
	int v5;
	int v6;
	int v7;
	int v8;
	int v9;
	int v11;
	int v12;
LOC_28088:
	SetGlobal(43, 2);
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\030\\*");
	DisablePlayMagic();
	DownBrightness(12, 5);
	v1 = intvAttackerMajor;
	v2 = intvDefenderMajor;
	v3 = intvIsLeft;
	v11 = 0;
	if (!(Rand(0, 3) == 3)) goto LOC_28180;
	v11 = Rand(0, 90);
LOC_28180:
	v6 = 18;
	if (!(v3 == 1)) goto LOC_281D8;
	v5 = 0;
	v12 = (-1);
	goto LOC_281F8;
LOC_281D8:
	v5 = 128;
	v12 = 1;
LOC_281F8:
	switch (a0) { 
		case 0: goto LOC_28230;
		case 1: goto LOC_28254;
		case 2: goto LOC_28278;
	}
	goto LOC_2829C;
LOC_28230:
	v9 = (120 - v11);
	goto LOC_2829C;
LOC_28254:
	v9 = (210 - v11);
	goto LOC_2829C;
LOC_28278:
	v9 = (330 - v11);
	goto LOC_2829C;
LOC_2829C:
	SetOverwhelming(v1, 1);
	v7 = GetObjectScreenX(v1);
	v8 = GetObjectScreenY(v1);
	SetViewCamera((v7 - (v12 * 180)), (v8 - v9));
	AddAttackCounter(v1, 2);
	Delay(15);
	SetObjectAnimate(v1, 4096);
	Delay(20);
	SetOverwhelming(v1, 0);
	PlaySound(v1, "m001snd01", 255);
	if (!a0) goto LOC_2852C;
	asynccall CreateHalfMoonNew(intvAttackerMajor, v5, 64, 0, 1, 0);
	asynccall CreateHalfMoonNew(intvAttackerMajor, v5, 64, 200, 0, 0);
	asynccall CreateHalfMoonNew(intvAttackerMajor, v5, 64, 400, 0, 0);
	asynccall CreateHalfMoonNew(intvAttackerMajor, v5, 192, 200, 0, 1);
	asynccall CreateHalfMoonNew(intvAttackerMajor, v5, 192, 400, 0, 1);
	goto LOC_285F8;
LOC_2852C:
	asynccall CreateHalfMoonNew(intvAttackerMajor, v5, 64, 0, 1, 0);
	asynccall CreateHalfMoonNew(intvAttackerMajor, v5, 64, 200, 0, 0);
	asynccall CreateHalfMoonNew(intvAttackerMajor, v5, 192, 200, 0, 1);
LOC_285F8:
	Delay(8);
	Wait("CheckHalfMoonNew");
	Delay(60);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void FirePillarCallback(int a0, int a1) callsign 31001 {
	int v1;
	if (!(a1 == intvDefenderMajor)) goto LOC_28730;
	if (!GetGlobal(44)) goto LOC_28698;
	v1 = (intvMagicAttackValue / GetGlobal(44));
LOC_28698:
	Hurt(a0, a1, v1);
	if (!(v1 > 0)) goto LOC_28728;
	SetGlobal(44, (GetGlobal(44) * 2));
	HitGeneral(a0, a1, 11002, 1, 48, 0);
LOC_28728:
	goto LOC_2877C;
LOC_28730:
	Hurt(a0, a1, 0);
	asynccall FireMan(a1, 10015, 60);
LOC_2877C:
	return;
}

void CreateFirePillarSource(int a0, int a1, int a2) {
	int v1;
	int v2;
	v1 = CreateObjectByReference(a0, 41001, 0, 8);
	SetCoordinateByReference_Cylind(v1, a0, a2, a1, 8);
	PlaySound1("m031snd03", 255);
	Delay(8);
	v2 = a1;
LOC_2882C:
	if (!(v2 > 0)) goto LOC_288D4;
	SetCoordinateByReference_Cylind(v1, a0, a2, v2, 8);
	Delay(1);
	a2 = ((a2 + 1) & 255);
	v2 = (v2 - 4);
	goto LOC_2882C;
LOC_288D4:
	SetObjectFadeOut(v1, 16, 1);
	return;
}

void CreateFirePillarLight(int a0, int a1, int a2, int a3, int a4) {
	int v1;
	int v2;
	int v3;
LOC_28900:
	v1 = CreateObjectByReference(a0, 41009, 0, 0);
	SetCoordinateByReference_Cylind(v1, a0, a1, a2, 0);
	SetObjectFlags(v1, (33554432 + 16777216));
	SetCallbackProcedure(v1, 31001);
	SetObjectScale(v1, 32768, 24576);
	v3 = CreateObjectByReference(a0, 41010, 0, 0);
	SetCoordinateByReference_Cylind(v3, a0, a1, a2, 0);
	SetObjectScale(v3, 32768, 16384);
	v2 = 0;
LOC_28A74:
	if (!(v2 < a4)) goto LOC_28BFC;
	a1 = ((a1 + 4) & 255);
	a2 = (a2 + 6);
	Delay(1);
	SetCoordinateByReference_Cylind(v1, a0, a1, a2, 0);
	SetCoordinateByReference_Cylind(v3, a0, a1, a2, 0);
	SetCoordinateByReference_Cylind(a3, a0, a1, a2, 0);
	if (!(v2 == (a4 - 16))) goto LOC_28BEC;
	SetObjectFadeOut(v1, 16, 1);
	SetObjectFadeOut(v3, 16, 1);
LOC_28BEC:
	v2++;
	goto LOC_28A74;
LOC_28BFC:
	return;
}

void CreateFirePillarBomb(int a0, int a1, int a2, int a3) {
	int v1;
	a0 = CreateObjectByReference(a0, 2501, 0, 0);
	v1 = CreateObjectByReference(a0, 41008, 0, 0);
	SetCoordinateByReference_Cylind(v1, a0, a1, a2, 0);
	SetObjectScale(v1, 32768, 65536);
	SetObjectFadeOut(v1, a3, 1);
	CreateFirePillarLight(a0, a1, a2, v1, a3);
	FreeObjectByHandle(a0);
	return;
}

void CreateFirePillar(int a0, int a1) {
	int v1;
	int v2;
	int v3;
	int v4;
LOC_28D3C:
	v1 = CreateObjectRaw(a0, a1, 0, 0, 2501);
	v3 = Rand(0, 255);
	v2 = 0;
LOC_28DB4:
	if (!(v2 < 4)) goto LOC_28E34;
	asynccall CreateFirePillarSource(v1, 320, v3);
	v3 = ((v3 + 64) & 255);
	v2++;
	goto LOC_28DB4;
LOC_28E34:
	Wait("CreateFirePillarSource");
	Delay(8);
	asynccall LockTargetXY(a0, a1, 2501, 0, 60, 31001, 2, 3, (33554432 + 16777216));
	asynccall LockTargetXY((a0 + 60), a1, 2501, 60, 2, 31001, 2, 3, 33554432);
	asynccall LockTargetXY((a0 - 60), a1, 2501, 60, 2, 31001, 2, 3, 33554432);
	asynccall LockTargetXY(a0, (a1 + 50), 2501, 60, 2, 31001, 2, 3, 33554432);
	asynccall LockTargetXY(a0, (a1 + 50), 2501, 60, 2, 31001, 2, 3, 33554432);
	asynccall LockTargetXY((a0 + 60), (a1 + 50), 2501, 60, 2, 31001, 2, 3, 33554432);
	asynccall LockTargetXY((a0 + 60), (a1 - 50), 2501, 60, 2, 31001, 2, 3, 33554432);
	asynccall LockTargetXY((a0 - 60), (a1 + 50), 2501, 60, 2, 31001, 2, 3, 33554432);
	asynccall LockTargetXY((a0 - 60), (a1 - 50), 2501, 60, 2, 31001, 2, 3, 33554432);
	asynccall LockTargetXY((a0 + 120), a1, 2501, 60, 2, 31001, 2, 3, 33554432);
	asynccall LockTargetXY((a0 - 120), a1, 2501, 60, 2, 31001, 2, 3, 33554432);
	asynccall LockTargetXY(a0, (a1 + 100), 2501, 60, 2, 31001, 2, 3, 33554432);
	asynccall LockTargetXY(a0, (a1 + 100), 2501, 60, 2, 31001, 2, 3, 33554432);
	asynccall LockTargetXY((a0 + 120), (a1 + 100), 2501, 60, 2, 31001, 2, 3, 33554432);
	asynccall LockTargetXY((a0 + 120), (a1 - 100), 2501, 60, 2, 31001, 2, 3, 33554432);
	asynccall LockTargetXY((a0 - 120), (a1 + 100), 2501, 60, 2, 31001, 2, 3, 33554432);
	asynccall LockTargetXY((a0 - 120), (a1 - 100), 2501, 60, 2, 31001, 2, 3, 33554432);
	PlaySound1("m031snd01", 255);
	v2 = 0;
LOC_295C0:
	if (!(v2 < 4)) goto LOC_2966C;
	v4 = CreateObjectByReference(v1, (41002 + v2), 0, (-12));
	Delay(4);
	if (!(v2 < 3)) goto LOC_2965C;
	FreeObjectByHandle(v4);
LOC_2965C:
	v2++;
	goto LOC_295C0;
LOC_2966C:
	v2 = 65536;
LOC_2967C:
	if (!(v2 < 131072)) goto LOC_296EC;
	SetObjectScale(v4, 65536, v2);
	Delay(2);
	v2 = (v2 + 8192);
	goto LOC_2967C;
LOC_296EC:
	Delay(40);
	PlaySound1("m031snd02", 255);
	asynccall CreateFirePillarBomb(v4, 32, 10, 68);
	asynccall CreateFirePillarBomb(v4, 96, 10, 68);
	asynccall CreateFirePillarBomb(v4, 160, 10, 68);
	asynccall CreateFirePillarBomb(v4, 224, 10, 68);
	SetObjectScaleShrink(v4, 4096);
	SetObjectFadeOut(v4, 16, 1);
	FreeObjectByHandle(v1);
	return;
}

void FirePillar(int a0) {
	int v1;
	int v2;
	int v3;
LOC_29820:
	SetGlobal(44, 1);
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\031\\*");
	DisablePlayMagic();
	DownBrightness(8, 5);
	SetOverwhelming(intvAttackerMajor, 1);
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v1, (v2 - 120));
	AddAttackCounter(intvAttackerMajor, 2);
	Delay(15);
	SetObjectAnimate(intvAttackerMajor, 32768);
	Delay(20);
	SetOverwhelming(intvAttackerMajor, 0);
	v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
	if (!(v3 == 0)) goto LOC_299C0;
	v3 = intvDefenderMajor;
LOC_299C0:
	v1 = GetObjectScreenX(v3);
	v2 = GetObjectScreenY(v3);
	MoveCamera(v1, (v2 - 140), 20);
	CreateFirePillar(v1, v2);
	Delay(60);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void SparkCallback(int a0, int a1) callsign 32001 {
	int v1;
	int v2;
	v1 = GetObjectContext(a0, 0);
	if (!(v1 == 4660)) goto LOC_29AD4;
	v2 = 2;
	goto LOC_29AE4;
LOC_29AD4:
	v2 = 2;
LOC_29AE4:
	Hurt(a0, a1, v2);
	asynccall SmallFireBall3(a0, a1, 20, 0);
	return;
}

void DecreaseHP() {
	int v1;
	v1 = 1;
LOC_29B50:
	if (!(v1 <= (intvMagicAttackValue - 4))) goto LOC_29BB8;
	DoHarmToMajor(intvDefenderMajor, 0, 1);
	Delay(4);
	v1++;
	goto LOC_29B50;
LOC_29BB8:
	return;
}

void CreateSparkleCenter(int a0, int a1) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	Delay(a1);
	v1 = CreateObjectByReference(a0, 42001, 0, 0);
	SetObjectScale(v1, 73728, 73728);
	v4 = GetObjectScreenX(v1);
	v5 = GetObjectScreenY(v1);
	v6 = GetObjectScreenZ(v1);
	v3 = Rand(60, 80);
	v2 = 0;
LOC_29CB4:
	if (!(v2 < (v3 / 4))) goto LOC_29D6C;
	Delay(6);
	SetObjectCoordinate(v1, (v4 + Rand((-16), 16)), v5, (v6 + Rand((-16), 16)));
	v2++;
	goto LOC_29CB4;
LOC_29D6C:
	SetObjectCoordinate(v1, v4, v5, v6);
	SetObjectFadeOut(v1, 16, 1);
	return;
}

void CreateSparkle(int a0) {
	int v1;
	int v2;
	int v4;
LOC_29DC4:
	PlaySound1("m032snd01", 255);
	a0 = CreateObjectByReference(a0, 2501, 0, 0);
	v1 = CreateObjectByReference(a0, 42004, 0, 48);
	SetObjectScale(v1, 98304, 98304);
	SetCallbackProcedure(v1, 32001);
	SetObjectFlags(v1, (16777216 + 33554432));
	SetObjectContext(v1, 0, 4660);
	Delay(4);
	SetObjectFadeOut(v1, 8, 1);
	v1 = CreateObjectByReference(a0, 42002, 0, 48);
	SetObjectScale(v1, 98304, 98304);
	asynccall CreateSparkleCenter(v1, Rand(0, 1));
	asynccall CreateSparkleCenter(v1, Rand(20, 48));
	asynccall CreateSparkleCenter(v1, Rand(20, 48));
	asynccall CreateSparkleCenter(v1, Rand(20, 48));
	asynccall DecreaseHP();
	Wait("CreateSparkleCenter");
	PlaySound1("m032snd02", 255);
	v2 = CreateObjectByReference(v1, 42003, 0, 0);
	SetObjectScaleShrink(v2, (-4096));
	v4 = 0;
LOC_2A11C:
	if (!(v4 < 256)) goto LOC_2A1A8;
	asynccall LockByCenter(a0, 2501, v4, 52, 2, 33554432, 32001);
	v4 = (v4 + 32);
	goto LOC_2A11C;
LOC_2A1A8:
	v4 = 0;
LOC_2A1B8:
	if (!(v4 < 256)) goto LOC_2A244;
	asynccall LockByCenter(a0, 2501, v4, 110, 2, 33554432, 32001);
	v4 = (v4 + 32);
	goto LOC_2A1B8;
LOC_2A244:
	Delay(2);
	SetObjectFadeOut(v1, 16, 1);
LOC_2A274:
	if (!IsObjectExist(v1)) goto LOC_2A2A4;
	Delay(1);
	goto LOC_2A274;
LOC_2A2A4:
	Delay(8);
	SetObjectFadeOut(v2, 16, 1);
	FreeObjectByHandle(a0);
	return;
}

void Sparkle(int a0) {
	int v1;
	int v2;
	int v3;
LOC_2A2F0:
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\032\\*");
	DisablePlayMagic();
	DownBrightness(12, 5);
	SetOverwhelming(intvAttackerMajor, 1);
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v1, (v2 - 120));
	AddAttackCounter(intvAttackerMajor, 2);
	AddAttackCounter(intvDefenderMajor, 5);
	Delay(15);
	SetObjectAnimate(intvAttackerMajor, 32768);
	Delay(20);
	SetOverwhelming(intvAttackerMajor, 0);
	v3 = intvDefenderMajor;
	v1 = GetObjectScreenX(v3);
	v2 = GetObjectScreenY(v3);
	MoveCamera(v1, (v2 - 140), 40);
	CreateSparkle(v3);
	Delay(60);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void FireWorkCallback(int a0, int a1) callsign 33001 {
	int v1;
	if (!(a1 == intvDefenderMajor)) goto LOC_2A5EC;
	if (!GetGlobal(45)) goto LOC_2A554;
	v1 = (intvMagicAttackValue / GetGlobal(45));
LOC_2A554:
	Hurt(a0, a1, v1);
	if (!(v1 > 0)) goto LOC_2A5E4;
	SetGlobal(45, (GetGlobal(45) * 2));
	HitGeneral(a0, a1, 11002, 1, 48, 0);
LOC_2A5E4:
	goto LOC_2A638;
LOC_2A5EC:
	Hurt(a0, a1, 0);
	asynccall FireMan(a1, 10015, 60);
LOC_2A638:
	return;
}

void TraceFlash(int a0, int a1, int a2) {
	int v1;
	int v2;
	int v3;
LOC_2A648:
	Delay(1);
	v1 = GetObjectScreenZ(a0);
	if ((v1 > 0)) goto LOC_2A648;
	if (!a1) goto LOC_2A7EC;
	v2 = CreateObjectByReference(a0, 43003, 0, 0);
	if (!a2) goto LOC_2A724;
	SetObjectFlags(v2, (16777216 + 33554432));
	SetCallbackProcedure(v2, 33001);
LOC_2A724:
	SetObjectScale(v2, 45056, 45056);
	v3 = CreateObjectByReference(v2, 2501, 0, 0);
	MoveObject(v3, 0, 48, 0);
	SetObjectFlags(v3, (16777216 + 33554432));
	SetCallbackProcedure(v3, 33001);
LOC_2A7EC:
	FreeObjectByHandle(a0);
	Delay(26);
	FreeObjectByHandle(v3);
	if (!a1) goto LOC_2A854;
	SetObjectFadeOut(v2, 16, 1);
LOC_2A854:
	return;
}

void CreateCannonFlash(int a0, int a1) {
	int v1;
	int v2;
	int v3;
	v3 = 0;
LOC_2A874:
	if (!(v3 < 6)) goto LOC_2AA38;
	v2 = 0;
LOC_2A8A0:
	if (!(v2 < 256)) goto LOC_2AA1C;
	v1 = CreateObjectByReference(a0, 43004, 0, 8);
	SetCoordinateByReference_Cylind(v1, a0, v2, 64, 8);
	SetObjectSpeed_Sphere(v1, v2, 44, 12);
	SetObjectOpacity(v1, (16 - (v3 * 2)));
	SetObjectScaleShrink(v1, (-2048));
	SetObjectGravity(v1, a1);
	asynccall TraceFlash(v1, (v3 == 5), 1);
	v2 = (v2 + 32);
	goto LOC_2A8A0;
LOC_2AA1C:
	Delay(1);
	v3++;
	goto LOC_2A874;
LOC_2AA38:
	return;
}

void CannonExplode(int a0, int a1) {
	int v1;
	v1 = CreateObjectRaw(a0, a1, 0, 0, 43003);
	SetObjectFlags(v1, (16777216 + 33554432));
	SetCallbackProcedure(v1, 33001);
	Delay(4);
	asynccall CreateCannonFlash(v1, Rand(30720, 36864));
	Delay(26);
	SetObjectFadeOut(v1, 16, 1);
	return;
}

void TraceCannon(int a0) {
	int v1;
	int v2;
	int v3;
LOC_2AB54:
	Delay(1);
	v3 = GetObjectScreenZ(a0);
	if ((v3 > 0)) goto LOC_2AB54;
	v1 = GetObjectScreenX(a0);
	v2 = GetObjectScreenY(a0);
	asynccall CannonExplode(v1, v2);
	FreeObjectByHandle(a0);
	return;
}

void ShootCannon() callsign 33002 {
	int v1;
	int v2;
	int v4;
	v2 = GetScriptLinkedObject();
	v4 = GetObjectDir(v2);
	v1 = CreateObjectByReference(v2, 43002, v4, 0);
	SetObjectScale(v1, 61440, 61440);
	SetCoordinateByReference_Cylind(v1, v2, v4, 45, 85);
	SetObjectSpeed_Cylind(v1, 30, 30);
	Delay(2);
	Delay(32);
	FreeObjectByHandle(v1);
	return;
}

void TraceCannonShadow(int a0, int a1, int a2, int a3) {
	int v1;
LOC_2AD3C:
	if (!IsObjectExist(a1)) goto LOC_2AE48;
	v1 = GetObjectScreenZ(a1);
	if (!(v1 <= 0)) goto LOC_2AD98;
	goto LOC_2AE48;
LOC_2AD98:
	Delay(1);
	if (!(a2 < 65536)) goto LOC_2AE00;
	a2 = (a2 + 8192);
	SetObjectScale(a0, a2, a2);
LOC_2AE00:
	if (!(a3 < 16)) goto LOC_2AE40;
	a3++;
	SetObjectOpacity(a0, a3);
LOC_2AE40:
	goto LOC_2AD3C;
LOC_2AE48:
	FreeObjectByHandle(a0);
	return;
}

void FlyCannonLB1(int a0, int a1, int a2) {
	int v1;
	Delay(a2);
	v1 = CreateObjectByReference(intvAttackerMajor, 43006, 0, 0);
	SetObjectCoordinate(v1, (a0 + 260), a1, 260);
	SetObjectSpeed_Sphere(v1, 128, (-32), 32);
	asynccall TraceCannon(v1);
	return;
}

void FlyCannonRB1(int a0, int a1, int a2) {
	int v1;
	Delay(a2);
	v1 = CreateObjectByReference(intvAttackerMajor, 43006, 128, 0);
	SetObjectCoordinate(v1, (a0 - 260), a1, 260);
	SetObjectSpeed_Sphere(v1, 0, (-32), 32);
	asynccall TraceCannon(v1);
	return;
}

void FlyCannonLB2(int a0, int a1, int a2) {
	int v1;
	int v2;
	v2 = GetObjectScreenX(a0);
	a1 = GetObjectScreenY(a0);
	Delay(a2);
	v1 = CreateObjectByReference(intvAttackerMajor, 43006, 0, 0);
	if (!(IsObjectExist(a0) == 0)) goto LOC_2B120;
	SetObjectCoordinate(v1, ((v2 + 260) + 24), a1, 260);
	goto LOC_2B18C;
LOC_2B120:
	SetCoordinateByReference_Cylind(v1, a0, 0, 0, 0);
	MoveObject(v1, (260 + 24), 0, 260);
LOC_2B18C:
	SetObjectSpeed_Sphere(v1, 128, (-32), 32);
	asynccall TraceCannon(v1);
	return;
}

void FlyCannonRB2(int a0, int a1, int a2) {
	int v1;
	int v2;
	v2 = GetObjectScreenX(a0);
	a1 = GetObjectScreenY(a0);
	Delay(a2);
	v1 = CreateObjectByReference(intvAttackerMajor, 43006, 128, 0);
	if (!(IsObjectExist(a0) == 0)) goto LOC_2B2E4;
	SetObjectCoordinate(v1, ((v2 - 260) - 24), a1, 260);
	goto LOC_2B354;
LOC_2B2E4:
	SetCoordinateByReference_Cylind(v1, a0, 0, 0, 0);
	MoveObject(v1, ((-260) - 24), 0, 260);
LOC_2B354:
	SetObjectSpeed_Sphere(v1, 0, (-32), 32);
	asynccall TraceCannon(v1);
	return;
}

void ProduceCannonEmitter(int a0, int a1, int a2, int a3) {
	int v1;
	Delay(a2);
	if (!(intvIsLeft == 1)) goto LOC_2B424;
	v1 = CreateObjectByReference(intvAttackerMajor, 43005, a3, 0);
	goto LOC_2B458;
LOC_2B424:
	v1 = CreateObjectByReference(intvAttackerMajor, 43001, a3, 0);
LOC_2B458:
	SetObjectCoordinate(v1, a0, a1, 0);
	SetObjectContext(v1, 0, 0);
	return;
}

void FireWork(int a0) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	int v7;
	int v8;
	int v9;
	int v10;
	int v11;
	int v12;
	int v13;
	int v14;
	int v15;
LOC_2B4B0:
	SetGlobal(45, 1);
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\033\\*");
	DisablePlayMagic();
	DownBrightness(12, 5);
	SetOverwhelming(intvAttackerMajor, 1);
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v1, (v2 - 120));
	AddAttackCounter(intvAttackerMajor, 2);
	Delay(15);
	SetObjectAnimate(intvAttackerMajor, 32768);
	Delay(20);
	SetOverwhelming(intvAttackerMajor, 0);
	if (!(intvIsLeft == 1)) goto LOC_2B630;
	v3 = 0;
	goto LOC_2B640;
LOC_2B630:
	v3 = 128;
LOC_2B640:
	switch (a0) { 
		case 0: goto LOC_2B66C;
		case 1: goto LOC_2B6A4;
	}
	goto LOC_2B6DC;
LOC_2B66C:
	v6 = 2;
	v4 = 4;
	v8 = 3;
	goto LOC_2B70C;
LOC_2B6A4:
	v6 = 1;
	v4 = 4;
	v8 = 5;
	goto LOC_2B70C;
LOC_2B6DC:
	v6 = 2;
	v4 = 4;
	v8 = 7;
LOC_2B70C:
	v7 = Rand(1, 30);
	asynccall ProduceCannonEmitter((((6576 + (96 * 7)) * (intvIsLeft == 0)) + ((240 - (96 * 7)) * (intvIsLeft != 0))), ((180 + ((v4 * 72) * 2)) - (72 * 2)), v7, v3);
	v4 = (v4 - v6);
	if ((v4 >= 0)) goto LOC_2B70C;
	if (!(a0 >= 2)) goto LOC_2B9D8;
	v6 = 2;
	v4 = 6;
LOC_2B878:
	v7 = Rand(1, 30);
	asynccall ProduceCannonEmitter(((((6576 + (96 * 7)) + (96 * 2)) * (intvIsLeft == 0)) + (((240 - (96 * 7)) - (96 * 2)) * (intvIsLeft != 0))), ((180 + ((v4 * 72) * 2)) - (72 * 1)), v7, v3);
	v4 = (v4 - v6);
	if ((v4 > 0)) goto LOC_2B878;
LOC_2B9D8:
	if (!(intvIsLeft == 1)) goto LOC_2BA40;
	MoveCamera(((240 - (96 * 4)) - 96), 0, 40);
	goto LOC_2BA90;
LOC_2BA40:
	MoveCamera((((6576 + (96 * 4)) - 1) + 96), 0, 40);
LOC_2BA90:
	Delay(80);
	v10 = GetSoldierCount((intvIsLeft ^ 1));
	v11 = 0;
	if (!(v10 < v8)) goto LOC_2BB0C;
	v11 = (v8 - v10);
LOC_2BB0C:
	if (!((v8 - v11) > 0)) goto LOC_2BD54;
	v5 = 0;
	v4 = (v8 - v11);
LOC_2BB60:
	v9 = GetRandomSoldierFromAlive((intvIsLeft ^ 1));
	v14 = GetObjectScreenX(v9);
	v15 = GetObjectScreenY(v9);
	if (!(v5 == 0)) goto LOC_2BC14;
	v5 = 1;
	v12 = v14;
	v13 = v14;
	goto LOC_2BC6C;
LOC_2BC14:
	if (!(v12 < v14)) goto LOC_2BC40;
	v12 = v14;
LOC_2BC40:
	if (!(v13 > v14)) goto LOC_2BC6C;
	v13 = v14;
LOC_2BC6C:
	if (!(intvIsLeft == 1)) goto LOC_2BCDC;
	asynccall FlyCannonRB2(v9, v15, (80 + Rand(1, 30)));
	goto LOC_2BD28;
LOC_2BCDC:
	asynccall FlyCannonLB2(v9, v15, (80 + Rand(1, 30)));
LOC_2BD28:
	v4--;
	if ((v4 > 0)) goto LOC_2BB60;
	goto LOC_2BD80;
LOC_2BD54:
	v12 = GetObjectScreenX(intvDefenderMajor);
	v13 = v12;
LOC_2BD80:
	if (!(v12 == v13)) goto LOC_2BDD4;
	v12 = (v12 + 512);
	v13 = (v13 - 512);
LOC_2BDD4:
	v9 = GetMostImportantSoldier((intvIsLeft ^ 1));
	if (!(v9 == 0)) goto LOC_2BE28;
	v9 = intvDefenderMajor;
LOC_2BE28:
	v12 = GetObjectScreenX(v9);
	v4 = v11;
LOC_2BE54:
	v14 = (v12 + Rand((-320), 320));
	v15 = Rand(150, 650);
	if (!(intvIsLeft == 1)) goto LOC_2BF1C;
	asynccall FlyCannonRB1(v14, v15, (80 + Rand(1, 30)));
	goto LOC_2BF68;
LOC_2BF1C:
	asynccall FlyCannonLB1(v14, v15, (80 + Rand(1, 30)));
LOC_2BF68:
	v4--;
	if ((v4 > 0)) goto LOC_2BE54;
	v9 = GetMostImportantSoldier((intvIsLeft ^ 1));
	if (!(v9 == 0)) goto LOC_2BFE0;
	v9 = intvDefenderMajor;
LOC_2BFE0:
	v14 = GetObjectScreenX(v9);
	v15 = GetObjectScreenY(v9);
	MoveCamera(v14, 0, 80);
	Wait("TraceFlash");
	Delay(180);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void PowderCallback(int a0, int a1) callsign 34001 {
	int v1;
	if (!((a1 == intvDefenderMajor) || (a1 == intvAttackerMajor))) goto LOC_2C17C;
	if (!GetGlobal(46)) goto LOC_2C0E4;
	v1 = (intvMagicAttackValue / GetGlobal(46));
LOC_2C0E4:
	Hurt(a0, a1, v1);
	if (!(v1 > 0)) goto LOC_2C174;
	SetGlobal(46, (GetGlobal(46) * 2));
	HitGeneral(a0, a1, 11002, 1, 48, 0);
LOC_2C174:
	goto LOC_2C1C8;
LOC_2C17C:
	Hurt(a0, a1, 0);
	asynccall FireMan(a1, 10015, 60);
LOC_2C1C8:
	return;
}

void CreatePowerExplode(int a0, int a1, int a2, int a3, int a4, int a5) {
	int v1;
	v1 = CreateObjectRaw(a0, a1, a2, a3, 44003);
	SetObjectSpeed_Sphere(v1, a3, (2 * Rand(8, 32)), a4);
	SetObjectScale(v1, 32768, 32768);
	SetObjectScaleShrink(v1, (-512));
	Delay(a5);
	SetObjectFadeOut(v1, 16, 1);
	return;
}

void CreatePowerSmoke(int a0, int a1, int a2, int a3, int a4, int a5, int a6) {
	int v1;
	if (!a6) goto LOC_2C384;
	v1 = CreateObjectRaw(a0, a1, a2, 0, 44004);
	SetObjectSpeed_Sphere(v1, a3, (2 * Rand(4, 28)), a4);
	goto LOC_2C40C;
LOC_2C384:
	v1 = CreateObjectRaw(a0, a1, a2, 0, 44002);
	SetObjectSpeed_Sphere(v1, a3, (2 * Rand(0, 32)), a4);
LOC_2C40C:
	Delay((a5 - 4));
	SetObjectScaleShrink(v1, (-1024));
	Delay(4);
	SetObjectFadeOut(v1, 16, 1);
	return;
}

void CreatePowderBreak(int a0, int a1) {
	int v1;
	int v2;
	Delay(a1);
	v2 = Rand(0, 255);
	v1 = CreateObjectByReference(a0, Rand(44005, 44011), v2, 20);
	SetObjectGravity(v1, 18432);
	SetCoordinateByReference_Cylind(v1, a0, v2, 4, Rand(20, 30));
	SetObjectSpeed_Sphere(v1, v2, Rand(36, 54), Rand(6, 10));
	PlaySound(intvDefenderMajor, "m034snd01", 255);
	Delay(8);
	SetObjectFadeOut(v1, 16, 2);
	return;
}

void CreatePower(int a0, int a1, int a2) {
	int v1;
	int v2;
	v1 = CreateObjectRaw(a0, a1, 0, 0, 44001);
	SetObjectFlags(v1, 262144);
	v2 = 0;
LOC_2C680:
	if (!(v2 <= 16)) goto LOC_2C6D4;
	SetObjectOpacity(v1, v2);
	Delay(1);
	v2++;
	goto LOC_2C680;
LOC_2C6D4:
	Delay(a2);
	SetObjectFlags(v1, 128);
	v2 = 0;
LOC_2C70C:
	if (!(v2 < 10)) goto LOC_2C778;
	asynccall CreatePowderBreak(v1, Rand(1, 3));
	v2++;
	goto LOC_2C70C;
LOC_2C778:
	asynccall LockTargetXY(a0, a1, 2501, 0, 2, 34001, 2, 3, 251658240);
	asynccall LockTargetXY((a0 + 60), a1, 2501, 0, 2, 34001, 2, 3, 251658240);
	asynccall LockTargetXY((a0 - 60), a1, 2501, 0, 2, 34001, 2, 3, 251658240);
	asynccall LockTargetXY(a0, (a1 + 48), 2501, 0, 2, 34001, 2, 3, 251658240);
	asynccall LockTargetXY(a0, (a1 + 48), 2501, 0, 2, 34001, 2, 3, 251658240);
	asynccall LockTargetXY((a0 + 60), (a1 + 48), 2501, 0, 2, 34001, 2, 3, 251658240);
	asynccall LockTargetXY((a0 + 60), (a1 - 48), 2501, 0, 2, 34001, 2, 3, 251658240);
	asynccall LockTargetXY((a0 - 60), (a1 + 48), 2501, 0, 2, 34001, 2, 3, 251658240);
	asynccall LockTargetXY((a0 - 60), (a1 - 48), 2501, 0, 2, 34001, 2, 3, 251658240);
	v2 = 0;
LOC_2CB54:
	if (!(v2 < 12)) goto LOC_2CD80;
	asynccall CreatePowerSmoke(a0, a1, Rand(20, 40), Rand(0, 255), Rand(2, 3), 16, 0);
	if (!((v2 & 1) == 0)) goto LOC_2CCA8;
	asynccall CreatePowerSmoke(a0, a1, Rand(20, 40), Rand(0, 255), Rand(2, 3), 18, 1);
LOC_2CCA8:
	Delay(1);
	if (!(v2 < 8)) goto LOC_2CD50;
	asynccall CreatePowerExplode(a0, a1, Rand(20, 40), Rand(0, 255), Rand(3, 4), 12);
LOC_2CD50:
	Delay(Rand(1, 2));
	v2++;
	goto LOC_2CB54;
LOC_2CD80:
	FreeObjectByHandle(v1);
	return;
}

void Powder(int a0) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
LOC_2CD9C:
	SetGlobal(46, 1);
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\034\\*");
	DisablePlayMagic();
	DownBrightness(12, 5);
	SetOverwhelming(intvAttackerMajor, 1);
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v1, (v2 - 120));
	AddAttackCounter(intvAttackerMajor, 2);
	Delay(15);
	SetObjectAnimate(intvAttackerMajor, 32768);
	Delay(20);
	SetOverwhelming(intvAttackerMajor, 0);
	switch (a0) { 
		case 0: goto LOC_2CF14;
		case 1: goto LOC_2CF2C;
	}
	goto LOC_2CF44;
LOC_2CF14:
	v5 = 2;
	goto LOC_2CF54;
LOC_2CF2C:
	v5 = 5;
	goto LOC_2CF54;
LOC_2CF44:
	v5 = 7;
LOC_2CF54:
	v4 = 0;
LOC_2CF64:
	if (!(v4 < v5)) goto LOC_2D314;
	v3 = GetRandomSoldierFromAlive((intvIsLeft ^ 1));
	if (!v3) goto LOC_2D040;
	v1 = (GetObjectScreenX(v3) + Rand((-8), 8));
	v2 = (GetObjectScreenY(v3) + Rand((-8), 8));
	goto LOC_2D0C0;
LOC_2D040:
	v1 = (GetObjectScreenX(intvDefenderMajor) + Rand((-250), 250));
	v2 = (GetObjectScreenY(intvDefenderMajor) + Rand((-200), 200));
LOC_2D0C0:
	if (!(v4 == 0)) goto LOC_2D114;
	asynccall MoveCamera(v1, (v2 - 140), 40);
LOC_2D114:
	asynccall CreatePower(v1, v2, Rand(40, 150));
	if (!Rand(0, 1)) goto LOC_2D1B0;
	v1 = (v1 + Rand(150, 200));
	goto LOC_2D1E0;
LOC_2D1B0:
	v1 = (v1 - Rand(150, 200));
LOC_2D1E0:
	if (!Rand(0, 1)) goto LOC_2D23C;
	v2 = (v2 + Rand(120, 170));
	goto LOC_2D26C;
LOC_2D23C:
	v2 = (v2 - Rand(120, 170));
LOC_2D26C:
	if (!(v2 > 800)) goto LOC_2D298;
	v2 = 800;
LOC_2D298:
	if (!(v2 < 200)) goto LOC_2D2C4;
	v2 = 200;
LOC_2D2C4:
	asynccall CreatePower(v1, v2, Rand(80, 120));
	v4++;
	goto LOC_2CF64;
LOC_2D314:
	Wait("CreatePower");
	Wait("CreatePowerSmoke");
	Delay(60);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void SpearCallback(int a0, int a1) callsign 35001 {
	int v1;
	if (!(a1 == intvDefenderMajor)) goto LOC_2D414;
	if (!GetGlobal(47)) goto LOC_2D3B4;
	v1 = (intvMagicAttackValue / GetGlobal(47));
LOC_2D3B4:
	Hurt(a0, a1, v1);
	if (!(v1 > 0)) goto LOC_2D40C;
	SetGlobal(47, (GetGlobal(47) * 2));
LOC_2D40C:
	goto LOC_2D434;
LOC_2D414:
	Hurt(a0, a1, 0);
LOC_2D434:
	asynccall Blood(a1, 16);
	return;
}

void CreateSpear(int a0, int a1, int a2) {
	int v1;
	Delay(a2);
	v1 = CreateObjectRaw(a0, a1, 0, 0, 45001);
	SetObjectFlags(v1, (33554432 + 16777216));
	SetCallbackProcedure(v1, 35001);
	Delay(20);
	FreeObjectByHandle(v1);
	v1 = CreateObjectRaw(a0, a1, 0, 0, 45002);
	Delay(8);
	SetObjectFadeOut(v1, 16, 1);
	return;
}

void CreateSpears(int a0, int a1) {
	int v1;
	int v2;
	int v3;
	int v4;
	v3 = (a0 + (36 * 3));
	v4 = (a1 + (42 * 2));
	v1 = (a0 - (36 * 3));
LOC_2D4E8:
	if (!(v1 <= v3)) goto LOC_2D5D0;
	v2 = (a1 - (42 * 2));
LOC_2D52C:
	if (!(v2 <= v4)) goto LOC_2D5AC;
	asynccall CreateSpear(v1, v2, Rand(0, 2));
	v2 = (v2 + 42);
	goto LOC_2D52C;
LOC_2D5AC:
	v1 = (v1 + 36);
	goto LOC_2D4E8;
LOC_2D5D0:
	return;
}

void Spear(int a0) {
	int v1;
	int v2;
	int v3;
	int v4;
LOC_2D700:
	SetGlobal(47, 1);
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\035\\*");
	DisablePlayMagic();
	DownBrightness(12, 5);
	SetOverwhelming(intvAttackerMajor, 1);
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v1, (v2 - 120));
	AddAttackCounter(intvAttackerMajor, 2);
	Delay(15);
	SetObjectAnimate(intvAttackerMajor, 32768);
	Delay(20);
	SetOverwhelming(intvAttackerMajor, 0);
	v4 = 0;
LOC_2D85C:
	if (!(v4 < (a0 + 1))) goto LOC_2DBD0;
	v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
	if (!(v3 == 0)) goto LOC_2D9C0;
	v3 = intvDefenderMajor;
	v1 = (GetObjectScreenX(v3) + CalcCos(Rand(0, 255), (10 * Rand(24, 32))));
	v2 = (GetObjectScreenY(v3) + CalcSin(Rand(0, 255), (10 * Rand(20, 30))));
	goto LOC_2D9F8;
LOC_2D9C0:
	v1 = GetObjectScreenX(v3);
	v2 = GetObjectScreenY(v3);
LOC_2D9F8:
	MoveCamera(v1, (v2 - 120), 20);
	PlaySound1("m035snd01", 230);
	asynccall CreateSpears(v1, v2);
	Delay(Rand(8, 16));
	v1 = (GetObjectScreenX(v3) + CalcCos(Rand(0, 255), (10 * Rand(24, 32))));
	v2 = (GetObjectScreenY(v3) + CalcSin(Rand(0, 255), (10 * Rand(20, 30))));
	PlaySound1("m035snd01", 180);
	asynccall CreateSpears(v1, v2);
	Delay(12);
	v4++;
	goto LOC_2D85C;
LOC_2DBD0:
	Wait("CreateSpear");
	Delay(60);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void SlashCallback(int a0, int a1) callsign 36001 {
	int v1;
	if (!(a1 == intvDefenderMajor)) goto LOC_2DCFC;
	if (!GetGlobal(48)) goto LOC_2DC64;
	v1 = (intvMagicAttackValue / GetGlobal(48));
LOC_2DC64:
	Hurt(a0, a1, v1);
	if (!(v1 > 0)) goto LOC_2DCF4;
	SetGlobal(48, (GetGlobal(48) * 2));
	HitGeneral(a0, a1, 13008, 2, 60, 0);
LOC_2DCF4:
	goto LOC_2DD48;
LOC_2DCFC:
	Hurt(a0, a1, 0);
	asynccall SmallFireBall3(a0, a1, 8, 0);
LOC_2DD48:
	return;
}

void CreateSlashByTarget(int a0, int a1, int a2, int a3) {
	int v1;
	int v2;
	int v3;
	v3 = 4096;
	v1 = CreateObjectByReference(a0, (46001 + a2), 128, a1);
	SetCoordinateByReference_Cylind(v1, a0, (192 - (a2 * 16)), Rand(80, 180), a1);
	SetObjectFlags(v1, 262144);
	v2 = 0;
LOC_2DE34:
	if (!(v2 <= 32)) goto LOC_2DEF0;
	if (!(v3 < 49152)) goto LOC_2DE90;
	SetObjectScale(v1, v3, v3);
LOC_2DE90:
	SetObjectOpacity(v1, (v2 / 2));
	Delay(1);
	v3 = (v3 + 2048);
	v2++;
	goto LOC_2DE34;
LOC_2DEF0:
	ClearObjectFlags(v1, 262144);
	Delay(24);
	PlaySound1("m036snd02", 255);
	SetObjectSpeed_Sphere(v1, (64 - (a2 * 16)), 0, 12);
	SetObjectFlags(v1, 50331648);
	SetCallbackProcedure(v1, 36001);
	SetObjectFadeOut(v1, 48, 1);
	Delay(a3);
	ClearObjectFlags(v1, 50331648);
	return;
}

void CreateSlash(int a0, int a1, int a2, int a3, int a4, int a5) {
	int v1;
	int v2;
	int v3;
	v3 = 4096;
	v1 = CreateObjectRaw(a0, a1, a2, 128, (46001 + a3));
	SetObjectFlags(v1, 262144);
	v2 = 0;
LOC_2E090:
	if (!(v2 <= 32)) goto LOC_2E14C;
	if (!(v3 < 49152)) goto LOC_2E0EC;
	SetObjectScale(v1, v3, v3);
LOC_2E0EC:
	SetObjectOpacity(v1, (v2 / 2));
	Delay(1);
	v3 = (v3 + 2048);
	v2++;
	goto LOC_2E090;
LOC_2E14C:
	ClearObjectFlags(v1, 262144);
	Delay(24);
	PlaySound1("m036snd02", 255);
	SetObjectSpeed_Sphere(v1, (64 - (a3 * 16)), 0, 12);
	if (!a5) goto LOC_2E21C;
	SetObjectFlags(v1, 50331648);
	SetCallbackProcedure(v1, 36001);
LOC_2E21C:
	SetObjectFadeOut(v1, 48, 1);
	Delay(a4);
	ClearObjectFlags(v1, 50331648);
	return;
}

void Slash(int a0) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	int v7;
	int v8;
LOC_2E270:
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\036\\*");
	DisablePlayMagic();
	DownBrightness(8, 5);
	SetOverwhelming(intvAttackerMajor, 1);
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v1, (v2 - 120));
	AddAttackCounter(intvAttackerMajor, 2);
	Delay(15);
	SetObjectAnimate(intvAttackerMajor, 32768);
	Delay(20);
	SetOverwhelming(intvAttackerMajor, 0);
	SetGlobal(48, 1);
	PlaySound1("m036snd01", 255);
	if (!(a0 == 0)) goto LOC_2E800;
	v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
	if (!(v3 == 0)) goto LOC_2E448;
	v3 = intvDefenderMajor;
LOC_2E448:
	v1 = GetObjectScreenX(v3);
	v2 = GetObjectScreenY(v3);
	v7 = GetObjectXY(v3);
	v8 = GetObjectBattleY(v3);
	MoveCamera(v1, (v2 - 140), 20);
	asynccall CreateSlashByTarget(v3, 40, Rand(0, 15), 0);
	v4 = 0;
LOC_2E534:
	if (!(v4 < 4)) goto LOC_2E71C;
	v6 = GetSoldierCountInRect((intvIsLeft ^ 1), v7, v8, 6, 5);
	if (!(v6 > 2)) goto LOC_2E66C;
	v3 = GetNthSoldierInRect((intvIsLeft ^ 1), v7, v8, 6, 5, Rand(0, (v6 - 1)));
	asynccall CreateSlashByTarget(v3, 40, Rand(0, 15), 12);
	goto LOC_2E70C;
LOC_2E66C:
	asynccall CreateSlash((v1 + Rand((-250), 250)), (v2 + Rand((-180), 180)), 40, Rand(0, 15), 12, 1);
LOC_2E70C:
	v4++;
	goto LOC_2E534;
LOC_2E71C:
	v4 = 0;
LOC_2E72C:
	if (!(v4 < 3)) goto LOC_2E7F8;
	asynccall CreateSlash((v1 + Rand((-250), 250)), (v2 + Rand((-180), 180)), 40, Rand(0, 15), 12, 0);
	v4++;
	goto LOC_2E72C;
LOC_2E7F8:
	goto LOC_2ECAC;
LOC_2E800:
	v5 = 0;
LOC_2E810:
	if (!(v5 < 3)) goto LOC_2ECAC;
	if (!(v5 == 0)) goto LOC_2E878;
	v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
	goto LOC_2E8A0;
LOC_2E878:
	v3 = GetRandomSoldierFromAlive((intvIsLeft ^ 1));
LOC_2E8A0:
	if (!(v3 == 0)) goto LOC_2E8CC;
	v3 = intvDefenderMajor;
LOC_2E8CC:
	v1 = GetObjectScreenX(v3);
	v2 = GetObjectScreenY(v3);
	v7 = GetObjectXY(v3);
	v8 = GetObjectBattleY(v3);
	MoveCamera(v1, (v2 - 140), 20);
	asynccall CreateSlashByTarget(v3, 40, Rand(0, 15), 0);
	v4 = 0;
LOC_2E9B8:
	if (!(v4 < 5)) goto LOC_2EBA0;
	v6 = GetSoldierCountInRect((intvIsLeft ^ 1), v7, v8, 6, 5);
	if (!(v6 > 2)) goto LOC_2EAF0;
	v3 = GetNthSoldierInRect((intvIsLeft ^ 1), v7, v8, 6, 5, Rand(0, (v6 - 1)));
	asynccall CreateSlashByTarget(v3, 40, Rand(0, 15), 24);
	goto LOC_2EB90;
LOC_2EAF0:
	asynccall CreateSlash((v1 + Rand((-250), 250)), (v2 + Rand((-180), 180)), 40, Rand(0, 15), 24, 1);
LOC_2EB90:
	v4++;
	goto LOC_2E9B8;
LOC_2EBA0:
	v4 = 0;
LOC_2EBB0:
	if (!(v4 < 4)) goto LOC_2EC7C;
	asynccall CreateSlash((v1 + Rand((-250), 250)), (v2 + Rand((-180), 180)), 40, Rand(0, 15), 12, 0);
	v4++;
	goto LOC_2EBB0;
LOC_2EC7C:
	Delay(Rand(56, 80));
	v5++;
	goto LOC_2E810;
LOC_2ECAC:
	Wait("CreateSlash");
	Delay(78);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void WallCallback(int a0, int a1) callsign 37001 {
	int v1;
	if (!(a1 == intvDefenderMajor)) goto LOC_2EDA0;
	if (!GetGlobal(50)) goto LOC_2ED40;
	v1 = (intvMagicAttackValue / GetGlobal(50));
LOC_2ED40:
	Hurt(a0, a1, v1);
	if (!(v1 > 0)) goto LOC_2ED98;
	SetGlobal(50, (GetGlobal(50) * 2));
LOC_2ED98:
	goto LOC_2EDEC;
LOC_2EDA0:
	Hurt(a0, a1, 0);
	asynccall FireMan(a1, 10015, 60);
LOC_2EDEC:
	return;
}

void CreateFireWallAttack(int a0, int a1) {
	int v1;
	int v2;
	int v3[12];
	int v15;
	v2 = 0;
	v1 = 0;
LOC_2EE1C:
	if (!(v1 < 10)) goto LOC_2EE60;
	v3[v1] = 0;
	v1++;
	goto LOC_2EE1C;
LOC_2EE60:
	if (!(a1 == 0)) goto LOC_2EE94;
	v15 = 862;
	goto LOC_2EEA4;
LOC_2EE94:
	v15 = 934;
LOC_2EEA4:
	v1 = v15;
LOC_2EEB4:
	if (!(v1 >= 100)) goto LOC_2EFA0;
	v3[v2] = CreateObjectRaw(a0, v1, 0, 0, 2501);
	SetObjectFlags(v3[v2], (33554432 + 16777216));
	SetCallbackProcedure(v3[v2], 37001);
	v2++;
	v1 = (v1 - (72 * 2));
	goto LOC_2EEB4;
LOC_2EFA0:
	if (!(GetGlobal(49) == 0)) goto LOC_2EFD0;
	Delay(1);
	goto LOC_2EFA0;
LOC_2EFD0:
	v1 = 0;
LOC_2EFE0:
	if (!(v1 < v2)) goto LOC_2F028;
	FreeObjectByHandle(v3[v1]);
	v1++;
	goto LOC_2EFE0;
LOC_2F028:
	return;
}

void CreateFireWall(int a0, int a1, int a2) {
	int v1;
	int v2;
	int v4;
	v1 = CreateObjectRaw(a0, a1, 0, 0, 2501);
	asynccall CreateFireWallAttack(a0, a2);
	v2 = 0;
LOC_2F0B0:
	if (!(v2 < 4)) goto LOC_2F17C;
	v4 = CreateObjectByReference(v1, (47001 + v2), 0, 34);
	SetObjectScale(v4, 32768, 65536);
	Delay(4);
	if (!(v2 < 3)) goto LOC_2F16C;
	FreeObjectByHandle(v4);
LOC_2F16C:
	v2++;
	goto LOC_2F0B0;
LOC_2F17C:
	v2 = 65536;
LOC_2F18C:
	if (!(v2 < 131072)) goto LOC_2F1FC;
	SetObjectScale(v4, 32768, v2);
	Delay(2);
	v2 = (v2 + 8192);
	goto LOC_2F18C;
LOC_2F1FC:
	Delay(280);
	SetGlobal(49, 1);
	SetObjectScaleShrink(v4, 4096);
	SetObjectFadeOut(v4, 16, 1);
	FreeObjectByHandle(v1);
	return;
}

void Wall(int a0) {
	int v1;
	int v2;
	int v3;
	int v4;
LOC_2F274:
	SetGlobal(50, 1);
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\037\\*");
	DisablePlayMagic();
	DownBrightness(8, 5);
	SetOverwhelming(intvAttackerMajor, 1);
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v1, (v2 - 120));
	AddAttackCounter(intvAttackerMajor, 2);
	Delay(15);
	SetObjectAnimate(intvAttackerMajor, 32768);
	Delay(20);
	SetOverwhelming(intvAttackerMajor, 0);
	v4 = GetSoldierMaxX2(intvIsLeft);
	SetGlobal(49, 0);
	if (!(v4 == (-1))) goto LOC_2F464;
	if (!(intvIsLeft == 1)) goto LOC_2F444;
	CastFail(intvAttackerMajor, 0);
	goto LOC_2F45C;
LOC_2F444:
	CastFail(intvAttackerMajor, 128);
LOC_2F45C:
	goto LOC_2FA40;
LOC_2F464:
	if (!(a0 == 0)) goto LOC_2F6E8;
	PlaySound1("m037snd01", 255);
	if (!(intvIsLeft == 1)) goto LOC_2F5D0;
	MoveCamera((v4 + ((96 * 5) / 2)), 422, 20);
	Delay(2);
	v3 = 900;
LOC_2F518:
	if (!(v3 >= 200)) goto LOC_2F5C8;
	asynccall CreateFireWall((v4 + ((96 * 3) / 2)), v3, a0);
	Delay(Rand(0, 1));
	v3 = (v3 - 120);
	goto LOC_2F518;
LOC_2F5C8:
	goto LOC_2F6E0;
LOC_2F5D0:
	MoveCamera((v4 - ((96 * 5) / 2)), 422, 20);
	Delay(2);
	v3 = 862;
LOC_2F630:
	if (!(v3 >= 200)) goto LOC_2F6E0;
	asynccall CreateFireWall((v4 - ((96 * 3) / 2)), v3, a0);
	Delay(Rand(0, 1));
	v3 = (v3 - 120);
	goto LOC_2F630;
LOC_2F6E0:
	goto LOC_2FA40;
LOC_2F6E8:
	PlaySound1("m037snd01", 255);
	if (!(intvIsLeft == 1)) goto LOC_2F8B4;
	MoveCamera((v4 + ((96 * 5) / 2)), 422, 20);
	Delay(2);
	v3 = 900;
LOC_2F780:
	if (!(v3 >= 200)) goto LOC_2F8AC;
	asynccall CreateFireWall((v4 + ((96 * 3) / 2)), v3, a0);
	Delay(Rand(0, 1));
	asynccall CreateFireWall(((v4 + ((96 * 3) / 2)) + 400), v3, a0);
	Delay(Rand(0, 1));
	v3 = (v3 - 120);
	goto LOC_2F780;
LOC_2F8AC:
	goto LOC_2FA40;
LOC_2F8B4:
	MoveCamera((v4 - ((96 * 5) / 2)), 422, 20);
	Delay(2);
	v3 = 862;
LOC_2F914:
	if (!(v3 >= 200)) goto LOC_2FA40;
	asynccall CreateFireWall((v4 - ((96 * 3) / 2)), v3, a0);
	Delay(Rand(0, 1));
	asynccall CreateFireWall(((v4 + ((96 * 3) / 2)) - 540), v3, a0);
	Delay(Rand(0, 1));
	v3 = (v3 - 120);
	goto LOC_2F914;
LOC_2FA40:
	Wait("CreateFireWall");
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v1, (v2 - 140));
	Delay(60);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void TwisterSwordCallback(int a0, int a1) callsign 38001 {
	int v1;
	if (!(a1 == intvDefenderMajor)) goto LOC_2FBCC;
	if (!GetGlobal(51)) goto LOC_2FB34;
	v1 = (intvMagicAttackValue / GetGlobal(51));
LOC_2FB34:
	Hurt(a0, a1, v1);
	if (!(v1 > 0)) goto LOC_2FBC4;
	SetGlobal(51, (GetGlobal(51) * 2));
	HitGeneral(a0, a1, 13008, 2, 60, 0);
LOC_2FBC4:
	goto LOC_2FC18;
LOC_2FBCC:
	Hurt(a0, a1, 0);
	asynccall SmallFireBall3(a0, a1, 8, 0);
LOC_2FC18:
	return;
}

void CreateTwisterSwordAttack(int a0, int a1) {
	int v1;
	int v2;
	int v3[24];
	int v27;
	v1 = 0;
LOC_2FC38:
	if (!(v1 < 8)) goto LOC_2FDF0;
	v2 = 0;
LOC_2FC64:
	if (!(v2 < 3)) goto LOC_2FDE0;
	v3[((v1 * 3) + v2)] = CreateObjectByReference(a0, 2501, 0, 0);
	if (!(((v1 & 1) == 0) && (v2 == 2))) goto LOC_2FD58;
	SetObjectFlags(v3[((v1 * 3) + v2)], 50331648);
	goto LOC_2FD94;
LOC_2FD58:
	SetObjectFlags(v3[((v1 * 3) + v2)], 33554432);
LOC_2FD94:
	SetCallbackProcedure(v3[((v1 * 3) + v2)], 38001);
	v2++;
	goto LOC_2FC64;
LOC_2FDE0:
	v1++;
	goto LOC_2FC38;
LOC_2FDF0:
	v27 = 0;
LOC_2FE00:
	if (!(v27 < 280)) goto LOC_2FF34;
	v1 = 0;
LOC_2FE2C:
	if (!(v1 < 8)) goto LOC_2FF18;
	v2 = 0;
LOC_2FE58:
	if (!(v2 < 3)) goto LOC_2FF08;
	SetCoordinateByReference_Cylind(v3[((v1 * 3) + v2)], a0, ((32 * v1) + v27), (60 + (v2 * 32)), a1);
	v2++;
	goto LOC_2FE58;
LOC_2FF08:
	v1++;
	goto LOC_2FE2C;
LOC_2FF18:
	Delay(1);
	v27++;
	goto LOC_2FE00;
LOC_2FF34:
	v1 = 0;
LOC_2FF44:
	if (!(v1 < 24)) goto LOC_2FF8C;
	FreeObjectByHandle(v3[v1]);
	v1++;
	goto LOC_2FF44;
LOC_2FF8C:
	return;
}

void CreateTwisterSword(int a0, int a1, int a2) {
	int v1;
	int v2[64];
	int v66;
	int v67;
	int v69;
LOC_2FF94:
	v1 = CreateObjectRaw(a0, a1, 0, 0, 2501);
	v66 = 0;
LOC_2FFE8:
	if (!(v66 < 16)) goto LOC_30144;
	v2[v66] = CreateObjectByReference(v1, (48001 + v66), 128, a2);
	SetCoordinateByReference_Cylind(v2[v66], v1, (64 - (16 * v66)), 0, a2);
	SetObjectScale(v2[v66], 40960, 40960);
	SetObjectFlags(v2[v66], 128);
	SetObjectContext(v2[v66], 0, ((v66 % 4) * 2));
	v66++;
	goto LOC_2FFE8;
LOC_30144:
	v67 = 0;
LOC_30154:
	if (!(v67 <= 280)) goto LOC_303A4;
	v66 = 0;
LOC_30180:
	if (!(v66 < 16)) goto LOC_30340;
	v69 = GetObjectContext(v2[v66], 0);
	if (!(v69 == 0)) goto LOC_30218;
	ClearObjectFlags(v2[v66], 128);
	v69--;
	goto LOC_30294;
LOC_30218:
	if (!(v69 == (-2))) goto LOC_3028C;
	SetObjectFlags(v2[v66], 128);
	v69 = ((3 * 2) - 1);
	goto LOC_30294;
LOC_3028C:
	v69--;
LOC_30294:
	SetObjectContext(v2[v66], 0, v69);
	if (!(v67 <= 90)) goto LOC_30330;
	SetCoordinateByReference_Cylind(v2[v66], v1, (64 - (16 * v66)), v67, a2);
LOC_30330:
	v66++;
	goto LOC_30180;
LOC_30340:
	if (!(v67 == 90)) goto LOC_30388;
	asynccall CreateTwisterSwordAttack(v1, a2);
LOC_30388:
	Delay(1);
	v67++;
	goto LOC_30154;
LOC_303A4:
	v66 = 0;
LOC_303B4:
	if (!(v66 < 16)) goto LOC_303FC;
	FreeObjectByHandle(v2[v66]);
	v66++;
	goto LOC_303B4;
LOC_303FC:
	PlaySound1("m038snd01", 255);
	v66 = 0;
LOC_30428:
	if (!(v66 < 8)) goto LOC_30610;
	v2[v66] = CreateObjectByReference(v1, (48017 + (v66 * 2)), 128, a2);
	SetCoordinateByReference_Cylind(v2[v66], v1, (64 - (32 * v66)), 90, a2);
	SetObjectScale(v2[v66], 40960, 40960);
	SetObjectSpeed_Sphere(v2[v66], (64 - (32 * v66)), 0, 10);
	if (!((v66 & 1) == 0)) goto LOC_305B8;
	SetObjectFlags(v2[v66], 50331648);
	goto LOC_305DC;
LOC_305B8:
	SetObjectFlags(v2[v66], 33554432);
LOC_305DC:
	SetCallbackProcedure(v2[v66], 38001);
	v66++;
	goto LOC_30428;
LOC_30610:
	v66 = 0;
LOC_30620:
	if (!(v66 < 8)) goto LOC_30678;
	SetObjectFadeOut(v2[v66], 48, 1);
	v66++;
	goto LOC_30620;
LOC_30678:
	FreeObjectByHandle(v1);
	return;
}

void TwisterSword() {
	int v1;
	int v2;
	int v3;
LOC_30694:
	SetGlobal(51, 1);
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\038\\*");
	DisablePlayMagic();
	DownBrightness(8, 5);
	SetOverwhelming(intvAttackerMajor, 1);
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v1, (v2 - 120));
	AddAttackCounter(intvAttackerMajor, 2);
	Delay(15);
	SetObjectAnimate(intvAttackerMajor, 32768);
	Delay(20);
	SetOverwhelming(intvAttackerMajor, 0);
	v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
	if (!(v3 == 0)) goto LOC_30834;
	v3 = intvDefenderMajor;
LOC_30834:
	v1 = GetObjectScreenX(v3);
	v2 = GetObjectScreenY(v3);
	MoveCamera(v1, (v2 - 120), 20);
	PlaySound1("m038snd02", 200);
	CreateTwisterSword(v1, v2, 50);
	Delay(70);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void CreateRollTubBreak(int a0) {
	int v1;
	int v2;
	v2 = Rand(0, 255);
	v1 = CreateObjectByReference(a0, Rand(49002, 49008), v2, 20);
	SetObjectGravity(v1, 16384);
	SetCoordinateByReference_Cylind(v1, a0, v2, 4, Rand(20, 30));
	SetObjectSpeed_Sphere(v1, v2, Rand(36, 60), Rand(8, 12));
	Delay(8);
	SetObjectFadeOut(v1, 16, 2);
	return;
}

void RollTubCallback(int a0, int a1) callsign 39001 {
	int v1;
	if (!(GetGlobal(1) == 0)) goto LOC_30950;
	DelayAmbientSound("m039snd02", 255, 20);
LOC_30950:
	v1 = 0;
LOC_30960:
	if (!(v1 < 8)) goto LOC_309B8;
	asynccall CreateRollTubBreak(a0);
	v1++;
	goto LOC_30960;
LOC_309B8:
	SetObjectFlags(a0, 128);
	ClearObjectFlags(a0, 251658240);
	Delay(1);
	if (!((a1 == intvDefenderMajor) && (GetGlobal(52) == 0))) goto LOC_30A68;
	SetGlobal(52, 1);
	Hurt(a0, a1, intvMagicAttackValue);
	goto LOC_30A88;
LOC_30A68:
	Hurt(a0, a1, 0);
LOC_30A88:
	FreeObjectByHandle(a0);
	return;
}

void CreateRollTub(int a0, int a1, int a2) {
	int v1;
	int v2;
	int v3;
	Delay(a2);
	v1 = CreateObjectRaw(a0, a1, 0, 0, 49001);
	SetObjectFlags(v1, (33554432 + 16777216));
	SetCallbackProcedure(v1, 39001);
	SetObjectSpeed_Sphere(v1, 192, 0, Rand(4, 6));
	SetObjectFlags(v1, 262144);
	v3 = 0;
LOC_30D08:
	if (!(v3 <= 16)) goto LOC_30D5C;
	SetObjectOpacity(v1, v3);
	Delay(2);
	v3++;
	goto LOC_30D08;
LOC_30D5C:
	ClearObjectFlags(v1, 262144);
LOC_30D78:
	Delay(1);
	if (!(IsObjectExist(v1) == 0)) goto LOC_30DB4;
	return;
LOC_30DB4:
	v2 = GetObjectScreenY(v1);
	if ((v2 >= 80)) goto LOC_30D78;
	SetObjectFadeOut(v1, 16, 1);
	return;
}

void RollTub(int a0) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	int v7;
LOC_30E18:
	SetGlobal(1, 0);
	SetGlobal(52, 0);
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\039\\*");
	DisablePlayMagic();
	DownBrightness(12, 5);
	SetOverwhelming(intvAttackerMajor, 1);
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v1, (v2 - 120));
	AddAttackCounter(intvAttackerMajor, 2);
	Delay(15);
	SetObjectAnimate(intvAttackerMajor, 32768);
	Delay(20);
	SetOverwhelming(intvAttackerMajor, 0);
	v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
	if (!(v3 == 0)) goto LOC_30FC8;
	v3 = intvDefenderMajor;
LOC_30FC8:
	v1 = GetObjectScreenX(v3);
	v2 = GetObjectScreenY(v3);
	v7 = (GetBattleHeightInScreenY() + 40);
	v6 = (v1 + (96 * 9));
	v4 = 0;
LOC_31058:
	if (!(v4 < (a0 + 1))) goto LOC_312B8;
	v5 = (v1 - (9 * 96));
LOC_310A8:
	if (!(v5 <= v6)) goto LOC_3118C;
	if (!(v4 == 0)) goto LOC_31128;
	asynccall CreateRollTub(v5, v7, Rand(52, 57));
	goto LOC_31168;
LOC_31128:
	asynccall CreateRollTub(v5, v7, Rand(2, 3));
LOC_31168:
	v5 = (v5 + 96);
	goto LOC_310A8;
LOC_3118C:
	Delay(4);
	if (!(v4 == 0)) goto LOC_31250;
	SetViewCamera((v1 - (9 * 96)), (v7 - 200));
	Delay(15);
	MoveCamera(v1, (v2 - 140), 40);
	PlaySound1("m039snd01", 255);
	goto LOC_31288;
LOC_31250:
	if (!(v4 == 2)) goto LOC_31288;
	PlaySound1("m039snd01", 255);
LOC_31288:
	Delay(Rand(10, 12));
	v4++;
	goto LOC_31058;
LOC_312B8:
	Wait("CreateRollTub");
	Wait("CreateRollTubBreak");
	Delay(60);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void EarthquakeCallback(int a0, int a1) callsign 40001 {
	return;
}

void CreateQuakeSmoke(int a0, int a1, int a2, int a3, int a4) {
	int v1;
	v1 = CreateObjectRaw(a0, a1, a2, 0, 50002);
	SetObjectSpeed_Cylind(v1, 0, a3);
	Delay((a4 - 4));
	SetObjectScaleShrink(v1, (-512));
	Delay(4);
	SetObjectFadeOut(v1, 16, 1);
	return;
}

void CreateQuakeStone(int a0, int a1, int a2) {
	int v1;
	int v2;
	Delay(a2);
	v2 = 0;
LOC_3140C:
	if (!(v2 < 4)) goto LOC_31518;
	asynccall CreateQuakeSmoke((a0 + Rand((-30), 30)), (a1 + Rand((-16), 16)), Rand(0, 10), Rand(2, 4), Rand(8, 12));
	Delay(Rand(2, 8));
	v2++;
	goto LOC_3140C;
LOC_31518:
	v1 = CreateObjectRaw(a0, a1, 0, (128 * Rand(0, 1)), 50001);
	SetObjectFlags(v1, 33554432);
	SetCallbackProcedure(v1, 40001);
	v2 = 0;
LOC_315BC:
	if (!(v2 < 70)) goto LOC_316E4;
	if (!(Rand(1, 3) == 1)) goto LOC_316C8;
	asynccall CreateQuakeSmoke((a0 + Rand((-30), 30)), (a1 + Rand((-16), 16)), Rand(0, 10), Rand(2, 4), Rand(8, 12));
LOC_316C8:
	Delay(1);
	v2++;
	goto LOC_315BC;
LOC_316E4:
	SetObjectFadeOut(v1, 16, 1);
	return;
}

void ShakeCamera(int a0, int a1, int a2) {
	int v1;
	v1 = 0;
LOC_31728:
	if (!(v1 < a2)) goto LOC_31810;
	if (!((v1 % 3) == 2)) goto LOC_317D0;
	SetViewCamera((a0 + Rand((-15), 15)), (a1 + Rand((-15), 15)));
LOC_317D0:
	Delay(1);
	if (!(GetGlobal(53) == 1)) goto LOC_31800;
	goto LOC_31810;
LOC_31800:
	v1++;
	goto LOC_31728;
LOC_31810:
	return;
}

void QuakeSound() {
	int v1;
	v1 = 0;
	PlaySound1("m040snd01", 255);
LOC_3184C:
	if (!(GetGlobal(53) == 0)) goto LOC_318D8;
	Delay(1);
	v1++;
	if (!(v1 > (99 - 20))) goto LOC_318D0;
	PlaySound1("m040snd01", 255);
	v1 = 0;
LOC_318D0:
	goto LOC_3184C;
LOC_318D8:
	return;
}

void Earthquake() {
	int v1;
	int v2;
	int v3;
	int v4;
LOC_318E0:
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\040\\*");
	DisablePlayMagic();
	DownBrightness(12, 5);
	SetOverwhelming(intvAttackerMajor, 1);
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v1, (v2 - 120));
	AddAttackCounter(intvAttackerMajor, 2);
	Delay(15);
	SetObjectAnimate(intvAttackerMajor, 32768);
	Delay(20);
	SetOverwhelming(intvAttackerMajor, 0);
	v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
	if (!(v3 == 0)) goto LOC_31A70;
	v3 = intvDefenderMajor;
LOC_31A70:
	v1 = GetObjectScreenX(v3);
	v2 = GetObjectScreenY(v3);
	MoveCamera(v1, (v2 - 120), 20);
	Delay(10);
	SetGlobal(53, 0);
	asynccall QuakeSound();
	asynccall ShakeCamera(v1, (v2 - 120), 9999);
	v4 = ((GetSoldierCount((intvIsLeft ^ 1)) / 2) + Rand(0, 10));
LOC_31BA8:
	Delay(Rand(1, 10));
	if (!((v4 % 3) == 0)) goto LOC_31C20;
	v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
	goto LOC_31C48;
LOC_31C20:
	v3 = GetRandomSoldierFromAlive((intvIsLeft ^ 1));
LOC_31C48:
	if (!((v3 == 0) || (v3 == intvDefenderMajor))) goto LOC_31D1C;
	v1 = (GetObjectScreenX(intvDefenderMajor) + (10 * Rand((-40), 40)));
	v2 = (GetObjectScreenY(intvDefenderMajor) + (10 * Rand((-30), 30)));
	goto LOC_31D54;
LOC_31D1C:
	v1 = GetObjectScreenX(v3);
	v2 = GetObjectScreenY(v3);
LOC_31D54:
	asynccall CreateQuakeStone(v1, v2, Rand(2, 12));
	v4--;
	if ((v4 > 0)) goto LOC_31BA8;
	Wait("CreateQuakeStone");
	Delay(28);
	SetGlobal(53, 1);
	Delay(60);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void InterceptorCallback(int a0, int a1) callsign 41001 {
	int v1;
	int v2;
	int v3;
	asynccall Blood(a1, 16);
	if (!(a1 == intvDefenderMajor)) goto LOC_31EF4;
	if (!GetGlobal(54)) goto LOC_31E94;
	v3 = (intvMagicAttackValue / GetGlobal(54));
LOC_31E94:
	Hurt(a0, a1, v3);
	if (!(v3 > 0)) goto LOC_31EEC;
	SetGlobal(54, (GetGlobal(54) * 2));
LOC_31EEC:
	goto LOC_31F14;
LOC_31EF4:
	Hurt(a0, a1, 0);
LOC_31F14:
	v1 = GetObjectContext(a0, 0);
	v1--;
	if (!(v1 <= 0)) goto LOC_31FB4;
	v2 = GetObjectContext(a0, 1);
	FreeObjectByHandle(a0);
	SetGlobal(55 + v2, 0);
	goto LOC_31FD8;
LOC_31FB4:
	SetObjectContext(a0, 0, v1);
LOC_31FD8:
	return;
}

void CreateInterceptor(int a0, int a1, int a2, int a3) {
	int v1;
LOC_31FE0:
	if (!(GetGlobal(55 + a3) != 0)) goto LOC_32014;
	return;
LOC_32014:
	a0 = BattleXToScreenX(a0);
	a1 = BattleXToScreenX(a1);
	v1 = CreateObjectRaw(a0, a1, 0, a2, 51001);
	SetObjectFlags(v1, (33554432 + 16777216));
	SetCallbackProcedure(v1, 41001);
	SetObjectContext(v1, 0, 3);
	SetObjectContext(v1, 1, a3);
	SetGlobal(55 + a3, v1);
	return;
}

void MakeInterceptorSound() {
	int v1;
	v1 = 0;
LOC_3214C:
	if (!(v1 < 5)) goto LOC_321B8;
	PlaySound1("m041snd01", (128 + (v1 * 15)));
	Delay(12);
	v1++;
	goto LOC_3214C;
LOC_321B8:
	return;
}

void Interceptor(int a0) {
	int v1;
	int v2;
	int v3;
	int v4;
LOC_321C0:
	SetGlobal(54, 1);
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\041\\*");
	DisablePlayMagic();
	DownBrightness(12, 5);
	SetOverwhelming(intvAttackerMajor, 1);
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v1, (v2 - 120));
	AddAttackCounter(intvAttackerMajor, 2);
	Delay(15);
	SetObjectAnimate(intvAttackerMajor, 32768);
	PlaySound(intvAttackerMajor, "m041snd02", 255);
	Delay(20);
	SetOverwhelming(intvAttackerMajor, 0);
	v4 = GetBattleWidth();
	if (!(intvIsLeft == 1)) goto LOC_326B0;
	switch (a0) { 
		case 0: goto LOC_3238C;
		case 1: goto LOC_32418;
	}
	goto LOC_32540;
LOC_3238C:
	v3 = 0;
LOC_3239C:
	if (!(v3 < 15)) goto LOC_32410;
	CreateInterceptor((v4 - 2), v3, 128, v3);
	v3 = (v3 + 2);
	goto LOC_3239C;
LOC_32410:
	goto LOC_32640;
LOC_32418:
	v3 = 0;
LOC_32428:
	if (!(v3 < 15)) goto LOC_3249C;
	CreateInterceptor((v4 - 2), v3, 128, v3);
	v3 = (v3 + 2);
	goto LOC_32428;
LOC_3249C:
	v3 = 1;
LOC_324AC:
	if (!(v3 < 15)) goto LOC_32538;
	CreateInterceptor((v4 - 3), v3, 128, ((15 + v3) - 1));
	v3 = (v3 + 2);
	goto LOC_324AC;
LOC_32538:
	goto LOC_32640;
LOC_32540:
	DisableEscape((intvIsLeft ^ 1));
	v3 = 0;
LOC_32570:
	if (!(v3 < 15)) goto LOC_325C4;
	CreateInterceptor(v4, v3, 128, v3);
	v3++;
	goto LOC_32570;
LOC_325C4:
	v3 = 0;
LOC_325D4:
	if (!(v3 < 15)) goto LOC_32640;
	CreateInterceptor((v4 + 1), v3, 128, (15 + v3));
	v3++;
	goto LOC_325D4;
LOC_32640:
	asynccall MakeInterceptorSound();
	MoveCamera((GetBattleWidthInScreenX() - 280), (v2 - 120), 40);
	goto LOC_329E4;
LOC_326B0:
	switch (a0) { 
		case 0: goto LOC_326DC;
		case 1: goto LOC_32768;
	}
	goto LOC_32884;
LOC_326DC:
	v3 = 0;
LOC_326EC:
	if (!(v3 < 15)) goto LOC_32760;
	CreateInterceptor(1, v3, 0, (30 + v3));
	v3 = (v3 + 2);
	goto LOC_326EC;
LOC_32760:
	goto LOC_3298C;
LOC_32768:
	v3 = 0;
LOC_32778:
	if (!(v3 < 15)) goto LOC_327EC;
	CreateInterceptor(1, v3, 0, (30 + v3));
	v3 = (v3 + 2);
	goto LOC_32778;
LOC_327EC:
	v3 = 1;
LOC_327FC:
	if (!(v3 < 15)) goto LOC_3287C;
	CreateInterceptor(2, v3, 0, ((45 + v3) - 1));
	v3 = (v3 + 2);
	goto LOC_327FC;
LOC_3287C:
	goto LOC_3298C;
LOC_32884:
	DisableEscape((intvIsLeft ^ 1));
	v3 = 0;
LOC_328B4:
	if (!(v3 < 15)) goto LOC_32918;
	CreateInterceptor((-1), v3, 0, (30 + v3));
	v3++;
	goto LOC_328B4;
LOC_32918:
	v3 = 0;
LOC_32928:
	if (!(v3 < 15)) goto LOC_3298C;
	CreateInterceptor((-2), v3, 0, (45 + v3));
	v3++;
	goto LOC_32928;
LOC_3298C:
	asynccall MakeInterceptorSound();
	MoveCamera(280, (v2 - 120), 40);
LOC_329E4:
	Delay(60);
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v1, (v2 - 120));
	Delay(30);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void XCallDragonCallback(int a0, int a1) callsign 42001 {
	if (!((a1 == intvDefenderMajor) && (GetGlobal(116) == 0))) goto LOC_32B20;
	Hurt(a0, a1, 2);
	SetGlobal(116, 1);
	asynccall LockTargetTime2(a1, 10015, 60);
	goto LOC_32B6C;
LOC_32B20:
	Hurt(a0, a1, 0);
	asynccall FireMan(a1, 10015, 60);
LOC_32B6C:
	return;
}

void Xsc3602(int a0, int a1, int a2, int a3) {
	int v1;
	int v2;
	int v3;
	int v5;
	int v6;
	switch (a0) { 
		case 0: goto LOC_32E28;
		case 1: goto LOC_32E40;
	}
	goto LOC_32E58;
LOC_32E28:
	v5 = 10;
	goto LOC_32E68;
LOC_32E40:
	v5 = 20;
	goto LOC_32E68;
LOC_32E58:
	v5 = 20;
LOC_32E68:
	v1 = 0;
LOC_32E78:
	if (!(v1 < v5)) goto LOC_32FDC;
	v2 = (a1 + Rand((-48), 48));
	v3 = (a2 + Rand((-16), 16));
	v6 = CreateObjectRaw(v2, v3, (a3 + 20), 0, Rand(28002, 28004));
	SetObjectSpeed_Cylind(v6, 0, Rand(10, 30));
	PlaySound1("m016a", 255);
	Delay(Rand(0, 5));
	v1++;
	goto LOC_32E78;
LOC_32FDC:
	Delay(15);
	return;
}

void Xsc3601(int a0, int a1, int a2, int a3, int a4) {
	int v1;
	int v2;
	Delay(Rand(10, 50));
	v1 = CreateObjectRaw(a2, a3, a4, a1, 28001);
	v2 = 60;
LOC_32BE8:
	if (!(v2 > 0)) goto LOC_32C54;
	a4++;
	SetObjectCoordinate(v1, a2, a3, a4);
	v2--;
	Delay(1);
	goto LOC_32BE8;
LOC_32C54:
	Delay(30);
	asynccall Xsc3602(a0, a2, a3, a4);
	v2 = 0;
LOC_32C9C:
	if (!(v2 < 10)) goto LOC_32D10;
	SetObjectScale(v1, 0, (65536 + (v2 * 65536)));
	Delay(1);
	v2++;
	goto LOC_32C9C;
LOC_32D10:
	if (!(v2 < 30)) goto LOC_32DD8;
	a4 = (a4 + (v2 * 10));
	SetObjectScale(v1, 0, (65536 + (v2 * 65536)));
	SetObjectCoordinate(v1, a2, a3, a4);
	Delay(1);
	v2++;
	goto LOC_32D10;
LOC_32DD8:
	FreeObjectByHandle(v1);
	return;
}

void Xsc3603(int a0, int a1, int a2, int a3) {
	int v1;
	int v2;
	int v3;
	CreateObjectRaw(a1, a2, Rand(320, 360), 0, 28012);
	Delay(Rand(4, 10));
	v1 = (2 + (3 * a0));
LOC_3308C:
	v1--;
	if (!(v1 > 0)) goto LOC_3318C;
	v2 = (a1 + Rand((-320), 320));
	v3 = (a2 + Rand((-100), 300));
	CreateObjectRaw(v2, v3, Rand(320, 360), 0, 28012);
	Delay(Rand(4, 10));
	goto LOC_3308C;
LOC_3318C:
	return;
}

void Xsc2512(int a0) {
	int v1;
	int v2;
	v1 = 16384;
	SetObjectSpeed_Cylind(a0, Rand(4, 12), Rand((-2), 8));
	SetObjectFriction1(a0, 8192);
	v2 = Rand(30, 40);
LOC_3323C:
	v2--;
	if (!(v2 > 0)) goto LOC_332B4;
	SetObjectScale(a0, v1, v1);
	v1 = (v1 - 256);
	Delay(1);
	goto LOC_3323C;
LOC_332B4:
	FreeObjectByHandle(a0);
	return;
}

void Xsc3604() callsign 42002 {
	int v1;
	int v2;
	int v3;
	v3 = GetScriptLinkedObject();
	SetScriptLinkedObject(intvAttackerMajor);
	asynccall MovingShadow(v3, GetGlobal(115), 28011, 0, 42001);
	asynccall MovingShadow(v3, GetGlobal(115), 28011, 128, 42001);
	asynccall MovingShadow(v3, GetGlobal(115), 28011, 64, 42001);
	asynccall MovingShadow(v3, GetGlobal(115), 28011, 192, 42001);
	SetObjectSpeed_Cylind(v3, 0, 0);
	SetObjectHasGravity(v3, 0);
	v2 = 0;
LOC_33440:
	if (!(v2 < 20)) goto LOC_33504;
	v1 = CreateObjectByReference(v3, 10003, Rand(0, 255), 0);
	SetObjectScale(v1, 16384, 16384);
	asynccall Xsc2512(v1);
	v2++;
	goto LOC_33440;
LOC_33504:
	PlaySound(v1, "m018snd03", 255);
	return;
}

void XCallDragon(int a0) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v7;
	int v8;
	int v9;
	int v10;
	int v11;
	int v12;
	int v13;
	SetGlobal(116, 0);
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	if (!(intvIsLeft == 1)) goto LOC_335B4;
	v10 = 0;
	goto LOC_335C4;
LOC_335B4:
	v10 = 128;
LOC_335C4:
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	v3 = GetObjectScreenZ(intvAttackerMajor);
	PlaySound1("m018snd01", 255);
	switch (a0) { 
		case 0: goto LOC_33660;
		case 1: goto LOC_336B4;
	}
	goto LOC_33708;
LOC_33660:
	asynccall Xsc3602(a0, v1, v2, v3);
	v11 = 50;
	SetGlobal(115, 110);
	goto LOC_33770;
LOC_336B4:
	asynccall Xsc3602(a0, v1, v2, v3);
	v11 = 100;
	SetGlobal(115, 220);
	goto LOC_33770;
LOC_33708:
	asynccall Xsc3601(a0, v10, v1, v2, v3);
	v11 = (100 + 95);
	SetGlobal(115, 330);
LOC_33770:
	v4 = 0;
LOC_33780:
	if (!(v4 < v11)) goto LOC_33B8C;
	v12 = Rand(1, 2);
	v5 = 0;
LOC_337D0:
	if (!(v5 < v12)) goto LOC_33984;
	v7 = (v1 + Rand((-48), 48));
	v8 = (v2 + Rand((-16), 16));
	if (!(a0 == 2)) goto LOC_33928;
	CreateObjectRaw((v7 - 48), v8, v9, 0, Rand(28006, 28007));
	CreateObjectRaw((v7 + 48), v8, v9, 0, Rand(28006, 28007));
	goto LOC_33974;
LOC_33928:
	CreateObjectRaw(v7, v8, v9, 0, Rand(28006, 28007));
LOC_33974:
	v5++;
	goto LOC_337D0;
LOC_33984:
	if (!(Rand(0, 3) == 0)) goto LOC_33ACC;
	v7 = (v1 + Rand((-48), 48));
	v8 = (v2 + Rand((-16), 16));
	v13 = CreateObjectRaw(v7, v8, v9, Rand(0, 255), Rand(28008, 28009));
	SetObjectSpeed_Cylind(v13, Rand(10, 16), Rand(10, 16));
LOC_33ACC:
	Delay(1);
	switch (v4) { 
		case 49: goto LOC_33B10;
		case 99: goto LOC_33B34;
		case 149: goto LOC_33B58;
	}
	goto LOC_33B7C;
LOC_33B10:
	PlaySound1("m018snd02", 255);
	goto LOC_33B7C;
LOC_33B34:
	PlaySound1("m018snd01", 255);
	goto LOC_33B7C;
LOC_33B58:
	PlaySound1("m018snd02", 255);
	goto LOC_33B7C;
LOC_33B7C:
	v4++;
	goto LOC_33780;
LOC_33B8C:
	if (!(a0 > 1)) goto LOC_33BBC;
	Wait("Xsc3601");
	goto LOC_33BC8;
LOC_33BBC:
	Wait("Xsc3602");
LOC_33BC8:
	v13 = GetMostImportantSoldier((intvIsLeft ^ 1));
	if (!(v13 == 0)) goto LOC_33C1C;
	v13 = intvDefenderMajor;
LOC_33C1C:
	v1 = GetObjectScreenX(v13);
	v2 = GetObjectScreenY(v13);
	v3 = GetObjectScreenZ(v13);
	if (!(v2 > 240)) goto LOC_33CC0;
	MoveCamera(v1, (v2 - 240), 20);
	goto LOC_33CE0;
LOC_33CC0:
	MoveCamera(v1, 0, 20);
LOC_33CE0:
	if (!(IsObjectExist(v13) == 0)) goto LOC_33D5C;
	v13 = GetMostImportantSoldier((intvIsLeft ^ 1));
	if (!(v13 == 0)) goto LOC_33D5C;
	v13 = intvDefenderMajor;
LOC_33D5C:
	v1 = GetObjectScreenX(v13);
	v2 = GetObjectScreenY(v13);
	v3 = GetObjectScreenZ(v13);
	asynccall Xsc3603(a0, v1, v2, v3);
	Wait("Xsc3603");
	Delay(65);
	return;
}

void XFireCowCallback(int a0, int a1) callsign 42003 {
	int v1;
	int v2;
	if (!(a1 == intvDefenderMajor)) goto LOC_33EFC;
	SetObjectFadeOut(a0, 2, 1);
	if (!GetGlobal(117)) goto LOC_33E70;
	v2 = (2 / GetGlobal(117));
LOC_33E70:
	Hurt(a0, a1, v2);
	if (!(v2 > 0)) goto LOC_33EF4;
	SetGlobal(117, (GetGlobal(117) * 2));
	asynccall LockTargetTime2(a1, 10015, 60);
LOC_33EF4:
	goto LOC_33F74;
LOC_33EFC:
	Hurt(a0, a1, 0);
	asynccall FireMan(a1, 10015, 60);
	asynccall Blood(a1, 16);
LOC_33F74:
	v1 = GetObjectContext(a0, 0);
	v1--;
	if (!(v1 <= 0)) goto LOC_33FE8;
	SetObjectFadeOut(a0, 2, 1);
	goto LOC_3400C;
LOC_33FE8:
	SetObjectContext(a0, 0, v1);
LOC_3400C:
	return;
}

void XAttachFireToCow(int a0, int a1) {
	int v1;
	int v2;
	v1 = CreateObjectByReference(a0, 37002, a1, 50);
	SetObjectScale(v1, 131072, 131072);
	SetObjectFlags(v1, 262144);
	SetObjectOpacity(v1, 12);
	v2 = CreateObjectByReference(a0, 37002, a1, 50);
	SetObjectScale(v2, 131072, 131072);
	SetObjectFlags(v2, 262144);
	SetObjectOpacity(v2, 12);
LOC_3413C:
	if (!IsObjectExist(a0)) goto LOC_34298;
	SetCoordinateByReference_Cylind(v1, a0, a1, 24, 50);
	MoveObject(v1, Rand((-8), 8), 0, Rand((-8), 8));
	SetCoordinateByReference_Cylind(v2, a0, (a1 - 128), 20, 50);
	MoveObject(v2, Rand((-8), 8), 0, Rand((-8), 8));
	Delay(1);
	goto LOC_3413C;
LOC_34298:
	FreeObjectByHandle(v1);
	FreeObjectByHandle(v2);
	return;
}

void XCreateFireCow(int a0, int a1, int a2, int a3, int a4) {
	int v1;
	int v2;
	int v3;
	v1 = CreateObjectRaw((a0 + (8 * Rand((-6), 6))), (a1 + (8 * Rand((-8), 8))), 0, a2, 37001);
	SetObjectSpeed_Sphere(v1, a2, 0, (8 + Rand((-2), 2)));
	SetCallbackProcedure(v1, 42003);
	SetObjectFlags(v1, (33554432 + 16777216));
	SetObjectContext(v1, 0, (8 + Rand((-4), 4)));
	asynccall XAttachFireToCow(v1, (a2 + 128));
	v3 = 1;
LOC_34490:
	if (!IsObjectExist(v1)) goto LOC_34668;
	v2 = GetObjectScreenX(v1);
	if (!(a3 == 1)) goto LOC_34544;
	if (!(v2 > (GetBattleWidthInScreenX() - 100))) goto LOC_3453C;
	SetObjectFadeOut(v1, 16, 2);
	goto LOC_34668;
LOC_3453C:
	goto LOC_3458C;
LOC_34544:
	if (!(v2 < 100)) goto LOC_3458C;
	SetObjectFadeOut(v1, 16, 2);
	goto LOC_34668;
LOC_3458C:
	Delay(1);
	v3++;
	if (!((v3 & 7) == 0)) goto LOC_34618;
	MoveObject(v1, 0, (8 * Rand((-3), 3)), 0);
LOC_34618:
	if (!(v3 > 500)) goto LOC_34660;
	SetObjectFadeOut(v1, 16, 2);
	goto LOC_34668;
LOC_34660:
	goto LOC_34490;
LOC_34668:
	return;
}

void XFireCow(int a0) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	SetGlobal(117, 1);
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	if (!(intvIsLeft == 1)) goto LOC_34708;
	v4 = (-1);
	v3 = 0;
	goto LOC_34728;
LOC_34708:
	v4 = 1;
	v3 = 128;
LOC_34728:
	v5 = GetSoldierMaxX(intvIsLeft);
	if (!(intvIsLeft == 1)) goto LOC_34764;
	goto LOC_34764;
LOC_34764:
	Delay(15);
	PlaySound1("m027snd01", 180);
	PlaySound1("m027snd02", 180);
	if (!(a0 == 0)) goto LOC_34958;
	asynccall XCreateFireCow((v5 + (1200 * v4)), (562 + 75), v3, intvIsLeft, 0);
	asynccall XCreateFireCow((v5 + (1200 * v4)), (562 - 75), v3, intvIsLeft, 0);
	asynccall XCreateFireCow((v5 + (1200 * v4)), (562 + 225), v3, intvIsLeft, (a0 + 1));
	asynccall XCreateFireCow((v5 + (1200 * v4)), (562 - 225), v3, intvIsLeft, 0);
	goto LOC_3503C;
LOC_34958:
	if (!(a0 == 1)) goto LOC_34C1C;
	asynccall XCreateFireCow((v5 + (1200 * v4)), (562 + 350), v3, intvIsLeft, 0);
	asynccall XCreateFireCow((v5 + (1200 * v4)), (562 + 250), v3, intvIsLeft, 0);
	asynccall XCreateFireCow((v5 + (1200 * v4)), (562 + 150), v3, intvIsLeft, 0);
	asynccall XCreateFireCow((v5 + (1200 * v4)), 562, v3, intvIsLeft, (a0 + 1));
	asynccall XCreateFireCow((v5 + (1200 * v4)), (562 - 150), v3, intvIsLeft, 0);
	asynccall XCreateFireCow((v5 + (1200 * v4)), (562 - 250), v3, intvIsLeft, 0);
	asynccall XCreateFireCow((v5 + (1200 * v4)), (562 - 350), v3, intvIsLeft, 0);
	goto LOC_3503C;
LOC_34C1C:
	asynccall XCreateFireCow((v5 + (1200 * v4)), (562 + 400), v3, intvIsLeft, 0);
	asynccall XCreateFireCow((v5 + (1200 * v4)), (562 + 320), v3, intvIsLeft, 0);
	asynccall XCreateFireCow((v5 + (1200 * v4)), (562 + 240), v3, intvIsLeft, 0);
	asynccall XCreateFireCow((v5 + (1200 * v4)), (562 + 160), v3, intvIsLeft, 0);
	asynccall XCreateFireCow((v5 + (1200 * v4)), (562 + 80), v3, intvIsLeft, 0);
	asynccall XCreateFireCow((v5 + (1200 * v4)), 562, v3, intvIsLeft, (a0 + 1));
	asynccall XCreateFireCow((v5 + (1200 * v4)), (562 - 80), v3, intvIsLeft, 0);
	asynccall XCreateFireCow((v5 + (1200 * v4)), (562 - 160), v3, intvIsLeft, 0);
	asynccall XCreateFireCow((v5 + (1200 * v4)), (562 - 240), v3, intvIsLeft, 0);
	asynccall XCreateFireCow((v5 + (1200 * v4)), (562 - 320), v3, intvIsLeft, 0);
	asynccall XCreateFireCow((v5 + (1200 * v4)), (562 - 400), v3, intvIsLeft, 0);
LOC_3503C:
	Delay(Rand(110, 160));
	PlaySound1("m027snd02", 210);
	Delay(10);
	PlaySound1("m027snd01", 160);
	Wait("XCreateFireCow");
	Delay(60);
	return;
}

void XSparkCallback(int a0, int a1) callsign 42004 {
	int v1;
	int v2;
	v1 = GetObjectContext(a0, 0);
	if (!(v1 == 4660)) goto LOC_35120;
	v2 = 2;
	goto LOC_35130;
LOC_35120:
	v2 = 2;
LOC_35130:
	Hurt(a0, a1, v2);
	asynccall SmallFireBall3(a0, a1, 20, 0);
	return;
}

void XDecreaseHP() {
	int v1;
	v1 = 1;
LOC_3519C:
	if (!(v1 <= (intvMagicAttackValue - 4))) goto LOC_35204;
	DoHarmToMajor(intvDefenderMajor, 0, 1);
	Delay(4);
	v1++;
	goto LOC_3519C;
LOC_35204:
	return;
}

void XCreateSparkleCenter(int a0, int a1) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	Delay(a1);
	v1 = CreateObjectByReference(a0, 42001, 0, 0);
	SetObjectScale(v1, 73728, 73728);
	v4 = GetObjectScreenX(v1);
	v5 = GetObjectScreenY(v1);
	v6 = GetObjectScreenZ(v1);
	v3 = Rand(60, 80);
	v2 = 0;
LOC_35300:
	if (!(v2 < (v3 / 4))) goto LOC_353B8;
	Delay(6);
	SetObjectCoordinate(v1, (v4 + Rand((-16), 16)), v5, (v6 + Rand((-16), 16)));
	v2++;
	goto LOC_35300;
LOC_353B8:
	SetObjectCoordinate(v1, v4, v5, v6);
	SetObjectFadeOut(v1, 16, 1);
	return;
}

void XCreateSparkle(int a0) {
	int v1;
	int v2;
	int v4;
LOC_35410:
	PlaySound1("m032snd01", 255);
	a0 = CreateObjectByReference(a0, 2501, 0, 0);
	v1 = CreateObjectByReference(a0, 42004, 0, 48);
	SetObjectScale(v1, 98304, 98304);
	SetCallbackProcedure(v1, 42004);
	SetObjectFlags(v1, (16777216 + 33554432));
	SetObjectContext(v1, 0, 4660);
	Delay(4);
	SetObjectFadeOut(v1, 8, 1);
	v1 = CreateObjectByReference(a0, 42002, 0, 48);
	SetObjectScale(v1, 98304, 98304);
	asynccall XCreateSparkleCenter(v1, Rand(0, 1));
	asynccall XCreateSparkleCenter(v1, Rand(20, 48));
	asynccall XCreateSparkleCenter(v1, Rand(20, 48));
	asynccall XCreateSparkleCenter(v1, Rand(20, 48));
	asynccall XDecreaseHP();
	Wait("XCreateSparkleCenter");
	PlaySound1("m032snd02", 255);
	v2 = CreateObjectByReference(v1, 42003, 0, 0);
	SetObjectScaleShrink(v2, (-4096));
	v4 = 0;
LOC_35768:
	if (!(v4 < 256)) goto LOC_357F4;
	asynccall LockByCenter(a0, 2501, v4, 52, 2, 33554432, 42004);
	v4 = (v4 + 32);
	goto LOC_35768;
LOC_357F4:
	v4 = 0;
LOC_35804:
	if (!(v4 < 256)) goto LOC_35890;
	asynccall LockByCenter(a0, 2501, v4, 110, 2, 33554432, 42004);
	v4 = (v4 + 32);
	goto LOC_35804;
LOC_35890:
	Delay(2);
	SetObjectFadeOut(v1, 16, 1);
LOC_358C0:
	if (!IsObjectExist(v1)) goto LOC_358F0;
	Delay(1);
	goto LOC_358C0;
LOC_358F0:
	Delay(8);
	SetObjectFadeOut(v2, 16, 1);
	FreeObjectByHandle(a0);
	return;
}

void XSparkle(int a0) {
	int v1;
	int v2;
	int v3;
	Delay(260);
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	AddAttackCounter(intvDefenderMajor, 5);
	v3 = intvDefenderMajor;
	v1 = GetObjectScreenX(v3);
	v2 = GetObjectScreenY(v3);
	XCreateSparkle(v3);
	Delay(60);
	return;
}

void MoveEnemyDelay(int a0, int a1) {
	int v1;
	int v2;
	Delay(a1);
	v1 = GetObjectScreenX(intvDefenderMajor);
	v2 = GetObjectScreenY(intvDefenderMajor);
	MoveCamera(v1, v2, a0);
	return;
}

void FinalX1() {
	int v1;
	int v2;
LOC_35A84:
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\032\\*");
	BatchLoadShape("magic\\027\\*");
	BatchLoadShape("magic\\018\\*");
	DisablePlayMagic();
	DownBrightness(8, 5);
	SetOverwhelming(intvAttackerMajor, 1);
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v1, (v2 - 120));
	AddAttackCounter(intvAttackerMajor, 2);
	Delay(15);
	SetObjectAnimate(intvAttackerMajor, 32768);
	Delay(20);
	SetOverwhelming(intvAttackerMajor, 0);
	asynccall XCallDragon(2);
	asynccall XFireCow(2);
	asynccall XSparkle(0);
	asynccall MoveEnemyDelay(40, 300);
	Wait("XCallDragon");
	Wait("XFireCow");
	Wait("XSparkle");
	Delay(20);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void XFirePillarCallback(int a0, int a1) callsign 43001 {
	int v1;
	if (!(a1 == intvDefenderMajor)) goto LOC_35DDC;
	if (!GetGlobal(118)) goto LOC_35D44;
	v1 = (2 / GetGlobal(118));
LOC_35D44:
	Hurt(a0, a1, v1);
	if (!(v1 > 0)) goto LOC_35DD4;
	SetGlobal(118, (GetGlobal(118) * 2));
	HitGeneral(a0, a1, 11002, 1, 48, 0);
LOC_35DD4:
	goto LOC_35E28;
LOC_35DDC:
	Hurt(a0, a1, 0);
	asynccall FireMan(a1, 10015, 60);
LOC_35E28:
	return;
}

void XCreateFirePillarSource(int a0, int a1, int a2) {
	int v1;
	int v2;
	v1 = CreateObjectByReference(a0, 41001, 0, 8);
	SetCoordinateByReference_Cylind(v1, a0, a2, a1, 8);
	PlaySound1("m031snd03", 255);
	Delay(8);
	v2 = a1;
LOC_35ED8:
	if (!(v2 > 0)) goto LOC_35F80;
	SetCoordinateByReference_Cylind(v1, a0, a2, v2, 8);
	Delay(1);
	a2 = ((a2 + 1) & 255);
	v2 = (v2 - 8);
	goto LOC_35ED8;
LOC_35F80:
	SetObjectFadeOut(v1, 16, 1);
	return;
}

void XCreateFirePillarLight(int a0, int a1, int a2, int a3, int a4) {
	int v1;
	int v2;
	int v3;
LOC_35FAC:
	v1 = CreateObjectByReference(a0, 41009, 0, 0);
	SetCoordinateByReference_Cylind(v1, a0, a1, a2, 0);
	SetObjectFlags(v1, (33554432 + 16777216));
	SetCallbackProcedure(v1, 43001);
	SetObjectScale(v1, 32768, 24576);
	v3 = CreateObjectByReference(a0, 41010, 0, 0);
	SetCoordinateByReference_Cylind(v3, a0, a1, a2, 0);
	SetObjectScale(v3, 32768, 16384);
	v2 = 0;
LOC_36120:
	if (!(v2 < a4)) goto LOC_362A8;
	a1 = ((a1 + 4) & 255);
	a2 = (a2 + 6);
	Delay(1);
	SetCoordinateByReference_Cylind(v1, a0, a1, a2, 0);
	SetCoordinateByReference_Cylind(v3, a0, a1, a2, 0);
	SetCoordinateByReference_Cylind(a3, a0, a1, a2, 0);
	if (!(v2 == (a4 - 16))) goto LOC_36298;
	SetObjectFadeOut(v1, 16, 1);
	SetObjectFadeOut(v3, 16, 1);
LOC_36298:
	v2++;
	goto LOC_36120;
LOC_362A8:
	return;
}

void XCreateFirePillarBomb(int a0, int a1, int a2, int a3) {
	int v1;
	a0 = CreateObjectByReference(a0, 2501, 0, 0);
	v1 = CreateObjectByReference(a0, 41008, 0, 0);
	SetCoordinateByReference_Cylind(v1, a0, a1, a2, 0);
	SetObjectScale(v1, 32768, 65536);
	SetObjectFadeOut(v1, a3, 1);
	XCreateFirePillarLight(a0, a1, a2, v1, a3);
	FreeObjectByHandle(a0);
	return;
}

void XCreateFirePillar(int a0, int a1) {
	int v1;
	int v2;
	int v3;
	int v4;
LOC_363E8:
	v1 = CreateObjectRaw(a0, a1, 0, 0, 2501);
	v3 = Rand(0, 255);
	v2 = 0;
LOC_36460:
	if (!(v2 < 4)) goto LOC_364E0;
	asynccall XCreateFirePillarSource(v1, 320, v3);
	v3 = ((v3 + 64) & 255);
	v2++;
	goto LOC_36460;
LOC_364E0:
	Wait("XCreateFirePillarSource");
	Delay(8);
	asynccall LockTargetXY(a0, a1, 2501, 0, 60, 43001, 2, 3, (33554432 + 16777216));
	asynccall LockTargetXY((a0 + 60), a1, 2501, 60, 2, 43001, 2, 3, 33554432);
	asynccall LockTargetXY((a0 - 60), a1, 2501, 60, 2, 43001, 2, 3, 33554432);
	asynccall LockTargetXY(a0, (a1 + 50), 2501, 60, 2, 43001, 2, 3, 33554432);
	asynccall LockTargetXY(a0, (a1 + 50), 2501, 60, 2, 43001, 2, 3, 33554432);
	asynccall LockTargetXY((a0 + 60), (a1 + 50), 2501, 60, 2, 43001, 2, 3, 33554432);
	asynccall LockTargetXY((a0 + 60), (a1 - 50), 2501, 60, 2, 43001, 2, 3, 33554432);
	asynccall LockTargetXY((a0 - 60), (a1 + 50), 2501, 60, 2, 43001, 2, 3, 33554432);
	asynccall LockTargetXY((a0 - 60), (a1 - 50), 2501, 60, 2, 43001, 2, 3, 33554432);
	asynccall LockTargetXY((a0 + 120), a1, 2501, 60, 2, 43001, 2, 3, 33554432);
	asynccall LockTargetXY((a0 - 120), a1, 2501, 60, 2, 43001, 2, 3, 33554432);
	asynccall LockTargetXY(a0, (a1 + 100), 2501, 60, 2, 43001, 2, 3, 33554432);
	asynccall LockTargetXY(a0, (a1 + 100), 2501, 60, 2, 43001, 2, 3, 33554432);
	asynccall LockTargetXY((a0 + 120), (a1 + 100), 2501, 60, 2, 43001, 2, 3, 33554432);
	asynccall LockTargetXY((a0 + 120), (a1 - 100), 2501, 60, 2, 43001, 2, 3, 33554432);
	asynccall LockTargetXY((a0 - 120), (a1 + 100), 2501, 60, 2, 43001, 2, 3, 33554432);
	asynccall LockTargetXY((a0 - 120), (a1 - 100), 2501, 60, 2, 43001, 2, 3, 33554432);
	PlaySound1("m031snd01", 255);
	v2 = 0;
LOC_36C6C:
	if (!(v2 < 4)) goto LOC_36D18;
	v4 = CreateObjectByReference(v1, (41002 + v2), 0, (-12));
	Delay(4);
	if (!(v2 < 3)) goto LOC_36D08;
	FreeObjectByHandle(v4);
LOC_36D08:
	v2++;
	goto LOC_36C6C;
LOC_36D18:
	v2 = 65536;
LOC_36D28:
	if (!(v2 < 131072)) goto LOC_36D98;
	SetObjectScale(v4, 65536, v2);
	Delay(2);
	v2 = (v2 + 8192);
	goto LOC_36D28;
LOC_36D98:
	Delay(40);
	PlaySound1("m031snd02", 255);
	asynccall XCreateFirePillarBomb(v4, 32, 10, 70);
	asynccall XCreateFirePillarBomb(v4, 96, 10, 70);
	asynccall XCreateFirePillarBomb(v4, 160, 10, 70);
	asynccall XCreateFirePillarBomb(v4, 224, 10, 70);
	SetObjectScaleShrink(v4, 4096);
	SetObjectFadeOut(v4, 16, 1);
	FreeObjectByHandle(v1);
	return;
}

void XFirePillar(int a0) {
	int v1;
	int v2;
	int v3;
	SetGlobal(118, 1);
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
	if (!(v3 == 0)) goto LOC_36F70;
	v3 = intvDefenderMajor;
LOC_36F70:
	v1 = GetObjectScreenX(v3);
	v2 = GetObjectScreenY(v3);
	MoveCamera(v1, (v2 - 180), 60);
	XCreateFirePillar(v1, v2);
	Wait("XCreateFirePillarBomb");
	Delay(60);
	return;
}

void XTaiChiCallback(int a0, int a1) callsign 43002 {
	if (!(a1 == intvDefenderMajor)) goto LOC_370B8;
	if (!(GetGlobal(119) < 4)) goto LOC_37094;
	Hurt(a0, a1, (2 / GetGlobal(119)));
	SetGlobal(119, (GetGlobal(119) * 2));
	goto LOC_370B0;
LOC_37094:
	ClearObjectFlags(a0, 16777216);
LOC_370B0:
	goto LOC_370D8;
LOC_370B8:
	Hurt(a0, a1, 0);
LOC_370D8:
	return;
}

void XCreateTaiChiBomb(int a0, int a1) {
	int v1;
	int v2;
	int v3;
	Delay(8);
	v1 = CreateObjectByReference(a0, 35002, 0, 0);
	SetObjectScale(v1, 4096, 4096);
	if (!a1) goto LOC_371A0;
	SetCallbackProcedure(v1, 43002);
	SetObjectFlags(v1, (33554432 + 16777216));
LOC_371A0:
	SetObjectScaleShrink(v1, (-4096));
	Delay(20);
	v2 = GetObjectScreenX(v1);
	v3 = GetObjectScreenY(v1);
	asynccall LockTargetXY((v2 + 64), v3, 2501, 0, 20, 43002, 0, 0, 33554432);
	asynccall LockTargetXY((v2 - 64), v3, 2501, 0, 20, 43002, 0, 0, 33554432);
	asynccall LockTargetXY(v2, (v3 + 50), 2501, 0, 20, 43002, 0, 0, 33554432);
	asynccall LockTargetXY(v2, (v3 - 50), 2501, 0, 20, 43002, 0, 0, 33554432);
	asynccall LockTargetXY((v2 + 64), (v3 + 50), 2501, 0, 20, 43002, 0, 0, 33554432);
	asynccall LockTargetXY((v2 + 64), (v3 - 50), 2501, 0, 20, 43002, 0, 0, 33554432);
	asynccall LockTargetXY((v2 - 64), (v3 + 50), 2501, 0, 20, 43002, 0, 0, 33554432);
	asynccall LockTargetXY((v2 - 64), (v3 - 50), 2501, 0, 20, 43002, 0, 0, 33554432);
	asynccall LockTargetXY((v2 + 128), v3, 2501, 0, 20, 43002, 0, 0, 33554432);
	asynccall LockTargetXY((v2 - 128), v3, 2501, 0, 20, 43002, 0, 0, 33554432);
	asynccall LockTargetXY(v2, (v3 + 100), 2501, 0, 20, 43002, 0, 0, 33554432);
	asynccall LockTargetXY(v2, (v3 - 100), 2501, 0, 20, 43002, 0, 0, 33554432);
	asynccall LockTargetXY((v2 + 128), (v3 + 100), 2501, 0, 20, 43002, 0, 0, 33554432);
	asynccall LockTargetXY((v2 + 128), (v3 - 100), 2501, 0, 20, 43002, 0, 0, 33554432);
	asynccall LockTargetXY((v2 - 128), (v3 + 100), 2501, 0, 20, 43002, 0, 0, 33554432);
	asynccall LockTargetXY((v2 - 128), (v3 - 100), 2501, 0, 20, 43002, 0, 0, 33554432);
	asynccall LockTargetXY(v2, (v3 + 150), 2501, 0, 16, 43002, 0, 0, 33554432);
	Delay(20);
	SetObjectFadeOut(v1, 16, 1);
	return;
}

void XCreateTaichi(int a0, int a1) {
	int v1;
	int v2;
	v1 = CreateObjectRaw(a0, a1, 0, 0, 35001);
	SetObjectLayer(v1, 2);
	SetObjectScale(v1, 98304, 98304);
	v2 = 0;
LOC_37A18:
	if (!(v2 <= 16)) goto LOC_37A6C;
	SetObjectOpacity(v1, v2);
	Delay(1);
	v2++;
	goto LOC_37A18;
LOC_37A6C:
	PlaySound1("m025snd01", 220);
	v2 = 0;
LOC_37A98:
	if (!(v2 < 3)) goto LOC_37B14;
	asynccall XCreateTaiChiBomb(v1, (v2 == 0));
	Delay((10 - v2));
	v2++;
	goto LOC_37A98;
LOC_37B14:
	Delay(30);
	v2 = 16;
LOC_37B30:
	if (!(v2 >= 0)) goto LOC_37B84;
	SetObjectOpacity(v1, v2);
	Delay(1);
	v2--;
	goto LOC_37B30;
LOC_37B84:
	FreeObjectByHandle(v1);
	return;
}

void XTaiChi(int a0) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	int v7;
	int v8;
	int v9;
	int v10;
	int v11;
	int v12;
	int v13;
	int v14;
	int v15;
	SetGlobal(119, 1);
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	v4 = 0;
LOC_37C00:
	if (!(v4 < (a0 + 1))) goto LOC_388C8;
	v5 = 0;
LOC_37C38:
	if (!(v5 < 5)) goto LOC_388AC;
	if (!(v5 == 0)) goto LOC_37D24;
	v3 = GetRandomSoldierFromAlive((intvIsLeft ^ 1));
	if (!(v3 == 0)) goto LOC_37CC4;
	v3 = intvDefenderMajor;
LOC_37CC4:
	v6 = GetObjectScreenX(v3);
	v7 = GetObjectScreenY(v3);
	v1 = v6;
	v2 = v7;
	goto LOC_38850;
LOC_37D24:
	if (!(v5 == 1)) goto LOC_37FF8;
	v1 = (v6 + Rand(200, 260));
	v2 = (v7 + Rand(220, 270));
	v9 = ScreenXToBattleX(v1);
	v10 = ScreenXToBattleX(v2);
	v8 = GetSoldierCountInRect((intvIsLeft ^ 1), v9, v10, 3, 3);
	if (!(v8 > 0)) goto LOC_37FF0;
	v12 = 0;
LOC_37E4C:
	if (!(v12 < (v8 / 2))) goto LOC_37FF0;
	v11 = GetNthSoldierInRect((intvIsLeft ^ 1), v9, v10, 3, 3, Rand(0, (v8 - 1)));
	if (!(v11 != intvDefenderMajor)) goto LOC_37FE0;
	v13 = GetObjectScreenX(v11);
	v14 = GetObjectScreenY(v11);
	v15 = (((v13 - v6) * (v13 - v6)) + ((v14 - v7) * (v14 - v7)));
	if (!(v15 >= 72200)) goto LOC_37FE0;
	v1 = v13;
	v2 = v14;
	goto LOC_37FF0;
LOC_37FE0:
	v12++;
	goto LOC_37E4C;
LOC_37FF0:
	goto LOC_38850;
LOC_37FF8:
	if (!(v5 == 2)) goto LOC_382CC;
	v1 = (v6 + Rand(200, 260));
	v2 = (v7 - Rand(220, 270));
	v9 = ScreenXToBattleX(v1);
	v10 = ScreenXToBattleX(v2);
	v8 = GetSoldierCountInRect((intvIsLeft ^ 1), v9, v10, 3, 3);
	if (!(v8 > 0)) goto LOC_382C4;
	v12 = 0;
LOC_38120:
	if (!(v12 < (v8 / 2))) goto LOC_382C4;
	v11 = GetNthSoldierInRect((intvIsLeft ^ 1), v9, v10, 3, 3, Rand(0, (v8 - 1)));
	if (!(v11 != intvDefenderMajor)) goto LOC_382B4;
	v13 = GetObjectScreenX(v11);
	v14 = GetObjectScreenY(v11);
	v15 = (((v13 - v6) * (v13 - v6)) + ((v14 - v7) * (v14 - v7)));
	if (!(v15 >= 72200)) goto LOC_382B4;
	v1 = v13;
	v2 = v14;
	goto LOC_382C4;
LOC_382B4:
	v12++;
	goto LOC_38120;
LOC_382C4:
	goto LOC_38850;
LOC_382CC:
	if (!(v5 == 3)) goto LOC_385A0;
	v1 = (v6 - Rand(200, 260));
	v2 = (v7 + Rand(220, 270));
	v9 = ScreenXToBattleX(v1);
	v10 = ScreenXToBattleX(v2);
	v8 = GetSoldierCountInRect((intvIsLeft ^ 1), v9, v10, 3, 3);
	if (!(v8 > 0)) goto LOC_38598;
	v12 = 0;
LOC_383F4:
	if (!(v12 < (v8 / 2))) goto LOC_38598;
	v11 = GetNthSoldierInRect((intvIsLeft ^ 1), v9, v10, 3, 3, Rand(0, (v8 - 1)));
	if (!(v11 != intvDefenderMajor)) goto LOC_38588;
	v13 = GetObjectScreenX(v11);
	v14 = GetObjectScreenY(v11);
	v15 = (((v13 - v6) * (v13 - v6)) + ((v14 - v7) * (v14 - v7)));
	if (!(v15 >= 72200)) goto LOC_38588;
	v1 = v13;
	v2 = v14;
	goto LOC_38598;
LOC_38588:
	v12++;
	goto LOC_383F4;
LOC_38598:
	goto LOC_38850;
LOC_385A0:
	v1 = (v6 - Rand(200, 260));
	v2 = (v7 - Rand(220, 270));
	v9 = ScreenXToBattleX(v1);
	v10 = ScreenXToBattleX(v2);
	v8 = GetSoldierCountInRect((intvIsLeft ^ 1), v9, v10, 3, 3);
	if (!(v8 > 0)) goto LOC_38850;
	v12 = 0;
LOC_386AC:
	if (!(v12 < (v8 / 2))) goto LOC_38850;
	v11 = GetNthSoldierInRect((intvIsLeft ^ 1), v9, v10, 3, 3, Rand(0, (v8 - 1)));
	if (!(v11 != intvDefenderMajor)) goto LOC_38840;
	v13 = GetObjectScreenX(v11);
	v14 = GetObjectScreenY(v11);
	v15 = (((v13 - v6) * (v13 - v6)) + ((v14 - v7) * (v14 - v7)));
	if (!(v15 >= 72200)) goto LOC_38840;
	v1 = v13;
	v2 = v14;
	goto LOC_38850;
LOC_38840:
	v12++;
	goto LOC_386AC;
LOC_38850:
	asynccall XCreateTaichi(v1, v2);
	Delay(Rand(8, 12));
	v5++;
	goto LOC_37C38;
LOC_388AC:
	Delay(45);
	v4++;
	goto LOC_37C00;
LOC_388C8:
	Wait("XCreateTaichi");
	Delay(60);
	return;
}

void XDuplicatorCallback(int a0, int a1) callsign 43003 {
	Hurt(a0, a1, intvMagicAttackValue);
	if (!((a1 == intvAttackerMajor) || (a1 == intvDefenderMajor))) goto LOC_38A58;
	asynccall SmallFireBall(a0, a1, 12, 0);
	Delay(4);
	asynccall SmallFireBall(a0, a1, 12, 0);
	Delay(4);
	asynccall SmallFireBall(a0, a1, 12, 0);
	Delay(4);
	asynccall SmallFireBall(a0, a1, 12, 0);
	Delay(4);
	asynccall SmallFireBall(a0, a1, 12, 0);
	goto LOC_38A84;
LOC_38A58:
	asynccall Blood(a1, 16);
LOC_38A84:
	return;
}

void XTraceDuplicator(int a0, int a1) {
	int v1;
	int v2;
	int v3;
	int v4;
	v4 = 0;
LOC_38AA4:
	if (!IsObjectExist(a0)) goto LOC_38C50;
	Delay(1);
	if (!(IsObjectExist(a1) == 0)) goto LOC_38AFC;
	goto LOC_38C50;
LOC_38AFC:
	v1 = GetObjectScreenX(a0);
	v2 = GetObjectScreenX(a1);
	v3 = (v2 - v1);
	if (!(v3 < 0)) goto LOC_38B80;
	v3 = (-v3);
LOC_38B80:
	if (!((v3 < 360) && (v4 < 2))) goto LOC_38BE8;
	SetObjectAnimate(a0, 4);
	v4 = 2;
	goto LOC_38C48;
LOC_38BE8:
	if (!((v3 < 1080) && (v4 < 1))) goto LOC_38C48;
	SetObjectAnimate(a0, 2);
	v4 = 1;
LOC_38C48:
	goto LOC_38AA4;
LOC_38C50:
	return;
}

void XMoveDuplicator(int a0, int a1, int a2) {
	int v1;
	int v2;
	int v3;
	v3 = GetObjectDir(a0);
	v2 = CreateObjectByReference(a0, a1, v3, 0);
	SetCoordinateByReference_Cylind(v2, a0, v3, (48 + 64), 0);
	v1 = a2;
LOC_38D00:
	SetCoordinateByReference_Cylind(v2, a0, v3, (48 + 64), 0);
	Delay(1);
	if (!(!IsObjectExist(a0))) goto LOC_38D74;
	goto LOC_38D98;
LOC_38D74:
	v1--;
	if ((v1 >= 0)) goto LOC_38D00;
LOC_38D98:
	FreeObjectByHandle(v2);
	return;
}

void XDuplicatorShadow(int a0, int a1, int a2, int a3, int a4) {
	int v1;
	if (!(a4 == 0)) goto LOC_38E60;
	return;
LOC_38E60:
	v1 = CreateObjectByReference(a0, a1, a2, 0);
	if (!(a2 == 0)) goto LOC_38EE8;
	MoveObject(v1, (-8), 0, 0);
	goto LOC_38F14;
LOC_38EE8:
	MoveObject(v1, 8, 0, 0);
LOC_38F14:
	SetObjectSpeed_Sphere(v1, a2, 0, a3);
	SetObjectFlags(v1, 262144);
	SetObjectOpacity(v1, a4);
	Delay(2);
	SetObjectFadeOut(v1, 20, 1);
LOC_38FA8:
	if (!IsObjectExist(a0)) goto LOC_39008;
	if (!(IsObjectExist(v1) == 0)) goto LOC_38FF4;
	return;
LOC_38FF4:
	Delay(1);
	goto LOC_38FA8;
LOC_39008:
	FreeObjectByHandle(v1);
	return;
}

void XProduceDuplicator(int a0, int a1, int a2, int a3) {
LOC_38DB4:
	if (!IsObjectExist(a0)) goto LOC_38E2C;
	asynccall XDuplicatorShadow(a0, a1, a2, (a3 - 8), 14);
	Delay(8);
	goto LOC_38DB4;
LOC_38E2C:
	return;
}

void XDuplicator(int a0) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	int v7;
	int v8;
	int v9;
	int v10;
	v10 = 0;
	if (!(Rand(0, 3) == 3)) goto LOC_39090;
	v10 = Rand(0, 90);
LOC_39090:
	v9 = (1 - (2 * (intvIsLeft != 0)));
	if (!(intvIsLeft == 1)) goto LOC_390F8;
	v8 = 0;
	goto LOC_39108;
LOC_390F8:
	v8 = 128;
LOC_39108:
	v5 = GetObjectScreenX(intvAttackerMajor);
	v7 = GetObjectScreenY(intvAttackerMajor);
	PlaySound(intvAttackerMajor, "att07", 255);
	v2 = CreateObjectByReference(intvAttackerMajor, 22002, v8, 0);
	SetObjectSpeed_Sphere(v2, v8, 0, 16);
	SetObjectFlags(v2, (16777216 + 33554432));
	SetCallbackProcedure(v2, 43003);
	asynccall ProduceShadowTime(v2, 9999);
	asynccall XMoveDuplicator(v2, 22001, 9999);
	v5 = 16;
	v4 = 1;
	v3 = 0;
	asynccall XTraceDuplicator(v2, intvDefenderMajor);
LOC_392BC:
	SetObjectSpeed_Sphere(v2, v8, 0, (v5 / 16));
	if (!(v5 < (24 * 16))) goto LOC_39354;
	v5 = (v5 + v4);
	v4 = (v4 + 1);
LOC_39354:
	if (!((v3 & 1) == 0)) goto LOC_393E8;
	if (!(v5 < 255)) goto LOC_393C4;
	PlaySound(v2, "m012snd01", 255);
	goto LOC_393E8;
LOC_393C4:
	PlaySound(v2, "m012snd01", 255);
LOC_393E8:
	v3++;
	Delay(1);
	v1 = IsObjectExist(v2);
	v6 = GetObjectScreenX(v2);
	if (!((v6 <= 50) || (v6 >= (GetBattleWidthInScreenX() - 51)))) goto LOC_3948C;
	FreeObjectByHandle(v2);
LOC_3948C:
	if (v1) goto LOC_392BC;
	Delay(60);
	return;
}

void FinalX2() {
	int v1;
	int v2;
LOC_394B0:
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\012\\*");
	BatchLoadShape("magic\\025\\*");
	BatchLoadShape("magic\\031\\*");
	DisablePlayMagic();
	DownBrightness(8, 5);
	SetOverwhelming(intvAttackerMajor, 1);
	v1 = GetObjectScreenX(intvAttackerMajor);
	v2 = GetObjectScreenY(intvAttackerMajor);
	SetViewCamera(v1, (v2 - 120));
	AddAttackCounter(intvAttackerMajor, 2);
	Delay(15);
	SetObjectAnimate(intvAttackerMajor, 32768);
	Delay(20);
	SetOverwhelming(intvAttackerMajor, 0);
	asynccall XDuplicator(0);
	Delay(20);
	asynccall XFirePillar(0);
	asynccall XTaiChi(2);
	Wait("XDuplicator");
	Wait("XFirePillar");
	Wait("XTaiChi");
	Delay(10);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void XProduceSoldier(int a0, int a1) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v6;
	int v7;
	v4 = 0;
	v1 = 0;
	v6 = GetBattleWidth();
	v7 = GetBattleHeight();
LOC_39750:
	v2 = (v7 / 2);
LOC_3976C:
	if (!(a0 == 1)) goto LOC_397B4;
	v3 = GetForceHandleByXY(v1, v2);
	goto LOC_397F0;
LOC_397B4:
	v3 = GetForceHandleByXY(((v6 - 1) - v1), v2);
LOC_397F0:
	if (!(v3 == 0)) goto LOC_39924;
	if (!(a0 == 1)) goto LOC_39888;
	v5 = CreateSoldier(a0, v1, v2);
	asynccall StepShow(v5, 10);
	goto LOC_398F8;
LOC_39888:
	v5 = CreateSoldier(a0, ((v6 - 1) - v1), v2);
	asynccall StepShow(v5, 10);
LOC_398F8:
	v4++;
	if (!(v4 >= a1)) goto LOC_39924;
	return;
LOC_39924:
	if (!(intvIsLeft == 1)) goto LOC_39984;
	v3 = GetForceHandleByXY(v1, ((v7 - 1) - v2));
	goto LOC_399D8;
LOC_39984:
	v3 = GetForceHandleByXY(((v6 - 1) - v1), ((v7 - 1) - v2));
LOC_399D8:
	if (!(v3 == 0)) goto LOC_39B3C;
	if (!(a0 == 1)) goto LOC_39A88;
	v5 = CreateSoldier(a0, v1, ((v7 - 1) - v2));
	asynccall StepShow(v5, 10);
	goto LOC_39B10;
LOC_39A88:
	v5 = CreateSoldier(a0, ((v6 - 1) - v1), ((v7 - 1) - v2));
	asynccall StepShow(v5, 10);
LOC_39B10:
	v4++;
	if (!(v4 >= a1)) goto LOC_39B3C;
	return;
LOC_39B3C:
	v2--;
	if ((v2 > 0)) goto LOC_3976C;
	v1++;
	if ((v1 <= v6)) goto LOC_39750;
	return;
}

void XMoreSoldier(int a0) {
	int v1;
	int v2;
	int v3;
	int v4;
	int v5;
	int v7;
LOC_39B8C:
	BatchLoadShape("magic\\000\\*");
	BatchLoadShape("magic\\002\\*");
	DisablePlayMagic();
	DownBrightness(8, 5);
	v1 = intvAttackerMajor;
	v3 = intvIsLeft;
	v4 = GetObjectScreenX(v1);
	v5 = GetObjectScreenY(v1);
	SetViewCamera(v4, (v5 - 120));
	AddAttackCounter(v1, 2);
	Delay(20);
	if (!(v3 == 1)) goto LOC_39CBC;
	v2 = 0;
	goto LOC_39CCC;
LOC_39CBC:
	v2 = 128;
LOC_39CCC:
	v7 = (200 - GetSoldierCount(v3));
	if (!(v7 < 0)) goto LOC_39D20;
	v7 = 0;
LOC_39D20:
	if (!(v7 > a0)) goto LOC_39D4C;
	v7 = a0;
LOC_39D4C:
	if (!(v7 != 0)) goto LOC_39F0C;
	asynccall XProduceSoldier(v3, v7);
	SetObjectAnimate(v1, 32768);
	Delay(24);
	SetObjectAnimate(v1, 65536);
	SetOverwhelming(v1, 0);
	Delay(6);
	PlaySound1("m002snd01", 255);
	PlaySound1("m002snd03", 255);
	if (!(intvIsLeft == 0)) goto LOC_39E7C;
	MoveCamera(6576, 0, 40);
	goto LOC_39E9C;
LOC_39E7C:
	MoveCamera(240, 0, 40);
LOC_39E9C:
	Delay(100);
	v4 = GetObjectScreenX(intvAttackerMajor);
	v5 = GetObjectScreenY(intvAttackerMajor);
	MoveCamera(v4, (v5 - 120), 20);
LOC_39F0C:
	Delay(20);
	RaiseBrightness(16, 5);
	EnablePlayMagic();
	return;
}

void F103() callsign 103 {
	if (!(GetGlobal(120) < 501)) goto LOC_39F70;
	SetGlobal(120, 501);
LOC_39F70:
	intvMagicAttackValue = GetGlobal(121);
	DoScript(GetGlobal(120), 0, 0, 0);
	return;
}

void F104() callsign 104 {
	if (!(GetGlobal(120) < 501)) goto LOC_39FE8;
	SetGlobal(120, 501);
	goto LOC_39FF0;
LOC_39FE8:
	SetGlobal(120, GetGlobal(120) + 1);
LOC_39FF0:
	if (!(GetGlobal(120) > 610)) goto LOC_3A01C;
	SetGlobal(120, 501);
LOC_3A01C:
	DoScript(GetGlobal(120), 0, 0, 0);
	return;
}

void F100() callsign 100 {
	if (!(GetGlobal(120) < 501)) goto LOC_3A07C;
	SetGlobal(120, 501);
LOC_3A07C:
	intvMagicAttackValue = GetGlobal(121);
	DoScript(GetGlobal(120), 0, 0, 0);
	return;
}

void F200() callsign 200 {
	if (!(GetGlobal(120) < 501)) goto LOC_3A0F4;
	SetGlobal(120, 501);
	goto LOC_3A0FC;
LOC_3A0F4:
	SetGlobal(120, GetGlobal(120) + 1);
LOC_3A0FC:
	if (!(GetGlobal(120) > 610)) goto LOC_3A128;
	SetGlobal(120, 501);
LOC_3A128:
	DoScript(GetGlobal(120), 0, 0, 0);
	return;
}

void F9999(int a0, int a1) callsign 9999 {
	SetGlobal(121, a1);
	SetGlobal(120, a0);
	return;
}

void InitializeBattle() {
	int v1;
	if (!(GetGlobal(120) == 0)) goto LOC_3A1B8;
	SetGlobal(120, 501);
LOC_3A1B8:
	SetGlobal(3, 0);
	SetGlobal(1, 0);
	SetGlobal(28 + 0, 0);
	SetGlobal(28 + 1, 0);
	SetGlobal(35 + 0, 0);
	SetGlobal(35 + 1, 0);
	v1 = 0;
LOC_3A248:
	if (!(v1 < 60)) goto LOC_3A28C;
	SetGlobal(55 + v1, 0);
	v1++;
	goto LOC_3A248;
LOC_3A28C:
	return;
}

void Magic1() callsign 501 {
	Heal(0);
	return;
}

void Magic2() callsign 502 {
	ShootObject(0, 1);
	return;
}

void Magic3() callsign 503 {
	ArrowSupport(17, 1);
	return;
}

void Magic4() callsign 504 {
	Spout(0);
	return;
}

void Magic5() callsign 505 {
	Powder(0);
	return;
}

void Magic6() callsign 506 {
	RollTub(0);
	return;
}

void Magic7() callsign 507 {
	OnFire(0);
	return;
}

void Magic8() callsign 508 {
	ExplodeRound(18, 160, 18003);
	return;
}

void Magic9() callsign 509 {
	MoreSoldier(17, 0);
	return;
}

void Magic10() callsign 510 {
	Thunder(20, 0);
	return;
}

void Magic11() callsign 511 {
	Thunder(9, 1);
	return;
}

void Magic12() callsign 512 {
	FireDragon(0);
	return;
}

void Magic13() callsign 513 {
	Interceptor(0);
	return;
}

void Magic14() callsign 514 {
	ShootObject(1, 0);
	return;
}

void Magic15() callsign 515 {
	ArrowSupport(29, 1);
	return;
}

void Magic16() callsign 516 {
	Spout(1);
	return;
}

void Magic17() callsign 517 {
	Powder(1);
	return;
}

void Magic18() callsign 518 {
	RollTub(1);
	return;
}

void Magic19() callsign 519 {
	ExplodeRound(32, 240, 18003);
	return;
}

void Magic20() callsign 520 {
	FireDragon(1);
	return;
}

void Magic21() callsign 521 {
	OnFire(1);
	return;
}

void Magic22() callsign 522 {
	Thunder(16, 1);
	return;
}

void Magic23() callsign 523 {
	MoreSoldier(32, 0);
	return;
}

void Magic24() callsign 524 {
	BackSoldier(25, 0);
	return;
}

void Magic25() callsign 525 {
	Thunder(62, 0);
	return;
}

void Magic26() callsign 526 {
	CallDragon(2);
	return;
}

void Magic27() callsign 527 {
	Interceptor(1);
	return;
}

void Magic28() callsign 528 {
	FireDragon(2);
	return;
}

void Magic29() callsign 529 {
	OnFire(2);
	return;
}

void Magic30() callsign 530 {
	Powder(2);
	return;
}

void Magic31() callsign 531 {
	ExplodeRound(48, 380, 18003);
	return;
}

void Magic32() callsign 532 {
	Spout(2);
	return;
}

void Magic33() callsign 533 {
	BackSoldier(35, 0);
	return;
}

void Magic34() callsign 534 {
	ArrowSupport(66, 1);
	return;
}

void Magic35() callsign 535 {
	RollTub(3);
	return;
}

void Magic36() callsign 536 {
	FireDragon(3);
	return;
}

void Magic37() callsign 537 {
	BigThunder();
	return;
}

void Magic38() callsign 538 {
	Thunder(40, 1);
	return;
}

void Magic39() callsign 539 {
	Interceptor(2);
	return;
}

void Magic40() callsign 540 {
	Earthquake();
	return;
}

void Magic41() callsign 541 {
	BackSoldier(45, 0);
	return;
}

void Magic42() callsign 542 {
	FireDragon(4);
	return;
}

void Magic43() callsign 543 {
	Sparkle(0);
	return;
}

void Magic44() callsign 544 {
	FinalX1();
	return;
}

void Magic45() callsign 545 {
	ArrowSupport(8, 0);
	return;
}

void Magic46() callsign 546 {
	RunningBow(0);
	return;
}

void Magic47() callsign 547 {
	StoneEmitter(0, 0, 9);
	return;
}

void Magic48() callsign 548 {
	ConvexStone(0);
	return;
}

void Magic49() callsign 549 {
	FireRing(0, 0);
	return;
}

void Magic50() callsign 550 {
	HalfMoon(0);
	return;
}

void Magic51() callsign 551 {
	Ice(0);
	return;
}

void Magic52() callsign 552 {
	ShootObject(0, 0);
	return;
}

void Magic53() callsign 553 {
	Slash(0);
	return;
}

void Magic54() callsign 554 {
	Tornado(0);
	return;
}

void Magic55() callsign 555 {
	RollDown(0);
	return;
}

void Magic56() callsign 556 {
	EightWayFire(2, 0, 19002);
	return;
}

void Magic57() callsign 557 {
	TaiChi(0);
	return;
}

void Magic58() callsign 558 {
	ShootObject(2, 0);
	return;
}

void Magic59() callsign 559 {
	FireCow(0);
	return;
}

void Magic60() callsign 560 {
	FireWork(0);
	return;
}

void Magic61() callsign 561 {
	FlyingSword(0);
	return;
}

void Magic62() callsign 562 {
	Spear(0);
	return;
}

void Magic63() callsign 563 {
	Heal(1);
	return;
}

void Magic64() callsign 564 {
	RunningBow(1);
	return;
}

void Magic65() callsign 565 {
	MoreSoldier(25, 0);
	return;
}

void Magic66() callsign 566 {
	BlackHole(0);
	return;
}

void Magic67() callsign 567 {
	RushCart(0);
	return;
}

void Magic68() callsign 568 {
	EightWayFire(2, 1, 19002);
	return;
}

void Magic69() callsign 569 {
	FireRing(1, 0);
	return;
}

void Magic70() callsign 570 {
	FlyingSword(1);
	return;
}

void Magic71() callsign 571 {
	FireRing(0, 1);
	return;
}

void Magic72() callsign 572 {
	ArrowSupport(33, 0);
	return;
}

void Magic73() callsign 573 {
	TaiChi(1);
	return;
}

void Magic74() callsign 574 {
	FireCow(1);
	return;
}

void Magic75() callsign 575 {
	ConvexStone(1);
	return;
}

void Magic76() callsign 576 {
	RollDown(1);
	return;
}

void Magic77() callsign 577 {
	HalfMoon(1);
	return;
}

void Magic78() callsign 578 {
	FireWork(1);
	return;
}

void Magic79() callsign 579 {
	Ice(1);
	return;
}

void Magic80() callsign 580 {
	Tornado(1);
	return;
}

void Magic81() callsign 581 {
	Slash(1);
	return;
}

void Magic82() callsign 582 {
	Spear(1);
	return;
}

void Magic83() callsign 583 {
	StoneEmitter(1, 1, 14);
	return;
}

void Magic84() callsign 584 {
	MoreSoldier(42, 0);
	return;
}

void Magic85() callsign 585 {
	Frozen();
	return;
}

void Magic86() callsign 586 {
	RushCart(1);
	return;
}

void Magic87() callsign 587 {
	RunningBow(2);
	return;
}

void Magic88() callsign 588 {
	Tornado(2);
	return;
}

void Magic89() callsign 589 {
	BlackHole(1);
	return;
}

void Magic90() callsign 590 {
	ArrowSupport(56, 0);
	return;
}

void Magic91() callsign 591 {
	FireRing(1, 1);
	return;
}

void Magic92() callsign 592 {
	HalfMoonNew(0);
	return;
}

void Magic93() callsign 593 {
	ConvexStone(2);
	return;
}

void Magic94() callsign 594 {
	Pa(0);
	return;
}

void Magic95() callsign 595 {
	FlyingSword(2);
	return;
}

void Magic96() callsign 596 {
	FireCow(2);
	return;
}

void Magic97() callsign 597 {
	RollDown(2);
	return;
}

void Magic98() callsign 598 {
	Duplicator(1);
	return;
}

void Magic99() callsign 599 {
	FireWork(2);
	return;
}

void Magic100() callsign 600 {
	Wall(0);
	return;
}

void Magic101() callsign 601 {
	Ice(2);
	return;
}

void Magic102() callsign 602 {
	HalfMoon(2);
	return;
}

void Magic103() callsign 603 {
	Spear(2);
	return;
}

void Magic104() callsign 604 {
	StoneEmitter(2, 1, 20);
	return;
}

void Magic105() callsign 605 {
	DemonDancing();
	return;
}

void Magic106() callsign 606 {
	TwisterSword();
	return;
}

void Magic107() callsign 607 {
	FirePillar(0);
	return;
}

void Magic108() callsign 608 {
	Wall(1);
	return;
}

void Magic109() callsign 609 {
	HalfMoonNew(1);
	return;
}

void Magic110() callsign 610 {
	FinalX2();
	return;
}

void Magic200() callsign 700 {
	XMoreSoldier(200);
	return;
}

