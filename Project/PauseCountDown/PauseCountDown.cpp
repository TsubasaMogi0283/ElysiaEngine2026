#include "PauseCountDown.h"

void PauseAssets::Initilaize() {
	for (uint8_t i = 0;i < COUNT_DOWN_NUMBER_;i++) {
		spriteArray_[i]->Draw();
	}
}

void PauseAssets::Update() {

}

void PauseAssets::Draw() {
	for (uint8_t i = 0;i < COUNT_DOWN_NUMBER_;i++) {
		spriteArray_[i]->Draw();
	}
}