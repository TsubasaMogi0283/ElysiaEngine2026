#pragma once

#include <vector>
#include "NoteLane.h"

/// <summary>
/// 1小節のデータ
/// </summary>
struct NoteBarInformation {
	//ノーツ
	std::vector<NoteLane::Information> notesLane = {};

	//BPM
	float_t bpm = 170.0f;
};