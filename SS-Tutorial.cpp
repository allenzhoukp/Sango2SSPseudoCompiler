// Encoding: GBK

// 此文件为三国2脚本教程第一部分的所有武将技、必杀技和士兵技的完整代码。
// 包括内容：
// Hello World：第四章第三节
// 大喝（武将技）：第四章
// 前后伏兵：第五章附录
// 毒箭：第六章
// 地火系：第七章
// 升天：第八章
// 大喝（必杀技）：第九章前半段
// 骑兵冲锋士兵技：第九章后半段

// 作者：远帆


#include "magic.cpp"

// Hello World
void HelloReal(string content) {
    DisablePlayMagic();    	// 禁止双方武将发武将技
    DownBrightness(12, 5); 	// 调用magic.cpp中写好的函数，让天空变黑
    
    SetOverwhelming(intvAttackerMajor, 1);	// 设置武将为无敌状态
    int x = GetObjectScreenX(intvAttackerMajor);  // 得到武将屏幕x坐标
    int y = GetObjectScreenY(intvAttackerMajor);  // 得到武将屏幕y坐标
    SetViewCamera(x, (y - 120));	//  指定屏幕坐标，将摄像机对准武将
    
    AddAttackCounter(intvAttackerMajor, 2);  // 武将定身2个定身时间单位
    Delay(15);					   // 脚本暂停15 Tick
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL1); // 主将把武器一招
    Delay(20);					   	// 脚本暂停20 Tick
    SetOverwhelming(intvAttackerMajor, 0);	// 结束武将无敌状态
    
    Prompt(3, 0, content + "%k%%e%");	// 武将技内容，输出content的值 
    
    RaiseBrightness(16, 5);		// 天空渐渐变白
    EnablePlayMagic();			// 允许双方武将发武将技
}

// =====================================================

// 大喝武将技主函数
void Roar() {
    DisablePlayMagic();     // 禁止双方武将发武将技
    DownBrightness(12, 5);  // 天空变黑
    
    SetOverwhelming(intvAttackerMajor, 1);  // 设置武将为无敌状态
    int x = GetObjectScreenX(intvAttackerMajor);  // 得到武将屏幕x坐标
    int y = GetObjectScreenY(intvAttackerMajor);  // 得到武将屏幕y坐标
    SetViewCamera(x, (y - 120));    //  指定屏幕坐标，将摄像机对准武将
    
    AddAttackCounter(intvAttackerMajor, 2); // 武将定身2个定身时间单位
    Delay(15);                      // 脚本暂停15 Tick
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL1); // 主将把武器一招
    Delay(20);                      // 脚本暂停20 Tick
    // 武将技准备部分结束
    
    // 大喝一声
    PlaySound1("Roar01", 255);
    
    // 遍历主将附近的所有九个格子，造成杀伤
    int majorBattleX = GetObjectBattleX(intvAttackerMajor);
    int majorBattleY = GetObjectBattleY(intvAttackerMajor);
    for (int i = majorBattleX - 1; i <= majorBattleX + 1; i++) {
        for (int j = majorBattleY - 1; j <= majorBattleY + 1; j++) {
            // 根据给定战场坐标获取士兵/主将
            int target = GetForceHandle(i, j);
            
            // 是不是自己人？是则直接跳过
            if (GetForceSide(target) == intvIsLeft)
                continue;
            
            // 是不是对方主将？
            if (target == intvDefenderMajor) {
                // 是：制造伤害
                DoHarmToMajor(target, 
                    intvAttackerMajor, intvMagicAttackValue);
            } else {
                // 不是：杀死士兵
                KillForce(target);
            }
        }
    }
    
    // 动画效果：创建光气环
    int circle = 
        CreateObjectByReference(intvAttackerMajor, 18002, 0, 40);
    
    // 手动扩散光气环。注意，由于上一段已经定义过i，所以此处不再定义
    for (i = 0; i < 20; i++) { 
        Delay(1);
        
        // 每Tick放大3/16
        int scale = 0x10000 + 0x10000 * 3 * i / 16; 
        SetObjectScale(circle, scale, scale);
        
        // 每Tick不透明度下降1点
        if (16 - i >= 0)
            SetObjectOpacity(circle, 16 - i); 
    }
    FreeObject(circle);     // 销毁光气环
    
    // 武将技主体部分结束。拖一会儿时间，显得自然一点
    Delay(10);
    
    // 武将技结束部分
    SetOverwhelming(intvAttackerMajor, 0);  // 结束武将无敌状态
    
    RaiseBrightness(16, 5);     // 天空渐渐变白
    EnablePlayMagic();          // 允许双方武将发武将技
}

// =====================================================

// 指定士兵在16 Tick内渐入
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

// 在左侧或右侧生成soldierCount个士兵
void CreateSoldierFrontBack(int soldierCount, int onLeft) {
    PlaySound1("m002snd01", 255);
    PlaySound1("m002snd03", 255);
    
    // 初始化坐标：底线中点
    int y = GetBattleHeight() / 2;
    int x;
    if (onLeft) x = 0;
    else x = GetBattleWidth() - 1;
    
    // 循环：招完所有兵为止
    int soldier, existing;
    int opposingY;
    while(soldierCount > 0) {
        // 检查战场上半部分的位置
        existing = GetForceHandle(x, y);
        if (existing == 0) {
            soldier = CreateSoldier(intvIsLeft, x, y);
            asynccall MyStepShow(soldier);
            soldierCount--;
        }
        // 检查是否已招完兵
        if (soldierCount <= 0)
            break;
            
        // 检查对称的下半部分的位置
        opposingY = GetBattleHeight() - 1 - y;
        existing = GetForceHandle(x, opposingY);
        if (existing == 0) {
            soldier = CreateSoldier(intvIsLeft, x, opposingY);
            asynccall MyStepShow(soldier);
            soldierCount--;
        }
        
        // 调整(x, y)位置：如果填满就换列，否则同列向上下方顺延
        if (opposingY == 1) {
            if (onLeft) x++;
            else x--;
            y = GetBattleHeight() / 2;
        } else {
            y++;
        }
    }
}

// =====================================================

// 前后伏兵主函数。两个参数是前伏兵和后伏兵的兵数
void MoreSoldierFrontBack(int frontSolCount, int backSolCount) {
    DisablePlayMagic();
    DownBrightness(12, 5);
    
    SetOverwhelming(intvAttackerMajor, 1);
    int x = GetObjectScreenX(intvAttackerMajor);
    int y = GetObjectScreenY(intvAttackerMajor);
    SetViewCamera(x, y - 120); //摄像机对准武将
    
    AddAttackCounter(intvAttackerMajor, 2);
    Delay(15);
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL2); // 主将把武器一招
    Delay(20);
    SetOverwhelming(intvAttackerMajor, 0);
    
    // 得到允许的招兵数
    int level = GetMajorLevel(intvIsLeft);
    if (level > 40) level = 40;
    int soldiersAllowed = level * 5 - GetSoldierCount(intvIsLeft);
    
    if (soldiersAllowed <= 0) {
        // 满兵：召唤铁锅
        CastFail(intvAttackerMajor, GetObjectDir(intvAttackerMajor));
        
    } else {
        // 无法补足兵：按比例分配前后伏兵
        int total = frontSolCount + backSolCount;
        if (soldiersAllowed < frontSolCount + backSolCount) {
            frontSolCount = soldiersAllowed * frontSolCount / total;
            backSolCount = soldiersAllowed - frontSolCount;
        }
    
        if (intvIsLeft) {
            CreateSoldierFrontBack(frontSolCount, 1); // 先召唤左伏兵
            MoveCamera(240, 0, 25);  // 摄像机移过去
            Delay(80);             // 等一会儿
            
            CreateSoldierFrontBack(backSolCount, 0); // 再召唤右伏兵
            MoveCamera(GetBattleWidthInScreenX() - 240, 0, 25); 
            Delay(80);
            
        } else {
            // 类似
            CreateSoldierFrontBack(frontSolCount, 0);
            MoveCamera(GetBattleWidthInScreenX() - 240, 0, 25);
            Delay(80);
            
            CreateSoldierFrontBack(backSolCount, 1);
            MoveCamera(240, 0, 25);
            Delay(80);
        }
        MoveCamera(x, y - 120, 25); //摄像机移回武将
    }
    
    RaiseBrightness(16, 5);
    EnablePlayMagic();
}


// =====================================================

// 中毒效果
void PoisonHarm(int target, int attackerMajor, 
        int attackValue, int interval, int times) {
    while(times > 0) {
        Delay(interval);
        
        // 造成伤害，添加武将被击中的动作和声音
        DoHarmToMajor(target, attackerMajor, attackValue);
        SetObjectAnimate(target, OAF_PAIN);
        PlaySound(target, "Yell02", 180);
        
        // 制造中毒特效动画
        int effect = CreateObjectByReference(target, 13102, 0, 0);
        SetCoordinateByReference_Cylind(effect, target,
            GetObjectDir(target), -10, 70);
        SetObjectFadeOut(effect, 12, 1);
        
        times--;
    }
}
        
// 击中主将的回调函数
void PoisonArrowCallback(int arrow, int target) callsign 11401{
    if (target == intvDefenderMajor) {
        // 销毁箭支和光芒
        SetObjectFadeOut(arrow, 2, 1);
        // 利用GetObjectContext，获取物件额外参数
        int light = GetObjectContext(arrow, 0);
        SetObjectFadeOut(light, 2, 1);
        
        // 对主将造成三分之一伤害
        Hurt(arrow, intvDefenderMajor, intvMagicAttackValue / 3);
        
        // 运行中毒效果，持续造成中毒伤害
        asynccall PoisonHarm(target, intvAttackerMajor,
            intvMagicAttackValue * 2 / 3 / 5, 120, 5);
        
        // 创建击中效果
        HitGeneral(arrow, target, 13102, 1, 60, 0);
        
    }
}

// 镜头锁定物件
void LockCameraOnObject(int object, int yOffset) {
    int dir = GetObjectDir(object);
    while(IsObjectExist(object)) {
        // 得到欲锁定物件和摄像机自身的x坐标
        int objectX = GetObjectScreenX(object);
        int cameraX = GetCameraX();
        
        // 检查边界：如果飞出战场，则直接退出
        if (cameraX < 0 || cameraX >= GetBattleWidthInScreenX())
            break;
            
        // 检查物件与摄像机的距离
        int dx = objectX - cameraX;
        // 如果物件正在飞离摄像机，则移动摄像机，否则什么也不做
        if ((dir == 0 && dx > 0) || (dir == 128 && dx < 0))
            SetCameraByReference(object, 0, yOffset);
        Delay(1);
    }
}

// 毒箭主函数
void ShootPoisonArrow() {
    DisablePlayMagic();
    DownBrightness(8, 5);
    SetOverwhelming(intvAttackerMajor, 1);
    
    // 设置初始摄像机位置；向射箭方向的反方向偏移180像素
    if (intvIsLeft)
        SetCameraByReference(intvAttackerMajor, 180, -120);
    else
        SetCameraByReference(intvAttackerMajor, -180, -120);
    Delay(15);
    SetOverwhelming(intvAttackerMajor, 0);
    
    // 主将拈弓
    SetObjectAnimate(intvAttackerMajor, OAF_SHOT1);
    Delay(15);
    PlaySound(intvAttackerMajor, "m003snd03", 255);
    
    // 创建箭支及其光芒，并将其射出
    int dir = GetObjectDir(intvAttackerMajor);
    int arrow = CreateObjectByReference(intvAttackerMajor,
        13001, dir, 0);
    SetCoordinateByReference_Cylind(arrow, intvAttackerMajor, 
        dir, 80, 80);
    int light = CreateObjectByReference(arrow, 13101, dir, 0);
    SetObjectSpeed_Cylind(arrow, 16, 0);
    SetObjectSpeed_Cylind(light, 16, 0);
    
    // 镜头锁定箭支
    asynccall LockCameraOnObject(arrow, -120);
    
    // 箭支加上打击目标
    SetObjectFlags(arrow, OF_ENEMYGENERAL);
    // 设置回调函数
    SetCallbackProcedure(arrow, 11401);
    // 设置箭支物件的0号额外参数为光芒物件（以在击中目标时销毁）
    SetObjectContext(arrow, 0, light);
    
    // 等待LockCameraOnObject函数退出
    Wait("LockCameraOnObject");
    Delay(60);
    
    // 武将技结束
    RaiseBrightness(16, 5);
    EnablePlayMagic();
}

// =====================================================

// 回调函数
void GroundFireCallback(int object, int target) callsign 11501 {
    if (target == intvDefenderMajor) {
        // 击中主将：从45号全局变量中获取单发伤害，
        // 主将每被烫一次伤害减半，且一个物件只能对主将造成一次伤害
        int harm = GetGlobal(45);
        if (harm > 0) {
            DoHarmToMajor(target, 0, harm);
            SetGlobal(45, harm / 2);
            ClearObjectFlags(object, OF_ENEMYGENERAL);
            HitGeneral(object, target, 11002, 1, 48, 0);
        }
        
    } else {
        // 击中士兵，士兵着火而死
        asynccall FireMan(target, 10015, 60);
    }
}

// 销毁落地的熔岩弹，对第一颗熔岩弹创建岩浆池和爆炸效果
void TraceLavaSplit(int centerX, int centerY, int lavaSplit, int splitNo) {

    // 等待熔岩弹落地
    do {
        Delay(1);
    } while(GetObjectScreenZ(lavaSplit) > 0);
    
    // 获取熔岩弹落地坐标，并销毁
    int x = GetObjectScreenX(lavaSplit);
    int y = GetObjectScreenY(lavaSplit);
    FreeObject(lavaSplit);
    
    // 如果不是第一颗熔岩弹，则工作结束，直接返回
    if (splitNo != 0) return;
    
    // 创建地板上的火焰并缩放
    int fire = CreateObjectRaw(x, y, 0, 0, 11541);
    SetObjectScale(fire, 0x10000 / 3, 0x10000 / 6);
    
    // 地面上的火焰制造杀伤效果，并设置回调函数
    SetObjectFlags(fire, OF_ATTACKENEMY);
    SetCallbackProcedure(fire, 11501);
    
    // 创建熔岩池；随机一个熔岩池编号，熔岩池随熔岩弹落地而逐步扩大
    int poolNo = Rand(1, 6);
    int pool = CreateObjectRaw(x, y, 0, 0, 11520 + poolNo);
    int poollava = CreateObjectRaw(x, y, 0, 0, 11530 + poolNo);
    for (int i = 0; i < 6; i++) {
        SetObjectScale(pool, 0x10000 / 10 * i, 0x10000 / 10 * i);
        SetObjectScale(poollava, 0x10000 / 10 * i, 0x10000 / 10 * i);
        Delay(1);
    }
    
    // 6 Tick过后，全部熔岩弹落地；制造爆炸
    int bomb = CreateObjectRaw(x, y, 0, 0, 11540);
    SetObjectScale(bomb, 0x10000 * 11 / 16, 0x10000 * 11 / 16);
    // 爆炸的杀伤效果和回调函数
    SetObjectFlags(bomb, OF_ATTACKENEMY);
    SetCallbackProcedure(bomb, 11501);
    
    // 爆炸效果26 Tick后开始淡出
    Delay(26);
    SetObjectFadeOut(bomb, 16, 1);
    
    // 熔岩池和地面火焰66 Tick后开始淡出，清除杀伤标记
    Delay(40);
    ClearObjectFlags(fire, OF_ATTACKALL);
    SetObjectFadeOut(fire, 12, 1);
    SetObjectFadeOut(pool, 12, 1);
    SetObjectFadeOut(poollava, 12, 1);
}

// 抛出熔岩弹
void ThrowLava(int centerX, int centerY, int dir, int phi, int initSpeed) {
    // 创造连续6个熔岩弹，形成一条链
    for (int i = 0; i < 6; i++) {
        // 创建熔岩弹
        int lavaSplit = CreateObjectRaw(centerX, centerY, 0, 0, 43004);
        // 按照给定的三维方向和初速度掷出
        SetObjectSpeed_Sphere(lavaSplit, dir, phi, initSpeed);
        // 设置熔岩弹的重力
        SetObjectHasGravity (lavaSplit, 1);
        SetObjectGravity(lavaSplit, 0x10000 * 3 / 10);
        // 设置不透明度：越靠近尾迹处越透明
        SetObjectOpacity(lavaSplit, 16 - i * 2);
        
        // 使用TraceLavaSplit函数，跟踪熔岩弹的位置，落地时销毁
        // 并根据第几颗熔岩弹创建岩浆池和爆炸
        asynccall TraceLavaSplit(centerX, centerY, lavaSplit, i);
        
        // 延迟1 Tick再抛下一个，形成拖尾
        Delay(1);
    }
}

// 制造地裂和循环喷发熔岩弹
void CreateVolcano(int centerX, int centerY) {
    
    // 制造三个物件：地裂；地裂里的岩浆；喷出的主火焰
    int leakNo = Rand(1, 4);
    int leak = CreateObjectRaw(centerX, centerY, 0, 0, 11500 + leakNo);
    int leaklava = CreateObjectRaw(centerX, centerY, 0, 0, 11510 + leakNo);
    int fire = CreateObjectRaw(centerX, centerY, 0, 0, 19002);
    SetObjectScale(fire, 0x10000 * 3 / 2, 0x10000 * 3 / 2);
    
    // 主火焰有杀伤
    SetObjectFlags(fire, OF_ATTACKENEMY);
    SetCallbackProcedure(fire, 11501);
    
    // 向外喷射岩浆
    for(int i = 0; i < 24; i++) {
        asynccall ThrowLava(centerX, centerY, 
            Rand(0, 255), Rand(40, 60), Rand(10, 15));
        Delay(6);
    }
    
    // 等待所有熔岩落地，熔岩池消失
    Wait("TraceLavaSplit");
    
    // 取消主火焰杀伤
    ClearObjectFlags(fire, OF_ATTACKENEMY);
    
    // 地裂、地裂岩浆和主火焰消失
    SetObjectFadeOut(fire, 3, 1);
    SetObjectFadeOut(leak, 16, 1);
    SetObjectFadeOut(leaklava, 16, 1);
}

// 地震效果：镜头摇晃和音效
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

// 地火主函数
void GroundFire(int level) {
    BatchLoadShape("magic\\115\\*");
    BatchLoadShape("magic\\033\\*");
    DisablePlayMagic();
    DownBrightness(12, 5);
    
    SetOverwhelming(intvAttackerMajor, 1);
    int x = GetObjectScreenX(intvAttackerMajor);
    int y = GetObjectScreenY(intvAttackerMajor);
    SetViewCamera(x, y - 120); //摄像机对准武将
    
    AddAttackCounter(intvAttackerMajor, 2);
    Delay(15);
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL2); // 武将把武器一招
    Delay(20);
    SetOverwhelming(intvAttackerMajor, 0);
    
    // 初始化对武将伤害
    SetGlobal(45, intvMagicAttackValue);
    
    // 地震效果
    asynccall ShakeCameraAndQuakeSound(80 * (level - 1) + 260);
    
    for (int i = 0; i < level; i++) {
        // 得到地火喷发点（人群密集处）
        int centerSoldier = GetMostImportantSoldier(!intvIsLeft);
        if (centerSoldier == 0)
            centerSoldier = intvDefenderMajor;
        
        int centerX = GetObjectScreenX(centerSoldier);
        int centerY = GetObjectScreenY(centerSoldier);
        
        // 不长在主将脚下
        if (centerSoldier == intvDefenderMajor) {
            if (Rand(0, 1) == 1) centerX += Rand(100, 200);
            else centerX -= Rand(100, 200);
            if (Rand(0, 1) == 1) centerY += Rand(50, 130);
            else centerY -= Rand(50, 130);
        }
        
        // 把摄像机移过去
        MoveCamera(centerX, centerY - 120, 12);
        
        // 制造地火
        asynccall CreateVolcano(centerX, centerY);
        Delay(68);
    }
    
    // 等待所有地火落地，岩浆池消失
    Wait("TraceLavaSplit");
    Delay(20);
    
    // 武将技结束
    RaiseBrightness(16, 5);
    EnablePlayMagic();
}

// =====================================================

// 升天指定士兵
void AscendSoldier(int soldier) {
    // 杀死士兵
    KillForce(soldier);
    // 取消物件重力
    SetObjectHasGravity(soldier, 0);
    // 手动每帧调整物件速度；velocity以0x10000为1，加速度为0.3
    int velocity = 0;
    for (int i = 0; i < 100; i++) {
        velocity += 0x10000 / 3;
        SetObjectSpeed_Cylind(soldier, 0, velocity / 0x10000);
        Delay(1);
    }
    // 将士兵淡出
    SetObjectFlags(soldier, OF_MIXER);
    for (i = 16; i >= 0; i--) {
        SetObjectOpacity(soldier, i);
        Delay(1);
    }
    FreeObject(soldier);
}

// 升天主函数
void Ascension() {
    DisablePlayMagic();
    DownBrightness(8, 5);
    SetOverwhelming(intvAttackerMajor, 1);
    
    SetCameraByReference(intvAttackerMajor, 0, -120);
    Delay(15);
    SetOverwhelming(intvAttackerMajor, 0);
    
    // 判断对方有没有士兵
    if (GetSoldierCount(!intvIsLeft) > 0) {
    
        // 获得对方（大致上）最靠前的士兵的战场X坐标
        int maxX = ScreenXToBattleX(GetSoldierMaxX2(intvIsLeft));
        
        // 在该士兵两侧各9格内，获取所有敌方士兵和主将的数目
        int targetCount = GetForceCountInRect(!intvIsLeft, maxX, 7, 
                9, GetBattleHeight());
        
        // 在同样的区域内，筛选出所有士兵，储存在targets数组中
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
        
        // 制造随机排列
        int temp;
        for (i = 0; i < targetCount - 1; i++) {
            j = Rand(i + 1, targetCount - 1);
            temp = targets[i];
            targets[i] = targets[j];
            targets[j] = temp;
        }
        
        // 移动摄像机到随机士兵身上
        int cameraFocus = targets[Rand(0, targetCount)];
        MoveCamera(GetObjectScreenX(cameraFocus), 
                   GetObjectScreenY(cameraFocus) -120, 
                   12);
        
        // 播放音效
        PlaySound1("m020snd01", 180);
        
        // 武将技总用时320 Tick；将士兵逐个升天
        int timeLeft = 320;
        for (i = 0; i < targetCount; i++) {
            asynccall AscendSoldier(targets[i]);
            
            int interval = Rand(2, 3);
            timeLeft -= interval;
            Delay(interval);
        }
        Delay(timeLeft);
        
    } else {
        // 召唤铁锅
        CastFail(intvAttackerMajor, GetObjectDir(intvAttackerMajor));
    }
    
    Delay(20);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
}

// =====================================================
// 各武将技的入口函数

// Hello World
void Magic111() callsign 800 {
    HelloReal("Hello World!");	// 调用实际执行的武将技函数
}

// 大喝
void Magic112() callsign 801 {
    Roar();  // 调用实际执行的武将技函数
}

// 前后伏兵
void Magic113() callsign 802 {
    // 调用实际执行的武将技函数
    MoreSoldierFrontBack(intvMagicAttackValue, intvMagicAttackValue);
}

// 毒箭
void Magic114() callsign 803 {
    ShootPoisonArrow();
}

// 地火系
void Magic115() callsign 804 {
    GroundFire(1);
}
void Magic116() callsign 805 {
    GroundFire(2);
}
void Magic117() callsign 806 {
    GroundFire(3);
}

// 升天
void Magic118() callsign 807 {
    Ascension();
}

// =====================================================

// 大喝必殺技
void RoarGenSkill() {
    
    // 得到所有原來預設變量的值
    int attackerMajor = GetScriptLinkedObject();
    int isLeft = GetForceSide(attackerMajor);
    int magicAttackValue = 10;
    
    // 使用內存操作得到對方主將的物件
    SangoObject* pDefenderMajorObject;
    if (isLeft) 
        pDefenderMajorObject = *(SangoObject**) (0x4A8A60 + 0x44);
    else pDefenderMajorObject = *(SangoObject**) (0x4B03A0 + 0x44);
    int defenderMajor = pDefenderMajorObject->dwHandle;
    
    SetObjectAnimate(attackerMajor, OAF_SPELL1); // 主將把武器一招
    
    // 大喝一聲
    PlaySound(attackerMajor, "Roar01", 255);
    
    // 遍歷主將周圍的所有九個格子，造成殺傷
    int majorBattleX = GetObjectBattleX(attackerMajor);
    int majorBattleY = GetObjectBattleY(attackerMajor);
    for (int i = majorBattleX - 1; i <= majorBattleX + 1; i++) {
        for (int j = majorBattleY - 1; j <= majorBattleY + 1; j++) {
            
            // 根據給定戰場坐標獲取士兵/主將
            int target = GetForceHandle(i, j);
            
            // 是不是自己人？是則直接跳過
            if (GetForceSide(target) == isLeft)
                continue;
            
            // 是不是對方主將？
            if (target == defenderMajor) {
                // 是：製造傷害
                DoHarmToMajor(target, 
                    attackerMajor, magicAttackValue);
            } else {
                // 不是：殺死士兵
                KillForce(target);
            }
        }
    }
    
    // 動畫效果：創建光氣環
    int circle = 
        CreateObjectByReference(attackerMajor, 18002, 0, 40);
    
    // 手動擴散光氣環。注意，由於上一段已經定義過i，所以此處不再定義
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

// 必殺技的入口函數，調用代碼55501
void GenSkill() callsign 55501 {
    // 先通過主將物件得到所屬方
    int attackerMajor = GetScriptLinkedObject();
    int isLeft = GetForceSide(attackerMajor);
    
    // 得到武將指針
    int general;
    if (isLeft) general = intvLeftGeneral;
    else general = intvRightGeneral;
    
    // 判斷是否為劉備，不是則直接退出函數
    if (GetGeneralName(general) != "糂称")
        return;

    if (Rand(0, 9999) < 500) // 發動概率5%
        RoarGenSkill();  // 調用實際執行的必殺技函數

}

// =====================================================

// 骑兵冲锋主函数
void HorseCharge() {
    // 获取施放士兵技的士兵，所属方和面朝的方向；
    int soldier = GetScriptLinkedObject();
    int isLeft = GetForceSide(soldier);
    int dir = GetObjectDir(soldier);
    
    // 通过内存操作获取对方主将物件；
    SangoObject* pDefenderMajorObject;
    if (isLeft) 
        pDefenderMajorObject = *(SangoObject**) (0x4A8A60 + 0x44);
    else pDefenderMajorObject = *(SangoObject**) (0x4B03A0 + 0x44);
    int defenderMajor = pDefenderMajorObject->dwHandle;

    // 获取士兵初始位置
    int initX = GetObjectScreenX(soldier);
    int initBattleX = ScreenXToBattleX(initX);
    int initY = GetObjectScreenY(soldier);
    int initBattleY = ScreenYToBattleY(initY);
        
    // 遍历士兵面前的三个士兵，逐个检查是不是自己人/对方主将，
    // 以确定冲锋距离
    int soldiersToCrush = 0;
    for (int i = 1; i <= 3; i++) {
        // 得到士兵前方的第i个士兵或主将
        int nextForce;
        if (dir == 0) 
            nextForce = 
                GetForceHandle(initBattleX + i, initBattleY);
        else 
            nextForce = 
                GetForceHandle(initBattleX - i, initBattleY);
        
        // 判断是否是自己人
        if (GetForceSide(nextForce) == isLeft) 
            break;
        
        // 判断是否是对方主将
        if (nextForce == defenderMajor)
            break;
        
        // 都不是，则冲锋距离加1
        soldiersToCrush++;
    }
    
    // 冲锋距离为0：直接退出
    if (soldiersToCrush == 0)
        return;
    
    // 确定冲锋的尽头点坐标；一格战场X坐标等于96点屏幕坐标
    int targetX;
    if (dir == 0)
        targetX = initX + soldiersToCrush * 96;
    else
        targetX = initX - soldiersToCrush * 96;

    //====================================================
    
    // 士兵技动画开始：播放士兵WALK序列
    SetObjectAnimate(soldier, OAF_WALK);
    
    // 手动动画：每冲过战场上的一格，用时9 Tick
    for (i = 0; i < 9 * soldiersToCrush; i++) {
        
        // 计算当前帧的士兵X坐标，并注意判断不要冲过战场边缘
        int newX = initX + (targetX-initX) * i / (9 * soldiersToCrush);
        if (newX < 0) 
            newX = 0;
        else if (newX > GetBattleWidthInScreenX()) 
            newX = GetBattleWidthInScreenX();
        
        // 强制设定士兵的位置、朝向和速度
        SetObjectDir(soldier, dir);
        SetObjectCoordinate(soldier, newX, initY, 0);
        SetObjectSpeed_Cylind(soldier, 0, 0);
        
        // 得到当前位置处的士兵
        int crushed = GetForceHandle(
            ScreenXToBattleX(newX), ScreenYToBattleY(initY));

        // 判断是不是自己人
        if (GetForceSide(crushed) != isLeft) {
            // 判断是否为对方主将（以防万一），不是则杀死士兵
            if (crushed != defenderMajor) 
                KillForce(crushed); 
        }
        
        // 手动动画步骤：等待下一帧
        Delay(1);
    }
    
    // 冲到目标位置后，让EXE自动将士兵拉回到原来的位置，无需任何操作
}

// 骑兵冲锋入口函数
void Soldier04CavalrySpc() callsign 50201 {
    // 施放概率为100%
    if (Rand(0, 9999) < 10000)
        HorseCharge();
}
