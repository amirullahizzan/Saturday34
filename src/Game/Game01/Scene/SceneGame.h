#pragma once

#include <System/Scene.h>

namespace Game01
{
	//! @brief タイトルシーン
	class SceneGame: public Scene::Base
	{
	public:
		// ALT-Menuで表示される名前
		BP_CLASS_DECL( SceneGame, "[Game01] SceneGame" );

		//! @brief 初期化
		//! @return 初期化済み
		bool Init() override;

		//! @brief 更新
		void Update() override;
	};

}	 // namespace Game01
