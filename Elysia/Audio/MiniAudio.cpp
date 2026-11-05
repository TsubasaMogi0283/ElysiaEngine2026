#include "MiniAudio.h"
#include <cassert>

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


#pragma endregion