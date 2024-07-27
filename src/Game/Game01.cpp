#include <System/Scene.h>
// 使用するコンポーネントをインクルードする必要がある
#include <System/Component/ComponentCamera.h>			   // カメラ機能を使用します
#include <System/Component/ComponentModel.h>			   // モデル機能を使用します
#include <System/Component/ComponentObjectController.h>	   // オブジェクトコントローラー機能を使用します
#include <System/Component/ComponentCollisionModel.h>	   // コリジョンモデル機能を使用します
#include <System/Component/ComponentCollisionCapsule.h>	   // コリジョンカプセル機能を使用します
#include <System/Component/ComponentCollisionSphere.h>	   // コリジョンスフィア機能を使用します
#include <System/Component/ComponentSpringArm.h>		   // スプリングアーム機能を使用します

//! Enemyクラス
class Enemy: public Object
{
public:
	BP_OBJECT_DECL( Enemy, u8"今回のEnemyです" );

	//! @brief 初期化
	//! @return 初期化済み
	bool Init() override
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
	void Update() override
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
	void OnHit( const ComponentCollision::HitInfo& hit_info ) override
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

private:
	bool is_dead_ = false;
};

// 以下を設定すればセーブとロードが出来るようになる
// (ただし現在アニメーション復帰せず)
CEREAL_REGISTER_TYPE( Enemy )
CEREAL_REGISTER_POLYMORPHIC_RELATION( Object, Enemy )

class Shot: public Object
{
public:
	BP_OBJECT_DECL( Shot, u8"今回のShotです" );

	bool Init() override
	{
		Super::Init();

		AddComponent<ComponentModel>( "data/Sample/Coin/GoldCoin.mv1" );

		AddComponent<ComponentCollisionSphere>()
			->SetRadius( 15 )
			->SetCollisionGroup( ComponentCollision::CollisionGroup::ETC )
			->SetHitCollisionGroup( (u32
			)ComponentCollision::CollisionGroup::ENEMY );

		return true;
	}

	void Update() override
	{
		Super::Update();
		AddTranslate( { 0, 0, -5 }, true );

		// 生存時間を減らす
		life_ -= GetDeltaTime();

		if( life_ <= 0.0f )
		{
			// 生存時間がなくなったら消滅させる
			Scene::Object::Release( SharedThis() );
		}
	}

private:
	float life_ = 5.0f;
};

// 以下を設定すればセーブとロードが出来るようになる
// (ただし現在アニメーション復帰せず)
CEREAL_REGISTER_TYPE( Shot )
CEREAL_REGISTER_POLYMORPHIC_RELATION( Object, Shot )

class Player: public Object
{
public:
	BP_OBJECT_DECL( Player, u8"今回の Player." );

	// ⓪ Initのテンプレートを作成
	// bool Init() override
	//{
	//	Super::Init();

	//	return true;
	//}
	// ① Playerのクラスの Initへ、シーンで作成した Player部分を移植する
	// ➁ auto obj = Scene::Object::Create<Object>( "Player" ); を消す
	// ③ obj-> を　this->　に変更する
	// ④ this-> は必要ではない。よって消去する。

	bool Init() override
	{
		Super::Init();

		// auto obj = Scene::Object::Create<Object>( "Player" );
		AddComponent<ComponentModel>( "data/Game/Model/Player/Maw J Laygo.mv1" )
			->SetAnimation( {
				{"idle",
				  "data/Game/Model/Player/Catwalk Idle 01.mv1",
				  0,
				  1.0f												  },
				{ "walk", "data/Game/Model/Player/Walking.mv1", 0, 1.0f},
		} );

		auto controller = AddComponent<ComponentObjectController>();
		controller->SetMoveSpeed( 0.5f );
		controller->SetRotateSpeed( 20.0f );
		controller->SetCameraRotate( { -25, 0, 0 } );

		// コリジョンカプセル コンポーネント
		AddComponent<ComponentCollisionCapsule>()
			->SetRadius( 3.0f )
			->SetHeight( 13.0f )
			->SetCollisionGroup( ComponentCollision::CollisionGroup::PLAYER )
			->SetHitCollisionGroup(
				(u32)ComponentCollision::CollisionGroup::GROUND |
				(u32)ComponentCollision::CollisionGroup::ENEMY
			)
			->UseGravity();

		return true;
	}

	//! @brief 更新
	void Update() override
	{
		Super::Update();

		//--------------------------------------------------
		// スペースキーが押されたら弾を撃つ ④
		//--------------------------------------------------
		if( Input::IsKeyDown( KEY_INPUT_SPACE ) )
		{
			// 位置(pos)と方向(mat)
			float3 pos = { 0, 0, 0 };
			matrix mat = matrix::identity();
			if( auto model = GetComponent<ComponentModel>() )
			{
				// モデルの完全な向きは、GetWorldMatrix()でとる必要があります。
				mat = model->GetWorldMatrix();
				pos = GetTranslate() + mul( float4{ 0, 60, -30, 0 }, mat ).xyz;
			}

			// Shotを作成する
			// 制御しやすいようにPlayerのモデルと同じ方向を向かせる
			Scene::Object::Create<Shot>( "Shot" )
				->SetMatrix( mat
				)	 // 位置はプレイヤーの前(30.0)、足元より上(60.0)におく
				->SetTranslate( pos
				);	  // (モデルの大きさが0.1倍されているため動かす量も大きい)
		}
		//--------------------------------------------------
	}
};

class Camera: public Object
{
public:
	BP_OBJECT_DECL( Camera, u8"今回の Camera." );

	// ① Cameraのクラスの Initへ、シーンで作成した Camera部分を移植する
	// ➁ auto obj = Scene::Object::Create<Object>( "Camera" ); を消す
	// ③ obj-> を　this->　に変更する
	// ④ this-> は必要ではない。よって消去する。

	bool Init() override
	{
		Super::Init();

		// auto obj = Scene::Object::Create<Object>( "Camera" );
		//  obj->AddComponent<ComponentCamera>()->SetPositionAndTarget({0,
		//  20, 50}, {0, 10, 0});

		// 追跡カメラ設定
		AddComponent<ComponentCamera>()
			->SetPositionAndTarget( { 0, 0, -1 }, { 0, 0, 0 } );
		AddComponent<ComponentSpringArm>(
		)	 // スプリングアーム　コンポーネント
			->SetSpringArmOffset( { 0, 5, 0 } )
			->SetSpringArmLength( 50.0f )
			->SetSpringArmObject( "Player" );

		AddComponent<ComponentCollisionSphere>();

		return true;
	}
};

//! @brief Game01シーン
class Game01: public Scene::Base
{
public:
	BP_CLASS_DECL( Game01, u8"Game01のシーン" );

	//! @brief 初期化
	//! @return 初期化済み
	bool Init() override
	{
		// Ground
		{
			auto obj = Scene::Object::Create<Object>( "Ground" );
			obj->AddComponent<ComponentModel>(
				"data/Sample/SwordBout/Stage/Stage00.mv1"
			);
			obj->AddComponent<ComponentCollisionModel>()->AttachToModel(
			);	  // モデルがないとアタッチできない
		}

#if 1	 // クラス指定でPlayerを作成する方法

		Scene::Object::Create<Player>( "Player" );

#else	 // シーンにてPlayerを作成する方法
		{
			auto obj = Scene::Object::Create<Object>( "Player" );
			obj->AddComponent<ComponentModel>( "data/Sample/Player/Model.mv1" )
				->SetAnimation( {
					{"idle",  "data/Sample/Player/Anim/Idle.mv1", 1, 1.0f},
					{ "walk", "data/Sample/Player/Anim/Walk.mv1", 1, 1.0f},
			} )
				//->SetScaleAxisXYZ({0.16, 0.05, 0.16})
				//->PlayAnimation("walk", true)
				;

			// obj->SetTranslate({0, 5, 0});    // 位置(0,5,0)に移動します

			auto controller = obj->AddComponent<ComponentObjectController>();
			controller->SetMoveSpeed( 0.5f );
			controller->SetRotateSpeed( 20.0f );
			controller->SetCameraRotate( { -25, 0, 0 } );

			// コリジョンカプセル コンポーネント
			obj->AddComponent<ComponentCollisionCapsule>()
				->SetRadius( 3.0f )
				->SetHeight( 13.0f )
				->SetCollisionGroup( ComponentCollision::CollisionGroup::PLAYER
				)
				->SetHitCollisionGroup(
					(u32)ComponentCollision::CollisionGroup::GROUND |
					(u32)ComponentCollision::CollisionGroup::ENEMY
				)
				->UseGravity();
		}
#endif
		// -----------------------------------------------------------------------------------------
		// 空オブジェクト(SkyDome)の追加 ④
		// -----------------------------------------------------------------------------------------
		{
			auto obj = Scene::Object::Create<Object>()->SetName( "Sky" );

			// オブジェクトにモデル能力を追加します
			obj->AddComponent<ComponentModel>(
				"data/Sample/SwordBout/Stage/Stage00_sky.mv1"
			);

			obj->SetTranslate( { 0, 0, 0 } );
		}

#if 1	 // クラスのカメラを起動する

		Scene::Object::Create<Camera>( "Camera" );

#else	 // シーンにObjectを作ってカメラ機能を持たせる
		 // Camera
		{
			auto obj = Scene::Object::Create<Object>( "Camera" );
			// obj->AddComponent<ComponentCamera>()->SetPositionAndTarget({0,
			// 20, 50}, {0, 10, 0});

			// 追跡カメラ設定
			obj->AddComponent<ComponentCamera>()
				->SetPositionAndTarget( { 0, 0, -1 }, { 0, 0, 0 } );
			obj->AddComponent<ComponentSpringArm>(
			)	 // スプリングアーム　コンポーネント
				->SetSpringArmOffset( { 0, 5, 0 } )
				->SetSpringArmLength( 50.0f )
				->SetSpringArmObject( "Player" );

			obj->AddComponent<ComponentCollisionSphere>();
		}
#endif

		// Enemy
		for( int i = 0; i < 10; i++ )
		{
			Scene::Object::Create<Enemy>( "Enemy" );
		}

		return true;
	}

	//! @brief 更新
	void Update() override
	{
		// 毎フレーム動作する
		//--------------------------------------------------------------
		// 雲を動かすように空をY軸で少しづつ回転させます　⑤
		//--------------------------------------------------------------
		if( auto sky = Scene::Object::Get<Object>( "Sky" ) )
		{
			sky->AddRotationAxisXYZ( { 0, 0.01f, 0 } );
		}
		//--------------------------------------------------------------
	}

private:
	// 必要であれば変数をここに追加する
};
