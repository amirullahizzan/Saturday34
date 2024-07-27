#pragma once

namespace Game01
{
	//! Enemyクラス
	class Enemy: public Object
	{
	public:
		BP_OBJECT_DECL( Enemy, "[Game01] Enemy" );

		//! @brief 初期化
		//! @return 初期化済み
		bool Init() override;

		//! @brief 更新
		void Update() override;

		// 当たり判定をここでとることができる
		void OnHit( const ComponentCollision::HitInfo& hit_info ) override;

	private:
		bool is_dead_ = false;
	};

}	 // namespace Game01
