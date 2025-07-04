#pragma once

#include "Utilities/String.h"

using namespace FMOD;

class Sounds
{
    DECLARE_SINGLETON(Sounds);

public:
    // 초기화 및 종료
    void Init();
    void Update();

    // 사운드 추가/제거
    void AddSound(const std::string& name, const std::wstring& file, bool loop = false);
    void RemoveSound(const std::string& name);

    // 재생 컨트롤
    void Play(const std::string& name, float volume = 1.0f);
    void Stop(const std::string& name);
    void Pause(const std::string& name);
    void Resum(const std::string& name);

    // 상태 조회
    bool IsPlaying(const std::string& name);
    bool IsPause(const std::string& name);

    // 볼륨
    void  SetVolume(const std::string& name, float volume);
    float GetVolume(const std::string& name);

    // 사운드 길이(초 단위 반환 등)
    float GetSoundLength(const std::string& name);

private:
    // FMOD System 해제를 위한 커스텀 deleter
    struct FModSystemDeleter
    {
        void operator()(System* ptr)
        {
            if (ptr)
            {
                ptr->close();
                ptr->release();
            }
        }
    };

    // FMOD Sound 해제를 위한 커스텀 deleter
    struct FModSoundDeleter
    {
        void operator()(Sound* ptr)
        {
            if (ptr)
                ptr->release();
        }
    };

    // 채널은 특별히 release() 같은 것이 없으나,
    // 사용 중에 stop()이 필요하다면 커스텀 deleter에서 처리할 수 있음
    struct FModChannelDeleter
    {
        void operator()(Channel* ptr)
        {
            // Channel은 적절히 정리하고 싶다면 stop() 호출 정도 가능
            // 실제로는 채널이 재생이 끝나면 자동으로 정리되므로,
            // 여기서 꼭 stop을 해야만 하는 것은 아님. 필요에 따라 사용.
            if (ptr)
                ptr->stop();
        }
    };

private:
    // FMOD 시스템은 유니크 포인터로 관리 (프로그램 종료 시 1개만 존재)
    std::unique_ptr<System, FModSystemDeleter> system;

    // 사운드 + 채널을 동시에 관리하기 위해 pair로 묶어서 map에 저장
    // key : 사운드 이름
    // value : (Sound, Channel)
    //  - Sound : std::shared_ptr (FModSoundDeleter 로 release)
    //  - Channel : std::shared_ptr (FModChannelDeleter 로 stop)
    std::map<std::string, std::pair<std::shared_ptr<Sound>,std::shared_ptr<Channel>>> sounds;

    // 전체 사운드 개수 제한 (FMOD 초기화 시 동시 재생 가능한 채널 수)
    UINT count = 0;
    float volum = 1.0f;
};

