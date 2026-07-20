#include "magic.cpp"

//////////////////// 新武將技：詐敗
void FeignDefeat() callsign 800 {
    DisablePlayMagic();
    DownBrightness(12, 5);
    SetOverwhelming(intvAttackerMajor, 1);
    int x = GetObjectScreenX(intvAttackerMajor);
    int y = GetObjectScreenY(intvAttackerMajor);
    SetViewCamera(x, (y - 120));    //  指定屏幕坐標，將攝像机對准武將
    AddAttackCounter(intvAttackerMajor, 2);
    Delay(15);
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL2); // 主將把武器一招
    Delay(20);
	
	Prompt(3, 0, "賊軍勢大，且暫避風頭！%k%%e%");
	Delay(1);

    SetOverwhelming(intvAttackerMajor, 0);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
}


//////////////////// 新武將技：大喝
// 入口函數，調用代碼801
void Roar() {
    DisablePlayMagic();     // 禁止雙方武將發武將技
    DownBrightness(12, 5);  // 天空變黑
    
    SetOverwhelming(intvAttackerMajor, 1);  // 設置武將為無敵狀態
    int x = GetObjectScreenX(intvAttackerMajor);  // 得到武將屏幕x坐標
    int y = GetObjectScreenY(intvAttackerMajor);  // 得到武將屏幕y坐標
    SetViewCamera(x, (y - 120));    //  指定屏幕坐標，將攝像机對准武將
    
    AddAttackCounter(intvAttackerMajor, 2); // 武將定身2個定身時間單位
    Delay(15);                      // 腳本暫停15 Tick
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL1); // 主將把武器一招
    Delay(20);                      // 腳本暫停20 Tick
    // 武將技准備部分結束
    
    // 大喝一聲，需判斷性別
		PlaySound1("Roar01", 255);
    
    // 遍歷主將附近的所有九個格子，造成殺傷
    int majorBattleX = GetObjectBattleX(intvAttackerMajor);
    int majorBattleY = GetObjectBattleY(intvAttackerMajor);
    for (int i = majorBattleX - 1; i <= majorBattleX + 1; i++) {
        for (int j = majorBattleY - 1; j <= majorBattleY + 1; j++) {
            // 根据給定戰場坐標獲取士兵/主將
            int target = GetForceHandle(i, j);
            
            // 是不是自己人？是則直接跳過
            if (GetForceSide(target) == intvIsLeft)
                continue;
            
            // 是不是對方主將？
            if (target == intvDefenderMajor) {
                // 是：制造傷害
                DoHarmToMajor(target, 
                    0, intvMagicAttackValue);
            } else {
                // 不是：殺死士兵
                KillForce(target);
            }
        }
    }
    
    // 動畫效果：創建光气環
    int circle = 
        CreateObjectByReference(intvAttackerMajor, 18002, 0, 40);
    
    // 手動擴散光气環。注意，由于上一段已經定義過i，所以此處不再定義
    for (i = 0; i < 20; i++) { 
        Delay(1);
        
        // 每Tick放大3/16
        int scale = 0x10000 + 0x10000 * 3 * i / 16; 
        SetObjectScale(circle, scale, scale);
        
        // 每Tick不透明度下降1點
        if (16 - i >= 0)
            SetObjectOpacity(circle, 16 - i); 
    }
    FreeObject(circle);     // 銷毀光气環
    
    // 武將技主体部分結束。拖一會儿時間，顯得自然一點
    Delay(10);
    
    // 武將技結束部分
    SetOverwhelming(intvAttackerMajor, 0);  // 結束武將無敵狀態
    
    RaiseBrightness(16, 5);     // 天空漸漸變白
    EnablePlayMagic();          // 允許雙方武將發武將技
}

void Magic112() callsign 801 {
    Roar();  // 調用實際執行的武將技函數
}


//////////////////// 新武將技：前后伏兵
// 指定士兵在16 Tick內漸入
void MyStepShow(int object) {
    SetObjectFlags(object, OF_MIXER);
    SetObjectOpacity(object, 0);
    Delay(20);
    for (int i = 1; i <= 16; i++) {
        Delay(1);
        SetObjectOpacity(object, i);
    }
    ClearObjectFlags(object, OF_MIXER);
}

// 在左側或右側生成soldierCount個士兵
void CreateSoldierFrontBack(int soldierCount, int onLeft) {
    PlaySound1("m002snd01", 255);
    PlaySound1("m002snd03", 255);
    
    // 初始化坐標：底線中點
    int y = GetBattleHeight() / 2;
    int x;
    if (onLeft) x = 0;
    else x = GetBattleWidth() - 1;
    
    // 循環：招完所有兵為止
    int soldier, existing;
    int opposingY;
    while(soldierCount > 0) {
        // 檢查戰場上半部分的位置
        existing = GetForceHandle(x, y);
        if (existing == 0) {
            soldier = CreateSoldier(intvIsLeft, x, y);
            asynccall MyStepShow(soldier);
            soldierCount--;
        }
        // 檢查是否已招完兵
        if (soldierCount <= 0)
            break;
            
        // 檢查對稱的下半部分的位置
        opposingY = GetBattleHeight() - 1 - y;
        existing = GetForceHandle(x, opposingY);
        if (existing == 0) {
            soldier = CreateSoldier(intvIsLeft, x, opposingY);
            asynccall MyStepShow(soldier);
            soldierCount--;
        }
        
        // 調整(x, y)位置：如果填滿就換列，否則同列向上下方順延
        if (opposingY == 1) {
            if (onLeft) x++;
            else x--;
            y = GetBattleHeight() / 2;
        } else {
            y++;
        }
    }
}

// 主函數
void MoreSoldierFrontBack() {
    DisablePlayMagic();
    DownBrightness(12, 5);

	// 前伏兵和後伏兵的兵數
	int backSolCount = intvMagicAttackValue / 2;
	int frontSolCount = intvMagicAttackValue - backSolCount;
    
    SetOverwhelming(intvAttackerMajor, 1);
    int x = GetObjectScreenX(intvAttackerMajor);
    int y = GetObjectScreenY(intvAttackerMajor);
    SetViewCamera(x, y - 120); //攝像機對準武將
    
    AddAttackCounter(intvAttackerMajor, 2);
    Delay(15);
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL2); // 主將把武器一招
    Delay(20);
    SetOverwhelming(intvAttackerMajor, 0);
    
    // 得到允許的招兵數
    int level = GetMajorLevel(intvIsLeft);
    if (level > 40) level = 40;
    int soldiersAllowed = level * 5 - GetSoldierCount(intvIsLeft);
    
    if (soldiersAllowed <= 0) {
        // 滿兵：召喚鐵鍋
        CastFail(intvAttackerMajor, GetObjectDir(intvAttackerMajor));
        
    } else {
        // 無法補足兵：按比例分配前後伏兵
        int total = frontSolCount + backSolCount;
        if (soldiersAllowed < frontSolCount + backSolCount) {
            frontSolCount = soldiersAllowed * frontSolCount / total;
            backSolCount = soldiersAllowed - frontSolCount;
        }
    
        if (intvIsLeft) {
            CreateSoldierFrontBack(frontSolCount, 1); // 先召喚左伏兵
            MoveCamera(240, 0, 25);  // 攝像機移過去
            Delay(80);             // 等一會兒
            
            CreateSoldierFrontBack(backSolCount, 0); // 再召喚右伏兵
            MoveCamera(GetBattleWidthInScreenX() - 240, 0, 25); 
            Delay(80);
            
        } else {
            // 類似
            CreateSoldierFrontBack(frontSolCount, 0);
            MoveCamera(GetBattleWidthInScreenX() - 240, 0, 25);
            Delay(80);
            
            CreateSoldierFrontBack(backSolCount, 1);
            MoveCamera(240, 0, 25);
            Delay(80);
        }
        MoveCamera(x, y - 120, 25); //攝像機移回武將
    }
    
    RaiseBrightness(16, 5);
    EnablePlayMagic();
}

// 入口函數
void Magic113() callsign 802 {
    MoreSoldierFrontBack();  // 調用實際執行的武將技函數
}


//////////////////// 新武將技：毒箭
// 中毒效果
void PoisonHarm(int target, int attackerMajor, 
        int attackValue, int interval, int times) {
    while(times > 0) {
        Delay(interval);
        
        // 造成傷害，添加武將被擊中的動作和聲音
        DoHarmToMajor(target, 0, attackValue);
        SetObjectAnimate(target, OAF_PAIN);
        PlaySound(target, "Yell02", 180);
        
        // 制造中毒特效動畫
        int effect = CreateObjectByReference(target, 13102, 0, 0);
        SetCoordinateByReference_Cylind(effect, target,
            GetObjectDir(target), -10, 70);
        SetObjectFadeOut(effect, 12, 1);
        
        times--;
    }
}

// 擊中主將的回調函數
void PoisonArrowCallback(int arrow, int target) callsign 11401{
    if (target == intvDefenderMajor) {
        // 銷毀箭支和光芒
        SetObjectFadeOut(arrow, 2, 1);
        // 利用GetObjectContext，獲取物件額外參數
        int light = GetObjectContext(arrow, 0);
        SetObjectFadeOut(light, 2, 1);
        
        // 第一擊對主將造成大約三分之一傷害，后續分5次中毒傷害
		int attack_2 = intvMagicAttackValue * 2 / 3 / 5;
		int attack_1 = intvMagicAttackValue - attack_2 * 5;
        Hurt(arrow, intvDefenderMajor, attack_1);
        
        // 運行中毒效果，持續造成中毒傷害
        asynccall PoisonHarm(target, intvAttackerMajor,
            attack_2, 120, 5);
        
        // 創建擊中效果
        HitGeneral(arrow, target, 13102, 1, 60, 0);
        
    }
}

// 鏡頭鎖定物件
void LockCameraOnObject(int object, int yOffset) {
    int dir = GetObjectDir(object);
    while(IsObjectExist(object)) {
        // 得到欲鎖定物件和攝像机自身的x坐標
        int objectX = GetObjectScreenX(object);
        int cameraX = GetCameraX();
        
        // 檢查邊界：如果飛出戰場，則直接退出
        if (cameraX < 0 || cameraX >= GetBattleWidthInScreenX())
            break;
            
        // 檢查物件与攝像机的距离
        int dx = objectX - cameraX;
        // 如果物件正在飛离攝像机，則移動攝像机，否則什么也不做
        if ((dir == 0 && dx > 0) || (dir == 128 && dx < 0))
            SetCameraByReference(object, 0, yOffset);
        Delay(1);
    }
}

// 主函數
void ShootPoisonArrow() {
    DisablePlayMagic();
    DownBrightness(8, 5);
    SetOverwhelming(intvAttackerMajor, 1);
    
    // 設置初始攝像机位置；向射箭方向的反方向偏移180像素
    if (intvIsLeft)
        SetCameraByReference(intvAttackerMajor, 180, -120);
    else
        SetCameraByReference(intvAttackerMajor, -180, -120);
    Delay(15);
    SetOverwhelming(intvAttackerMajor, 0);
    
    // 主將拈弓
    SetObjectAnimate(intvAttackerMajor, OAF_SHOT1);
    Delay(15);
    PlaySound(intvAttackerMajor, "m003snd03", 255);
    
    // 創建箭支及其光芒，并將其射出
    int dir = GetObjectDir(intvAttackerMajor);
    int arrow = CreateObjectByReference(intvAttackerMajor,
        13001, dir, 0);
    SetCoordinateByReference_Cylind(arrow, intvAttackerMajor, 
        dir, 80, 80);
    int light = CreateObjectByReference(arrow, 13101, dir, 0);
    SetObjectSpeed_Cylind(arrow, 16, 0);
    SetObjectSpeed_Cylind(light, 16, 0);
    
    // 鏡頭鎖定箭支
    asynccall LockCameraOnObject(arrow, -120);
    
    // 箭支加上打擊目標
    SetObjectFlags(arrow, OF_ENEMYGENERAL);
    // 設置回調函數
    SetCallbackProcedure(arrow, 11401);
    // 設置箭支物件的0號額外參數為光芒物件（以在擊中目標時銷毀）
    SetObjectContext(arrow, 0, light);
    
    // 等待LockCameraOnObject函數退出
    Wait("LockCameraOnObject");
    Delay(60);
    
    // 武將技結束
    RaiseBrightness(16, 5);
    EnablePlayMagic();
}

// 入口函數
void Magic114() callsign 803 {
    ShootPoisonArrow();
}


//////////////////// 新武將技：地火、地焰涌、地焰星燃
// 使用全局變量：201，確保對主將傷害遞減
// 回調函數
void GroundFireCallback(int object, int target) callsign 11501 {
    if (target == intvDefenderMajor) {
        // 擊中主將：從45號全局變量中獲取單發傷害，
        // 主將每被燙一次傷害減半，且一個物件只能對主將造成一次傷害
        int harm = GetGlobal(201);
        if (harm > 0) {
            DoHarmToMajor(target, 0, harm);
            SetGlobal(201, harm / 2);
            ClearObjectFlags(object, OF_ENEMYGENERAL);
            HitGeneral(object, target, 11002, 1, 48, 0);
        }
        
    } else {
        // 擊中士兵，士兵著火而死
        asynccall FireMan(target, 10015, 60);
    }
}

// 銷毀落地的熔岩彈，對第一顆熔岩彈創建岩漿池和爆炸效果
void TraceLavaSplit(int centerX, int centerY, int lavaSplit, int splitNo) {

    // 等待熔岩彈落地
    do {
        Delay(1);
    } while(GetObjectScreenZ(lavaSplit) > 0);
    
    // 獲取熔岩彈落地坐標，并銷毀
    int x = GetObjectScreenX(lavaSplit);
    int y = GetObjectScreenY(lavaSplit);
    FreeObject(lavaSplit);
    
    // 如果不是第一顆熔岩彈，則工作結束，直接返回
    if (splitNo != 0) return;
    
    // 創建地板上的火焰并縮放
    int fire = CreateObjectRaw(x, y, 0, 0, 11541);
    SetObjectScale(fire, 0x10000 / 3, 0x10000 / 6);
    
    // 地面上的火焰制造殺傷效果，并設置回調函數
    SetObjectFlags(fire, OF_ATTACKENEMY);
    SetCallbackProcedure(fire, 11501);
    
    // 創建熔岩池；隨机一個熔岩池編號，熔岩池隨熔岩彈落地而逐步擴大
    int poolNo = Rand(1, 6);
    int pool = CreateObjectRaw(x, y, 0, 0, 11520 + poolNo);
    int poollava = CreateObjectRaw(x, y, 0, 0, 11530 + poolNo);
    for (int i = 0; i < 6; i++) {
        SetObjectScale(pool, 0x10000 / 10 * i, 0x10000 / 10 * i);
        SetObjectScale(poollava, 0x10000 / 10 * i, 0x10000 / 10 * i);
        Delay(1);
    }
    
    // 6 Tick過后，全部熔岩彈落地；制造爆炸
    int bomb = CreateObjectRaw(x, y, 0, 0, 11540);
    SetObjectScale(bomb, 0x10000 * 11 / 16, 0x10000 * 11 / 16);
    // 爆炸的殺傷效果和回調函數
    SetObjectFlags(bomb, OF_ATTACKENEMY);
    SetCallbackProcedure(bomb, 11501);
    
    // 爆炸效果26 Tick后開始淡出
    Delay(26);
    SetObjectFadeOut(bomb, 16, 1);
    
    // 熔岩池和地面火焰66 Tick后開始淡出，清除殺傷標記
    Delay(40);
    ClearObjectFlags(fire, OF_ATTACKALL);
    SetObjectFadeOut(fire, 12, 1);
    SetObjectFadeOut(pool, 12, 1);
    SetObjectFadeOut(poollava, 12, 1);
}

// 拋出熔岩彈
void ThrowLava(int centerX, int centerY, int dir, int phi, int initSpeed) {
    // 創造連續6個熔岩彈，形成一條鏈
    for (int i = 0; i < 6; i++) {
        // 創建熔岩彈
        int lavaSplit = CreateObjectRaw(centerX, centerY, 0, 0, 43004);
        // 按照給定的三維方向和初速度擲出
        SetObjectSpeed_Sphere(lavaSplit, dir, phi, initSpeed);
        // 設置熔岩彈的重力
        SetObjectHasGravity (lavaSplit, 1);
        SetObjectGravity(lavaSplit, 0x10000 * 3 / 10);
        // 設置不透明度：越靠近尾跡處越透明
        SetObjectOpacity(lavaSplit, 16 - i * 2);
        
        // 使用TraceLavaSplit函數，跟蹤熔岩彈的位置，落地時銷毀
        // 并根据第几顆熔岩彈創建岩漿池和爆炸
        asynccall TraceLavaSplit(centerX, centerY, lavaSplit, i);
        
        // 延遲1 Tick再拋下一個，形成拖尾
        Delay(1);
    }
}

// 制造地裂和循環噴發熔岩彈
void CreateVolcano(int centerX, int centerY) {
    
    // 制造三個物件：地裂；地裂里的岩漿；噴出的主火焰
    int leakNo = Rand(1, 4);
    int leak = CreateObjectRaw(centerX, centerY, 0, 0, 11500 + leakNo);
    int leaklava = CreateObjectRaw(centerX, centerY, 0, 0, 11510 + leakNo);
    int fire = CreateObjectRaw(centerX, centerY, 0, 0, 19002);
    SetObjectScale(fire, 0x10000 * 3 / 2, 0x10000 * 3 / 2);
    
    // 主火焰有殺傷
    SetObjectFlags(fire, OF_ATTACKENEMY);
    SetCallbackProcedure(fire, 11501);
    
    // 向外噴射岩漿
    for(int i = 0; i < 24; i++) {
        asynccall ThrowLava(centerX, centerY, 
            Rand(0, 255), Rand(40, 60), Rand(10, 15));
        Delay(6);
    }
    
    // 等待所有熔岩落地，熔岩池消失
    Wait("TraceLavaSplit");
    
    // 取消主火焰殺傷
    ClearObjectFlags(fire, OF_ATTACKENEMY);
    
    // 地裂、地裂岩漿和主火焰消失
    SetObjectFadeOut(fire, 3, 1);
    SetObjectFadeOut(leak, 16, 1);
    SetObjectFadeOut(leaklava, 16, 1);
}

// 地震效果：鏡頭搖晃和音效
void ShakeCameraAndQuakeSound(int duration) {
    for (int i = 0; i < duration; i++) {
        if (i % 3 != 2) 
            SetViewCamera(GetCameraX() + Rand(-15, 15),
                          GetCameraY() + Rand(-15, 15));
        if (i % 80 == 0)
            PlaySound1("m040snd01", 255);
        
        Delay(1);
    }
}

void GroundFire(int level) {
    BatchLoadShape("magic\\115\\*");
    BatchLoadShape("magic\\033\\*");
    DisablePlayMagic();
    DownBrightness(12, 5);
    
    SetOverwhelming(intvAttackerMajor, 1);
    int x = GetObjectScreenX(intvAttackerMajor);
    int y = GetObjectScreenY(intvAttackerMajor);
    SetViewCamera(x, y - 120); //攝像机對准武將
    
    AddAttackCounter(intvAttackerMajor, 2);
    Delay(15);
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL2); // 武將把武器一招
    Delay(20);
    SetOverwhelming(intvAttackerMajor, 0);
    
    // 初始化對武將傷害
    SetGlobal(201, intvMagicAttackValue);
    
    // 地震效果
    asynccall ShakeCameraAndQuakeSound(80 * (level - 1) + 260);
    
    for (int i = 0; i < level; i++) {
        // 得到地火噴發點（人群密集處）
        int centerSoldier = GetMostImportantSoldier(!intvIsLeft);
        if (centerSoldier == 0)
            centerSoldier = intvDefenderMajor;
        
        int centerX = GetObjectScreenX(centerSoldier);
        int centerY = GetObjectScreenY(centerSoldier);
        
        // 不長在主將腳下
        if (centerSoldier == intvDefenderMajor) {
            if (Rand(0, 1) == 1) centerX += Rand(100, 200);
            else centerX -= Rand(100, 200);
            if (Rand(0, 1) == 1) centerY += Rand(50, 130);
            else centerY -= Rand(50, 130);
        }
        
        // 把攝像机移過去
        MoveCamera(centerX, centerY - 120, 12);
        
        // 制造地火
        asynccall CreateVolcano(centerX, centerY);
        Delay(68);
    }
    
    // 等待所有地火落地，岩漿池消失
    Wait("TraceLavaSplit");
    Delay(20);
    
    // 武將技結束
    RaiseBrightness(16, 5);
    EnablePlayMagic();
}

// 入口函數
void Magic115() callsign 804 {
    GroundFire(1);
}
void Magic116() callsign 805 {
    GroundFire(2);
}
void Magic117() callsign 806 {
    GroundFire(3);
}


//////////////////// 新武將技：升天

// 升天指定士兵
void AscendSoldier(int soldier) {
    // 殺死士兵
    KillForce(soldier);
    // 取消物件重力
    SetObjectHasGravity(soldier, 0);
    // 手動每幀調整物件速度；velocity以0x10000為1，加速度為0.3
    int velocity = 0;
    for (int i = 0; i < 100; i++) {
        velocity += 0x10000 / 3;
        SetObjectSpeed_Cylind(soldier, 0, velocity / 0x10000);
        Delay(1);
    }
    // 將士兵淡出
    SetObjectFlags(soldier, OF_MIXER);
    for (i = 16; i >= 0; i--) {
        SetObjectOpacity(soldier, i);
        Delay(1);
    }
    FreeObject(soldier);
}

// 主函數
void Ascension() {
    DisablePlayMagic();
    DownBrightness(8, 5);
    SetOverwhelming(intvAttackerMajor, 1);
    
    SetCameraByReference(intvAttackerMajor, 0, -120);
    Delay(15);
    SetOverwhelming(intvAttackerMajor, 0);
    
    // 判斷對方有沒有士兵
    if (GetSoldierCount(!intvIsLeft) > 0) {
    
        // 獲得對方（大致上）最靠前的士兵的戰場X坐標
        int maxX = ScreenXToBattleX(GetSoldierMaxX2(intvIsLeft));
        
        // 在該士兵兩側各9格內，獲取所有敵方士兵和主將的數目
        int targetCount = GetForceCountInRect(!intvIsLeft, maxX, 7, 
                9, GetBattleHeight());
        
        // 在同樣的區域內，篩選出所有士兵，儲存在targets數組中
        int targets[200];
        int j = 0;
        for (int i = 0; i < targetCount; i++) {
            int target = GetNthForceInRect(!intvIsLeft, maxX, 7, 
                9, GetBattleHeight(), i);
            if (target != intvDefenderMajor) {
                targets[j] = target;
                j++;
            }
        }
        targetCount = j;
        
        // 制造隨机排列
        int temp;
        for (i = 0; i < targetCount - 1; i++) {
            j = Rand(i + 1, targetCount - 1);
            temp = targets[i];
            targets[i] = targets[j];
            targets[j] = temp;
        }
        
        // 移動攝像机到隨机士兵身上
        int cameraFocus = targets[Rand(0, targetCount)];
        MoveCamera(GetObjectScreenX(cameraFocus), 
                   GetObjectScreenY(cameraFocus) -120, 
                   12);
        
        // 播放音效
        PlaySound1("m020snd01", 180);
        
        // 武將技總用時320 Tick；將士兵逐個升天
        int timeLeft = 320;
        for (i = 0; i < targetCount; i++) {
            asynccall AscendSoldier(targets[i]);
            
            int interval = Rand(4, 8);
            timeLeft -= interval;
            Delay(interval);
        }
        Delay(timeLeft);
        
    } else {
        // 召喚鐵鍋
        CastFail(intvAttackerMajor, GetObjectDir(intvAttackerMajor));
    }
    
    Delay(20);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
}


// 主函數（改造成按比例處理所有士兵）
// persent：百分之多少
void AscensionAll(int persent) {
    DisablePlayMagic();
    DownBrightness(8, 5);
    SetOverwhelming(intvAttackerMajor, 1);
    
    SetCameraByReference(intvAttackerMajor, 0, -120);
    Delay(15);
    SetOverwhelming(intvAttackerMajor, 0);
    
    // 獲取所有敵方士兵的數目
    int targetCount = GetSoldierCount(!intvIsLeft);

    // 判斷對方有沒有士兵
    if (targetCount > 0) {
    
        // 篩選出所有士兵，儲存在targets數組中
        int targets[201];
        int j = 0;
        for (int i = 0; i < targetCount; i++) {
            int target = GetNthForceInRect(!intvIsLeft, 35, 7, 
                36, 7, i);
            if (target != intvDefenderMajor) {
                targets[j] = target;
                j++;
            }
        }
        
        // 制造隨机排列
        int temp;
        for (i = 0; i < targetCount - 1; i++) {
            j = Rand(i + 1, targetCount - 1);
            temp = targets[i];
            targets[i] = targets[j];
            targets[j] = temp;
        }
        
        // 移動攝像机到隨机士兵身上
        int cameraFocus = targets[Rand(0, targetCount)];
        MoveCamera(GetObjectScreenX(cameraFocus), 
                   GetObjectScreenY(cameraFocus) -120, 
                   12);
        
        // 播放音效
        PlaySound1("m020snd01", 180);
        
		//按百分比計算殺兵數，外加5人
		int damageCount = targetCount * persent / Rand(90, 110) + 5;
		
        // 武將技總用時320 Tick；將士兵逐個升天
        int timeLeft = 320;
		int timeInterval = 320 / damageCount;
        for (i = 0; i < damageCount; i++) {
            asynccall AscendSoldier(targets[i]);
            
            timeLeft -= timeInterval;
            Delay(timeInterval);
        }
        Delay(timeLeft);
		
		        
    } else {
        // 召喚鐵鍋
        CastFail(intvAttackerMajor, GetObjectDir(intvAttackerMajor));
    }
    
    Delay(20);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
}

// 入口函數
void Magic118() callsign 807 {
    AscensionAll(30);
}

// 入口函數
void Magic119() callsign 808 {
    AscensionAll(60);
}



//////////////////// 公用自定義函數（必殺技）
// 獲取武將名稱
string U_GetGeneralName(){
	int attackerMajor = GetScriptLinkedObject();
	int isLeft = GetForceSide(attackerMajor);

    int general;
    if (isLeft)
		general = intvLeftGeneral;
    else
		general = intvRightGeneral;

	return GetGeneralName(general);
}


//////////////////// 必殺技（攻擊）：大喝
void U_Roar(int attackValue) {
    
    // 得到所有原來的預設變量的值
    int attackerMajor = GetScriptLinkedObject();
    int isLeft = GetForceSide(attackerMajor);
    
    // 使用內存操作得到對方主將的物件
    SangoObject* pDefenderMajorObject;
    if (isLeft) 
        pDefenderMajorObject = *(SangoObject**) (0x4A8A60 + 0x44);
    else
		pDefenderMajorObject = *(SangoObject**) (0x4B03A0 + 0x44);

    int defenderMajor = pDefenderMajorObject->dwHandle;
    
    SetObjectAnimate(attackerMajor, OAF_SPELL1); // 主將把武器一招
    
    // 大喝一聲
    PlaySound(attackerMajor, "Roar01", 255);
    
    // 遍歷主將附近的所有九個格子，造成殺傷
    int majorBattleX = GetObjectBattleX(attackerMajor);
    int majorBattleY = GetObjectBattleY(attackerMajor);
    for (int i = majorBattleX - 1; i <= majorBattleX + 1; i++) {
        for (int j = majorBattleY - 1; j <= majorBattleY + 1; j++) {
            
            // 根據給定戰場坐標獲取士兵/主將
            int target = GetForceHandleByXY(i, j);
            
            // 是不是自己人？是則直接跳過
            if (GetForceSide(target) == isLeft)
                continue;
            
            // 是不是對方主將？
            if (target == defenderMajor) {
                // 是：製造傷害
                DoHarmToMajor(target, 
                    attackerMajor, attackValue);
            } else {
                // 不是：殺死士兵
                KillForce(target);
            }
        }
    }
    
    // 動畫效果：創建光氣環
    int circle = 
        CreateObjectByReference(attackerMajor, 18002, 0, 40);
    
    // 手動擴散光氣環。註意，由於上一段已經定義過i，所以此處不再定義
    for (i = 0; i < 20; i++) { 
        Delay(1);
        
        // 每Tick放大3/16
        int scale = 0x10000 + 0x10000 * 3 * i / 16; 
        SetObjectScale(circle, scale, scale);
        
        // 每Tick不透明度下降1點
        if (16 - i >= 0)
            SetObjectOpacity(circle, 16 - i); 
    }
    FreeObject(circle);     // 銷毀光氣環
    
}


//////////////////// 必殺技（等待）：回復體力
void U_RecoverHP() {
	int attackerMajor = GetScriptLinkedObject();
	int isLeft = GetForceSide(attackerMajor);
	General* general;
	if (isLeft)
		general = intvLeftGeneral;
	else
		general = intvRightGeneral;
		
	int wHPMax = general->wHPMax;
	int wHP = general->wHP;
	if(wHP < wHPMax){
		general->wHP = wHP + 1;
	}
}

//////////////////// 必殺技（等待）：回復技力
void U_RecoverMP() {
	int attackerMajor = GetScriptLinkedObject();
	int isLeft = GetForceSide(attackerMajor);
	General* general;
	if (isLeft)
		general = intvLeftGeneral;
	else
		general = intvRightGeneral;
		
	int wMPMax = general->wMPMax;
	int wMP = general->wMP;
	if(wMP < wMPMax){
		general->wMP = wMP + 1;
	}
}

// 開始攻擊，具體執行
// 參數：0為前方，1為後方
void StartAttack(int isBack){
	
	string name = U_GetGeneralName();
	
	
	//大喝（概率殺死周圍兵，或傷將10血）
	if(name == "張飛"
			|| name == "孫策"
			|| name == "龐德"
			|| name == "華雄"
			|| name == "顏良"
			|| name == "文鴦"){
		if (Rand(0, 9999) < 2000){
			U_Roar(10);
		}
	}
}

// 結束攻擊，具體執行
// 參數：0為前方，1為後方
void EndAttack(int isBack){
	
}

//////////////////// 動畫序列內嵌的事件：
// 前攻擊開始
void EventAttackFrontStart() callsign 55501 {
	
	StartAttack(0);
	
}

//////////////////// 動畫序列內嵌的事件：
// 後攻擊開始
void EventAttackBackStart() callsign 55502 {
	
	StartAttack(1);
	
}

//////////////////// 動畫序列內嵌的事件：
// 前攻擊結束
void EventAttackFrontEnd() callsign 55511 {
	
	EndAttack(0);
	
}

//////////////////// 動畫序列內嵌的事件：
// 後攻擊結束
void EventAttackBackEnd() callsign 55512 {
	
	EndAttack(1);
	
}

// 等待
void EventWait() callsign 55504 {
	

}

// 前進
void EventWalk() callsign 55505 {
	
}

// 防禦
void EventDefense() callsign 55506 {
	
}

// 被打到
void EventPain() callsign 55507 {
	
}

// 死亡，實際戰後才響應
void EventDeath() callsign 55508 {
	
}

// 勝利，注意獲取到的物件是敗方
void EventWin() callsign 55509 {
	
	//Prompt(3, 0, "EventWin TJ:" + (string)TJ + "%k%%e%");
	//Delay(1);
	
}
