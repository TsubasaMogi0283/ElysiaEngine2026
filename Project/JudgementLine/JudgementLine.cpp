#include "JudgementLine.h"

#include <TextureManager.h>
#include <WindowsSetup.h>
#include <Easing.h>
#include <imgui.h>
#include <SingleCalculation.h>
#include <Camera.h>

JudgementLine::JudgementLine(){
	//インスタンスの取得
	//テクスチャ管理クラス
	textureManager_ = Elysia::TextureManager::GetInstance();
	//ウィンドウクラス
	windowSetup_ = Elysia::WindowsSetup::GetInstance();
}

void JudgementLine::Initialize(const uint32_t& normalNoteModelHandle, const std::array<uint32_t, static_cast<uint8_t>(NoteJudgement::Selection::Size)> judgementTextureHandleArray){

	//モデルの読み込み
	lineModel_ = Elysia::Model::Create(normalNoteModelHandle);
	lineWorldTransform_.Initialize();
	lineWorldTransform_.scale.x = 0.05f;
	lineWorldTransform_.scale.y = 10.0f;
	lineWorldTransform_.translate.x = LINE_POSITION_X_;
	lineWorldTransform_.translate.y = 10.0f;
	//判定線のマテリアルを生成
	lineMaterial_.Initialize();
	lineMaterial_.color = { .x = 1.0f,.y = 0.0f,.z = 0.0f,.w = 1.0f };
	//テクスチャの情報を入れていく
	judgementTextureHandleArray_ = judgementTextureHandleArray;
	assert(!judgementTextureHandleArray_.empty());

	//UI
	//一旦Perfectで初期化
	for (uint8_t i = 0u;i < UI_AMOUNT_;i++) {
		judgementUIArray_[i].sprite = Elysia::Sprite::Create(judgementTextureHandleArray_[static_cast<uint8_t>(NoteJudgement::Selection::Perfect)]);
		judgementUIArray_[i].sprite->SetAnchorPoint({ .x = 0.5f,.y = 0.5f });
	}


	
}

void JudgementLine::UpdateObject3D(){

	//更新
	lineWorldTransform_.Update();
	lineMaterial_.Update();
}

void JudgementLine::UpdateSprite(const Camera& camera){

	
	//判定UIの更新
	for (uint8_t i = 0u;i < UI_AMOUNT_;i++) {
		//使用状態の時
		if (judgementUIArray_[i].isUse) {
			//時間変化
			judgementUIArray_[i].currentTime += DELTA_TIME_;
			//線形補間
			float_t t = SingleCalculation::InverseLerp(0.0f, UI_MOVE_TIME_, judgementUIArray_[i].currentTime);
			t = std::clamp(t, 0.0f, 1.0f);

			for (uint8_t j = 0u;j < static_cast<uint8_t>(NoteLane::Place::Size);j++) {
				uiPositionArray_[j] = playerPosition_;
				uiPositionArray_[j].y = LANE_POSITION_Y_[static_cast<uint8_t>(j)];
				Matrix4x4 viewportMatrix = Matrix4x4::MakeViewportMatrix(0.0f, 0.0f, windowSetup_->GetInstance()->GetClientSize().x, windowSetup_->GetInstance()->GetClientSize().y, 0.0f, 1.0f);
				Matrix4x4 viewProjectionViewportMatrix = Matrix4x4::Multiply(Matrix4x4::Multiply(camera.viewMatrix, camera.projectionMatrix), viewportMatrix);
				uiPositionArray_[j] = Vector3<float_t>::MakeTransformCoordinate(uiPositionArray_[j], viewProjectionViewportMatrix);

			}
			
			//スケールと座標用
			float_t transformEasedT = Easing::EaseOutCirc(t);
			judgementUIArray_[i].sprite->SetScale({ .x = transformEasedT ,.y = transformEasedT });
			float_t newPositionY = SingleCalculation::Lerp(uiPositionArray_[static_cast<uint8_t>(judgementUIArray_[i].place)].y, judgementUIArray_[i].movedPositionY, transformEasedT);
			judgementUIArray_[i].sprite->SetPosition({ .x = uiPositionArray_[static_cast<uint8_t>(judgementUIArray_[i].place)].x ,.y = newPositionY });

			//透明度
			float_t transparencyEasedT = Easing::EaseOutQuart(t);
			judgementUIArray_[i].sprite->SetTransparency(transparencyEasedT);

			//表示時間を超えたら未使用に初期化
			if (judgementUIArray_[i].currentTime >= APPEAR_TIME_) {
				judgementUIArray_[i].isUse = false;
				judgementUIArray_[i].currentTime = 0.0f;
				judgementUIArray_[i].ratio = 0.0f;
				judgementUIArray_[i].initialPositionY = 0.0f;
				judgementUIArray_[i].movedPositionY = 0.0f;
				judgementUIArray_[i].textureHandle = judgementTextureHandleArray_[static_cast<uint8_t>(NoteJudgement::Selection::Perfect)];
				judgementUIArray_[i].judgement=NoteJudgement::Selection::Size;

			}
		}
	}

#ifdef _DEBUG
	ImGui::Begin("使用チェック");
	for (uint8_t i = 0u;i < UI_AMOUNT_;i++) {
		ImGui::Checkbox("IsUse", &judgementUIArray_[i].isUse);
	}
	ImGui::End();
#endif // _DEBUG
}

void JudgementLine::DrawObject3D(const Camera& camera, const BaseLight& baseLight){
	//モデルの描画
	lineModel_->Draw(lineWorldTransform_, camera, lineMaterial_, baseLight);
}

void JudgementLine::DrawSprite(){

	//判定UI
	for (uint8_t i = 0u;i < UI_AMOUNT_;i++) {
		if (judgementUIArray_[i].isUse) {
			judgementUIArray_[i].sprite->Draw(judgementUIArray_[i].textureHandle);
		}
	}
}

void JudgementLine::SetReleaseUI(const NoteLane::Place& place, const NoteJudgement::Selection& selection){
	
	for (uint8_t i = 0u;i < UI_AMOUNT_;i++) {
		//未使用状態のものから使う
		if (!judgementUIArray_[i].isUse) {
			//使用中にする
			judgementUIArray_[i].isUse = true;
			//場所を記録
			judgementUIArray_[i].place = place;
			//テクスチャハンドルの設定
			judgementUIArray_[i].textureHandle = judgementTextureHandleArray_[static_cast<uint8_t>(selection)];
			//判定
			judgementUIArray_[i].judgement = selection;


			//高さの設定
			float_t height = 0.0f;
			if (judgementUIArray_[i].place == NoteLane::Place::Up) {
				height = 300.0f;
			} 
			else {
				height = 500.0f;
			}

			//初期座標と移動後の座標を設定
			judgementUIArray_[i].initialPositionY = height;

			//ミスの設定をする
			if (judgementUIArray_[i].judgement==NoteJudgement::Selection::Miss) {
				judgementUIArray_[i].movedPositionY = height + UI_MOVE_DISTANCE_;

			} 
			//失敗の設定をする
			else {
				judgementUIArray_[i].movedPositionY = height - UI_MOVE_DISTANCE_;
			}
			
			break;
		}
	}
}
