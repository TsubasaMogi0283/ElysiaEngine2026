#include "JudgementResultManager.h"

#include <cassert>

void JudgementResultManager::Register(const std::string& musicName, const std::string& level, const NotesJudgementResult& result){

    //BEの時の復習でランキングを実装していく

    //まずは楽曲を探す
    auto it = noteJudgementResultMap.find(musicName);
    if (it != noteJudgementResultMap.end()) {

        //レベルで探す
        auto it2 = it->second.find(level);
        if (it2 != it->second.end()) {

            //スコアで見ていく
            for (uint8_t i = 0u;i < RANKING_NUMBER_;i++) {
                //ランキング差し替え
                int tmp=it2->second.rankingArray[i].score;
            }
            

        }

    } else {
        assert(0);
    }
}
