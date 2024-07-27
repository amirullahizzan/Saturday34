#include "Player.h"
#include "Shot.h"
#include <Game/Game01/Component/ComponentAttack.h>
#include <Game/Game01/Component/ComponentAttack2.h>
#include <Game/Game01/Component/ComponentAttack3.h>
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentObjectController.h>
#include <System/Component/ComponentCollisionCapsule.h>
#include <System/Component/ComponentAttachModel.h>

namespace Game01
{

	bool Player::Init() /* override */
	{
		Super::Init();

		// auto obj = Scene::Object::Create<Object>( "Player" );
		auto model = AddComponent<ComponentModel>( "data/Game/Model/Player/Maw J Laygo.mv1" );
		model->SetAnimation( {
			{"idle",
			  "data/Game/Model/Player/Catwalk Idle 01.mv1",
			  0,
			  1.0f															 },
			{ "walk",	  "data/Game/Model/Player/Walking.mv1",			0, 1.0f},
			{ "attack",	"data/Game/Model/Player/Punching.mv1",		   0, 1.0f},
			{ "attack2",
			  "data/Game/Model/Player/Mutant Punch.mv1",
			  0,
			  1.0f															 },
			{ "attack3",
			  "data/Game/Model/Player/Standing 2H Magic Attack 02.mv1",
			  0,
			  1.0f															 },
		} );

		//Scene::CreateObjectPtr<Object>();
	//	AddComponent<ComponentAttachModel>(  )
	//		->SetAttachObject( SharedThis(), "mixamorig:LeftHand" );
#if 1
		auto controller = AddComponent<ComponentObjectController>();
		controller->SetMoveSpeed( 0.5f );
		controller->SetRotateSpeed( 20.0f );
		controller->SetCameraRotate( { -25, 0, 0 } );
#endif
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
	void Player::Update() /* override */
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
#if 0
			// Shotを作成する
			// 制御しやすいようにPlayerのモデルと同じ方向を向かせる
			Scene::Object::Create<Shot>( "Shot" )
				->SetMatrix( mat
				)	 // 位置はプレイヤーの前(30.0)、足元より上(60.0)におく
				->SetTranslate( pos
				);	  // (モデルの大きさが0.1倍されているため動かす量も大きい)
#endif
		}
		if( Input::IsKeyDown( KEY_INPUT_B ) )
		{
			auto atk1 = GetComponent<ComponentAttack>();
			auto atk2 = GetComponent<ComponentAttack2>();
			auto atk3 = GetComponent<ComponentAttack3>();

			if( !atk1 && !atk2 && !atk3 )
				AddComponent<ComponentAttack>();
		}

		// if( Input::IsKeyDown( KEY_INPUT_N ) )
		//{
		//	auto atk1 = GetComponent<ComponentAttack>();
		//	auto atk2 = GetComponent<ComponentAttack2>();

		//	if( !atk1 && !atk2 )
		//		AddComponent<ComponentAttack2>();
		//}

		//	if( Input::IsKeyDown( KEY_INPUT_DELETE ) )
		//{
		//	auto atk2 = GetComponent<ComponentAttack2>();
		//	auto atk3 = GetComponent<ComponentAttack3>();

		//	if( !atk2 && !atk3 )
		//		AddComponent<ComponentAttack3>();
		//}

		//--------------------------------------------------
	}
}	 // namespace Game01
