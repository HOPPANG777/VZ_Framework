#pragma once

enum class AxeState
{
    AXE_IDLE,
    AXE_IDLE_R,
	AXE_IDLE_L,
    AXE_PREPARE,	// (필요시) 던지기 전 준비 상태 (예: 경로 점선 표시)
	AXE_PREPARE_R,
	AXE_PREPARE_L,
    AXE_THROWN,     // 던져진 상태(직선에 가까운 궤적)
	AXE_THROWN_R,
	AXE_THROWN_L,
    AXE_RETURNING,  // 플레이어에게 돌아오는 상태 (Bézier 커브 등)
	AXE_RETURNING_R,
	AXE_RETURNING_L,
};