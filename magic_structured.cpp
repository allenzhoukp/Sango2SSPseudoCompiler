#define OAF_WAIT      1
#define OAF_WALK      2
#define OAF_DEFENSE   8
#define OAF_PAIN      0x10
#define OAF_DEATH     0x20
#define OAF_WIN       0x40
#define OAF_ATTACK2   0x800
#define OAF_ATTACK3   0x1000
#define OAF_ATTACK4   0x2000
#define OAF_ATTACK5   0x4000
#define OAF_SPELL1    0x8000
#define OAF_SPELL2    0x10000
#define OAF_SHOT1     0x20000
#define OAF_SHOT2     0x40000
#define OAF_SHOT3     0x80000
#define OAF_ATTACK6   0x100000
#define OAF_ATTACK7   0x200000
#define OAF_ATTACK8   0x400000
#define OAF_ATTACK    4

#define OF_MAN              0x00000001
#define OF_MISSILE          0x00000002
#define OF_BACKGROUND       0x00000004
#define OF_CAVALRY          0x00000008
#define OF_BLOCKMAN         0x00000010
#define OF_BLOCKMISSILE     0x00000020
#define OF_BIGSHAPE         0x00000040
#define OF_TRIGGERMAN       0x00000100
#define OF_TRIGGERMISSILE   0x00000200
#define OF_NOGRAVITY        0x00010000
#define OF_NONSYNCHRONISM   0x00020000
#define OF_MIXER            0x00040000
#define OF_WHITELIGHT       0x00080000
#define OF_DARKLIGHT        0x00100000
#define OF_ENEMYGENERAL     0x01000000
#define OF_ENEMYFORCE       0x02000000
#define OF_MYGENERAL        0x04000000
#define OF_MYFORCE          0x08000000
#define OF_ATTACKENEMY      0x03000000
#define OF_ATTACKMY         0x0c000000
#define OF_ATTACKALL        0x0F000000
#define OF_TARGET           0x00200000
#define OF_BOMB             0x00400000

void DelayAmbientSound(string a0, int a1, int a2)  {
    SetGlobal(1, 1);
    PlaySound1(a0, a1);
    Delay(a2);
    SetGlobal(1, 0);
    return;
}
int GetSoldierMaxX2(int a0)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    v1 = GetSoldierCount((a0 ^ 1));
    if (!(v1 > 0)) {
        return (-1);
    } else {
        v2 = ((v1 / 2) + 1);
        v4 = GetRandomSoldierHandleFromAlive((a0 ^ 1));
        v3 = GetObjectScreenX(v4);
        do {
            v4 = GetRandomSoldierHandleFromAlive((a0 ^ 1));
            v5 = GetObjectScreenX(v4);
            if (!(a0 == 1)) {
                if (v3 < v5) {
                    v3 = v5;
                }
            } else {
                if (v3 > v5) {
                    v3 = v5;
                }
            }
            v2--;
        } while (v2 > 0);
        v3 = ScreenXToBattleX(v3);
        v3 = BattleXToScreenX(v3);
        return v3;
    }
}
int GetSoldierMaxX(int a0)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    v1 = GetSoldierCount((a0 ^ 1));
    if (!(v1 > 0)) {
        if (!(a0 == 1)) {
            v5 = GetObjectScreenX(intvDefenderMajor);
            v3 = (v5 + 320);
        } else {
            v5 = GetObjectScreenX(intvDefenderMajor);
            v3 = (v5 - 320);
        }
    } else {
        v2 = ((v1 / 2) + 1);
        v4 = GetRandomSoldierHandleFromAlive((a0 ^ 1));
        v3 = GetObjectScreenX(v4);
        do {
            v4 = GetRandomSoldierHandleFromAlive((a0 ^ 1));
            v5 = GetObjectScreenX(v4);
            if (!(a0 == 1)) {
                if (v3 < v5) {
                    v3 = v5;
                }
            } else {
                if (v3 > v5) {
                    v3 = v5;
                }
            }
            v2--;
        } while (v2 > 0);
    }
    return v3;
}
void HoldAllDelay(int a0)  {
    SetAttackCounterTo9999();
    Delay(a0);
    RestoreAttackCounter();
    return;
}
void ProduceShadowTime(int a0, int a1)  {
    int v1;
    v1 = a1;
    do {
        CreateShadowObject(a0, 12, 1);
        if (!IsObjectExist(a0)) {
            break;
        }
        Delay(1);
        v1--;
    } while (v1 >= 0);
    return;
}
void MovingShadow(int a0, int a1, int a2, int a3, int a4)  {
    int v1;
    int v2;
    v2 = CreateObjectByReference(a0, a2, a3, 0);
    SetObjectFlags(v2, OF_ENEMYFORCE);
    SetCallbackProcedure(v2, a4);
    v1 = 1;
    do {
        CreateShadowObject1(v2, 4, 1, 8192);
        SetCoordinateByReference_Cylind(v2, a0, a3, (v1 * 16), 0);
        Delay(1);
        v1++;
    } while (v1 <= a1);
    SetObjectFadeOut(v2, 16, 1);
    return;
}
void LockByCenter(int a0, int a1, int a2, int a3, int a4, int a5, int a6)  {
    int v2;
    v2 = CreateObjectByReference(a0, a1, a2, 0);
    SetCoordinateByReference_Cylind(v2, a0, a2, a3, 0);
    SetObjectFlags(v2, a5);
    SetCallbackProcedure(v2, a6);
    Delay(a4);
    FreeObject(v2);
    return;
}
void LockTargetXY(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8)  {
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
void LockTargetTime2(int a0, int a1, int a2)  {
    int v1;
    int v2;
    int v3;
    int v4;
    if (!((a0 == intvDefenderMajor) || (a0 == intvAttackerMajor))) {
        v4 = 0;
    } else {
        if (!(GetMajorHP(GetSoldierSide(a0)) != 0)) {
            v4 = 0;
        } else {
            v4 = 40;
        }
    }
    v3 = GetObjectDir(a0);
    v2 = CreateObjectByReference(a0, a1, v3, 0);
    v1 = a2;
    do {
        if (!IsObjectExist(a0)) {
            break;
        }
        SetCoordinateByReference_Cylind(v2, a0, 0, 0, v4);
        Delay(1);
        v1--;
    } while (v1 >= 0);
    SetObjectFadeOut(v2, 16, 1);
    return;
}
void Hurt(int a0, int a1, int a2)  {
    if (!((a1 == intvAttackerMajor) || (a1 == intvDefenderMajor))) {
        PlaySound(a1, "Hit01", 255);
    } else {
        ClearObjectFlags(a0, (OF_MYGENERAL + OF_ENEMYGENERAL));
        if (a2 <= 0) {
            return;
        }
        DoHarmToMajor(a1, 0, a2);
    }
    return;
}
void MoveCamera(int a0, int a1, int a2)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    v2 = (GetCameraX() * 1000);
    v3 = (GetCameraY() * 1000);
    a0 = (a0 * 1000);
    a1 = (a1 * 1000);
    v4 = (a0 - v2);
    v4 = (v4 / a2);
    v5 = (a1 - v3);
    v5 = (v5 / a2);
    v1 = a2;
    while ((v1 > 0)) {
        v2 = (v2 + v4);
        v3 = (v3 + v5);
        SetViewCamera((v2 / 1000), (v3 / 1000));
        if (((v2 / 100) == (a0 / 100)) && ((v3 / 100) == (a1 / 100))) {
            v1 = 1;
        }
        v1--;
        Delay(1);
    }
    return;
}
void LockCameraSimple(int a0, int a1)  {
    int v1;
    int v2;
    v2 = GetCameraX();
    while (IsObjectExist(a0)) {
        v1 = GetObjectScreenX(a0);
        if (((v1 - v2) > 40) || ((v1 - v2) < 40)) {
            SetViewCamera(v1, (GetObjectScreenY(a0) + a1));
        }
        Delay(1);
        v2 = GetCameraX();
    }
    return;
}
void LockCameraSimple2(int a0, int a1)  {
    int v1;
    int v2;
    v2 = GetCameraX();
    while (IsObjectExist(a0)) {
        v1 = GetObjectScreenX(a0);
        if (((v1 - v2) > 10) || ((v1 - v2) < 10)) {
            SetViewCamera(v1, (GetObjectScreenY(a0) + a1));
        }
        Delay(1);
        v2 = GetCameraX();
    }
    return;
}
void LockCameraLine(int a0, int a1, int a2, int a3)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    v5 = 0;
    v2 = GetCameraX();
    v3 = GetCameraY();
    if (!(a2 == 1)) {
        v4 = (-100);
    } else {
        v4 = 100;
    }
    v1 = GetObjectScreenX(a0);
    while (1) {
        if (!(a2 == 1)) {
            v1 = (v1 - 8);
            if (v1 < 270) {
                break;
            }
        } else {
            v1 = (v1 + 8);
            if (v1 > (GetBattleWidthInScreenX() - 270)) {
                break;
            }
        }
        if (((v1 - v2) > 40) || ((v1 - v2) < 40)) {
            SetViewCamera((v1 + v4), (v3 + a1));
        }
        Delay(1);
        v2 = GetCameraX();
        v5++;
        if (v5 > a3) {
            break;
        }
    }
    return;
}
void LockCameraLineDelay(int a0, int a1, int a2, int a3)  {
    int v1;
    int v2;
    int v3;
    int v4;
    v2 = GetCameraX();
    v3 = GetCameraY();
    if (!(a2 == 1)) {
        v4 = (-100);
    } else {
        v4 = 100;
    } while (IsObjectExist(a0)) {
        v1 = GetObjectScreenX(a0);
        if (!(a2 == 1)) {
            if (v1 < GetObjectScreenX(intvDefenderMajor)) {
                break;
            }
        } else {
            if (v1 > GetObjectScreenX(intvDefenderMajor)) {
                break;
            }
        }
        if (((v1 - v2) > 40) || ((v1 - v2) < 40)) {
            SetViewCamera((v1 + v4), (v3 + a1));
        }
        Delay(1);
        a3--;
        if (a3 <= 0) {
            break;
        }
        v2 = GetCameraX();
    }
    return;
}
void LockCamera(int a0, int a1, int a2, int a3)  {
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
    while ((v3 == 1)) {
        v2 = GetObjectScreenX(a0);
        v4 = GetCameraX();
        if (!(a3 == 0)) {
            if (!((v2 - v4) >= a1)) {
                v5 = v4;
            } else {
                v5 = (v2 - a1);
                v6 = (v6 + 1);
            }
        } else {
            if (!((v4 - v2) >= a1)) {
                v5 = v4;
            } else {
                v5 = (v2 + a1);
                v6 = (v6 - 1);
            }
        }
        SetViewCamera((v5 + v6), (GetObjectScreenY(a0) + a2));
        Delay(1);
        v3 = IsObjectExist(a0);
        if (!(v1 == 1)) {
            if (!(a3 == 0)) {
                if (v4 >= GetObjectScreenX(intvDefenderMajor)) {
                    v3 = 0;
                }
            } else {
                if (v4 <= GetObjectScreenX(intvDefenderMajor)) {
                    v3 = 0;
                }
            }
        } else {
            v3 = 0;
        }
    }
    if (v1 == 0) {
        if (!(a3 == 0)) {
            a0 = intvDefenderMajor;
        } else {
            a0 = intvDefenderMajor;
        }
        asynccall MoveCamera(GetObjectScreenX(a0), (GetObjectScreenY(a0) - 120), 30);
    }
    return;
}
void LockCamera2(int a0, int a1, int a2, int a3, int a4)  {
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
    while ((v3 == 1)) {
        v2 = GetObjectScreenX(a0);
        v4 = GetCameraX();
        if (!(a3 == 0)) {
            if (!((v2 - v4) >= a1)) {
                v5 = v4;
            } else {
                v5 = (v2 - a1);
                v6 = (v6 + 1);
            }
        } else {
            if (!((v4 - v2) >= a1)) {
                v5 = v4;
            } else {
                v5 = (v2 + a1);
                v6 = (v6 - 1);
            }
        }
        SetViewCamera((v5 + v6), (GetObjectScreenY(a0) + a2));
        Delay(1);
        v3 = IsObjectExist(a0);
        if (!(v1 == 1)) {
            if (!(a3 == 0)) {
                if (v4 >= GetObjectScreenX(intvDefenderMajor)) {
                    v3 = 0;
                }
            } else {
                if (v4 <= GetObjectScreenX(intvDefenderMajor)) {
                    v3 = 0;
                }
            }
        } else {
            v3 = 0;
        }
    }
    v7 = GetCameraX();
    v8 = GetCameraY();
    while (IsObjectExist(a4)) {
        v2 = GetObjectScreenX(a4);
        if (!(a3 == 1)) {
            if (v2 <= (v7 + 40)) {
                break;
            }
        } else {
            if (v2 >= (v7 - 40)) {
                break;
            }
        }
        Delay(2);
    }
    v8 = (v8 - GetObjectScreenY(a4));
    if (IsObjectExist(a4)) {
        LockCameraSimple2(a4, v8);
    }
    return;
}
void FireMan(int a0, int a1, int a2)  {
    int v1;
    int v2;
    int v3;
    v2 = GetObjectDir(a0);
    v3 = CreateObjectByReference(a0, a1, v2, 0);
    v1 = 0;
    while ((v1 < a2)) {
        SetCoordinateByReference_Cylind(v3, a0, 0, 0, 0);
        Delay(1);
        if (IsObjectExist(a0) == 0) {
            break;
        }
        v1++;
    }
    SetObjectFadeOut(v3, 16, 1);
    return;
}
void CreateBlood(int a0, int a1, int a2)  {
    int v1;
    v1 = CreateObjectByReference(a0, 10012, 0, a1);
    SetObjectScale(v1, 32768, 32768);
    SetObjectSpeed_Sphere(v1, a2, Rand(24, 40), 7);
    Delay(2);
    SetObjectFadeOut(v1, 8, 1);
    return;
}
void Blood(int a0, int a1)  {
    int v1;
    int v2;
    v2 = (128 * Rand(0, 1));
    v1 = 0;
    while ((v1 < 32)) {
        asynccall CreateBlood(a0, a1, v2);
        Delay(Rand(0, 1));
        v1++;
    }
    return;
}
void SmallFireBall(int a0, int a1, int a2, int a3)  {
    int v1;
    int v2;
    PlaySound(a1, "hit01", 255);
    v2 = 0;
    while ((v2 < a2)) {
        v1 = CreateObjectByReference(a1, 10003, Rand(0, 255), a3);
        SetObjectScale(v1, 12288, 12288);
        SetObjectSpeed_Cylind(v1, Rand(4, 12), Rand((-2), 8));
        SetObjectFriction1(v1, 8192);
        SetObjectFadeOut(v1, 32, 1);
        SetObjectScaleShrink(v1, 256);
        v2++;
    }
    return;
}
void SmallFireBall2(int a0, int a1, int a2, int a3)  {
    int v1;
    int v2;
    int v3;
    PlaySound(a1, "hit01", 255);
    v2 = 0;
    while ((v2 < a2)) {
        v1 = CreateObjectByReference(a1, 10003, Rand(0, 255), a3);
        v3 = Rand(8192, 12288);
        SetObjectScale(v1, v3, v3);
        SetObjectSpeed_Cylind(v1, Rand(3, 7), Rand((-1), 5));
        SetObjectFriction1(v1, 8192);
        SetObjectFadeOut(v1, 32, 1);
        SetObjectScaleShrink(v1, 256);
        v2++;
    }
    return;
}
void SmallFireBall3(int a0, int a1, int a2, int a3)  {
    int v1;
    int v2;
    PlaySound(a1, "hit01", 255);
    v2 = 0;
    while ((v2 < a2)) {
        v1 = CreateObjectByReference(a1, 10013, Rand(0, 255), a3);
        SetObjectScale(v1, 12288, 12288);
        SetObjectSpeed_Cylind(v1, Rand(4, 12), Rand((-2), 8));
        SetObjectFriction1(v1, 8192);
        SetObjectFadeOut(v1, 32, 1);
        SetObjectScaleShrink(v1, 256);
        v2++;
    }
    return;
}
void SmallFireBall4(int a0, int a1, int a2, int a3)  {
    int v1;
    int v2;
    PlaySound(a1, "hit01", 255);
    v2 = 0;
    while ((v2 < a2)) {
        v1 = CreateObjectByReference(a1, 10014, Rand(0, 255), a3);
        SetObjectScale(v1, 12288, 12288);
        SetObjectSpeed_Cylind(v1, Rand(4, 12), Rand((-2), 8));
        SetObjectFriction1(v1, 8192);
        SetObjectFadeOut(v1, 32, 1);
        SetObjectScaleShrink(v1, 256);
        v2++;
    }
    return;
}
void DownBrightness(int a0, int a1)  {
    int v1;
    v1 = GetBackgroundBrightness();
    while ((v1 > a0)) {
        Delay(a1);
        v1--;
        SetBackgroundBrightness(v1);
    }
    SetBackgroundBrightness(16);
    LockBrightness(a0);
    SetGlobal(2, a0);
    return;
}
void RaiseBrightness(int a0, int a1)  {
    int v1;
    UnlockBrightness();
    v1 = GetGlobal(2);
    SetBackgroundBrightness(v1);
    while ((v1 < a0)) {
        Delay(a1);
        v1++;
        SetBackgroundBrightness(v1);
    }
    SetBackgroundBrightness(a0);
    return;
}
void CreateDot(int a0)  {
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
void CreateDots(int a0)  {
    int v1;
    int v2;
    v2 = 0;
    while ((v2 < 10)) {
        v1 = 0;
        while ((v1 < 4)) {
            asynccall CreateDot(a0);
            Delay(Rand(2, 4));
            v1++;
        }
        v2++;
    }
    return;
}
void CreateLine(int a0, int a1)  {
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
case 0:
        v6 = 10005;
        a1 = 128;
        v5 = Rand(51, 72);
        v2 = (v2 + v5);
        break;
case 1:
        v6 = 10006;
        a1 = 128;
        v5 = Rand(52, 77);
        v2 = (v2 + v5);
        v4 = (v4 + v5);
        break;
case 2:
        v6 = 10007;
        a1 = 0;
        v5 = Rand(50, 71);
        v4 = (v4 + v5);
        break;
case 3:
        v6 = 10006;
        a1 = 0;
        v5 = Rand(50, 70);
        v2 = (v2 - v5);
        v4 = (v4 + v5);
        break;
case 4:
        v6 = 10005;
        a1 = 0;
        v5 = Rand(52, 70);
        v2 = (v2 - v5);
        break;
case 5:
        v6 = 10009;
        a1 = 0;
        v5 = Rand(52, 73);
        v2 = (v2 - v5);
        v4 = (v4 - v5);
        break;
case 6:
        v6 = 10008;
        a1 = 0;
        v5 = Rand(51, 77);
        v4 = (v4 - v5);
        break;
case 7:
        v6 = 10009;
        a1 = 128;
        v5 = Rand(50, 70);
        v2 = (v2 + v5);
        v4 = (v4 - v5);
        break;
        break;
    }
    v1 = CreateObjectByReference(a0, v6, a1, 0);
    SetObjectCoordinate(v1, v2, v3, v4);
    ApproachObjectTowards(v1, a0, Rand(3, 5));
    SetObjectFadeOut(v1, 16, 1);
    return;
}
void CreateLines(int a0)  {
    int v1;
    int v2[20];
    v1 = 0;
    while ((v1 < 20)) {
        v2[v1] = (Rand(0, 1023) & 7);
        if (v1 > 0) {
            if (v2[v1] == v2[(v1 - 1)]) {
                v1--;
            }
        }
        v1++;
    }
    v1 = 0;
    while ((v1 < 20)) {
        asynccall CreateLine(a0, v2[v1]);
        Delay(Rand(6, 8));
        v1++;
    }
    return;
}
void PowerExplode(int a0, int a1)  {
    int v1;
    v1 = CreateObjectByReference(a0, 10011, 0, 0);
    SetObjectSpeed_Sphere(v1, 0, (a1 * 32), 2);
    CreateShadowObject(v1, 4, 2);
    Delay(5);
    SetObjectFadeOut(v1, 16, 1);
    return;
}
void KeepPower(int a0, int a1)  {
    int v1;
    int v2;
    if (!(a1 == 1)) {
        a1 = (-1);
    } else {
        a1 = 1;
    }
    v1 = CreateObjectByReference(a0, 10010, 0, 0);
    a1 = (a1 * 47);
    SetCoordinateByReference_Cylind(v1, a0, 0, a1, 110);
    SetObjectOpacity(v1, 12);
    asynccall CreateDots(v1);
    Delay(10);
    asynccall CreateLines(v1);
    Delay(160);
    v2 = 11;
    while ((v2 >= 0)) {
        SetObjectOpacity(v1, v2);
        Delay(1);
        v2--;
    }
    return;
}
void StepShow(int a0, int a1)  {
    int v1;
    SetObjectFlags(a0, OF_MIXER);
    SetObjectOpacity(a0, 0);
    Delay(20);
    v1 = 1;
    while ((v1 <= 16)) {
        Delay(a1);
        SetObjectOpacity(a0, v1);
        v1++;
    }
    ClearObjectFlags(a0, OF_MIXER);
    return;
}
void HitGeneral(int a0, int a1, int a2, int a3, int a4, int a5)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    if (!GetGlobal(3)) {
        SetGlobal(3, 1);
        v2 = CreateObjectByReference(a1, 2501, 0, 0);
        v3 = GetObjectScreenX(a1);
        v4 = GetObjectScreenY(a1);
        SetObjectCoordinate(v2, v3, v4, a4);
        v1 = 0;
        while ((v1 < 7)) {
            v5 = CreateObjectByReference(v2, a2, 0, 0);
            SetObjectScale(v5, 49152, 49152);
            MoveObject(v5, Rand((-48), 48), 0, Rand((-48), 48));
            switch (a3) {
            case 1:
                asynccall SmallFireBall(a0, a1, 8, a4);
                break;
            case 2:
                asynccall SmallFireBall3(a0, a1, 8, a4);
                break;
            case 3:
                asynccall SmallFireBall4(a0, a1, 8, a4);
                break;
                break;
            }
            Delay(Rand(3, 7));
            v1++;
        }
        FreeObject(v2);
        Delay(a5);
        SetGlobal(3, 0);
        return;
    } else {
        return;
    }
}
void DisableAttack(int a0, int a1, int a2)  {
    ClearObjectFlags(a0, OF_ATTACKALL);
    Delay(a2);
    SetObjectFlags(a0, a1);
    return;
}
void NoMoreSoldierCallback(int a0, int a1) callsign 1000  {
    int v1;
    int v2;
    PlaySound1("m000snd01", 255);
    ClearObjectFlags(a0, OF_MYGENERAL);
    SetObjectAnimate(a1, OAF_PAIN);
    v2 = GetObjectDir(a0);
    v1 = CreateObjectByReference(a0, 10018, v2, 16);
    SetObjectGravity(v1, 10752);
    if (!(intvIsLeft == 1)) {
        v2 = 83;
    } else {
        v2 = 45;
    }
    SetObjectSpeed_Sphere(v1, 0, v2, 5);
    SetObjectFadeOut(v1, 16, 3);
    FreeObject(a0);
    return;
}
void CastFail(int a0, int a1)  {
    int v1;
    v1 = CreateObjectByReference(a0, 10017, a1, 256);
    SetObjectFlags(v1, OF_MYGENERAL);
    SetCallbackProcedure(v1, 1000);
    SetObjectAnimate(a0, OAF_SPELL1);
    SetOverwhelming(a0, 0);
    Delay(60);
    return;
}
void HalfMoonCallback(int a0, int a1) callsign 1001  {
    Hurt(a0, a1, intvMagicAttackValue);
    if (!(a1 == intvDefenderMajor)) {
        asynccall FireMan(a1, 10015, 60);
        asynccall SmallFireBall(a0, a1, 8, 0);
    } else {
        SetObjectFadeOut(a0, 2, 1);
        HitGeneral(a0, a1, 11002, 1, 48, 0);
    }
    return;
}
void CreateHalfMoonSmoke(int a0, int a1, int a2, int a3, int a4)  {
    int v1;
    if (!(intvIsLeft == 1)) {
        v1 = CreateObjectByReference(a0, 11003, 128, 0);
    } else {
        v1 = CreateObjectByReference(a0, 11003, 0, 0);
    }
    MoveObject(v1, a1, a2, a3);
    SetObjectScale(v1, 32768, 20480);
    SetObjectSpeed_Cylind(v1, 14, Rand(1, 2));
    Delay(1);
    SetObjectFadeOut(v1, 16, 1);
    SetObjectScaleShrink(v1, ((-2048) - a4));
    while (IsObjectExist(v1)) {
        Delay(1);
        if (IsObjectExist(a0) == 0) {
            FreeObject(v1);
            break;
        }
    }
    return;
}
void ShowHalfMoonSmoke(int a0)  {
    int v1;
    v1 = 0;
    while (IsObjectExist(a0)) {
        if (!(intvIsLeft == 1)) {
            asynccall CreateHalfMoonSmoke(a0, ((7 * Rand(8, 6)) + ((v1 * 2) * Rand(2, 4))), 0, (-2), (256 * v1));
        } else {
            asynccall CreateHalfMoonSmoke(a0, ((7 * Rand((-8), (-6))) - ((v1 * 2) * Rand(2, 4))), 0, (-2), (256 * v1));
        }
        Delay(Rand(1, 1));
        v1 = ((v1 + 1) % 12);
    }
    return;
}
void HalfMoonAll(int a0, int a1, int a2, int a3, int a4)  {
    int v1;
    int v2;
    v1 = CreateObjectByReference(a0, 11001, a1, 0);
    SetCoordinateByReference_Cylind(v1, a0, a3, a4, 1);
    SetObjectSpeed_Sphere(v1, a1, 0, a2);
    SetObjectFlags(v1, OF_ENEMYFORCE);
    SetCallbackProcedure(v1, 1001);
    asynccall ShowHalfMoonSmoke(v1);
    v2 = 600;
    do {
        if (!IsObjectExist(a0)) {
            v2 = 0;
        }
        Delay(1);
        v2--;
    } while (v2 >= 0);
    Delay(120);
    SetObjectFadeOut(v1, 16, 1);
    return;
}
void HalfMoon(int a0)  {
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
    BatchLoadShape("magic\\000\\*");
    BatchLoadShape("magic\\001\\*");
    DisablePlayMagic();
    DownBrightness(12, 5);
    v1 = intvAttackerMajor;
    v2 = intvDefenderMajor;
    v3 = intvIsLeft;
    v11 = 0;
    if (Rand(0, 3) == 3) {
        v11 = Rand(0, 90);
    }
    v6 = 18;
    if (!(v3 == 1)) {
        v5 = 128;
        v12 = 1;
    } else {
        v5 = 0;
        v12 = (-1);
    }
    switch (a0) {
    case 0:
        v9 = (120 - v11);
        break;
    case 1:
        v9 = (210 - v11);
        break;
    case 2:
        v9 = (330 - v11);
        break;
        break;
    }
    SetOverwhelming(v1, 1);
    v7 = GetObjectScreenX(v1);
    v8 = GetObjectScreenY(v1);
    SetViewCamera((v7 - (v12 * 180)), (v8 - v9));
    AddAttackCounter(v1, 2);
    Delay(15);
    SetObjectAnimate(v1, OAF_ATTACK3);
    Delay(20);
    SetOverwhelming(v1, 0);
    PlaySound(v1, "m001snd01", 255);
    v4 = CreateObjectByReference(v1, 11001, v5, 0);
    SetCoordinateByReference_Cylind(v4, v1, v5, 64, 1);
    SetObjectSpeed_Sphere(v4, v5, 0, v6);
    SetObjectFlags(v4, (OF_ENEMYGENERAL | OF_ENEMYFORCE));
    SetCallbackProcedure(v4, 1001);
    asynccall ShowHalfMoonSmoke(v4);
    switch (a0) {
    case 0:
        asynccall LockCamera(v4, 180, (-v9), v3);
        break;
    case 1:
        asynccall HalfMoonAll(v4, v5, v6, 48, (v12 * 140));
        asynccall HalfMoonAll(v4, v5, v6, 208, (v12 * 140));
        asynccall LockCamera(v4, 180, (-v9), v3);
        break;
    case 2:
        asynccall HalfMoonAll(v4, v5, v6, 48, (140 * v12));
        asynccall HalfMoonAll(v4, v5, v6, 48, (280 * v12));
        asynccall HalfMoonAll(v4, v5, v6, 208, (140 * v12));
        asynccall HalfMoonAll(v4, v5, v6, 208, (280 * v12));
        asynccall LockCamera(v4, 180, (-v9), v3);
        break;
        break;
    }
    do {
        v10 = IsObjectExist(v4);
        if (v10 == 1) {
            v7 = GetObjectScreenX(v4);
            if ((v7 <= 0) || (v7 >= GetBattleWidthInScreenX())) {
                FreeObject(v4);
            }
        }
        Delay(1);
    } while (v10 != 0);
    Delay(70);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void ProduceSoldier(int a0, int a1)  {
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
    do {
        v2 = (v7 / 2);
        do {
            if (!(a0 == 1)) {
                v3 = GetForceHandleByXY(((v6 - 1) - v1), v2);
            } else {
                v3 = GetForceHandleByXY(v1, v2);
            }
            if (v3 == 0) {
                if (!(a0 == 1)) {
                    v5 = CreateSoldier(a0, ((v6 - 1) - v1), v2);
                    asynccall StepShow(v5, 10);
                } else {
                    v5 = CreateSoldier(a0, v1, v2);
                    asynccall StepShow(v5, 10);
                }
                v4++;
                if (v4 >= a1) {
                    return;
                }
            }
            if (!(intvIsLeft == 1)) {
                v3 = GetForceHandleByXY(((v6 - 1) - v1), ((v7 - 1) - v2));
            } else {
                v3 = GetForceHandleByXY(v1, ((v7 - 1) - v2));
            }
            if (v3 == 0) {
                if (!(a0 == 1)) {
                    v5 = CreateSoldier(a0, ((v6 - 1) - v1), ((v7 - 1) - v2));
                    asynccall StepShow(v5, 10);
                } else {
                    v5 = CreateSoldier(a0, v1, ((v7 - 1) - v2));
                    asynccall StepShow(v5, 10);
                }
                v4++;
                if (v4 >= a1) {
                    return;
                }
            }
            v2--;
        } while (v2 > 0);
        v1++;
    } while (v1 <= v6);
    return;
}
void MoreSoldier(int a0, int a1)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    int v6;
    int v7;
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
    if (!(v3 == 1)) {
        v2 = 128;
    } else {
        v2 = 0;
    }
    v6 = GetMajorLevel(v3);
    if (v6 > 40) {
        v6 = 40;
    }
    v7 = ((v6 * 5) - GetSoldierCount(v3));
    if (v7 < 0) {
        v7 = 0;
    }
    if (v7 > a0) {
        v7 = a0;
    }
    if (!(v7 != 0)) {
        CastFail(v1, v2);
    } else {
        asynccall ProduceSoldier(v3, v7);
        SetObjectAnimate(v1, OAF_SPELL1);
        Delay(24);
        SetObjectAnimate(v1, OAF_SPELL2);
        SetOverwhelming(v1, 0);
        Delay(6);
        PlaySound1("m002snd01", 255);
        PlaySound1("m002snd03", 255);
        if (!(intvIsLeft == 0)) {
            MoveCamera(240, 0, 40);
        } else {
            MoveCamera(6576, 0, 40);
        }
        Delay(100);
        v4 = GetObjectScreenX(intvAttackerMajor);
        v5 = GetObjectScreenY(intvAttackerMajor);
        MoveCamera(v4, (v5 - 120), 20);
    }
    Delay(8);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void BombLight(int a0, int a1)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    v2 = GetObjectDir(a0);
    switch (a1) {
    case 0:
        v1 = CreateObjectByReference(a0, 7019, v2, 50);
        SetObjectOpacity(v1, 6);
        SetObjectScale(v1, 131072, 131072);
        Delay(10);
        SetObjectFadeOut(v1, 16, 1);
        break;
    case 1:
        v1 = CreateObjectByReference(a0, 13012, v2, 50);
        SetObjectScale(v1, 40960, 40960);
        Delay(20);
        SetObjectFadeOut(v1, 16, 1);
        break;
    case 2:
        v1 = CreateObjectByReference(a0, 7021, v2, 50);
        SetObjectScale(v1, 131072, 131072);
        Delay(20);
        SetObjectFadeOut(v1, 16, 1);
        break;
    case 3:
        v1 = CreateObjectByReference(a0, 7046, v2, 80);
        SetObjectScale(v1, 32768, 32768);
        Delay(4);
        SetObjectFadeOut(v1, 16, 1);
        Delay(4);
        v1 = CreateObjectByReference(a0, 7047, v2, 80);
        if (!(v2 == 0)) {
            MoveObject(v1, (-16), 0, 0);
        } else {
            MoveObject(v1, 16, 0, 0);
        }
        v4 = 32768;
        v5 = 65536;
        SetObjectScale(v1, v4, v5);
        SetObjectFadeOut(v1, 32, 1);
        v3 = 0;
        while ((v3 < 16)) {
            SetObjectScale(v1, v4, v5);
            Delay(1);
            v4 = (v4 + 3072);
            v5 = (v5 - 3840);
            if (v3 == 4) {
                SetObjectSpeed_Cylind(v1, (-1), 0);
            }
            v3++;
        }
        break;
        break;
    }
    return;
}
void ShootObjectCallback(int a0, int a1) callsign 3001  {
    int v8;
    int v9;
    int v10;
    if (!(a1 == intvDefenderMajor)) {
        Hurt(a0, a1, 0);
        asynccall Blood(a1, 16);
    } else {
        v8 = GetObjectContext(a0, 0);
        v9 = GetObjectContext(a0, 1);
        v10 = GetObjectDir(a0);
        if (v8 == 1) {
            DecreaseGatherTick(a1, 50);
        }
        Hurt(a0, a1, intvMagicAttackValue);
        if (!(v8 == 2)) {
            SetObjectFadeOut(a0, 2, 1);
        } else {
            SetObjectFadeOut(a0, 16, 8);
        }
        if (!((v8 == 0) && (v9 == 0))) {
            if (!((v8 == 1) && (v9 == 0))) {
                if (!((v8 == 2) && (v9 == 0))) {
                    if ((v8 == 0) && (v9 == 1)) {
                        HitGeneral(a0, a1, 13008, 2, 60, 0);
                    }
                } else {
                    HitGeneral(a0, a1, 13009, 1, 60, 0);
                }
            } else {
                HitGeneral(a0, a1, 13009, 1, 60, 0);
            }
        } else {
            HitGeneral(a0, a1, 13009, 1, 60, 0);
        }
    }
    return;
}
void ZoomFreq(int a0, int a1)  {
    int v1;
    v1 = 0;
    while (IsObjectExist(a0)) {
        if (!(v1 == 0)) {
            SetObjectScale(a0, 61440, 61440);
            v1 = 0;
        } else {
            SetObjectScale(a0, 40960, 40960);
            v1 = 1;
        }
        Delay(a1);
    }
    return;
}
void CosZoomY(int a0, int a1)  {
    int v1;
    int v2;
    v2 = 0;
    while (IsObjectExist(a0)) {
        v1 = CalcCos(v2, a1);
        SetObjectScale(a0, 32768, v1);
        Delay(1);
        v2 = (v2 + 16);
        v2 = (v2 & 255);
    }
    return;
}
void ScaleShadow(int a0, int a1, int a2, int a3, int a4, int a5)  {
    int v1;
    if (!(a5 == 0)) {
        v1 = CreateObjectByReference(a0, a1, a2, 0);
        if (!(a2 == 0)) {
            MoveObject(v1, 8, 0, 0);
        } else {
            MoveObject(v1, (-8), 0, 0);
        }
        SetObjectSpeed_Sphere(v1, a2, 0, a3);
        SetObjectFlags(v1, OF_MIXER);
        SetObjectOpacity(v1, a5);
        SetObjectScale(v1, a4, a4);
        Delay(2);
        SetObjectFadeOut(v1, 60, 1);
        SetObjectScaleShrink(v1, (-(8192 / 12)));
        while (IsObjectExist(a0)) {
            if (IsObjectExist(v1) == 0) {
                return;
            }
            Delay(4);
        }
        FreeObject(v1);
        return;
    } else {
        return;
    }
}
void FireTail(int a0, int a1, int a2)  {
    int v1;
    v1 = CreateObjectByReference(a0, 2501, a1, 0);
    if (!(a1 == 0)) {
        MoveObject(v1, 16, 0, 0);
    } else {
        MoveObject(v1, (-16), 0, 0);
    }
    SetObjectSpeed_Sphere(v1, a1, 0, a2);
    while (IsObjectExist(a0)) {
        asynccall ScaleShadow(v1, 13010, a1, (a2 - 2), 20480, 14);
        Delay(10);
    }
    FreeObject(v1);
    return;
}
void FlyLight(int a0, int a1, int a2, int a3, int a4)  {
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
    if (!(a3 == 0)) {
        if (a4 == 0) {
            v5 = CreateObjectByReference(v2, 13011, v3, 0);
            SetCoordinateByReference_Cylind(v5, v2, v3, 0, 0);
            SetObjectSpeed_Sphere(v5, v3, 0, 16);
            if (!(intvIsLeft == 1)) {
                MoveObject(v5, (-9), 0, 0);
            } else {
                MoveObject(v5, 9, 0, 0);
            }
            asynccall CosZoomY(v2, 81920);
        }
    } else {
        if (!(a4 == 0)) {
        } else {
            SetObjectScale(v2, 49152, 49152);
            asynccall FireTail(v2, v3, 16);
        }
    }
    v1 = a2;
    do {
        v4 = IsObjectExist(a0);
        if (!v4) {
            v1 = 0;
        }
        Delay(1);
        v1--;
    } while (v1 >= 0);
    if (!(a4 == 2)) {
        FreeObject(v2);
        if (v5) {
            FreeObject(v5);
        }
    } else {
        SetObjectFadeOut(v2, 16, 1);
        if (v5) {
            SetObjectFadeOut(v5, 16, 1);
        }
    }
    return;
}
void ShootObject(int a0, int a1)  {
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
    BatchLoadShape("magic\\000\\*");
    BatchLoadShape("magic\\003\\*");
    DisablePlayMagic();
    DownBrightness(8, 5);
    v1 = intvAttackerMajor;
    v2 = intvDefenderMajor;
    v3 = intvIsLeft;
    v10 = 0;
    if (Rand(0, 3) == 3) {
        v10 = Rand(0, 90);
    }
    if (!(v3 == 1)) {
        v5 = 128;
        v9 = 1;
    } else {
        v5 = 0;
        v9 = (-1);
    }
    SetOverwhelming(v1, 1);
    v7 = GetObjectScreenX(v1);
    v8 = GetObjectScreenY(v1);
    SetViewCamera((v7 - (180 * v9)), ((v8 - 120) + v10));
    Delay(15);
    switch (a0) {
    case 0:
        SetObjectAnimate(v1, OAF_SHOT1);
        SetOverwhelming(v1, 0);
        Delay(15);
        PlaySound(v1, "m003snd01", 255);
        break;
    case 1:
        SetObjectAnimate(v1, OAF_SHOT2);
        SetOverwhelming(v1, 0);
        Delay(15);
        PlaySound(v1, "m003snd02", 255);
        break;
        SetObjectAnimate(v1, OAF_SHOT2);
        SetOverwhelming(v1, 0);
        Delay(15);
        PlaySound(v1, "m003snd03", 255);
        break;
    }
    v4 = CreateObjectByReference(v1, (13001 + (a0 * 2)), v5, 0);
    SetObjectContext(v4, 0, a0);
    SetObjectContext(v4, 1, a1);
    if (!(a0 != 2)) {
        SetCoordinateByReference_Cylind(v4, v1, v5, 36, (80 - 11));
    } else {
        SetCoordinateByReference_Cylind(v4, v1, v5, 80, 80);
    }
    SetObjectSpeed_Sphere(v4, v5, 0, 16);
    SetObjectFlags(v4, (OF_ENEMYGENERAL + (OF_ENEMYFORCE * (a0 == 2))));
    SetCallbackProcedure(v4, 3001);
    if (!(a1 == 0)) {
        asynccall FlyLight(v4, 13007, 9999, a1, a0);
    } else {
        asynccall FlyLight(v4, (13002 + (a0 * 2)), 9999, a1, a0);
    }
    asynccall LockCamera(v4, 180, ((-120) + v10), v3);
    do {
        v6 = IsObjectExist(v4);
        v7 = GetObjectScreenX(v4);
        if ((v7 <= 0) || (v7 >= GetBattleWidthInScreenX())) {
            FreeObject(v4);
            break;
        }
        Delay(1);
    } while (v6 == 1);
    Delay(60);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void RushcartCallback(int a0, int a1) callsign 4001  {
    Hurt(a0, a1, intvMagicAttackValue);
    asynccall Blood(a1, 16);
    return;
}
void CreateRushCart(int a0, int a1, int a2, int a3, int a4)  {
    int v1;
    Delay(Rand(0, 15));
    v1 = CreateObjectByReference(intvAttackerMajor, a2, a3, 0);
    SetObjectCoordinate(v1, a0, a1, 0);
    SetObjectFlags(v1, OF_ENEMYFORCE);
    SetCallbackProcedure(v1, 4001);
    SetObjectSpeed_Sphere(v1, a3, 0, 8);
    SetObjectScale(v1, 98304, 98304);
    if (a4) {
        asynccall LockCameraLineDelay(v1, ((2 - a4) * 80), intvIsLeft, 280);
    }
    Delay(400);
    SetObjectFadeOut(v1, 16, 1);
    return;
}
void RushCart(int a0)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v7;
    int v8;
    int v9;
    int v11;
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
    SetObjectAnimate(v1, OAF_SPELL2);
    SetOverwhelming(v1, 0);
    Delay(30);
    if (!(intvIsLeft == 1)) {
        v11 = 1;
        v4 = 128;
    } else {
        v11 = (-1);
        v4 = 0;
    }
    v9 = GetSoldierMaxX(intvIsLeft);
    if (!(v3 == 1)) {
        SetViewCamera((v9 + 320), 0);
    } else {
        SetViewCamera((v9 - 320), 0);
    }
    Delay(15);
    if (!(a0 == 0)) {
        asynccall CreateRushCart((v9 + (1200 * v11)), (108 - 72), 2501, v4, 0);
        asynccall CreateRushCart((v9 + (1200 * v11)), 108, 14001, v4, 0);
        asynccall CreateRushCart((v9 + (1200 * v11)), (252 - 72), 2501, v4, 0);
        asynccall CreateRushCart((v9 + (1200 * v11)), 252, 14001, v4, 0);
        asynccall CreateRushCart((v9 + (1200 * v11)), (828 - 72), 2501, v4, 0);
        asynccall CreateRushCart((v9 + (1200 * v11)), 828, 14001, v4, 0);
        asynccall CreateRushCart((v9 + (1200 * v11)), (972 - 72), 2501, v4, 0);
        asynccall CreateRushCart((v9 + (1200 * v11)), 972, 14001, v4, (a0 + 1));
    } else {
        asynccall CreateRushCart((v9 + (1200 * v11)), (252 - 72), 2501, v4, 0);
        asynccall CreateRushCart((v9 + (1200 * v11)), 252, 14001, v4, 0);
        asynccall CreateRushCart((v9 + (1200 * v11)), (828 - 72), 2501, v4, 0);
        asynccall CreateRushCart((v9 + (1200 * v11)), 828, 14001, v4, (a0 + 1));
    }
    Wait("CreateRushCart");
    Delay(40);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void LockTargetTime3(int a0, int a1, int a2, int a3, int a4)  {
    int v1;
    int v2;
    int v3;
    v3 = GetObjectDir(a0);
    v2 = CreateObjectByReference(a0, a1, v3, 0);
    SetObjectFlags(v2, a3);
    SetCallbackProcedure(v2, a4);
    v1 = a2;
    do {
        SetCoordinateByReference_Cylind(v2, a0, 0, 0, 0);
        Delay(1);
        v1--;
    } while (v1 >= 0);
    SetObjectFadeOut(v2, 16, 1);
    return;
}
void LockGeneral(int a0, int a1)  {
    int v1;
    int v2;
    ClearObjectFlags(a0, OF_ATTACKALL);
    v1 = CreateObjectByReference(a1, 2501, 0, 0);
    if ((a1 == intvAttackerMajor) || (a1 == intvDefenderMajor)) {
        v2 = GetSoldierSide(a1);
        if (!(GetMajorHP(v2) == 0)) {
            MoveObject(v1, 0, 0, 48);
        } else {
            MoveObject(v1, 0, 0, 0);
        }
    }
    asynccall LockTargetTime2(v1, 15007, 8);
    asynccall SmallFireBall3(a0, v1, 20, 0);
    Delay(8);
    FreeObject(v1);
    return;
}
void BigThunderCallback(int a0, int a1) callsign 5002  {
    int v1;
    int v2;
    Hurt(a0, a1, (intvMagicAttackValue / 2));
    ClearObjectFlags(a0, OF_ATTACKALL);
    v1 = CreateObjectByReference(a1, 2501, 0, 0);
    if ((a1 == intvAttackerMajor) || (a1 == intvDefenderMajor)) {
        v2 = GetSoldierSide(a1);
        if (!(GetMajorHP(v2) == 0)) {
            MoveObject(v1, 0, 0, 48);
        } else {
            MoveObject(v1, 0, 0, 0);
        }
    }
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
    FreeObject(v1);
    return;
}
void ThunderAttachAttack(int a0, int a1, int a2)  {
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
    while ((v4 < 12)) {
        v3 = CreateObjectByReference(a0, 15006, (Rand(0, 1) * 128), 0);
        MoveObject(v3, Rand((-12), 12), 1, Rand(16, 42));
        SetObjectScale(v3, Rand(131072, 196608), 98304);
        SetObjectFadeOut(v3, Rand(16, 32), 1);
        Delay(Rand(1, 2));
        v4++;
    }
    Delay(20);
    LockByCenter(a0, 2501, 0, 0, 2, OF_ENEMYFORCE, 0);
    return;
}
void ThunderAttach(int a0)  {
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
    if (v3) {
        if (GetObjectContext(v3, 0) != 14409) {
            SetObjectContext(v3, 0, 14409);
            asynccall ThunderAttachAttack(v3, v5, v6);
        }
    }
    if (v4) {
        if (GetObjectContext(v4, 0) != 14409) {
            SetObjectContext(v4, 0, 14409);
            asynccall ThunderAttachAttack(v4, v5, v6);
        }
    }
    return;
}
void ThunderCallback(int a0, int a1) callsign 5001  {
    ClearObjectFlags(a0, OF_ATTACKALL);
    if (GetGlobal(4) == 1) {
        asynccall ThunderAttach(a1);
    }
    if (GetGlobal(5) == 0) {
        SetGlobal(5, 1);
        Hurt(a0, a1, intvMagicAttackValue);
    }
    if (!((a1 == intvAttackerMajor) || (a1 == intvDefenderMajor))) {
        asynccall LockTargetTime2(a1, 15007, 8);
        asynccall SmallFireBall3(a0, a1, 15, 0);
    } else {
        asynccall LockGeneral(a0, a1);
    }
    return;
}
void LockTargetTime(int a0, int a1, int a2)  {
    int v1;
    int v2;
    int v3;
    Delay(Rand(40, 70));
    v3 = GetObjectDir(a0);
    v2 = CreateObjectByReference(a0, a1, v3, 0);
    SetObjectFlags(v2, OF_ENEMYFORCE);
    SetCallbackProcedure(v2, 5001);
    SetObjectScale(v2, 98304, 65536);
    v1 = a2;
    do {
        SetCoordinateByReference_Cylind(v2, a0, 0, 0, 0);
        Delay(1);
        v1--;
    } while (v1 >= 0);
    SetObjectFadeOut(v2, 16, 1);
    return;
}
void CreateThunderSparkle(int a0, int a1, int a2)  {
    int v1;
    int v2;
    int v3;
    v3 = CreateObjectRaw(a0, a1, 0, 0, 2501);
    v1 = 8;
    while (v1) {
        v2 = CreateObjectByReference(v3, 10013, Rand(0, 255), a2);
        SetObjectScale(v2, 12288, 12288);
        SetObjectSpeed_Cylind(v2, Rand(4, 12), Rand((-2), 8));
        SetObjectFriction1(v2, 8192);
        SetObjectFadeOut(v2, 32, 1);
        SetObjectScaleShrink(v2, 256);
        v1--;
    }
    FreeObject(v3);
    return;
}
void CreateThunderSmoke(int a0, int a1)  {
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
void LockThunder(int a0, int a1, int a2)  {
    int v1;
    int v2;
    Delay(Rand(40, 70));
    if (Rand(1, 3) == 1) {
        asynccall CreateThunderSmoke(a0, a1);
    }
    if (!GetGlobal(4)) {
        v2 = Rand(15001, 15003);
    } else {
        v2 = 15010;
    }
    v1 = CreateObjectByReference(intvAttackerMajor, v2, a2, 0);
    SetObjectCoordinate(v1, a0, a1, 0);
    SetObjectFlags(v1, OF_ENEMYFORCE);
    SetCallbackProcedure(v1, 5001);
    Delay(4);
    SetObjectFadeOut(v1, 16, 1);
    return;
}
void Thunder(int a0, int a1)  {
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
    SetObjectAnimate(v1, OAF_SPELL1);
    SetOverwhelming(v1, 0);
    Delay(30);
    if (!(intvIsLeft == 1)) {
        v4 = 128;
    } else {
        v4 = 0;
    }
    v17 = GetSoldierCount((intvIsLeft ^ 1));
    v18 = 0;
    if (v17 < a0) {
        v18 = (a0 - v17);
    }
    if (!((a0 - v18) > 0)) {
        v19 = GetObjectScreenX(v2);
        v20 = v19;
        v21 = GetObjectScreenY(v1);
    } else {
        v8 = 0;
        v7 = (a0 - v18);
        do {
            if (!(v8 == 0)) {
                if (!(v8 > 8)) {
                    v14 = GetSoldierCountInRect((intvIsLeft ^ 1), v12, v13, 3, 4);
                    v5 = GetNthSoldierInRect((intvIsLeft ^ 1), v12, v13, 3, 4, Rand(0, (v14 - 1)));
                    if ((v5 == 0) || (v5 == intvDefenderMajor)) {
                        v5 = GetRandomSoldierHandleFromAlive((intvIsLeft ^ 1));
                    }
                } else {
                    v5 = GetRandomSoldierHandleFromAlive((intvIsLeft ^ 1));
                }
            } else {
                v6 = GetMostImportantSoldier((intvIsLeft ^ 1));
                if (v6 == 0) {
                    v6 = intvDefenderMajor;
                }
                v12 = GetObjectXY(v6);
                v13 = GetObjectBattleY(v6);
                v5 = v6;
                v15 = GetObjectScreenX(v5);
                v16 = GetObjectScreenY(v5);
            }
            v9 = GetObjectScreenX(v5);
            v10 = GetObjectScreenY(v5);
            if (!(v8 == 0)) {
                if (v19 < v9) {
                    v19 = v9;
                }
                if (v20 > v9) {
                    v20 = v9;
                }
                if (v21 > v10) {
                    v21 = v10;
                }
            } else {
                v8 = 1;
                v19 = v9;
                v20 = v9;
                v21 = v10;
            }
            if (!GetGlobal(4)) {
                v11 = Rand(15001, 15003);
            } else {
                v11 = 15010;
            }
            asynccall LockTargetTime(v5, v11, 4);
            v7--;
        } while (v7 > 0);
    }
    if (v19 == v20) {
        v19 = (v19 + 320);
        v20 = (v20 - 320);
    }
    v7 = v18;
    if (v7 > 20) {
        v7 = 20;
    }
    do {
        v9 = Rand(v20, v19);
        v10 = Rand(150, 650);
        asynccall LockThunder(v9, v10, (128 * Rand(0, 1)));
        v7--;
    } while (v7 > 0);
    v21 = (v21 - 120);
    if (!(v17 == 0)) {
        MoveCamera(v15, (v16 - 140), 40);
    } else {
        v9 = ((v19 + v20) / 2);
        MoveCamera(v9, v21, 40);
    }
    Delay(60);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void BigThunder()  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    int v6;
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
    SetObjectAnimate(v1, OAF_SPELL1);
    SetOverwhelming(v1, 0);
    Delay(20);
    if (!(v3 == 1)) {
        v4 = 128;
    } else {
        v4 = 0;
    }
    v5 = GetObjectScreenX(v2);
    v6 = GetObjectScreenY(v2);
    PlaySound1("m005snd01", 255);
    MoveCamera(v5, (v6 - 120), 40);
    asynccall LockTargetTime3(v2, 15008, 4, OF_ENEMYGENERAL, 5002);
    Delay(4);
    asynccall LockTargetTime3(v2, 15009, 4, OF_ENEMYGENERAL, 5002);
    Delay(60);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void HideArrow(int a0, int a1)  {
    Delay(Rand(30, 40));
    if (a1) {
        SetObjectFadeOut(a1, 32, 1);
    }
    SetObjectFadeOut(a0, 32, 1);
    return;
}
void TraceArrow(int a0, int a1)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    int v6;
    int v7;
    v7 = 0;
    while (IsObjectExist(a0)) {
        v3 = GetObjectScreenZ(a0);
        if (v3 == 0) {
            v1 = GetObjectScreenX(a0);
            v2 = GetObjectScreenY(a0);
            v5 = GetObjectDir(a0);
            if (!(GetGlobal(7) > 0)) {
                v6 = Rand(16012, 16013);
            } else {
                v6 = Rand(16012, 16018);
                SetGlobal(7, GetGlobal(7) + 1);
            }
            v4 = CreateObjectByReference(a0, v6, v5, 0);
            if ((a1 == 1) && ((Rand(1, 256) & 1) == 0)) {
                if ((v6 == 7027) || (v6 == 7028)) {
                    v7 = CreateObjectByReference(a0, 7045, v5, 0);
                    SetObjectScale(v7, 32768, 49152);
                }
            }
            asynccall HideArrow(v4, v7);
            FreeObject(a0);
            break;
        }
        Delay(1);
    }
    return;
}
void ArrowCallback1(int a0, int a1) callsign 6001  {
    int v1;
    if (!(a1 == intvDefenderMajor)) {
        Hurt(a0, a1, 0);
    } else {
        if (GetGlobal(8)) {
            v1 = (intvMagicAttackValue / GetGlobal(8));
        }
        Hurt(a0, a1, v1);
        if (v1 > 0) {
            SetGlobal(8, (GetGlobal(8) * 2));
        }
    }
    return;
}
void ArrowCallback2(int a0, int a1) callsign 6002  {
    int v3;
    if (!(a1 == intvDefenderMajor)) {
        Hurt(a0, a1, 0);
        asynccall FireMan(a1, 10015, 60);
    } else {
        if (GetGlobal(8)) {
            v3 = (intvMagicAttackValue / GetGlobal(8));
        }
        Hurt(a0, a1, v3);
        if (v3 > 0) {
            SetGlobal(8, (GetGlobal(8) * 2));
            asynccall LockTargetTime2(a1, 10015, 60);
        }
    }
    return;
}
void FlyArrowLU() callsign 6003  {
    int v1;
    int v2;
    v2 = GetScriptLinkedObject();
    v1 = CreateObjectByReference(v2, 16003, 128, 0);
    SetCoordinateByReference_Cylind(v1, v2, 128, 64, 104);
    SetObjectSpeed_Sphere(v1, 128, 32, 32);
    PlaySound(v1, "arrow", 255);
    if (GetGlobal(6) == 1) {
        v1 = CreateObjectByReference(v2, 16009, 128, 0);
        SetCoordinateByReference_Cylind(v1, v2, 128, 64, 104);
        SetObjectSpeed_Sphere(v1, 128, 32, 32);
    }
    return;
}
void FlyArrowRU() callsign 6004  {
    int v1;
    int v2;
    v2 = GetScriptLinkedObject();
    v1 = CreateObjectByReference(intvAttackerMajor, 16003, 0, 0);
    SetCoordinateByReference_Cylind(v1, v2, 0, 64, 104);
    SetObjectSpeed_Sphere(v1, 128, 96, 32);
    PlaySound(v1, "arrow", 255);
    if (GetGlobal(6) == 1) {
        v1 = CreateObjectByReference(v2, 16009, 0, 0);
        SetCoordinateByReference_Cylind(v1, v2, 0, 64, 104);
        SetObjectSpeed_Sphere(v1, 128, 96, 32);
    }
    return;
}
void FlyArrowLB2(int a0, int a1, int a2, int a3, int a4)  {
    int v1;
    Delay(a2);
    v1 = CreateObjectByReference(intvAttackerMajor, 16004, 128, 0);
    SetObjectCoordinate(v1, (a0 + 260), a1, 260);
    SetObjectSpeed_Sphere(v1, 128, (-32), 32);
    if (!a4) {
        ClearObjectFlags(v1, OF_ATTACKALL);
        asynccall TraceArrow(v1, a3);
    } else {
        SetCallbackProcedure(v1, 6001);
    }
    if (a3 == 1) {
        if (a4) {
            SetCallbackProcedure(v1, 6002);
        }
        v1 = CreateObjectByReference(intvAttackerMajor, 16010, 128, 0);
        SetObjectCoordinate(v1, (a0 + 260), a1, 260);
        SetObjectSpeed_Sphere(v1, 128, (-32), 32);
    }
    return;
}
void FlyArrowRB2(int a0, int a1, int a2, int a3, int a4)  {
    int v1;
    Delay(a2);
    v1 = CreateObjectByReference(intvAttackerMajor, 16007, 0, 0);
    SetObjectCoordinate(v1, (a0 - 260), a1, 260);
    SetObjectSpeed_Sphere(v1, 0, (-32), 32);
    if (!a4) {
        ClearObjectFlags(v1, OF_ATTACKALL);
        asynccall TraceArrow(v1, a3);
    } else {
        SetCallbackProcedure(v1, 6001);
    }
    if (a3 == 1) {
        if (a4) {
            SetCallbackProcedure(v1, 6002);
        }
        v1 = CreateObjectByReference(intvAttackerMajor, 16010, 0, 0);
        SetObjectCoordinate(v1, (a0 - 260), a1, 260);
        SetObjectSpeed_Sphere(v1, 0, (-32), 32);
    }
    return;
}
void FlyArrowLB1(int a0, int a1, int a2, int a3)  {
    int v1;
    Delay(a2);
    v1 = CreateObjectByReference(intvAttackerMajor, 16004, 128, 0);
    SetCoordinateByReference_Cylind(v1, a0, 0, 0, 0);
    MoveObject(v1, (260 + 24), 0, 260);
    SetObjectSpeed_Sphere(v1, 128, (-32), 32);
    SetCallbackProcedure(v1, 6001);
    if (a3 == 1) {
        SetCallbackProcedure(v1, 6002);
        v1 = CreateObjectByReference(intvAttackerMajor, 16010, 128, 0);
        SetCoordinateByReference_Cylind(v1, a0, 0, 0, 0);
        MoveObject(v1, (260 + 24), 0, 260);
        SetObjectSpeed_Sphere(v1, 128, (-32), 32);
    }
    return;
}
void FlyArrowRB1(int a0, int a1, int a2, int a3)  {
    int v1;
    Delay(a2);
    v1 = CreateObjectByReference(intvAttackerMajor, 16007, 0, 0);
    SetCoordinateByReference_Cylind(v1, a0, 0, 0, 0);
    MoveObject(v1, ((-260) - 24), 0, 260);
    SetObjectSpeed_Sphere(v1, 0, (-32), 32);
    SetCallbackProcedure(v1, 6001);
    if (a3 == 1) {
        SetCallbackProcedure(v1, 6002);
        v1 = CreateObjectByReference(intvAttackerMajor, 16010, 0, 0);
        SetCoordinateByReference_Cylind(v1, a0, 0, 0, 0);
        MoveObject(v1, ((-260) - 24), 0, 260);
        SetObjectSpeed_Sphere(v1, 0, (-32), 32);
    }
    return;
}
void ChangeRightFirePos(int a0, int a1)  {
    Delay((38 + a1));
    MoveObject(a0, (-14), 0, 31);
    Delay(8);
    MoveObject(a0, 10, 0, 30);
    Delay(8);
    FreeObject(a0);
    return;
}
void ChangeLeftFirePos(int a0, int a1)  {
    Delay((38 + a1));
    MoveObject(a0, 14, 0, 31);
    Delay(8);
    MoveObject(a0, (-10), 0, 30);
    Delay(8);
    FreeObject(a0);
    return;
}
void AddSoldierRight(int a0, int a1, int a2)  {
    int v1;
    Delay(a2);
    v1 = CreateObjectByReference(intvAttackerMajor, 16001, 128, 0);
    SetObjectCoordinate(v1, a0, a1, 0);
    return;
}
void AddSoldierLeft(int a0, int a1, int a2)  {
    int v1;
    Delay(a2);
    v1 = CreateObjectByReference(intvAttackerMajor, 16002, 0, 0);
    SetObjectCoordinate(v1, a0, a1, 0);
    return;
}
void ProduceArrowSoldier(int a0, int a1, int a2)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    int v6;
    v2 = a1;
    v4 = 0;
    v6 = ((GetBattleWidth() * 96) - 240);
    do {
        v1 = 9;
        do {
            if (!(a0 == 1)) {
                v5 = CreateObjectByReference(intvAttackerMajor, 16005, 128, 0);
                SetObjectCoordinate(v5, ((v6 + (96 * 7)) + (v4 * 96)), (180 + (v1 * 72)), 0);
                v3 = Rand(1, 30);
                asynccall AddSoldierRight(((v6 + (96 * 7)) + (v4 * 96)), (180 + (v1 * 72)), v3);
                if (a2 == 1) {
                    v5 = CreateObjectByReference(intvAttackerMajor, 16008, 128, 0);
                    SetObjectCoordinate(v5, (((v6 + (96 * 7)) - 27) + (v4 * 96)), (180 + (v1 * 72)), 14);
                    asynccall ChangeRightFirePos(v5, v3);
                }
            } else {
                v5 = CreateObjectByReference(intvAttackerMajor, 16006, 0, 0);
                SetObjectCoordinate(v5, ((240 - (96 * 7)) - (v4 * 96)), (180 + (v1 * 72)), 0);
                v3 = Rand(1, 30);
                asynccall AddSoldierLeft(((240 - (96 * 7)) - (v4 * 96)), (180 + (v1 * 72)), v3);
                if (a2 == 1) {
                    v5 = CreateObjectByReference(intvAttackerMajor, 16008, 0, 0);
                    SetObjectCoordinate(v5, (((240 - (96 * 7)) + 27) - (v4 * 96)), (180 + (v1 * 72)), 14);
                    asynccall ChangeLeftFirePos(v5, v3);
                }
            }
            v2--;
            if (v2 <= 0) {
                v1 = 0;
            }
            v1--;
        } while (v1 >= 0);
        v4++;
    } while (v2 > 0);
    return;
}
void ArrowSupport(int a0, int a1)  {
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
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL2);
    SetOverwhelming(intvAttackerMajor, 0);
    Delay(30);
    ProduceArrowSoldier(intvIsLeft, a0, a1);
    if (!(intvIsLeft == 1)) {
        asynccall MoveCamera(((v6 + (96 * 4)) - 1), 0, 40);
    } else {
        asynccall MoveCamera((240 - (96 * 4)), 0, 40);
    }
    Wait("MoveCamera");
    Delay(60);
    v9 = GetSoldierCount((intvIsLeft ^ 1));
    v10 = 0;
    if (v9 < a0) {
        v10 = (a0 - v9);
    }
    if (!((a0 - v10) > 0)) {
        v11 = GetObjectScreenX(intvDefenderMajor);
        v12 = v11;
    } else {
        v2 = 0;
        v1 = (a0 - v10);
        while ((v1 > 0)) {
            v3 = GetRandomSoldierHandleFromAlive((intvIsLeft ^ 1));
            v4 = GetObjectScreenX(v3);
            v5 = GetObjectScreenY(v3);
            if (!(v2 == 0)) {
                if (v11 < v4) {
                    v11 = v4;
                }
                if (v12 > v4) {
                    v12 = v4;
                }
            } else {
                v2 = 1;
                v11 = v4;
                v12 = v4;
            }
            if (!(intvIsLeft == 1)) {
                asynccall FlyArrowLB1(v3, v5, (80 + Rand(1, 30)), a1);
            } else {
                asynccall FlyArrowRB1(v3, v5, (80 + Rand(1, 30)), a1);
            }
            v1--;
        }
    }
    if (!((v11 - v12) >= 650)) {
        v11 = ((v11 + v12) / 2);
    } else {
        if (intvIsLeft == 1) {
            v11 = v12;
        }
    }
    v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
    if (v3 == 0) {
        v3 = intvDefenderMajor;
    }
    v11 = GetObjectScreenX(v3);
    v1 = v10;
    do {
        v4 = (v11 + Rand((-400), 400));
        v5 = Rand(150, 650);
        if (!(intvIsLeft == 1)) {
            asynccall FlyArrowLB2(v4, v5, (80 + Rand(1, 30)), a1, 1);
        } else {
            asynccall FlyArrowRB2(v4, v5, (80 + Rand(1, 30)), a1, 1);
        }
        v1--;
    } while (v1 > 0);
    v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
    if (v3 == 0) {
        v3 = intvDefenderMajor;
    }
    v11 = GetObjectScreenX(v3);
    v1 = (((a0 / 10) * 5) + ((Rand(32, 50) - a0) / 8));
    SetGlobal(7, (v1 / 4));
    do {
        v4 = (v11 + Rand((-400), 400));
        v5 = Rand(200, 600);
        v7 = ScreenXToBattleX(v4);
        v8 = ScreenYToBattleY(v5);
        if (GetForceHandleByXY(v7, v8)) {
            v8 = BattleXToScreenX(v8);
            if (!Rand(0, 1)) {
                v5 = (v8 + (8 * Rand(1, 2)));
            } else {
                v5 = (v8 - (8 * Rand(1, 2)));
            }
        }
        if (!(intvIsLeft == 1)) {
            asynccall FlyArrowLB2(v4, v5, (80 + Rand(1, 30)), a1, 0);
        } else {
            asynccall FlyArrowRB2(v4, v5, (80 + Rand(1, 30)), a1, 0);
        }
        v1--;
    } while (v1 > 0);
    v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
    if (v3 == 0) {
        v3 = intvDefenderMajor;
    }
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
void StoneEmitterCallback1(int a0, int a1) callsign 7001  {
    int v1;
    if (!(a1 == intvDefenderMajor)) {
        Hurt(a0, a1, 0);
    } else {
        if (GetGlobal(10)) {
            v1 = (intvMagicAttackValue / GetGlobal(10));
        }
        Hurt(a0, a1, v1);
        if (v1 > 0) {
            SetGlobal(10, (GetGlobal(10) * 2));
        }
    }
    return;
}
void StoneEmitterCallback2(int a0, int a1) callsign 7002  {
    int v1;
    if (!(a1 == intvDefenderMajor)) {
        Hurt(a0, a1, 0);
        asynccall FireMan(a1, 10015, 60);
    } else {
        if (GetGlobal(10)) {
            v1 = (intvMagicAttackValue / GetGlobal(10));
        }
        Hurt(a0, a1, v1);
        if (v1 > 0) {
            SetGlobal(10, (GetGlobal(10) * 2));
            asynccall LockTargetTime2(a1, 10015, 60);
        }
    }
    return;
}
void StoneEmitterCmd3() callsign 7005  {
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
    if (v5) {
        FreeObject(v5);
        v5 = CreateObjectByReference(v1, 17022, v4, 0);
        SetObjectSpeed_Cylind(v5, 30, 30);
    }
    Delay(2);
    v2 = CreateObjectByReference(v3, 17004, v4, 0);
    SetCoordinateByReference_Cylind(v2, v3, 0, 0, 0);
    SetObjectSpeed_Cylind(v2, 30, 0);
    SetObjectFadeOut(v2, 32, 1);
    SetObjectScaleShrink(v2, 1024);
    Delay(32);
    FreeObject(v1);
    if (v5) {
        FreeObject(v5);
    }
    return;
}
void StoneExplode(int a0)  {
    int v1;
    int v2;
    PlaySound1("m007snd03", 200);
    v2 = GetObjectDir(a0);
    v1 = CreateObjectByReference(a0, 2501, v2, 0);
    SetObjectFlags(v1, OF_ENEMYFORCE);
    if (!(GetGlobal(9) == 0)) {
        SetCallbackProcedure(v1, 7002);
    } else {
        SetCallbackProcedure(v1, 7001);
    }
    SetObjectLife(v1, 2);
    return;
}
void Func7005(int a0)  {
    int v1;
    int v2;
    v1 = 65536;
    SetObjectSpeed_Cylind(a0, Rand(4, 6), Rand(8, 12));
    SetObjectGravity(a0, 24576);
    SetObjectFriction1(a0, 8192);
    v2 = Rand(20, 30);
    while ( 1 ) {
        v2--;
        if (!(v2 > 0))
            break;
        SetObjectScale(a0, v1, v1);
        v1 = (v1 - 256);
        if (v2 == 15) {
            ClearObjectFlags(a0, OF_ATTACKALL);
        }
        Delay(1);
    }
    FreeObject(a0);
    return;
}
void FallExplode1() callsign 7003  {
    int v1;
    int v2;
    int v3;
    v3 = GetScriptLinkedObject();
    SetScriptLinkedObject(intvAttackerMajor);
    asynccall StoneExplode(v3);
    v2 = 0;
    while ((v2 < 6)) {
        v1 = CreateObjectByReference(v3, Rand(17007, 17011), Rand(0, 255), 0);
        if (!(GetGlobal(9) == 0)) {
            SetCallbackProcedure(v1, 7002);
        } else {
            SetCallbackProcedure(v1, 7001);
        }
        SetObjectScale(v1, 65536, 65536);
        asynccall Func7005(v1);
        v2++;
    }
    return;
}
void FireStoneExplode(int a0)  {
    int v1;
    int v2;
    if (GetGlobal(11) == 0) {
        PlaySound1("m007snd02", 255);
        SetGlobal(11, 1);
    }
    v2 = GetObjectDir(a0);
    v1 = CreateObjectByReference(a0, 17012, v2, 0);
    SetObjectScale(v1, 57344, 57344);
    SetObjectFlags(v1, OF_ENEMYFORCE);
    ClearObjectFlags(v1, (OF_ENEMYGENERAL + OF_MYGENERAL));
    if (!(GetGlobal(9) == 0)) {
        SetCallbackProcedure(v1, 7002);
    } else {
        SetCallbackProcedure(v1, 7001);
    }
    return;
}
void Func7006(int a0)  {
    int v1;
    int v2;
    v1 = 65536;
    SetObjectSpeed_Cylind(a0, Rand(4, 12), Rand((-2), 8));
    SetObjectFriction1(a0, 8192);
    v2 = Rand(30, 40);
    while ( 1 ) {
        v2--;
        if (!(v2 > 0))
            break;
        SetObjectScale(a0, v1, v1);
        v1 = (v1 - 256);
        Delay(1);
    }
    FreeObject(a0);
    return;
}
void FallExplode2() callsign 7004  {
    int v1;
    int v2;
    int v3;
    v3 = GetScriptLinkedObject();
    SetScriptLinkedObject(intvAttackerMajor);
    asynccall FireStoneExplode(v3);
    v2 = 0;
    while ((v2 < 10)) {
        v1 = CreateObjectByReference(v3, Rand(17016, 17020), Rand(0, 255), 0);
        if ((v2 % 3) == 0) {
            SetObjectFlags(v1, OF_ENEMYFORCE);
            ClearObjectFlags(v1, (OF_ENEMYGENERAL + OF_MYGENERAL));
            if (!(GetGlobal(9) == 0)) {
                SetCallbackProcedure(v1, 7002);
            } else {
                SetCallbackProcedure(v1, 7001);
            }
        }
        SetObjectScale(v1, 65536, 65536);
        asynccall Func7006(v1);
        v2++;
    }
    return;
}
void ProduceStoneEmitter(int a0, int a1, int a2, int a3, int a4)  {
    int v1;
    int v2;
    Delay(a2);
    if (!(intvIsLeft == 1)) {
        v1 = CreateObjectByReference(intvAttackerMajor, 17002, a3, 0);
    } else {
        v1 = CreateObjectByReference(intvAttackerMajor, 17023, a3, 0);
    }
    SetObjectCoordinate(v1, a0, a1, 0);
    SetObjectContext(v1, 0, 0);
    if (a4 == 1) {
        v2 = CreateObjectByReference(v1, 17021, a3, 0);
        SetObjectScale(v2, 81920, 81920);
        SetObjectContext(v1, 0, v2);
        if (!(intvIsLeft == 1)) {
            MoveObject(v2, 63, 0, 42);
        } else {
            MoveObject(v2, (-63), 0, 42);
        }
    }
    return;
}
void TraceStoneShadow(int a0, int a1, int a2, int a3)  {
    int v1;
    while (IsObjectExist(a1)) {
        v1 = GetObjectScreenZ(a1);
        if (v1 <= 0) {
            break;
        }
        Delay(1);
        if (a2 < 65536) {
            a2 = (a2 + 8192);
            SetObjectScale(a0, a2, a2);
        }
        if (a3 < 16) {
            a3++;
            SetObjectOpacity(a0, a3);
        }
    }
    FreeObject(a0);
    return;
}
void FlyStoneLB1(int a0, int a1, int a2, int a3, int a4)  {
    int v1;
    int v2;
    Delay(a2);
    if (!(a3 == 0)) {
        v1 = CreateObjectByReference(intvAttackerMajor, 17014, 128, 0);
    } else {
        v1 = CreateObjectByReference(intvAttackerMajor, 17005, 128, 0);
    }
    SetObjectCoordinate(v1, (a0 + 260), a1, 260);
    SetObjectSpeed_Sphere(v1, 128, (-32), 32);
    SetObjectFlags(v1, OF_ENEMYFORCE);
    SetCallbackProcedure(v1, a4);
    v2 = CreateObjectByReference(intvAttackerMajor, 17004, 128, 0);
    SetObjectCoordinate(v2, (a0 + 264), a1, 0);
    SetObjectSpeed_Cylind(v2, 23, 0);
    SetObjectScale(v2, 32768, 32768);
    SetObjectFlags(v2, OF_MIXER);
    SetObjectOpacity(v2, 4);
    asynccall TraceStoneShadow(v2, v1, 32768, 4);
    if (a3 == 1) {
        v1 = CreateObjectByReference(intvAttackerMajor, 17013, 128, 0);
        SetObjectCoordinate(v1, (a0 + 260), a1, 260);
        SetObjectSpeed_Sphere(v1, 128, (-32), 32);
        SetObjectScale(v1, 98304, 98304);
    }
    return;
}
void FlyStoneRB1(int a0, int a1, int a2, int a3, int a4)  {
    int v1;
    int v2;
    Delay(a2);
    if (!(a3 == 0)) {
        v1 = CreateObjectByReference(intvAttackerMajor, 17015, 128, 0);
    } else {
        v1 = CreateObjectByReference(intvAttackerMajor, 17006, 128, 0);
    }
    SetObjectCoordinate(v1, (a0 - 260), a1, 260);
    SetObjectSpeed_Sphere(v1, 0, (-32), 32);
    SetObjectFlags(v1, OF_ENEMYFORCE);
    SetCallbackProcedure(v1, a4);
    v2 = CreateObjectByReference(intvAttackerMajor, 17004, 0, 0);
    SetObjectCoordinate(v2, (a0 - 264), a1, 0);
    SetObjectSpeed_Cylind(v2, 23, 0);
    SetObjectScale(v2, 32768, 32768);
    SetObjectFlags(v2, OF_MIXER);
    SetObjectOpacity(v2, 4);
    asynccall TraceStoneShadow(v2, v1, 32768, 4);
    if (a3 == 1) {
        v1 = CreateObjectByReference(intvAttackerMajor, 17013, 0, 0);
        SetObjectCoordinate(v1, (a0 - 260), a1, 260);
        SetObjectSpeed_Sphere(v1, 0, (-32), 32);
        SetObjectScale(v1, 98304, 98304);
    }
    return;
}
void FlyStoneLB2(int a0, int a1, int a2, int a3, int a4)  {
    int v1;
    int v2;
    int v3;
    v2 = GetObjectScreenX(a0);
    a1 = GetObjectScreenY(a0);
    Delay(a2);
    if (!(a3 == 0)) {
        v1 = CreateObjectByReference(intvAttackerMajor, 17014, 128, 0);
    } else {
        v1 = CreateObjectByReference(intvAttackerMajor, 17005, 128, 0);
    }
    v3 = CreateObjectByReference(intvAttackerMajor, 17004, 128, 0);
    if (!(IsObjectExist(a0) == 0)) {
        SetCoordinateByReference_Cylind(v1, a0, 0, 0, 0);
        MoveObject(v1, (260 + 24), 0, 260);
        SetCoordinateByReference_Cylind(v3, a0, 0, 0, 0);
        SetObjectCoordinate(v3, (264 + 24), 0, 0);
    } else {
        SetObjectCoordinate(v1, ((v2 + 260) + 24), a1, 260);
        SetObjectCoordinate(v3, ((v2 + 264) + 24), a1, 0);
    }
    SetObjectSpeed_Sphere(v1, 128, (-32), 32);
    SetObjectFlags(v1, OF_ENEMYFORCE);
    SetCallbackProcedure(v1, a4);
    SetObjectSpeed_Cylind(v3, 23, 0);
    SetObjectScale(v3, 32768, 32768);
    SetObjectFlags(v3, OF_MIXER);
    SetObjectOpacity(v3, 4);
    asynccall TraceStoneShadow(v3, v1, 32768, 4);
    if (a3 == 1) {
        v1 = CreateObjectByReference(intvAttackerMajor, 17013, 128, 0);
        SetCoordinateByReference_Cylind(v1, a0, 0, 0, 0);
        MoveObject(v1, (260 + 24), 0, 260);
        SetObjectSpeed_Sphere(v1, 128, (-32), 32);
        SetObjectScale(v1, 98304, 98304);
    }
    return;
}
void FlyStoneRB2(int a0, int a1, int a2, int a3, int a4)  {
    int v1;
    int v2;
    int v3;
    v2 = GetObjectScreenX(a0);
    a1 = GetObjectScreenY(a0);
    Delay(a2);
    if (!(a3 == 0)) {
        v1 = CreateObjectByReference(intvAttackerMajor, 17015, 128, 0);
    } else {
        v1 = CreateObjectByReference(intvAttackerMajor, 17006, 128, 0);
    }
    v3 = CreateObjectByReference(intvAttackerMajor, 17004, 0, 0);
    if (!(IsObjectExist(a0) == 0)) {
        SetCoordinateByReference_Cylind(v1, a0, 0, 0, 0);
        MoveObject(v1, ((-260) - 24), 0, 260);
        SetCoordinateByReference_Cylind(v3, a0, 0, 0, 0);
        SetObjectCoordinate(v3, ((-264) - 24), 0, 0);
    } else {
        SetObjectCoordinate(v1, ((v2 - 260) - 24), a1, 260);
        SetObjectCoordinate(v3, ((v2 - 264) - 24), a1, 0);
    }
    SetObjectSpeed_Sphere(v1, 0, (-32), 32);
    SetObjectFlags(v1, OF_ENEMYFORCE);
    SetCallbackProcedure(v1, a4);
    SetObjectSpeed_Cylind(v3, 23, 0);
    SetObjectScale(v3, 32768, 32768);
    SetObjectFlags(v3, OF_MIXER);
    SetObjectOpacity(v3, 4);
    asynccall TraceStoneShadow(v3, v1, 32768, 4);
    if (a3 == 1) {
        v1 = CreateObjectByReference(intvAttackerMajor, 17013, 0, 0);
        SetCoordinateByReference_Cylind(v1, a0, 0, 0, 0);
        MoveObject(v1, ((-260) - 24), 0, 260);
        SetObjectSpeed_Sphere(v1, 0, (-32), 32);
        SetObjectScale(v1, 98304, 98304);
    }
    return;
}
void StoneEmitter(int a0, int a1, int a2)  {
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
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL2);
    SetOverwhelming(intvAttackerMajor, 0);
    Delay(30);
    if (!(a1 == 0)) {
        v7 = 7002;
    } else {
        v7 = 7001;
    }
    if (!(intvIsLeft == 1)) {
        v6 = 128;
    } else {
        v6 = 0;
    }
    if (!(a0 == 0)) {
        v8 = 1;
        v1 = 6;
    } else {
        v8 = 2;
        v1 = 4;
    }
    do {
        v13 = Rand(1, 30);
        asynccall ProduceStoneEmitter((((6576 + (96 * 7)) * (intvIsLeft == 0)) + ((240 - (96 * 7)) * (intvIsLeft != 0))), ((180 + ((v1 * 72) * 2)) - (72 * 2)), v13, v6, a1);
        v1 = (v1 - v8);
    } while (v1 >= 0);
    if (a0 == 2) {
        v8 = 1;
        v1 = 5;
        do {
            v13 = Rand(1, 30);
            asynccall ProduceStoneEmitter(((((6576 + (96 * 7)) + (96 * 2)) * (intvIsLeft == 0)) + (((240 - (96 * 7)) - (96 * 2)) * (intvIsLeft != 0))), ((180 + ((v1 * 72) * 2)) - (72 * 1)), v13, v6, a1);
            v1 = (v1 - v8);
        } while (v1 > 0);
    }
    if (!(intvIsLeft == 1)) {
        asynccall MoveCamera((((6576 + (96 * 4)) - 1) + 96), 0, 40);
    } else {
        asynccall MoveCamera(((240 - (96 * 4)) - 96), 0, 40);
    }
    Wait("MoveCamera");
    Delay(92);
    v9 = GetSoldierCount((intvIsLeft ^ 1));
    v10 = 0;
    if (v9 < a2) {
        v10 = (a2 - v9);
    }
    if (!((a2 - v10) > 0)) {
        v11 = GetObjectScreenX(intvDefenderMajor);
        v12 = v11;
    } else {
        v2 = 0;
        v1 = (a2 - v10);
        do {
            v3 = GetRandomSoldierHandleFromAlive((intvIsLeft ^ 1));
            v4 = GetObjectScreenX(v3);
            v5 = GetObjectScreenY(v3);
            if (!(v2 == 0)) {
                if (v11 < v4) {
                    v11 = v4;
                }
                if (v12 > v4) {
                    v12 = v4;
                }
            } else {
                v2 = 1;
                v11 = v4;
                v12 = v4;
            }
            if (!(intvIsLeft == 1)) {
                asynccall FlyStoneLB2(v3, v5, (80 + Rand(1, 30)), a1, v7);
            } else {
                asynccall FlyStoneRB2(v3, v5, (80 + Rand(1, 30)), a1, v7);
            }
            v1--;
        } while (v1 > 0);
    }
    if (v11 == v12) {
        v11 = (v11 + 512);
        v12 = (v12 - 512);
    }
    v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
    if (v3 == 0) {
        v3 = intvDefenderMajor;
    }
    v11 = GetObjectScreenX(v3);
    v1 = v10;
    do {
        v4 = (v11 + Rand((-320), 320));
        v5 = Rand(150, 650);
        if (!(intvIsLeft == 1)) {
            asynccall FlyStoneLB1(v4, v5, (80 + Rand(1, 30)), a1, v7);
        } else {
            asynccall FlyStoneRB1(v4, v5, (80 + Rand(1, 30)), a1, v7);
        }
        v1--;
    } while (v1 > 0);
    v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
    if (v3 == 0) {
        v3 = intvDefenderMajor;
    }
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
void ExplodeRoundCallback(int a0, int a1) callsign 8001  {
    int v1;
    if (!(a1 == intvDefenderMajor)) {
        Hurt(a0, a1, 0);
        asynccall FireMan(a1, 10016, 60);
    } else {
        if (GetGlobal(12)) {
            v1 = (intvMagicAttackValue / GetGlobal(12));
        }
        Hurt(a0, a1, v1);
        if (v1 > 0) {
            SetGlobal(12, (GetGlobal(12) * 2));
            asynccall LockTargetTime2(a1, 10016, 60);
        }
    }
    return;
}
void ProduceAirCircle(int a0, int a1, int a2)  {
    int v1;
    int v2;
    v1 = CreateObjectByReference(a0, a1, 0, 0);
    SetCoordinateByReference_Cylind(v1, a0, 0, 0, 0);
    SetObjectOpacity(v1, 8);
    SetObjectLayer(v1, 2);
    v2 = 1;
    while ((v2 <= 40)) {
        Delay(1);
        if (IsObjectExist(v1) == 0) {
            break;
        }
        SetObjectScale(v1, (65536 + (12288 * v2)), (65536 + (12288 * v2)));
        if (v2 == a2) {
            SetObjectFadeOut(v1, 16, 1);
        }
        v2++;
    }
    return;
}
void CreateExplodeRound(int a0, int a1)  {
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
void ProduceRound(int a0, int a1, int a2, int a3)  {
    int v1;
    int v2;
    Delay(Rand(1, 30));
    v2 = Rand(0, 255);
    v1 = CreateObjectByReference(a0, a1, v2, 0);
    SetCallbackProcedure(v1, a3);
    SetObjectFlags(v1, OF_ATTACKENEMY);
    SetCoordinateByReference_Cylind(v1, a0, v2, a2, 0);
    asynccall CreateExplodeRound(v1, Rand(0, 1));
    asynccall CreateExplodeRound(v1, Rand(2, 3));
    asynccall CreateExplodeRound(v1, Rand(4, 5));
    asynccall CreateExplodeRound(v1, Rand(5, 6));
    asynccall CreateExplodeRound(v1, Rand(7, 8));
    asynccall CreateExplodeRound(v1, Rand(9, 10));
    return;
}
void ExplodeRound(int a0, int a1, int a2)  {
    int v1;
    int v3;
    int v4;
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
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL1);
    v3 = GetObjectScreenX(intvAttackerMajor);
    v4 = GetObjectScreenY(intvAttackerMajor);
    Delay(10);
    ProduceAirCircle(intvAttackerMajor, 18002, ((a0 * 3) / 18));
    SetOverwhelming(intvAttackerMajor, 0);
    Delay(20);
    v1 = 0;
    while ((v1 < a0)) {
        asynccall ProduceRound(intvAttackerMajor, a2, Rand(72, a1), 8001);
        Delay(Rand(1, 3));
        if (v1 == 0) {
            PlaySound1("m008snd01", 255);
        }
        v1++;
    }
    Wait("ProduceRound");
    Wait("CreateExplodeRound");
    Delay(30);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void EightWayFireCallback(int a0, int a1) callsign 9001  {
    int v1;
    asynccall LockTargetTime2(a1, 10015, 60);
    if (!(a1 == intvDefenderMajor)) {
        Hurt(a0, a1, 0);
        asynccall FireMan(a1, 10015, 60);
    } else {
        if (GetGlobal(13)) {
            v1 = (intvMagicAttackValue / GetGlobal(13));
        }
        Hurt(a0, a1, v1);
        if (v1 > 0) {
            SetGlobal(13, (GetGlobal(13) * 2));
            asynccall LockTargetTime2(a1, 10015, 60);
        }
    }
    return;
}
void CreateEightWay(int a0, int a1, int a2, int a3, int a4)  {
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
    SetObjectFlags(v1, OF_MIXER);
    v2 = 0;
    while ((v2 < (a1 * 2))) {
        SetObjectOpacity(v1, v4);
        SetObjectScale(v1, 98304, v3);
        SetCoordinateByReference_Cylind(v1, a0, a3, ((v2 + 1) * v5), 0);
        Delay(2);
        v3 = (v3 + 2048);
        v4 = (v4 + v6);
        if (!(v4 >= 16)) {
            if (v4 <= 0) {
                break;
            }
        } else {
            v6 = (-3);
        }
        if (a4) {
            a3 = ((a3 + 8) & 255);
            if (a1 & 1) {
                v5--;
            }
        }
        v2++;
    }
    FreeObject(v1);
    return;
}
void EightWayFireMotion(int a0, int a1, int a2, int a3, int a4)  {
    int v1;
    v1 = 0;
    while ((v1 < (a1 - 8))) {
        asynccall CreateEightWay(a0, a1, a2, a3, a4);
        Delay(4);
        v1++;
    }
    return;
}
void EightWayFire(int a0, int a1, int a2)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
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
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL1);
    SetOverwhelming(intvAttackerMajor, 0);
    SetGlobal(13, 1);
    v3 = GetObjectScreenX(intvAttackerMajor);
    v4 = GetObjectScreenY(intvAttackerMajor);
    if (!(a0 == 0)) {
        if (!(a0 == 1)) {
            v1 = 7;
            v2 = 1;
            v5 = 14;
        } else {
            v1 = 6;
            v2 = 2;
            v5 = 10;
        }
    } else {
        v1 = 4;
        v2 = 4;
        v5 = 8;
    }
    do {
        asynccall EightWayFireMotion(intvAttackerMajor, v5, a2, (v1 * 32), a1);
        v1 = (v1 - v2);
    } while (v1 >= 0);
    PlaySound1("m009snd01", 255);
    Wait("CreateEightWay");
    Delay(30);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void ConvexStoneCallback(int a0, int a1) callsign 10001  {
    Hurt(a0, a1, intvMagicAttackValue);
    return;
}
void Func10002(int a0, int a1) callsign 10002  {
    int v1;
    int v2;
    v2 = 0;
    while ((v2 < 10)) {
        v1 = CreateObjectByReference(a0, Rand(20005, 20010), a1, 0);
        ClearObjectFlags(v1, OF_NOGRAVITY);
        SetObjectGravity(v1, 10752);
        SetObjectScale(v1, 98304, 98304);
        SetObjectSpeed_Sphere(v1, Rand(0, 256), Rand((64 - 32), (64 + 32)), 6);
        SetObjectFadeOut(v1, 32, 1);
        SetObjectScaleShrink(v1, 512);
        v2++;
    }
    return;
}
void Func10003(int a0, int a1) callsign 10003  {
    int v1;
    int v2;
    v2 = 0;
    while ((v2 < 10)) {
        v1 = CreateObjectByReference(a0, Rand(20005, 20010), a1, 0);
        ClearObjectFlags(v1, OF_NOGRAVITY);
        SetObjectGravity(v1, 10752);
        SetObjectScale(v1, 98304, 98304);
        SetObjectSpeed_Sphere(v1, Rand(0, 256), Rand((64 - 32), (64 + 32)), 7);
        SetObjectFadeOut(v1, 32, 1);
        SetObjectScaleShrink(v1, 512);
        v2++;
    }
    return;
}
void TraceStone(int a0, int a1)  {
    int v1;
    int v2;
    v1 = 0;
    while (IsObjectExist(a0)) {
        v2 = GetObjectScreenZ(a0);
        if (v2 <= 0) {
            switch (v1) {
            case 0:
                SetObjectFlags(a0, OF_NOGRAVITY);
                SetObjectSpeed_Sphere(a0, a1, Rand(46, 54), Rand(6, 8));
                Delay(3);
                ClearObjectFlags(a0, OF_NOGRAVITY);
                SetObjectGravity(a0, 49152);
                v1++;
                break;
            case 1:
                SetObjectFlags(a0, OF_NOGRAVITY);
                SetObjectSpeed_Sphere(a0, a1, Rand(42, 50), Rand(4, 6));
                Delay(2);
                ClearObjectFlags(a0, OF_NOGRAVITY);
                SetObjectGravity(a0, 32768);
                v1++;
                break;
            case 2:
                SetObjectFlags(a0, OF_NOGRAVITY);
                SetObjectSpeed_Sphere(a0, a1, Rand(40, 44), Rand(2, 4));
                Delay(1);
                ClearObjectFlags(a0, OF_NOGRAVITY);
                SetObjectGravity(a0, 16384);
                v1++;
                break;
                break;
            }
        }
        Delay(1);
    }
    return;
}
void BrokenStone(int a0, int a1, int a2, int a3, int a4, int a5)  {
    int v1;
    int v2;
    v1 = CreateObjectByReference(a0, a1, v2, a3);
    if (!(a2 > 0)) {
        a2 = (-a2);
        v2 = 128;
    } else {
        v2 = 0;
    }
    SetCoordinateByReference_Cylind(v1, a0, v2, a2, a3);
    SetObjectScale(v1, Rand(43008, 73728), Rand(61440, 73728));
    SetObjectScaleShrink(v1, 512);
    Delay(2);
    SetObjectSpeed_Sphere(v1, a5, Rand(52, 60), Rand(6, 8));
    asynccall TraceStone(v1, a5);
    SetObjectFadeOut(v1, 48, 1);
    return;
}
void ProduceSomethingXY(int a0, int a1, int a2, int a3, int a4, int a5)  {
    int v1;
    v1 = CreateObjectByReference(intvAttackerMajor, a2, a3, 0);
    if (a4) {
        DoScript(a4, v1, a3, 0);
    }
    SetObjectCoordinate(v1, a0, a1, 0);
    SetObjectSpeed_Sphere(v1, a3, 0, 0);
    SetObjectScale(v1, 98304, 98304);
    if (!a5) {
        Delay(8);
        SetObjectFadeOut(v1, 16, 1);
    } else {
        Delay(12);
        if (!(a2 == 20002)) {
            asynccall BrokenStone(v1, 20011, Rand((-7), (-9)), 120, 0, 128);
            asynccall BrokenStone(v1, 20012, Rand(6, 8), 107, 0, 0);
            asynccall BrokenStone(v1, 20014, Rand((-8), (-9)), 99, 0, 128);
            asynccall BrokenStone(v1, 20013, Rand(7, 9), 85, 0, 0);
            asynccall BrokenStone(v1, 20011, Rand((-6), (-7)), 70, 0, 128);
            asynccall BrokenStone(v1, 20014, Rand(8, 10), 67, 0, 0);
            asynccall BrokenStone(v1, 20012, Rand((-6), (-7)), 52, 0, 128);
            asynccall BrokenStone(v1, 20013, Rand(7, 9), 49, 0, 0);
        } else {
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
        }
        Delay(2);
        SetObjectFlags(v1, 128);
        Wait("BrokenStone");
        FreeObject(v1);
    }
    return;
}
void ConvexStone(int a0)  {
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
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL2);
    SetOverwhelming(intvAttackerMajor, 0);
    Delay(30);
    if (!(intvIsLeft == 1)) {
        v10 = 128;
    } else {
        v10 = 0;
    }
    v5 = GetObjectScreenX(intvDefenderMajor);
    v6 = GetObjectScreenY(intvDefenderMajor);
    MoveCamera(v5, (v6 - 120), 40);
    Delay(15);
    v5 = GetObjectScreenX(intvDefenderMajor);
    v6 = GetObjectScreenY(intvDefenderMajor);
    if (!(a0 == 0)) {
        if (!(a0 == 1)) {
            v1 = 5;
            v9 = 10003;
        } else {
            v1 = 3;
        }
    } else {
        v1 = 1;
    }
    v7 = 0;
    v8 = 0;
    v11 = v7;
    v12 = v8;
    do {
        asynccall ProduceSomethingXY((v5 + v7), (v6 + v8), (20001 + (a0 == 2)), v10, v9, 1);
        asynccall ProduceSomethingXY((v5 + v7), (v6 + v8), (20003 + (a0 == 2)), v10, v9, 0);
        asynccall LockTargetXY((v5 + v7), (v6 + v8), 2501, v10, 8, 10001, 0, 0, (OF_ENEMYFORCE + OF_ENEMYGENERAL));
        v2 = 1;
        while ((v2 < 6)) {
            v14 = (((v6 + v8) - ((72 * 3) / 2)) + ((v2 * 72) / 2));
            v3 = 1;
            while ((v3 < 6)) {
                v13 = (((v5 + v7) - ((96 * 3) / 2)) + ((v3 * 96) / 2));
                asynccall LockTargetXY(v13, v14, 2501, v10, 8, 10001, 0, 0, OF_ENEMYFORCE);
                v3++;
            }
            v2++;
        }
        Delay(Rand(4, 12));
        while (1) {
            v7 = Rand((-240), 240);
            if ((v7 > (v11 + 28)) || (v7 < (v11 - 28))) {
                break;
            }
        } while (1) {
            v8 = Rand((-110), 340);
            if ((v8 > (v12 + 36)) || (v8 < (v12 - 36))) {
                break;
            }
        }
        v11 = v7;
        v12 = v8;
        v1--;
    } while (v1 > 0);
    Wait("CreateEightWay");
    Delay(60);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void TornadorCallback(int a0, int a1) callsign 11001  {
    int v1;
    int v2;
    int v3;
    int v4;
    SetObjectFlags(a1, OF_NOGRAVITY);
    SetObjectSpeed_Cylind(a1, 0, Rand(6, 10));
    v1 = Rand(0, 255);
    v4 = Rand(0, 18);
    do {
        v3 = GetObjectScreenZ(a0);
        v2 = GetObjectScreenZ(a1);
        SetCoordinateByReference_Cylind(a1, a0, v1, v4, (v2 - v3));
        Delay(1);
        v1 = ((v1 + 16) & 255);
        if (v2 > 240) {
            break;
        }
        if (!IsObjectExist(a1)) {
            break;
        }
    } while (IsObjectExist(a0));
    FreeObject(a1);
    return;
}
void TornadoMotion(int a0, int a1, int a2, int a3, int a4, int a5, int a6)  {
    int v1;
    int v2;
    v1 = a4;
    while (IsObjectExist(a1)) {
        if (!a6) {
            v2 = CalcCos(v1, a3);
        } else {
            v2 = CalcSin(v1, a3);
        }
        SetCoordinateByReference_Cylind(a1, a0, 0, v2, a2);
        Delay(1);
        v1 = ((v1 + 4) & 255);
    }
    return;
}
void TornadoStoneMotion(int a0, int a1)  {
    int v1;
    int v2;
    int v3;
    int v4;
    SetObjectFlags(a1, OF_NOGRAVITY);
    SetObjectSpeed_Cylind(a1, 0, Rand(4, 12));
    v1 = Rand(0, 255);
    v4 = Rand(40, 80);
    do {
        v3 = GetObjectScreenZ(a0);
        v2 = GetObjectScreenZ(a1);
        SetCoordinateByReference_Cylind(a1, a0, v1, v4, (v2 - v3));
        Delay(1);
        v1 = ((v1 + 4) & 255);
        if (v2 > 240) {
            break;
        }
        if (!IsObjectExist(a1)) {
            break;
        }
    } while (IsObjectExist(a0));
    FreeObject(a1);
    return;
}
void TornadoStone(int a0)  {
    int v1;
    Delay(Rand(10, 18));
    v1 = CreateObjectByReference(a0, 21003, 0, 0);
    ClearObjectFlags(v1, 524288);
    SetObjectScale(v1, Rand(16384, 49152), Rand(16384, 49152));
    asynccall TornadoStoneMotion(a0, v1);
    return;
}
void BottomSmoke(int a0)  {
    int v1;
    v1 = CreateObjectByReference(a0, 7050, 0, 0);
    SetCoordinateByReference_Cylind(v1, a0, Rand(128, 255), Rand(16, 32), 0);
    SetObjectScale(v1, 16384, 18432);
    SetObjectSpeed_Cylind(v1, 0, Rand(4, 12));
    SetObjectFadeOut(v1, 12, 1);
    SetObjectScaleShrink(v1, (-512));
    return;
}
void BottomMotion(int a0, int a1)  {
    int v1;
    v1 = 0;
    while (IsObjectExist(a1)) {
        SetCoordinateByReference_Cylind(a1, a0, 0, 0, 0);
        if ((v1 % 5) == 0) {
            asynccall TornadoStone(a1);
        }
        Delay(1);
        v1++;
    }
    return;
}
void ProduceTornado(int a0)  {
    int v1;
    int v2[12];
    int v14;
    int v15;
    v15 = (Rand(0, 2134) & 1);
    v1 = CreateObjectByReference(a0, 21002, 0, 0);
    asynccall BottomMotion(a0, v1);
    v14 = 0;
    while ((v14 < 12)) {
        v2[v14] = CreateObjectByReference(a0, 21001, 128, 0);
        SetObjectScale(v2[v14], (16384 + ((81920 / 12) * v14)), (65536 + ((98304 / 12) * v14)));
        asynccall TornadoMotion(a0, v2[v14], ((v14 + 1) * 16), Rand(8, 16), (32 * (v14 / 2)), v14, v15);
        v14++;
    } while (IsObjectExist(a0)) {
        Delay(2);
    }
    SetObjectFadeOut(v1, 16, 1);
    v14 = 0;
    while ((v14 < 12)) {
        Delay(2);
        SetObjectFadeOut(v2[v14], 16, 1);
        v14++;
    }
    return;
}
void DummyMotion(int a0, int a1, int a2, int a3)  {
    int v1;
    int v2;
    int v3;
    int v4;
    v1 = Rand(3, 4);
    v4 = GetSoldierCount((intvIsLeft ^ 1));
    if (!(v4 == 0)) {
        if (!(v4 < 3)) {
            v2 = Rand((-2), 1);
        } else {
            v2 = Rand((-2), (-3));
        }
    } else {
        v2 = (-1);
    }
    v3 = Rand(1, 3);
    while (IsObjectExist(a1)) {
        SetCoordinateByReference_Cylind(a0, a1, a2, a3, 0);
        Delay(1);
        a2 = ((a2 + v3) & 255);
        if ((a2 % v1) == 0) {
            a3 = (a3 + v2);
        }
    }
    FreeObject(a0);
    return;
}
int CreateDummyRef(int a0, int a1, int a2)  {
    int v1;
    v1 = CreateObjectByReference(a0, 2501, 0, 0);
    SetCoordinateByReference_Cylind(v1, a0, a1, a2, 0);
    SetObjectFlags(v1, (OF_ENEMYFORCE + OF_MYFORCE));
    SetCallbackProcedure(v1, 11001);
    return v1;
}
void WaitTarget(int a0, int a1, int a2)  {
    int v1;
    int v2;
    int v3;
    while (IsObjectExist(a0)) {
        v1 = GetObjectScreenX(a0);
        v2 = GetObjectScreenY(a0);
        if (((v1 < (a1 + (96 / 4))) && (v1 > (a1 - (96 / 4)))) && ((v2 < (a2 + (72 / 4))) && (v2 > (a2 - (72 / 4))))) {
            break;
        }
        Delay(1);
        v3++;
        if (v3 > 60) {
            break;
        }
    }
    return;
}
int GetSpeed(int a0, int a1, int a2, int a3)  {
    int v1;
    return 10;
}
int GetDir(int a0, int a1, int a2, int a3)  {
    float v1;
    float v2;
    a2 = (a2 - a0);
    a3 = (a3 - a1);
    if (!(a2 == 0)) {
        if (!(a3 == 0)) {
            if (!(a2 >= 0)) {
                v1 = ((float)(-a2));
            } else {
                v1 = ((float)a2);
            }
            if (!(a3 >= 0)) {
                v2 = ((float)(-a3));
            } else {
                v2 = ((float)a3);
            }
            v2 = (v2 / v1);
            if (!((a2 > 0) && (a3 > 0))) {
                if (!((a2 < 0) && (a3 > 0))) {
                    if (!((a2 < 0) && (a3 < 0))) {
                        if (!(v2 < (float) 5773 / 10000)) {
                            if (!(v2 >= (float) 1732 / 1000)) {
                                return 96;
                            } else {
                                return 0;
                            }
                        } else {
                            return 128;
                        }
                    } else {
                        if (!(v2 < (float) 5773 / 10000)) {
                            if (!(v2 >= (float) 1732 / 1000)) {
                                return 32;
                            } else {
                                return 64;
                            }
                        } else {
                            return 0;
                        }
                    }
                } else {
                    if (!(v2 < (float) 5773 / 10000)) {
                        if (!(v2 >= (float) 1732 / 1000)) {
                            return 224;
                        } else {
                            return 192;
                        }
                    } else {
                        return 0;
                    }
                }
            } else {
                if (!(v2 < (float) 5773 / 10000)) {
                    if (!(v2 >= (float) 1732 / 1000)) {
                        return 160;
                    } else {
                        return 192;
                    }
                } else {
                    return 128;
                }
            }
        } else {
            if (!(a2 >= 0)) {
                return 0;
            } else {
                return 128;
            }
        }
    } else {
        if (!(a3 >= 0)) {
            return 64;
        } else {
            return 192;
        }
    }
}
void Tornado(int a0)  {
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
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL2);
    SetOverwhelming(intvAttackerMajor, 0);
    if (!(intvIsLeft == 1)) {
        v3 = 128;
    } else {
        v3 = 0;
    }
    v8 = GetMostImportantSoldier((intvIsLeft ^ 1));
    if (v8 == 0) {
        v8 = GetRandomSoldierHandleFromAlive((intvIsLeft ^ 1));
        if (v8 == 0) {
            v8 = intvDefenderMajor;
        }
    }
    v9 = CreateObjectByReference(v8, 2501, 0, 0);
    SetObjectLife(v9, ((a0 * 30) + Rand(160, 200)));
    PlaySound1("m011snd01", 255);
    asynccall LockCameraSimple(v9, (-320));
    v7 = 0;
    while ((v7 <= a0)) {
        v11 = (((v7 + 1) * 85) + Rand((-10), 10));
        v13 = ((v7 * 83) + Rand((-10), 10));
        v4[v7] = CreateDummyRef(v9, v13, v11);
        asynccall ProduceTornado(v4[v7]);
        asynccall DummyMotion(v4[v7], v9, v13, v11);
        v7++;
    } while (IsObjectExist(v9)) {
        if (IsObjectExist(v8) == 0) {
            v8 = GetMostImportantSoldier((intvIsLeft ^ 1));
        }
        if (!v8) {
            v12 = GetRandomSoldierHandleFromAlive((intvIsLeft ^ 1));
            if (v12 == 0) {
                v12 = intvDefenderMajor;
            }
        } else {
            v1 = GetObjectXY(v8);
            v2 = GetObjectBattleY(v8);
            v11 = Rand(4, 8);
            v10 = GetSoldierCountInRect((intvIsLeft ^ 1), v1, v2, v11, v11);
            v12 = 0;
            if (v10 > 1) {
                v12 = GetNthSoldierInRect((intvIsLeft ^ 1), v1, v2, v11, v11, Rand(1, v10));
            }
            if (v12 == 0) {
                v12 = GetRandomSoldierHandleFromAlive((intvIsLeft ^ 1));
                if (v12 == 0) {
                    v12 = intvDefenderMajor;
                }
            }
        }
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
    }
    Wait("ProduceTornado");
    Delay(60);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void DuplicatorCallback(int a0, int a1) callsign 12001  {
    Hurt(a0, a1, intvMagicAttackValue);
    if (!((a1 == intvAttackerMajor) | (a1 == intvDefenderMajor))) {
        asynccall Blood(a1, 16);
    } else {
        asynccall SmallFireBall(a0, a1, 12, 0);
        Delay(4);
        asynccall SmallFireBall(a0, a1, 12, 0);
        Delay(4);
        asynccall SmallFireBall(a0, a1, 12, 0);
        Delay(4);
        asynccall SmallFireBall(a0, a1, 12, 0);
        Delay(4);
        asynccall SmallFireBall(a0, a1, 12, 0);
    }
    return;
}
void TraceDuplicator(int a0, int a1)  {
    int v1;
    int v2;
    int v3;
    int v4;
    v4 = 0;
    while (IsObjectExist(a0)) {
        Delay(1);
        if (IsObjectExist(a1) == 0) {
            break;
        }
        v1 = GetObjectScreenX(a0);
        v2 = GetObjectScreenX(a1);
        v3 = (v2 - v1);
        if (v3 < 0) {
            v3 = (-v3);
        }
        if (!((v3 < 360) && (v4 < 2))) {
            if ((v3 < 1080) && (v4 < 1)) {
                SetObjectAnimate(a0, 2);
                v4 = 1;
            }
        } else {
            SetObjectAnimate(a0, OAF_ATTACK);
            v4 = 2;
        }
    }
    return;
}
void MoveDuplicator(int a0, int a1, int a2)  {
    int v1;
    int v2;
    int v3;
    v3 = GetObjectDir(a0);
    v2 = CreateObjectByReference(a0, a1, v3, 0);
    SetCoordinateByReference_Cylind(v2, a0, v3, (48 + 64), 0);
    v1 = a2;
    do {
        SetCoordinateByReference_Cylind(v2, a0, v3, (48 + 64), 0);
        Delay(1);
        if (!IsObjectExist(a0)) {
            break;
        }
        v1--;
    } while((v1 >= 0));
    FreeObject(v2);
    return;
}
void DuplicatorShadow(int a0, int a1, int a2, int a3, int a4)  {
    int v1;
    if (!(a4 == 0)) {
        v1 = CreateObjectByReference(a0, a1, a2, 0);
        if (!(a2 == 0)) {
            MoveObject(v1, 8, 0, 0);
        } else {
            MoveObject(v1, (-8), 0, 0);
        }
        SetObjectSpeed_Sphere(v1, a2, 0, a3);
        SetObjectFlags(v1, OF_MIXER);
        SetObjectOpacity(v1, a4);
        Delay(2);
        SetObjectFadeOut(v1, 20, 1);
        while (IsObjectExist(a0)) {
            if (IsObjectExist(v1) == 0) {
                return;
            }
            Delay(1);
        }
        FreeObject(v1);
        return;
    } else {
        return;
    }
}
void ProduceDuplicator(int a0, int a1, int a2, int a3)  {
    while (IsObjectExist(a0)) {
        asynccall DuplicatorShadow(a0, a1, a2, (a3 - 8), 14);
        Delay(8);
    }
    return;
}
void Duplicator(int a0)  {
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
    BatchLoadShape("magic\\000\\*");
    BatchLoadShape("magic\\012\\*");
    DisablePlayMagic();
    DownBrightness(12, 5);
    v10 = 0;
    if (Rand(0, 3) == 3) {
        v10 = Rand(0, 90);
    }
    v9 = (1 - (2 * (intvIsLeft != 0)));
    if (!(intvIsLeft == 1)) {
        v8 = 128;
    } else {
        v8 = 0;
    }
    SetOverwhelming(intvAttackerMajor, 1);
    v5 = GetObjectScreenX(intvAttackerMajor);
    v7 = GetObjectScreenY(intvAttackerMajor);
    SetViewCamera((v5 - (180 * v9)), ((v7 - 120) + v10));
    Delay(15);
    SetObjectAnimate(intvAttackerMajor, OAF_ATTACK5);
    PlaySound(intvAttackerMajor, "att07", 255);
    SetOverwhelming(intvAttackerMajor, 0);
    Delay(10);
    v2 = CreateObjectByReference(intvAttackerMajor, 22002, v8, 0);
    SetObjectSpeed_Sphere(v2, v8, 0, 16);
    SetObjectFlags(v2, (OF_ENEMYGENERAL + OF_ENEMYFORCE));
    SetCallbackProcedure(v2, 12001);
    asynccall ProduceShadowTime(v2, 9999);
    asynccall MoveDuplicator(v2, 22001, 9999);
    asynccall LockCamera(v2, 180, ((-120) + v10), intvIsLeft);
    v5 = 16;
    v4 = 1;
    v3 = 0;
    asynccall TraceDuplicator(v2, intvDefenderMajor);
    do {
        SetObjectSpeed_Sphere(v2, v8, 0, (v5 / 16));
        if (v5 < (24 * 16)) {
            v5 = (v5 + v4);
            v4 = (v4 + 1);
        }
        if ((v3 & 1) == 0) {
            if (!(v5 < 255)) {
                PlaySound(v2, "m012snd01", 255);
            } else {
                PlaySound(v2, "m012snd01", 255);
            }
        }
        v3++;
        Delay(1);
        v1 = IsObjectExist(v2);
        v6 = GetObjectScreenX(v2);
        if ((v6 <= 0) || (v6 >= GetBattleWidthInScreenX())) {
            FreeObject(v2);
        }
    } while (v1);
    Delay(60);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void StopAllSoldier(int a0, int a1)  {
    int v1;
    int v2;
    int v3;
    int v4;
    v1 = 0;
    while ((v1 < 71)) {
        v2 = 0;
        while ((v2 < 15)) {
            v3 = GetForceHandleByXY(v1, v2);
            if (v3 && (v3 != intvDefenderMajor)) {
                v4 = GetSoldierSide(v3);
                if (a0 == v4) {
                    AddAttackCounter(v3, a1);
                }
            }
            v2++;
        }
        v1++;
    }
    return;
}
void Func13001(int a0)  {
    int v1;
    MoveObject(a0, 0, 0, 256);
    Delay(2);
    SetObjectScale(a0, 65536, 65536);
    SetObjectSpeed_Sphere(a0, 128, 192, 3);
    Delay(64);
    SetObjectSpeed_Sphere(a0, 128, 192, 0);
    Delay(64);
    v1 = 0;
    while ((v1 < 10)) {
        SetObjectScale(a0, 0, (65536 + (v1 * 65536)));
        Delay(1);
        v1++;
    } while ((v1 < 30)) {
        SetObjectScale(a0, 0, (65536 + (v1 * 65536)));
        MoveObject(a0, 0, 0, 30);
        Delay(1);
        v1++;
    }
    FreeObject(a0);
    return;
}
void ProduceSomethingXY2(int a0, int a1, int a2, int a3)  {
    int v1;
    v1 = CreateObjectByReference(intvAttackerMajor, a2, 128, 0);
    SetObjectCoordinate(v1, a0, a1, 0);
    asynccall Func13001(v1);
    Wait("Func13001");
    SetObjectFadeOut(v1, 16, 1);
    return;
}
void Frozen()  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    int v6;
    int v7;
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
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL2);
    SetOverwhelming(intvAttackerMajor, 0);
    Delay(30);
    if (!(intvIsLeft == 1)) {
        v6 = 128;
    } else {
        v6 = 0;
    }
    v7 = GetSoldierCount((intvIsLeft ^ 1));
    if (!(v7 > 0)) {
        if (!(intvIsLeft == 1)) {
            v3 = GetObjectScreenX(intvDefenderMajor);
            v5 = (v3 + 320);
        } else {
            v3 = GetObjectScreenX(intvDefenderMajor);
            v5 = (v3 - 320);
        }
    } else {
        v1 = ((v7 / 2) + 1);
        v2 = GetRandomSoldierHandleFromAlive((intvIsLeft ^ 1));
        v5 = GetObjectScreenX(v2);
        do {
            v2 = GetRandomSoldierHandleFromAlive((intvIsLeft ^ 1));
            v3 = GetObjectScreenX(v2);
            if (!(intvIsLeft == 1)) {
                if (v5 < v3) {
                    v5 = v3;
                }
            } else {
                if (v5 > v3) {
                    v5 = v3;
                }
            }
            v1--;
        } while (v1 > 0);
    }
    if (!(intvIsLeft == 1)) {
        v5 = (v5 - 320);
        SetViewCamera(v5, 0);
    } else {
        v5 = (v5 + 320);
        SetViewCamera(v5, 0);
    }
    Delay(15);
    PlaySound1("m013snd01", 255);
    if (!(intvIsLeft == 1)) {
        asynccall ProduceSomethingXY2((v5 - 240), (468 + 240), 23001, v6);
        asynccall ProduceSomethingXY2((v5 - 80), (468 + 240), 23002, v6);
        asynccall ProduceSomethingXY2((v5 + 80), (468 + 240), 23003, v6);
        asynccall ProduceSomethingXY2((v5 + 240), (468 + 240), 23004, v6);
        asynccall ProduceSomethingXY2((v5 - 240), (468 - 240), 23005, v6);
        asynccall ProduceSomethingXY2((v5 - 80), (468 - 240), 23006, v6);
        asynccall ProduceSomethingXY2((v5 + 80), (468 - 240), 23007, v6);
        asynccall ProduceSomethingXY2((v5 + 240), (468 - 240), 23008, v6);
    } else {
        asynccall ProduceSomethingXY2((v5 - 240), (468 + 240), 23009, v6);
        asynccall ProduceSomethingXY2((v5 - 80), (468 + 240), 23010, v6);
        asynccall ProduceSomethingXY2((v5 + 80), (468 + 240), 23011, v6);
        asynccall ProduceSomethingXY2((v5 + 240), (468 + 240), 23012, v6);
        asynccall ProduceSomethingXY2((v5 - 240), (468 - 240), 23013, v6);
        asynccall ProduceSomethingXY2((v5 - 80), (468 - 240), 23014, v6);
        asynccall ProduceSomethingXY2((v5 + 80), (468 - 240), 23015, v6);
        asynccall ProduceSomethingXY2((v5 + 240), (468 - 240), 23016, v6);
    }
    asynccall StopAllSoldier((intvIsLeft ^ 1), 25);
    Wait("ProduceSomethingXY2");
    Delay(60);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void DemonDancingCallback(int a0, int a1) callsign 14001  {
    Hurt(a0, a1, GetGlobal(16));
    if (GetGlobal(16) == 20) {
        SetGlobal(16, 0);
    }
    return;
}
void CreateLastAttack(int a0, int a1)  {
    int v1;
    int v2;
    v1 = CreateObjectByReference(a0, 24005, a1, 0);
    SetObjectFlags(v1, (OF_ENEMYGENERAL + OF_ENEMYFORCE));
    v2 = 0;
    while ((v2 <= 20)) {
        SetCoordinateByReference_Cylind(v1, a0, a1, (48 + (v2 * 4)), 64);
        Delay(1);
        v2++;
    }
    SetObjectFadeOut(v1, 16, 1);
    return;
}
void DemonDancing()  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    int v6;
    int v7;
    BatchLoadShape("magic\\000\\*");
    BatchLoadShape("magic\\014\\*");
    DisablePlayMagic();
    DownBrightness(12, 5);
    SetOverwhelming(intvAttackerMajor, 1);
    v4 = GetObjectScreenX(intvAttackerMajor);
    v5 = GetObjectScreenY(intvAttackerMajor);
    SetViewCamera(v4, (v5 - 120));
    if (!(intvIsLeft == 1)) {
        v2 = GetForceHandleByXY(((v4 / 96) - 1), (v5 / 72));
    } else {
        v2 = GetForceHandleByXY(((v4 / 96) + 1), (v5 / 72));
    }
    if (!(v2 == intvDefenderMajor)) {
        v6 = 0;
    } else {
        v6 = 1;
    }
    if (!(intvIsLeft == 1)) {
        v7 = 128;
    } else {
        v7 = 0;
    }
    v4 = GetObjectScreenX(intvAttackerMajor);
    v5 = GetObjectScreenY(intvAttackerMajor);
    SetViewCamera(v4, (v5 - 120));
    if (!(intvIsLeft == 0)) {
        v2 = GetForceHandleByXY(((v4 / 96) + 1), (v5 / 72));
    } else {
        v2 = GetForceHandleByXY(((v4 / 96) - 1), (v5 / 72));
    }
    if (!(v2 == intvDefenderMajor)) {
        AddAttackCounter(intvAttackerMajor, 1);
        SetObjectAnimate(intvAttackerMajor, OAF_ATTACK5);
        SetOverwhelming(intvAttackerMajor, 0);
        Delay(10);
        PlaySound1("att07", 255);
        PlaySound(intvAttackerMajor, "yell01", 255);
        v2 = CreateObjectByReference(intvAttackerMajor, 24001, v7, 0);
        SetCoordinateByReference_Cylind(v2, intvAttackerMajor, v7, 58, 30);
        Delay(20);
        SetObjectFadeOut(v2, 16, 1);
        SetGlobal(16, 2);
        if (!(intvIsLeft == 1)) {
            asynccall LockTargetXY((v4 - 96), v5, 2501, v7, 2, 14001, 0, 0, (OF_ENEMYFORCE + OF_ENEMYGENERAL));
        } else {
            asynccall LockTargetXY((v4 + 96), v5, 2501, v7, 2, 14001, 0, 0, (OF_ENEMYFORCE + OF_ENEMYGENERAL));
        }
    } else {
        AddAttackCounter(intvAttackerMajor, 4);
        AddAttackCounter(intvDefenderMajor, 4);
        SetObjectAnimate(intvAttackerMajor, OAF_ATTACK4);
        SetOverwhelming(intvAttackerMajor, 0);
        Delay(10);
        PlaySound1("att07", 255);
        PlaySound(intvAttackerMajor, "yell01", 255);
        v2 = CreateObjectByReference(intvAttackerMajor, 24001, v7, 0);
        SetCoordinateByReference_Cylind(v2, intvAttackerMajor, v7, 58, 30);
        Delay(20);
        SetObjectFadeOut(v2, 16, 1);
        v1 = 20;
        do {
            SetGlobal(16, 2);
            if (!(intvIsLeft == 1)) {
                asynccall LockTargetXY((v4 - 96), v5, 2501, v7, 2, 14001, 0, 0, (OF_ENEMYFORCE + OF_ENEMYGENERAL));
            } else {
                asynccall LockTargetXY((v4 + 96), v5, 2501, v7, 2, 14001, 0, 0, (OF_ENEMYFORCE + OF_ENEMYGENERAL));
            }
            v3 = CreateObjectByReference(intvDefenderMajor, Rand(24002, 24004), v7, Rand(64, (48 + 64)));
            MoveObject(v3, Rand((-32), 32), 0, 0);
            SetObjectFadeOut(v3, 16, 1);
            asynccall SmallFireBall(v3, v3, 10, 0);
            Delay(5);
            v1--;
        } while (v1 > 0);
        Delay(10);
        PlaySound(intvAttackerMajor, "att07", 255);
        CreateLastAttack(intvAttackerMajor, v7);
        SetGlobal(16, 20);
        if (!(intvIsLeft == 1)) {
            asynccall LockTargetXY((v4 - 96), v5, 2501, v7, 2, 14001, 0, 0, (OF_ENEMYFORCE + OF_ENEMYGENERAL));
        } else {
            asynccall LockTargetXY((v4 + 96), v5, 2501, v7, 2, 14001, 0, 0, (OF_ENEMYFORCE + OF_ENEMYGENERAL));
        }
    }
    Delay(90);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void FlyingSwordCallback(int a0, int a1) callsign 15001  {
    int v1;
    if (!(a1 == intvDefenderMajor)) {
        Hurt(a0, a1, 0);
    } else {
        if (GetGlobal(17)) {
            v1 = (intvMagicAttackValue / GetGlobal(17));
        }
        Hurt(a0, a1, v1);
        if (v1 > 0) {
            SetGlobal(17, (GetGlobal(17) * 2));
        }
    }
    return;
}
void Func15010(int a0, int a1)  {
    int v1;
    int v2;
    v2 = 0;
    while ((v2 < 15)) {
        v1 = CreateObjectByReference(a1, Rand(25001, 25002), Rand(0, 255), 0);
        SetObjectGravity(v1, 10752);
        SetObjectScale(v1, 32768, 32768);
        SetObjectSpeed_Sphere(v1, Rand(0, 256), Rand(16, (128 - 16)), 4);
        SetObjectFadeOut(v1, 64, 1);
        SetObjectScaleShrink(v1, 512);
        v2++;
    }
    return;
}
void Func15009(int a0, int a1, int a2)  {
    int v1;
    v1 = CreateObjectByReference(intvAttackerMajor, 2501, 0, 0);
    SetObjectCoordinate(v1, a0, a1, a2);
    asynccall LockTargetTime2(v1, 25008, 8);
    asynccall Func15010(v1, v1);
    Delay(8);
    FreeObject(v1);
    return;
}
void TraceSmallSword(int a0)  {
    int v1;
    int v2;
    int v3;
    int v4;
    while (IsObjectExist(a0)) {
        if (GetObjectScreenZ(a0) <= 4) {
            v1 = GetObjectScreenX(a0);
            v2 = GetObjectScreenY(a0);
            FreeObject(a0);
            v3 = 0;
            while ((v3 < 10)) {
                v4 = CreateObjectRaw(v1, v2, 0, Rand(0, 255), Rand(25001, 25002));
                SetObjectGravity(v4, 10752);
                SetObjectScale(v4, 32768, 32768);
                SetObjectSpeed_Sphere(v4, Rand(0, 256), Rand(16, 32), 4);
                SetObjectFadeOut(v4, 16, 1);
                SetObjectScaleShrink(v4, 512);
                v3++;
            }
            break;
        }
        Delay(1);
    }
    return;
}
void CreateSmallSwords(int a0, int a1, int a2)  {
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
    while ((v2 < v7)) {
        v8 = GetNthSoldierInRect((intvIsLeft ^ 1), v4, v5, 5, 4, Rand(0, (v7 - 1)));
        if (v8) {
            Delay(Rand(0, 4));
            v1 = CreateObjectByReference(v8, 25005, a2, 280);
            SetObjectSpeed_Sphere(v1, 0, 192, 60);
            SetObjectFlags(v1, OF_ENEMYFORCE);
            SetCallbackProcedure(v1, 15001);
            PlaySound(v1, "att04", 128);
            asynccall TraceSmallSword(v1);
            v6++;
            if (v6 >= 16) {
                break;
            }
        }
        v2++;
    }
    v2 = v6;
    while ((v2 < 24)) {
        Delay(Rand(2, 6));
        v1 = CreateObjectByReference(a0, 25005, a2, 0);
        SetCoordinateByReference_Cylind(v1, a0, Rand(0, 255), Rand(64, 130), 280);
        SetObjectSpeed_Sphere(v1, 0, 192, 16);
        PlaySound(v1, "att04", 128);
        asynccall TraceSmallSword(v1);
        v2++;
    }
    return;
}
void SwordBomb(int a0, int a1, int a2)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    int v6;
    asynccall LockTargetXY((a0 + 96), a1, 2501, a2, 8, 15001, 0, 1, (OF_ENEMYGENERAL + OF_ENEMYFORCE));
    asynccall LockTargetXY((a0 - 96), a1, 2501, a2, 8, 15001, 0, 1, (OF_ENEMYGENERAL + OF_ENEMYFORCE));
    asynccall LockTargetXY(a0, (a1 + 72), 2501, a2, 8, 15001, 0, 1, (OF_ENEMYGENERAL + OF_ENEMYFORCE));
    asynccall LockTargetXY(a0, (a1 - 72), 2501, a2, 8, 15001, 0, 1, (OF_ENEMYGENERAL + OF_ENEMYFORCE));
    asynccall LockTargetXY((a0 + 96), (a1 + 72), 2501, a2, 8, 15001, 0, 1, (OF_ENEMYGENERAL + OF_ENEMYFORCE));
    asynccall LockTargetXY((a0 + 96), (a1 - 72), 2501, a2, 8, 15001, 0, 1, (OF_ENEMYGENERAL + OF_ENEMYFORCE));
    asynccall LockTargetXY((a0 - 96), (a1 + 72), 2501, a2, 8, 15001, 0, 1, (OF_ENEMYGENERAL + OF_ENEMYFORCE));
    asynccall LockTargetXY((a0 - 96), (a1 - 72), 2501, a2, 8, 15001, 0, 1, (OF_ENEMYGENERAL + OF_ENEMYFORCE));
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
    while ((v1 < 16)) {
        v2 = 0;
        while ((v2 < Rand(1, 2))) {
            v3 = (a0 + Rand((-48), 48));
            v4 = (a1 + Rand((-16), 16));
            CreateObjectRaw(v3, v4, v5, 0, Rand(25007, 25008));
            v2++;
        }
        if (Rand(0, 3) == 0) {
            v3 = (a0 + Rand((-48), 48));
            v4 = (a1 + Rand((-16), 16));
            v6 = CreateObjectRaw(v3, v4, v5, Rand(0, 255), Rand(25009, 25010));
            SetObjectSpeed_Cylind(v6, Rand(10, 16), Rand(10, 16));
        }
        Delay(1);
        v1++;
    }
    return;
}
void CreateBigSword(int a0, int a1)  {
    int v1;
    int v2;
    int v3;
    int v4;
    v3 = CreateObjectByReference(a0, 25006, a1, 0);
    MoveObject(v3, 0, 0, 300);
    SetObjectScale(v3, 65536, 65536);
    SetObjectSpeed_Sphere(v3, 0, 192, 40);
    SetObjectFlags(v3, (OF_ENEMYGENERAL + OF_ENEMYFORCE));
    SetCallbackProcedure(v3, 15001);
    v4 = 1;
    while ((v4 < 10)) {
        SetObjectScale(v3, (98304 + (v4 * 8192)), (106496 + (v4 * 8192)));
        Delay(1);
        v4++;
    }
    Delay(10);
    v1 = GetObjectScreenX(v3);
    v2 = GetObjectScreenY(v3);
    asynccall SwordBomb(v1, v2, a1);
    SetObjectFadeOut(v3, 16, 2);
    return;
}
void SwordAttack(int a0, int a1)  {
    int v2;
    int v3;
    int v4;
    int v5;
    v5 = GetSoldierCount((intvIsLeft ^ 1));
    v2 = 0;
    if (!(v5 > 0)) {
        v2 = intvDefenderMajor;
    } else {
        if (a1) {
            v2 = GetMostImportantSoldier((intvIsLeft ^ 1));
        }
        if (v2 == 0) {
            v2 = GetRandomSoldierHandleFromAlive((intvIsLeft ^ 1));
        }
    }
    v3 = GetObjectScreenX(v2);
    v4 = GetObjectScreenY(v2);
    MoveCamera(v3, (v4 - 240), 20);
    asynccall CreateBigSword(v2, a0);
    Delay(10);
    asynccall CreateSmallSwords(v2, Rand(0, 63), a0);
    Delay(90);
    return;
}
void ProduceSwordShadow(int a0, int a1, int a2)  {
    while (IsObjectExist(a0)) {
        CreateShadowObject(a0, a1, a2);
        Delay(3);
    }
    return;
}
void LargerSword(int a0, int a1, int a2, int a3)  {
    int v1;
    int v3;
    int v4;
    int v5;
    v3 = 256;
    v4 = 256;
    v1 = 0;
    while ((v1 < 32)) {
        SetObjectScale(a0, v3, v4);
        Delay(1);
        if (v3 < a1) {
            v3 = (v3 + 2048);
        }
        if (v4 < a2) {
            v4 = (v4 + 2048);
        }
        v1++;
    }
    Delay(30);
    v5 = CreateObjectByReference(a0, 25004, a3, 0);
    FreeObject(a0);
    SetObjectSpeed_Sphere(v5, 0, 64, (-1));
    Delay(25);
    SetObjectSpeed_Sphere(v5, 0, 64, 16);
    asynccall ProduceSwordShadow(v5, 16, 1);
    SetObjectLife(v5, 60);
    return;
}
void PrepareSword(int a0, int a1, int a2)  {
    int v1;
    v1 = CreateObjectByReference(a0, 25003, a2, 0);
    SetCoordinateByReference_Cylind(v1, a0, a1, 128, 64);
    SetObjectSpeed_Sphere(v1, 0, 64, 1);
    SetObjectScale(v1, 256, 256);
    asynccall LargerSword(v1, 65536, 65536, a2);
    return;
}
void FlyingSword(int a0)  {
    int v1;
    int v3;
    int v4;
    int v5;
    int v6;
    int v7;
    int v8;
    int v9;
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
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL1);
    SetOverwhelming(intvAttackerMajor, 0);
    if (!(intvIsLeft == 1)) {
        v7 = 128;
    } else {
        v7 = 0;
    }
    v5 = GetObjectScreenX(intvAttackerMajor);
    GetObjectScreenY(intvAttackerMajor);
    Delay(15);
    switch (a0) {
    case 0:
        v8 = 4;
        PlaySound1("m015snd01", 255);
        break;
    case 1:
        v8 = 8;
        PlaySound1("m015snd01", 255);
        break;
    case 2:
        v8 = 16;
        PlaySound1("m015snd03", 255);
        break;
        break;
    }
    v6 = (256 / v8);
    v9 = Rand(0, 63);
    v1 = 0;
    while ((v1 < v8)) {
        asynccall PrepareSword(intvAttackerMajor, (v9 + (v1 * v6)), v7);
        Delay(1);
        v1++;
    }
    Delay(135);
    v1 = 0;
    while ((v1 <= a0)) {
        asynccall SwordAttack(v7, ((2 - v1) / 2));
        Delay(100);
        v1++;
    }
    Wait("SwordBomb");
    Wait("TraceSmallSword");
    Delay(60);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void CreateHolyBall(int a0, int a1, int a2)  {
    int v1;
    int v2;
    int v3;
    Delay(Rand(1, 8));
    v1 = CreateObjectByReference(a0, a2, 0, a1);
    v2 = Rand(49152, 81920);
    SetObjectScale(v1, v2, v2);
    SetCoordinateByReference_Cylind(v1, a0, Rand(0, 255), Rand(8, 38), a1);
    SetObjectFlags(v1, OF_MIXER);
    SetObjectOpacity(v1, 0);
    SetObjectSpeed_Cylind(v1, 0, Rand(1, 7));
    v3 = 1;
    while ((v3 <= 8)) {
        Delay(1);
        SetObjectOpacity(v1, v3);
        v3++;
    }
    v3 = Rand(16, 32);
    SetObjectScaleShrink(v1, 256);
    SetObjectFadeOut(v1, v3, 1);
    Delay(v3);
    return;
}
void RoundHolyBall(int a0, int a1, int a2, int a3)  {
    int v1;
    v1 = CreateObjectByReference(a0, a3, 0, a1);
    SetObjectScale(v1, 81920, 81920);
    SetCoordinateByReference_Cylind(v1, a0, 192, 16, a1);
    MoveObject(v1, a2, 0, 0);
    Delay(2);
    SetObjectFadeOut(v1, 8, 1);
    return;
}
void IncreaseHP()  {
    int v1;
    int v2;
    v1 = GetMajorHP(intvIsLeft);
    v2 = 1;
    while ((v2 <= intvMagicAttackValue)) {
        SetMajorHP(intvIsLeft, (v1 + v2));
        Delay(2);
        v2++;
    }
    return;
}
void TraceHolyLight(int a0, int a1, int a2, int a3)  {
    int v1;
    int v2;
    int v3;
    v3 = (a2 - a1);
    v1 = 1;
    while ((v1 <= a3)) {
        v2 = (a1 + ((v3 / a3) * v1));
        SetObjectScale(a0, v2, 65536);
        Delay(1);
        v1++;
    }
    Delay(60);
    SetObjectScaleShrink(a0, 1024);
    SetObjectFadeOut(a0, 48, 1);
    Delay(48);
    return;
}
void CreateHolyLight(int a0, int a1)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    int v6;
    switch (a1) {
    case 0:
        v4 = 26001;
        v6 = 26004;
        break;
    case 1:
        v4 = 26002;
        v6 = 26005;
        break;
    case 2:
        v4 = 26003;
        v6 = 26006;
        break;
        break;
    }
    if (!(intvIsLeft == 1)) {
        v5 = (-6);
    } else {
        v5 = 0;
    }
    v1 = CreateObjectByReference(a0, v4, 0, 220);
    SetObjectScale(v1, 2048, 65536);
    SetCoordinateByReference_Cylind(v1, a0, 192, 16, 240);
    MoveObject(v1, v5, 0, 0);
    SetObjectGravity(v1, 98304);
    v2 = 0;
    while ((v2 < 30)) {
        asynccall RoundHolyBall(a0, (240 - (v2 * 8)), v5, v6);
        Delay(1);
        v2++;
    }
    Delay(40);
    asynccall TraceHolyLight(v1, 2048, 69632, 32);
    asynccall IncreaseHP();
    v3 = 0;
    while ((v3 < 8)) {
        v2 = 0;
        while ((v2 < 10)) {
            asynccall CreateHolyBall(a0, Rand(5, (40 + (v3 * 2))), v6);
            v2++;
        }
        Delay(Rand(8, 12));
        v3++;
    }
    Wait("TraceHolyLight");
    return;
}
void Heal(int a0)  {
    int v1;
    int v2;
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
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL1);
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
void FireDragonCallback(int a0, int a1) callsign 17001  {
    if (!(a1 == intvDefenderMajor)) {
        Hurt(a0, a1, 0);
        asynccall FireMan(a1, 10015, 60);
    } else {
        Hurt(a0, a1, GetGlobal(18));
        SetGlobal(18, 2);
        asynccall LockTargetTime2(a1, 10015, 60);
    }
    return;
}
void CreateStart(int a0, int a1)  {
    int v1;
    int v2;
    v1 = CreateObjectByReference(a0, 27010, a1, 64);
    SetObjectScale(v1, 131072, 114688);
    SetObjectFlags(v1, OF_MIXER);
    v2 = 0;
    while ((v2 < 16)) {
        SetObjectOpacity(v1, v2);
        Delay(1);
        v2++;
    }
    ClearObjectFlags(v1, OF_MIXER);
    Delay(64);
    SetObjectFadeOut(v1, 16, 2);
    return;
}
void sc4502(int a0, int a1, int a2)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    v1 = CreateObjectByReference(a0, (27005 + ((GetGlobal(19) / 4) & 1)), a1, 0);
    v3 = 0;
    v4 = 0;
    do {
        if (!(v3 > 3)) {
            SetObjectScale(v1, 256, 256);
        } else {
            SetObjectScale(v1, 65536, 65536);
            v3 = 0;
            v4++;
            if (v4 == 7) {
                v3 = 1;
            }
            if (v4 == 8) {
                v2 = CreateObjectByReference(v1, 27007, a1, 0);
                v5 = ((a2 & 255) - 128);
                if (v5 < 0) {
                    v5 = (0 - v5);
                }
                if (!(v5 < 16)) {
                    if (!(v5 < 32)) {
                        if (!(v5 < 48)) {
                            if (!(v5 < 80)) {
                                if (!(v5 < 96)) {
                                    if (!(v5 < 112)) {
                                        SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017c40001");
                                    } else {
                                        SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017c20001");
                                    }
                                } else {
                                    SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017c10001");
                                }
                            } else {
                                SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017c10001");
                            }
                        } else {
                            SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017c30001");
                        }
                    } else {
                        SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017c30001");
                    }
                } else {
                    SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017c50001");
                }
                v5 = (a1 & 255);
                if ((v5 >= 192) && (v5 < 224)) {
                    SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017c50001");
                }
                if ((v5 > 170) && (v5 < 192)) {
                    SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017c70001");
                }
                SetObjectScale(v2, (65536 * (1 - (2 * (((a1 - 64) & 255) > 128)))), 65536);
                SetObjectScale(v1, (49152 * (1 - (2 * (((a1 - 64) & 255) > 128)))), 49152);
                SetObjectLife(v2, 1);
            }
            if (v4 == 9) {
                SetObjectScale(v1, 256, 256);
            }
        }
        Delay(1);
        v3++;
        if (v4 == 9) {
            break;
        }
    } while (IsObjectExist(a0));
    FreeObject(v1);
    return;
}
void sc4501(int a0, int a1)  {
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
    SetObjectFlags(v2, (OF_ENEMYFORCE + OF_ENEMYGENERAL));
    SetCallbackProcedure(v2, 17001);
    v1 = (60 * 6);
    v4 = 128;
    v5 = 0;
    PlaySound1("m017snd01", 255);
    do {
        SetObjectSpeed_Sphere(v2, v4, 0, 7);
        v6 = GetObjectScreenX(v2);
        v7 = GetObjectScreenY(v2);
        v8 = CalcSin(v5, 32);
        SetObjectCoordinate(v2, v6, v7, (64 + v8));
        SetObjectScale(v2, (65536 * (1 - (2 * (((v4 - 64) & 255) > 128)))), 65536);
        v3 = ((v5 & 255) - 128);
        if (v3 < 0) {
            v3 = (0 - v3);
        }
        if (!(v3 < 16)) {
            if (!(v3 < 32)) {
                if (!(v3 < 48)) {
                    if (!(v3 < 80)) {
                        if (!(v3 < 96)) {
                            if (!(v3 < 112)) {
                                SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017a30001");
                            } else {
                                SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017a30001");
                            }
                        } else {
                            SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017a10001");
                        }
                    } else {
                        SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017a10001");
                    }
                } else {
                    SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017a20001");
                }
            } else {
                SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017a20001");
            }
        } else {
            SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017a40001");
        }
        v3 = (v4 & 255);
        if ((v3 >= 192) && (v3 < 224)) {
            SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017a40001");
        }
        if ((v3 > 170) && (v3 < 192)) {
            SetObjectDrawingFileItem(v2, "MAGIC\\017\\m017a60001");
        }
        asynccall sc4502(v2, v4, v5);
        Delay(1);
        SetGlobal(19, GetGlobal(19) + 1);
        v4 = (v4 + 2);
        v5 = (v5 + 4);
        v1--;
    } while (v1 > 0);
    SetObjectFadeOut(v2, 16, 1);
    Delay(16);
    SetGlobal(20, GetGlobal(20) + 1);
    return;
}
void FireDragon(int a0)  {
    int v2;
    int v3;
    int v4;
    int v6;
    int v7;
    int v8;
    int v9;
    int v10;
    int v11;
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
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL2);
    Delay(20);
    SetOverwhelming(intvAttackerMajor, 0);
    SetGlobal(19, 0);
    SetGlobal(20, 0);
    if (!(intvIsLeft == 1)) {
        v11 = 128;
    } else {
        v11 = 128;
    }
    v2 = GetMostImportantSoldier((intvIsLeft ^ 1));
    if (v2 == 0) {
        v2 = intvDefenderMajor;
    }
    v3 = GetObjectScreenX(v2);
    v4 = GetObjectScreenY(v2);
    MoveCamera(v3, (v4 - 220), 20);
    v2 = CreateObjectByReference(v2, 2501, v11, 0);
    PlaySound1("m017snd02", 255);
    SetObjectCoordinate(v2, (v3 + 120), (v4 + 120), 0);
    asynccall sc4501(v2, v11);
    Delay(50);
    if (a0 > 0) {
        v6 = CreateObjectByReference(v2, 2501, v11, 0);
        v8 = GetMostImportantSoldier((intvIsLeft ^ 1));
        if (v8 == 0) {
            v8 = intvDefenderMajor;
        }
        v3 = GetObjectScreenX(v8);
        v4 = GetObjectScreenY(v8);
        MoveCamera(v3, (v4 - 220), 20);
        SetObjectCoordinate(v6, (v3 + 120), (v4 + 120), 0);
        asynccall sc4501(v6, v11);
        Delay(50);
    }
    if (a0 > 1) {
        v7 = CreateObjectByReference(v2, 2501, v11, 0);
        v8 = GetMostImportantSoldier((intvIsLeft ^ 1));
        if (v8 == 0) {
            v8 = intvDefenderMajor;
        }
        v3 = GetObjectScreenX(v8);
        v4 = GetObjectScreenY(v8);
        MoveCamera(v3, (v4 - 220), 20);
        SetObjectCoordinate(v7, (v3 + 120), (v4 + 120), 0);
        asynccall sc4501(v7, v11);
        Delay(50);
    }
    if (a0 > 2) {
        v9 = CreateObjectByReference(v2, 2501, v11, 0);
        v8 = GetRandomSoldierHandleFromAlive((intvIsLeft ^ 1));
        if (v8 == 0) {
            v8 = intvDefenderMajor;
        }
        v3 = GetObjectScreenX(v8);
        v4 = GetObjectScreenY(v8);
        MoveCamera(v3, (v4 - 220), 20);
        SetObjectCoordinate(v9, (v3 + 120), (v4 + 120), 0);
        asynccall sc4501(v9, v11);
        Delay(50);
    }
    if (a0 > 3) {
        v10 = CreateObjectByReference(v2, 2501, v11, 0);
        v8 = GetRandomSoldierHandleFromAlive((intvIsLeft ^ 1));
        if (v8 == 0) {
            v8 = intvDefenderMajor;
        }
        v3 = GetObjectScreenX(v8);
        v4 = GetObjectScreenY(v8);
        MoveCamera(v3, (v4 - 220), 20);
        SetObjectCoordinate(v10, (v3 + 120), (v4 + 120), 0);
        asynccall sc4501(v10, v11);
    }
    do {
        Delay(1);
    } while (GetGlobal(20) <= a0);
    FreeObject(v2);
    FreeObject(v6);
    FreeObject(v7);
    FreeObject(v9);
    FreeObject(v10);
    Delay(60);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void CallDragonCallback(int a0, int a1) callsign 18001  {
    if (!((a1 == intvDefenderMajor) && (GetGlobal(22) == 0))) {
        Hurt(a0, a1, 0);
        asynccall FireMan(a1, 10015, 60);
    } else {
        Hurt(a0, a1, intvMagicAttackValue);
        SetGlobal(22, 1);
        asynccall LockTargetTime2(a1, 10015, 60);
    }
    return;
}
void sc3602(int a0, int a1, int a2, int a3)  {
    int v1;
    int v2;
    int v3;
    int v5;
    int v6;
    switch (a0) {
    case 0:
        v5 = 10;
        break;
    case 1:
        v5 = 20;
        break;
        v5 = 20;
        break;
    }
    v1 = 0;
    while ((v1 < v5)) {
        v2 = (a1 + Rand((-48), 48));
        v3 = (a2 + Rand((-16), 16));
        v6 = CreateObjectRaw(v2, v3, (a3 + 20), 0, Rand(28002, 28004));
        SetObjectSpeed_Cylind(v6, 0, Rand(10, 30));
        PlaySound1("m016a", 255);
        Delay(Rand(0, 5));
        v1++;
    }
    Delay(15);
    return;
}
void sc3601(int a0, int a1, int a2, int a3, int a4)  {
    int v1;
    int v2;
    Delay(Rand(10, 50));
    v1 = CreateObjectRaw(a2, a3, a4, a1, 28001);
    v2 = 60;
    while ((v2 > 0)) {
        a4++;
        SetObjectCoordinate(v1, a2, a3, a4);
        v2--;
        Delay(1);
    }
    Delay(30);
    asynccall sc3602(a0, a2, a3, a4);
    v2 = 0;
    while ((v2 < 10)) {
        SetObjectScale(v1, 0, (65536 + (v2 * 65536)));
        Delay(1);
        v2++;
    } while ((v2 < 30)) {
        a4 = (a4 + (v2 * 10));
        SetObjectScale(v1, 0, (65536 + (v2 * 65536)));
        SetObjectCoordinate(v1, a2, a3, a4);
        Delay(1);
        v2++;
    }
    FreeObject(v1);
    return;
}
void sc3603(int a0, int a1, int a2, int a3)  {
    int v1;
    int v2;
    int v3;
    CreateObjectRaw(a1, a2, Rand(320, 360), 0, 28005);
    Delay(Rand(4, 10));
    v1 = (2 + (3 * a0));
    while ( 1 ) {
        v1--;
        if (!(v1 > 0))
            break;
        v2 = (a1 + Rand((-320), 320));
        v3 = (a2 + Rand((-100), 300));
        CreateObjectRaw(v2, v3, Rand(320, 360), 0, 28005);
        Delay(Rand(4, 10));
    }
    return;
}
void sc2512(int a0)  {
    int v1;
    int v2;
    v1 = 16384;
    SetObjectSpeed_Cylind(a0, Rand(4, 12), Rand((-2), 8));
    SetObjectFriction1(a0, 8192);
    v2 = Rand(30, 40);
    while ( 1 ) {
        v2--;
        if (!(v2 > 0))
            break;
        SetObjectScale(a0, v1, v1);
        v1 = (v1 - 256);
        Delay(1);
    }
    FreeObject(a0);
    return;
}
void sc3604() callsign 18002  {
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
    while ((v2 < 20)) {
        v1 = CreateObjectByReference(v3, 10003, Rand(0, 255), 0);
        SetObjectScale(v1, 16384, 16384);
        asynccall sc2512(v1);
        v2++;
    }
    PlaySound(v1, "m018snd03", 255);
    return;
}
void CallDragon(int a0)  {
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
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL1);
    Delay(20);
    SetOverwhelming(intvAttackerMajor, 0);
    if (!(intvIsLeft == 1)) {
        v10 = 128;
    } else {
        v10 = 0;
    }
    v1 = GetObjectScreenX(intvAttackerMajor);
    v2 = GetObjectScreenY(intvAttackerMajor);
    v3 = GetObjectScreenZ(intvAttackerMajor);
    PlaySound1("m018snd01", 255);
    switch (a0) {
    case 0:
        asynccall sc3602(a0, v1, v2, v3);
        v11 = 50;
        SetGlobal(21, 110);
        break;
    case 1:
        asynccall sc3602(a0, v1, v2, v3);
        v11 = 100;
        SetGlobal(21, 220);
        break;
        asynccall sc3601(a0, v10, v1, v2, v3);
        v11 = (100 + 95);
        SetGlobal(21, 330);
        break;
    }
    v4 = 0;
    while ((v4 < v11)) {
        v12 = Rand(1, 2);
        v5 = 0;
        while ((v5 < v12)) {
            v7 = (v1 + Rand((-48), 48));
            v8 = (v2 + Rand((-16), 16));
            if (!(a0 == 2)) {
                CreateObjectRaw(v7, v8, v9, 0, Rand(28006, 28007));
            } else {
                CreateObjectRaw((v7 - 48), v8, v9, 0, Rand(28006, 28007));
                CreateObjectRaw((v7 + 48), v8, v9, 0, Rand(28006, 28007));
            }
            v5++;
        }
        if (Rand(0, 3) == 0) {
            v7 = (v1 + Rand((-48), 48));
            v8 = (v2 + Rand((-16), 16));
            v13 = CreateObjectRaw(v7, v8, v9, Rand(0, 255), Rand(28008, 28009));
            SetObjectSpeed_Cylind(v13, Rand(10, 16), Rand(10, 16));
        }
        Delay(1);
        switch (v4) {
        case 49:
            PlaySound1("m018snd02", 255);
            break;
        case 99:
            PlaySound1("m018snd01", 255);
            break;
        case 149:
            PlaySound1("m018snd02", 255);
            break;
            break;
        }
        v4++;
    }
    if (!(a0 > 1)) {
        Wait("sc3602");
    } else {
        Wait("sc3601");
    }
    v13 = GetMostImportantSoldier((intvIsLeft ^ 1));
    if (v13 == 0) {
        v13 = intvDefenderMajor;
    }
    v1 = GetObjectScreenX(v13);
    v2 = GetObjectScreenY(v13);
    v3 = GetObjectScreenZ(v13);
    if (!(v2 > 240)) {
        asynccall MoveCamera(v1, 0, 20);
    } else {
        asynccall MoveCamera(v1, (v2 - 240), 20);
    }
    Wait("MoveCamera");
    if (IsObjectExist(v13) == 0) {
        v13 = GetMostImportantSoldier((intvIsLeft ^ 1));
        if (v13 == 0) {
            v13 = intvDefenderMajor;
        }
    }
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
void RunningBowCallback(int a0, int a1) callsign 19001  {
    int v1;
    SetObjectFadeOut(a0, 2, 1);
    if (!(a1 == intvDefenderMajor)) {
        Hurt(a0, a1, 0);
        asynccall FireMan(a1, 10015, 60);
        SmallFireBall2(a0, a1, 15, 35);
    } else {
        SetGlobal(23, GetGlobal(23) + 1);
        if (!(GetGlobal(23) == 0)) {
            v1 = 2;
            SmallFireBall2(a0, a1, 12, 48);
        } else {
            v1 = (intvMagicAttackValue - (5 + ((GetGlobal(24) * 2) * 2)));
            HitGeneral(a0, a1, 11002, 1, 48, 0);
        }
        Hurt(a0, a1, v1);
    }
    return;
}
void CreateBowLight(int a0, int a1, int a2, int a3)  {
    int v1;
    v1 = CreateObjectByReference(a0, a1, a2, 0);
    SetObjectSpeed_Sphere(v1, a2, 0, a3);
    SetObjectScale(v1, 24576, 16384);
    while (IsObjectExist(a0)) {
        Delay(1);
    }
    SetObjectFadeOut(v1, 4, 1);
    return;
}
int CreateBow(int a0, int a1)  {
    int v1;
    v1 = CreateObjectByReference(a0, 29001, a1, 0);
    SetObjectFlags(v1, OF_MIXER);
    SetObjectOpacity(v1, 0);
    SetObjectFlags(v1, (OF_ENEMYGENERAL + OF_ENEMYFORCE));
    SetCallbackProcedure(v1, 19001);
    SetCoordinateByReference_Cylind(v1, a0, a1, 80, 80);
    return v1;
}
void TraceBow(int a0, int a1)  {
    int v1;
    ClearObjectFlags(a0, OF_MIXER);
    SetObjectSpeed_Sphere(a0, a1, 0, 16);
    asynccall CreateBowLight(a0, 29002, a1, 16);
    while (IsObjectExist(a0)) {
        v1 = GetObjectScreenX(a0);
        if ((v1 <= 0) || (v1 >= GetBattleWidthInScreenX())) {
            FreeObject(a0);
            break;
        }
        Delay(1);
    }
    return;
}
void RunningBow(int a0)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    int v6;
    int v7[9];
    int v16;
    SetGlobal(23, 0);
    SetGlobal(24, a0);
    BatchLoadShape("magic\\000\\*");
    BatchLoadShape("magic\\019\\*");
    DisablePlayMagic();
    DownBrightness(12, 5);
    if (!(Rand(0, 3) == 3)) {
        v5 = 0;
    } else {
        v5 = Rand(0, 90);
    }
    if (!(intvIsLeft == 1)) {
        v4 = 128;
        v3 = 1;
    } else {
        v4 = 0;
        v3 = (-1);
    }
    switch (a0) {
    case 0:
        v16 = 5;
        SetGlobal(23, 5);
        break;
    case 1:
        v16 = 7;
        SetGlobal(23, 7);
        break;
        v16 = 9;
        SetGlobal(23, 9);
        break;
    }
    SetOverwhelming(intvAttackerMajor, 1);
    v1 = GetObjectScreenX(intvAttackerMajor);
    v2 = GetObjectScreenY(intvAttackerMajor);
    SetViewCamera((v1 - (v3 * 180)), ((v2 - 140) + v5));
    Delay(15);
    SetObjectAnimate(intvAttackerMajor, OAF_SHOT3);
    Delay(15);
    PlaySound(intvAttackerMajor, "m019snd01", 255);
    SetOverwhelming(intvAttackerMajor, 0);
    v6 = 0;
    while ((v6 < v16)) {
        v7[v6] = CreateBow(intvAttackerMajor, v4);
        v6++;
    }
    v6 = 0;
    while ((v6 < v16)) {
        asynccall TraceBow(v7[v6], v4);
        if (!(v6 < 4)) {
            Delay(5);
        } else {
            Delay((3 + v6));
        }
        v6++;
    }
    asynccall LockCamera2(v7[0], 180, ((-160) + v5), intvIsLeft, v7[(v16 - 1)]);
    Wait("TraceBow");
    Delay(60);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void BlackHoleCallback(int a0, int a1)  {
    int v1;
    int v2;
    v1 = GetObjectScreenY(a0);
    ApproachObjectTowards(a1, a0, Rand(16, 25));
    while (IsObjectExist(a0)) {
        Delay(1);
        MoveObject(a1, 0, 0, 2);
        v2 = GetObjectScreenY(a1);
        if (v2 >= (v1 - 10)) {
            break;
        }
    }
    FreeObject(a1);
    return;
}
void KillMan(int a0, int a1)  {
    int v2;
    SetObjectContext(a0, 0, 123);
    v2 = 0;
    while ((v2 < 20)) {
        ApproachObjectTowards(a0, a1, Rand(4, 5));
        Delay(1);
        v2++;
    }
    v2 = 0;
    while ((v2 < 30)) {
        ApproachObjectTowards(a0, a1, Rand(2, 3));
        Delay(1);
        v2++;
    }
    KillSoldier(a0);
    asynccall BlackHoleCallback(a1, a0);
    return;
}
void Engulf(int a0, int a1, int a2, int a3)  {
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
    if (!(a3 == 0)) {
        v241 = 4;
        v242 = 16;
        v243 = 10;
    } else {
        v241 = 3;
        v242 = 9;
        v243 = 8;
    }
    v234 = 0;
    while ((v234 < v243)) {
        v232 = GetSoldierCountInRect((intvIsLeft ^ 1), v236, v237, v241, v242);
        if (v232 > 0) {
            v233 = 0;
            while ((v233 < v232)) {
                v1[v233] = GetNthSoldierInRect((intvIsLeft ^ 1), v236, v237, v241, v242, v233);
                v233++;
            }
            v233 = 0;
            while ((v233 < v232)) {
                v239 = Rand(0, (v232 - 1));
                v240 = v1[v233];
                v1[v233] = v1[v239];
                v1[v239] = v240;
                v233++;
            }
            if (!(a3 == 0)) {
                v232 = Rand(6, 9);
            } else {
                v232 = Rand(4, 7);
            }
            v233 = 0;
            while ((v233 < v232)) {
                if ((GetObjectContext(v1[v233], 0) != 123) && (v1[v233] != intvDefenderMajor)) {
                    asynccall KillMan(v1[v233], a0);
                }
                v245 = Rand(3, 4);
                Delay(v245);
                v244 = (v244 + v245);
                v233++;
            }
        }
        v234++;
    }
    if (!((a3 == 0) && (v244 < 160))) {
        if ((a3 != 0) && (v244 < 350)) {
            Delay((350 - v244));
        }
    } else {
        Delay((160 - v244));
    }
    return;
}
void CreateBlackHoleBall(int a0)  {
    int v1;
    while (GetGlobal(25)) {
        v1 = CreateObjectByReference(a0, 30013, 0, 0);
        SetCoordinateByReference_Cylind(v1, a0, Rand(152, 232), Rand(200, 300), Rand((-80), 80));
        ApproachObjectTowards(v1, a0, Rand(7, 12));
        SetObjectFadeOut(v1, 16, 1);
        Delay(Rand(10, 18));
    }
    return;
}
void CreateBlackHoleStar(int a0, int a1, int a2, int a3, int a4)  {
    int v1;
    v1 = CreateObjectByReference(a0, a1, 0, 0);
    SetObjectScale(v1, 131072, 131072);
    SetCoordinateByReference_Sphere(v1, a0, 0, a3, a2, 0);
    ApproachObjectTowards(v1, a0, a4);
    SetObjectFadeOut(v1, (a2 / a4), 1);
    while (IsObjectExist(v1)) {
        CreateShadowObject(v1, 4, 1);
        Delay(1);
        if (GetGlobal(25) == 0) {
            FreeObject(v1);
        }
    }
    return;
}
void CreateBlackHoleStars(int a0)  {
    asynccall CreateBlackHoleBall(a0);
    while (GetGlobal(25)) {
        asynccall CreateBlackHoleStar(a0, Rand(30003, 30011), Rand(30, 130), Rand(0, 127), Rand(3, 8));
        asynccall CreateBlackHoleStar(a0, Rand(30003, 30011), Rand(30, 130), Rand(128, 255), Rand(3, 8));
        Delay(Rand(0, 4));
    }
    return;
}
void CreateBlackHole(int a0, int a1, int a2)  {
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
    v2 = GetObjectScreenX(a0);
    v4 = GetObjectScreenY(a0);
    v3 = GetObjectScreenX(a1);
    v5 = GetObjectScreenY(a1);
    v11 = GetSoldierMaxX2(intvIsLeft);
    if (!(v11 == (-1))) {
        if (!(intvIsLeft == 1)) {
            v2 = (v11 - 200);
        } else {
            v2 = (v11 + 200);
        }
    } else {
        v2 = ((v2 + v3) / 2);
    }
    v4 = ((v4 + v5) / 2);
    MoveCamera(v2, v4, 10);
    if (!(a2 == 0)) {
        v9 = CreateObjectRaw(v2, (v4 + 700), 100, 128, 30014);
    } else {
        v9 = CreateObjectRaw(v2, (v4 + 700), 100, 128, 30012);
    }
    v6 = 0;
    while ((v6 < 106496)) {
        if (!(v6 < 24576)) {
            Delay(1);
        } else {
            Delay(6);
        }
        if (v6 == 24576) {
            SetObjectFadeOut(v9, 16, 2);
            v7 = CreateObjectRaw(v2, (v4 + 716), 100, 128, 30002);
            v1 = CreateObjectRaw(v2, (v4 + 700), 100, 128, 30001);
        }
        if (!(v6 >= 24576)) {
            SetObjectScale(v9, (v6 * 12), (v6 * 12));
        } else {
            SetObjectScale(v7, v6, v6);
            SetObjectScale(v1, v6, v6);
        }
        v6 = (v6 + 4096);
    }
    SetGlobal(25, 1);
    if (a2 > 0) {
        v10 = CreateObjectRaw(v2, (v4 + 700), 100, 128, 30015);
        SetObjectScale(v10, 94208, 94208);
    }
    asynccall CreateBlackHoleStars(v7);
    asynccall Engulf(v1, v2, v4, a2);
    Wait("Engulf");
    Delay(150);
    if (a2 > 0) {
        FreeObject(v10);
    }
    SetGlobal(25, 0);
    v6 = 131072;
    while ((v6 >= 0)) {
        SetObjectScale(v1, v6, v6);
        SetObjectScale(v7, v6, v6);
        Delay(1);
        v6 = (v6 - 10240);
    }
    SetObjectFadeOut(v1, 2, 1);
    SetObjectFadeOut(v7, 2, 1);
    return;
}
void BlackHole(int a0)  {
    int v1;
    int v2;
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
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL2);
    Delay(40);
    SetOverwhelming(intvAttackerMajor, 0);
    if (!(a0 == 0)) {
        PlaySound1("m020snd02", 210);
    } else {
        PlaySound1("m020snd01", 180);
    }
    CreateBlackHole(intvAttackerMajor, intvDefenderMajor, a0);
    Delay(40);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void FireRingCallback(int a0, int a1) callsign 20001  {
    int v1;
    int v2;
    if (!(a1 == intvDefenderMajor)) {
        Hurt(a0, a1, 0);
        asynccall FireMan(a1, 10015, 60);
    } else {
        v2 = GetObjectContext(a0, 0);
        if (GetGlobal(26 + v2)) {
            v1 = (GetGlobal(30) / GetGlobal(26 + v2));
        }
        Hurt(a0, a1, v1);
        if (v1 > 0) {
            SetGlobal(26 + v2, (GetGlobal(26 + v2) * 2));
            asynccall HitGeneral(a0, a1, 11002, 1, 48, 60);
        }
    }
    return;
}
void CreateFire(int a0)  {
    int v1;
    v1 = CreateObjectByReference(a0, 31001, 0, 0);
    SetObjectScale(v1, 65536, 81920);
    asynccall StepShow(v1, 8);
    while (IsObjectExist(a0)) {
        Delay(1);
        SetCoordinateByReference_Cylind(v1, a0, 192, 8, 0);
    }
    SetObjectFadeOut(v1, 8, 1);
    return;
}
void FireRingMotion2(int a0, int a1, int a2, int a3, int a4)  {
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
    while (IsObjectExist(a0)) {
        SetCoordinateByReference_Cylind(a0, a1, a2, v4, a4);
        Delay(1);
        if (!(a4 >= 80)) {
            if (a4 <= 50) {
                v3 = 1;
            }
        } else {
            v3 = (-1);
        }
        a4 = (a4 + v3);
        a2 = ((a2 + 2) & 255);
        v2++;
        if ((v2 & 3) == 0) {
            v1++;
        }
        if (!(v4 >= 240)) {
            if (v4 <= a3) {
                v5 = 5;
            }
        } else {
            v5 = (-5);
        }
        v4 = (v4 + v5);
    }
    return;
}
void FireRingMotion(int a0, int a1, int a2, int a3, int a4, int a5)  {
    int v1;
    int v2;
    int v3;
    v1 = 0;
    v2 = 0;
    v3 = 1;
    while (IsObjectExist(a0)) {
        SetCoordinateByReference_Cylind(a0, a1, a2, a3, a4);
        Delay(1);
        if (!(a4 >= 80)) {
            if (a4 <= 50) {
                v3 = 1;
            }
        } else {
            v3 = (-1);
        }
        a4 = (a4 + v3);
        a2 = ((a2 + a5) & 255);
        v2++;
        if ((v2 & 3) == 0) {
            v1++;
        }
    }
    return;
}
void CreateFireRing(int a0, int a1, int a2, int a3, int a4, int a5, int a6)  {
    int v1;
    int v2;
    v2 = 0;
    a1 = (a1 & 255);
    v1 = CreateObjectByReference(a0, 31002, 0, 0);
    SetCoordinateByReference_Cylind(v1, a0, a1, a2, a3);
    SetCallbackProcedure(v1, 20001);
    SetObjectFlags(v1, (OF_ENEMYFORCE + OF_ENEMYGENERAL));
    if (!(intvIsLeft == 1)) {
        SetObjectContext(v1, 0, 1);
    } else {
        SetObjectContext(v1, 0, 0);
    }
    asynccall StepShow(v1, 8);
    asynccall CreateFire(v1);
    if (!(a5 == 0)) {
        asynccall FireRingMotion2(v1, a0, a1, a2, a3);
        while (IsObjectExist(a0)) {
            Delay(1);
            v2++;
            if (v2 > 400) {
                break;
            }
        }
        SetObjectFadeOut(v1, 8, 1);
        if (!(a6 == 1)) {
            SetGlobal(28 + 1, 0);
        } else {
            SetGlobal(28 + 0, 0);
        }
    } else {
        asynccall FireRingMotion(v1, a0, a1, a2, a3, a4);
        while (IsObjectExist(a0)) {
            Delay(1);
            v2++;
            if (v2 > 400) {
                break;
            }
        }
        SetObjectFadeOut(v1, 8, 1);
        if (!(a6 == 1)) {
            SetGlobal(28 + 1, 0);
        } else {
            SetGlobal(28 + 0, 0);
        }
    }
    return;
}
void FireRing(int a0, int a1)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    int v6;
    v6 = 0;
    if (!(intvIsLeft == 1)) {
        if (intvIsLeft == 0) {
            if (!(GetGlobal(28 + 1) == 1)) {
                SetGlobal(28 + 1, 1);
                SetGlobal(26 + 1, 1);
            } else {
                v6 = 1;
            }
        }
    } else {
        if (!(GetGlobal(28 + 0) == 1)) {
            SetGlobal(28 + 0, 1);
            SetGlobal(26 + 0, 1);
        } else {
            v6 = 1;
        }
    }
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
    if (!(v6 == 0)) {
        if (!(intvIsLeft == 1)) {
            CastFail(intvAttackerMajor, 128);
        } else {
            CastFail(intvAttackerMajor, 0);
        }
    } else {
        SetObjectAnimate(intvAttackerMajor, OAF_SPELL1);
        PlaySound1("m021snd01", 200);
        Delay(20);
        SetOverwhelming(intvAttackerMajor, 0);
        v5 = 4;
        v4 = (((80 - 50) - 10) / v5);
        v3 = 0;
        while ((v3 < v5)) {
            asynccall CreateFireRing(intvAttackerMajor, ((v3 * 256) / v5), 60, ((50 + 5) + (v3 * v4)), (4 + (a0 * 2)), a1, intvIsLeft);
            v3++;
        }
        if (a0 > 0) {
            if (a1 > 0) {
                Delay(20);
            }
            v5 = 6;
            v4 = (((80 - 50) - 10) / v5);
            v3 = 0;
            while ((v3 < v5)) {
                asynccall CreateFireRing(intvAttackerMajor, (32 + ((v3 * 256) / v5)), 120, ((50 + 5) + (v3 * v4)), 2, a1, intvIsLeft);
                v3++;
            }
        }
    }
    Delay(60);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void IceCallback(int a0, int a1) callsign 22001  {
    if (!(a1 == intvDefenderMajor)) {
        Hurt(a0, a1, 0);
    } else {
        if (!(GetGlobal(31) < 4)) {
            ClearObjectFlags(a0, OF_ENEMYGENERAL);
        } else {
            Hurt(a0, a1, (intvMagicAttackValue / GetGlobal(31)));
            SetGlobal(31, (GetGlobal(31) * 2));
        }
    }
    return;
}
void CreateIceShatter(int a0, int a1, int a2, int a3)  {
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
void CreateFogSpeed(int a0, int a1, int a2, int a3, int a4)  {
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
void CreateIceFog(int a0, int a1, int a2)  {
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
void CreateIce(int a0, int a1, int a2, int a3, int a4, int a5)  {
    int v1;
    int v2;
    Delay(a4);
    v1 = CreateObjectRaw(a0, a1, 0, 128, 32002);
    SetObjectScale(v1, a2, a3);
    if (a5) {
        SetObjectFlags(v1, (OF_ENEMYFORCE + OF_ENEMYGENERAL));
        SetCallbackProcedure(v1, 22001);
    }
    asynccall CreateFogSpeed((a0 + Rand((-2), 2)), (a1 + Rand(4, 12)), Rand(48, 60), Rand(2, 4), Rand(12, 20));
    asynccall CreateFogSpeed(((a0 + 32) + Rand((-2), 2)), (a1 + Rand(4, 12)), Rand(48, 64), Rand(2, 4), Rand(12, 20));
    asynccall CreateFogSpeed(((a0 - 32) + Rand((-2), 2)), (a1 + Rand(4, 12)), Rand(48, 64), Rand(2, 4), Rand(12, 20));
    v2 = 0;
    while ((v2 < 8)) {
        asynccall CreateIceShatter(v1, (((a3 * (v2 + 1)) / 65536) * 20), Rand(26, 36), Rand(2, 5));
        Delay(1);
        MoveObject(v1, 2, 0, 0);
        v2++;
    }
    asynccall LockTargetXY((a0 + 54), a1, 2501, 0, 4, 22001, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY((a0 - 54), a1, 2501, 0, 4, 22001, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY(a0, (a1 + 45), 2501, 0, 4, 22001, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY(a0, (a1 + 45), 2501, 0, 4, 22001, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY((a0 + 54), (a1 + 45), 2501, 0, 4, 22001, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY((a0 + 54), (a1 - 45), 2501, 0, 4, 22001, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY((a0 - 54), (a1 + 45), 2501, 0, 4, 22001, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY((a0 - 54), (a1 - 45), 2501, 0, 4, 22001, 0, 0, OF_ENEMYFORCE);
    Delay(35);
    SetObjectFadeOut(v1, 16, 1);
    return;
}
void Ice(int a0)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    int v6;
    int v7;
    int v8;
    int v9;
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
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL1);
    Delay(20);
    SetOverwhelming(intvAttackerMajor, 0);
    v4 = 0;
    while ((v4 < (a0 + 1))) {
        if (!(v4 == a0)) {
            v3 = GetRandomSoldierHandleFromAlive((intvIsLeft ^ 1));
        } else {
            v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
        }
        if (v3 == 0) {
            v3 = intvDefenderMajor;
        }
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
        if (!Rand(0, 1)) {
            v6 = (v2 - Rand(180, 230));
        } else {
            v6 = (v2 + Rand(180, 240));
        }
        v8 = ScreenXToBattleX(v5);
        v9 = ScreenXToBattleX(v6);
        v7 = GetSoldierCountInRect((intvIsLeft ^ 1), v8, v9, 5, 4);
        if (v7 > 0) {
            v3 = GetNthSoldierInRect((intvIsLeft ^ 1), v8, v9, 5, 4, Rand(0, (v7 - 1)));
            if (v3 != intvDefenderMajor) {
                v5 = GetObjectScreenX(v3);
                v6 = GetObjectScreenY(v3);
            }
        }
        PlaySound1("m022snd01", 200);
        asynccall CreateIce(v5, v6, 65536, 65536, 0, 1);
        asynccall CreateIce((v5 + 44), (v6 + 12), 49152, 45056, Rand(3, 4), 0);
        asynccall CreateIce((v5 - 44), (v6 + 10), 53248, 49152, Rand(4, 5), 0);
        asynccall CreateIce((v5 + Rand((-4), 4)), (v6 - 16), 49152, 36864, Rand(6, 7), 0);
        Delay(Rand(4, 12));
        v5 = (v1 - Rand(170, 260));
        if (!Rand(0, 1)) {
            v6 = (v2 - Rand(190, 240));
        } else {
            v6 = (v2 + Rand(190, 230));
        }
        v8 = ScreenXToBattleX(v5);
        v9 = ScreenXToBattleX(v6);
        v7 = GetSoldierCountInRect((intvIsLeft ^ 1), v8, v9, 5, 4);
        if (v7 > 0) {
            v3 = GetNthSoldierInRect((intvIsLeft ^ 1), v8, v9, 5, 4, Rand(0, (v7 - 1)));
            if (v3 != intvDefenderMajor) {
                v5 = GetObjectScreenX(v3);
                v6 = GetObjectScreenY(v3);
            }
        }
        PlaySound1("m022snd01", 220);
        asynccall CreateIce(v5, v6, 65536, 65536, 0, 1);
        asynccall CreateIce((v5 + 44), (v6 + 12), 49152, 45056, Rand(3, 4), 0);
        asynccall CreateIce((v5 - 44), (v6 + 10), 53248, 49152, Rand(4, 5), 0);
        asynccall CreateIce((v5 + Rand((-4), 4)), (v6 - 16), 49152, 36864, Rand(6, 7), 0);
        Delay(30);
        v4++;
    }
    Wait("CreateIce");
    Delay(60);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void OnFireCallback(int a0, int a1) callsign 23001  {
    int v1;
    int v2;
    if (!((a1 == intvDefenderMajor) || (a1 == intvAttackerMajor))) {
        asynccall FireMan(a1, 10015, 60);
        Hurt(a0, a1, 0);
    } else {
        v2 = GetObjectContext(a0, 0);
        if (GetGlobal(32 + v2)) {
            v1 = (GetGlobal(34) / GetGlobal(32 + v2));
        }
        Hurt(a0, a1, v1);
        if (v1 > 0) {
            SetGlobal(32 + v2, (GetGlobal(32 + v2) * 2));
            asynccall LockTargetTime2(a1, 10015, 60);
        }
    }
    return;
}
void CreateOnFireSmoke(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7)  {
    int v1;
    v1 = CreateObjectRaw(a0, a1, a2, 0, 33002);
    if (!(a7 == 0)) {
        SetObjectScale(v1, 53248, 53248);
    } else {
        SetObjectScale(v1, 8192, 16384);
    }
    SetObjectSpeed_Sphere(v1, 0, a5, a4);
    Delay(2);
    SetObjectScaleShrink(v1, (-a6));
    if (!(a7 == 0)) {
        Delay(4);
    } else {
        Delay(12);
    }
    SetObjectFadeOut(v1, (a3 - 12), 1);
    return;
}
void CreateOnFireFire(int a0, int a1, int a2)  {
    int v1;
    int v2;
    v1 = CreateObjectRaw(a0, a1, 0, (Rand(0, 1) * 128), 33001);
    SetObjectFlags(v1, OF_ATTACKALL);
    SetCallbackProcedure(v1, 23001);
    if (!(intvIsLeft == 1)) {
        SetObjectContext(v1, 0, 1);
    } else {
        SetObjectContext(v1, 0, 0);
    }
    v2 = 4096;
    while ((v2 <= 81920)) {
        SetObjectScale(v1, v2, v2);
        Delay(2);
        v2 = (v2 + 4096);
    }
    Delay(150);
    PlaySound1("m023snd01", 128);
    Delay(250);
    SetObjectFadeOut(v1, 16, 1);
    SetObjectFadeOut(a2, 16, 1);
    return;
}
void CreateOnFire(int a0, int a1)  {
    int v1;
    int v3;
    int v4;
    int v5;
    PlaySound1("m023snd01", 224);
    v1 = 0;
    while ((v1 < 80)) {
        asynccall CreateOnFireSmoke((a0 + Rand((-4), 4)), (a1 + Rand((-4), 4)), 0, Rand(64, 80), Rand(1, 3), Rand(50, 60), Rand(512, 768), 0);
        Delay(Rand(1, 4));
        v1++;
    }
    PlaySound1("m023snd02", 224);
    v3 = CreateObjectRaw(0, 0, 0, 0, 2501);
    asynccall CreateOnFireFire(a0, a1, v3);
    v1 = 0;
    while (IsObjectExist(v3)) {
        if ((v1 % 12) == 0) {
            v4 = Rand((-24), 24);
            if (!(v4 > 0)) {
                v5 = (((5 * v4) / 2) + 80);
            } else {
                v5 = ((((-5) * v4) / 2) + 80);
            }
            asynccall CreateOnFireSmoke((a0 + v4), (a1 + Rand((-4), 4)), v5, Rand(48, 60), Rand(1, 2), Rand(54, 74), Rand(768, 1024), 1);
        }
        Delay(1);
        v1++;
    }
    return;
}
void TraceOnFire(int a0)  {
    Wait("CreateOnFire");
    if (!(a0 == 1)) {
        SetGlobal(35 + 1, 0);
    } else {
        SetGlobal(35 + 0, 0);
    }
    return;
}
void OnFire(int a0)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    int v6;
    int v7;
    int v8;
    int v9;
    v3 = intvAttackerMajor;
    v4 = intvDefenderMajor;
    v5 = intvIsLeft;
    v9 = 0;
    if (!(intvIsLeft == 1)) {
        if (intvIsLeft == 0) {
            if (!(GetGlobal(35 + 1) == 1)) {
                SetGlobal(35 + 1, 1);
                SetGlobal(32 + 1, 1);
            } else {
                v9 = 1;
            }
        }
    } else {
        if (!(GetGlobal(35 + 0) == 1)) {
            SetGlobal(35 + 0, 1);
            SetGlobal(32 + 0, 1);
        } else {
            v9 = 1;
        }
    }
    SetGlobal(34, intvMagicAttackValue);
    BatchLoadShape("magic\\000\\*");
    BatchLoadShape("magic\\023\\*");
    switch (a0) {
    case 0:
        v6 = 5;
        break;
    case 1:
        v6 = 9;
        break;
        v6 = 13;
        break;
    }
    DisablePlayMagic();
    DownBrightness(12, 5);
    SetOverwhelming(v3, 1);
    v1 = GetObjectScreenX(v3);
    v2 = GetObjectScreenY(v3);
    SetViewCamera(v1, (v2 - 120));
    AddAttackCounter(v3, 2);
    Delay(15);
    if (!(v9 == 0)) {
        if (!(intvIsLeft == 1)) {
            CastFail(intvAttackerMajor, 128);
        } else {
            CastFail(intvAttackerMajor, 0);
        }
    } else {
        SetObjectAnimate(v3, OAF_SPELL1);
        Delay(20);
        SetOverwhelming(v3, 0);
        v8 = 0;
        while ((v8 < v6)) {
            v7 = GetRandomSoldierHandleFromAlive((v5 ^ 1));
            if (!(v7 == 0)) {
                v1 = GetObjectScreenX(v7);
                v2 = GetObjectScreenY(v7);
            } else {
                v1 = GetObjectScreenX(v4);
                v2 = GetObjectScreenY(v4);
                if (!Rand(0, 1)) {
                    v1 = (v1 - Rand(40, 400));
                } else {
                    v1 = (v1 + Rand(40, 400));
                }
                if (!Rand(0, 1)) {
                    v2 = (v2 - Rand(30, 300));
                } else {
                    v2 = (v2 + Rand(30, 300));
                }
            }
            if (v8 == 0) {
                MoveCamera(v1, 200, 30);
            }
            asynccall CreateOnFire(v1, v2);
            Delay(Rand(0, 2));
            v8++;
        }
        asynccall TraceOnFire(intvIsLeft);
    }
    Delay(60);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void ProduceBackSoldier(int a0, int a1)  {
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
    do {
        v2 = (v7 / 2);
        do {
            if (!(a0 == 1)) {
                v3 = GetForceHandleByXY(v1, v2);
            } else {
                v3 = GetForceHandleByXY(((v6 - 1) - v1), v2);
            }
            if (v3 == 0) {
                if (!(a0 == 1)) {
                    v5 = CreateSoldier(a0, v1, v2);
                    asynccall StepShow(v5, 10);
                } else {
                    v5 = CreateSoldier(a0, ((v6 - 1) - v1), v2);
                    asynccall StepShow(v5, 10);
                }
                v4++;
                if (v4 >= a1) {
                    return;
                }
            }
            if (!(intvIsLeft == 1)) {
                v3 = GetForceHandleByXY(v1, ((v7 - 1) - v2));
            } else {
                v3 = GetForceHandleByXY(((v6 - 1) - v1), ((v7 - 1) - v2));
            }
            if (v3 == 0) {
                if (!(a0 == 1)) {
                    v5 = CreateSoldier(a0, v1, ((v7 - 1) - v2));
                    asynccall StepShow(v5, 10);
                } else {
                    v5 = CreateSoldier(a0, ((v6 - 1) - v1), ((v7 - 1) - v2));
                    asynccall StepShow(v5, 10);
                }
                v4++;
                if (v4 >= a1) {
                    return;
                }
            }
            v2--;
        } while (v2 > 0);
        v1++;
    } while (v1 <= v6);
    return;
}
void BackSoldier(int a0, int a1)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    int v6;
    int v7;
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
    if (!(v3 == 1)) {
        v2 = 128;
    } else {
        v2 = 0;
    }
    v6 = GetMajorLevel(v3);
    if (v6 > 40) {
        v6 = 40;
    }
    v7 = ((v6 * 5) - GetSoldierCount(v3));
    if (v7 < 0) {
        v7 = 0;
    }
    if (v7 > a0) {
        v7 = a0;
    }
    if (!(v7 != 0)) {
        CastFail(v1, v2);
    } else {
        asynccall ProduceBackSoldier(v3, v7);
        SetObjectAnimate(v1, OAF_SPELL1);
        Delay(24);
        SetObjectAnimate(v1, OAF_SPELL2);
        SetOverwhelming(v1, 0);
        Delay(6);
        PlaySound1("m002snd01", 255);
        PlaySound1("m002snd03", 255);
        if (!(intvIsLeft == 1)) {
            asynccall MoveCamera(240, 0, 40);
        } else {
            asynccall MoveCamera(6576, 0, 40);
        }
        Wait("MoveCamera");
        Delay(100);
        v4 = GetObjectScreenX(intvAttackerMajor);
        v5 = GetObjectScreenY(intvAttackerMajor);
        MoveCamera(v4, (v5 - 120), 20);
    }
    Delay(8);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void TaiChiCallback(int a0, int a1) callsign 25001  {
    if (!(a1 == intvDefenderMajor)) {
        Hurt(a0, a1, 0);
    } else {
        if (!(GetGlobal(37) < 4)) {
            ClearObjectFlags(a0, OF_ENEMYGENERAL);
        } else {
            Hurt(a0, a1, (intvMagicAttackValue / GetGlobal(37)));
            SetGlobal(37, (GetGlobal(37) * 2));
        }
    }
    return;
}
void CreateTaiChiBomb(int a0, int a1)  {
    int v1;
    int v2;
    int v3;
    Delay(8);
    v1 = CreateObjectByReference(a0, 35002, 0, 0);
    SetObjectScale(v1, 4096, 4096);
    if (a1) {
        SetCallbackProcedure(v1, 25001);
        SetObjectFlags(v1, (OF_ENEMYFORCE + OF_ENEMYGENERAL));
    }
    SetObjectScaleShrink(v1, (-4096));
    Delay(20);
    v2 = GetObjectScreenX(v1);
    v3 = GetObjectScreenY(v1);
    asynccall LockTargetXY((v2 + 64), v3, 2501, 0, 20, 25001, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY((v2 - 64), v3, 2501, 0, 20, 25001, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY(v2, (v3 + 50), 2501, 0, 20, 25001, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY(v2, (v3 - 50), 2501, 0, 20, 25001, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY((v2 + 64), (v3 + 50), 2501, 0, 20, 25001, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY((v2 + 64), (v3 - 50), 2501, 0, 20, 25001, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY((v2 - 64), (v3 + 50), 2501, 0, 20, 25001, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY((v2 - 64), (v3 - 50), 2501, 0, 20, 25001, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY((v2 + 128), v3, 2501, 0, 20, 25001, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY((v2 - 128), v3, 2501, 0, 20, 25001, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY(v2, (v3 + 100), 2501, 0, 20, 25001, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY(v2, (v3 - 100), 2501, 0, 20, 25001, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY((v2 + 128), (v3 + 100), 2501, 0, 20, 25001, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY((v2 + 128), (v3 - 100), 2501, 0, 20, 25001, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY((v2 - 128), (v3 + 100), 2501, 0, 20, 25001, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY((v2 - 128), (v3 - 100), 2501, 0, 20, 25001, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY(v2, (v3 + 150), 2501, 0, 20, 25001, 0, 0, OF_ENEMYFORCE);
    Delay(20);
    SetObjectFadeOut(v1, 16, 1);
    return;
}
void CreateTaichi(int a0, int a1)  {
    int v1;
    int v2;
    v1 = CreateObjectRaw(a0, a1, 0, 0, 35001);
    SetObjectLayer(v1, 2);
    SetObjectScale(v1, 98304, 98304);
    v2 = 0;
    while ((v2 <= 16)) {
        SetObjectOpacity(v1, v2);
        Delay(1);
        v2++;
    }
    PlaySound1("m025snd01", 220);
    v2 = 0;
    while ((v2 < 3)) {
        asynccall CreateTaiChiBomb(v1, (v2 == 0));
        Delay((10 - v2));
        v2++;
    }
    Delay(30);
    v2 = 16;
    while ((v2 >= 0)) {
        SetObjectOpacity(v1, v2);
        Delay(1);
        v2--;
    }
    FreeObject(v1);
    return;
}
void TaiChi(int a0)  {
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
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL1);
    Delay(20);
    SetOverwhelming(intvAttackerMajor, 0);
    v4 = 0;
    while ((v4 < (a0 + 1))) {
        v5 = 0;
        while ((v5 < 3)) {
            if (!(v5 == 0)) {
                if (!(v5 == 1)) {
                    v1 = (v6 - Rand(200, 260));
                    if (!Rand(0, 1)) {
                        v2 = (v7 - Rand(220, 270));
                    } else {
                        v2 = (v7 + Rand(220, 270));
                    }
                    v9 = ScreenXToBattleX(v1);
                    v10 = ScreenXToBattleX(v2);
                    v8 = GetSoldierCountInRect((intvIsLeft ^ 1), v9, v10, 3, 3);
                    if (v8 > 0) {
                        v12 = 0;
                        while ((v12 < (v8 / 2))) {
                            v11 = GetNthSoldierInRect((intvIsLeft ^ 1), v9, v10, 3, 3, Rand(0, (v8 - 1)));
                            if (v11 != intvDefenderMajor) {
                                v13 = GetObjectScreenX(v11);
                                v14 = GetObjectScreenY(v11);
                                v15 = (((v13 - v6) * (v13 - v6)) + ((v14 - v7) * (v14 - v7)));
                                if (v15 >= 72200) {
                                    v1 = v13;
                                    v2 = v14;
                                    break;
                                }
                            }
                            v12++;
                        }
                    }
                } else {
                    v1 = (v6 + Rand(200, 260));
                    if (!Rand(0, 1)) {
                        v2 = (v7 - Rand(220, 270));
                    } else {
                        v2 = (v7 + Rand(220, 270));
                    }
                    v9 = ScreenXToBattleX(v1);
                    v10 = ScreenXToBattleX(v2);
                    v8 = GetSoldierCountInRect((intvIsLeft ^ 1), v9, v10, 3, 3);
                    if (v8 > 0) {
                        v12 = 0;
                        while ((v12 < (v8 / 2))) {
                            v11 = GetNthSoldierInRect((intvIsLeft ^ 1), v9, v10, 3, 3, Rand(0, (v8 - 1)));
                            if (v11 != intvDefenderMajor) {
                                v13 = GetObjectScreenX(v11);
                                v14 = GetObjectScreenY(v11);
                                v15 = (((v13 - v6) * (v13 - v6)) + ((v14 - v7) * (v14 - v7)));
                                if (v15 >= 72200) {
                                    v1 = v13;
                                    v2 = v14;
                                    break;
                                }
                            }
                            v12++;
                        }
                    }
                }
            } else {
                v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
                if (v3 == 0) {
                    v3 = intvDefenderMajor;
                }
                v6 = GetObjectScreenX(v3);
                v7 = GetObjectScreenY(v3);
                v1 = v6;
                v2 = v7;
                MoveCamera(v6, (v7 - 300), 40);
            }
            asynccall CreateTaichi(v1, v2);
            Delay(Rand(8, 12));
            v5++;
        }
        Delay(45);
        v4++;
    }
    Wait("CreateTaichi");
    Delay(60);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void SpoutCallback(int a0) callsign 26001  {
    int v1;
    int v2;
    int v3;
    if (!(a0 == intvDefenderMajor)) {
        v2 = GetObjectScreenX(a0);
        v3 = (GetObjectScreenY(a0) - 16);
        KillSoldier(a0);
        SetObjectFlags(a0, OF_NOGRAVITY);
        SetObjectSpeed_Cylind(a0, 0, 8);
        do {
            v1 = GetObjectScreenZ(a0);
            SetObjectCoordinate(a0, (v2 + Rand((-12), 12)), (v3 + Rand((-9), 2)), (v1 + Rand((-3), 3)));
            Delay(1);
            if (v1 > 240) {
                break;
            }
            if (!IsObjectExist(a0)) {
                break;
            }
        } while (1);
        FreeObject(a0);
        return;
    } else {
        if (GetGlobal(38) < 4) {
            Hurt(0, a0, (intvMagicAttackValue / GetGlobal(38)));
            SetGlobal(38, (GetGlobal(38) * 2));
        }
        return;
    }
}
void SpoutKillEdge(int a0, int a1)  {
    int v1[4];
    int v5;
    a0 = ScreenXToBattleX(a0);
    a1 = ScreenXToBattleX(a1);
    v1[0] = GetForceHandleByXY((a0 + 1), a1);
    v1[1] = GetForceHandleByXY(a0, (a1 + 1));
    v1[2] = GetForceHandleByXY((a0 - 1), a1);
    v1[3] = GetForceHandleByXY(a0, (a1 - 1));
    v5 = 0;
    while ((v5 < 4)) {
        if (v1[v5]) {
            if (!((v1[v5] == intvDefenderMajor) || (v1[v5] == intvAttackerMajor))) {
                if (GetSoldierSide(v1[v5]) == intvIsLeft) {
                    v1[v5] = 0;
                }
            } else {
                v1[v5] = 0;
            }
        }
        v5++;
    }
    v5 = 0;
    while ((v5 < 4)) {
        if (v1[v5] != 0) {
            SetObjectSpeed_Sphere(v1[v5], (v5 * 64), Rand(10, 36), 8);
        }
        v5++;
    }
    Delay(2);
    v5 = 0;
    while ((v5 < 4)) {
        if (v1[v5] != 0) {
            KillSoldier(v1[v5]);
        }
        v5++;
    }
    return;
}
void TraceStoneBomb(int a0)  {
    Delay(4);
    SetObjectGravity(a0, 16384);
    Delay(6);
    SetObjectFadeOut(a0, 16, 1);
    return;
}
void CreateStoneBomb(int a0, int a1)  {
    int v1;
    int v2;
    int v3;
    int v4;
    v2 = 0;
    while ((v2 < 10)) {
        v1 = CreateObjectRaw((a0 + Rand((-8), 8)), (a1 + Rand((-8), 8)), 0, 0, Rand(36002, 36003));
        SetObjectGravity(v1, 10240);
        v3 = Rand(0, 255);
        v4 = Rand(32768, 49152);
        SetObjectScale(v1, v4, v4);
        SetObjectSpeed_Sphere(v1, v3, Rand(32, 50), 6);
        asynccall TraceStoneBomb(v1);
        Delay(Rand(0, 2));
        v2++;
    }
    return;
}
void CreateSpout(int a0, int a1, int a2)  {
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
    if (a2) {
        asynccall MoveCamera(a0, (a1 - 240), 5);
    }
    v2 = 0;
    while ((v2 < 4)) {
        MoveObject(v6, 0, 0, 56);
        v1 = CreateObjectRaw(a0, a1, 0, 0, (36005 + (v2 * 2)));
        if (v2 < 3) {
            Delay(1);
            FreeObject(v1);
        }
        v2++;
    }
    v2 = 73728;
    while ((v2 <= 131072)) {
        MoveObject(v6, 0, 0, 28);
        SetObjectScale(v1, 65536, v2);
        Delay(1);
        v2 = (v2 + 8192);
    }
    Delay(24);
    SetObjectFadeOut(v1, 16, 1);
    SetObjectScaleShrink(v1, 4096);
    SetObjectFadeOut(v5, 16, 1);
    SetObjectScaleShrink(v5, 4096);
    SetObjectFadeOut(v6, 16, 1);
    SetObjectScaleShrink(v6, 4096);
    return;
}
void CreateSpoutAround(int a0, int a1, int a2)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    int v6;
    int v7[45];
    int v52;
    PlaySound1("m026snd01", 200);
    v1 = ScreenXToBattleX(a0);
    v2 = ScreenXToBattleX(a1);
    v6 = GetSoldierCountInRect((intvIsLeft ^ 1), v1, v2, 4, 2);
    v5 = 0;
    v4 = 0;
    while ((v4 < v6)) {
        v7[v5] = GetNthSoldierInRect((intvIsLeft ^ 1), v1, v2, 4, 2, v4);
        if (v7[v4] != intvDefenderMajor) {
            v5++;
        }
        v4++;
    }
    v6 = v5;
    v4 = 0;
    while ((v4 < v6)) {
        v52 = Rand(0, (v6 - 1));
        v3 = v7[v4];
        v7[v4] = v7[v52];
        v7[v52] = v3;
        v4++;
    }
    v4 = 0;
    while ((v4 < a2)) {
        Delay(Rand(4, 8));
        if (!(v6 > 0)) {
            v1 = (a0 + Rand((-432), 432));
            v2 = (a1 + Rand((-187), 187));
            asynccall CreateSpout(v1, v2, 0);
        } else {
            v1 = GetObjectScreenX(v7[v4]);
            v2 = GetObjectScreenY(v7[v4]);
            asynccall SpoutKillEdge(v1, v2);
            asynccall SpoutCallback(v7[v4]);
            PlaySound1("m026snd01", 230);
            asynccall CreateSpout(v1, v2, 0);
            v6--;
        }
        v4++;
    }
    return;
}
void Spout(int a0)  {
    int v1;
    int v2;
    int v3;
    int v4;
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
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL1);
    Delay(20);
    SetOverwhelming(intvAttackerMajor, 0);
    v3 = 0;
    while ((v3 < 3)) {
        v4 = GetRandomSoldierHandleFromAlive((intvIsLeft ^ 1));
        if (!v4) {
            v1 = (GetObjectScreenX(intvDefenderMajor) + Rand((-200), 200));
            v2 = (GetObjectScreenY(intvDefenderMajor) + Rand((-150), 150));
        } else {
            v1 = GetObjectScreenX(v4);
            v2 = GetObjectScreenY(v4);
            asynccall SpoutKillEdge(v1, v2);
            asynccall SpoutCallback(v4);
        }
        asynccall CreateSpout(v1, v2, 1);
        CreateSpoutAround(v1, v2, (a0 * 2));
        Delay(40);
        v3++;
    }
    Wait("CreateSpout");
    Delay(60);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void FireCowCallback(int a0, int a1) callsign 27001  {
    int v1;
    int v2;
    int v3;
    if (!(a1 == intvDefenderMajor)) {
        Hurt(a0, a1, 0);
        asynccall FireMan(a1, 10015, 60);
        asynccall Blood(a1, 16);
    } else {
        SetObjectFadeOut(a0, 2, 1);
        v3 = GetObjectContext(a0, 1);
        if (GetGlobal(39 + v3)) {
            v2 = (intvMagicAttackValue / GetGlobal(39 + v3));
        }
        Hurt(a0, a1, v2);
        if (v2 > 0) {
            SetGlobal(39 + v3, (GetGlobal(39 + v3) * 2));
            asynccall LockTargetTime2(a1, 10015, 60);
        }
    }
    v1 = GetObjectContext(a0, 0);
    v1--;
    if (!(v1 <= 0)) {
        SetObjectContext(a0, 0, v1);
    } else {
        SetObjectFadeOut(a0, 2, 1);
    }
    return;
}
void AttachFireToCow(int a0, int a1)  {
    int v1;
    int v2;
    v1 = CreateObjectByReference(a0, 37002, a1, 50);
    SetObjectScale(v1, 131072, 131072);
    SetObjectFlags(v1, OF_MIXER);
    SetObjectOpacity(v1, 12);
    v2 = CreateObjectByReference(a0, 37002, a1, 50);
    SetObjectScale(v2, 131072, 131072);
    SetObjectFlags(v2, OF_MIXER);
    SetObjectOpacity(v2, 12);
    while (IsObjectExist(a0)) {
        SetCoordinateByReference_Cylind(v1, a0, a1, 24, 50);
        MoveObject(v1, Rand((-8), 8), 0, Rand((-8), 8));
        SetCoordinateByReference_Cylind(v2, a0, (a1 - 128), 20, 50);
        MoveObject(v2, Rand((-8), 8), 0, Rand((-8), 8));
        Delay(1);
    }
    FreeObject(v1);
    FreeObject(v2);
    return;
}
void CreateFireCow(int a0, int a1, int a2, int a3, int a4)  {
    int v1;
    int v2;
    int v3;
    v1 = CreateObjectRaw((a0 + (8 * Rand((-6), 6))), (a1 + (8 * Rand((-8), 8))), 0, a2, 37001);
    SetObjectSpeed_Sphere(v1, a2, 0, (8 + Rand((-2), 2)));
    SetCallbackProcedure(v1, 27001);
    SetObjectFlags(v1, (OF_ENEMYFORCE + OF_ENEMYGENERAL));
    SetObjectContext(v1, 0, (8 + Rand((-2), 4)));
    if (!(a3 == 1)) {
        SetObjectContext(v1, 1, 1);
    } else {
        SetObjectContext(v1, 1, 0);
    }
    asynccall AttachFireToCow(v1, (a2 + 128));
    if (a4) {
        asynccall LockCameraLine(v1, ((3 - a4) * 80), a3, 280);
    }
    v3 = 1;
    while (IsObjectExist(v1)) {
        v2 = GetObjectScreenX(v1);
        if (!(a3 == 1)) {
            if (v2 <= 0) {
                SetObjectFadeOut(v1, 16, 2);
                break;
            }
        } else {
            if (v2 >= GetBattleWidthInScreenX()) {
                SetObjectFadeOut(v1, 16, 2);
                break;
            }
        }
        Delay(1);
        v3++;
        if ((v3 & 7) == 0) {
            MoveObject(v1, 0, (8 * Rand((-3), 3)), 0);
        }
    }
    return;
}
void FireCow(int a0)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    if (!(intvIsLeft == 1)) {
        SetGlobal(39 + 1, 1);
    } else {
        SetGlobal(39 + 0, 1);
    }
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
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL1);
    Delay(20);
    SetOverwhelming(intvAttackerMajor, 0);
    Delay(10);
    if (!(intvIsLeft == 1)) {
        v4 = 1;
        v3 = 128;
    } else {
        v4 = (-1);
        v3 = 0;
    }
    v5 = GetSoldierMaxX(intvIsLeft);
    if (!(intvIsLeft == 1)) {
        SetViewCamera((v5 + 320), 0);
    } else {
        SetViewCamera((v5 - 320), 0);
    }
    Delay(15);
    PlaySound1("m027snd01", 180);
    PlaySound1("m027snd02", 180);
    if (!(a0 == 0)) {
        if (!(a0 == 1)) {
            asynccall CreateFireCow((v5 + (1200 * v4)), (562 + 400), v3, intvIsLeft, 0);
            asynccall CreateFireCow((v5 + (1200 * v4)), (562 + 300), v3, intvIsLeft, 0);
            asynccall CreateFireCow((v5 + (1200 * v4)), (562 + 200), v3, intvIsLeft, 0);
            asynccall CreateFireCow((v5 + (1200 * v4)), (562 + 100), v3, intvIsLeft, 0);
            asynccall CreateFireCow((v5 + (1200 * v4)), 562, v3, intvIsLeft, (a0 + 1));
            asynccall CreateFireCow((v5 + (1200 * v4)), (562 - 100), v3, intvIsLeft, 0);
            asynccall CreateFireCow((v5 + (1200 * v4)), (562 - 200), v3, intvIsLeft, 0);
            asynccall CreateFireCow((v5 + (1200 * v4)), (562 - 300), v3, intvIsLeft, 0);
            asynccall CreateFireCow((v5 + (1200 * v4)), (562 - 400), v3, intvIsLeft, 0);
        } else {
            asynccall CreateFireCow((v5 + (1200 * v4)), (562 + 350), v3, intvIsLeft, 0);
            asynccall CreateFireCow((v5 + (1200 * v4)), (562 + 250), v3, intvIsLeft, 0);
            asynccall CreateFireCow((v5 + (1200 * v4)), (562 + 150), v3, intvIsLeft, 0);
            asynccall CreateFireCow((v5 + (1200 * v4)), 562, v3, intvIsLeft, (a0 + 1));
            asynccall CreateFireCow((v5 + (1200 * v4)), (562 - 150), v3, intvIsLeft, 0);
            asynccall CreateFireCow((v5 + (1200 * v4)), (562 - 250), v3, intvIsLeft, 0);
            asynccall CreateFireCow((v5 + (1200 * v4)), (562 - 350), v3, intvIsLeft, 0);
        }
    } else {
        asynccall CreateFireCow((v5 + (1200 * v4)), (562 + 75), v3, intvIsLeft, 0);
        asynccall CreateFireCow((v5 + (1200 * v4)), (562 - 75), v3, intvIsLeft, 0);
        asynccall CreateFireCow((v5 + (1200 * v4)), (562 + 225), v3, intvIsLeft, (a0 + 1));
        asynccall CreateFireCow((v5 + (1200 * v4)), (562 - 225), v3, intvIsLeft, 0);
    }
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
void CreateRollStoneBreak(int a0, int a1)  {
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
void RollDownCallback(int a0, int a1) callsign 28001  {
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
    if (!(a1 == intvDefenderMajor)) {
        Hurt(a0, a1, 0);
        if (v2 != 0) {
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
        }
        if (v2 == 0) {
            a0 = GetObjectContext(a0, 0);
        }
        v1 = GetObjectContext(a0, 0);
        v1--;
        if (!(v1 <= 0)) {
            SetObjectContext(a0, 0, v1);
        } else {
            if (GetGlobal(1) == 0) {
                DelayAmbientSound("m028snd02", 255, 10);
            }
            v8 = 0;
            while ((v8 < 3)) {
                asynccall CreateRollStoneBreak(a0, Rand(10, 20));
                asynccall CreateRollStoneBreak(a0, Rand(25, 35));
                asynccall CreateRollStoneBreak(a0, Rand(40, 50));
                asynccall CreateRollStoneBreak(a0, Rand(55, 65));
                v8++;
            }
            SetObjectFlags(a0, 128);
            ClearObjectFlags(a0, OF_ATTACKALL);
            Delay(4);
            FreeObject(a0);
        }
        return;
    } else {
        if (GetGlobal(41)) {
            v9 = (intvMagicAttackValue / GetGlobal(41));
        }
        Hurt(a0, a1, v9);
        if (v9 > 0) {
            SetGlobal(41, (GetGlobal(41) * 2));
        }
        if (v2 == 0) {
            a0 = GetObjectContext(a0, 0);
        }
        if (GetGlobal(1) == 0) {
            DelayAmbientSound("m028snd02", 255, 10);
        }
        v8 = 0;
        while ((v8 < 3)) {
            asynccall CreateRollStoneBreak(a0, Rand(10, 20));
            asynccall CreateRollStoneBreak(a0, Rand(25, 35));
            asynccall CreateRollStoneBreak(a0, Rand(40, 50));
            asynccall CreateRollStoneBreak(a0, Rand(55, 65));
            v8++;
        }
        SetObjectFlags(a0, 128);
        ClearObjectFlags(a0, OF_ATTACKALL);
        Delay(4);
        FreeObject(a0);
        return;
    }
}
void CreateRollStone(int a0, int a1, int a2)  {
    int v1;
    int v2;
    int v3;
    v3 = CreateObjectRaw((a0 + 96), (a1 + 8), 0, 0, 2501);
    SetObjectSpeed_Sphere(v3, 192, 0, a2);
    SetObjectFlags(v3, OF_ATTACKENEMY);
    SetCallbackProcedure(v3, 28001);
    SetObjectContext(v3, 0, 0);
    SetObjectContext(v3, 1, 0);
    SetObjectContext(v3, 2, 0);
    v2 = CreateObjectRaw((a0 - 96), (a1 + 8), 0, 0, 2501);
    SetObjectSpeed_Sphere(v2, 192, 0, a2);
    SetObjectFlags(v2, OF_ATTACKENEMY);
    SetCallbackProcedure(v2, 28001);
    SetObjectContext(v2, 0, 0);
    SetObjectContext(v2, 1, 0);
    SetObjectContext(v2, 2, 0);
    v1 = CreateObjectRaw(a0, a1, 0, 0, 38001);
    SetObjectSpeed_Sphere(v1, 192, 0, a2);
    SetObjectScale(v1, 49152, 49152);
    SetObjectFlags(v1, OF_ATTACKENEMY);
    SetCallbackProcedure(v1, 28001);
    SetObjectContext(v1, 0, 8);
    SetObjectContext(v1, 1, v2);
    SetObjectContext(v1, 2, v3);
    SetObjectContext(v2, 0, v1);
    SetObjectContext(v3, 0, v1);
    while ((GetObjectScreenY(v1) > 0)) {
        Delay(2);
    }
    FreeObject(v2);
    FreeObject(v3);
    SetObjectFadeOut(v1, 16, 1);
    return;
}
void TracingCamera(int a0, int a1, int a2, int a3, int a4)  {
    int v1;
    int v2;
    int v3;
    int v4;
    Delay(a3);
    v1 = CreateObjectRaw(a0, (GetBattleHeightInScreenY() - 100), 0, 0, 2501);
    v2 = CreateObjectRaw(a1, 400, 0, 0, 2501);
    ApproachObjectTowards(v1, v2, a2);
    if (!(a4 == 1)) {
        do {
            v3 = GetObjectScreenX(v1);
            v4 = GetObjectScreenY(v1);
            Delay(2);
            SetViewCamera(v3, (v4 - 240));
        } while (v3 > a1);
    } else {
        do {
            v3 = GetObjectScreenX(v1);
            v4 = GetObjectScreenY(v1);
            Delay(2);
            SetViewCamera(v3, (v4 - 240));
        } while (v3 < a1);
    }
    FreeObject(v1);
    FreeObject(v2);
    return;
}
void RollDown(int a0)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    int v6;
    int v7;
    int v8;
    int v9;
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
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL1);
    Delay(20);
    SetOverwhelming(intvAttackerMajor, 0);
    switch (a0) {
    case 0:
        v8 = 12;
        v9 = 8;
        break;
    case 1:
        v8 = 14;
        v9 = 8;
        break;
        v8 = 16;
        v9 = 8;
        break;
    }
    v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
    if (v3 == 0) {
        v3 = intvDefenderMajor;
    }
    if (!(intvIsLeft == 1)) {
        v4 = ((((GetObjectScreenX(v3) - ((a0 + 1) * 250)) + (Rand((-25), 25) * 4)) + 100) + Rand((-20), 20));
    } else {
        v4 = ((((GetObjectScreenX(v3) + ((a0 + 1) * 250)) + (Rand((-25), 25) * 4)) - 100) + Rand((-20), 20));
    }
    v5 = GetObjectScreenY(v3);
    v7 = (((a0 + 1) * 2) + 1);
    if (!(intvIsLeft == 1)) {
        asynccall TracingCamera((v4 + (v7 * 250)), (v4 + ((a0 + 1) * 250)), v8, 0, intvIsLeft);
    } else {
        asynccall TracingCamera((v4 - (v7 * 250)), (v4 - ((a0 + 1) * 250)), v8, 0, intvIsLeft);
    }
    v6 = 0;
    while ((v6 < v7)) {
        PlaySound1("m028snd01", 255);
        asynccall CreateRollStone(v4, (GetBattleHeightInScreenY() + 200), v9);
        Delay(10);
        if (!(intvIsLeft == 1)) {
            v4 = (v4 + 250);
        } else {
            v4 = (v4 - 250);
        }
        v6++;
    }
    Wait("TracingCamera");
    Wait("CreateRollStone");
    Delay(60);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void PaCallback(int a0, int a1) callsign 29001  {
    int v1;
    if (!(a1 == intvDefenderMajor)) {
        Hurt(a0, a1, 0);
    } else {
        if (GetGlobal(42)) {
            v1 = (intvMagicAttackValue / GetGlobal(42));
        }
        Hurt(a0, a1, v1);
        if (v1 > 0) {
            SetGlobal(42, (GetGlobal(42) * 2));
        }
    }
    return;
}
void CreatePaBomb(int a0, int a1, int a2)  {
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
    while ((v5 <= 10)) {
        Delay(2);
        SetObjectScale(v4, (4096 + ((12288 / 10) * v5)), 65536);
        v5++;
    }
    SetObjectFadeOut(v4, 8, 1);
    v1 = CreateObjectByReference(a0, 39002, 0, 0);
    SetCoordinateByReference_Cylind(v1, a0, a2, a1, 0);
    SetObjectScale(v1, 4096, 4096);
    SetCallbackProcedure(v1, 29001);
    SetObjectFlags(v1, (OF_ENEMYFORCE + OF_ENEMYGENERAL));
    SetObjectScaleShrink(v1, (-4096));
    Delay(10);
    v2 = GetObjectScreenX(v1);
    v3 = GetObjectScreenY(v1);
    asynccall LockTargetXY((v2 + 20), v3, 2501, 0, 8, 29001, 0, 0, (OF_ENEMYFORCE + OF_ENEMYGENERAL));
    asynccall LockTargetXY(v2, (v3 + 20), 2501, 0, 8, 29001, 0, 0, (OF_ENEMYFORCE + OF_ENEMYGENERAL));
    asynccall LockTargetXY((v2 - 20), v3, 2501, 0, 8, 29001, 0, 0, (OF_ENEMYFORCE + OF_ENEMYGENERAL));
    asynccall LockTargetXY(v2, (v3 - 20), 2501, 0, 8, 29001, 0, 0, (OF_ENEMYFORCE + OF_ENEMYGENERAL));
    Delay(10);
    SetObjectFadeOut(v1, 16, 1);
    return;
}
void BigBombCircle(int a0, int a1)  {
    int v1;
    int v2;
    v1 = CreateObjectByReference(a0, a1, 0, 0);
    v2 = 1;
    while ((v2 < 32)) {
        SetObjectScale(v1, (65536 + (6144 * v2)), (65536 + (6144 * v2)));
        Delay(1);
        v2++;
    }
    SetObjectFadeOut(v1, 16, 1);
    return;
}
void BigPaBombHurt(int a0)  {
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
    while ((v2 < 16)) {
        asynccall LockByCenter(a0, 16018, v6, v3, 4, (OF_ENEMYFORCE + OF_ENEMYGENERAL), 29001);
        v6 = ((v6 + 64) & 255);
        if ((v2 % 4) == 3) {
            v3 = (v3 + 40);
            v6 = (v6 + 32);
        }
        v2++;
    }
    return;
}
void CreateBigPaBomb(int a0)  {
    int v1;
    int v2;
    PlaySound1("m029snd02", 255);
    v1 = CreateObjectByReference(a0, 39012, 0, 220);
    SetObjectSpeed_Cylind(v1, 0, (-20));
    do {
        Delay(1);
        v2 = GetObjectScreenZ(v1);
    } while (v2 > 0);
    asynccall BigPaBombHurt(v1);
    SetObjectFadeOut(v1, 8, 1);
    asynccall BigBombCircle(a0, 39013);
    Delay(10);
    asynccall BigBombCircle(a0, 39014);
    Delay(10);
    asynccall BigBombCircle(a0, 39013);
    return;
}
void CreateRotateLight(int a0, int a1, int a2, int a3)  {
    int v1;
    int v2;
    int v3;
    v1 = CreateObjectByReference(a0, 39016, 0, 0);
    SetCoordinateByReference_Cylind(v1, a0, a1, a2, 0);
    SetObjectFlags(v1, (OF_ENEMYFORCE + OF_ENEMYGENERAL));
    SetObjectScale(v1, 49152, 32768);
    v3 = CreateObjectByReference(a0, 39017, 0, 0);
    SetCoordinateByReference_Cylind(v3, a0, a1, a2, 0);
    SetObjectScale(v3, 49152, 20480);
    v2 = 0;
    while ((v2 < 128)) {
        a1 = ((a1 + 8) & 255);
        if ((v2 % 4) == 0) {
            a2 = (a2 + 2);
        }
        Delay(1);
        SetCoordinateByReference_Cylind(v1, a0, a1, a2, 0);
        SetCoordinateByReference_Cylind(v3, a0, a1, a2, 0);
        SetCoordinateByReference_Cylind(a3, a0, a1, a2, 0);
        v2++;
    }
    SetObjectFadeOut(v1, 8, 1);
    SetObjectFadeOut(v3, 8, 1);
    return;
}
void CreateRotateBomb(int a0, int a1, int a2)  {
    int v1;
    int v2;
    v1 = CreateObjectByReference(a0, 39015, 0, 220);
    SetCoordinateByReference_Cylind(v1, a0, a1, a2, 220);
    SetObjectScale(v1, 49152, 65536);
    SetObjectSpeed_Cylind(v1, 0, (-20));
    do {
        Delay(1);
        v2 = GetObjectScreenZ(v1);
    } while (v2 > 0);
    CreateRotateLight(a0, a1, a2, v1);
    SetObjectFadeOut(v1, 16, 1);
    return;
}
void CreatePaLight(int a0, int a1, int a2, int a3)  {
    int v1;
    v1 = CreateObjectByReference(a0, a2, 128, 0);
    FreeObject(a3);
    Delay(9);
    FreeObject(v1);
    v1 = CreateObjectByReference(a0, a1, 128, 45);
    SetObjectSpeed_Cylind(v1, 0, 16);
    asynccall ProduceShadowTime(v1, 9999);
    Delay(18);
    FreeObject(v1);
    return;
}
void CreatePa(int a0, int a1)  {
    int v1;
    int v2;
    int v3;
    int v4[7];
    PlaySound1("m029snd01", 255);
    v1 = CreateObjectRaw(a0, a1, 0, 0, 39001);
    SetObjectLayer(v1, 2);
    SetObjectFlags(v1, OF_MIXER);
    v2 = 0;
    while ((v2 <= 16)) {
        SetObjectOpacity(v1, v2);
        Delay(1);
        v2++;
    }
    ClearObjectFlags(v1, OF_MIXER);
    SetObjectFlags(v1, 128);
    v2 = 0;
    while ((v2 < 8)) {
        v4[v2] = CreateObjectByReference(v1, (39003 + v2), 128, 0);
        v2++;
    }
    Delay(10);
    v2 = 0;
    while ((v2 < 4)) {
        asynccall CreatePaLight(v1, (39003 + v2), (39015 + v2), v4[v2]);
        asynccall CreatePaLight(v1, ((39003 + v2) + 4), ((39015 + 4) + v2), v4[(v2 + 4)]);
        Delay(4);
        v2++;
    }
    Wait("CreatePaLight");
    Delay(20);
    CreateBigPaBomb(v1);
    PlaySound1("m029snd03", 255);
    v3 = 0;
    while ((v3 < 6)) {
        v2 = 0;
        while ((v2 < 6)) {
            asynccall CreatePaBomb(v1, (130 + (60 * v3)), (Rand(0, 128) + (40 * v2)));
            v2++;
        }
        Delay(Rand(1, 8));
        v3++;
    }
    Wait("CreatePaBomb");
    Delay(10);
    v2 = 8;
    while ((v2 >= 0)) {
        SetObjectOpacity(v1, v2);
        Delay(2);
        v2--;
    }
    return;
}
void Pa(int a0)  {
    int v1;
    int v2;
    int v3;
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
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL1);
    Delay(20);
    SetOverwhelming(intvAttackerMajor, 0);
    v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
    if (v3 == 0) {
        v3 = intvDefenderMajor;
    }
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
void HalfMoonNewCallback(int a0, int a1) callsign 30001  {
    int v1;
    if (!(a1 == intvDefenderMajor)) {
        asynccall FireMan(a1, 10016, 60);
        Hurt(a0, a1, 0);
    } else {
        FreeObject(a0);
        if (GetGlobal(43)) {
            v1 = (intvMagicAttackValue / GetGlobal(43));
        }
        Hurt(a0, a1, v1);
        if (v1 > 0) {
            SetGlobal(43, (GetGlobal(43) * 2));
        }
        HitGeneral(a0, a1, 40003, 2, 48, 0);
    }
    return;
}
void CheckHalfMoonNew(int a0, int a1)  {
    int v1;
    while (IsObjectExist(a0)) {
        Delay(1);
        v1 = GetObjectScreenX(a0);
        if (!(intvIsLeft == 1)) {
            if (v1 < 100) {
                FreeObject(a0);
                break;
            }
        } else {
            if (v1 > (GetBattleWidthInScreenX() - 100)) {
                FreeObject(a0);
                break;
            }
        }
    }
    FreeObject(a1);
    return;
}
void HalfMoonNewMotion(int a0, int a1, int a2, int a3)  {
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
    if (!(v9 < 0)) {
        if (v9 < 512) {
            v3 = (v1 + 512);
        }
    } else {
        v9 = (-v9);
        if (v9 < 512) {
            v3 = (v1 - 512);
        }
    }
    v12 = ((float)v9);
    v12 = ((v12 * 128) / 4704);
    v10 = ((int)v12);
    v8 = 0;
    while ((v8 < v10)) {
        v6 = (v1 + (((v3 - v1) * v8) / v10));
        if (!(a3 == 0)) {
            v5 = (v4 - CalcSin(((v8 * 128) / v10), a2));
        } else {
            v5 = (v4 + CalcSin(((v8 * 128) / v10), a2));
        }
        SetObjectCoordinate(a0, v6, v5, v7);
        SetCoordinateByReference_Cylind(a1, a0, 0, 0, 0);
        Delay(1);
        v8++;
    }
    if (((float)v10) != v12) {
        SetObjectCoordinate(a0, v3, v4, v7);
        SetCoordinateByReference_Cylind(a1, a0, 0, 0, 0);
    }
    if (IsObjectExist(a0)) {
        SetObjectFadeOut(a0, 16, 1);
        v8 = 128;
        while (IsObjectExist(a0)) {
            v6 = (v1 + (((v3 - v1) * v8) / 128));
            if (!(a3 == 0)) {
                v5 = (v4 - CalcSin(v8, a2));
            } else {
                v5 = (v4 + CalcSin(v8, a2));
            }
            SetObjectCoordinate(a0, v6, v5, v7);
            SetCoordinateByReference_Cylind(a1, a0, 0, 0, 0);
            Delay(1);
            v8++;
        }
    }
    return;
}
int GetGeneralWidth()  {
    int v1;
    v1 = (GetObjectScreenX(intvAttackerMajor) - GetObjectScreenX(intvDefenderMajor));
    if (v1 < 0) {
        v1 = (-v1);
    }
    return v1;
}
void CreateHalfMoonNew(int a0, int a1, int a2, int a3, int a4, int a5)  {
    int v1;
    int v2;
    int v4;
    v1 = CreateObjectByReference(a0, 40001, a1, 0);
    SetCoordinateByReference_Cylind(v1, a0, a2, a3, 0);
    SetObjectFlags(v1, (OF_ENEMYFORCE + OF_ENEMYGENERAL));
    SetCallbackProcedure(v1, 30001);
    SetObjectScale(v1, 49152, 49152);
    v2 = CreateObjectByReference(v1, 40002, a1, 0);
    SetCoordinateByReference_Cylind(v2, v1, 0, 0, 0);
    v4 = GetGeneralWidth();
    asynccall CheckHalfMoonNew(v1, v2);
    if (a4) {
        asynccall LockCameraSimple(v1, (-330));
    }
    if (!(v4 < 1200)) {
        asynccall HalfMoonNewMotion(v1, v2, a3, a5);
    } else {
        ApproachObjectTowards(v1, intvDefenderMajor, 30);
        ApproachObjectTowards(v2, intvDefenderMajor, 30);
    }
    return;
}
void HalfMoonNew(int a0)  {
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
    SetGlobal(43, 2);
    BatchLoadShape("magic\\000\\*");
    BatchLoadShape("magic\\030\\*");
    DisablePlayMagic();
    DownBrightness(12, 5);
    v1 = intvAttackerMajor;
    v2 = intvDefenderMajor;
    v3 = intvIsLeft;
    v11 = 0;
    if (Rand(0, 3) == 3) {
        v11 = Rand(0, 90);
    }
    v6 = 18;
    if (!(v3 == 1)) {
        v5 = 128;
        v12 = 1;
    } else {
        v5 = 0;
        v12 = (-1);
    }
    switch (a0) {
    case 0:
        v9 = (120 - v11);
        break;
    case 1:
        v9 = (210 - v11);
        break;
    case 2:
        v9 = (330 - v11);
        break;
        break;
    }
    SetOverwhelming(v1, 1);
    v7 = GetObjectScreenX(v1);
    v8 = GetObjectScreenY(v1);
    SetViewCamera((v7 - (v12 * 180)), (v8 - v9));
    AddAttackCounter(v1, 2);
    Delay(15);
    SetObjectAnimate(v1, OAF_ATTACK3);
    Delay(20);
    SetOverwhelming(v1, 0);
    PlaySound(v1, "m001snd01", 255);
    if (!a0) {
        asynccall CreateHalfMoonNew(intvAttackerMajor, v5, 64, 0, 1, 0);
        asynccall CreateHalfMoonNew(intvAttackerMajor, v5, 64, 200, 0, 0);
        asynccall CreateHalfMoonNew(intvAttackerMajor, v5, 192, 200, 0, 1);
    } else {
        asynccall CreateHalfMoonNew(intvAttackerMajor, v5, 64, 0, 1, 0);
        asynccall CreateHalfMoonNew(intvAttackerMajor, v5, 64, 200, 0, 0);
        asynccall CreateHalfMoonNew(intvAttackerMajor, v5, 64, 400, 0, 0);
        asynccall CreateHalfMoonNew(intvAttackerMajor, v5, 192, 200, 0, 1);
        asynccall CreateHalfMoonNew(intvAttackerMajor, v5, 192, 400, 0, 1);
    }
    Delay(8);
    Wait("CheckHalfMoonNew");
    Delay(60);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void FirePillarCallback(int a0, int a1) callsign 31001  {
    int v1;
    if (!((a1 == intvDefenderMajor))) {
        Hurt(a0, a1, 0);
        asynccall FireMan(a1, 10015, 60);
    } else {
        if (GetGlobal(44)) {
            v1 = (intvMagicAttackValue / GetGlobal(44));
        }
        Hurt(a0, a1, v1);
        if ((v1 > 0)) {
            SetGlobal(44, (GetGlobal(44) * 2));
            HitGeneral(a0, a1, 11002, 1, 48, 0);
        }
    }
    return;
}
void CreateFirePillarSource(int a0, int a1, int a2)  {
    int v1;
    int v2;
    v1 = CreateObjectByReference(a0, 41001, 0, 8);
    SetCoordinateByReference_Cylind(v1, a0, a2, a1, 8);
    PlaySound1("m031snd03", 255);
    Delay(8);
    v2 = a1;
    while (((v2 > 0))) {
        SetCoordinateByReference_Cylind(v1, a0, a2, v2, 8);
        Delay(1);
        a2 = ((a2 + 1) & 255);
        v2 = (v2 - 4);
    }
    SetObjectFadeOut(v1, 16, 1);
    return;
}
void CreateFirePillarLight(int a0, int a1, int a2, int a3, int a4)  {
    int v1;
    int v2;
    int v3;
    v1 = CreateObjectByReference(a0, 41009, 0, 0);
    SetCoordinateByReference_Cylind(v1, a0, a1, a2, 0);
    SetObjectFlags(v1, (OF_ENEMYFORCE + OF_ENEMYGENERAL));
    SetCallbackProcedure(v1, 31001);
    SetObjectScale(v1, 32768, 24576);
    v3 = CreateObjectByReference(a0, 41010, 0, 0);
    SetCoordinateByReference_Cylind(v3, a0, a1, a2, 0);
    SetObjectScale(v3, 32768, 16384);
    v2 = 0;
    while (((v2 < a4))) {
        a1 = ((a1 + 4) & 255);
        a2 = (a2 + 6);
        Delay(1);
        SetCoordinateByReference_Cylind(v1, a0, a1, a2, 0);
        SetCoordinateByReference_Cylind(v3, a0, a1, a2, 0);
        SetCoordinateByReference_Cylind(a3, a0, a1, a2, 0);
        if ((v2 == (a4 - 16))) {
            SetObjectFadeOut(v1, 16, 1);
            SetObjectFadeOut(v3, 16, 1);
        }
        v2++;
    }
    return;
}
void CreateFirePillarBomb(int a0, int a1, int a2, int a3)  {
    int v1;
    a0 = CreateObjectByReference(a0, 2501, 0, 0);
    v1 = CreateObjectByReference(a0, 41008, 0, 0);
    SetCoordinateByReference_Cylind(v1, a0, a1, a2, 0);
    SetObjectScale(v1, 32768, 65536);
    SetObjectFadeOut(v1, a3, 1);
    CreateFirePillarLight(a0, a1, a2, v1, a3);
    FreeObject(a0);
    return;
}
void CreateFirePillar(int a0, int a1)  {
    int v1;
    int v2;
    int v3;
    int v4;
    v1 = CreateObjectRaw(a0, a1, 0, 0, 2501);
    v3 = Rand(0, 255);
    v2 = 0;
    while (((v2 < 4))) {
        asynccall CreateFirePillarSource(v1, 320, v3);
        v3 = ((v3 + 64) & 255);
        v2++;
    }
    Wait("CreateFirePillarSource");
    Delay(8);
    asynccall LockTargetXY(a0, a1, 2501, 0, 60, 31001, 2, 3, (OF_ENEMYFORCE + OF_ENEMYGENERAL));
    asynccall LockTargetXY((a0 + 60), a1, 2501, 60, 2, 31001, 2, 3, OF_ENEMYFORCE);
    asynccall LockTargetXY((a0 - 60), a1, 2501, 60, 2, 31001, 2, 3, OF_ENEMYFORCE);
    asynccall LockTargetXY(a0, (a1 + 50), 2501, 60, 2, 31001, 2, 3, OF_ENEMYFORCE);
    asynccall LockTargetXY(a0, (a1 + 50), 2501, 60, 2, 31001, 2, 3, OF_ENEMYFORCE);
    asynccall LockTargetXY((a0 + 60), (a1 + 50), 2501, 60, 2, 31001, 2, 3, OF_ENEMYFORCE);
    asynccall LockTargetXY((a0 + 60), (a1 - 50), 2501, 60, 2, 31001, 2, 3, OF_ENEMYFORCE);
    asynccall LockTargetXY((a0 - 60), (a1 + 50), 2501, 60, 2, 31001, 2, 3, OF_ENEMYFORCE);
    asynccall LockTargetXY((a0 - 60), (a1 - 50), 2501, 60, 2, 31001, 2, 3, OF_ENEMYFORCE);
    asynccall LockTargetXY((a0 + 120), a1, 2501, 60, 2, 31001, 2, 3, OF_ENEMYFORCE);
    asynccall LockTargetXY((a0 - 120), a1, 2501, 60, 2, 31001, 2, 3, OF_ENEMYFORCE);
    asynccall LockTargetXY(a0, (a1 + 100), 2501, 60, 2, 31001, 2, 3, OF_ENEMYFORCE);
    asynccall LockTargetXY(a0, (a1 + 100), 2501, 60, 2, 31001, 2, 3, OF_ENEMYFORCE);
    asynccall LockTargetXY((a0 + 120), (a1 + 100), 2501, 60, 2, 31001, 2, 3, OF_ENEMYFORCE);
    asynccall LockTargetXY((a0 + 120), (a1 - 100), 2501, 60, 2, 31001, 2, 3, OF_ENEMYFORCE);
    asynccall LockTargetXY((a0 - 120), (a1 + 100), 2501, 60, 2, 31001, 2, 3, OF_ENEMYFORCE);
    asynccall LockTargetXY((a0 - 120), (a1 - 100), 2501, 60, 2, 31001, 2, 3, OF_ENEMYFORCE);
    PlaySound1("m031snd01", 255);
    v2 = 0;
    while (((v2 < 4))) {
        v4 = CreateObjectByReference(v1, (41002 + v2), 0, (-12));
        Delay(4);
        if ((v2 < 3)) {
            FreeObject(v4);
        }
        v2++;
    }
    v2 = 65536;
    while (((v2 < 131072))) {
        SetObjectScale(v4, 65536, v2);
        Delay(2);
        v2 = (v2 + 8192);
    }
    Delay(40);
    PlaySound1("m031snd02", 255);
    asynccall CreateFirePillarBomb(v4, 32, 10, 68);
    asynccall CreateFirePillarBomb(v4, 96, 10, 68);
    asynccall CreateFirePillarBomb(v4, 160, 10, 68);
    asynccall CreateFirePillarBomb(v4, 224, 10, 68);
    SetObjectScaleShrink(v4, 4096);
    SetObjectFadeOut(v4, 16, 1);
    FreeObject(v1);
    return;
}
void FirePillar(int a0)  {
    int v1;
    int v2;
    int v3;
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
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL1);
    Delay(20);
    SetOverwhelming(intvAttackerMajor, 0);
    v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
    if ((v3 == 0)) {
        v3 = intvDefenderMajor;
    }
    v1 = GetObjectScreenX(v3);
    v2 = GetObjectScreenY(v3);
    MoveCamera(v1, (v2 - 140), 20);
    CreateFirePillar(v1, v2);
    Delay(60);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void SparkCallback(int a0, int a1) callsign 32001  {
    int v1;
    int v2;
    v1 = GetObjectContext(a0, 0);
    if (!((v1 == 4660))) {
        v2 = 2;
    } else {
        v2 = 2;
    }
    Hurt(a0, a1, v2);
    asynccall SmallFireBall3(a0, a1, 20, 0);
    return;
}
void DecreaseHP()  {
    int v1;
    v1 = 1;
    while (((v1 <= (intvMagicAttackValue - 4)))) {
        DoHarmToMajor(intvDefenderMajor, 0, 1);
        Delay(4);
        v1++;
    }
    return;
}
void CreateSparkleCenter(int a0, int a1)  {
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
    while (((v2 < (v3 / 4)))) {
        Delay(6);
        SetObjectCoordinate(v1, (v4 + Rand((-16), 16)), v5, (v6 + Rand((-16), 16)));
        v2++;
    }
    SetObjectCoordinate(v1, v4, v5, v6);
    SetObjectFadeOut(v1, 16, 1);
    return;
}
void CreateSparkle(int a0)  {
    int v1;
    int v2;
    int v4;
    PlaySound1("m032snd01", 255);
    a0 = CreateObjectByReference(a0, 2501, 0, 0);
    v1 = CreateObjectByReference(a0, 42004, 0, 48);
    SetObjectScale(v1, 98304, 98304);
    SetCallbackProcedure(v1, 32001);
    SetObjectFlags(v1, (OF_ENEMYGENERAL + OF_ENEMYFORCE));
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
    while (((v4 < 256))) {
        asynccall LockByCenter(a0, 2501, v4, 52, 2, OF_ENEMYFORCE, 32001);
        v4 = (v4 + 32);
    }
    v4 = 0;
    while (((v4 < 256))) {
        asynccall LockByCenter(a0, 2501, v4, 110, 2, OF_ENEMYFORCE, 32001);
        v4 = (v4 + 32);
    }
    Delay(2);
    SetObjectFadeOut(v1, 16, 1);
    while ((IsObjectExist(v1))) {
        Delay(1);
    }
    Delay(8);
    SetObjectFadeOut(v2, 16, 1);
    FreeObject(a0);
    return;
}
void Sparkle(int a0)  {
    int v1;
    int v2;
    int v3;
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
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL1);
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
void FireWorkCallback(int a0, int a1) callsign 33001  {
    int v1;
    if (!((a1 == intvDefenderMajor))) {
        Hurt(a0, a1, 0);
        asynccall FireMan(a1, 10015, 60);
    } else {
        if (GetGlobal(45)) {
            v1 = (intvMagicAttackValue / GetGlobal(45));
        }
        Hurt(a0, a1, v1);
        if ((v1 > 0)) {
            SetGlobal(45, (GetGlobal(45) * 2));
            HitGeneral(a0, a1, 11002, 1, 48, 0);
        }
    }
    return;
}
void TraceFlash(int a0, int a1, int a2)  {
    int v1;
    int v2;
    int v3;
    do {
        Delay(1);
        v1 = GetObjectScreenZ(a0);
    } while ((v1 > 0));
    if (a1) {
        v2 = CreateObjectByReference(a0, 43003, 0, 0);
        if (a2) {
            SetObjectFlags(v2, (OF_ENEMYGENERAL + OF_ENEMYFORCE));
            SetCallbackProcedure(v2, 33001);
        }
        SetObjectScale(v2, 45056, 45056);
        v3 = CreateObjectByReference(v2, 2501, 0, 0);
        MoveObject(v3, 0, 48, 0);
        SetObjectFlags(v3, (OF_ENEMYGENERAL + OF_ENEMYFORCE));
        SetCallbackProcedure(v3, 33001);
    }
    FreeObject(a0);
    Delay(26);
    FreeObject(v3);
    if (a1) {
        SetObjectFadeOut(v2, 16, 1);
    }
    return;
}
void CreateCannonFlash(int a0, int a1)  {
    int v1;
    int v2;
    int v3;
    v3 = 0;
    while (((v3 < 6))) {
        v2 = 0;
        while (((v2 < 256))) {
            v1 = CreateObjectByReference(a0, 43004, 0, 8);
            SetCoordinateByReference_Cylind(v1, a0, v2, 64, 8);
            SetObjectSpeed_Sphere(v1, v2, 44, 12);
            SetObjectOpacity(v1, (16 - (v3 * 2)));
            SetObjectScaleShrink(v1, (-2048));
            SetObjectGravity(v1, a1);
            asynccall TraceFlash(v1, (v3 == 5), 1);
            v2 = (v2 + 32);
        }
        Delay(1);
        v3++;
    }
    return;
}
void CannonExplode(int a0, int a1)  {
    int v1;
    v1 = CreateObjectRaw(a0, a1, 0, 0, 43003);
    SetObjectFlags(v1, (OF_ENEMYGENERAL + OF_ENEMYFORCE));
    SetCallbackProcedure(v1, 33001);
    Delay(4);
    asynccall CreateCannonFlash(v1, Rand(30720, 36864));
    Delay(26);
    SetObjectFadeOut(v1, 16, 1);
    return;
}
void TraceCannon(int a0)  {
    int v1;
    int v2;
    int v3;
    do {
        Delay(1);
        v3 = GetObjectScreenZ(a0);
    } while ((v3 > 0));
    v1 = GetObjectScreenX(a0);
    v2 = GetObjectScreenY(a0);
    asynccall CannonExplode(v1, v2);
    FreeObject(a0);
    return;
}
void ShootCannon() callsign 33002  {
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
    FreeObject(v1);
    return;
}
void TraceCannonShadow(int a0, int a1, int a2, int a3)  {
    int v1;
    while ((IsObjectExist(a1))) {
        v1 = GetObjectScreenZ(a1);
        if ((v1 <= 0)) {
            break;
        }
        Delay(1);
        if ((a2 < 65536)) {
            a2 = (a2 + 8192);
            SetObjectScale(a0, a2, a2);
        }
        if ((a3 < 16)) {
            a3++;
            SetObjectOpacity(a0, a3);
        }
    }
    FreeObject(a0);
    return;
}
void FlyCannonLB1(int a0, int a1, int a2)  {
    int v1;
    Delay(a2);
    v1 = CreateObjectByReference(intvAttackerMajor, 43006, 0, 0);
    SetObjectCoordinate(v1, (a0 + 260), a1, 260);
    SetObjectSpeed_Sphere(v1, 128, (-32), 32);
    asynccall TraceCannon(v1);
    return;
}
void FlyCannonRB1(int a0, int a1, int a2)  {
    int v1;
    Delay(a2);
    v1 = CreateObjectByReference(intvAttackerMajor, 43006, 128, 0);
    SetObjectCoordinate(v1, (a0 - 260), a1, 260);
    SetObjectSpeed_Sphere(v1, 0, (-32), 32);
    asynccall TraceCannon(v1);
    return;
}
void FlyCannonLB2(int a0, int a1, int a2)  {
    int v1;
    int v2;
    v2 = GetObjectScreenX(a0);
    a1 = GetObjectScreenY(a0);
    Delay(a2);
    v1 = CreateObjectByReference(intvAttackerMajor, 43006, 0, 0);
    if (!((IsObjectExist(a0) == 0))) {
        SetCoordinateByReference_Cylind(v1, a0, 0, 0, 0);
        MoveObject(v1, (260 + 24), 0, 260);
    } else {
        SetObjectCoordinate(v1, ((v2 + 260) + 24), a1, 260);
    }
    SetObjectSpeed_Sphere(v1, 128, (-32), 32);
    asynccall TraceCannon(v1);
    return;
}
void FlyCannonRB2(int a0, int a1, int a2)  {
    int v1;
    int v2;
    v2 = GetObjectScreenX(a0);
    a1 = GetObjectScreenY(a0);
    Delay(a2);
    v1 = CreateObjectByReference(intvAttackerMajor, 43006, 128, 0);
    if (!((IsObjectExist(a0) == 0))) {
        SetCoordinateByReference_Cylind(v1, a0, 0, 0, 0);
        MoveObject(v1, ((-260) - 24), 0, 260);
    } else {
        SetObjectCoordinate(v1, ((v2 - 260) - 24), a1, 260);
    }
    SetObjectSpeed_Sphere(v1, 0, (-32), 32);
    asynccall TraceCannon(v1);
    return;
}
void ProduceCannonEmitter(int a0, int a1, int a2, int a3)  {
    int v1;
    Delay(a2);
    if (!((intvIsLeft == 1))) {
        v1 = CreateObjectByReference(intvAttackerMajor, 43001, a3, 0);
    } else {
        v1 = CreateObjectByReference(intvAttackerMajor, 43005, a3, 0);
    }
    SetObjectCoordinate(v1, a0, a1, 0);
    SetObjectContext(v1, 0, 0);
    return;
}
void FireWork(int a0)  {
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
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL1);
    Delay(20);
    SetOverwhelming(intvAttackerMajor, 0);
    if (!((intvIsLeft == 1))) {
        v3 = 128;
    } else {
        v3 = 0;
    }
    switch (a0) {
        case 0:
        v6 = 2;
        v4 = 4;
        v8 = 3;
            break;
        case 1:
        v6 = 1;
        v4 = 4;
        v8 = 5;
            break;
        v6 = 2;
        v4 = 4;
        v8 = 7;
            break;
    }
    do {
        v7 = Rand(1, 30);
        asynccall ProduceCannonEmitter((((6576 + (96 * 7)) * (intvIsLeft == 0)) + ((240 - (96 * 7)) * (intvIsLeft != 0))), ((180 + ((v4 * 72) * 2)) - (72 * 2)), v7, v3);
        v4 = (v4 - v6);
    } while ((v4 >= 0));
    if ((a0 >= 2)) {
        v6 = 2;
        v4 = 6;
        do {
            v7 = Rand(1, 30);
            asynccall ProduceCannonEmitter(((((6576 + (96 * 7)) + (96 * 2)) * (intvIsLeft == 0)) + (((240 - (96 * 7)) - (96 * 2)) * (intvIsLeft != 0))), ((180 + ((v4 * 72) * 2)) - (72 * 1)), v7, v3);
            v4 = (v4 - v6);
        } while ((v4 > 0));
    }
    if (!((intvIsLeft == 1))) {
        MoveCamera((((6576 + (96 * 4)) - 1) + 96), 0, 40);
    } else {
        MoveCamera(((240 - (96 * 4)) - 96), 0, 40);
    }
    Delay(80);
    v10 = GetSoldierCount((intvIsLeft ^ 1));
    v11 = 0;
    if ((v10 < v8)) {
        v11 = (v8 - v10);
    }
    if (!(((v8 - v11) > 0))) {
        v12 = GetObjectScreenX(intvDefenderMajor);
        v13 = v12;
    } else {
        v5 = 0;
        v4 = (v8 - v11);
        do {
            v9 = GetRandomSoldierHandleFromAlive((intvIsLeft ^ 1));
            v14 = GetObjectScreenX(v9);
            v15 = GetObjectScreenY(v9);
            if (!((v5 == 0))) {
                if ((v12 < v14)) {
                    v12 = v14;
                }
                if ((v13 > v14)) {
                    v13 = v14;
                }
            } else {
                v5 = 1;
                v12 = v14;
                v13 = v14;
            }
            if (!((intvIsLeft == 1))) {
                asynccall FlyCannonLB2(v9, v15, (80 + Rand(1, 30)));
            } else {
                asynccall FlyCannonRB2(v9, v15, (80 + Rand(1, 30)));
            }
            v4--;
        } while ((v4 > 0));
    }
    if ((v12 == v13)) {
        v12 = (v12 + 512);
        v13 = (v13 - 512);
    }
    v9 = GetMostImportantSoldier((intvIsLeft ^ 1));
    if ((v9 == 0)) {
        v9 = intvDefenderMajor;
    }
    v12 = GetObjectScreenX(v9);
    v4 = v11;
    do {
        v14 = (v12 + Rand((-320), 320));
        v15 = Rand(150, 650);
        if (!((intvIsLeft == 1))) {
            asynccall FlyCannonLB1(v14, v15, (80 + Rand(1, 30)));
        } else {
            asynccall FlyCannonRB1(v14, v15, (80 + Rand(1, 30)));
        }
        v4--;
    } while ((v4 > 0));
    v9 = GetMostImportantSoldier((intvIsLeft ^ 1));
    if ((v9 == 0)) {
        v9 = intvDefenderMajor;
    }
    v14 = GetObjectScreenX(v9);
    v15 = GetObjectScreenY(v9);
    MoveCamera(v14, 0, 80);
    Wait("TraceFlash");
    Delay(180);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void PowderCallback(int a0, int a1) callsign 34001  {
    int v1;
    if (!(((a1 == intvDefenderMajor) || (a1 == intvAttackerMajor)))) {
        Hurt(a0, a1, 0);
        asynccall FireMan(a1, 10015, 60);
    } else {
        if (GetGlobal(46)) {
            v1 = (intvMagicAttackValue / GetGlobal(46));
        }
        Hurt(a0, a1, v1);
        if ((v1 > 0)) {
            SetGlobal(46, (GetGlobal(46) * 2));
            HitGeneral(a0, a1, 11002, 1, 48, 0);
        }
    }
    return;
}
void CreatePowerExplode(int a0, int a1, int a2, int a3, int a4, int a5)  {
    int v1;
    v1 = CreateObjectRaw(a0, a1, a2, a3, 44003);
    SetObjectSpeed_Sphere(v1, a3, (2 * Rand(8, 32)), a4);
    SetObjectScale(v1, 32768, 32768);
    SetObjectScaleShrink(v1, (-512));
    Delay(a5);
    SetObjectFadeOut(v1, 16, 1);
    return;
}
void CreatePowerSmoke(int a0, int a1, int a2, int a3, int a4, int a5, int a6)  {
    int v1;
    if (!(a6)) {
        v1 = CreateObjectRaw(a0, a1, a2, 0, 44002);
        SetObjectSpeed_Sphere(v1, a3, (2 * Rand(0, 32)), a4);
    } else {
        v1 = CreateObjectRaw(a0, a1, a2, 0, 44004);
        SetObjectSpeed_Sphere(v1, a3, (2 * Rand(4, 28)), a4);
    }
    Delay((a5 - 4));
    SetObjectScaleShrink(v1, (-1024));
    Delay(4);
    SetObjectFadeOut(v1, 16, 1);
    return;
}
void CreatePowderBreak(int a0, int a1)  {
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
void CreatePower(int a0, int a1, int a2)  {
    int v1;
    int v2;
    v1 = CreateObjectRaw(a0, a1, 0, 0, 44001);
    SetObjectFlags(v1, OF_MIXER);
    v2 = 0;
    while (((v2 <= 16))) {
        SetObjectOpacity(v1, v2);
        Delay(1);
        v2++;
    }
    Delay(a2);
    SetObjectFlags(v1, 128);
    v2 = 0;
    while (((v2 < 10))) {
        asynccall CreatePowderBreak(v1, Rand(1, 3));
        v2++;
    }
    asynccall LockTargetXY(a0, a1, 2501, 0, 2, 34001, 2, 3, OF_ATTACKALL);
    asynccall LockTargetXY((a0 + 60), a1, 2501, 0, 2, 34001, 2, 3, OF_ATTACKALL);
    asynccall LockTargetXY((a0 - 60), a1, 2501, 0, 2, 34001, 2, 3, OF_ATTACKALL);
    asynccall LockTargetXY(a0, (a1 + 48), 2501, 0, 2, 34001, 2, 3, OF_ATTACKALL);
    asynccall LockTargetXY(a0, (a1 + 48), 2501, 0, 2, 34001, 2, 3, OF_ATTACKALL);
    asynccall LockTargetXY((a0 + 60), (a1 + 48), 2501, 0, 2, 34001, 2, 3, OF_ATTACKALL);
    asynccall LockTargetXY((a0 + 60), (a1 - 48), 2501, 0, 2, 34001, 2, 3, OF_ATTACKALL);
    asynccall LockTargetXY((a0 - 60), (a1 + 48), 2501, 0, 2, 34001, 2, 3, OF_ATTACKALL);
    asynccall LockTargetXY((a0 - 60), (a1 - 48), 2501, 0, 2, 34001, 2, 3, OF_ATTACKALL);
    v2 = 0;
    while (((v2 < 12))) {
        asynccall CreatePowerSmoke(a0, a1, Rand(20, 40), Rand(0, 255), Rand(2, 3), 16, 0);
        if (((v2 & 1) == 0)) {
            asynccall CreatePowerSmoke(a0, a1, Rand(20, 40), Rand(0, 255), Rand(2, 3), 18, 1);
        }
        Delay(1);
        if ((v2 < 8)) {
            asynccall CreatePowerExplode(a0, a1, Rand(20, 40), Rand(0, 255), Rand(3, 4), 12);
        }
        Delay(Rand(1, 2));
        v2++;
    }
    FreeObject(v1);
    return;
}
void Powder(int a0)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
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
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL1);
    Delay(20);
    SetOverwhelming(intvAttackerMajor, 0);
    switch (a0) {
        case 0:
        v5 = 2;
            break;
        case 1:
        v5 = 5;
            break;
        v5 = 7;
            break;
    }
    v4 = 0;
    while (((v4 < v5))) {
        v3 = GetRandomSoldierHandleFromAlive((intvIsLeft ^ 1));
        if (!(v3)) {
            v1 = (GetObjectScreenX(intvDefenderMajor) + Rand((-250), 250));
            v2 = (GetObjectScreenY(intvDefenderMajor) + Rand((-200), 200));
        } else {
            v1 = (GetObjectScreenX(v3) + Rand((-8), 8));
            v2 = (GetObjectScreenY(v3) + Rand((-8), 8));
        }
        if ((v4 == 0)) {
            asynccall MoveCamera(v1, (v2 - 140), 40);
        }
        asynccall CreatePower(v1, v2, Rand(40, 150));
        if (!(Rand(0, 1))) {
            v1 = (v1 - Rand(150, 200));
        } else {
            v1 = (v1 + Rand(150, 200));
        }
        if (!(Rand(0, 1))) {
            v2 = (v2 - Rand(120, 170));
        } else {
            v2 = (v2 + Rand(120, 170));
        }
        if ((v2 > 800)) {
            v2 = 800;
        }
        if ((v2 < 200)) {
            v2 = 200;
        }
        asynccall CreatePower(v1, v2, Rand(80, 120));
        v4++;
    }
    Wait("CreatePower");
    Wait("CreatePowerSmoke");
    Delay(60);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void SpearCallback(int a0, int a1) callsign 35001  {
    int v1;
    if (!((a1 == intvDefenderMajor))) {
        Hurt(a0, a1, 0);
    } else {
        if (GetGlobal(47)) {
            v1 = (intvMagicAttackValue / GetGlobal(47));
        }
        Hurt(a0, a1, v1);
        if ((v1 > 0)) {
            SetGlobal(47, (GetGlobal(47) * 2));
        }
    }
    asynccall Blood(a1, 16);
    return;
}
void CreateSpear(int a0, int a1, int a2)  {
    int v1;
    Delay(a2);
    v1 = CreateObjectRaw(a0, a1, 0, 0, 45001);
    SetObjectFlags(v1, (OF_ENEMYFORCE + OF_ENEMYGENERAL));
    SetCallbackProcedure(v1, 35001);
    Delay(20);
    FreeObject(v1);
    v1 = CreateObjectRaw(a0, a1, 0, 0, 45002);
    Delay(8);
    SetObjectFadeOut(v1, 16, 1);
    return;
}
void CreateSpears(int a0, int a1)  {
    int v1;
    int v2;
    int v3;
    int v4;
    v3 = (a0 + (36 * 3));
    v4 = (a1 + (42 * 2));
    v1 = (a0 - (36 * 3));
    while (((v1 <= v3))) {
        v2 = (a1 - (42 * 2));
        while (((v2 <= v4))) {
            asynccall CreateSpear(v1, v2, Rand(0, 2));
            v2 = (v2 + 42);
        }
        v1 = (v1 + 36);
    }
    return;
}
void Spear(int a0)  {
    int v1;
    int v2;
    int v3;
    int v4;
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
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL1);
    Delay(20);
    SetOverwhelming(intvAttackerMajor, 0);
    v4 = 0;
    while (((v4 < (a0 + 1)))) {
        v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
        if (!((v3 == 0))) {
            v1 = GetObjectScreenX(v3);
            v2 = GetObjectScreenY(v3);
        } else {
            v3 = intvDefenderMajor;
            v1 = (GetObjectScreenX(v3) + CalcCos(Rand(0, 255), (10 * Rand(24, 32))));
            v2 = (GetObjectScreenY(v3) + CalcSin(Rand(0, 255), (10 * Rand(20, 30))));
        }
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
    }
    Wait("CreateSpear");
    Delay(60);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void SlashCallback(int a0, int a1) callsign 36001  {
    int v1;
    if (!((a1 == intvDefenderMajor))) {
        Hurt(a0, a1, 0);
        asynccall SmallFireBall3(a0, a1, 8, 0);
    } else {
        if (GetGlobal(48)) {
            v1 = (intvMagicAttackValue / GetGlobal(48));
        }
        Hurt(a0, a1, v1);
        if ((v1 > 0)) {
            SetGlobal(48, (GetGlobal(48) * 2));
            HitGeneral(a0, a1, 13008, 2, 60, 0);
        }
    }
    return;
}
void CreateSlashByTarget(int a0, int a1, int a2, int a3)  {
    int v1;
    int v2;
    int v3;
    v3 = 4096;
    v1 = CreateObjectByReference(a0, (46001 + a2), 128, a1);
    SetCoordinateByReference_Cylind(v1, a0, (192 - (a2 * 16)), Rand(80, 180), a1);
    SetObjectFlags(v1, OF_MIXER);
    v2 = 0;
    while (((v2 <= 32))) {
        if ((v3 < 49152)) {
            SetObjectScale(v1, v3, v3);
        }
        SetObjectOpacity(v1, (v2 / 2));
        Delay(1);
        v3 = (v3 + 2048);
        v2++;
    }
    ClearObjectFlags(v1, OF_MIXER);
    Delay(24);
    PlaySound1("m036snd02", 255);
    SetObjectSpeed_Sphere(v1, (64 - (a2 * 16)), 0, 12);
    SetObjectFlags(v1, OF_ATTACKENEMY);
    SetCallbackProcedure(v1, 36001);
    SetObjectFadeOut(v1, 48, 1);
    Delay(a3);
    ClearObjectFlags(v1, OF_ATTACKENEMY);
    return;
}
void CreateSlash(int a0, int a1, int a2, int a3, int a4, int a5)  {
    int v1;
    int v2;
    int v3;
    v3 = 4096;
    v1 = CreateObjectRaw(a0, a1, a2, 128, (46001 + a3));
    SetObjectFlags(v1, OF_MIXER);
    v2 = 0;
    while (((v2 <= 32))) {
        if ((v3 < 49152)) {
            SetObjectScale(v1, v3, v3);
        }
        SetObjectOpacity(v1, (v2 / 2));
        Delay(1);
        v3 = (v3 + 2048);
        v2++;
    }
    ClearObjectFlags(v1, OF_MIXER);
    Delay(24);
    PlaySound1("m036snd02", 255);
    SetObjectSpeed_Sphere(v1, (64 - (a3 * 16)), 0, 12);
    if (a5) {
        SetObjectFlags(v1, OF_ATTACKENEMY);
        SetCallbackProcedure(v1, 36001);
    }
    SetObjectFadeOut(v1, 48, 1);
    Delay(a4);
    ClearObjectFlags(v1, OF_ATTACKENEMY);
    return;
}
void Slash(int a0)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    int v6;
    int v7;
    int v8;
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
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL1);
    Delay(20);
    SetOverwhelming(intvAttackerMajor, 0);
    SetGlobal(48, 1);
    PlaySound1("m036snd01", 255);
    if (!((a0 == 0))) {
        v5 = 0;
        while (((v5 < 3))) {
            if (!((v5 == 0))) {
                v3 = GetRandomSoldierHandleFromAlive((intvIsLeft ^ 1));
            } else {
                v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
            }
            if ((v3 == 0)) {
                v3 = intvDefenderMajor;
            }
            v1 = GetObjectScreenX(v3);
            v2 = GetObjectScreenY(v3);
            v7 = GetObjectXY(v3);
            v8 = GetObjectBattleY(v3);
            MoveCamera(v1, (v2 - 140), 20);
            asynccall CreateSlashByTarget(v3, 40, Rand(0, 15), 0);
            v4 = 0;
            while (((v4 < 5))) {
                v6 = GetSoldierCountInRect((intvIsLeft ^ 1), v7, v8, 6, 5);
                if (!((v6 > 2))) {
                    asynccall CreateSlash((v1 + Rand((-250), 250)), (v2 + Rand((-180), 180)), 40, Rand(0, 15), 24, 1);
                } else {
                    v3 = GetNthSoldierInRect((intvIsLeft ^ 1), v7, v8, 6, 5, Rand(0, (v6 - 1)));
                    asynccall CreateSlashByTarget(v3, 40, Rand(0, 15), 24);
                }
                v4++;
            }
            v4 = 0;
            while (((v4 < 4))) {
                asynccall CreateSlash((v1 + Rand((-250), 250)), (v2 + Rand((-180), 180)), 40, Rand(0, 15), 12, 0);
                v4++;
            }
            Delay(Rand(56, 80));
            v5++;
        }
    } else {
        v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
        if ((v3 == 0)) {
            v3 = intvDefenderMajor;
        }
        v1 = GetObjectScreenX(v3);
        v2 = GetObjectScreenY(v3);
        v7 = GetObjectXY(v3);
        v8 = GetObjectBattleY(v3);
        MoveCamera(v1, (v2 - 140), 20);
        asynccall CreateSlashByTarget(v3, 40, Rand(0, 15), 0);
        v4 = 0;
        while (((v4 < 4))) {
            v6 = GetSoldierCountInRect((intvIsLeft ^ 1), v7, v8, 6, 5);
            if (!((v6 > 2))) {
                asynccall CreateSlash((v1 + Rand((-250), 250)), (v2 + Rand((-180), 180)), 40, Rand(0, 15), 12, 1);
            } else {
                v3 = GetNthSoldierInRect((intvIsLeft ^ 1), v7, v8, 6, 5, Rand(0, (v6 - 1)));
                asynccall CreateSlashByTarget(v3, 40, Rand(0, 15), 12);
            }
            v4++;
        }
        v4 = 0;
        while (((v4 < 3))) {
            asynccall CreateSlash((v1 + Rand((-250), 250)), (v2 + Rand((-180), 180)), 40, Rand(0, 15), 12, 0);
            v4++;
        }
    }
    Wait("CreateSlash");
    Delay(78);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void WallCallback(int a0, int a1) callsign 37001  {
    int v1;
    if (!((a1 == intvDefenderMajor))) {
        Hurt(a0, a1, 0);
        asynccall FireMan(a1, 10015, 60);
    } else {
        if (GetGlobal(50)) {
            v1 = (intvMagicAttackValue / GetGlobal(50));
        }
        Hurt(a0, a1, v1);
        if ((v1 > 0)) {
            SetGlobal(50, (GetGlobal(50) * 2));
        }
    }
    return;
}
void CreateFireWallAttack(int a0, int a1)  {
    int v1;
    int v2;
    int v3[12];
    int v15;
    v2 = 0;
    v1 = 0;
    while (((v1 < 10))) {
        v3[v1] = 0;
        v1++;
    }
    if (!((a1 == 0))) {
        v15 = 934;
    } else {
        v15 = 862;
    }
    v1 = v15;
    while (((v1 >= 100))) {
        v3[v2] = CreateObjectRaw(a0, v1, 0, 0, 2501);
        SetObjectFlags(v3[v2], (OF_ENEMYFORCE + OF_ENEMYGENERAL));
        SetCallbackProcedure(v3[v2], 37001);
        v2++;
        v1 = (v1 - (72 * 2));
    } while (((GetGlobal(49) == 0))) {
        Delay(1);
    }
    v1 = 0;
    while (((v1 < v2))) {
        FreeObject(v3[v1]);
        v1++;
    }
    return;
}
void CreateFireWall(int a0, int a1, int a2)  {
    int v1;
    int v2;
    int v4;
    v1 = CreateObjectRaw(a0, a1, 0, 0, 2501);
    asynccall CreateFireWallAttack(a0, a2);
    v2 = 0;
    while (((v2 < 4))) {
        v4 = CreateObjectByReference(v1, (47001 + v2), 0, 34);
        SetObjectScale(v4, 32768, 65536);
        Delay(4);
        if ((v2 < 3)) {
            FreeObject(v4);
        }
        v2++;
    }
    v2 = 65536;
    while (((v2 < 131072))) {
        SetObjectScale(v4, 32768, v2);
        Delay(2);
        v2 = (v2 + 8192);
    }
    Delay(280);
    SetGlobal(49, 1);
    SetObjectScaleShrink(v4, 4096);
    SetObjectFadeOut(v4, 16, 1);
    FreeObject(v1);
    return;
}
void Wall(int a0)  {
    int v1;
    int v2;
    int v3;
    int v4;
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
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL1);
    Delay(20);
    SetOverwhelming(intvAttackerMajor, 0);
    v4 = GetSoldierMaxX2(intvIsLeft);
    SetGlobal(49, 0);
    if (!((v4 == (-1)))) {
        if (!((a0 == 0))) {
            PlaySound1("m037snd01", 255);
            if (!((intvIsLeft == 1))) {
                MoveCamera((v4 - ((96 * 5) / 2)), 422, 20);
                Delay(2);
                v3 = 862;
                while (((v3 >= 200))) {
                    asynccall CreateFireWall((v4 - ((96 * 3) / 2)), v3, a0);
                    Delay(Rand(0, 1));
                    asynccall CreateFireWall(((v4 + ((96 * 3) / 2)) - 540), v3, a0);
                    Delay(Rand(0, 1));
                    v3 = (v3 - 120);
                }
            } else {
                MoveCamera((v4 + ((96 * 5) / 2)), 422, 20);
                Delay(2);
                v3 = 900;
                while (((v3 >= 200))) {
                    asynccall CreateFireWall((v4 + ((96 * 3) / 2)), v3, a0);
                    Delay(Rand(0, 1));
                    asynccall CreateFireWall(((v4 + ((96 * 3) / 2)) + 400), v3, a0);
                    Delay(Rand(0, 1));
                    v3 = (v3 - 120);
                }
            }
        } else {
            PlaySound1("m037snd01", 255);
            if (!((intvIsLeft == 1))) {
                MoveCamera((v4 - ((96 * 5) / 2)), 422, 20);
                Delay(2);
                v3 = 862;
                while (((v3 >= 200))) {
                    asynccall CreateFireWall((v4 - ((96 * 3) / 2)), v3, a0);
                    Delay(Rand(0, 1));
                    v3 = (v3 - 120);
                }
            } else {
                MoveCamera((v4 + ((96 * 5) / 2)), 422, 20);
                Delay(2);
                v3 = 900;
                while (((v3 >= 200))) {
                    asynccall CreateFireWall((v4 + ((96 * 3) / 2)), v3, a0);
                    Delay(Rand(0, 1));
                    v3 = (v3 - 120);
                }
            }
        }
    } else {
        if (!((intvIsLeft == 1))) {
            CastFail(intvAttackerMajor, 128);
        } else {
            CastFail(intvAttackerMajor, 0);
        }
    }
    Wait("CreateFireWall");
    v1 = GetObjectScreenX(intvAttackerMajor);
    v2 = GetObjectScreenY(intvAttackerMajor);
    SetViewCamera(v1, (v2 - 140));
    Delay(60);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void TwisterSwordCallback(int a0, int a1) callsign 38001  {
    int v1;
    if (!((a1 == intvDefenderMajor))) {
        Hurt(a0, a1, 0);
        asynccall SmallFireBall3(a0, a1, 8, 0);
    } else {
        if (GetGlobal(51)) {
            v1 = (intvMagicAttackValue / GetGlobal(51));
        }
        Hurt(a0, a1, v1);
        if ((v1 > 0)) {
            SetGlobal(51, (GetGlobal(51) * 2));
            HitGeneral(a0, a1, 13008, 2, 60, 0);
        }
    }
    return;
}
void CreateTwisterSwordAttack(int a0, int a1)  {
    int v1;
    int v2;
    int v3[24];
    int v27;
    v1 = 0;
    while (((v1 < 8))) {
        v2 = 0;
        while (((v2 < 3))) {
            v3[((v1 * 3) + v2)] = CreateObjectByReference(a0, 2501, 0, 0);
            if (!((((v1 & 1) == 0) && (v2 == 2)))) {
                SetObjectFlags(v3[((v1 * 3) + v2)], OF_ENEMYFORCE);
            } else {
                SetObjectFlags(v3[((v1 * 3) + v2)], OF_ATTACKENEMY);
            }
            SetCallbackProcedure(v3[((v1 * 3) + v2)], 38001);
            v2++;
        }
        v1++;
    }
    v27 = 0;
    while (((v27 < 280))) {
        v1 = 0;
        while (((v1 < 8))) {
            v2 = 0;
            while (((v2 < 3))) {
                SetCoordinateByReference_Cylind(v3[((v1 * 3) + v2)], a0, ((32 * v1) + v27), (60 + (v2 * 32)), a1);
                v2++;
            }
            v1++;
        }
        Delay(1);
        v27++;
    }
    v1 = 0;
    while (((v1 < 24))) {
        FreeObject(v3[v1]);
        v1++;
    }
    return;
}
void CreateTwisterSword(int a0, int a1, int a2)  {
    int v1;
    int v2[64];
    int v66;
    int v67;
    int v69;
    v1 = CreateObjectRaw(a0, a1, 0, 0, 2501);
    v66 = 0;
    while (((v66 < 16))) {
        v2[v66] = CreateObjectByReference(v1, (48001 + v66), 128, a2);
        SetCoordinateByReference_Cylind(v2[v66], v1, (64 - (16 * v66)), 0, a2);
        SetObjectScale(v2[v66], 40960, 40960);
        SetObjectFlags(v2[v66], 128);
        SetObjectContext(v2[v66], 0, ((v66 % 4) * 2));
        v66++;
    }
    v67 = 0;
    while (((v67 <= 280))) {
        v66 = 0;
        while (((v66 < 16))) {
            v69 = GetObjectContext(v2[v66], 0);
            if (!((v69 == 0))) {
                if (!((v69 == (-2)))) {
                    v69--;
                } else {
                    SetObjectFlags(v2[v66], 128);
                    v69 = ((3 * 2) - 1);
                }
            } else {
                ClearObjectFlags(v2[v66], 128);
                v69--;
            }
            SetObjectContext(v2[v66], 0, v69);
            if ((v67 <= 90)) {
                SetCoordinateByReference_Cylind(v2[v66], v1, (64 - (16 * v66)), v67, a2);
            }
            v66++;
        }
        if ((v67 == 90)) {
            asynccall CreateTwisterSwordAttack(v1, a2);
        }
        Delay(1);
        v67++;
    }
    v66 = 0;
    while (((v66 < 16))) {
        FreeObject(v2[v66]);
        v66++;
    }
    PlaySound1("m038snd01", 255);
    v66 = 0;
    while (((v66 < 8))) {
        v2[v66] = CreateObjectByReference(v1, (48017 + (v66 * 2)), 128, a2);
        SetCoordinateByReference_Cylind(v2[v66], v1, (64 - (32 * v66)), 90, a2);
        SetObjectScale(v2[v66], 40960, 40960);
        SetObjectSpeed_Sphere(v2[v66], (64 - (32 * v66)), 0, 10);
        if (!(((v66 & 1) == 0))) {
            SetObjectFlags(v2[v66], OF_ENEMYFORCE);
        } else {
            SetObjectFlags(v2[v66], OF_ATTACKENEMY);
        }
        SetCallbackProcedure(v2[v66], 38001);
        v66++;
    }
    v66 = 0;
    while (((v66 < 8))) {
        SetObjectFadeOut(v2[v66], 48, 1);
        v66++;
    }
    FreeObject(v1);
    return;
}
void TwisterSword()  {
    int v1;
    int v2;
    int v3;
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
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL1);
    Delay(20);
    SetOverwhelming(intvAttackerMajor, 0);
    v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
    if ((v3 == 0)) {
        v3 = intvDefenderMajor;
    }
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
void CreateRollTubBreak(int a0)  {
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
void RollTubCallback(int a0, int a1) callsign 39001  {
    int v1;
    if ((GetGlobal(1) == 0)) {
        DelayAmbientSound("m039snd02", 255, 20);
    }
    v1 = 0;
    while (((v1 < 8))) {
        asynccall CreateRollTubBreak(a0);
        v1++;
    }
    SetObjectFlags(a0, 128);
    ClearObjectFlags(a0, OF_ATTACKALL);
    Delay(1);
    if (!(((a1 == intvDefenderMajor) && (GetGlobal(52) == 0)))) {
        Hurt(a0, a1, 0);
    } else {
        SetGlobal(52, 1);
        Hurt(a0, a1, intvMagicAttackValue);
    }
    FreeObject(a0);
    return;
}
void CreateRollTub(int a0, int a1, int a2)  {
    int v1;
    int v2;
    int v3;
    Delay(a2);
    v1 = CreateObjectRaw(a0, a1, 0, 0, 49001);
    SetObjectFlags(v1, (OF_ENEMYFORCE + OF_ENEMYGENERAL));
    SetCallbackProcedure(v1, 39001);
    SetObjectSpeed_Sphere(v1, 192, 0, Rand(4, 6));
    SetObjectFlags(v1, OF_MIXER);
    v3 = 0;
    while (((v3 <= 16))) {
        SetObjectOpacity(v1, v3);
        Delay(2);
        v3++;
    }
    ClearObjectFlags(v1, OF_MIXER);
    do {
        Delay(1);
        if ((IsObjectExist(v1) == 0)) {
            return;
        }
        v2 = GetObjectScreenY(v1);
    } while ((v2 >= 80));
    SetObjectFadeOut(v1, 16, 1);
    return;
}
void RollTub(int a0)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    int v6;
    int v7;
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
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL1);
    Delay(20);
    SetOverwhelming(intvAttackerMajor, 0);
    v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
    if ((v3 == 0)) {
        v3 = intvDefenderMajor;
    }
    v1 = GetObjectScreenX(v3);
    v2 = GetObjectScreenY(v3);
    v7 = (GetBattleHeightInScreenY() + 40);
    v6 = (v1 + (96 * 9));
    v4 = 0;
    while (((v4 < (a0 + 1)))) {
        v5 = (v1 - (9 * 96));
        while (((v5 <= v6))) {
            if (!((v4 == 0))) {
                asynccall CreateRollTub(v5, v7, Rand(2, 3));
            } else {
                asynccall CreateRollTub(v5, v7, Rand(52, 57));
            }
            v5 = (v5 + 96);
        }
        Delay(4);
        if (!((v4 == 0))) {
            if ((v4 == 2)) {
                PlaySound1("m039snd01", 255);
            }
        } else {
            SetViewCamera((v1 - (9 * 96)), (v7 - 200));
            Delay(15);
            MoveCamera(v1, (v2 - 140), 40);
            PlaySound1("m039snd01", 255);
        }
        Delay(Rand(10, 12));
        v4++;
    }
    Wait("CreateRollTub");
    Wait("CreateRollTubBreak");
    Delay(60);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void EarthquakeCallback(int a0, int a1) callsign 40001  {
    return;
}
void CreateQuakeSmoke(int a0, int a1, int a2, int a3, int a4)  {
    int v1;
    v1 = CreateObjectRaw(a0, a1, a2, 0, 50002);
    SetObjectSpeed_Cylind(v1, 0, a3);
    Delay((a4 - 4));
    SetObjectScaleShrink(v1, (-512));
    Delay(4);
    SetObjectFadeOut(v1, 16, 1);
    return;
}
void CreateQuakeStone(int a0, int a1, int a2)  {
    int v1;
    int v2;
    Delay(a2);
    v2 = 0;
    while (((v2 < 4))) {
        asynccall CreateQuakeSmoke((a0 + Rand((-30), 30)), (a1 + Rand((-16), 16)), Rand(0, 10), Rand(2, 4), Rand(8, 12));
        Delay(Rand(2, 8));
        v2++;
    }
    v1 = CreateObjectRaw(a0, a1, 0, (128 * Rand(0, 1)), 50001);
    SetObjectFlags(v1, OF_ENEMYFORCE);
    SetCallbackProcedure(v1, 40001);
    v2 = 0;
    while (((v2 < 70))) {
        if ((Rand(1, 3) == 1)) {
            asynccall CreateQuakeSmoke((a0 + Rand((-30), 30)), (a1 + Rand((-16), 16)), Rand(0, 10), Rand(2, 4), Rand(8, 12));
        }
        Delay(1);
        v2++;
    }
    SetObjectFadeOut(v1, 16, 1);
    return;
}
void ShakeCamera(int a0, int a1, int a2)  {
    int v1;
    v1 = 0;
    while (((v1 < a2))) {
        if (((v1 % 3) == 2)) {
            SetViewCamera((a0 + Rand((-15), 15)), (a1 + Rand((-15), 15)));
        }
        Delay(1);
        if ((GetGlobal(53) == 1)) {
            break;
        }
        v1++;
    }
    return;
}
void QuakeSound()  {
    int v1;
    v1 = 0;
    PlaySound1("m040snd01", 255);
    while (((GetGlobal(53) == 0))) {
        Delay(1);
        v1++;
        if ((v1 > (99 - 20))) {
            PlaySound1("m040snd01", 255);
            v1 = 0;
        }
    }
    return;
}
void Earthquake()  {
    int v1;
    int v2;
    int v3;
    int v4;
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
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL1);
    Delay(20);
    SetOverwhelming(intvAttackerMajor, 0);
    v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
    if ((v3 == 0)) {
        v3 = intvDefenderMajor;
    }
    v1 = GetObjectScreenX(v3);
    v2 = GetObjectScreenY(v3);
    MoveCamera(v1, (v2 - 120), 20);
    Delay(10);
    SetGlobal(53, 0);
    asynccall QuakeSound();
    asynccall ShakeCamera(v1, (v2 - 120), 9999);
    v4 = ((GetSoldierCount((intvIsLeft ^ 1)) / 2) + Rand(0, 10));
    do {
        Delay(Rand(1, 10));
        if (!(((v4 % 3) == 0))) {
            v3 = GetRandomSoldierHandleFromAlive((intvIsLeft ^ 1));
        } else {
            v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
        }
        if (!(((v3 == 0) || (v3 == intvDefenderMajor)))) {
            v1 = GetObjectScreenX(v3);
            v2 = GetObjectScreenY(v3);
        } else {
            v1 = (GetObjectScreenX(intvDefenderMajor) + (10 * Rand((-40), 40)));
            v2 = (GetObjectScreenY(intvDefenderMajor) + (10 * Rand((-30), 30)));
        }
        asynccall CreateQuakeStone(v1, v2, Rand(2, 12));
        v4--;
    } while ((v4 > 0));
    Wait("CreateQuakeStone");
    Delay(28);
    SetGlobal(53, 1);
    Delay(60);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
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
        SetGlobal(55 + v2, 0);
    }
    return;
}
void CreateInterceptor(int a0, int a1, int a2, int a3)  {
    int v1;
    if (!((GetGlobal(55 + a3) != 0))) {
        a0 = BattleXToScreenX(a0);
        a1 = BattleXToScreenX(a1);
        v1 = CreateObjectRaw(a0, a1, 0, a2, 51001);
        SetObjectFlags(v1, (OF_ENEMYFORCE + OF_ENEMYGENERAL));
        SetCallbackProcedure(v1, 41001);
        SetObjectContext(v1, 0, 3);
        SetObjectContext(v1, 1, a3);
        SetGlobal(55 + a3, v1);
        return;
    } else {
        return;
    }
}
void MakeInterceptorSound()  {
    int v1;
    v1 = 0;
    while (((v1 < 5))) {
        PlaySound1("m041snd01", (128 + (v1 * 15)));
        Delay(12);
        v1++;
    }
    return;
}
void Interceptor(int a0)  {
    int v1;
    int v2;
    int v3;
    int v4;
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
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL1);
    PlaySound(intvAttackerMajor, "m041snd02", 255);
    Delay(20);
    SetOverwhelming(intvAttackerMajor, 0);
    v4 = GetBattleWidth();
    if (!((intvIsLeft == 1))) {
        switch (a0) {
        case 0:
            v3 = 0;
            while (((v3 < 15))) {
                CreateInterceptor(1, v3, 0, (30 + v3));
                v3 = (v3 + 2);
            }
            break;
        case 1:
            v3 = 0;
            while (((v3 < 15))) {
                CreateInterceptor(1, v3, 0, (30 + v3));
                v3 = (v3 + 2);
            }
            v3 = 1;
            while (((v3 < 15))) {
                CreateInterceptor(2, v3, 0, ((45 + v3) - 1));
                v3 = (v3 + 2);
            }
            break;
        case 2:
            DisableEscape((intvIsLeft ^ 1));
            v3 = 0;
            while (((v3 < 15))) {
                CreateInterceptor((-1), v3, 0, (30 + v3));
                v3++;
            }
            v3 = 0;
            while (((v3 < 15))) {
                CreateInterceptor((-2), v3, 0, (45 + v3));
                v3++;
            }
            break;
        }
        asynccall MakeInterceptorSound();
        MoveCamera(280, (v2 - 120), 40);
    } else {
        switch (a0) {
        case 0:
            v3 = 0;
            while (((v3 < 15))) {
                CreateInterceptor((v4 - 2), v3, 128, v3);
                v3 = (v3 + 2);
            }
            break;
        case 1:
            v3 = 0;
            while (((v3 < 15))) {
                CreateInterceptor((v4 - 2), v3, 128, v3);
                v3 = (v3 + 2);
            }
            v3 = 1;
            while (((v3 < 15))) {
                CreateInterceptor((v4 - 3), v3, 128, ((15 + v3) - 1));
                v3 = (v3 + 2);
            }
            break;
        case 2:
            DisableEscape((intvIsLeft ^ 1));
            v3 = 0;
            while (((v3 < 15))) {
                CreateInterceptor(v4, v3, 128, v3);
                v3++;
            }
            v3 = 0;
            while (((v3 < 15))) {
                CreateInterceptor((v4 + 1), v3, 128, (15 + v3));
                v3++;
            }
            break;
        }
        asynccall MakeInterceptorSound();
        MoveCamera((GetBattleWidthInScreenX() - 280), (v2 - 120), 40);
    }
    Delay(60);
    v1 = GetObjectScreenX(intvAttackerMajor);
    v2 = GetObjectScreenY(intvAttackerMajor);
    SetViewCamera(v1, (v2 - 120));
    Delay(30);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void XCallDragonCallback(int a0, int a1) callsign 42001  {
    if (!(((a1 == intvDefenderMajor) && (GetGlobal(116) == 0)))) {
        Hurt(a0, a1, 0);
        asynccall FireMan(a1, 10015, 60);
    } else {
        Hurt(a0, a1, 2);
        SetGlobal(116, 1);
        asynccall LockTargetTime2(a1, 10015, 60);
    }
    return;
}
void Xsc3602(int a0, int a1, int a2, int a3)  {
    int v1;
    int v2;
    int v3;
    int v5;
    int v6;
    switch (a0) {
        case 0:
        v5 = 10;
            break;
        case 1:
        v5 = 20;
            break;
        v5 = 20;
            break;
    }
    v1 = 0;
    while (((v1 < v5))) {
        v2 = (a1 + Rand((-48), 48));
        v3 = (a2 + Rand((-16), 16));
        v6 = CreateObjectRaw(v2, v3, (a3 + 20), 0, Rand(28002, 28004));
        SetObjectSpeed_Cylind(v6, 0, Rand(10, 30));
        PlaySound1("m016a", 255);
        Delay(Rand(0, 5));
        v1++;
    }
    Delay(15);
    return;
}
void Xsc3601(int a0, int a1, int a2, int a3, int a4)  {
    int v1;
    int v2;
    Delay(Rand(10, 50));
    v1 = CreateObjectRaw(a2, a3, a4, a1, 28001);
    v2 = 60;
    while (((v2 > 0))) {
        a4++;
        SetObjectCoordinate(v1, a2, a3, a4);
        v2--;
        Delay(1);
    }
    Delay(30);
    asynccall Xsc3602(a0, a2, a3, a4);
    v2 = 0;
    while (((v2 < 10))) {
        SetObjectScale(v1, 0, (65536 + (v2 * 65536)));
        Delay(1);
        v2++;
    } while (((v2 < 30))) {
        a4 = (a4 + (v2 * 10));
        SetObjectScale(v1, 0, (65536 + (v2 * 65536)));
        SetObjectCoordinate(v1, a2, a3, a4);
        Delay(1);
        v2++;
    }
    FreeObject(v1);
    return;
}
void Xsc3603(int a0, int a1, int a2, int a3)  {
    int v1;
    int v2;
    int v3;
    CreateObjectRaw(a1, a2, Rand(320, 360), 0, 28012);
    Delay(Rand(4, 10));
    v1 = (2 + (3 * a0));
    while ( 1 ) {
        v1--;
        if (!((v1 > 0)))
            break;
        v2 = (a1 + Rand((-320), 320));
        v3 = (a2 + Rand((-100), 300));
        CreateObjectRaw(v2, v3, Rand(320, 360), 0, 28012);
        Delay(Rand(4, 10));
    }
    return;
}
void Xsc2512(int a0)  {
    int v1;
    int v2;
    v1 = 16384;
    SetObjectSpeed_Cylind(a0, Rand(4, 12), Rand((-2), 8));
    SetObjectFriction1(a0, 8192);
    v2 = Rand(30, 40);
    while ( 1 ) {
        v2--;
        if (!((v2 > 0)))
            break;
        SetObjectScale(a0, v1, v1);
        v1 = (v1 - 256);
        Delay(1);
    }
    FreeObject(a0);
    return;
}
void Xsc3604() callsign 42002  {
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
    while (((v2 < 20))) {
        v1 = CreateObjectByReference(v3, 10003, Rand(0, 255), 0);
        SetObjectScale(v1, 16384, 16384);
        asynccall Xsc2512(v1);
        v2++;
    }
    PlaySound(v1, "m018snd03", 255);
    return;
}
void XCallDragon(int a0)  {
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
    if (!((intvIsLeft == 1))) {
        v10 = 128;
    } else {
        v10 = 0;
    }
    v1 = GetObjectScreenX(intvAttackerMajor);
    v2 = GetObjectScreenY(intvAttackerMajor);
    v3 = GetObjectScreenZ(intvAttackerMajor);
    PlaySound1("m018snd01", 255);
    switch (a0) {
        case 0:
        asynccall Xsc3602(a0, v1, v2, v3);
        v11 = 50;
        SetGlobal(115, 110);
            break;
        case 1:
        asynccall Xsc3602(a0, v1, v2, v3);
        v11 = 100;
        SetGlobal(115, 220);
            break;
        asynccall Xsc3601(a0, v10, v1, v2, v3);
        v11 = (100 + 95);
        SetGlobal(115, 330);
            break;
    }
    v4 = 0;
    while (((v4 < v11))) {
        v12 = Rand(1, 2);
        v5 = 0;
        while (((v5 < v12))) {
            v7 = (v1 + Rand((-48), 48));
            v8 = (v2 + Rand((-16), 16));
            if (!((a0 == 2))) {
                CreateObjectRaw(v7, v8, v9, 0, Rand(28006, 28007));
            } else {
                CreateObjectRaw((v7 - 48), v8, v9, 0, Rand(28006, 28007));
                CreateObjectRaw((v7 + 48), v8, v9, 0, Rand(28006, 28007));
            }
            v5++;
        }
        if ((Rand(0, 3) == 0)) {
            v7 = (v1 + Rand((-48), 48));
            v8 = (v2 + Rand((-16), 16));
            v13 = CreateObjectRaw(v7, v8, v9, Rand(0, 255), Rand(28008, 28009));
            SetObjectSpeed_Cylind(v13, Rand(10, 16), Rand(10, 16));
        }
        Delay(1);
        switch (v4) {
            case 49:
            PlaySound1("m018snd02", 255);
                break;
            case 99:
            PlaySound1("m018snd01", 255);
                break;
            case 149:
            PlaySound1("m018snd02", 255);
                break;
                break;
        }
        v4++;
    }
    if (!((a0 > 1))) {
        Wait("Xsc3602");
    } else {
        Wait("Xsc3601");
    }
    v13 = GetMostImportantSoldier((intvIsLeft ^ 1));
    if ((v13 == 0)) {
        v13 = intvDefenderMajor;
    }
    v1 = GetObjectScreenX(v13);
    v2 = GetObjectScreenY(v13);
    v3 = GetObjectScreenZ(v13);
    if (!((v2 > 240))) {
        MoveCamera(v1, 0, 20);
    } else {
        MoveCamera(v1, (v2 - 240), 20);
    }
    if ((IsObjectExist(v13) == 0)) {
        v13 = GetMostImportantSoldier((intvIsLeft ^ 1));
        if ((v13 == 0)) {
            v13 = intvDefenderMajor;
        }
    }
    v1 = GetObjectScreenX(v13);
    v2 = GetObjectScreenY(v13);
    v3 = GetObjectScreenZ(v13);
    asynccall Xsc3603(a0, v1, v2, v3);
    Wait("Xsc3603");
    Delay(65);
    return;
}
void XFireCowCallback(int a0, int a1) callsign 42003  {
    int v1;
    int v2;
    if (!((a1 == intvDefenderMajor))) {
        Hurt(a0, a1, 0);
        asynccall FireMan(a1, 10015, 60);
        asynccall Blood(a1, 16);
    } else {
        SetObjectFadeOut(a0, 2, 1);
        if (GetGlobal(117)) {
            v2 = (2 / GetGlobal(117));
        }
        Hurt(a0, a1, v2);
        if ((v2 > 0)) {
            SetGlobal(117, (GetGlobal(117) * 2));
            asynccall LockTargetTime2(a1, 10015, 60);
        }
    }
    v1 = GetObjectContext(a0, 0);
    v1--;
    if (!((v1 <= 0))) {
        SetObjectContext(a0, 0, v1);
    } else {
        SetObjectFadeOut(a0, 2, 1);
    }
    return;
}
void XAttachFireToCow(int a0, int a1)  {
    int v1;
    int v2;
    v1 = CreateObjectByReference(a0, 37002, a1, 50);
    SetObjectScale(v1, 131072, 131072);
    SetObjectFlags(v1, OF_MIXER);
    SetObjectOpacity(v1, 12);
    v2 = CreateObjectByReference(a0, 37002, a1, 50);
    SetObjectScale(v2, 131072, 131072);
    SetObjectFlags(v2, OF_MIXER);
    SetObjectOpacity(v2, 12);
    while ((IsObjectExist(a0))) {
        SetCoordinateByReference_Cylind(v1, a0, a1, 24, 50);
        MoveObject(v1, Rand((-8), 8), 0, Rand((-8), 8));
        SetCoordinateByReference_Cylind(v2, a0, (a1 - 128), 20, 50);
        MoveObject(v2, Rand((-8), 8), 0, Rand((-8), 8));
        Delay(1);
    }
    FreeObject(v1);
    FreeObject(v2);
    return;
}
void XCreateFireCow(int a0, int a1, int a2, int a3, int a4)  {
    int v1;
    int v2;
    int v3;
    v1 = CreateObjectRaw((a0 + (8 * Rand((-6), 6))), (a1 + (8 * Rand((-8), 8))), 0, a2, 37001);
    SetObjectSpeed_Sphere(v1, a2, 0, (8 + Rand((-2), 2)));
    SetCallbackProcedure(v1, 42003);
    SetObjectFlags(v1, (OF_ENEMYFORCE + OF_ENEMYGENERAL));
    SetObjectContext(v1, 0, (8 + Rand((-4), 4)));
    asynccall XAttachFireToCow(v1, (a2 + 128));
    v3 = 1;
    while ((IsObjectExist(v1))) {
        v2 = GetObjectScreenX(v1);
        if (!((a3 == 1))) {
            if ((v2 < 100)) {
                SetObjectFadeOut(v1, 16, 2);
                break;
            }
        } else {
            if ((v2 > (GetBattleWidthInScreenX() - 100))) {
                SetObjectFadeOut(v1, 16, 2);
                break;
            }
        }
        Delay(1);
        v3++;
        if (((v3 & 7) == 0)) {
            MoveObject(v1, 0, (8 * Rand((-3), 3)), 0);
        }
        if ((v3 > 500)) {
            SetObjectFadeOut(v1, 16, 2);
            break;
        }
    }
    return;
}
void XFireCow(int a0)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    SetGlobal(117, 1);
    v1 = GetObjectScreenX(intvAttackerMajor);
    v2 = GetObjectScreenY(intvAttackerMajor);
    if (!((intvIsLeft == 1))) {
        v4 = 1;
        v3 = 128;
    } else {
        v4 = (-1);
        v3 = 0;
    }
    v5 = GetSoldierMaxX(intvIsLeft);
    if ((intvIsLeft == 1)) {
    }
    Delay(15);
    PlaySound1("m027snd01", 180);
    PlaySound1("m027snd02", 180);
    if (!((a0 == 0))) {
        if (!((a0 == 1))) {
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
        } else {
            asynccall XCreateFireCow((v5 + (1200 * v4)), (562 + 350), v3, intvIsLeft, 0);
            asynccall XCreateFireCow((v5 + (1200 * v4)), (562 + 250), v3, intvIsLeft, 0);
            asynccall XCreateFireCow((v5 + (1200 * v4)), (562 + 150), v3, intvIsLeft, 0);
            asynccall XCreateFireCow((v5 + (1200 * v4)), 562, v3, intvIsLeft, (a0 + 1));
            asynccall XCreateFireCow((v5 + (1200 * v4)), (562 - 150), v3, intvIsLeft, 0);
            asynccall XCreateFireCow((v5 + (1200 * v4)), (562 - 250), v3, intvIsLeft, 0);
            asynccall XCreateFireCow((v5 + (1200 * v4)), (562 - 350), v3, intvIsLeft, 0);
        }
    } else {
        asynccall XCreateFireCow((v5 + (1200 * v4)), (562 + 75), v3, intvIsLeft, 0);
        asynccall XCreateFireCow((v5 + (1200 * v4)), (562 - 75), v3, intvIsLeft, 0);
        asynccall XCreateFireCow((v5 + (1200 * v4)), (562 + 225), v3, intvIsLeft, (a0 + 1));
        asynccall XCreateFireCow((v5 + (1200 * v4)), (562 - 225), v3, intvIsLeft, 0);
    }
    Delay(Rand(110, 160));
    PlaySound1("m027snd02", 210);
    Delay(10);
    PlaySound1("m027snd01", 160);
    Wait("XCreateFireCow");
    Delay(60);
    return;
}
void XSparkCallback(int a0, int a1) callsign 42004  {
    int v1;
    int v2;
    v1 = GetObjectContext(a0, 0);
    if (!((v1 == 4660))) {
        v2 = 2;
    } else {
        v2 = 2;
    }
    Hurt(a0, a1, v2);
    asynccall SmallFireBall3(a0, a1, 20, 0);
    return;
}
void XDecreaseHP()  {
    int v1;
    v1 = 1;
    while (((v1 <= (intvMagicAttackValue - 4)))) {
        DoHarmToMajor(intvDefenderMajor, 0, 1);
        Delay(4);
        v1++;
    }
    return;
}
void XCreateSparkleCenter(int a0, int a1)  {
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
    while (((v2 < (v3 / 4)))) {
        Delay(6);
        SetObjectCoordinate(v1, (v4 + Rand((-16), 16)), v5, (v6 + Rand((-16), 16)));
        v2++;
    }
    SetObjectCoordinate(v1, v4, v5, v6);
    SetObjectFadeOut(v1, 16, 1);
    return;
}
void XCreateSparkle(int a0)  {
    int v1;
    int v2;
    int v4;
    PlaySound1("m032snd01", 255);
    a0 = CreateObjectByReference(a0, 2501, 0, 0);
    v1 = CreateObjectByReference(a0, 42004, 0, 48);
    SetObjectScale(v1, 98304, 98304);
    SetCallbackProcedure(v1, 42004);
    SetObjectFlags(v1, (OF_ENEMYGENERAL + OF_ENEMYFORCE));
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
    while (((v4 < 256))) {
        asynccall LockByCenter(a0, 2501, v4, 52, 2, OF_ENEMYFORCE, 42004);
        v4 = (v4 + 32);
    }
    v4 = 0;
    while (((v4 < 256))) {
        asynccall LockByCenter(a0, 2501, v4, 110, 2, OF_ENEMYFORCE, 42004);
        v4 = (v4 + 32);
    }
    Delay(2);
    SetObjectFadeOut(v1, 16, 1);
    while ((IsObjectExist(v1))) {
        Delay(1);
    }
    Delay(8);
    SetObjectFadeOut(v2, 16, 1);
    FreeObject(a0);
    return;
}
void XSparkle(int a0)  {
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
void MoveEnemyDelay(int a0, int a1)  {
    int v1;
    int v2;
    Delay(a1);
    v1 = GetObjectScreenX(intvDefenderMajor);
    v2 = GetObjectScreenY(intvDefenderMajor);
    MoveCamera(v1, v2, a0);
    return;
}
void FinalX1()  {
    int v1;
    int v2;
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
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL1);
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
void XFirePillarCallback(int a0, int a1) callsign 43001  {
    int v1;
    if (!((a1 == intvDefenderMajor))) {
        Hurt(a0, a1, 0);
        asynccall FireMan(a1, 10015, 60);
    } else {
        if (GetGlobal(118)) {
            v1 = (2 / GetGlobal(118));
        }
        Hurt(a0, a1, v1);
        if ((v1 > 0)) {
            SetGlobal(118, (GetGlobal(118) * 2));
            HitGeneral(a0, a1, 11002, 1, 48, 0);
        }
    }
    return;
}
void XCreateFirePillarSource(int a0, int a1, int a2)  {
    int v1;
    int v2;
    v1 = CreateObjectByReference(a0, 41001, 0, 8);
    SetCoordinateByReference_Cylind(v1, a0, a2, a1, 8);
    PlaySound1("m031snd03", 255);
    Delay(8);
    v2 = a1;
    while (((v2 > 0))) {
        SetCoordinateByReference_Cylind(v1, a0, a2, v2, 8);
        Delay(1);
        a2 = ((a2 + 1) & 255);
        v2 = (v2 - 8);
    }
    SetObjectFadeOut(v1, 16, 1);
    return;
}
void XCreateFirePillarLight(int a0, int a1, int a2, int a3, int a4)  {
    int v1;
    int v2;
    int v3;
    v1 = CreateObjectByReference(a0, 41009, 0, 0);
    SetCoordinateByReference_Cylind(v1, a0, a1, a2, 0);
    SetObjectFlags(v1, (OF_ENEMYFORCE + OF_ENEMYGENERAL));
    SetCallbackProcedure(v1, 43001);
    SetObjectScale(v1, 32768, 24576);
    v3 = CreateObjectByReference(a0, 41010, 0, 0);
    SetCoordinateByReference_Cylind(v3, a0, a1, a2, 0);
    SetObjectScale(v3, 32768, 16384);
    v2 = 0;
    while (((v2 < a4))) {
        a1 = ((a1 + 4) & 255);
        a2 = (a2 + 6);
        Delay(1);
        SetCoordinateByReference_Cylind(v1, a0, a1, a2, 0);
        SetCoordinateByReference_Cylind(v3, a0, a1, a2, 0);
        SetCoordinateByReference_Cylind(a3, a0, a1, a2, 0);
        if ((v2 == (a4 - 16))) {
            SetObjectFadeOut(v1, 16, 1);
            SetObjectFadeOut(v3, 16, 1);
        }
        v2++;
    }
    return;
}
void XCreateFirePillarBomb(int a0, int a1, int a2, int a3)  {
    int v1;
    a0 = CreateObjectByReference(a0, 2501, 0, 0);
    v1 = CreateObjectByReference(a0, 41008, 0, 0);
    SetCoordinateByReference_Cylind(v1, a0, a1, a2, 0);
    SetObjectScale(v1, 32768, 65536);
    SetObjectFadeOut(v1, a3, 1);
    XCreateFirePillarLight(a0, a1, a2, v1, a3);
    FreeObject(a0);
    return;
}
void XCreateFirePillar(int a0, int a1)  {
    int v1;
    int v2;
    int v3;
    int v4;
    v1 = CreateObjectRaw(a0, a1, 0, 0, 2501);
    v3 = Rand(0, 255);
    v2 = 0;
    while (((v2 < 4))) {
        asynccall XCreateFirePillarSource(v1, 320, v3);
        v3 = ((v3 + 64) & 255);
        v2++;
    }
    Wait("XCreateFirePillarSource");
    Delay(8);
    asynccall LockTargetXY(a0, a1, 2501, 0, 60, 43001, 2, 3, (OF_ENEMYFORCE + OF_ENEMYGENERAL));
    asynccall LockTargetXY((a0 + 60), a1, 2501, 60, 2, 43001, 2, 3, OF_ENEMYFORCE);
    asynccall LockTargetXY((a0 - 60), a1, 2501, 60, 2, 43001, 2, 3, OF_ENEMYFORCE);
    asynccall LockTargetXY(a0, (a1 + 50), 2501, 60, 2, 43001, 2, 3, OF_ENEMYFORCE);
    asynccall LockTargetXY(a0, (a1 + 50), 2501, 60, 2, 43001, 2, 3, OF_ENEMYFORCE);
    asynccall LockTargetXY((a0 + 60), (a1 + 50), 2501, 60, 2, 43001, 2, 3, OF_ENEMYFORCE);
    asynccall LockTargetXY((a0 + 60), (a1 - 50), 2501, 60, 2, 43001, 2, 3, OF_ENEMYFORCE);
    asynccall LockTargetXY((a0 - 60), (a1 + 50), 2501, 60, 2, 43001, 2, 3, OF_ENEMYFORCE);
    asynccall LockTargetXY((a0 - 60), (a1 - 50), 2501, 60, 2, 43001, 2, 3, OF_ENEMYFORCE);
    asynccall LockTargetXY((a0 + 120), a1, 2501, 60, 2, 43001, 2, 3, OF_ENEMYFORCE);
    asynccall LockTargetXY((a0 - 120), a1, 2501, 60, 2, 43001, 2, 3, OF_ENEMYFORCE);
    asynccall LockTargetXY(a0, (a1 + 100), 2501, 60, 2, 43001, 2, 3, OF_ENEMYFORCE);
    asynccall LockTargetXY(a0, (a1 + 100), 2501, 60, 2, 43001, 2, 3, OF_ENEMYFORCE);
    asynccall LockTargetXY((a0 + 120), (a1 + 100), 2501, 60, 2, 43001, 2, 3, OF_ENEMYFORCE);
    asynccall LockTargetXY((a0 + 120), (a1 - 100), 2501, 60, 2, 43001, 2, 3, OF_ENEMYFORCE);
    asynccall LockTargetXY((a0 - 120), (a1 + 100), 2501, 60, 2, 43001, 2, 3, OF_ENEMYFORCE);
    asynccall LockTargetXY((a0 - 120), (a1 - 100), 2501, 60, 2, 43001, 2, 3, OF_ENEMYFORCE);
    PlaySound1("m031snd01", 255);
    v2 = 0;
    while (((v2 < 4))) {
        v4 = CreateObjectByReference(v1, (41002 + v2), 0, (-12));
        Delay(4);
        if ((v2 < 3)) {
            FreeObject(v4);
        }
        v2++;
    }
    v2 = 65536;
    while (((v2 < 131072))) {
        SetObjectScale(v4, 65536, v2);
        Delay(2);
        v2 = (v2 + 8192);
    }
    Delay(40);
    PlaySound1("m031snd02", 255);
    asynccall XCreateFirePillarBomb(v4, 32, 10, 70);
    asynccall XCreateFirePillarBomb(v4, 96, 10, 70);
    asynccall XCreateFirePillarBomb(v4, 160, 10, 70);
    asynccall XCreateFirePillarBomb(v4, 224, 10, 70);
    SetObjectScaleShrink(v4, 4096);
    SetObjectFadeOut(v4, 16, 1);
    FreeObject(v1);
    return;
}
void XFirePillar(int a0)  {
    int v1;
    int v2;
    int v3;
    SetGlobal(118, 1);
    v1 = GetObjectScreenX(intvAttackerMajor);
    v2 = GetObjectScreenY(intvAttackerMajor);
    v3 = GetMostImportantSoldier((intvIsLeft ^ 1));
    if ((v3 == 0)) {
        v3 = intvDefenderMajor;
    }
    v1 = GetObjectScreenX(v3);
    v2 = GetObjectScreenY(v3);
    MoveCamera(v1, (v2 - 180), 60);
    XCreateFirePillar(v1, v2);
    Wait("XCreateFirePillarBomb");
    Delay(60);
    return;
}
void XTaiChiCallback(int a0, int a1) callsign 43002  {
    if (!((a1 == intvDefenderMajor))) {
        Hurt(a0, a1, 0);
    } else {
        if (!((GetGlobal(119) < 4))) {
            ClearObjectFlags(a0, OF_ENEMYGENERAL);
        } else {
            Hurt(a0, a1, (2 / GetGlobal(119)));
            SetGlobal(119, (GetGlobal(119) * 2));
        }
    }
    return;
}
void XCreateTaiChiBomb(int a0, int a1)  {
    int v1;
    int v2;
    int v3;
    Delay(8);
    v1 = CreateObjectByReference(a0, 35002, 0, 0);
    SetObjectScale(v1, 4096, 4096);
    if (a1) {
        SetCallbackProcedure(v1, 43002);
        SetObjectFlags(v1, (OF_ENEMYFORCE + OF_ENEMYGENERAL));
    }
    SetObjectScaleShrink(v1, (-4096));
    Delay(20);
    v2 = GetObjectScreenX(v1);
    v3 = GetObjectScreenY(v1);
    asynccall LockTargetXY((v2 + 64), v3, 2501, 0, 20, 43002, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY((v2 - 64), v3, 2501, 0, 20, 43002, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY(v2, (v3 + 50), 2501, 0, 20, 43002, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY(v2, (v3 - 50), 2501, 0, 20, 43002, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY((v2 + 64), (v3 + 50), 2501, 0, 20, 43002, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY((v2 + 64), (v3 - 50), 2501, 0, 20, 43002, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY((v2 - 64), (v3 + 50), 2501, 0, 20, 43002, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY((v2 - 64), (v3 - 50), 2501, 0, 20, 43002, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY((v2 + 128), v3, 2501, 0, 20, 43002, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY((v2 - 128), v3, 2501, 0, 20, 43002, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY(v2, (v3 + 100), 2501, 0, 20, 43002, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY(v2, (v3 - 100), 2501, 0, 20, 43002, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY((v2 + 128), (v3 + 100), 2501, 0, 20, 43002, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY((v2 + 128), (v3 - 100), 2501, 0, 20, 43002, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY((v2 - 128), (v3 + 100), 2501, 0, 20, 43002, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY((v2 - 128), (v3 - 100), 2501, 0, 20, 43002, 0, 0, OF_ENEMYFORCE);
    asynccall LockTargetXY(v2, (v3 + 150), 2501, 0, 16, 43002, 0, 0, OF_ENEMYFORCE);
    Delay(20);
    SetObjectFadeOut(v1, 16, 1);
    return;
}
void XCreateTaichi(int a0, int a1)  {
    int v1;
    int v2;
    v1 = CreateObjectRaw(a0, a1, 0, 0, 35001);
    SetObjectLayer(v1, 2);
    SetObjectScale(v1, 98304, 98304);
    v2 = 0;
    while (((v2 <= 16))) {
        SetObjectOpacity(v1, v2);
        Delay(1);
        v2++;
    }
    PlaySound1("m025snd01", 220);
    v2 = 0;
    while (((v2 < 3))) {
        asynccall XCreateTaiChiBomb(v1, (v2 == 0));
        Delay((10 - v2));
        v2++;
    }
    Delay(30);
    v2 = 16;
    while (((v2 >= 0))) {
        SetObjectOpacity(v1, v2);
        Delay(1);
        v2--;
    }
    FreeObject(v1);
    return;
}
void XTaiChi(int a0)  {
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
    while (((v4 < (a0 + 1)))) {
        v5 = 0;
        while (((v5 < 5))) {
            if (!((v5 == 0))) {
                if (!((v5 == 1))) {
                    if (!((v5 == 2))) {
                        if (!((v5 == 3))) {
                            v1 = (v6 - Rand(200, 260));
                            v2 = (v7 - Rand(220, 270));
                            v9 = ScreenXToBattleX(v1);
                            v10 = ScreenXToBattleX(v2);
                            v8 = GetSoldierCountInRect((intvIsLeft ^ 1), v9, v10, 3, 3);
                            if ((v8 > 0)) {
                                v12 = 0;
                                while (((v12 < (v8 / 2)))) {
                                    v11 = GetNthSoldierInRect((intvIsLeft ^ 1), v9, v10, 3, 3, Rand(0, (v8 - 1)));
                                    if ((v11 != intvDefenderMajor)) {
                                        v13 = GetObjectScreenX(v11);
                                        v14 = GetObjectScreenY(v11);
                                        v15 = (((v13 - v6) * (v13 - v6)) + ((v14 - v7) * (v14 - v7)));
                                        if ((v15 >= 72200)) {
                                            v1 = v13;
                                            v2 = v14;
                                            break;
                                        }
                                    }
                                    v12++;
                                }
                            }
                        } else {
                            v1 = (v6 - Rand(200, 260));
                            v2 = (v7 + Rand(220, 270));
                            v9 = ScreenXToBattleX(v1);
                            v10 = ScreenXToBattleX(v2);
                            v8 = GetSoldierCountInRect((intvIsLeft ^ 1), v9, v10, 3, 3);
                            if ((v8 > 0)) {
                                v12 = 0;
                                while (((v12 < (v8 / 2)))) {
                                    v11 = GetNthSoldierInRect((intvIsLeft ^ 1), v9, v10, 3, 3, Rand(0, (v8 - 1)));
                                    if ((v11 != intvDefenderMajor)) {
                                        v13 = GetObjectScreenX(v11);
                                        v14 = GetObjectScreenY(v11);
                                        v15 = (((v13 - v6) * (v13 - v6)) + ((v14 - v7) * (v14 - v7)));
                                        if ((v15 >= 72200)) {
                                            v1 = v13;
                                            v2 = v14;
                                            break;
                                        }
                                    }
                                    v12++;
                                }
                            }
                        }
                    } else {
                        v1 = (v6 + Rand(200, 260));
                        v2 = (v7 - Rand(220, 270));
                        v9 = ScreenXToBattleX(v1);
                        v10 = ScreenXToBattleX(v2);
                        v8 = GetSoldierCountInRect((intvIsLeft ^ 1), v9, v10, 3, 3);
                        if ((v8 > 0)) {
                            v12 = 0;
                            while (((v12 < (v8 / 2)))) {
                                v11 = GetNthSoldierInRect((intvIsLeft ^ 1), v9, v10, 3, 3, Rand(0, (v8 - 1)));
                                if ((v11 != intvDefenderMajor)) {
                                    v13 = GetObjectScreenX(v11);
                                    v14 = GetObjectScreenY(v11);
                                    v15 = (((v13 - v6) * (v13 - v6)) + ((v14 - v7) * (v14 - v7)));
                                    if ((v15 >= 72200)) {
                                        v1 = v13;
                                        v2 = v14;
                                        break;
                                    }
                                }
                                v12++;
                            }
                        }
                    }
                } else {
                    v1 = (v6 + Rand(200, 260));
                    v2 = (v7 + Rand(220, 270));
                    v9 = ScreenXToBattleX(v1);
                    v10 = ScreenXToBattleX(v2);
                    v8 = GetSoldierCountInRect((intvIsLeft ^ 1), v9, v10, 3, 3);
                    if ((v8 > 0)) {
                        v12 = 0;
                        while (((v12 < (v8 / 2)))) {
                            v11 = GetNthSoldierInRect((intvIsLeft ^ 1), v9, v10, 3, 3, Rand(0, (v8 - 1)));
                            if ((v11 != intvDefenderMajor)) {
                                v13 = GetObjectScreenX(v11);
                                v14 = GetObjectScreenY(v11);
                                v15 = (((v13 - v6) * (v13 - v6)) + ((v14 - v7) * (v14 - v7)));
                                if ((v15 >= 72200)) {
                                    v1 = v13;
                                    v2 = v14;
                                    break;
                                }
                            }
                            v12++;
                        }
                    }
                }
            } else {
                v3 = GetRandomSoldierHandleFromAlive((intvIsLeft ^ 1));
                if ((v3 == 0)) {
                    v3 = intvDefenderMajor;
                }
                v6 = GetObjectScreenX(v3);
                v7 = GetObjectScreenY(v3);
                v1 = v6;
                v2 = v7;
            }
            asynccall XCreateTaichi(v1, v2);
            Delay(Rand(8, 12));
            v5++;
        }
        Delay(45);
        v4++;
    }
    Wait("XCreateTaichi");
    Delay(60);
    return;
}
void XDuplicatorCallback(int a0, int a1) callsign 43003  {
    Hurt(a0, a1, intvMagicAttackValue);
    if (!(((a1 == intvAttackerMajor) || (a1 == intvDefenderMajor)))) {
        asynccall Blood(a1, 16);
    } else {
        asynccall SmallFireBall(a0, a1, 12, 0);
        Delay(4);
        asynccall SmallFireBall(a0, a1, 12, 0);
        Delay(4);
        asynccall SmallFireBall(a0, a1, 12, 0);
        Delay(4);
        asynccall SmallFireBall(a0, a1, 12, 0);
        Delay(4);
        asynccall SmallFireBall(a0, a1, 12, 0);
    }
    return;
}
void XTraceDuplicator(int a0, int a1)  {
    int v1;
    int v2;
    int v3;
    int v4;
    v4 = 0;
    while ((IsObjectExist(a0))) {
        Delay(1);
        if ((IsObjectExist(a1) == 0)) {
            break;
        }
        v1 = GetObjectScreenX(a0);
        v2 = GetObjectScreenX(a1);
        v3 = (v2 - v1);
        if ((v3 < 0)) {
            v3 = (-v3);
        }
        if (!(((v3 < 360) && (v4 < 2)))) {
            if (((v3 < 1080) && (v4 < 1))) {
                SetObjectAnimate(a0, 2);
                v4 = 1;
            }
        } else {
            SetObjectAnimate(a0, OAF_ATTACK);
            v4 = 2;
        }
    }
    return;
}
void XMoveDuplicator(int a0, int a1, int a2)  {
    int v1;
    int v2;
    int v3;
    v3 = GetObjectDir(a0);
    v2 = CreateObjectByReference(a0, a1, v3, 0);
    SetCoordinateByReference_Cylind(v2, a0, v3, (48 + 64), 0);
    v1 = a2;
    do {
        SetCoordinateByReference_Cylind(v2, a0, v3, (48 + 64), 0);
        Delay(1);
        if ((!IsObjectExist(a0))) {
            break;
        }
        v1--;
    } while ((v1 >= 0));
    FreeObject(v2);
    return;
}
void XDuplicatorShadow(int a0, int a1, int a2, int a3, int a4)  {
    int v1;
    if (!((a4 == 0))) {
        v1 = CreateObjectByReference(a0, a1, a2, 0);
        if (!((a2 == 0))) {
            MoveObject(v1, 8, 0, 0);
        } else {
            MoveObject(v1, (-8), 0, 0);
        }
        SetObjectSpeed_Sphere(v1, a2, 0, a3);
        SetObjectFlags(v1, OF_MIXER);
        SetObjectOpacity(v1, a4);
        Delay(2);
        SetObjectFadeOut(v1, 20, 1);
        while ((IsObjectExist(a0))) {
            if ((IsObjectExist(v1) == 0)) {
                return;
            }
            Delay(1);
        }
        FreeObject(v1);
        return;
    } else {
        return;
    }
}
void XProduceDuplicator(int a0, int a1, int a2, int a3)  {
    while ((IsObjectExist(a0))) {
        asynccall XDuplicatorShadow(a0, a1, a2, (a3 - 8), 14);
        Delay(8);
    }
    return;
}
void XDuplicator(int a0)  {
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
    if ((Rand(0, 3) == 3)) {
        v10 = Rand(0, 90);
    }
    v9 = (1 - (2 * (intvIsLeft != 0)));
    if (!((intvIsLeft == 1))) {
        v8 = 128;
    } else {
        v8 = 0;
    }
    v5 = GetObjectScreenX(intvAttackerMajor);
    v7 = GetObjectScreenY(intvAttackerMajor);
    PlaySound(intvAttackerMajor, "att07", 255);
    v2 = CreateObjectByReference(intvAttackerMajor, 22002, v8, 0);
    SetObjectSpeed_Sphere(v2, v8, 0, 16);
    SetObjectFlags(v2, (OF_ENEMYGENERAL + OF_ENEMYFORCE));
    SetCallbackProcedure(v2, 43003);
    asynccall ProduceShadowTime(v2, 9999);
    asynccall XMoveDuplicator(v2, 22001, 9999);
    v5 = 16;
    v4 = 1;
    v3 = 0;
    asynccall XTraceDuplicator(v2, intvDefenderMajor);
    do {
        SetObjectSpeed_Sphere(v2, v8, 0, (v5 / 16));
        if ((v5 < (24 * 16))) {
            v5 = (v5 + v4);
            v4 = (v4 + 1);
        }
        if (((v3 & 1) == 0)) {
            if (!((v5 < 255))) {
                PlaySound(v2, "m012snd01", 255);
            } else {
                PlaySound(v2, "m012snd01", 255);
            }
        }
        v3++;
        Delay(1);
        v1 = IsObjectExist(v2);
        v6 = GetObjectScreenX(v2);
        if (((v6 <= 50) || (v6 >= (GetBattleWidthInScreenX() - 51)))) {
            FreeObject(v2);
        }
    } while (v1);
    Delay(60);
    return;
}
void FinalX2()  {
    int v1;
    int v2;
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
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL1);
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
void XProduceSoldier(int a0, int a1)  {
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
    do {
        v2 = (v7 / 2);
        do {
            if (!((a0 == 1))) {
                v3 = GetForceHandleByXY(((v6 - 1) - v1), v2);
            } else {
                v3 = GetForceHandleByXY(v1, v2);
            }
            if ((v3 == 0)) {
                if (!((a0 == 1))) {
                    v5 = CreateSoldier(a0, ((v6 - 1) - v1), v2);
                    asynccall StepShow(v5, 10);
                } else {
                    v5 = CreateSoldier(a0, v1, v2);
                    asynccall StepShow(v5, 10);
                }
                v4++;
                if ((v4 >= a1)) {
                    return;
                }
            }
            if (!((intvIsLeft == 1))) {
                v3 = GetForceHandleByXY(((v6 - 1) - v1), ((v7 - 1) - v2));
            } else {
                v3 = GetForceHandleByXY(v1, ((v7 - 1) - v2));
            }
            if ((v3 == 0)) {
                if (!((a0 == 1))) {
                    v5 = CreateSoldier(a0, ((v6 - 1) - v1), ((v7 - 1) - v2));
                    asynccall StepShow(v5, 10);
                } else {
                    v5 = CreateSoldier(a0, v1, ((v7 - 1) - v2));
                    asynccall StepShow(v5, 10);
                }
                v4++;
                if ((v4 >= a1)) {
                    return;
                }
            }
            v2--;
        } while ((v2 > 0));
        v1++;
    } while ((v1 <= v6));
    return;
}
void XMoreSoldier(int a0)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    int v7;
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
    if (!((v3 == 1))) {
        v2 = 128;
    } else {
        v2 = 0;
    }
    v7 = (200 - GetSoldierCount(v3));
    if ((v7 < 0)) {
        v7 = 0;
    }
    if ((v7 > a0)) {
        v7 = a0;
    }
    if ((v7 != 0)) {
        asynccall XProduceSoldier(v3, v7);
        SetObjectAnimate(v1, OAF_SPELL1);
        Delay(24);
        SetObjectAnimate(v1, OAF_SPELL2);
        SetOverwhelming(v1, 0);
        Delay(6);
        PlaySound1("m002snd01", 255);
        PlaySound1("m002snd03", 255);
        if (!((intvIsLeft == 0))) {
            MoveCamera(240, 0, 40);
        } else {
            MoveCamera(6576, 0, 40);
        }
        Delay(100);
        v4 = GetObjectScreenX(intvAttackerMajor);
        v5 = GetObjectScreenY(intvAttackerMajor);
        MoveCamera(v4, (v5 - 120), 20);
    }
    Delay(20);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
    return;
}
void F103() callsign 103  {
    if ((GetGlobal(120) < 501)) {
        SetGlobal(120, 501);
    }
    intvMagicAttackValue = GetGlobal(121);
    DoScript(GetGlobal(120), 0, 0, 0);
    return;
}
void F104() callsign 104  {
    if (!((GetGlobal(120) < 501))) {
        SetGlobal(120, GetGlobal(120) + 1);
    } else {
        SetGlobal(120, 501);
    }
    if ((GetGlobal(120) > 610)) {
        SetGlobal(120, 501);
    }
    DoScript(GetGlobal(120), 0, 0, 0);
    return;
}
void F100() callsign 100  {
    if ((GetGlobal(120) < 501)) {
        SetGlobal(120, 501);
    }
    intvMagicAttackValue = GetGlobal(121);
    DoScript(GetGlobal(120), 0, 0, 0);
    return;
}
void F200() callsign 200  {
    if (!((GetGlobal(120) < 501))) {
        SetGlobal(120, GetGlobal(120) + 1);
    } else {
        SetGlobal(120, 501);
    }
    if ((GetGlobal(120) > 610)) {
        SetGlobal(120, 501);
    }
    DoScript(GetGlobal(120), 0, 0, 0);
    return;
}
void F9999(int a0, int a1) callsign 9999  {
    SetGlobal(121, a1);
    SetGlobal(120, a0);
    return;
}
void InitializeBattle()  {
    int v1;
    if ((GetGlobal(120) == 0)) {
        SetGlobal(120, 501);
    }
    SetGlobal(3, 0);
    SetGlobal(1, 0);
    SetGlobal(28 + 0, 0);
    SetGlobal(28 + 1, 0);
    SetGlobal(35 + 0, 0);
    SetGlobal(35 + 1, 0);
    v1 = 0;
    while (((v1 < 60))) {
        SetGlobal(55 + v1, 0);
        v1++;
    }
    return;
}
void Magic1() callsign 501  {
    Heal(0);
    return;
}
void Magic2() callsign 502  {
    ShootObject(0, 1);
    return;
}
void Magic3() callsign 503  {
    ArrowSupport(17, 1);
    return;
}
void Magic4() callsign 504  {
    Spout(0);
    return;
}
void Magic5() callsign 505  {
    Powder(0);
    return;
}
void Magic6() callsign 506  {
    RollTub(0);
    return;
}
void Magic7() callsign 507  {
    OnFire(0);
    return;
}
void Magic8() callsign 508  {
    ExplodeRound(18, 160, 18003);
    return;
}
void Magic9() callsign 509  {
    MoreSoldier(17, 0);
    return;
}
void Magic10() callsign 510  {
    Thunder(20, 0);
    return;
}
void Magic11() callsign 511  {
    Thunder(9, 1);
    return;
}
void Magic12() callsign 512  {
    FireDragon(0);
    return;
}
void Magic13() callsign 513  {
    Interceptor(0);
    return;
}
void Magic14() callsign 514  {
    ShootObject(1, 0);
    return;
}
void Magic15() callsign 515  {
    ArrowSupport(29, 1);
    return;
}
void Magic16() callsign 516  {
    Spout(1);
    return;
}
void Magic17() callsign 517  {
    Powder(1);
    return;
}
void Magic18() callsign 518  {
    RollTub(1);
    return;
}
void Magic19() callsign 519  {
    ExplodeRound(32, 240, 18003);
    return;
}
void Magic20() callsign 520  {
    FireDragon(1);
    return;
}
void Magic21() callsign 521  {
    OnFire(1);
    return;
}
void Magic22() callsign 522  {
    Thunder(16, 1);
    return;
}
void Magic23() callsign 523  {
    MoreSoldier(32, 0);
    return;
}
void Magic24() callsign 524  {
    BackSoldier(25, 0);
    return;
}
void Magic25() callsign 525  {
    Thunder(62, 0);
    return;
}
void Magic26() callsign 526  {
    CallDragon(2);
    return;
}
void Magic27() callsign 527  {
    Interceptor(1);
    return;
}
void Magic28() callsign 528  {
    FireDragon(2);
    return;
}
void Magic29() callsign 529  {
    OnFire(2);
    return;
}
void Magic30() callsign 530  {
    Powder(2);
    return;
}
void Magic31() callsign 531  {
    ExplodeRound(48, 380, 18003);
    return;
}
void Magic32() callsign 532  {
    Spout(2);
    return;
}
void Magic33() callsign 533  {
    BackSoldier(35, 0);
    return;
}
void Magic34() callsign 534  {
    ArrowSupport(66, 1);
    return;
}
void Magic35() callsign 535  {
    RollTub(3);
    return;
}
void Magic36() callsign 536  {
    FireDragon(3);
    return;
}
void Magic37() callsign 537  {
    BigThunder();
    return;
}
void Magic38() callsign 538  {
    Thunder(40, 1);
    return;
}
void Magic39() callsign 539  {
    Interceptor(2);
    return;
}
void Magic40() callsign 540  {
    Earthquake();
    return;
}
void Magic41() callsign 541  {
    BackSoldier(45, 0);
    return;
}
void Magic42() callsign 542  {
    FireDragon(4);
    return;
}
void Magic43() callsign 543  {
    Sparkle(0);
    return;
}
void Magic44() callsign 544  {
    FinalX1();
    return;
}
void Magic45() callsign 545  {
    ArrowSupport(8, 0);
    return;
}
void Magic46() callsign 546  {
    RunningBow(0);
    return;
}
void Magic47() callsign 547  {
    StoneEmitter(0, 0, 9);
    return;
}
void Magic48() callsign 548  {
    ConvexStone(0);
    return;
}
void Magic49() callsign 549  {
    FireRing(0, 0);
    return;
}
void Magic50() callsign 550  {
    HalfMoon(0);
    return;
}
void Magic51() callsign 551  {
    Ice(0);
    return;
}
void Magic52() callsign 552  {
    ShootObject(0, 0);
    return;
}
void Magic53() callsign 553  {
    Slash(0);
    return;
}
void Magic54() callsign 554  {
    Tornado(0);
    return;
}
void Magic55() callsign 555  {
    RollDown(0);
    return;
}
void Magic56() callsign 556  {
    EightWayFire(2, 0, 19002);
    return;
}
void Magic57() callsign 557  {
    TaiChi(0);
    return;
}
void Magic58() callsign 558  {
    ShootObject(2, 0);
    return;
}
void Magic59() callsign 559  {
    FireCow(0);
    return;
}
void Magic60() callsign 560  {
    FireWork(0);
    return;
}
void Magic61() callsign 561  {
    FlyingSword(0);
    return;
}
void Magic62() callsign 562  {
    Spear(0);
    return;
}
void Magic63() callsign 563  {
    Heal(1);
    return;
}
void Magic64() callsign 564  {
    RunningBow(1);
    return;
}
void Magic65() callsign 565  {
    MoreSoldier(25, 0);
    return;
}
void Magic66() callsign 566  {
    BlackHole(0);
    return;
}
void Magic67() callsign 567  {
    RushCart(0);
    return;
}
void Magic68() callsign 568  {
    EightWayFire(2, 1, 19002);
    return;
}
void Magic69() callsign 569  {
    FireRing(1, 0);
    return;
}
void Magic70() callsign 570  {
    FlyingSword(1);
    return;
}
void Magic71() callsign 571  {
    FireRing(0, 1);
    return;
}
void Magic72() callsign 572  {
    ArrowSupport(33, 0);
    return;
}
void Magic73() callsign 573  {
    TaiChi(1);
    return;
}
void Magic74() callsign 574  {
    FireCow(1);
    return;
}
void Magic75() callsign 575  {
    ConvexStone(1);
    return;
}
void Magic76() callsign 576  {
    RollDown(1);
    return;
}
void Magic77() callsign 577  {
    HalfMoon(1);
    return;
}
void Magic78() callsign 578  {
    FireWork(1);
    return;
}
void Magic79() callsign 579  {
    Ice(1);
    return;
}
void Magic80() callsign 580  {
    Tornado(1);
    return;
}
void Magic81() callsign 581  {
    Slash(1);
    return;
}
void Magic82() callsign 582  {
    Spear(1);
    return;
}
void Magic83() callsign 583  {
    StoneEmitter(1, 1, 14);
    return;
}
void Magic84() callsign 584  {
    MoreSoldier(42, 0);
    return;
}
void Magic85() callsign 585  {
    Frozen();
    return;
}
void Magic86() callsign 586  {
    RushCart(1);
    return;
}
void Magic87() callsign 587  {
    RunningBow(2);
    return;
}
void Magic88() callsign 588  {
    Tornado(2);
    return;
}
void Magic89() callsign 589  {
    BlackHole(1);
    return;
}
void Magic90() callsign 590  {
    ArrowSupport(56, 0);
    return;
}
void Magic91() callsign 591  {
    FireRing(1, 1);
    return;
}
void Magic92() callsign 592  {
    HalfMoonNew(0);
    return;
}
void Magic93() callsign 593  {
    ConvexStone(2);
    return;
}
void Magic94() callsign 594  {
    Pa(0);
    return;
}
void Magic95() callsign 595  {
    FlyingSword(2);
    return;
}
void Magic96() callsign 596  {
    FireCow(2);
    return;
}
void Magic97() callsign 597  {
    RollDown(2);
    return;
}
void Magic98() callsign 598  {
    Duplicator(1);
    return;
}
void Magic99() callsign 599  {
    FireWork(2);
    return;
}
void Magic100() callsign 600  {
    Wall(0);
    return;
}
void Magic101() callsign 601  {
    Ice(2);
    return;
}
void Magic102() callsign 602  {
    HalfMoon(2);
    return;
}
void Magic103() callsign 603  {
    Spear(2);
    return;
}
void Magic104() callsign 604  {
    StoneEmitter(2, 1, 20);
    return;
}
void Magic105() callsign 605  {
    DemonDancing();
    return;
}
void Magic106() callsign 606  {
    TwisterSword();
    return;
}
void Magic107() callsign 607  {
    FirePillar(0);
    return;
}
void Magic108() callsign 608  {
    Wall(1);
    return;
}
void Magic109() callsign 609  {
    HalfMoonNew(1);
    return;
}
void Magic110() callsign 610  {
    FinalX2();
    return;
}
void Magic200() callsign 700  {
    XMoreSoldier(200);
    return;
}
