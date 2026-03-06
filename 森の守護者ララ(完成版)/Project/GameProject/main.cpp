#include "TaskManager.h"
#include "Player.h"
#include "Slime.h"
#include "Mosubi.h"
#include "Rock.h"
#include "Potion.h"
#include "Mana.h"
#include "Ground.h"                   // 3/18変更（西川）
#include "BGTree1.h"                   // 3/18変更（西川）
#include "BGTree2.h"                   // 3/18変更（西川）
#include "BGTree3.h"                   // 3/18変更（西川）
#include "BGFog.h"                   // 3/18変更（西川）
#include "Boss.h"

//UI関係
#include "UI.h"
#include <chrono>
#include "HPBar.h"
//

// 影関係
#include "Shadow1.h"
#include"Shadow2.h"

#include <cstdlib> // rand() と srand() のため
#include <ctime>   // time() のため

#include <iostream>
#include <thread>


//--------------------------------------------
//グローバル変数領域
//--------------------------------------------
Player* g_player = nullptr;			 // プレイヤーのポインタ
Slime* g_slime = nullptr;			 // キューゴンのポインタ
Mosubi* g_mosubi = nullptr;			 // モスビのポインタ
Rock* g_rock = nullptr;				 // 岩のポインタ
Potion* g_potion = nullptr;          // ポーションのポインタ
Mana* g_mana = nullptr;              // マナのポインタ
Boss* g_boss = nullptr;              // ボスのポインタ			// 3/18変更（西川）
Ground* g_ground = nullptr;// 地面画像のポインタ				// 3/18変更（西川）
BGTree1* g_bgTree1 = nullptr;// 背景1のポインタ					// 3/18変更（西川）
BGTree2* g_bgTree2 = nullptr;// 背景2のポインタ					// 3/18変更（西川）
BGTree3* g_bgTree3 = nullptr;// 背景3のポインタ					// 3/18変更（西川）
BGFog* g_bgFog = nullptr;// 背景4のポインタ       　            // 3/18変更（西川）
Shadow1* g_shadow1 = nullptr; // プレイヤーの影のポインタ
Shadow2* g_shadow2 = nullptr; // プレイヤー以外のすべての影のポインタ

// ゲームリセットに必要
//std::vector<Slime*> g_slimes;  // 複数のキュウゴンを保持する
//std::vector<Mosubi*> g_mosubis;  // 複数のモスビを保持する
//std::vector<Rock*> g_rocks;  // 複数の岩を保持する

std::vector<EnemyBase*> g_enemybases; // 全ての障害物を保持
std::vector<Shadow2*> g_shadow2s; // 全ての影(プレイヤー以外)を保持

// 追加
std::vector<Ground*> g_grounds; // 全ての障害物を保持
// 他のソースファイルで定義されている変数を使う
extern float testScrollX;
std::vector<BGTree1*> g_bgTree1s; // 全ての障害物を保持
std::vector<BGTree2*> g_bgTree2s; // 全ての障害物を保持
std::vector<BGTree3*> g_bgTree3s; // 全ての障害物を保持
std::vector<BGFog*> g_bgFogs; // 全ての障害物を保持
//

//int generatedCount = 0; // 障害物生成のカウント
float elapsedTime1;

// 経過時間を追跡するための変数
std::chrono::steady_clock::time_point lastGenerationTime;
float generationInterval = 3.0f; // 障害物生成の間隔（秒単位）
//int generatedCount = 0;


CImage* g_field = nullptr;	// フィールド画像のポインタ

// UI関係
UI* g_ui = nullptr;      // UI クラスのポインタ
std::chrono::steady_clock::time_point g_startTime; // ゲーム開始時間
//


// BGM関係(追加)
// グローバル変数としてBGMを管理する変数を追加
CSoundBase* g_bgm = nullptr;
bool isBGM = true; // BGMの判定
//

// シーン関係
enum class EState
{
	ESTART,  //ゲーム開始
	EPLAY,   //ゲーム中
	ECLEAR,  //ゲームクリア
	EOVER,   //ゲームオーバー
};

EState mState;

bool isGame; // ゲームの判定
bool isStart; // 経過時間の判定
bool isDeath = false; // 死亡の判定(仮)
bool isClear = false; // クリア時間の判定
float deathCount = 0.0f; // 死亡カウント(仮)
float g_clearTime = 0.0f; // ゲームクリア時の経過時間
bool isFinish = false; // クリアの判定(仮)
float finishCount = 0.0f; // クリアカウント(仮)
float ax;
int sN = 0;


// パターンの関数
void Pataan1()
{
	ax = 2880.0f;
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax - 70.0f) + (1920 * sN), -200.0f, -200.0f))); // 影の生成
	g_enemybases.push_back(new Slime(0, CVector3D((ax - 70.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_shadow2s.push_back(new Shadow2(0, CVector3D(ax + (1920 * sN), -200.0f, -50.0f))); // 影の生成
	g_enemybases.push_back(new Mosubi(0, CVector3D(ax + (1920 * sN), 160.0f, -80.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan2()
{
	ax = 2880.0f;
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax - 70.0f) + (1920 * sN), -200.0f, -200.0f))); // 影の生成
	g_enemybases.push_back(new Mosubi(0, CVector3D((ax - 70.0f) + (1920 * sN), 160.0f, -230.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D(ax + (1920 * sN), 0.0f, -80.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan3()
{
	ax = 2880.0f;
	g_enemybases.push_back(new Rock(0, CVector3D((ax - 70.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_shadow2s.push_back(new Shadow2(0, CVector3D(ax + (1920 * sN), -200.0f, -50.0f))); // 影の生成
	g_enemybases.push_back(new Mosubi(0, CVector3D(ax + (1920 * sN), 160.0f, -80.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan4()
{
	ax = 2880.0f;
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax - 70.0f) + (1920 * sN), -200.0f, -230.0f))); // 影の生成
	g_enemybases.push_back(new Slime(0, CVector3D((ax - 70.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D(ax + (1920 * sN), 0.0f, -80.0f)));
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax + 70.0f) + (1920 * sN), -200.0f, 100.0f))); // 影の生成
	g_enemybases.push_back(new Slime(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan5()
{
	ax = 2880.0f;
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax - 70.0f) + (1920 * sN), -200.0f, -230.0f))); // 影の生成
	g_enemybases.push_back(new Slime(0, CVector3D((ax - 70.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D(ax + (1920 * sN), 0.0f, -80.0f)));
	//g_enemybases.push_back(new Potion(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan6()
{
	ax = 2880.0f;
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax - 70.0f) + (1920 * sN), -200.0f, -200.0f))); // 影の生成
	g_enemybases.push_back(new Mosubi(0, CVector3D((ax - 70.0f) + (1920 * sN), 160.0f, -230.0f)));
	g_enemybases.push_back(new Mana(0, CVector3D((ax + 300.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D(ax + (1920 * sN), 0.0f, -80.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan7()
{
	ax = 2880.0f;
	g_enemybases.push_back(new Rock(0, CVector3D((ax - 70.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_shadow2s.push_back(new Shadow2(0, CVector3D(ax + (1920 * sN), -200.0f, -50.0f))); // 影の生成
	g_enemybases.push_back(new Mosubi(0, CVector3D(ax + (1920 * sN), 160.0f, -80.0f)));
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax + 70.0f) + (1920 * sN), -200.0f, 100.0f))); // 影の生成
	g_enemybases.push_back(new Slime(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan8()
{
	ax = 2880.0f;
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax - 70.0f) + (1920 * sN), -200.0f, -200.0f))); // 影の生成
	g_enemybases.push_back(new Mosubi(0, CVector3D((ax - 70.0f) + (1920 * sN), 160.0f, -230.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D(ax + (1920 * sN), 0.0f, -80.0f)));
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax + 70.0f) + (1920 * sN), -200.0f, 100.0f))); // 影の生成
	g_enemybases.push_back(new Slime(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan9()
{
	ax = 2880.0f;
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax - 70.0f) + (1920 * sN), -200.0f, -230.0f))); // 影の生成
	g_enemybases.push_back(new Slime(0, CVector3D((ax - 70.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D(ax + (1920 * sN), 0.0f, -80.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan10()
{
	ax = 2880.0f;
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax - 70.0f) + (1920 * sN), -200.0f, -230.0f))); // 影の生成
	g_enemybases.push_back(new Slime(0, CVector3D((ax - 70.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D(ax + (1920 * sN), 0.0f, -80.0f)));
	g_enemybases.push_back(new Mana(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan11()
{
	ax = 2880.0f;
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax - 70.0f) + (1920 * sN), -200.0f, -200.0f))); // 影の生成
	g_enemybases.push_back(new Mosubi(0, CVector3D((ax - 70.0f) + (1920 * sN), 160.0f, -230.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D(ax + (1920 * sN), 0.0f, -80.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan12()
{
	ax = 2880.0f;
	g_enemybases.push_back(new Potion(0, CVector3D((ax - 70.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D(ax + (1920 * sN), 0.0f, -80.0f)));
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax + 70.0f) + (1920 * sN), -200.0f, 100.0f))); // 影の生成
	g_enemybases.push_back(new Slime(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan13()
{
	ax = 2880.0f;
	g_enemybases.push_back(new Rock(0, CVector3D((ax - 70.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D(ax + (1920 * sN), 0.0f, -80.0f)));
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax + 70.0f) + (1920 * sN), -200.0f, 100.0f))); // 影の生成
	g_enemybases.push_back(new Slime(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan14()
{
	ax = 2880.0f;
	g_enemybases.push_back(new Potion(0, CVector3D((ax - 70.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D(ax + (1920 * sN), 0.0f, -80.0f)));
	g_enemybases.push_back(new Potion(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan15()
{
	ax = 2880.0f;
	g_enemybases.push_back(new Rock(0, CVector3D((ax - 70.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax + 70.0f) + (1920 * sN), -200.0f, 100.0f))); // 影の生成
	g_enemybases.push_back(new Slime(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
	g_enemybases.push_back(new Mana(0, CVector3D((ax + 400.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan16()
{
	ax = 2880.0f;
	g_enemybases.push_back(new Rock(0, CVector3D((ax - 70.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D(ax + (1920 * sN), 0.0f, -80.0f)));
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax + 70.0f) + (1920 * sN), -200.0f, 100.0f))); // 影の生成
	g_enemybases.push_back(new Slime(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
	g_enemybases.push_back(new Mana(0, CVector3D((ax + 400.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan17()
{
	ax = 2880.0f;
	g_enemybases.push_back(new Rock(0, CVector3D((ax - 70.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D(ax + (1920 * sN), 0.0f, -80.0f)));
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax + 70.0f) + (1920 * sN), -200.0f, 100.0f))); // 影の生成
	g_enemybases.push_back(new Slime(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
	g_enemybases.push_back(new Mana(0, CVector3D((ax + 400.0f) + (1920 * sN), 0.0f, 70.0f)));
	g_enemybases.push_back(new Potion(0, CVector3D((ax -100.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan18()
{
	ax = 2880.0f;
	g_enemybases.push_back(new Potion(0, CVector3D((ax - 100.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D((ax - 70.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_enemybases.push_back(new Mana(0, CVector3D((ax + 250.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_shadow2s.push_back(new Shadow2(0, CVector3D(ax + (1920 * sN), -200.0f, -50.0f))); // 影の生成
	g_enemybases.push_back(new Mosubi(0, CVector3D(ax + (1920 * sN), 160.0f, -80.0f)));
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax + 70.0f) + (1920 * sN), -200.0f, 100.0f))); // 影の生成
	g_enemybases.push_back(new Slime(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan19()
{
	ax = 2880.0f;
	g_enemybases.push_back(new Rock(0, CVector3D((ax - 70.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_enemybases.push_back(new Mana(0, CVector3D((ax + 250.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_shadow2s.push_back(new Shadow2(0, CVector3D(ax + (1920 * sN), -200.0f, -50.0f))); // 影の生成
	g_enemybases.push_back(new Mosubi(0, CVector3D(ax + (1920 * sN), 160.0f, -80.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
	g_enemybases.push_back(new Mana(0, CVector3D((ax + 400.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan20()
{
	ax = 2880.0f;
	g_enemybases.push_back(new Potion(0, CVector3D((ax - 70.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D(ax + (1920 * sN), 0.0f, -80.0f)));
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax + 300.0f) + (1920 * sN), -200.0f, -50.0f))); // 影の生成
	g_enemybases.push_back(new Slime(0, CVector3D((ax + 300.0f) + (1920 * sN), 0.0f, -80.0f)));
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax + 70.0f) + (1920 * sN), -200.0f, 100.0f))); // 影の生成
	g_enemybases.push_back(new Slime(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
	g_enemybases.push_back(new Mana(0, CVector3D((ax + 400.0f) + (1920 * sN), 0.0f, 70.0f)));
}

void GenerateObject()
{
	// ランダムの種を一度だけ設定
	static bool seedSet = false;
	if (!seedSet) 
	{
		srand(static_cast<unsigned int>(time(0))); // ランダムの種を設定
		seedSet = true;
	}

	// 0～7のランダムなパターンを選択
	int randomPattern = rand() % 20;
	//int randomPattern = 5;

	// ランダムなパターンに応じて関数を呼び出す
	switch (randomPattern) 
	{
	case 0: Pataan1(); break;
	case 1: Pataan2(); break;
	case 2: Pataan3(); break;
	case 3: Pataan4(); break;
	case 4: Pataan5(); break;
	case 5: Pataan6(); break;
	case 6: Pataan7(); break;
	case 7: Pataan8(); break;
	case 8: Pataan9(); break;
	case 9: Pataan10(); break;
	case 10: Pataan11(); break;
	case 11: Pataan12(); break;
	case 12: Pataan13(); break;
	case 13: Pataan14(); break;
	case 14: Pataan15(); break;
	case 15: Pataan16(); break;
	case 16: Pataan17(); break;
	case 17: Pataan18(); break;
	case 18: Pataan19(); break;
	case 19: Pataan20(); break;
	}

	sN++;

	//printf("Integer: %d\n", sN);
}


// ゲーム再スタート(リセット)用(Mainより後に呼ぶ(Init()やRelese()を呼ぶ)とエラーになるため)
void ResetGame()
{
	// プレイヤーの再生成
	if (g_player)
	{
		g_player->Kill();
	}
	g_player = new Player(CVector3D(SCREEN_WIDTH * 0.0f, 0.0f, -80.0f));

	// プレイヤーの影の再生成
	if (g_shadow1)
	{
		g_shadow1->Kill();
	}
	g_shadow1 = new Shadow1(0, CVector3D(0.0f, -300.0f, -140.0f));;

	// 障害物の削除
	// 生成されている全ての障害物を削除
	for (auto& enemybase : g_enemybases) 
	{
		enemybase->Kill();  // 既存のキュウゴンを削除
	}
	g_enemybases.clear();  // ベクターをクリアして、ポインタをリセット

	// キュウゴン/モスビの影の削除
	for (auto& shadow2 : g_shadow2s)
	{
		shadow2->Kill();
	}
	g_shadow2s.clear();

	// ここから追加
	// ボスの再生成
	if (g_boss)
	{
		g_boss->Kill();
	}
	g_boss = new Boss(CVector3D(100.0f, 400.0f, -335.0f));
	//

	// 背景の再生成
	///*// 生成されている全ての背景を削除
	for (auto& ground : g_grounds)
	{
		//delete ground;  // `delete` を使って確実にメモリ解放
		ground->Kill();  // 既存のキュウゴンを削除
	}
	g_grounds.clear();  // ベクターをクリアして、ポインタをリセット
	testScrollX = 0.0f;  // スクロール位置をリセット
	g_grounds.push_back(new Ground(0, CVector3D(0.0f, 0.0f, -320.0f)));
	g_grounds.push_back(new Ground(0, CVector3D(1920.0f, 0.0f, -320.0f)));
	g_grounds.push_back(new Ground(0, CVector3D(3840.0f, 0.0f, -320.0f)));

	for (auto& bgTree1 : g_bgTree1s)
	{
		//delete ground;  // `delete` を使って確実にメモリ解放
		bgTree1->Kill();  // 既存のキュウゴンを削除
	}
	g_bgTree1s.clear();  // ベクターをクリアして、ポインタをリセット
	testScrollX = 0.0f;
	g_bgTree1 = new BGTree1(0, CVector3D(0.0f, 0.0f, -330.0f));                   // 3/18変更（西川）
	g_bgTree1 = new BGTree1(0, CVector3D(1920.0f, 0.0f, -330.0f));                // 3/18変更（西川）
	g_bgTree1 = new BGTree1(0, CVector3D(3840.0f, 0.0f, -330.0f));                // 3/18変更（西川）

	for (auto& bgTree2 : g_bgTree2s)
	{
		//delete ground;  // `delete` を使って確実にメモリ解放
		bgTree2->Kill();  // 既存のキュウゴンを削除
	}
	g_bgTree2s.clear();  // ベクターをクリアして、ポインタをリセット
	testScrollX = 0.0f;
	g_bgTree2 = new BGTree2(0, CVector3D(0.0f, 0.0f, -340.0f));                   // 3/17変更（西川）
	g_bgTree2 = new BGTree2(0, CVector3D(1920.0f, 0.0f, -340.0f));                // 3/17変更（西川）
	g_bgTree2 = new BGTree2(0, CVector3D(3840.0f, 0.0f, -340.0f));                // 3/17変更（西川）

	for (auto& bgTree3 : g_bgTree3s)
	{
		//delete ground;  // `delete` を使って確実にメモリ解放
		bgTree3->Kill();  // 既存のキュウゴンを削除
	}
	g_bgTree3s.clear();  // ベクターをクリアして、ポインタをリセット
	testScrollX = 0.0f;
	g_bgTree3 = new BGTree3(0, CVector3D(0.0f, 0.0f, -350.0f));                   // 3/17変更（西川）
	g_bgTree3 = new BGTree3(0, CVector3D(1920.0f, 0.0f, -350.0f));                // 3/17変更（西川）
	g_bgTree3 = new BGTree3(0, CVector3D(3840.0f, 0.0f, -350.0f));                // 3/17変更（西川）

	for (auto& bgFog : g_bgFogs)
	{
		//delete ground;  // `delete` を使って確実にメモリ解放
		bgFog->Kill();  // 既存のキュウゴンを削除
	}
	g_bgFogs.clear();  // ベクターをクリアして、ポインタをリセット
	testScrollX = 0.0f;
	g_bgFog = new BGFog(0, CVector3D(0.0f, 0.0f, -360.0f));                   // 3/17変更（西川）
	g_bgFog = new BGFog(0, CVector3D(1920.0f, 0.0f, -360.0f));                // 3/17変更（西川）
	g_bgFog = new BGFog(0, CVector3D(3840.0f, 0.0f, -360.0f));                // 3/17変更（西川）
	//*/

	// UIの再生成
	delete g_ui;
	g_ui = new UI();
	g_ui->InitHp("HPUI.png", 3, 100, 100);  // HPアイコン画像をセット
	g_ui->InitMana("mana.png", 100, 100);  // マナアイコン画像をセット
	g_ui->InitTimeBack("timeback.png", 700, 120); // タイムの後ろの板画像をセット
	g_ui->InitBossHPBar("boss_hp_back.png", "boss_hp_front.png", 800, 50); // HPバーをセット
}

void MainLoop()
{
	//--------------------------------------------------------------
	//ゲーム中の動きはここに書く
	//ゲーム中はこの関数_を1秒間に60回呼び出している
	//--------------------------------------------------------------

	// シーン関係
	switch (mState)
	{
	case EState::ESTART: // ゲームスタート状態だったら、
		isGame = false; // ゲーム(MainLoop())を止める
		g_ui->Start("title.png", 1920, 1080); // タイトル画面の描画

		if (PUSH(CInput::eButton10)) // Enterキーが押されたら、
		{
			// BGM関係(追加)
			// タイトルBGMを停止
			SOUND("title_bgm")->Stop();
			// BGMのフラグをtrueに
			isBGM = true;
			//

			isStart = true; // 経過時間(タイム)を動かす
			mState = EState::EPLAY; // ゲームプレイ状態にする
		}
		break;
	case EState::EPLAY: // ゲームプレイ状態だったら、
		isGame = true; // ゲーム(MainLoop())を動かす

		// BGM関係(追加)
		// isBGMの判定
		if (isBGM)
		{
			// ゲームBGMを再生
			SOUND("game_bgm")->Play(true);
			isBGM = false;
		}
		//
		break;
	case EState::ECLEAR: // ゲームクリア状態だったら、
		isGame = false; // ゲーム(MainLoop())を止める

		g_ui->Clear("gameclear.png", 1920, 1080); // クリア画面の描画

		// BGM関係(追加)
		// isBGMの判定
		if (isBGM)
		{
			// クリアBGMを再生
			SOUND("clear_bgm")->Play(false);
			isBGM = false;
		}
		//

		// 経過時間を保存（初めてクリア状態になったときのみ）
		if (isClear)
		{
			// 現在の時間を取得
			auto now = std::chrono::steady_clock::now();

			// ゲーム開始時間からの経過時間を計算（秒単位）
			g_clearTime = std::chrono::duration<float>(now - g_startTime).count();
			isClear = false; // 再計測を防ぐ
		}

		// UIにクリア時間を表示
		g_ui->SetClearTime(g_clearTime);
		g_ui->Render1();

		/*// Enterキーを押すとゲーム終了
		if (PUSH(CInput::eButton10)) // Enterキーが押されたら
		{
			// **クリーンアップ処理**
			TaskManager::ClearInstance(); // ゲームのインスタンスを削除

			// **プログラムを安全に終了**
			PostQuitMessage(0); // Windowsアプリの正しい終了方法
			// exit(0); // 代わりに exit(0) でもOK
		}*/

		if (PUSH(CInput::eButton10)) // Enterキーが押されたら、
		{
			// **ゲームの初期化
			// ゲームのインスタンス削除
			//TaskManager::ClearInstance();

			// 新しいインスタンス作成
			//TaskManager::Instance();

			// ゲームの初期化
			ResetGame();

			// BGM関係(追加)
			// クリアBGMを停止
			SOUND("clear_bgm")->Stop();
			// BGMのフラグをtrueに
			isBGM = true;
			//

			// ゲームの状態を初期化
			deathCount = 0.0f;
			isDeath = false;
			isStart = true;

			sN = 0;

			// 必要なタスクを更新
			//TaskManager::Instance()->Update();

			mState = EState::EPLAY; // ゲームスタート状態にする
		}
		break;
	case EState::EOVER: // ゲームオーバー状態だったら、
		isGame = false; // ゲーム(MainLoop())を止める

		g_ui->Over("gameover.png", 1920, 1080); // クリア画面の描画

		// BGM関係(追加)
		// isBGMの判定
		if (isBGM)
		{
			// オーバーBGMを再生
			SOUND("over_bgm")->Play(false);
			isBGM = false;
		}
		//

		// Enterキーを押すとゲーム終了
		if (PUSH(CInput::eButton10)) // Enterキーが押されたら
		{
			// ゲームの初期化
			ResetGame();

			// BGM関係(追加)
			// クリアBGMを停止
			SOUND("over_bgm")->Stop();
			// BGMのフラグをtrueに
			isBGM = true;
			//

			// ゲームの状態を初期化
			deathCount = 0.0f;
			isDeath = false;
			isStart = true;

			sN = 0;

			mState = EState::EPLAY; // ゲームスタート状態にする

			/*
			// **クリーンアップ処理**
			TaskManager::ClearInstance(); // ゲームのインスタンスを削除

			// **プログラムを安全に終了**
			PostQuitMessage(0); // Windowsアプリの正しい終了方法
			// exit(0); // 代わりに exit(0) でもOK
			*/
		}

		break;
	}

	// 死亡判定(仮)
	if (isDeath)
	{
		// BGM関係(追加)
			// ゲームBGMを停止
		SOUND("game_bgm")->Stop();
		// BGMのフラグをtrueに
		isBGM = true;
		//
		//

		mState = EState::EOVER;
		isDeath = false;
	}

	// クリア判定(仮)
	if (isFinish)
	{
		// BGM関係(追加)
		// ゲームBGMを停止
		SOUND("game_bgm")->Stop();
		// BGMのフラグをtrueに
		isBGM = true;
		//
		//

		mState = EState::ECLEAR;
		isFinish = false;
	}

	// ゲーム中(EState::EPLAY)なら、以下を実行
	if (isGame)
	{
		// ゲーム中なら経過時間を最初に0にする
		if (isStart)
		{
			// ゲーム開始時間を記録
			g_startTime = std::chrono::steady_clock::now();
			//
			isStart = false;
		}


		// 現在の時間を取得
		auto now1 = std::chrono::steady_clock::now();
		std::chrono::duration<float> elapsedTime1 = now1 - lastGenerationTime; // 経過時間を計算

		// 一定時間ごとに障害物を生成
		if (elapsedTime1.count() >= generationInterval)
		{
			// 障害物を生成
			GenerateObject();

			// 最後の生成時間を更新
			lastGenerationTime = now1;
		}

		// UI関係
		// 経過時間を計算
		auto now = std::chrono::steady_clock::now();
		float elapsedTime = std::chrono::duration<float>(now - g_startTime).count();

		// UI に経過時間をセット
		g_ui->SetElapsedTime(elapsedTime);

		// 全てのタスクを更新
		TaskManager::Instance()->Update();

		//　全ての当たり判定のチェック
		TaskManager::Instance()->CheckCollisions();

		// フィールド画像を描画
		//g_field->Draw();

		// 影の描画
		//g_player->RenderShadow();
		/* 以下に修正
		g_slime->RenderShadow();
		g_mosubi->RenderShadow();*/
		
		// 影は一旦削除
		//for (auto& slime : g_slimes) {
		//	slime->RenderShadow();
		//}
		//for (auto& mosubi : g_mosubis) {
		//	mosubi->RenderShadow();
		//}
		//for (auto& enemybase : g_enemybases) {
				//enemybase->RenderShadow();
			//}

		// ここは後から修正
		//g_potion->RenderShadow1();
		//g_mana->RenderShadow1();
		//

		// 全てのタスクを描画
		TaskManager::Instance()->Render();

		// UI関係
		// UI の描画
		g_ui->Render();

		// ボスHPバーの表示
		g_ui->RenderBossHP();

		// HPを共有
		g_ui->SetHP(Player::Hp());

		// ボス用
		// HPを共有
		g_ui->SetBossHP(Boss::Hp());  // ボスのHPを設定

		// 死亡判定(仮)
		if (Player::Hp() <= 0)
		{
			deathCount++;
			if (deathCount >= 110.0f)
			{
				isDeath = true;
				deathCount = 0.0f;
			}
		}

		// クリア判定(仮)
		if (Boss::Hp() <= 0)
		{
			finishCount++;
			if (finishCount >= 60.0f)
			{
				isFinish = true;
				finishCount = 0.0f;

				isClear = true; // 再計算を防ぐ
			}
		}

		// デバッグ文字の描画
		DebugPrint::Render();
	}
}
void Init()
{
	CFPS::SetFPS(60);
	//フレーム制御初期化
	CFPS::Init();
	//ボタンの設定
	CInput::Init();
	CInput::SetButton(0, CInput::eButton1, 'Z');
	CInput::SetButton(0, CInput::eButton2, 'X');
	CInput::SetButton(0, CInput::eButton3, 'C');
	CInput::SetButton(0, CInput::eButton4, 'V');
	CInput::SetButton(0, CInput::eButton5, VK_SPACE);
	CInput::SetButton(0, CInput::eButton10, VK_RETURN);
	CInput::SetButton(0, CInput::eUp, VK_UP);
	CInput::SetButton(0, CInput::eDown, VK_DOWN);
	CInput::SetButton(0, CInput::eLeft, VK_LEFT);
	CInput::SetButton(0, CInput::eRight, VK_RIGHT);
	CInput::SetButton(0, CInput::eMouseL, VK_LBUTTON);
	CInput::SetButton(0, CInput::eMouseR, VK_RBUTTON);
	CInput::SetButton(0, CInput::eMouseC, VK_MBUTTON);
	//	CInput::SetMouseInside(true);
	//	CInput::ShowCursor(false);
	CInput::Update();
	CInput::Update();

	SetCurrentDirectory("data");
	CSound::GetInstance();

	//-----------------------------------------------------
	//初期化の命令を書く
	//ゲーム起動時に一度だけ呼ばれる
	//-----------------------------------------------------

	// シーン関係(追加)
	// 最初はタイトル画面を表示
	mState = EState::ESTART;

	// フィールドの画像を生成
	g_field = CImage::CreateImage("background.png");

	// プレイヤー/ボス/障害物/背景/UIの初期化
	ResetGame();


	// ポーションとマナの表示
	//g_potion = new Potion(0,
		//CVector3D(SCREEN_WIDTH * 2.00f, 0.0f, -80.0f));
	//g_mana = new Mana(0,
		//CVector3D(SCREEN_WIDTH * 2.40f, 0.0f, -80.0f));

	// BGM関係(追加)
	// 各BGMの読み込み
	// タイトルBGM
	SOUND("title_bgm")->Load("title.wav", 1, false); // 変更
	// ゲームBGM
	SOUND("game_bgm")->Load("game.wav", 1, false); // 変更
	// クリアBGM
	SOUND("clear_bgm")->Load("clear.wav", 1, false); // 変更
	// オーバーBGM
	SOUND("over_bgm")->Load("over.wav", 1, false); // 変更

	// 音量の調整（0.0f ～ 1.0f の範囲で指定）
	// タイトルBGM
	SOUND("title_bgm")->Volume(0.01f);  // 01%の音量
	// ゲームBGM
	SOUND("game_bgm")->Volume(0.01f);  // 01%の音量
	// クリアBGM
	SOUND("clear_bgm")->Volume(0.01f);  // 01%の音量
	// オーバーBGM
	SOUND("over_bgm")->Volume(0.01f);  // 01%の音量

	// タイトルBGMの再生
	SOUND("title_bgm")->Play(true);
	//
}


void Release()
{
	DebugPrint::ClearInstance();
	CLoadThread::ClearInstance();
	CSound::ClearInstance();
	CResourceManager::ClearInstance();
	TaskManager::ClearInstance();

	// UI関係(追加)
	// UI を解放
	delete g_ui;
	g_ui = nullptr;
	//
}

static void ResizeCallback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);

	//画面解像度変動
	CCamera::GetCamera()->SetSize((float)w, (float)h);
	//画面解像度固定
	//CCamera::GetCamera()->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	CCamera::GetCamera()->Viewport(0, 0, w, h);
	CCamera::GetCurrent()->Perspective(DtoR(60.0), (float)w / (float)h, 1.0, 1000.0);

	glfwGetWindowPos(window, &GL::window_x, &GL::window_y);
	GL::UpdateWindowRect(GL::window_x, GL::window_y, w, h);
	CInput::UpdateClipCursor(true);

}
static void WheelCallback(GLFWwindow* _window, double _offsetx, double _offsety)
{
	CInput::AddMouseWheel((int)_offsety);
}
static void PosCallback(GLFWwindow* _window, int x, int y)
{
	GL::window_x = x;
	GL::window_y = y;
	GL::UpdateWindosRect(x, y, GL::window_width, GL::window_height);
	CInput::UpdateClipCursor(true);
}
static void FocusCallback(GLFWwindow* _window, int f)
{
	CInput::UpdateClipCursor(f);
	GL::focus = f;
}

static void error_callback(int error, const char* description)
{
	printf("Error: %s\n", description);
}

//フルスクリーン?ウインドウモードの切り替え
//Alt+Enterで切り替える
void CheckFullScreen()
{
	static int key_enter = 0;
	int key_enter_buf = key_enter;
	if (key_enter_buf ^ (key_enter = glfwGetKey(GL::window, GLFW_KEY_ENTER)) && key_enter && (glfwGetKey(GL::window, GLFW_KEY_LEFT_ALT) || glfwGetKey(GL::window, GLFW_KEY_RIGHT_ALT)))
	{
		GL::ChangeFullScreen(!GL::full_screen);
	}
}

int __main(int* argcp, char** argv)
{
	// メモリリーク検出
	//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	//OpenGL4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) return -1;
	//	glutInit(argcp, argv);

	GL::window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Simple", nullptr, nullptr);
	glfwGetWindowSize(GL::window, &GL::window_width, &GL::window_height);
	glfwGetWindowPos(GL::window, &GL::window_x, &GL::window_y);

	glfwSetFramebufferSizeCallback(GL::window, ResizeCallback);
	glfwSetScrollCallback(GL::window, WheelCallback);
	glfwSetWindowFocusCallback(GL::window, FocusCallback);
	glfwSetWindowPosCallback(GL::window, PosCallback);
	if (!GL::window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(GL::window);
	glfwSwapInterval(1);
	ResizeCallback(GL::window, SCREEN_WIDTH, SCREEN_HEIGHT);

	GLenum err = glewInit();
	if (err == GLEW_OK)
	{
		printf("%s\n", glewGetString(GLEW_VERSION));
	}
	else
	{
		printf("%s\n", glewGetErrorString(err));
		getchar();
		return -1;
	}
	HDC glDc = wglGetCurrentDC();
	GL::hWnd = WindowFromDC(glDc);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);//ブレンドの有効化
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//固定シェーダー用
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_ALPHA_TEST);

	Init();
	while (!glfwWindowShouldClose(GL::window))
	{
		CheckFullScreen();


		CInput::Update();
		//各バッファーをクリア
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		MainLoop();


		glfwSwapBuffers(GL::window);


		CFPS::Wait();

		char title[32];
		sprintf_s(title, "Lara, Guardian of the Forest");
		glfwSetWindowTitle(GL::window, title);

		glfwPollEvents();
		if (glfwGetKey(GL::window, GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(GL::window, GL_TRUE);
		}

	}

	glfwTerminate();

	Release();
	return 0;
}

INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR argv, INT argc)
{
	return __main(&__argc, __argv);
}

int main(int argc, char** argv)
{
	return __main(&argc, argv);
}
