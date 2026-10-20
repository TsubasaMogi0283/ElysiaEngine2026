#include "GlobalVariables.h"

#include <imgui.h>

#include "ImGuiManager.h"
#include "WindowsSetup.h"


Elysia::GlobalVariables* Elysia::GlobalVariables::GetInstance(){
    static GlobalVariables instance;
    return &instance;
}

void Elysia::GlobalVariables::CreateGroup(const std::string& groupName){
    //指定名のオブジェクトが無ければ追加
    datas_[groupName];
}

void Elysia::GlobalVariables::SetValue(const std::string& groupName, const std::string& key,const int32_t& value){
    //グループの参照
    Group& group = datas_[groupName];
    
    //新しい項目のデータを設定
    Item newItem{};
    newItem.value = value;

    //設定した項目をstd::mapに追加
    group.items[key] = newItem;

}

void Elysia::GlobalVariables::SetValue(const std::string& groupName, const std::string& key,const float& value){
    //グループの参照
    Group& group = datas_[groupName];

    //新しい項目のデータを設定
    Item newItem{};
    newItem.value = value;

    //設定した項目をstd::mapに追加
    group.items[key] = newItem;

}

void Elysia::GlobalVariables::SetValue(const std::string& groupName, const std::string& key, const Vector2<float_t>& value){
    //グループの参照
    Group& group = datas_[groupName];

    //新しい項目のデータを設定
    Item newItem{};
    newItem.value = value;

    //設定した項目をstd::mapに追加
    group.items[key] = newItem;
}

void Elysia::GlobalVariables::SetValue(const std::string& groupName, const std::string& key, const Vector3<float_t>& value){
    //グループの参照
    Group& group = datas_[groupName];

    //新しい項目のデータを設定
    Item newItem{};
    newItem.value = value;

    //設定した項目をstd::mapに追加
    group.items[key] = newItem;

}

void Elysia::GlobalVariables::AddItem(const std::string& groupName, const std::string& key,const int32_t& value){
    

    //グループを検索
    std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);


    //無かったら止める
    assert(itGroup != datas_.end());

    
    //まず参照
    Group& group = itGroup->second;
    std::map<std::string, Item>::iterator itItem = group.items.find(key);
    //無かったら追加
    if (itItem == itGroup->second.items.end()) {
        SetValue(groupName, key, value);
    }
    
}

void Elysia::GlobalVariables::AddItem(const std::string& groupName, const std::string& key,const float& value){
    //グループを検索
    std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);

    //無かったら止める
    assert(itGroup != datas_.end());


    //まず参照
    Group& group = itGroup->second;
    std::map<std::string, Item>::iterator itItem = group.items.find(key);
    //無かったら追加
    if (itItem == itGroup->second.items.end()) {
        SetValue(groupName, key, value);
    }
}

void Elysia::GlobalVariables::AddItem(const std::string& groupName, const std::string& key, const Vector2<float_t>& value){
    //グループを検索
    std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);

    //無かったら止める
    assert(itGroup != datas_.end());

    //まず参照
    Group& group = itGroup->second;
    std::map<std::string, Item>::iterator itItem = group.items.find(key);
    //無かったら追加
    if (itItem == itGroup->second.items.end()) {
        SetValue(groupName, key, value);
    }
}

void Elysia::GlobalVariables::AddItem(const std::string& groupName, const std::string& key, const Vector3<float_t>& value){
    //グループを検索
    std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);

    //無かったら止める
    assert(itGroup != datas_.end());

    //まず参照
    Group& group = itGroup->second;
    std::map<std::string, Item>::iterator itItem = group.items.find(key);
    //無かったら追加
    if (itItem == itGroup->second.items.end()) {
        SetValue(groupName, key, value);
    }
}

int32_t Elysia::GlobalVariables::GetIntValue(const std::string& groupName, const std::string& key) {
    //グループを検索
    std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);

    //無かったら止める
    assert(itGroup != datas_.end());
    //参照
    Group& group = datas_.at(groupName);
    //キーを検索
    std::map<std::string, Item>::const_iterator itKey = group.items.find(key);
    //無かったら止める
    assert(itKey != group.items.end());
    //SaveFileより
    return std::get<int32_t>(itKey->second.value);



}

float Elysia::GlobalVariables::GetFloatValue(const std::string& groupName, const std::string& key) {
    //グループを検索
    std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);

    //無かったら止める
    assert(itGroup != datas_.end());
    //参照
    Group& group = datas_.at(groupName);
    //キーを検索
    std::map<std::string, Item>::const_iterator itKey = group.items.find(key);
    //無かったら止める
    assert(itKey != group.items.end());
    //SaveFileより
    return std::get<float>(itKey->second.value);

}

Vector2<float_t> Elysia::GlobalVariables::GetVector2Value(const std::string& groupName, const std::string& key){

    //グループを検索
    std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);

    //無かったら止める
    assert(itGroup != datas_.end());
    //参照
    Group& group = datas_.at(groupName);
    //キーを検索
    std::map<std::string, Item>::const_iterator itKey = group.items.find(key);
    //無かったら止める
    assert(itKey != group.items.end());
    //SaveFileより
    return std::get<Vector2<float_t>>(itKey->second.value);


}

Vector3<float_t> Elysia::GlobalVariables::GetVector3Value(const std::string& groupName, const std::string& key) {
    //グループを検索
    std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);
    
    //無かったら止める
    assert(itGroup != datas_.end());
    //参照
    Group& group = datas_.at(groupName);
    //キーを検索
    std::map<std::string, Item>::const_iterator itKey = group.items.find(key);
    //無かったら止める
    assert(itKey != group.items.end());
    //SaveFileより
    return std::get<Vector3<float_t>>(itKey->second.value);


}

void Elysia::GlobalVariables::SaveFile(const std::string& groupName){
    //グループを検索
    std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);

    //無かったら止める
    assert(itGroup!=datas_.end());

    nlohmann::json root;
    //json::objectはstd::mapみたいなもの
    root = nlohmann::json::object();

    //jsonオブジェクト登録
    root[groupName] = nlohmann::json::object();

    //各項目について
    for (std::map<std::string, Item>::iterator itItem = itGroup->second.items.begin();
        itItem != itGroup->second.items.end(); ++itItem) {
        //項目名を取得
        const std::string& itemName = itItem->first;
        //項目の参照を取得
        Item& item = itItem->second;


        //int32_tの場合
        if (std::holds_alternative<int32_t>(item.value)) {
            //int32_t型の値を登録
            root[groupName][itemName] = std::get<int32_t>(item.value);
        }
        //floatの場合
        else if (std::holds_alternative<float>(item.value)) {
            //float型の値を登録
            root[groupName][itemName] = std::get<float>(item.value);
        }
        //Vector2の場合
        else if (std::holds_alternative<Vector2<float_t>>(item.value)) {
            //float型のjson配列登録
            Vector2<float_t> value = std::get<Vector2<float_t>>(item.value);
            root[groupName][itemName] = nlohmann::json::array({ value.x, value.y});
        }
        //Vector3の場合
        else if (std::holds_alternative<Vector3<float_t>>(item.value)) {
            //float型のjson配列登録
            Vector3<float_t> value = std::get<Vector3<float_t>>(item.value);
            root[groupName][itemName] = nlohmann::json::array({value.x, value.y, value.z});
        }

    }


    //ディレクトリが無ければ作成する
    std::filesystem::path dir(DIRECTORY_PATH_);
    if (!std::filesystem::exists(DIRECTORY_PATH_)) {
        std::filesystem::create_directories(DIRECTORY_PATH_);
    }

    //書き込むJSONファイルのフルパスを合成する
    std::string filePath = DIRECTORY_PATH_ + groupName + ".json";
    //書き込む用ファイルストリーム
    std::ofstream ofs;
    //ファイルを書き込み用に開く
    ofs.open(filePath);

    //ファイルを開くのに失敗した場合
    if (ofs.fail()) {
        //書き込み用のファイルを開けるのに失敗したよ
        std::string message = "Failed open data file for write.";
        //WindowsAPIのメッセージボックスをここで使う
        MessageBoxA(nullptr, message.c_str(), "AdjustmentItems", 0u);
        assert(0);
        return;
    }

    //ファイルにjson文字列を書き込む(インデント幅4)
    //rootオブジェクトに格納されている全データをJSON文字列に変換して、
    //書き込み用のファイルに流し込む。
    ofs << std::setw(4) << root << std::endl;


    //書き込み用に開いたファイルを閉じる。
    ofs.close();


}

void Elysia::GlobalVariables::LoadAllFile(){
    //保存先ディレクトリのパスをローカル変数で宣言する
    std::filesystem::path directory(DIRECTORY_PATH_);

    //無かったらreturn
    if (!std::filesystem::exists(DIRECTORY_PATH_)) {
        return;
    }

    //各ファイルの処理
    std::filesystem::directory_iterator dirIt(DIRECTORY_PATH_);
    //directory_iterator...ディレクトリないのファイルを1つずつ指すためのポインタのようなもの
    for (const std::filesystem::directory_entry& entry : dirIt) {

        //パスによる判別
        //ファイルパスを取得
        const std::filesystem::path& filePath = entry.path();

        //ファイル拡張子を取得
        std::string extension = filePath.extension().string();
        //.jsonファイル以外はスキップ
        if (extension.compare(".json") != 0) {
            continue;
        }

        //extension...拡張子のみ抽出
        //stem...拡張子を除くファイル名を抽出
        //ファイル読み込み
        LoadFile(filePath.stem().string());

    }
}

void Elysia::GlobalVariables::LoadFile(const std::string& groupName){
    //読み込むJSONファイルのフルパスを合成する
    std::string filePath = DIRECTORY_PATH_ + groupName + ".json";
    //読み込み用のファイルストリーム
    std::ifstream ifs;
    ifs.open(filePath);

    //書き込みはofstream
    //読み込みはifstream

    //読み込みが失敗した場合
    if (ifs.fail()) {
        //読み込み用のファイルを開けるのに失敗したよ
        std::string message = "Failed open data file for read.";
        //WindowsAPIのメッセージボックスをここで使う
        MessageBoxA(nullptr, message.c_str(), "AdjustmentItems", 0u);
        assert(0);
        return;
    }

    nlohmann::json root = {};

    //json文字列からjsonのデータ構造に展開
    ifs >> root;
    //ファイルを閉じる
    ifs.close();

    //グループ登録確認
    //グループを検索
    nlohmann::json::iterator itGroup = root.find(groupName);

    //未登録チェック
    assert(itGroup != root.end());

    //確認アイテムについて
    for (nlohmann::json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {
        //アイテム名(キー)を取得
        const std::string& itemName = itItem.key();
    
        if(itItem->is_number_integer()){
            //int型の値を登録
            int32_t value = itItem->get<int32_t>();
            SetValue(groupName, itemName, value);
        }
        else if (itItem->is_number_float()) {
            //float型の値を登録
            //floatは無いのでその代わりdoubleで
            double value = itItem->get<double>();
            SetValue(groupName, itemName, static_cast<float>(value));
        }
        //要素数2の配列であれば
        else if (itItem->is_array() && itItem->size() == 2) {
            //float型のJson配列登録
            Vector2 value = {.x = itItem->at(0).get<float>(),.y = itItem->at(1).get<float>()};
            SetValue(groupName, itemName, value);
        }
        //要素数3の配列であれば
        else if (itItem->is_array() && itItem->size() == 3) {
            //float型のJson配列登録
            Vector3 value = {.x = itItem->at(0).get<float>(),.y = itItem->at(1).get<float>(),.z = itItem->at(2).get<float>() };
            SetValue(groupName, itemName, value);
        }
    }
}

void Elysia::GlobalVariables::Update(){

}
