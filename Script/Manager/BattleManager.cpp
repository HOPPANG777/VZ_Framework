#include "stdafx.h"
#include "BattleManager.h"

BattleManager::BattleManager()
{
    // 전투 관련 초기화 코드 (예: 플레이어 및 적 유닛 생성)
	currentState = BattleState::PLAYER_COMMAND;
}

BattleManager::~BattleManager() 
{
    // 자원 정리
    for (std::shared_ptr<UnitBase>& p : playerUnits)
    {
        p.reset();
    }

    for (std::shared_ptr<UnitBase>& e : enemyUnits)
    {
        e.reset();
    }
}

void BattleManager::Initialize() 
{
    // 전투 씬이 시작될 때 호출, 유닛 생성 및 UI 초기화
    // 예: 
    // playerUnits.push_back(std::make_shared<PlayerUnit>(...));
    // enemyUnits.push_back(std::make_shared<EnemyUnit>(...));

    currentState = BattleState::PLAYER_COMMAND;
}

void BattleManager::Reset() 
{
    // 전투가 새로 시작되거나 재시작될 때 호출
    for (std::shared_ptr<UnitBase>& p : playerUnits)
    {
        p.reset();
    }
    
	for (std::shared_ptr<UnitBase>& e : enemyUnits)
	{
		e.reset();
	}

    currentState = BattleState::PLAYER_COMMAND;
}

void BattleManager::Update() 
{
    // 전투 진행 상태에 따라 각 단계의 로직을 처리
    switch (currentState)
    {
    case BattleState::PLAYER_COMMAND:
        ProcessPlayerCommand();
        break;
    case BattleState::PLAYER_ACTION:
        ProcessPlayerAction();
        break;
    case BattleState::ENEMY_ACTION:
        ProcessEnemyAction();
        break;
    case BattleState::TURN_END:
        EndTurn();
        break;
    case BattleState::VICTORY:
        // 승리 처리 (예: 보상, 씬 전환)
        break;
    case BattleState::DEFEAT:
        // 패배 처리 (예: 게임 오버, 재시도)
        break;
    default:
        break;
    }

    // 전투 중 유닛 업데이트 (필요시)
    for (std::shared_ptr<UnitBase>& unit : playerUnits) unit->Update();
    for (std::shared_ptr<UnitBase>& unit : enemyUnits) unit->Update();
}

void BattleManager::ProcessPlayerCommand() 
{
    // 플레이어 명령 선택 UI가 활성화된 상태로 대기
    // UI에서 OnPlayerCommandSelected()를 호출하면 명령이 설정되어 상태 전환.
    // 여기서는 별도의 로직 없이 UI 입력을 기다린다고 가정.
}

void BattleManager::OnPlayerCommandSelected(int commandIndex) 
{
    // 플레이어가 명령 버튼을 클릭했을 때 호출.
    // commandIndex에 따라 플레이어의 행동을 설정한다.
    // 예: 0 = 공격, 1 = 마법, 2 = 아이템, 3 = 방어 등
    // 이 예제에서는 단순히 PLAYER_ACTION 상태로 전환
    currentState = BattleState::PLAYER_ACTION;
}

void BattleManager::ProcessPlayerAction() 
{
    // 플레이어의 선택한 명령을 실행.
    // 공격 애니메이션 실행, 데미지 계산, 효과 처리 등을 포함할 수 있다.
    // 실행 후 상태를 ENEMY_ACTION으로 전환.
    currentState = BattleState::ENEMY_ACTION;
}

void BattleManager::ProcessEnemyAction() 
{
    // 적 유닛의 AI 행동 실행.
    // 각 적 유닛이 행동을 선택하고 실행하도록 처리.
    // 모든 적의 행동이 끝나면 상태를 TURN_END로 전환.
    currentState = BattleState::TURN_END;
}

void BattleManager::EndTurn() 
{
    // 턴 종료 시 각 유닛의 상태 업데이트, 회복, 버프/디버프 정리 등을 처리.
    CheckBattleOutcome();
    // 다음 턴 시작을 위해 상태를 PLAYER_COMMAND로 전환.
    currentState = BattleState::PLAYER_COMMAND;
}

void BattleManager::CheckBattleOutcome() 
{
    // 플레이어와 적 유닛의 체력을 확인하여 승리 또는 패배 조건을 검사.
    bool allEnemiesDefeated = true;
    for (std::shared_ptr<UnitBase>& enemy : enemyUnits)
    {
        // 예시: if (enemy->GetHP() > 0) { allEnemiesDefeated = false; break; }
    }
    
    bool allPlayersDefeated = true;
    for (std::shared_ptr<UnitBase>& player : playerUnits)
    {
        // 예시: if (player->GetHP() > 0) { allPlayersDefeated = false; break; }
    }

    if (allEnemiesDefeated) currentState = BattleState::VICTORY;
    else if (allPlayersDefeated) currentState = BattleState::DEFEAT;
}