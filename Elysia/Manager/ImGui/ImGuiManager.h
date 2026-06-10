#pragma once

/**
 * @file ImGuiManager.h
 * @brief ImGui管理クラス
 * @author 茂木翼
 */


 /// <summary>
 /// ElysiaEngine
 /// </summary>
namespace Elysia {

	/// <summary>
	/// ImGui管理クラス
	/// </summary>
	class ImGuiManager final {
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		ImGuiManager() = default;

		/// <summary>
		/// デストラクタ
		/// </summary>
		~ImGuiManager() = default;

	public:
		/// <summary>
		/// コピーコンストラクタ禁止
		/// </summary>
		/// <param name="imGui"></param>
		ImGuiManager(const ImGuiManager& imGui) = delete;

	public:

		/// <summary>
		/// 初期化
		/// </summary>
		void Initialize();

		/// <summary>
		/// フレーム開始
		/// </summary>
		void BeginFrame();

		/// <summary>
		/// 描画
		/// </summary>
		void Draw();

		/// <summary>
		/// 描画終わり
		/// </summary>
		void EndDraw();

		/// <summary>
		/// 解放
		/// </summary>
		void Finalize();

	};

};

