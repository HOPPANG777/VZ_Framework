#include "Framework.h"
#include "Sounds.h"

Sounds::Sounds()
    : count(150), volum(1.0f)
{
    // 필요한 경우 생성자에서 작업
    Init();
}
Sounds::~Sounds()
{
    // smart pointer들이 자동으로 해제 담당
    // system unique_ptr도 소멸자에서 close(), release() 자동 호출
}
///////////////////////////////////////////////////////////////////////////

void Sounds::Init()
{
    // FMOD 시스템을 유니크 포인터로 생성
    System* rawSystem = nullptr;
    System_Create(&rawSystem);
    if (!rawSystem) throw std::runtime_error("FMOD System Create Failed.");

    // unique_ptr에 소유권을 넘김
    system = std::unique_ptr<System, FModSystemDeleter>(rawSystem);

    // 시스템 초기화
    FMOD_RESULT result = system->init(count, FMOD_INIT_NORMAL, nullptr);
    if (result != FMOD_OK) throw std::runtime_error("FMOD System init Failed.");
}

void Sounds::Update()
{
    if (system)
        system->update();
}

void Sounds::AddSound(const std::string& name, const std::wstring& file, bool loop)
{
    if (!system) return;

    // Sound 생성
    Sound* rawSound = nullptr;

    FMOD_MODE mode = (loop) ? FMOD_LOOP_NORMAL : FMOD_DEFAULT;
    FMOD_RESULT result = system->createStream(String::ToString(file).c_str(),
        mode,
        nullptr,
        &rawSound);
    if (result != FMOD_OK || !rawSound)
    {
        // 실패 시 예외나 로그
        throw std::runtime_error("Failed to create FMOD Sound.");
    }

    // shared_ptr로 소유 (deleter는 FModSoundDeleter)
    std::shared_ptr<Sound> spSound(rawSound, FModSoundDeleter());

    // 채널은 아직 playSound() 호출 전이므로 nullptr
    std::shared_ptr<Channel> spChannel(nullptr, FModChannelDeleter());

    // map에 등록 (기존에 있으면 덮어씀)
    sounds[name] = std::make_pair(spSound, spChannel);
}

void Sounds::RemoveSound(const std::string& name)
{
    auto iter = sounds.find(name);
    if (iter == sounds.end())
        return;

    // 혹시 재생 중인 채널이 있다면 stop() 가능
    if (iter->second.second) // shared_ptr<Channel>
        iter->second.second->stop();

    // map에서 삭제 -> 소멸자에서 sound release 자동 호출
    sounds.erase(iter);
}

void Sounds::Play(const std::string& name, float volume)
{
    if (!system) return;

    // name에 해당하는 사운드 찾기
    auto iter = sounds.find(name);
    if (iter == sounds.end()) return;

    // 이미 사운드가 존재
    std::shared_ptr<Sound>  spSound = iter->second.first;
    std::shared_ptr<Channel> spChannel = iter->second.second;

    if (!spSound) return; // 사운드가 없으면 재생 불가

    // 재생(출력) - 재생 시 채널이 새로 할당됨
    Channel* rawChannel = nullptr;
    FMOD_RESULT result = system->playSound(spSound.get(), nullptr, false, &rawChannel);
    if (result != FMOD_OK)
        return;

    // 새로 받은 Channel*을 shared_ptr로 변환
    std::shared_ptr<Channel> newChannel(rawChannel, FModChannelDeleter());

    // 볼륨 설정
    newChannel->setVolume(volume);

    // map에 갱신 (사운드 포인터는 그대로, 채널 포인터만 갱신)
    iter->second.second = newChannel;
}

void Sounds::Stop(const std::string& name)
{
    auto iter = sounds.find(name);
    if (iter == sounds.end()) return;

    if (iter->second.second)
        iter->second.second->stop();
}

void Sounds::Pause(const std::string& name)
{
    auto iter = sounds.find(name);
    if (iter == sounds.end()) return;

    if (iter->second.second)
        iter->second.second->setPaused(true);
}

void Sounds::Resum(const std::string& name)
{
    auto iter = sounds.find(name);
    if (iter == sounds.end()) return;

    if (iter->second.second)
        iter->second.second->setPaused(false);
}

bool Sounds::IsPlaying(const std::string& name)
{
    auto iter = sounds.find(name);
    if (iter == sounds.end()) return false;

    if (!iter->second.second)
        return false;

    bool playing = false;
    iter->second.second->isPlaying(&playing);
    return playing;
}

bool Sounds::IsPause(const std::string& name)
{
    auto iter = sounds.find(name);
    if (iter == sounds.end()) return false;

    if (!iter->second.second)
        return false;

    bool paused = false;
    iter->second.second->getPaused(&paused);
    return paused;
}

void Sounds::SetVolume(const std::string& name, float volume)
{
    auto iter = sounds.find(name);
    if (iter == sounds.end()) return;

    if (iter->second.second)
        iter->second.second->setVolume(volume);
}

float Sounds::GetVolume(const std::string& name)
{
    auto iter = sounds.find(name);
    if (iter == sounds.end()) return 0.0f;

    if (!iter->second.second)
        return 0.0f;

    float volume = 0.0f;
    iter->second.second->getVolume(&volume);
    return volume;
}

float Sounds::GetSoundLength(const std::string& name)
{
    auto iter = sounds.find(name);
    if (iter == sounds.end())
        return 0.0f;

    std::shared_ptr<Sound> spSound = iter->second.first;
    if (!spSound)
        return 0.0f;

    // FMOD_TIMEUNIT_MS 기준으로 길이를 얻어온다 (밀리초 단위)
    unsigned int lengthMs = 0;
    FMOD_RESULT result = spSound->getLength(&lengthMs, FMOD_TIMEUNIT_MS);
    if (result != FMOD_OK)
        return 0.0f;

    // 초 단위로 변환하여 반환 (필요하다면 밀리초 그대로 반환해도 됨)
    float lengthSec = lengthMs / 1000.0f;
    return lengthSec;
}