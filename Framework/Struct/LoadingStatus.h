#pragma once

struct LoadingStatus    // 수정 필요
{
    size_t totalTasks = 0;       // 전체 로딩 작업 개수
    size_t completedTasks = 0;   // 완료된 작업 개수
    bool IsComplete() const { return completedTasks >= totalTasks; }
    float Progress() const { return totalTasks > 0 ? float(completedTasks) / totalTasks : 1.0f; }
};