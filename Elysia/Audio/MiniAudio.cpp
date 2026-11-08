#include "MiniAudio.h"
#include <cassert>
#include <Windows.h>

Elysia::MiniAudio* Elysia::MiniAudio::GetInstance(){
    static Elysia::MiniAudio instance;
    return &instance;
}

void Elysia::MiniAudio::Initialize(){
    // 二重初期化の防止
    if (isInitialized_) {
        return;
    }

    //エンジンの初期化
    ma_engine_config config = ma_engine_config_init();
    config.channels = 2;
    config.sampleRate = 0;

    ma_result result = ma_engine_init(&config, &engine_);
    if (result != MA_SUCCESS) {
        // オーディオデバイスが無い環境でも落ちないようにする
        isInitialized_ = false;
        return;
    }
    isInitialized_ = true;

    // マスター音量
    ma_engine_set_volume(&engine_, 1.0f);

    // グループ（BGM、SE用など）。
    for (auto& group : submixGroupArray_) {
        ma_sound_group_init(&engine_, 0, nullptr, &group);
    }
}

void Elysia::MiniAudio::Finalize(){

    //サウンドの解放
	for(auto& soundInfo : soundArray_) {
		if (soundInfo && soundInfo->sound) {
			ma_sound_uninit(soundInfo->sound.get());
			soundInfo->sound.reset();
		}
	}

    //サブミックスの解放
    for (uint8_t i = 0; i < SUBMIXVOICE_AMOUNT_; i++) {
        ma_sound_group_uninit(&submixGroupArray_[i]);
    }

	//最後にエンジンを解放する
    ma_engine_uninit(&engine_);
}

#pragma region 基本セット

int32_t Elysia::MiniAudio::Load(const std::string& filePath){
    //初期化していなければ止める
    assert(isInitialized_);

    //既存だったらそれを返す
    for (uint16_t i = 0u; i < SOUND_AMOUNT_; i++) {
        if (soundArray_[i] && soundArray_[i]->path == filePath) {
			return soundArray_[i]->handle;
        }
    }

    //空きを探し空だった所から入れていく
    int32_t handle = -1;
    for (int32_t i = 0; i < SOUND_AMOUNT_; ++i) {
        if (!soundArray_[i]) {
            handle = i;
            break;
        }
    }
    if (handle < 0) {
        return -1; // 上限オーバー
    }

    //データの作成
	std::unique_ptr<SoundData> soundData = std::make_unique<SoundData>();
    soundData->sound = std::make_unique<ma_sound>();

    //読み込み
    const ma_uint32 flags = MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_NO_SPATIALIZATION;
    ma_result result = ma_sound_init_from_file(
        &engine_, filePath.c_str(), flags, nullptr, nullptr, soundData->sound.get());
    if (result != MA_SUCCESS) {
        return -1;
    }

    //登録
    soundData->path = filePath;
    soundData->handle = handle;
    soundArray_[handle] = std::move(soundData);

    return handle;

}

void Elysia::MiniAudio::Play(const int32_t audioHandle,const bool& isLoop){
    ma_sound* sound = GetSound(audioHandle);
    if (!sound) {
        return;
    }
    ma_sound_stop(sound);
    ma_sound_seek_to_pcm_frame(sound, 0); 
    ma_sound_set_looping(sound, isLoop ? MA_TRUE : MA_FALSE);
    ma_sound_set_stop_time_in_pcm_frames(sound, ~(ma_uint64)0);
    ma_sound_start(sound);

}

void Elysia::MiniAudio::Pause(const int32_t& audioHandle){
    //データの取得
    ma_sound* sound = GetSound(audioHandle);

	//見つかれば一時停止
    if (sound) {
        ma_sound_stop(sound);
    }
}

void Elysia::MiniAudio::Resume(const int32_t& audioHandle){
    //データの取得
    ma_sound* sound = GetSound(audioHandle);

    //見つかれば再開
    if (sound) { 
        ma_sound_start(sound); 
    }  

}

void Elysia::MiniAudio::Stop(const int32_t& audioHandle){
    ma_sound* sound = GetSound(audioHandle);
    if (sound) {
        ma_sound_stop(sound);
        ma_sound_seek_to_pcm_frame(sound, 0);
    }
}

float_t Elysia::MiniAudio::GetPlayCurrentTime(const int32_t& audioHandle){
    //データの取得
    ma_sound* sound = GetSound(audioHandle);

    //無かった場合0を返す
    if (!sound) {
        return 0.0f;
    }

    //長さ(カーソル)を取得
    float cursor = 0.0f;
    if (ma_sound_get_cursor_in_seconds(sound, &cursor) != MA_SUCCESS) {
        return 0.0f;
    }
    return cursor;
}


float_t Elysia::MiniAudio::GetLength(const int32_t& audioHandle) {
    //データの取得
    ma_sound* sound = GetSound(audioHandle);

    //無かった場合0を返す
    if (!sound) {
        return 0.0f;
    }

    //長さの取得
    float_t length = 0.0f;
    if (ma_sound_get_length_in_seconds(sound, &length) != MA_SUCCESS) {
        return 0.0f;
    }
    return length;
}

bool Elysia::MiniAudio::SetLoopRange(ma_sound* sound, float_t& startTime, const float_t& endTime){

    //サンプルレートの取得
    ma_uint32 sampleRate = 0;
    if (ma_sound_get_data_format(sound, nullptr, nullptr, &sampleRate, nullptr, 0) != MA_SUCCESS
        || sampleRate == 0) {
        return false;
    }

    //長さを取得
    ma_uint64 length = 0;
    if (ma_sound_get_length_in_pcm_frames(sound, &length) != MA_SUCCESS || length == 0) {
        return false;
    }

    //0より小さくならないようにする
    if (startTime < 0.0f) {
        startTime = 0.0f;
    }
    ma_uint64 beginFrame = static_cast<ma_uint64>(startTime * sampleRate);
    ma_uint64 endFrame = 0u;
    
    if (endTime < 0.0f) {
        endFrame = length;
    }
    else {
        endFrame = static_cast<ma_uint64>(endTime * sampleRate);
    }
    
    //全体の長さより大きくなってはいけない
    if (endFrame > length) {
        endFrame = length;
    }
    if (beginFrame >= endFrame) {
        return false;
    }

    return ma_data_source_set_loop_point_in_pcm_frames(
        ma_sound_get_data_source(sound), beginFrame, endFrame) == MA_SUCCESS;
}

void Elysia::MiniAudio::StartLoop(ma_sound* sound, const float_t& startTime){
    ma_uint32 sampleRate = 0u;
    ma_sound_get_data_format(sound, nullptr, nullptr, &sampleRate, nullptr, 0u);

    ma_sound_stop(sound);
    ma_sound_seek_to_pcm_frame(sound, static_cast<ma_uint64>(startTime * sampleRate));
    ma_sound_set_looping(sound, MA_TRUE);
    ma_sound_set_stop_time_in_pcm_frames(sound, ~(ma_uint64)0);
    ma_sound_start(sound);
}


void Elysia::MiniAudio::PartlyLoop(const int32_t& audioHandle, float_t& startTime, const float_t& endTime){
    ma_sound* sound = GetSound(audioHandle);
    if (!sound || !SetLoopRange(sound, startTime, endTime)) {
        return;
    }
    StartLoop(sound, startTime);
}

void Elysia::MiniAudio::ExitLoop(const int32_t& audioHandle){
    if (ma_sound* sound = GetSound(audioHandle)) {
        ma_sound_set_looping(sound, MA_FALSE);
    }
}


#pragma endregion