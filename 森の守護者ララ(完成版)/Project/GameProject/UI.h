#pragma once

#ifndef UI_H
#define UI_H

/*
* UI
* ユーザーインターフェースクラス
*/

class UI
{
public:
    // コンストラクタ
    UI();

    //デストラクタ
    ~UI();

    // シーン
    // スタート画面
    void Start(const char* titleTexture, int gWidth, int gHeight);

    // ゲームクリア画面
    void Clear(const char* clearTexture, int gWidth, int gHeight);

    // ゲームオーバー
    void Over(const char* overTexture, int gWidth, int gHeight);

    // HPアイコンの設定
    void InitHp(const char* hpTexture, int maxHP, int iconWidth, int iconHeight);

    // マナアイコンの設定
    void InitMana(const char* manaTexture, int iconWidth, int iconHeight);

    // タイムの後ろの板の設定
    void InitTimeBack(const char* timeBackTexture, int iconWidth, int iconHeight);

    // 経過時間を設定する関数
    void SetElapsedTime(float time);

    // クリア時間を設定する関数
    void SetClearTime(float time);

    // UIの表示
    void Render();

    // クリア時間の表示
    void Render1();

    // HPを設定
    void SetHP(int hp); 

    // マナを設定
    void SetMana(int mana);
    
    // ボス関係
    void InitBossHPBar(const char* hpBarBackTexture, const char* hpBarFrontTexture, int barWidth, int barHeight);

    void SetBossHP(int hp);

    void RenderBossHP();

private:
    // シーン(追加)
    std::vector<CImage> m_titleG; // タイトル画面用
    std::vector<CImage> m_clearG; // クリア画面用
    std::vector<CImage> m_overG;  // オーバー画面用

    float m_elapsedTime; // 経過時間を保存
    float m_clearTime = 0.0f; // クリア時間を保存

    std::vector<CImage> m_hpIcons; // HPアイコン用
    int m_maxHP; // 最大HP
    int m_currentHP; // 現在のHP

    std::vector<CImage> m_manaIcons; // マナアイコン用
    int m_maxMana; // 最大マナ
    int m_currentMana; // 現在のマナ

    std::vector<CImage> m_timebackIcon; // タイムの後ろの板用

    // ボス関係
    std::vector<CImage> m_bossHPBarBack; // 背景バー用
    std::vector<CImage> m_bossHPBarFront; // 前面バー用
    int m_bossHPBarWidth; // ボスHPバーの最大幅
    int m_bossHPBarHeight; // ボスHPバーの高さ
    int m_maxBossHP; // 最大HP
    int m_currentBossHP; // 現在のHP
};

#endif
