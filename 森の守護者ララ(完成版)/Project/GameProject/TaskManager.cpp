#include "TaskManager.h"
#include "Task.h"
#include "ObjectBase.h"

#include <algorithm>
#include <Player.h>
#include <Potion.h>
#include <Mana.h>
#include <Slime.h>
#include <Boss.h>


#undef min
#undef max

// TaskManagerのインスタンスの静的なメンバ変数を定義
TaskManager* TaskManager::ms_instance = nullptr;

// コンストラクタ
TaskManager::TaskManager()
{
}

// デストラクタ
TaskManager::~TaskManager()
{
	// 全てのタスクを削除
	DeleteAll();
}

// TaskManagerのインスタンスを取得
TaskManager* TaskManager::Instance()
{
	// インスタンスが生成されてなければ、インスタンス生成後に返す
	if (ms_instance == nullptr)
	{
		ms_instance = new TaskManager();
	}
	return ms_instance;
}

// TaskManagerのインスタンスを破棄
void TaskManager::ClearInstance()
{
	// インスタンスが生成されていたら、削除する
	if (ms_instance != nullptr)
	{
		delete ms_instance;
		ms_instance = nullptr;
	}
}

// タスクリストにタスクを追加
void TaskManager::Add(Task* task, bool sort)
{
	// 並び替え時の追加処理でなければ、
	if (!sort)
	{
		// 追加されるタスクがオブジェクトであれば、オブジェクトリストにも追加
		if (task->m_prio == (int)ETaskPrio::Object)
		{
			m_objectList.push_back(task);
		}
	}

	auto itr = m_taskList.begin();
	auto end = m_taskList.end();
	while (itr != end)
	{
		Task* curr = *itr;
		// 追加するタスクの優先度の値の方が小さい場合は、その位置にタスクを追加する
		if (task->m_prio < curr->m_prio)
		{
			m_taskList.insert(itr, task);
			return;
		}
		// 追加するタスクと現在調べているタスクの優先度が同じ場合は、m_sortOrder の順番でリストに追加する
		else if (task->m_prio == curr->m_prio)
		{
			if (task->m_sortOrder < curr->m_sortOrder)
			{
				m_taskList.insert(itr, task);
				return;
			}
		}
		++itr;
	}

	// リストの間に追加する場所が見つからなかったので、リストの一番最後に追加
	m_taskList.push_back(task);
}

// タスクリストからタスクを取り除く
void TaskManager::Remove(Task* task, bool sort)
{
	// 並び替え時でなければ
	if (!sort)
	{
		// オブジェクトのリストからも取り除く
		if (task->m_prio == (int)ETaskPrio::Object)
		{
			m_objectList.remove(task);
		}
	}

	m_taskList.remove(task);
}

// 全てのタスクを削除
void TaskManager::DeleteAll()
{
	// オブジェクトのリストも空にする
	m_objectList.clear();

	// タスクリストの先頭から順番に削除する
	auto itr = m_taskList.begin();
	auto end = m_taskList.end();
	while(itr != end)
	{
		// 削除するタスクのアドレスを一旦記憶し、リストから取り除いた後に、タスクを削除する
		Task* del = *itr;
		itr = m_taskList.erase(itr);
		delete del;
	}
}

// 削除フラグが立っているタスクを削除
void TaskManager::DeleteKilledTasks()
{
	// タスクリストの先頭から順番に調べる
	auto itr = m_taskList.begin();
	auto end = m_taskList.end();
	while (itr != end)
	{
		// 削除フラグが建っていたら、削除する
		Task* task = *itr;
		if (task->IsKill())
		{
			Task* del = *itr;
			itr = m_taskList.erase(itr);
			delete del;
		}
		// 削除フラグが立っていなければ、次のタスクを調べる
		else
		{
			++itr;
		}
	}
}

// リストに登録されているタスクを更新
void TaskManager::Update()
{
	// 削除フラグが立っているタスクを削除
	DeleteKilledTasks();

	// リストの先頭から順番に更新処理を呼び出す
	for (Task* task : m_taskList)
	{
		// タスクが有効であれば、更新
		if (task->IsEnable())
		task->Update();
	}
}

// リストに登録されているタスクを描画
void TaskManager::Render()
{
	// オブジェクトリストの中身を奥行きの座標が大きい順に並び変える
	m_objectList.sort
	(
		[](const Task* task0, const Task* task1)
		{
			const ObjectBase* obj0 = dynamic_cast<const ObjectBase*>(task0);
			const ObjectBase* obj1 = dynamic_cast<const ObjectBase*>(task1);
			return obj0->GetPos().z < obj1->GetPos().z;
		}
	);

	// 並び変えたオブジェクトリストの先頭から順番に SortOrder の値を設定
	int sortOrder = 0;
	for (Task* obj : m_objectList)
	{
		obj->SetSortOrder(sortOrder);
		sortOrder++;
	}

	// リストの先頭から順番に描画処理を呼び出す
	for (Task* task : m_taskList)
	{
		// タスクが有効かつ、表示状態ならば、描画
		if (task->IsEnable() && task->IsShow())
		task->Render();
	}
}

void TaskManager::CheckCollisions()
{
	// m_objectList内の総当たりチェック
	for (auto itrA = m_objectList.begin(); itrA != m_objectList.end(); ++itrA)
	{
		for (auto itrB = std::next(itrA); itrB != m_objectList.end(); ++itrB)
		{
			ObjectBase* objA = dynamic_cast<ObjectBase*>(*itrA);
			ObjectBase* objB = dynamic_cast<ObjectBase*>(*itrB);

			if (objA && objB)
			{
				if (dynamic_cast<Player*>(objA))
				{

				}
				else
				{
					ObjectBase* objC = objB;
					objB = objA;
					objA = objC;
				}
				// 衝突判定                // 3/17変更（西川）
				if (IsColliding(objA, objB))
				{
					if (dynamic_cast<Player*>(objA))
					{
						
						if (dynamic_cast<Potion*>(objB))
						{
							Player* player = dynamic_cast<Player*>(objA);
							Potion* potion = dynamic_cast<Potion*>(objB);

							// 衝突解消（押し出し処理）を実行
							//ResolveCollision(objA, objB);

							player->TakeHeal(1); // 回復値を仮に1とする
							potion->Alpha0();
						}
						//ーーーーーーーーーーーーーーーーーーーーーーーーーーここから
						else if (dynamic_cast<Mana*>(objB))
						{
							Player* player = dynamic_cast<Player*>(objA);
							Mana* mana = dynamic_cast<Mana*>(objB);

							// m_objectList内からBossクラスのオブジェクトを探す
							for (auto& obj : m_objectList)
							{
								// dynamic_castでBossクラスかどうかを確認
								Boss* boss = dynamic_cast<Boss*>(obj);
								if (boss)
								{
									// Bossオブジェクトが見つかった場合の処理
									Boss* boss = dynamic_cast<Boss*>(obj);
									boss->TakeDamage(1);
									mana->Alpha0();
									player->Attack(); // 攻撃状態に移行
								}
							}
						}
						//ーーーーーーーーーーーーーーーーーーーーーーーーーーーここまで
						else
						{
							Player* player = dynamic_cast<Player*>(objA);
							player->TakeDamage(1); // ダメージ値を仮に1とする
							// 衝突解消（押し出し処理）を実行
							ResolveCollision(objA, objB);
						}
					}
				}
			}
		}
	}
}

bool TaskManager::IsColliding(ObjectBase* objA, ObjectBase* objB)
{
	// 各オブジェクトのCHIP_SIZEを取得
	float chipSizeAx = objA->GetChipSizeX();
	float chipSizeAy = objA->GetChipSizeY();
	float chipSizeAz = objA->GetChipSizeZ();
	float chipSizeBx = objB->GetChipSizeX();
	float chipSizeBy = objB->GetChipSizeY();
	float chipSizeBz = objB->GetChipSizeZ();

	// オブジェクトの位置を取得
	auto posA = objA->GetPos();
	auto posB = objB->GetPos();

	// AABBによる衝突判定
	
	bool isXOverlap = !(posB.x + chipSizeBx < posA.x || posB.x > posA.x + chipSizeAx);
	bool isZOverlap = !(posB.z + chipSizeBz < posA.z || posB.z > posA.z + chipSizeAz);
	bool isYOverlap = !(posB.y + chipSizeBy < posA.y || posB.y > posA.y + chipSizeAy);
	
	/*
	bool isXOverlap = (posA.x + chipSizeAx > posB.x) && (posA.x < posB.x + chipSizeBx);
	bool isZOverlap = (posA.z + chipSizeAz > posB.z) && (posA.z < posB.z + chipSizeBz);
	bool isYOverlap = (posA.y + chipSizeAy > posB.y) && (posA.y < posB.y + chipSizeBy);
	*/
	return isXOverlap && isZOverlap && isYOverlap;
}

void TaskManager::ResolveCollision(ObjectBase* objA, ObjectBase* objB)
{
	// 各オブジェクトのCHIP_SIZEを取得
	float chipSizeAx = objA->GetChipSizeX();
	float chipSizeAy = objA->GetChipSizeY();
	float chipSizeAz = objA->GetChipSizeZ();
	float chipSizeBx = objB->GetChipSizeX();
	float chipSizeBy = objB->GetChipSizeY();
	float chipSizeBz = objB->GetChipSizeZ();

	// 位置を取得
	auto posA = objA->GetPos();
	auto posB = objB->GetPos();

	// X方向とY方向の重なりを計算
	float overlapX = std::min(posB.x + chipSizeBx, posA.x + chipSizeAx) - std::max(posB.x, posA.x);
	float overlapZ = std::min(posB.z + chipSizeBz, posA.z + chipSizeAz) - std::max(posB.z, posA.z);
	float overlapY = std::min(posB.y + chipSizeBy, posA.y + chipSizeAy) - std::max(posB.y, posA.y);

	// 最小の重なり方向に基づいて押し出し処理を実行
	if (overlapX < overlapZ && overlapX < overlapY)
	{
		// X方向に押し出し
		if (posA.x < posB.x)
		{
			posA.x -= overlapX;
		}
		else
		{
			posA.x += overlapX;
		}
	}
	else if (overlapZ < overlapX && overlapZ < overlapY)
	{
		// Z方向に押し出し
		if (posA.z < posB.z)
		{
			posA.z -= overlapZ;
		}
		else
		{
			posA.z += overlapZ;
		}
	}
	else
	{
		// Y方向に押し出し
		if (posA.y < posB.y)
		{
			posA.y -= overlapY;
		}
		else
		{
			posA.y += overlapY;
		}
	}

	// 更新された位置をセット
	objA->SetPos(posA);
}