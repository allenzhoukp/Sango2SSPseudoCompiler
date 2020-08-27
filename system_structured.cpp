// --------------------
// This is the adjusted original decompliled code from SYSTEM.SO.
// Notice: This file is encoded with Chinese-Traditional (Big5) charset.
//         Editing (and saving) this file under other charsets may cause ENCODING PROBLEMS!
//         ! IF YOU CAN'T SEE THE SENTENCE BELOW, change the charset of your editor to Big5 IMMEDIATELY !
//    Charset example: 如果這行字能正常顯示，那麼您使用的字符集是正確的。
// --------------------


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
    do {
        v2 = (v2 + v4);
        v3 = (v3 + v5);
        SetViewCamera((v2 / 1000), (v3 / 1000));
        if ((((v2 / 100) == (a0 / 100)) && ((v3 / 100) == (a1 / 100)))) {
            v1 = 1;
        }
        v1--;
        Delay(1);
    }
    while ((v1 > 0));
    return;
}
void CheckGround(int a0)  {
    return;
    while ((GetObjectScreenZ(a0) > 0)) {
	    Delay(1);
    }
	SetObjectStep_Cylind(a0, 0, 0);
	SetObjectScaleShrink(a0, 4096);
	SetObjectFadeOut(a0, 16, 1);
	return;
}
void WeatherObjectDeath() callsign 99  {
    int v1;
    v1 = GetScriptLinkedObject();
    SetObjectStep_Cylind(v1, 0, 0);
    SetObjectScaleShrink(v1, 4096);
    SetObjectFadeOut(v1, 16, 1);
    return;
}
void ProduceRain()  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    while ( 1 ) {
        if (!(1))
            break;
        v5 = Rand(3, 10);
        while ( 1 ) {
            v5--;
            if (!((v5 > 0)))
                break;
            v1 = Rand(0, 1024);
            v2 = Rand(0, 1024);
            v3 = Rand(300, 500);
            v4 = CreateObjectRaw(v1, v2, v3, 0, (Rand(0, 2) + 9994));
            SetObjectStep_Cylind(v4, Rand(2, 5), 0);
        }
        Delay(1);
    }
    return;
}
void ProduceSnow()  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    int v6;
    int v7;
    int v8;
    v7 = Rand(0, 255);
    v6 = Rand(1, 3);
    while ( 1 ) {
        if (!(1))
            break;
        v5 = Rand(3, 10);
        while ( 1 ) {
            v5--;
            if (!((v5 > 0)))
                break;
            v1 = Rand(0, 1024);
            v2 = Rand(0, 1024);
            v3 = Rand(300, 500);
            v4 = CreateObjectRaw(v1, v2, v3, 0, (Rand(0, 2) + 9994));
            SetObjectDrawingFileItem(v4, "MAGIC\\M013WingA6.SHP");
            SetObjectGravity(v4, Rand(256, 512));
            v8 = Rand(24576, 32768);
            SetObjectScale(v4, v8, v8);
            v6 = Rand(2, 4);
            SetObjectStep_Sphere(v4, (v7 + Rand((-10), 10)), 0, v6);
        }
        Delay(10);
    }
    return;
}
void ProduceFog()  {
    int v1;
    int v2;
    int v3;
    int v4;
    v1 = Rand(0, 1024);
    v2 = Rand(0, 1024);
    v3 = Rand(300, 500);
    v4 = CreateObjectRaw(v1, v2, v3, 0, (Rand(0, 2) + 9994));
    SetObjectDrawingFileItem(v4, "MENU\\GAMEOVER.SHP");
    return;
}
void Battle2() callsign 62  {
    return;
}
void EnterWar4() callsign 64  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    v1 = 6;
    while ( 1 ) {
        v1--;
        if (!((v1 > 0)))
            break;
        v3 = BattleYToScreenY(Rand(14, 17));
        v2 = BattleXToScreenX(Rand((-2), 3));
        v2 = (v2 + Rand(0, 96));
        v3 = (v3 + Rand(0, 72));
        v4 = CreateObjectRaw(v2, v3, 0, 0, 402);
        v5 = Rand(32768, 65536);
        SetObjectScale(v4, v5, v5);
        v3 = BattleYToScreenY(Rand(14, 17));
        v2 = BattleXToScreenX(Rand(4, 8));
        v2 = (v2 + Rand(0, 96));
        v3 = (v3 + Rand(0, 72));
        v4 = CreateObjectRaw(v2, v3, 0, 0, 402);
        v5 = Rand(32768, 65536);
        SetObjectScale(v4, v5, v5);
        v3 = BattleYToScreenY(Rand(14, 17));
        v2 = BattleXToScreenX(Rand(48, 53));
        v2 = (v2 + Rand(0, 96));
        v3 = (v3 + Rand(0, 72));
        v4 = CreateObjectRaw(v2, v3, 0, 0, 402);
        v5 = Rand(32768, 65536);
        SetObjectScale(v4, v5, v5);
        v3 = BattleYToScreenY(Rand(14, 17));
        v2 = BattleXToScreenX(Rand(54, 58));
        v2 = (v2 + Rand(0, 96));
        v3 = (v3 + Rand(0, 72));
        v4 = CreateObjectRaw(v2, v3, 0, 0, 402);
        v5 = Rand(32768, 65536);
        SetObjectScale(v4, v5, v5);
    }
    v1 = Rand(6, 9);
    while ( 1 ) {
        v1--;
        if (!((v1 > 0)))
            break;
        v3 = BattleYToScreenY(Rand(14, 17));
        v2 = BattleXToScreenX(Rand(9, 47));
        v2 = (v2 + Rand(0, 96));
        v3 = (v3 + Rand(0, 72));
        v4 = CreateObjectRaw(v2, v3, 0, 0, 402);
        v5 = Rand(32768, 65536);
        SetObjectScale(v4, v5, v5);
    }
    v1 = 6;
    while ( 1 ) {
        v1--;
        if (!((v1 > 0)))
            break;
        v3 = BattleYToScreenY(Rand(14, 17));
        v2 = BattleXToScreenX(Rand((-2), (-10)));
        v2 = (v2 + Rand(0, 96));
        v3 = (v3 + Rand(0, 72));
        v4 = CreateObjectRaw(v2, v3, 0, 0, 402);
        v5 = Rand(32768, 65536);
        SetObjectScale(v4, v5, v5);
        v3 = BattleYToScreenY(Rand(14, 17));
        v2 = BattleXToScreenX(Rand(58, 66));
        v2 = (v2 + Rand(0, 96));
        v3 = (v3 + Rand(0, 72));
        v4 = CreateObjectRaw(v2, v3, 0, 0, 402);
        v5 = Rand(32768, 65536);
        SetObjectScale(v4, v5, v5);
    }
    return;
}
void EnterWar5() callsign 65  {
    int v1;
    int v2;
    int v3;
    int v4;
    v1 = 0;
    while ( 1 ) {
        if (!((v1 < 11)))
            break;
        v3 = BattleYToScreenY(15);
        v2 = BattleXToScreenX((v1 * 7));
        v4 = CreateObjectRaw(v2, v3, 0, 0, 403);
        v4 = CreateObjectRaw(v2, v3, 70, 0, 404);
        v1++;
    }
    return;
}
void EnterWar7()  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    v1 = 0;
    while ( 1 ) {
        if (!((v1 < 60)))
            break;
        v3 = BattleYToScreenY(Rand(14, 17));
        v2 = BattleXToScreenX(Rand(0, 71));
        v2 = (v2 + Rand(0, 96));
        v3 = (v3 + Rand(0, 72));
        v4 = CreateObjectRaw(v2, v3, 0, 0, Rand(400, 401));
        v5 = Rand(40960, 65536);
        SetObjectScale(v4, v5, v5);
        v1++;
    }
    return;
}
void LoadingBattleMap(int a0) callsign 50  {
    switch (a0) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            PlaySound1("BG03A", 255);
            break;
        case 4:
            if ((IsCurrentEffectOn() == 1)) {
            EnterWar4();
            }
            break;
        case 5:
            PlaySound1("BG04A", 255);
            if ((IsCurrentEffectOn() == 1)) {
                EnterWar5();
            }
            break;
        case 6:
            break;
        case 7:
            if ((IsCurrentEffectOn() == 1)) {
                EnterWar7();
            }
            break;
        case 8:
            PlaySound1("BG08A", 255);
            if ((IsCurrentEffectOn() == 1)) {
                ProduceSnow();
            }
            break;
        case 9:
            PlaySound1("BG09A", 255);
            break;
        case 10:
            break;
        case 11:
            break;
        default:
            break;
    }
    return;
}
void BattleYield(int a0, int a1, int a2, int a3, int a4, int a5, int a6)  {
    int v1;
    int v2;
    int v3;
    int v4;
    v1 = GetObjectScreenX(a0);
    v2 = GetObjectScreenY(a0);
    v3 = GetObjectScreenX(a1);
    v4 = GetObjectScreenY(a1);
    MoveCamera(v1, (v2 - 128), 60);
    Prompt1(a4, 0, "%S2%將軍，請加入我方，共創統一大業!!%k%%e%");
    MoveCamera(v3, (v4 - 128), 60);
    Prompt1(a5, 0, "就讓我們共創統一大業吧!!%k%%e%");
    MoveCamera(v1, (v2 - 128), 60);
    Prompt1(a4, 0, "將軍真是明智之舉!!%k%%e%");
    SetBattleResult(a6);
    AllowBattleOperation(0);
    return;
}
void StartBattle(int a0) callsign 51  {
    int v3;
    int v4;
    int v5;
    int v6;
    int v7;
    int v8;
    int v9;
    SetSXString(3, 1, GetString(intvRightGeneral));
    SetSXString(2, 1, GetString(intvLeftGeneral));
    SetSXString(3, 2, GetString(intvLeftGeneral));
    SetSXString(2, 2, GetString(intvRightGeneral));
    if (!((a0 == 1))) {
        if (!((a0 == (-1)))) {
            v6 = GetGeneralType(intvRightGeneral);
            v5 = GetGeneralType(intvLeftGeneral);
            v8 = GetObjectScreenX(intvDefenderMajor);
            v9 = GetObjectScreenY(intvDefenderMajor);
            MoveCamera(v8, (v9 - 128), 30);
            switch (v5) {
                case 0:
                    if (!((Rand(0, 1) == 0))) {
                        v3 = Rand(380, 399);
                    } else {
                        v3 = Rand(300, 319);
                    }
                    break;
                case 1:
                    if (!((Rand(0, 1) == 0))) {
                        v3 = Rand(380, 399);
                    } else {
                        v3 = Rand(320, 339);
                    }
                    break;
                case 2:
                    if (!((v6 == 0))) {
                        if (!((Rand(0, 1) == 0))) {
                            v3 = Rand(380, 399);
                        } else {
                            v3 = Rand(340, 359);
                        }
                    } else {
                        v7 = Rand(0, 2);
                        if (!((v7 == 0))) {
                            if (!((v7 == 1))) {
                                v3 = Rand(360, 379);
                            } else {
                                v3 = Rand(380, 399);
                            }
                        } else {
                            v3 = Rand(340, 359);
                        }
                    }
                    break;
                default:
                    break;
            }
            PromptWithMessageIndex1(2, 0, v3);
            v8 = GetObjectScreenX(intvAttackerMajor);
            v9 = GetObjectScreenY(intvAttackerMajor);
            MoveCamera(v8, (v9 - 128), 60);
            do {
                switch (v6) {
                    case 0:
                        v7 = Rand(0, 2);
                        if (!((v7 == 0))) {
                            if (!((v7 == 1))) {
                                v4 = Rand(400, 409);
                            } else {
                                v4 = Rand(380, 399);
                            }
                        } else {
                            v4 = Rand(300, 319);
                        }
                        break;
                    case 1:
                        v7 = Rand(0, 2);
                        if (!((v7 == 0))) {
                            if (!((v7 == 1))) {
                                v4 = Rand(400, 409);
                            } else {
                                v4 = Rand(380, 399);
                            }
                        } else {
                            v4 = Rand(320, 339);
                        }
                        break;
                    case 2:
                        if (!((v5 == 0))) {
                            v7 = Rand(0, 2);
                            if (!((v7 == 0))) {
                                if (!((v7 == 1))) {
                                    v4 = Rand(400, 409);
                                } else {
                                    v4 = Rand(380, 399);
                                }
                            } else {
                                v4 = Rand(340, 359);
                            }
                        } else {
                            v7 = Rand(0, 3);
                            if (!((v7 == 0))) {
                                if (!((v7 == 1))) {
                                    if (!((v7 == 2))) {
                                        v4 = Rand(400, 409);
                                    } else {
                                        v4 = Rand(360, 379);
                                    }
                                } else {
                                    v4 = Rand(380, 399);
                                }
                            } else {
                                v4 = Rand(340, 359);
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
            while ((v4 == v3));
            PromptWithMessageIndex1(3, 0, v4);
            AllowBattleOperation(1);
            return;
        } else {
            BattleYield(intvAttackerMajor, intvDefenderMajor, intvRightGeneral, intvLeftGeneral, 3, 2, 0);
            return;
        }
    } else {
        BattleYield(intvDefenderMajor, intvAttackerMajor, intvLeftGeneral, intvRightGeneral, 2, 3, 1);
        return;
    }
}
void AllRush() callsign 59  {
    int v1;
    int v2;
    int v3;
    DisablePlayMagic();
    SetAttackCounterTo9999();
    Wait("MoveCamera");
    v1 = GetObjectScreenX(intvAttackerMajor);
    v2 = GetObjectScreenY(intvAttackerMajor);
    SetObjectAnimate(intvAttackerMajor, 65536);
    MoveCamera(v1, (v2 - 128), 20);
    RestoreAttackCounter();
    v3 = (155 + (GetSoldierCount(intvIsLeft) / 2));
    if ((v3 > 255)) {
        v3 = 255;
    }
    PlaySound1("Rush01", v3);
    Prompt1((3 - intvIsLeft), 0, "全軍突擊!%k%%e%");
    Delay(44);
    EnablePlayMagic();
    return;
}
void BattleEndTalk(int a0) callsign 56  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v6;
    int v7;
    v4 = GetGeneralType(intvRightGeneral);
    v3 = GetGeneralType(intvLeftGeneral);
    switch (a0) {
        case 1:
            v1 = 2;
            v6 = GetString(intvRightGeneral);
            v7 = GetString(intvLeftGeneral);
            v2 = 1;
            if (((v3 == 2) && (v4 == 0))) {
                v2 = 3;
            }
            break;
        case 2:
            v1 = 3;
            v7 = GetString(intvRightGeneral);
            v6 = GetString(intvLeftGeneral);
            v2 = 1;
            if (((v4 == 2) && (v3 == 0))) {
                v2 = 3;
            }
            break;
        case 256:
            v2 = (-1);
            break;
        case 512:
            v2 = (-1);
            break;
        case 4:
            v1 = 2;
            v2 = 2;
            v6 = GetString(intvRightGeneral);
            v7 = GetString(intvLeftGeneral);
            break;
        case 8:
            v1 = 3;
            v2 = 2;
            v7 = GetString(intvRightGeneral);
            v6 = GetString(intvLeftGeneral);
            break;
        case 16:
            v1 = 3;
            v2 = 0;
            v7 = GetString(intvRightGeneral);
            v6 = GetString(intvLeftGeneral);
            break;
        default:
            break;
    }
    SetSXString(v1, 1, v7);
    SetSXString(v1, 2, v6);
    switch (v2) {
        case 0:
            PromptWithMessageIndex1(v1, 0, Rand(270, 290));
            break;
        case 1:
            PromptWithMessageIndex1(v1, 0, Rand(500, 568));
            break;
        case 2:
            PromptWithMessageIndex1(v1, 0, Rand(600, 705));
            break;
        case 3:
            PromptWithMessageIndex1(v1, 0, Rand(240, 263));
            break;
        default:
            break;
    }
    EndBattle();
    return;
}
void ReportResult(int a0) callsign 6  {
    Delay(1);
    SetSXString(0, 1, GetString(intvRightGeneral));
    SetSXString(0, 2, GetString(intvLeftGeneral));
    SetSXString(0, 3, GetString(intvAttackerKingGeneral));
    switch (a0) {
        case 1:
            Prompt1(0, intvAttackerKingGeneral, "很遺憾，我軍戰敗了....%K%%E%");
            break;
        case 2:
            Prompt1(0, intvRightGeneral, "我軍勝利!!%K%%E%");
            break;
        case 4:
            Prompt1(0, intvAttackerKingGeneral, "我軍戰敗，%S1%將軍被俘了....%K%%E%");
            break;
        case 8:
            Prompt1(0, intvRightGeneral, "我軍大勝!俘虜敵將%S2%!!%K%%E%");
            break;
        case 256:
            Prompt1(0, intvAttackerKingGeneral, "%S1%將軍投靠敵方了....%K%%E%");
            break;
        case 512:
            Prompt1(0, intvRightGeneral, "說服%S2%將軍加入我方!!%K%%E%");
            break;
        case 16:
            Prompt1(0, intvRightGeneral, "雙方平手....%K%%E%");
            break;
        default:
            Prompt1(0, intvRightGeneral, "結果不明.......%K%%E%");
            break;
    }
    return;
}
void SearchTalk(int a0) callsign 5  {
    while ( 1 ) {
        if (!((GetGlobal(1) != 0)))
            break;
        Delay(1);
    }
    SetGlobal(1, 1);
    SetSXString(0, 1, GetString(intvRightGeneral));
    SetSXString(0, 3, GetString(intvAttackerKingGeneral));
    switch (a0) {
        case 0:
            Prompt1(0, intvRightGeneral, "什麼都找不到....%k%%E%");
            break;
        case 1:
            Prompt1(0, intvRightGeneral, "找到物品!!%k%%E%");
            break;
        case 16:
            if ((intvRightGeneral != intvLeftGeneral)) {
                PromptWithMessageIndex1(0, intvRightGeneral, 800);
            }
            PromptWithMessageIndex1(0, intvLeftGeneral, Rand(801, 809));
            PromptWithMessageIndex1(1, intvAttackerKingGeneral, 810);
            PlaySound1("SUCCESS", 255);
            PromptWithMessageIndex1(0, intvLeftGeneral, 819);
            Prompt1(1, intvAttackerKingGeneral, "%e%");
            if (!((intvRightGeneral != intvLeftGeneral))) {
                Prompt1(0, intvLeftGeneral, "%e%");
            } else {
                PromptWithMessageIndex1(0, intvLeftGeneral, 812);
                PromptWithMessageIndex1(0, intvRightGeneral, 813);
            }
            break;
        case 17:
            if ((intvRightGeneral != intvLeftGeneral)) {
                PromptWithMessageIndex1(0, intvRightGeneral, 800);
            }
            PromptWithMessageIndex1(1, intvAttackerKingGeneral, Rand(820, 826));
            PromptWithMessageIndex1(0, intvLeftGeneral, Rand(830, 840));
            PlaySound1("SUCCESS", 255);
            PromptWithMessageIndex1(1, intvAttackerKingGeneral, 811);
            Prompt1(1, intvAttackerKingGeneral, "%e%");
            if (!((intvRightGeneral != intvLeftGeneral))) {
                Prompt1(0, intvLeftGeneral, "%e%");
            } else {
                PromptWithMessageIndex1(0, intvLeftGeneral, 812);
                PromptWithMessageIndex1(0, intvRightGeneral, 813);
            }
            break;
        case 18:
            if ((intvRightGeneral != intvLeftGeneral)) {
                PromptWithMessageIndex1(0, intvRightGeneral, 800);
            }
            PromptWithMessageIndex1(0, intvLeftGeneral, Rand(801, 809));
            PromptWithMessageIndex1(1, intvAttackerKingGeneral, Rand(851, 861));
            PlaySound1("FAILURE2", 255);
            PromptWithMessageIndex1(0, intvLeftGeneral, 814);
            Prompt1(1, intvAttackerKingGeneral, "%e%");
            break;
        case 19:
            if ((intvRightGeneral != intvLeftGeneral)) {
                PromptWithMessageIndex1(0, intvRightGeneral, 800);
            }
            PromptWithMessageIndex1(0, intvLeftGeneral, Rand(801, 809));
            PromptWithMessageIndex1(1, intvAttackerKingGeneral, Rand(850, 860));
            PlaySound1("FAILURE2", 255);
            PromptWithMessageIndex1(0, intvLeftGeneral, 815);
            Prompt1(1, intvAttackerKingGeneral, "%e%");
            break;
        case 20:
            if (!((intvRightGeneral == intvLeftGeneral))) {
                PromptWithMessageIndex1(0, intvRightGeneral, 817);
                PromptWithMessageIndex1(1, intvAttackerKingGeneral, 818);
                PlaySound1("SUCCESS", 255);
                PromptWithMessageIndex1(0, intvLeftGeneral, Rand(894, 899));
                PromptWithMessageIndex1(1, intvAttackerKingGeneral, Rand(900, 903));
                PromptWithMessageIndex1(0, intvLeftGeneral, 812);
                PromptWithMessageIndex1(0, intvRightGeneral, Rand(904, 907));
            } else {
                PromptWithMessageIndex1(0, intvRightGeneral, 816);
                PromptWithMessageIndex1(1, intvAttackerKingGeneral, Rand(870, 874));
                PlaySound1("SUCCESS", 255);
                PromptWithMessageIndex1(0, intvLeftGeneral, Rand(875, 878));
                PromptWithMessageIndex1(1, intvAttackerKingGeneral, Rand(890, 893));
                Prompt1(0, intvLeftGeneral, "%e%");
            }
            break;
        default:
            break;
    }
    SetGlobal(1, 0);
    return;
}
void KillGeneral() callsign 70  {
    int v1;
    v1 = Rand(0, 10);
    PromptWithMessageIndex1(0, intvRightGeneral, (910 + (v1 * 2)));
    PromptWithMessageIndex1(1, intvLeftGeneral, ((910 + (v1 * 2)) + 1));
    Prompt1(0, intvRightGeneral, "%E%");
    Delay(20);
    KillGeneral_Perform();
    return;
}
void FreeGeneral() callsign 71  {
    PromptWithMessageIndex1(0, intvRightGeneral, Rand(940, 945));
    Delay(20);
    FreeGeneral_Perform();
    return;
}
void YieldGeneral(int a0, int a1) callsign 72  {
    SetSXString(1, 2, GetString(intvLeftGeneral));
    SetSXString(0, 2, GetString(intvLeftGeneral));
    switch (a0) {
        case 1:
            PromptWithMessageIndex1(0, intvRightGeneral, Rand(950, 966));
            PlaySound1("SUCCESS", 255);
            PromptWithMessageIndex1(1, intvLeftGeneral, Rand(980, 988));
            Prompt1(0, intvRightGeneral, "%E%");
            break;
        case 0:
            PromptWithMessageIndex1(0, intvRightGeneral, Rand(950, 966));
            PromptWithMessageIndex1(1, intvLeftGeneral, Rand(970, 978));
            Prompt1(0, intvRightGeneral, "%E%");
            break;
        default:
            break;
    }
    Delay(20);
    YieldGeneral_Perform();
    return;
}
void DownBrightness(int a0, int a1)  {
    int v1;
    v1 = GetBackgroundBrightness();
    while ( 1 ) {
        if (!((v1 > a0)))
            break;
        Delay(a1);
        v1--;
        SetBackgroundBrightness(v1);
    }
    SetBackgroundBrightness(a0);
    return;
}
void RaiseBrightness(int a0, int a1)  {
    int v1;
    v1 = GetBackgroundBrightness();
    while ( 1 ) {
        if (!((v1 < a0)))
            break;
        Delay(a1);
        v1++;
        SetBackgroundBrightness(v1);
    }
    SetBackgroundBrightness(a0);
    return;
}
void Test01() callsign 98  {
    DoHarmToMajor(intvAttackerMajor, 0, 10000);
    return;
}
void EnterMapMoveCamera(int a0, int a1, int a2) callsign 58  {
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    int v6;
    int v7;
    if ((GetGlobal(2) == 0)) {
        SetGlobal(2, 1);
        v1 = GetBigmapStartX();
        v2 = GetBigmapStartY();
        do {
            v5 = (a0 - v1);
            v6 = (a1 - v2);
            v7 = v5;
            if ((v7 < 0)) {
                v7 = (-v7);
            }
            if (!(((v7 / 2) < a2))) {
                if (!((v5 > 0))) {
                    v3 = (-a2);
                } else {
                    v3 = a2;
                }
            } else {
                v3 = (v5 / 2);
            }
            v7 = v6;
            if ((v7 < 0)) {
                v7 = (-v7);
            }
            if (!(((v7 / 2) < a2))) {
                if (!((v6 > 0))) {
                    v4 = (-a2);
                } else {
                    v4 = a2;
                }
            } else {
                v4 = (v6 / 2);
            }
            v1 = (v1 + v3);
            v2 = (v2 + v4);
            SetBigmapCamera(v1, v2);
            Delay(1);
        } while (((v3 != 0) || (v4 != 0)));
        SetGlobal(2, 0);
    }
    return;
}


void MajorDeath(int a0, int a1) callsign 57 {
	int v1;
	int v2;
	PlaySound(a0, "Yell03", 255);
	PlaySound(a0, "Horse02", 255);
	Wait("MoveCamera");
	v1 = GetObjectScreenX(a0);
	v2 = GetObjectScreenY(a0);
	MoveCamera(v1, (v2 - 128), 20);
	Delay(10);
	if (a1 != 0){
	    PlaySound1("VICTORY", 255);
    }
	return;
}

void EnterWarprep() callsign 54 {
	BatchLoadShape("MENU\\D0_*");
	BatchLoadShape("MENU\\D1_2A");
	BatchLoadShape("MENU\\D1_2B");
	BatchLoadShape("MENU\\D12*");
	BatchLoadShape("MENU\\WARPREP*");
	return;
}

void InitializeBattle() {
	int v1;
	if (GetGlobal(120) == 0){
        SetGlobal(120, 501);
    }
	SetGlobal(3, 0);
	SetGlobal(1, 0);
	SetGlobal(28 + 0, 0);
	SetGlobal(28 + 1, 0);
	SetGlobal(35 + 0, 0);
	SetGlobal(35 + 1, 0);
	v1 = 0;
    while(v1 < 60) {
        SetGlobal(55 + v1, 0);
        v1++;
    }
	return;
}

void EnterBattle() callsign 55 {
	asynccall InitializeBattle();
	return;
}

void EnterMapmode() callsign 53 {
	SetGlobal(2, 0);
	BatchLoadShape("MENU\\D3*");
	BatchLoadShape("MENU\\D4*");
	BatchLoadShape("MENU\\D28");
	BatchLoadShape("MENU\\D1_2A");
	BatchLoadShape("MENU\\D1_2B");
	BatchLoadShape("MENU\\D12*");
	return;
}

void EnterPolicy() callsign 52 {
	BatchLoadShape("MENU\\S0*");
	BatchLoadShape("MENU\\C3_B*");
	BatchLoadShape("MENU\\D1*");
	return;
}

