#include "Enemy.h"
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentCollisionCapsule.h>

namespace Game01
{

	//! @brief 初期化
	//! @return 初期化済み
	bool Enemy::Init() /*override*/
	{
		Super::Init();

		std::vector<Animation::Desc> anim = {
			{"walk",	  "data/Sample/Enemy/Anim/Walk.mv1",	 0, 1.0f},
			{ "death", "data/Sample/Enemy/Anim/Death.mv1", 0, 1.0f}
		};

		AddComponent<ComponentModel>( "data/Sample/Enemy/Model.mv1" )
			->SetAnimation( anim );

		SetTranslate( { GetRand( 500 ) - 250, 5, GetRand( 500 ) - 250 }
		);	  // 位置(-20,5,-50)に移動します

		// コリジョンカプセル コンポーネント
		AddComponent<ComponentCollisionCapsule>()
			->SetCollisionGroup( ComponentCollision::CollisionGroup::ENEMY )
			->SetRadius( 5.5f )
			->SetHeight( 21.5f )
			->UseGravity();

		return true;
	}

	//! @brief 更新
	void Enemy::Update() /* override */
	{
		Super::Update();

		// 弾当たってたら、ここより下にいきたくない
		if( is_dead_ )
			return;

		// アニメーションしてなければ歩きモーションへ
		if( auto model = GetComponent<ComponentModel>() )
			model->PlayAnimationNoSame( "walk", true );

		//-------------------------------------------------------------------------------
		// 敵の方向をプレイヤーのほうに向ける　④
		//-------------------------------------------------------------------------------
		auto player = Scene::Object::Get<Object>( "Player" );

		// プレイヤーとエネミーが存在している時のみ
		if( player )
		{
			// プレイヤーの方向に、敵の向きを最大3度回転させます
			SetRotationToPositionWithLimit( player->GetTranslate(), 3.0f );

			//-------------------------------------------------------------------------------
			// 敵を前に進める　⑤
			//-------------------------------------------------------------------------------
			// モデルの向きはGizmoを見る限り-Zの方向を向いていることがわかる
			AddTranslate( { 0, 0, -0.1 }, true );
		}
		//-------------------------------------------------------------------------------
	}

	// 当たり判定をここでとることができる
	void Enemy::OnHit( const ComponentCollision::HitInfo& hit_info
	) /*override*/
	{
		// 相手のコリジョンについている名前を取得できます
		auto other_name = hit_info.hit_collision_->GetOwner()->GetNameDefault();

		// 自分の名前を取得できます
		auto my_name = hit_info.collision_->GetOwner()->GetNameDefault();
#if 0
		printfDx( "自分 　　　　　: %s \n", my_name.data() );
		printfDx( "当たっている   : %s \n", other_name.data() );
		printfDx( "---------------------------\n" );
#endif
		if( other_name == "Shot" )
		{
			// deadフラグを立てる 倒れたことにする
			is_dead_ = true;

			// Deathモーションに変更
			// アニメーションしてなければ倒れるモーションへ
			if( auto model = GetComponent<ComponentModel>() )
				model->PlayAnimationNoSame( "death" );

			// コリジョンを消す
			if( auto collision = GetComponent<ComponentCollisionCapsule>() )
				RemoveComponent( collision );

			// 自分(Enemy)を消去
			// Scene::Object::Release( SharedThis() );

			// 相手のコリジョンのオーナーを消去
			Scene::Object::Release( hit_info.hit_collision_->GetOwnerPtr() );
		}

		// 最後にこれを入れてください。ここでめり込みの解消を行っています。
		Super::OnHit( hit_info );
	}

}	 // namespace Game01
