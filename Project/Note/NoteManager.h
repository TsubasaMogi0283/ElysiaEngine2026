#pragma once

#include <vector>
#include <Note/BaseNote.h>

/// <summary>
/// ノーツ管理クラス
/// </summary>
class NoteManager{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	NoteManager() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="normalNoteModelHandle"></param>
	/// <param name="longNoteModelHandle"></param>
	void Initialize(const uint32_t& normalNoteModelHandle, const uint32_t& longNoteModelHandle );
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void DrawObject3D(const Camera& camera, const BaseLight& baseLight);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~NoteManager() = default;

private:
	

};

