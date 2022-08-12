// 该文件包含所有来自原版magic.so的工具函数。
// magic.cpp中的这些函数已经删除，改为include本文件。
// 部分函数已添加注释。
// Encoding: UTF-8

// 此处OAF_ATTACK的define必须在所有OAF_ATTACK*的后面，否则会得到意料之外的编译结果。
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

// 播放指定的音频，给定音频文件名、音量和时长。
// 这个函数在原版magic下，总是在GetGlobal(1) == 0时才执行，在这种情况下其作用为
// （如果没有其它环境音正在播放则）播放指定的环境音。
void DelayAmbientSound(string soundName, int volume, int duration)  {
    SetGlobal(1, 1);
    PlaySound1(soundName, volume);
    Delay(duration);
    SetGlobal(1, 0);
    return;
}
// 得到指定方的敌方尽可能靠前的士兵，主将不算。
int GetSoldierMaxX2(int isLeft)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    v1 = GetSoldierCount((isLeft ^ 1));
    if (!(v1 > 0)) {
        return (-1);
    } else {
        v2 = ((v1 / 2) + 1);
        v4 = GetRandomSoldierHandleFromAlive((isLeft ^ 1));
        v3 = GetObjectScreenX(v4);
        do {
            v4 = GetRandomSoldierHandleFromAlive((isLeft ^ 1));
            v5 = GetObjectScreenX(v4);
            if (!(isLeft == 1)) {
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
// 得到指定方的敌方尽可能靠前的士兵，如果没有士兵则得到主将。
int GetSoldierMaxX(int isLeft)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    v1 = GetSoldierCount((isLeft ^ 1));
    if (!(v1 > 0)) {
        if (!(isLeft == 1)) {
            v5 = GetObjectScreenX(intvDefenderMajor);
            v3 = (v5 + 320);
        } else {
            v5 = GetObjectScreenX(intvDefenderMajor);
            v3 = (v5 - 320);
        }
    } else {
        v2 = ((v1 / 2) + 1);
        v4 = GetRandomSoldierHandleFromAlive((isLeft ^ 1));
        v3 = GetObjectScreenX(v4);
        do {
            v4 = GetRandomSoldierHandleFromAlive((isLeft ^ 1));
            v5 = GetObjectScreenX(v4);
            if (!(isLeft == 1)) {
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
// 全场定身指定时长。
void HoldAllDelay(int duration)  {
    SetAttackCounterTo9999();
    Delay(duration);
    RestoreAttackCounter();
    return;
}
// 制造指定物件的“影子”物件，在duration Tick内每帧制造一个和指定物件长得完全一样的物件，
// 每个物件持续12 Tick渐变到消失。
// 猜想是用于制造指定物件的“残影”效果。原版magic下应用之一是狂雷。
void ProduceShadowTime(int sourceObject, int duration)  {
    int v1;
    v1 = duration;
    do {
        CreateShadowObject(sourceObject, 12, 1);
        if (!IsObjectExist(sourceObject)) {
            break;
        }
        Delay(1);
        v1--;
    } while (v1 >= 0);
    return;
}
// 在refObject旁边制造一个指定Sequence的物件，该物件向dir方向每帧移动16像素，并且具备杀伤敌方士兵的能力，
// 持续duration帧，每帧都会留下一个持续存在的“残影”。
// 杀伤的回调函数由callbackFuncCallsign指定。
// 该函数原版magic下为分身斩专用函数。
void MovingShadow(int refObject, int duration, int sequence, int dir, int callbackFuncCallsign)  {
    int v1;
    int v2;
    v2 = CreateObjectByReference(refObject, sequence, dir, 0);
    SetObjectFlags(v2, OF_ENEMYFORCE);
    SetCallbackProcedure(v2, callbackFuncCallsign);
    v1 = 1;
    do {
        CreateShadowObject1(v2, 4, 1, 8192);
        SetCoordinateByReference_Cylind(v2, refObject, dir, (v1 * 16), 0);
        Delay(1);
        v1++;
    } while (v1 <= duration);
    SetObjectFadeOut(v2, 16, 1);
    return;
}
// 在refObject旁边制造一个指定sequence的物件，其与原物件之间的位置相对偏移由(dir, dirOffset)指定，
// 存在时间为life，标记为flags，回调函数为callbackFuncCallsign 
// ----该函数是在指定物件旁边快速生成具备杀伤功能的物件的“快捷”函数。
void LockByCenter(int refObject, int sequence, int dir, int dirOffset, 
                  int life, int flags, int callbackFuncCallsign)  {
    int v2;
    v2 = CreateObjectByReference(refObject, sequence, dir, 0);
    SetCoordinateByReference_Cylind(v2, refObject, dir, dirOffset, 0);
    SetObjectFlags(v2, flags);
    SetCallbackProcedure(v2, callbackFuncCallsign);
    Delay(life);
    FreeObject(v2);
    return;
}
// 在随机时间 Rand(delayRandLeft, delayRandRight) 后，
// 制造一个指定sequence的物件，指定其方向为dir，位置为(objX, objY),
// 存在时间为life，标记为flags，回调函数为callbackFuncCallsign,
// 生命周期到达后，在16 Tick内淡出。
// ----该函数是在指定位置快速生成具备杀伤功能的物件的“快捷”函数。
void LockTargetXY(int objX, int objY, int sequence, int dir, int life, 
                  int callbackFuncCallsign, 
                  int delayRandLeft, int delayRandRight, int flags)  {
    int v1;
    Delay(Rand(delayRandLeft, delayRandRight));
    v1 = CreateObjectByReference(intvAttackerMajor, sequence, dir, 0);
    SetObjectCoordinate(v1, objX, objY, 0);
    SetObjectFlags(v1, flags);
    SetCallbackProcedure(v1, callbackFuncCallsign);
    Delay(life);
    SetObjectFadeOut(v1, 16, 1);
    return;
}
// 制造一个指定sequence的物件并锁定在target物件（主将或士兵）上。
// 该物件存在时间为life，生命周期到达后在16 Tick内淡出。
void LockTargetTime2(int target, int sequence, int life)  {
    int v1;
    int v2;
    int v3;
    int v4;
    if (!((target == intvDefenderMajor) || (target == intvAttackerMajor))) {
        v4 = 0;
    } else {
        if (!(GetMajorHP(GetSoldierSide(target)) != 0)) {
            v4 = 0;
        } else {
            v4 = 40;
        }
    }
    v3 = GetObjectDir(target);
    v2 = CreateObjectByReference(target, sequence, v3, 0);
    v1 = life;
    do {
        if (!IsObjectExist(target)) {
            break;
        }
        SetCoordinateByReference_Cylind(v2, target, 0, 0, v4);
        Delay(1);
        v1--;
    } while (v1 >= 0);
    SetObjectFadeOut(v2, 16, 1);
    return;
}
// object物件给目标target造成attackValue点伤害。
// 如果击中主将，则清除object物件的打击武将标记。
void Hurt(int object, int target, int attackValue)  {
    if (!((target == intvAttackerMajor) || (target == intvDefenderMajor))) {
        PlaySound(target, "Hit01", 255);
    } else {
        ClearObjectFlags(object, (OF_MYGENERAL + OF_ENEMYGENERAL));
        if (attackValue <= 0) {
            return;
        }
        DoHarmToMajor(target, 0, attackValue);
    }
    return;
}
// 在duration时间内平移摄像机至(x, y)处。
void MoveCamera(int x, int y, int duration)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    v2 = (GetCameraX() * 1000);
    v3 = (GetCameraY() * 1000);
    x = (x * 1000);
    y = (y * 1000);
    v4 = (x - v2);
    v4 = (v4 / duration);
    v5 = (y - v3);
    v5 = (v5 / duration);
    v1 = duration;
    while ((v1 > 0)) {
        v2 = (v2 + v4);
        v3 = (v3 + v5);
        SetViewCamera((v2 / 1000), (v3 / 1000));
        if (((v2 / 100) == (x / 100)) && ((v3 / 100) == (y / 100))) {
            v1 = 1;
        }
        v1--;
        Delay(1);
    }
    return;
}

// 在object存在的时间内锁定摄像机在其上，yOffset指定摄像机y轴上的偏移。
// 例外情况：如果摄像机和object的x坐标刚好相差40像素，则摄像机不动。
void LockCameraSimple(int object, int yOffset)  {
    int v1;
    int v2;
    v2 = GetCameraX();
    while (IsObjectExist(object)) {
        v1 = GetObjectScreenX(object);
        if (((v1 - v2) > 40) || ((v1 - v2) < 40)) {
            SetViewCamera(v1, (GetObjectScreenY(object) + yOffset));
        }
        Delay(1);
        v2 = GetCameraX();
    }
    return;
}
// 在object存在的时间内锁定摄像机在其上，yOffset指定摄像机y轴上的偏移。
// 例外情况：如果摄像机和object的x坐标刚好相差10像素，则摄像机不动。
void LockCameraSimple2(int object, int yOffset)  {
    int v1;
    int v2;
    v2 = GetCameraX();
    while (IsObjectExist(object)) {
        v1 = GetObjectScreenX(object);
        if (((v1 - v2) > 10) || ((v1 - v2) < 10)) {
            SetViewCamera(v1, (GetObjectScreenY(object) + yOffset));
        }
        Delay(1);
        v2 = GetCameraX();
    }
    return;
}
// 让摄像机从指定物件（通常为我方主将）出发，以每帧8像素的速度向对方主将的方向横向平移，
// 直至离边界270像素或者时间到为止。
// 摄像机的初始位置与isLeft有关，为朝对方主将方向偏移100像素。
void LockCameraLine(int sourceObject, int yOffset, int isLeft, int duration)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    v5 = 0;
    v2 = GetCameraX();
    v3 = GetCameraY();
    if (!(isLeft == 1)) {
        v4 = (-100);
    } else {
        v4 = 100;
    }
    v1 = GetObjectScreenX(sourceObject);
    while (1) {
        if (!(isLeft == 1)) {
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
            SetViewCamera((v1 + v4), (v3 + yOffset));
        }
        Delay(1);
        v2 = GetCameraX();
        v5++;
        if (v5 > duration) {
            break;
        }
    }
    return;
}
// 让摄像机从指定物件（通常为我方主将）出发，以每帧8像素的速度向对方主将的方向横向平移，
// 直至超过对方主将或者时间到为止。
// 摄像机的初始位置与isLeft有关，为朝对方主将方向偏移100像素。
void LockCameraLineDelay(int sourceObject, int yOffset, int isLeft, int duration)  {
    int v1;
    int v2;
    int v3;
    int v4;
    v2 = GetCameraX();
    v3 = GetCameraY();
    if (!(isLeft == 1)) {
        v4 = (-100);
    } else {
        v4 = 100;
    } while (IsObjectExist(sourceObject)) {
        v1 = GetObjectScreenX(sourceObject);
        if (!(isLeft == 1)) {
            if (v1 < GetObjectScreenX(intvDefenderMajor)) {
                break;
            }
        } else {
            if (v1 > GetObjectScreenX(intvDefenderMajor)) {
                break;
            }
        }
        if (((v1 - v2) > 40) || ((v1 - v2) < 40)) {
            SetViewCamera((v1 + v4), (v3 + yOffset));
        }
        Delay(1);
        duration--;
        if (duration <= 0) {
            break;
        }
        v2 = GetCameraX();
    }
    return;
}
// 以某种“加速”逻辑，在物件存在时间内，将摄像机不停地移动以跟随该物件，
// 直到物件销毁或摄像机达到对方主将身后为止，此时再将摄像机移向对方主将。
// ----因此该物件通常应当是打击对方主将的。
// 需指定一个“参考”xDiff：如果摄像机X坐标离物件太远，则会强制调整摄像机到距离物件xDiff距离以内。
void LockCamera(int object, int xDiff, int yOffset, int isLeft)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    int v6;
    v6 = 0;
    v4 = GetObjectScreenX(object);
    v1 = 0;
    v3 = IsObjectExist(object);
    while ((v3 == 1)) {
        v2 = GetObjectScreenX(object);
        v4 = GetCameraX();
        if (!(isLeft == 0)) {
            if (!((v2 - v4) >= xDiff)) {
                v5 = v4;
            } else {
                v5 = (v2 - xDiff);
                v6 = (v6 + 1);
            }
        } else {
            if (!((v4 - v2) >= xDiff)) {
                v5 = v4;
            } else {
                v5 = (v2 + xDiff);
                v6 = (v6 - 1);
            }
        }
        SetViewCamera((v5 + v6), (GetObjectScreenY(object) + yOffset));
        Delay(1);
        v3 = IsObjectExist(object);
        if (!(v1 == 1)) {
            if (!(isLeft == 0)) {
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
        if (!(isLeft == 0)) {
            object = intvDefenderMajor;
        } else {
            object = intvDefenderMajor;
        }
        asynccall MoveCamera(GetObjectScreenX(object), (GetObjectScreenY(object) - 120), 30);
    }
    return;
}
// 以某种“加速”逻辑，在物件object存在时间内，将摄像机不停地移动以跟随该物件，
// 直到物件object销毁或摄像机达到对方主将身后，
// （在此期间需指定一个“参考”xDiff：如果摄像机X坐标离object太远，则会强制调整摄像机到距离objectxDiff距离以内）
// 然后，等待secondObject通过摄像机面前，继续锁定secondObject.
void LockCamera2(int object, int xDiff, int yOffset, int isLeft, int secondObject)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    int v6;
    int v7;
    int v8;
    v6 = 0;
    v4 = GetObjectScreenX(object);
    v1 = 0;
    v3 = IsObjectExist(object);
    while ((v3 == 1)) {
        v2 = GetObjectScreenX(object);
        v4 = GetCameraX();
        if (!(isLeft == 0)) {
            if (!((v2 - v4) >= xDiff)) {
                v5 = v4;
            } else {
                v5 = (v2 - xDiff);
                v6 = (v6 + 1);
            }
        } else {
            if (!((v4 - v2) >= xDiff)) {
                v5 = v4;
            } else {
                v5 = (v2 + xDiff);
                v6 = (v6 - 1);
            }
        }
        SetViewCamera((v5 + v6), (GetObjectScreenY(object) + yOffset));
        Delay(1);
        v3 = IsObjectExist(object);
        if (!(v1 == 1)) {
            if (!(isLeft == 0)) {
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
    while (IsObjectExist(secondObject)) {
        v2 = GetObjectScreenX(secondObject);
        if (!(isLeft == 1)) {
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
    v8 = (v8 - GetObjectScreenY(secondObject));
    if (IsObjectExist(secondObject)) {
        LockCameraSimple2(secondObject, v8);
    }
    return;
}
// 为目标士兵制造火人效果。火焰物件为10015时为橙色火焰，10016为紫色火焰。
void FireMan(int target, int fireObjectSequence, int duration)  {
    int v1;
    int v2;
    int v3;
    v2 = GetObjectDir(target);
    v3 = CreateObjectByReference(target, fireObjectSequence, v2, 0);
    v1 = 0;
    while ((v1 < duration)) {
        SetCoordinateByReference_Cylind(v3, target, 0, 0, 0);
        Delay(1);
        if (IsObjectExist(target) == 0) {
            break;
        }
        v1++;
    }
    SetObjectFadeOut(v3, 16, 1);
    return;
}
// 在target物件上制造单个喷出的血液效果，初始高度为zOffset，水平面上的方向为dir。
void CreateBlood(int target, int zOffset, int dir)  {
    int v1;
    v1 = CreateObjectByReference(target, 10012, 0, zOffset);
    SetObjectScale(v1, 32768, 32768);
    SetObjectSpeed_Sphere(v1, dir, Rand(24, 40), 7);
    Delay(2);
    SetObjectFadeOut(v1, 8, 1);
    return;
}
// 在target物件上制造连续喷出的血液效果（共抛出32个物件），初始高度为zOffset。
void Blood(int target, int zOffset)  {
    int v1;
    int v2;
    v2 = (128 * Rand(0, 1));
    v1 = 0;
    while ((v1 < 32)) {
        asynccall CreateBlood(target, zOffset, v2);
        Delay(Rand(0, 1));
        v1++;
    }
    return;
}
// 以下4个函数用于击中目标target时，生成“爆炸效果”中飞溅的小光球（碎屑），并播放主将被击中的声音。
// 1, 2号函数抛出橙黄色的小光球，2号函数的碎屑速度比1号要低一些，大小也要小一些。
// 3, 4号函数的小光球参数和1号函数相同，3号抛出紫色的小光球，4号抛出蓝青色的小光球。
// zOffset参数指定产生碎屑的高度，fireballCount指定生成的碎屑数量。
void SmallFireBall(int magicObject, int target, int fireballCount, int zOffset)  {
    int v1;
    int v2;
    PlaySound(target, "hit01", 255);
    v2 = 0;
    while ((v2 < fireballCount)) {
        v1 = CreateObjectByReference(target, 10003, Rand(0, 255), zOffset);
        SetObjectScale(v1, 12288, 12288);
        SetObjectSpeed_Cylind(v1, Rand(4, 12), Rand((-2), 8));
        SetObjectFriction1(v1, 8192);
        SetObjectFadeOut(v1, 32, 1);
        SetObjectScaleShrink(v1, 256);
        v2++;
    }
    return;
}
void SmallFireBall2(int magicObject, int target, int fireballCount, int zOffset)  {
    int v1;
    int v2;
    int v3;
    PlaySound(target, "hit01", 255);
    v2 = 0;
    while ((v2 < fireballCount)) {
        v1 = CreateObjectByReference(target, 10003, Rand(0, 255), zOffset);
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
void SmallFireBall3(int magicObject, int target, int fireballCount, int zOffset)  {
    int v1;
    int v2;
    PlaySound(target, "hit01", 255);
    v2 = 0;
    while ((v2 < fireballCount)) {
        v1 = CreateObjectByReference(target, 10013, Rand(0, 255), zOffset);
        SetObjectScale(v1, 12288, 12288);
        SetObjectSpeed_Cylind(v1, Rand(4, 12), Rand((-2), 8));
        SetObjectFriction1(v1, 8192);
        SetObjectFadeOut(v1, 32, 1);
        SetObjectScaleShrink(v1, 256);
        v2++;
    }
    return;
}
void SmallFireBall4(int magicObject, int target, int fireballCount, int zOffset)  {
    int v1;
    int v2;
    PlaySound(target, "hit01", 255);
    v2 = 0;
    while ((v2 < fireballCount)) {
        v1 = CreateObjectByReference(target, 10014, Rand(0, 255), zOffset);
        SetObjectScale(v1, 12288, 12288);
        SetObjectSpeed_Cylind(v1, Rand(4, 12), Rand((-2), 8));
        SetObjectFriction1(v1, 8192);
        SetObjectFadeOut(v1, 32, 1);
        SetObjectScaleShrink(v1, 256);
        v2++;
    }
    return;
}
// 天空变黑。
// 目标brightness以0为最暗，16为最亮，通常施放武将技时为12。
// timePerUnit指定下降每点brightness所用的时间（Tick）。
void DownBrightness(int targetBrightness, int timePerUnit)  {
    int v1;
    v1 = GetBackgroundBrightness();
    while ((v1 > targetBrightness)) {
        Delay(timePerUnit);
        v1--;
        SetBackgroundBrightness(v1);
    }
    SetBackgroundBrightness(16);
    LockBrightness(targetBrightness);
    SetGlobal(2, targetBrightness);
    return;
}
// 天空变白。
// 目标brightness以0为最暗，16为最亮，通常施放武将技时为12。
// timePerUnit指定下降每点brightness所用的时间（Tick）。
void RaiseBrightness(int targetBrightness, int timePerUnit)  {
    int v1;
    UnlockBrightness();
    v1 = GetGlobal(2);
    SetBackgroundBrightness(v1);
    while ((v1 < targetBrightness)) {
        Delay(timePerUnit);
        v1++;
        SetBackgroundBrightness(v1);
    }
    SetBackgroundBrightness(targetBrightness);
    return;
}
// 以下四个函数只被KeepPower函数直接或间接调用。
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
// 该函数似乎意在制造一个向外“释放”能量的动画效果。
// 不过，该函数在原版magic下从未被调用。
void PowerExplode(int a0, int a1)  {
    int v1;
    v1 = CreateObjectByReference(a0, 10011, 0, 0);
    SetObjectSpeed_Sphere(v1, 0, (a1 * 32), 2);
    CreateShadowObject(v1, 4, 2);
    Delay(5);
    SetObjectFadeOut(v1, 16, 1);
    return;
}
// 该函数似乎意在制造一个“集聚”能量的动画效果，
// 其中心是一个光球，有射线向该光球汇集。
// 从光球的高度判断，该函数应当只能被用于主将。
// 不过，该函数在原版magic下从未被调用。
void KeepPower(int major, int isLeft)  {
    int v1;
    int v2;
    if (!(isLeft == 1)) {
        isLeft = (-1);
    } else {
        isLeft = 1;
    }
    v1 = CreateObjectByReference(major, 10010, 0, 0);
    isLeft = (isLeft * 47);
    SetCoordinateByReference_Cylind(v1, major, 0, isLeft, 110);
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
// 制造渐入效果；第二个参数指定过多少 Tick 变化 1 点不透明度。
void StepShow(int object, int ticksPerOpacity)  {
    int v1;
    SetObjectFlags(object, OF_MIXER);
    SetObjectOpacity(object, 0);
    Delay(20);
    v1 = 1;
    while ((v1 <= 16)) {
        Delay(ticksPerOpacity);
        SetObjectOpacity(object, v1);
        v1++;
    }
    ClearObjectFlags(object, OF_MIXER);
    return;
}
// 创建击中的“爆炸效果”。
// 第一、第二个参数表示造成伤害的物件和被击中的主将；
// 第三个参数 effectSequence 表示爆炸效果物件在Things.ini中的原型的sequence，
//      该函数会自动创建多个这样的爆炸效果物件；
// 第四个参数 ballType 表示以何种方式创建爆炸产生的“小光球”，
//      填 1、2、3分别表示三种不同的小光球颜色（橙黄/紫色/蓝青），填 0 则表示不创建小光球。
// 第五个参数 zOffset 表示爆炸效果距地面的高度。
// 最后一个参数是爆炸效果拖延的“时间”，即该效果结束之后多少时间内新的HitGeneral函数造成的效果无效。
void HitGeneral(int magicObject, int target, int effectSequence, int ballType, int zOffset, int delayTime)  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    if (!GetGlobal(3)) {
        SetGlobal(3, 1);
        v2 = CreateObjectByReference(target, 2501, 0, 0);
        v3 = GetObjectScreenX(target);
        v4 = GetObjectScreenY(target);
        SetObjectCoordinate(v2, v3, v4, zOffset);
        v1 = 0;
        while ((v1 < 7)) {
            v5 = CreateObjectByReference(v2, effectSequence, 0, 0);
            SetObjectScale(v5, 49152, 49152);
            MoveObject(v5, Rand((-48), 48), 0, Rand((-48), 48));
            switch (ballType) {
            case 1:
                asynccall SmallFireBall(magicObject, target, 8, zOffset);
                break;
            case 2:
                asynccall SmallFireBall3(magicObject, target, 8, zOffset);
                break;
            case 3:
                asynccall SmallFireBall4(magicObject, target, 8, zOffset);
                break;
                break;
            }
            Delay(Rand(3, 7));
            v1++;
        }
        FreeObject(v2);
        Delay(delayTime);
        SetGlobal(3, 0);
        return;
    } else {
        return;
    }
}
// 在delayTime内取消object的所有打击目标标记，之后，设置object的标记为flags。
void DisableAttack(int object, int flags, int delayTime)  {
    ClearObjectFlags(object, OF_ATTACKALL);
    Delay(delayTime);
    SetObjectFlags(object, flags);
    return;
}
// 天降铁锅。
void CastFail(int major, int dir)  {
    int v1;
    v1 = CreateObjectByReference(major, 10017, dir, 256);
    SetObjectFlags(v1, OF_MYGENERAL);
    SetCallbackProcedure(v1, 1000);
    SetObjectAnimate(major, OAF_SPELL1);
    SetOverwhelming(major, 0);
    Delay(60);
    return;
}