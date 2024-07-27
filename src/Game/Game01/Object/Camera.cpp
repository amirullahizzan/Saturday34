#include "Camera.h"
#include <System/Component/ComponentCamera.h>
#include <System/Component/ComponentSpringArm.h>
#include <System/Component/ComponentCollisionSphere.h>

namespace Game01
{
	bool Camera::Init() /* override */
	{
		Super::Init();

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

}	 // namespace Game01
