#pragma once

#include "Enum/BattleState.h"
#include "GameObject/UnitBase.h"

class BattleManager
{
	DECLARE_SINGLETON(BattleManager);

public:
    // 초기화 및 재설정
    void Initialize();
    void Reset();

    // 매 프레임 호출되는 업데이트 함수
    void Update();

    // 플레이어의 명령 입력을 외부에서 받을 수 있는 함수 (예: UI 버튼에서 호출)
    void OnPlayerCommandSelected(int commandIndex);

    // 현재 전투 상태 반환
    BattleState GetCurrentState() const { return currentState; }

private:
    // 상태별 처리 함수
    void ProcessPlayerCommand();
    void ProcessPlayerAction();
    void ProcessEnemyAction();
    void EndTurn();
    void CheckBattleOutcome();

    // 전투 상태를 저장하는 변수
    BattleState currentState;

    // 예시: 전투에 참여하는 유닛 리스트
    std::vector<std::shared_ptr<UnitBase>> playerUnits;
    std::vector<std::shared_ptr<UnitBase>> enemyUnits;

    // 추가: 전투 UI, 명령 선택 인덱스 등 필요한 변수들

};

