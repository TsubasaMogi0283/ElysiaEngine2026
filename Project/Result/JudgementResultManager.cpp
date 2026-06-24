#include "JudgementResultManager.h"

#include <cassert>
#include <algorithm>

void JudgementResultManager::Register(const std::string& musicName, const std::string& level, const NotesJudgementResult& result){

    //ToDo
    //BEの時の復習でランキングを実装していきたい

    //まずは楽曲を探す
    auto it = noteJudgementResultMap.find(musicName);
    if (it != noteJudgementResultMap.end()) {

        //レベルで探す
        auto it2 = it->second.find(level);
        if (it2 != it->second.end()) {

            //スコアが高い順に並べ替える
            auto& ranking = it2->second.rankingArray;
            //一番スコアが低いのは3位なので、まずはそこを比べて後で並べ替える
            //vectorでやったら楽に済むが3つのデータしかいらないのでarrayがふさわしい
            if (result.score >= ranking[RANKING_NUMBER_ - 1u].score) {
                ranking[RANKING_NUMBER_ - 1u] = result;
            }
            //並べ替え
            std::sort(ranking.begin(), ranking.end(), [](const NotesJudgementResult& a, const NotesJudgementResult& b) {
                return a.score > b.score;
              }
            );
        }
    } else {
        //楽曲が見つからない
        assert(0);
    }
}
