#include "SceneGame.h"

#include <Game/Game01/Object/Player.h>
#include <Game/Game01/Object/Camera.h>
#include <Game/Game01/Object/Enemy.h>

#include <System/Object.h>
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentCollisionModel.h>

namespace Game01
{
	//! @brief 初期化
	//! @return 初期化済み
	bool SceneGame::Init()
	{
		// Ground
		{
			auto obj = Scene::Object::Create<Object>( "Ground" );
			obj->AddComponent<ComponentModel>(
				"data/Sample/SwordBout/Stage/Stage00.mv1"
			);
			obj->AddComponent<ComponentCollisionModel>()	//
				->AttachToModel();							// モデルがないとアタッチできない
		}

		Scene::Object::Create<Player>( "Player" );

		Scene::Object::Create<Camera>( "Camera" );

		// Enemy
		for( int i = 0; i < 10; i++ )
		{
			Scene::Object::Create<Enemy>( "Enemy" );
		}
		return true;
	}

	//! @brief 更新
	void SceneGame::Update()
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

}	 // namespace Game01
