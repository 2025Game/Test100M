#include "UI.h"
#include "DebugPrint.h"
#include <cstdio>

// コンストラクタ
UI::UI() : m_elapsedTime(0), m_maxHP(3), m_currentHP(3), m_currentMana(0), m_maxBossHP(20), m_currentBossHP(20) {}

// デストラクタ
UI::~UI() {}

// シーン(追加)
// スタート画面の設定
void UI::Start(const char* titleTexture, int gWidth, int gHeight)
{
    CImage StartG;
    StartG.Load(titleTexture); // データの読み込み
    StartG.SetSize(gWidth, gHeight); // サイズを指定
    StartG.SetPos(0, 0); // 表示する場所(左上基準)

    m_titleG.push_back(StartG);

    m_titleG[0].Draw(); // 描画
}

// クリア画面の設定
void UI::Clear(const char* clearTexture, int gWidth, int gHeight)
{
    CImage ClearG;
    ClearG.Load(clearTexture); // データの読み込み
    ClearG.SetSize(gWidth, gHeight); // サイズを指定
    ClearG.SetPos(0, 0); // 表示する場所(左上基準)

    m_clearG.push_back(ClearG);

    m_clearG[0].Draw(); // 描画
}

// オーバー画面
void UI::Over(const char* overTexture, int gWidth, int gHeight)
{
    CImage OverG;
    OverG.Load(overTexture); // データの読み込み
    OverG.SetSize(gWidth, gHeight); // サイズを指定
    OverG.SetPos(0, 0); // 表示する場所(左上基準)

    m_overG.push_back(OverG);

    m_overG[0].Draw(); // 描画
}

// HPアイコンの設定
void UI::InitHp(const char* hpTexture, int maxHP, int iconWidth, int iconHeight) {
    m_maxHP = maxHP;
    m_currentHP = maxHP;
    m_hpIcons.clear();

    // HPアイコンをロード
    for (int i = 0; i < maxHP; ++i) {
        CImage hpIcon;
        hpIcon.Load(hpTexture);
        hpIcon.SetSize(iconWidth, iconHeight);  // アイコンのサイズを設定
        hpIcon.SetPos(20 + i * 100, 30); // 左上に3つ並べる
        m_hpIcons.push_back(hpIcon);
    }
}

// マナアイコンの設定
void UI::InitMana(const char* manaTexture, int iconWidth, int iconHeight) {
    m_currentMana = 0;  // 初期マナは 0
    m_manaIcons.clear();

    CImage manaIcon;
    manaIcon.Load(manaTexture);
    manaIcon.SetSize(iconWidth, iconHeight);
    manaIcon.SetPos(20, 130); // マナアイコンの位置（HPの下）

    m_manaIcons.push_back(manaIcon);
}

// タイムの後ろの板の設定
void UI::InitTimeBack(const char* timebackTexture, int iconWidth, int iconHeight)
{
    CImage timebackIcon;
    timebackIcon.Load(timebackTexture);
    timebackIcon.SetSize(iconWidth, iconHeight);
    timebackIcon.SetPos(1280, 20);

    m_timebackIcon.push_back(timebackIcon);
}

// 経過時間を設定する関数
void UI::SetElapsedTime(float time)
{
    m_elapsedTime = time;
}

// クリア時間を設定
void UI::SetClearTime(float time)
{
    m_clearTime = time;
}

// UIの表示
void UI::Render()
{
    // タイムの表示より前に後ろの画像を描画
    m_timebackIcon[0].Draw();

    // 文字列の作成
    char timeText[32];
    sprintf_s(timeText, "Time:%.2f sec", m_elapsedTime);
    // 文字列を画面に表示
    // フォントインスタンスを作成(PCに入っているフォント)
    CREATE_FONT_F("customFont", "C:\\Windows\\Fonts\\msgothic.ttc", 80); // サイズ80のMSゴシックを使用
    // フォントを使って文字列を描画(横位置(左基準)、縦位置(上基準)、R、G、B、表示するテキスト)
    FONT("customFont")->Draw(1300, 100, 0.8f, 0.3f, 0.5f, timeText, "Time: %.2f sec", m_elapsedTime);
    //FONT("customFont")->Draw(1350, 100, 1.0f, 1.0f, 1.0f, timeText, "Time: %.2f sec", m_elapsedTime);

    // HPの変更
    for (int i = 0; i < m_maxHP; ++i) {
        if (i < m_currentHP)
        {
            // HPが残っているアイコンは通常の色で描画
            m_hpIcons[i].SetColor(1.0f, 1.0f, 1.0f, 1.0f); // 白色
        }
        else
        {
            // HPが減ったアイコンは黒く描画
            m_hpIcons[i].SetColor(0.1f, 0.1f, 0.1f, 0.8f); // 黒色
        }
        // アイコンを描画
        m_hpIcons[i].Draw();
    }

    /*// マナアイコン描画（1つだけ表示）
    if (m_currentMana > 0 && !m_manaIcons.empty())
    {
        // マナが残っている場合は通常の色で描画
        m_manaIcons[0].SetColor(1.0f, 1.0f, 1.0f, 1.0f); // 白色
    }
    else
    {
        // マナが消失した場合は黒く描画
        if (!m_manaIcons.empty())
        {
            m_manaIcons[0].SetColor(0.1f, 0.1f, 0.1f, 0.8f); // 黒色
        }
    }
    // マナアイコンを描画
    if (!m_manaIcons.empty())
    {
        m_manaIcons[0].Draw();
    }*/
}

// クリア時間の表示
void UI::Render1()
{
    // 文字列を作成
    char timeText[32];
    sprintf_s(timeText, "Time:%.2f sec", m_clearTime);

    // フォントインスタンスを作成（初回のみ）
    CREATE_FONT_F("customFont", "C:\\Windows\\Fonts\\msgothic.ttc", 100);

    // クリアタイムの表示
    FONT("customFont")->Draw(155, 690, 1.0f, 0.5f, 0.5f, timeText);
}

// HPを設定
void UI::SetHP(int hp)
{
    // HPが最大値を超えないように制限
    m_currentHP = (hp > m_maxHP) ? m_maxHP : hp;

    // HPが減ったらアイコンを黒く設定
    for (int i = 0; i < m_currentHP; ++i) {
        m_hpIcons[i].SetColor(1.0f, 1.0f, 1.0f, 1.0f);  // 白
    }
    for (int i = m_currentHP; i < m_maxHP; ++i) {
        m_hpIcons[i].SetColor(0.0f, 0.0f, 0.0f, 1.0f);  // 黒
    }
}

// マナを設定（1つまで）
void UI::SetMana(int mana)
{
    m_currentMana = (mana > 0) ? 1 : 0; // 1以上なら1に制限
}


// ボスHPバーの初期化
void UI::InitBossHPBar(const char* hpBarBackTexture, const char* hpBarFrontTexture, int barWidth, int barHeight)
{
    m_bossHPBarWidth = barWidth; // 最大幅を保存
    m_bossHPBarHeight = barHeight; // 高さを保存

    // 背景バー
    CImage bossHPBarBack;
    bossHPBarBack.Load(hpBarBackTexture);
    bossHPBarBack.SetSize(barWidth, barHeight);
    bossHPBarBack.SetPos(480, 80);  // 画面上部に配置
    m_bossHPBarBack.push_back(bossHPBarBack);

    // 前面バー（HPの減少を反映）
    CImage bossHPBarFront;
    bossHPBarFront.Load(hpBarFrontTexture);
    bossHPBarFront.SetSize(barWidth, barHeight);
    bossHPBarFront.SetPos(480, 80);
    m_bossHPBarFront.push_back(bossHPBarFront);
}

// ボスの HP を設定
void UI::SetBossHP(int hp)
{
    m_currentBossHP = (hp > m_maxBossHP) ? m_maxBossHP : hp;

    // HPの割合からバーの幅を計算
    float hpRatio = static_cast<float>(m_currentBossHP) / m_maxBossHP;
    int newWidth = static_cast<int>(hpRatio * m_bossHPBarWidth);

    if (!m_bossHPBarFront.empty()) {
        m_bossHPBarFront[0].SetSize(newWidth, m_bossHPBarHeight); // 高さは固定
    }
}

// ボスHPバーの描画
void UI::RenderBossHP()
{
    // 文字列を作成
    char timeText[32];
    sprintf_s(timeText, "DragonHP");

    // フォントインスタンスを作成（初回のみ）
    CREATE_FONT_F("customFont", "C:\\Windows\\Fonts\\msgothic.ttc", 50);

    // 文字の表示
    FONT("customFont")->Draw(500, 75, 1.0f, 0.0f, 0.0f, timeText);

    if (!m_bossHPBarBack.empty()) {
        m_bossHPBarBack[0].Draw();  // 背景を描画
    }
    if (!m_bossHPBarFront.empty()) {
        m_bossHPBarFront[0].Draw(); // HPバーを描画
    }
}