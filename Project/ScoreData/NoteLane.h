#pragma once
/**
 * @file NoteLane.h
 * @brief レーンのノーツ情報
 * @author 茂木翼
 */

#include <cstdint>
#include <Note/NoteType.h>

/// <summary>
/// レーンのノーツ
/// </summary>
namespace NoteLane {
	/// <summary>
	/// 情報
	/// </summary>
	struct Information {
		//上
		NoteType upNote = NoteType::None;
		//下
		NoteType downNote = NoteType::None;
	};

	/// <summary>
	/// 位置
	/// </summary>
	enum class Place {
		Up,
		Down,
		Size
	};
}