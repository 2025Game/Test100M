#include "Task.h"
#include "TaskManager.h"

// コンストラクタ
Task::Task(int prio, int sortOrder)
	: m_isEnable(true)
	, m_isShow(true)
	, m_isKill(false)
	, m_prio(prio)
	, m_sortOrder(sortOrder)
{
	// TaskManagerのリストに自身を追加
	TaskManager::Instance()->Add(this);
}

// デストラクタ
Task::~Task()
{
	// TaskManagerのリストから自信を取り除く
	TaskManager::Instance()->Remove(this);
}

// 有効フラグを設定
void Task::SetEbnable(bool enable)
{
	m_isEnable = enable;
}
// 有効フラグを取得
bool Task::IsEnable() const
{
	return m_isEnable;
}

// 表示フラグを設定
void Task::SetShow(bool show)
{
	m_isShow = show;
}
// 表示フラグを取得
bool Task::IsShow() const
{
	return m_isShow;
}

// タスクを削除
void Task::Kill()
{
	m_isKill = true;
}
// 削除フラグが立っているか
bool Task::IsKill() const
{
	return m_isKill;
}

// 優先度を設定
void Task::SetPriority(int prio)
{
	m_prio = prio;

	// 一度リストから取り除いて、再度追加しなおすことで並び変える
	TaskManager::Instance()->Remove(this, true);
	TaskManager::Instance()->Add(this, true);
}
// 優先度を取得
int Task::GetPriority() const
{
	return m_prio;
}

// 優先度内の順番を設定
void Task::SetSortOrder(int order)
{
	m_sortOrder = order;

	// 一度リストから取り除いて、再度追加しなおすことで並び変える
	TaskManager::Instance()->Remove(this, true);
	TaskManager::Instance()->Add(this, true);
}
// 優先度内の順番を取得
int Task::GetSortOrder() const
{
	return m_sortOrder;
}

// 更新
void Task::Update()
{
}

// 描画
void Task::Render()
{
}
