#pragma once

enum class AxeState
{
    AXE_IDLE,
    AXE_PREPARE,    // (필요시) 던지기 전 준비 상태 (예: 경로 점선 표시)
    AXE_THROWN,     // 던져진 상태(직선에 가까운 궤적)
    AXE_RETURNING   // 플레이어에게 돌아오는 상태 (Bézier 커브 등)
};