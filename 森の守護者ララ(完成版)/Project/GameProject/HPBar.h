#pragma once

class HPBar {
private:
    float maxHP;
    float currentHP;
    float x, y;   // 位置
    float width, height; // サイズ
public:
    // デフォルトコンストラクタ
    HPBar() : maxHP(1.0f), currentHP(1.0f), x(0), y(0), width(100), height(10) {}

    HPBar(float max, float xPos, float yPos, float w, float h);

    void SetHP(float hp);
    void Draw();
};
