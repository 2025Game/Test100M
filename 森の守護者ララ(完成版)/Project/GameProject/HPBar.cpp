#include "HPBar.h"
//#include <algorithm> // ここに移動
//#include <GL/gl.h>   // OpenGL用 (必要なら追加)

HPBar::HPBar(float max, float xPos, float yPos, float w, float h)
    : maxHP(max), currentHP(max), x(xPos), y(yPos), width(w), height(h) {
}

void HPBar::SetHP(float hp) {
    //currentHP = std::max(0.0f, std::min(hp, maxHP)); // std:: をつける
    currentHP = (hp < 0.0f) ? 0.0f : (hp > maxHP ? maxHP : hp);
}

void HPBar::Draw() {
    float hpRatio = currentHP / maxHP;

    // 背景バー（黒色）
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    // HPの部分（赤色）
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width * hpRatio, y);
    glVertex2f(x + width * hpRatio, y + height);
    glVertex2f(x, y + height);
    glEnd();
}
