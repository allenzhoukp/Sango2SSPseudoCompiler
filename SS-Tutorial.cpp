// Encoding: GBK

// ���ļ�Ϊ����2�ű��̵̳�һ���ֵ������佫������ɱ����ʿ�������������롣
// �������ݣ�
// Hello World�������µ�����
// ��ȣ��佫������������
// ǰ������������¸�¼
// ������������
// �ػ�ϵ��������
// ���죺�ڰ���
// ��ȣ���ɱ�������ھ���ǰ���
// ������ʿ�������ھ��º���

// ���ߣ�Զ��


#include "magic.cpp"

// Hello World
void HelloReal(string content) {
    DisablePlayMagic();    	// ��ֹ˫���佫���佫��
    DownBrightness(12, 5); 	// ����magic.cpp��д�õĺ���������ձ��
    
    SetOverwhelming(intvAttackerMajor, 1);	// �����佫Ϊ�޵�״̬
    int x = GetObjectScreenX(intvAttackerMajor);  // �õ��佫��Ļx����
    int y = GetObjectScreenY(intvAttackerMajor);  // �õ��佫��Ļy����
    SetViewCamera(x, (y - 120));	//  ָ����Ļ���꣬���������׼�佫
    
    AddAttackCounter(intvAttackerMajor, 2);  // �佫����2������ʱ�䵥λ
    Delay(15);					   // �ű���ͣ15 Tick
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL1); // ����������һ��
    Delay(20);					   	// �ű���ͣ20 Tick
    SetOverwhelming(intvAttackerMajor, 0);	// �����佫�޵�״̬
    
    Prompt(3, 0, content + "%k%%e%");	// �佫�����ݣ����content��ֵ 
    
    RaiseBrightness(16, 5);		// ��ս������
    EnablePlayMagic();			// ����˫���佫���佫��
}

// =====================================================

// ����佫��������
void Roar() {
    DisablePlayMagic();     // ��ֹ˫���佫���佫��
    DownBrightness(12, 5);  // ��ձ��
    
    SetOverwhelming(intvAttackerMajor, 1);  // �����佫Ϊ�޵�״̬
    int x = GetObjectScreenX(intvAttackerMajor);  // �õ��佫��Ļx����
    int y = GetObjectScreenY(intvAttackerMajor);  // �õ��佫��Ļy����
    SetViewCamera(x, (y - 120));    //  ָ����Ļ���꣬���������׼�佫
    
    AddAttackCounter(intvAttackerMajor, 2); // �佫����2������ʱ�䵥λ
    Delay(15);                      // �ű���ͣ15 Tick
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL1); // ����������һ��
    Delay(20);                      // �ű���ͣ20 Tick
    // �佫��׼�����ֽ���
    
    // ���һ��
    PlaySound1("Roar01", 255);
    
    // �����������������оŸ����ӣ����ɱ��
    int majorBattleX = GetObjectBattleX(intvAttackerMajor);
    int majorBattleY = GetObjectBattleY(intvAttackerMajor);
    for (int i = majorBattleX - 1; i <= majorBattleX + 1; i++) {
        for (int j = majorBattleY - 1; j <= majorBattleY + 1; j++) {
            // ���ݸ���ս�������ȡʿ��/����
            int target = GetForceHandle(i, j);
            
            // �ǲ����Լ��ˣ�����ֱ������
            if (GetForceSide(target) == intvIsLeft)
                continue;
            
            // �ǲ��ǶԷ�������
            if (target == intvDefenderMajor) {
                // �ǣ������˺�
                DoHarmToMajor(target, 
                    intvAttackerMajor, intvMagicAttackValue);
            } else {
                // ���ǣ�ɱ��ʿ��
                KillForce(target);
            }
        }
    }
    
    // ����Ч��������������
    int circle = 
        CreateObjectByReference(intvAttackerMajor, 18002, 0, 40);
    
    // �ֶ���ɢ��������ע�⣬������һ���Ѿ������i�����Դ˴����ٶ���
    for (i = 0; i < 20; i++) { 
        Delay(1);
        
        // ÿTick�Ŵ�3/16
        int scale = 0x10000 + 0x10000 * 3 * i / 16; 
        SetObjectScale(circle, scale, scale);
        
        // ÿTick��͸�����½�1��
        if (16 - i >= 0)
            SetObjectOpacity(circle, 16 - i); 
    }
    FreeObject(circle);     // ���ٹ�����
    
    // �佫�����岿�ֽ�������һ���ʱ�䣬�Ե���Ȼһ��
    Delay(10);
    
    // �佫����������
    SetOverwhelming(intvAttackerMajor, 0);  // �����佫�޵�״̬
    
    RaiseBrightness(16, 5);     // ��ս������
    EnablePlayMagic();          // ����˫���佫���佫��
}

// =====================================================

// ָ��ʿ����16 Tick�ڽ���
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

// �������Ҳ�����soldierCount��ʿ��
void CreateSoldierFrontBack(int soldierCount, int onLeft) {
    PlaySound1("m002snd01", 255);
    PlaySound1("m002snd03", 255);
    
    // ��ʼ�����꣺�����е�
    int y = GetBattleHeight() / 2;
    int x;
    if (onLeft) x = 0;
    else x = GetBattleWidth() - 1;
    
    // ѭ�����������б�Ϊֹ
    int soldier, existing;
    int opposingY;
    while(soldierCount > 0) {
        // ���ս���ϰ벿�ֵ�λ��
        existing = GetForceHandle(x, y);
        if (existing == 0) {
            soldier = CreateSoldier(intvIsLeft, x, y);
            asynccall MyStepShow(soldier);
            soldierCount--;
        }
        // ����Ƿ��������
        if (soldierCount <= 0)
            break;
            
        // ���ԳƵ��°벿�ֵ�λ��
        opposingY = GetBattleHeight() - 1 - y;
        existing = GetForceHandle(x, opposingY);
        if (existing == 0) {
            soldier = CreateSoldier(intvIsLeft, x, opposingY);
            asynccall MyStepShow(soldier);
            soldierCount--;
        }
        
        // ����(x, y)λ�ã���������ͻ��У�����ͬ�������·�˳��
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

// ǰ�����������������������ǰ�����ͺ�����ı���
void MoreSoldierFrontBack(int frontSolCount, int backSolCount) {
    DisablePlayMagic();
    DownBrightness(12, 5);
    
    SetOverwhelming(intvAttackerMajor, 1);
    int x = GetObjectScreenX(intvAttackerMajor);
    int y = GetObjectScreenY(intvAttackerMajor);
    SetViewCamera(x, y - 120); //�������׼�佫
    
    AddAttackCounter(intvAttackerMajor, 2);
    Delay(15);
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL2); // ����������һ��
    Delay(20);
    SetOverwhelming(intvAttackerMajor, 0);
    
    // �õ�������б���
    int level = GetMajorLevel(intvIsLeft);
    if (level > 40) level = 40;
    int soldiersAllowed = level * 5 - GetSoldierCount(intvIsLeft);
    
    if (soldiersAllowed <= 0) {
        // �������ٻ�����
        CastFail(intvAttackerMajor, GetObjectDir(intvAttackerMajor));
        
    } else {
        // �޷������������������ǰ�����
        int total = frontSolCount + backSolCount;
        if (soldiersAllowed < frontSolCount + backSolCount) {
            frontSolCount = soldiersAllowed * frontSolCount / total;
            backSolCount = soldiersAllowed - frontSolCount;
        }
    
        if (intvIsLeft) {
            CreateSoldierFrontBack(frontSolCount, 1); // ���ٻ������
            MoveCamera(240, 0, 25);  // ������ƹ�ȥ
            Delay(80);             // ��һ���
            
            CreateSoldierFrontBack(backSolCount, 0); // ���ٻ��ҷ���
            MoveCamera(GetBattleWidthInScreenX() - 240, 0, 25); 
            Delay(80);
            
        } else {
            // ����
            CreateSoldierFrontBack(frontSolCount, 0);
            MoveCamera(GetBattleWidthInScreenX() - 240, 0, 25);
            Delay(80);
            
            CreateSoldierFrontBack(backSolCount, 1);
            MoveCamera(240, 0, 25);
            Delay(80);
        }
        MoveCamera(x, y - 120, 25); //������ƻ��佫
    }
    
    RaiseBrightness(16, 5);
    EnablePlayMagic();
}


// =====================================================

// �ж�Ч��
void PoisonHarm(int target, int attackerMajor, 
        int attackValue, int interval, int times) {
    while(times > 0) {
        Delay(interval);
        
        // ����˺�������佫�����еĶ���������
        DoHarmToMajor(target, attackerMajor, attackValue);
        SetObjectAnimate(target, OAF_PAIN);
        PlaySound(target, "Yell02", 180);
        
        // �����ж���Ч����
        int effect = CreateObjectByReference(target, 13102, 0, 0);
        SetCoordinateByReference_Cylind(effect, target,
            GetObjectDir(target), -10, 70);
        SetObjectFadeOut(effect, 12, 1);
        
        times--;
    }
}
        
// ���������Ļص�����
void PoisonArrowCallback(int arrow, int target) callsign 11401{
    if (target == intvDefenderMajor) {
        // ���ټ�֧�͹�â
        SetObjectFadeOut(arrow, 2, 1);
        // ����GetObjectContext����ȡ����������
        int light = GetObjectContext(arrow, 0);
        SetObjectFadeOut(light, 2, 1);
        
        // �������������֮һ�˺�
        Hurt(arrow, intvDefenderMajor, intvMagicAttackValue / 3);
        
        // �����ж�Ч������������ж��˺�
        asynccall PoisonHarm(target, intvAttackerMajor,
            intvMagicAttackValue * 2 / 3 / 5, 120, 5);
        
        // ��������Ч��
        HitGeneral(arrow, target, 13102, 1, 60, 0);
        
    }
}

// ��ͷ�������
void LockCameraOnObject(int object, int yOffset) {
    int dir = GetObjectDir(object);
    while(IsObjectExist(object)) {
        // �õ����������������������x����
        int objectX = GetObjectScreenX(object);
        int cameraX = GetCameraX();
        
        // ���߽磺����ɳ�ս������ֱ���˳�
        if (cameraX < 0 || cameraX >= GetBattleWidthInScreenX())
            break;
            
        // ��������������ľ���
        int dx = objectX - cameraX;
        // ���������ڷ�������������ƶ������������ʲôҲ����
        if ((dir == 0 && dx > 0) || (dir == 128 && dx < 0))
            SetCameraByReference(object, 0, yOffset);
        Delay(1);
    }
}

// ����������
void ShootPoisonArrow() {
    DisablePlayMagic();
    DownBrightness(8, 5);
    SetOverwhelming(intvAttackerMajor, 1);
    
    // ���ó�ʼ�����λ�ã����������ķ�����ƫ��180����
    if (intvIsLeft)
        SetCameraByReference(intvAttackerMajor, 180, -120);
    else
        SetCameraByReference(intvAttackerMajor, -180, -120);
    Delay(15);
    SetOverwhelming(intvAttackerMajor, 0);
    
    // �����鹭
    SetObjectAnimate(intvAttackerMajor, OAF_SHOT1);
    Delay(15);
    PlaySound(intvAttackerMajor, "m003snd03", 255);
    
    // ������֧�����â�����������
    int dir = GetObjectDir(intvAttackerMajor);
    int arrow = CreateObjectByReference(intvAttackerMajor,
        13001, dir, 0);
    SetCoordinateByReference_Cylind(arrow, intvAttackerMajor, 
        dir, 80, 80);
    int light = CreateObjectByReference(arrow, 13101, dir, 0);
    SetObjectSpeed_Cylind(arrow, 16, 0);
    SetObjectSpeed_Cylind(light, 16, 0);
    
    // ��ͷ������֧
    asynccall LockCameraOnObject(arrow, -120);
    
    // ��֧���ϴ��Ŀ��
    SetObjectFlags(arrow, OF_ENEMYGENERAL);
    // ���ûص�����
    SetCallbackProcedure(arrow, 11401);
    // ���ü�֧�����0�Ŷ������Ϊ��â��������ڻ���Ŀ��ʱ���٣�
    SetObjectContext(arrow, 0, light);
    
    // �ȴ�LockCameraOnObject�����˳�
    Wait("LockCameraOnObject");
    Delay(60);
    
    // �佫������
    RaiseBrightness(16, 5);
    EnablePlayMagic();
}

// =====================================================

// �ص�����
void GroundFireCallback(int object, int target) callsign 11501 {
    if (target == intvDefenderMajor) {
        // ������������45��ȫ�ֱ����л�ȡ�����˺���
        // ����ÿ����һ���˺����룬��һ�����ֻ�ܶ��������һ���˺�
        int harm = GetGlobal(45);
        if (harm > 0) {
            DoHarmToMajor(target, 0, harm);
            SetGlobal(45, harm / 2);
            ClearObjectFlags(object, OF_ENEMYGENERAL);
            HitGeneral(object, target, 11002, 1, 48, 0);
        }
        
    } else {
        // ����ʿ����ʿ���Ż����
        asynccall FireMan(target, 10015, 60);
    }
}

// ������ص����ҵ����Ե�һ�����ҵ������ҽ��غͱ�ըЧ��
void TraceLavaSplit(int centerX, int centerY, int lavaSplit, int splitNo) {

    // �ȴ����ҵ����
    do {
        Delay(1);
    } while(GetObjectScreenZ(lavaSplit) > 0);
    
    // ��ȡ���ҵ�������꣬������
    int x = GetObjectScreenX(lavaSplit);
    int y = GetObjectScreenY(lavaSplit);
    FreeObject(lavaSplit);
    
    // ������ǵ�һ�����ҵ�������������ֱ�ӷ���
    if (splitNo != 0) return;
    
    // �����ذ��ϵĻ��沢����
    int fire = CreateObjectRaw(x, y, 0, 0, 11541);
    SetObjectScale(fire, 0x10000 / 3, 0x10000 / 6);
    
    // �����ϵĻ�������ɱ��Ч���������ûص�����
    SetObjectFlags(fire, OF_ATTACKENEMY);
    SetCallbackProcedure(fire, 11501);
    
    // �������ҳأ����һ�����ҳر�ţ����ҳ������ҵ���ض�������
    int poolNo = Rand(1, 6);
    int pool = CreateObjectRaw(x, y, 0, 0, 11520 + poolNo);
    int poollava = CreateObjectRaw(x, y, 0, 0, 11530 + poolNo);
    for (int i = 0; i < 6; i++) {
        SetObjectScale(pool, 0x10000 / 10 * i, 0x10000 / 10 * i);
        SetObjectScale(poollava, 0x10000 / 10 * i, 0x10000 / 10 * i);
        Delay(1);
    }
    
    // 6 Tick����ȫ�����ҵ���أ����챬ը
    int bomb = CreateObjectRaw(x, y, 0, 0, 11540);
    SetObjectScale(bomb, 0x10000 * 11 / 16, 0x10000 * 11 / 16);
    // ��ը��ɱ��Ч���ͻص�����
    SetObjectFlags(bomb, OF_ATTACKENEMY);
    SetCallbackProcedure(bomb, 11501);
    
    // ��ըЧ��26 Tick��ʼ����
    Delay(26);
    SetObjectFadeOut(bomb, 16, 1);
    
    // ���ҳغ͵������66 Tick��ʼ���������ɱ�˱��
    Delay(40);
    ClearObjectFlags(fire, OF_ATTACKALL);
    SetObjectFadeOut(fire, 12, 1);
    SetObjectFadeOut(pool, 12, 1);
    SetObjectFadeOut(poollava, 12, 1);
}

// �׳����ҵ�
void ThrowLava(int centerX, int centerY, int dir, int phi, int initSpeed) {
    // ��������6�����ҵ����γ�һ����
    for (int i = 0; i < 6; i++) {
        // �������ҵ�
        int lavaSplit = CreateObjectRaw(centerX, centerY, 0, 0, 43004);
        // ���ո�������ά����ͳ��ٶ�����
        SetObjectSpeed_Sphere(lavaSplit, dir, phi, initSpeed);
        // �������ҵ�������
        SetObjectHasGravity (lavaSplit, 1);
        SetObjectGravity(lavaSplit, 0x10000 * 3 / 10);
        // ���ò�͸���ȣ�Խ����β����Խ͸��
        SetObjectOpacity(lavaSplit, 16 - i * 2);
        
        // ʹ��TraceLavaSplit�������������ҵ���λ�ã����ʱ����
        // �����ݵڼ������ҵ������ҽ��غͱ�ը
        asynccall TraceLavaSplit(centerX, centerY, lavaSplit, i);
        
        // �ӳ�1 Tick������һ�����γ���β
        Delay(1);
    }
}

// ������Ѻ�ѭ���緢���ҵ�
void CreateVolcano(int centerX, int centerY) {
    
    // ����������������ѣ���������ҽ��������������
    int leakNo = Rand(1, 4);
    int leak = CreateObjectRaw(centerX, centerY, 0, 0, 11500 + leakNo);
    int leaklava = CreateObjectRaw(centerX, centerY, 0, 0, 11510 + leakNo);
    int fire = CreateObjectRaw(centerX, centerY, 0, 0, 19002);
    SetObjectScale(fire, 0x10000 * 3 / 2, 0x10000 * 3 / 2);
    
    // ��������ɱ��
    SetObjectFlags(fire, OF_ATTACKENEMY);
    SetCallbackProcedure(fire, 11501);
    
    // ���������ҽ�
    for(int i = 0; i < 24; i++) {
        asynccall ThrowLava(centerX, centerY, 
            Rand(0, 255), Rand(40, 60), Rand(10, 15));
        Delay(6);
    }
    
    // �ȴ�����������أ����ҳ���ʧ
    Wait("TraceLavaSplit");
    
    // ȡ��������ɱ��
    ClearObjectFlags(fire, OF_ATTACKENEMY);
    
    // ���ѡ������ҽ�����������ʧ
    SetObjectFadeOut(fire, 3, 1);
    SetObjectFadeOut(leak, 16, 1);
    SetObjectFadeOut(leaklava, 16, 1);
}

// ����Ч������ͷҡ�κ���Ч
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

// �ػ�������
void GroundFire(int level) {
    BatchLoadShape("magic\\115\\*");
    BatchLoadShape("magic\\033\\*");
    DisablePlayMagic();
    DownBrightness(12, 5);
    
    SetOverwhelming(intvAttackerMajor, 1);
    int x = GetObjectScreenX(intvAttackerMajor);
    int y = GetObjectScreenY(intvAttackerMajor);
    SetViewCamera(x, y - 120); //�������׼�佫
    
    AddAttackCounter(intvAttackerMajor, 2);
    Delay(15);
    SetObjectAnimate(intvAttackerMajor, OAF_SPELL2); // �佫������һ��
    Delay(20);
    SetOverwhelming(intvAttackerMajor, 0);
    
    // ��ʼ�����佫�˺�
    SetGlobal(45, intvMagicAttackValue);
    
    // ����Ч��
    asynccall ShakeCameraAndQuakeSound(80 * (level - 1) + 260);
    
    for (int i = 0; i < level; i++) {
        // �õ��ػ��緢�㣨��Ⱥ�ܼ�����
        int centerSoldier = GetMostImportantSoldier(!intvIsLeft);
        if (centerSoldier == 0)
            centerSoldier = intvDefenderMajor;
        
        int centerX = GetObjectScreenX(centerSoldier);
        int centerY = GetObjectScreenY(centerSoldier);
        
        // ��������������
        if (centerSoldier == intvDefenderMajor) {
            if (Rand(0, 1) == 1) centerX += Rand(100, 200);
            else centerX -= Rand(100, 200);
            if (Rand(0, 1) == 1) centerY += Rand(50, 130);
            else centerY -= Rand(50, 130);
        }
        
        // ��������ƹ�ȥ
        MoveCamera(centerX, centerY - 120, 12);
        
        // ����ػ�
        asynccall CreateVolcano(centerX, centerY);
        Delay(68);
    }
    
    // �ȴ����еػ���أ��ҽ�����ʧ
    Wait("TraceLavaSplit");
    Delay(20);
    
    // �佫������
    RaiseBrightness(16, 5);
    EnablePlayMagic();
}

// =====================================================

// ����ָ��ʿ��
void AscendSoldier(int soldier) {
    // ɱ��ʿ��
    KillForce(soldier);
    // ȡ���������
    SetObjectHasGravity(soldier, 0);
    // �ֶ�ÿ֡��������ٶȣ�velocity��0x10000Ϊ1�����ٶ�Ϊ0.3
    int velocity = 0;
    for (int i = 0; i < 100; i++) {
        velocity += 0x10000 / 3;
        SetObjectSpeed_Cylind(soldier, 0, velocity / 0x10000);
        Delay(1);
    }
    // ��ʿ������
    SetObjectFlags(soldier, OF_MIXER);
    for (i = 16; i >= 0; i--) {
        SetObjectOpacity(soldier, i);
        Delay(1);
    }
    FreeObject(soldier);
}

// ����������
void Ascension() {
    DisablePlayMagic();
    DownBrightness(8, 5);
    SetOverwhelming(intvAttackerMajor, 1);
    
    SetCameraByReference(intvAttackerMajor, 0, -120);
    Delay(15);
    SetOverwhelming(intvAttackerMajor, 0);
    
    // �ж϶Է���û��ʿ��
    if (GetSoldierCount(!intvIsLeft) > 0) {
    
        // ��öԷ��������ϣ��ǰ��ʿ����ս��X����
        int maxX = ScreenXToBattleX(GetSoldierMaxX2(intvIsLeft));
        
        // �ڸ�ʿ�������9���ڣ���ȡ���ез�ʿ������������Ŀ
        int targetCount = GetForceCountInRect(!intvIsLeft, maxX, 7, 
                9, GetBattleHeight());
        
        // ��ͬ���������ڣ�ɸѡ������ʿ����������targets������
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
        
        // �����������
        int temp;
        for (i = 0; i < targetCount - 1; i++) {
            j = Rand(i + 1, targetCount - 1);
            temp = targets[i];
            targets[i] = targets[j];
            targets[j] = temp;
        }
        
        // �ƶ�����������ʿ������
        int cameraFocus = targets[Rand(0, targetCount)];
        MoveCamera(GetObjectScreenX(cameraFocus), 
                   GetObjectScreenY(cameraFocus) -120, 
                   12);
        
        // ������Ч
        PlaySound1("m020snd01", 180);
        
        // �佫������ʱ320 Tick����ʿ���������
        int timeLeft = 320;
        for (i = 0; i < targetCount; i++) {
            asynccall AscendSoldier(targets[i]);
            
            int interval = Rand(2, 3);
            timeLeft -= interval;
            Delay(interval);
        }
        Delay(timeLeft);
        
    } else {
        // �ٻ�����
        CastFail(intvAttackerMajor, GetObjectDir(intvAttackerMajor));
    }
    
    Delay(20);
    RaiseBrightness(16, 5);
    EnablePlayMagic();
}

// =====================================================
// ���佫������ں���

// Hello World
void Magic111() callsign 800 {
    HelloReal("Hello World!");	// ����ʵ��ִ�е��佫������
}

// ���
void Magic112() callsign 801 {
    Roar();  // ����ʵ��ִ�е��佫������
}

// ǰ�����
void Magic113() callsign 802 {
    // ����ʵ��ִ�е��佫������
    MoreSoldierFrontBack(intvMagicAttackValue, intvMagicAttackValue);
}

// ����
void Magic114() callsign 803 {
    ShootPoisonArrow();
}

// �ػ�ϵ
void Magic115() callsign 804 {
    GroundFire(1);
}
void Magic116() callsign 805 {
    GroundFire(2);
}
void Magic117() callsign 806 {
    GroundFire(3);
}

// ����
void Magic118() callsign 807 {
    Ascension();
}

// =====================================================

// ��ȱؚ���
void RoarGenSkill() {
    
    // �õ�����ԭ���A�O׃����ֵ
    int attackerMajor = GetScriptLinkedObject();
    int isLeft = GetForceSide(attackerMajor);
    int magicAttackValue = 10;
    
    // ʹ�Ãȴ�����õ��������������
    SangoObject* pDefenderMajorObject;
    if (isLeft) 
        pDefenderMajorObject = *(SangoObject**) (0x4A8A60 + 0x44);
    else pDefenderMajorObject = *(SangoObject**) (0x4B03A0 + 0x44);
    int defenderMajor = pDefenderMajorObject->dwHandle;
    
    SetObjectAnimate(attackerMajor, OAF_SPELL1); // ����������һ��
    
    // ���һ
    PlaySound(attackerMajor, "Roar01", 255);
    
    // ��v�����܇������оł����ӣ���ɚ���
    int majorBattleX = GetObjectBattleX(attackerMajor);
    int majorBattleY = GetObjectBattleY(attackerMajor);
    for (int i = majorBattleX - 1; i <= majorBattleX + 1; i++) {
        for (int j = majorBattleY - 1; j <= majorBattleY + 1; j++) {
            
            // �����o���������˫@ȡʿ��/����
            int target = GetForceHandle(i, j);
            
            // �ǲ����Լ��ˣ��Ǆtֱ�����^
            if (GetForceSide(target) == isLeft)
                continue;
            
            // �ǲ��ǌ���������
            if (target == defenderMajor) {
                // �ǣ��u�����
                DoHarmToMajor(target, 
                    attackerMajor, magicAttackValue);
            } else {
                // ���ǣ�����ʿ��
                KillForce(target);
            }
        }
    }
    
    // �Ӯ�Ч�����������h
    int circle = 
        CreateObjectByReference(attackerMajor, 18002, 0, 40);
    
    // �քӔUɢ���h��ע�⣬�����һ���ѽ����x�^i�����Դ�̎���ٶ��x
    for (i = 0; i < 20; i++) { 
        Delay(1);
        
        // ÿTick�Ŵ�3/16
        int scale = 0x10000 + 0x10000 * 3 * i / 16; 
        SetObjectScale(circle, scale, scale);
        
        // ÿTick��͸�����½�1�c
        if (16 - i >= 0)
            SetObjectOpacity(circle, 16 - i); 
    }
    FreeObject(circle);     // �N�����h
    
}

// �ؚ�������ں������{�ô��a55501
void GenSkill() callsign 55501 {
    // ��ͨ�^��������õ����ٷ�
    int attackerMajor = GetScriptLinkedObject();
    int isLeft = GetForceSide(attackerMajor);
    
    // �õ��䌢ָ�
    int general;
    if (isLeft) general = intvLeftGeneral;
    else general = intvRightGeneral;
    
    // �Д��Ƿ�鄢�䣬���Ǆtֱ���˳�����
    if (GetGeneralName(general) != "�B��")
        return;

    if (Rand(0, 9999) < 500) // �l�Ӹ���5%
        RoarGenSkill();  // �{�Ì��H���еıؚ�������

}

// =====================================================

// ������������
void HorseCharge() {
    // ��ȡʩ��ʿ������ʿ�������������泯�ķ���
    int soldier = GetScriptLinkedObject();
    int isLeft = GetForceSide(soldier);
    int dir = GetObjectDir(soldier);
    
    // ͨ���ڴ������ȡ�Է����������
    SangoObject* pDefenderMajorObject;
    if (isLeft) 
        pDefenderMajorObject = *(SangoObject**) (0x4A8A60 + 0x44);
    else pDefenderMajorObject = *(SangoObject**) (0x4B03A0 + 0x44);
    int defenderMajor = pDefenderMajorObject->dwHandle;

    // ��ȡʿ����ʼλ��
    int initX = GetObjectScreenX(soldier);
    int initBattleX = ScreenXToBattleX(initX);
    int initY = GetObjectScreenY(soldier);
    int initBattleY = ScreenYToBattleY(initY);
        
    // ����ʿ����ǰ������ʿ�����������ǲ����Լ���/�Է�������
    // ��ȷ��������
    int soldiersToCrush = 0;
    for (int i = 1; i <= 3; i++) {
        // �õ�ʿ��ǰ���ĵ�i��ʿ��������
        int nextForce;
        if (dir == 0) 
            nextForce = 
                GetForceHandle(initBattleX + i, initBattleY);
        else 
            nextForce = 
                GetForceHandle(initBattleX - i, initBattleY);
        
        // �ж��Ƿ����Լ���
        if (GetForceSide(nextForce) == isLeft) 
            break;
        
        // �ж��Ƿ��ǶԷ�����
        if (nextForce == defenderMajor)
            break;
        
        // �����ǣ���������1
        soldiersToCrush++;
    }
    
    // ������Ϊ0��ֱ���˳�
    if (soldiersToCrush == 0)
        return;
    
    // ȷ�����ľ�ͷ�����ꣻһ��ս��X�������96����Ļ����
    int targetX;
    if (dir == 0)
        targetX = initX + soldiersToCrush * 96;
    else
        targetX = initX - soldiersToCrush * 96;

    //====================================================
    
    // ʿ����������ʼ������ʿ��WALK����
    SetObjectAnimate(soldier, OAF_WALK);
    
    // �ֶ�������ÿ���ս���ϵ�һ����ʱ9 Tick
    for (i = 0; i < 9 * soldiersToCrush; i++) {
        
        // ���㵱ǰ֡��ʿ��X���꣬��ע���жϲ�Ҫ���ս����Ե
        int newX = initX + (targetX-initX) * i / (9 * soldiersToCrush);
        if (newX < 0) 
            newX = 0;
        else if (newX > GetBattleWidthInScreenX()) 
            newX = GetBattleWidthInScreenX();
        
        // ǿ���趨ʿ����λ�á�������ٶ�
        SetObjectDir(soldier, dir);
        SetObjectCoordinate(soldier, newX, initY, 0);
        SetObjectSpeed_Cylind(soldier, 0, 0);
        
        // �õ���ǰλ�ô���ʿ��
        int crushed = GetForceHandle(
            ScreenXToBattleX(newX), ScreenYToBattleY(initY));

        // �ж��ǲ����Լ���
        if (GetForceSide(crushed) != isLeft) {
            // �ж��Ƿ�Ϊ�Է��������Է���һ����������ɱ��ʿ��
            if (crushed != defenderMajor) 
                KillForce(crushed); 
        }
        
        // �ֶ��������裺�ȴ���һ֡
        Delay(1);
    }
    
    // �嵽Ŀ��λ�ú���EXE�Զ���ʿ�����ص�ԭ����λ�ã������κβ���
}

// ��������ں���
void Soldier04CavalrySpc() callsign 50201 {
    // ʩ�Ÿ���Ϊ100%
    if (Rand(0, 9999) < 10000)
        HorseCharge();
}
