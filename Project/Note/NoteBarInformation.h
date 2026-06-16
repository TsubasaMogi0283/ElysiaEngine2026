#pragma once

#include <vector>
#include "NoteLane.h"

/// <summary>
/// 1小節のデータ
/// </summary>
struct NoteBarInformation {
	//ノーツ
	std::vector<NoteLane> notes = {};

	//BPM
	float bpm = 170.0f;
};