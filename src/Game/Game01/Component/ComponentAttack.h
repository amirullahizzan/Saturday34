#pragma once
#include <System/Object.h>
#include <Game/Game01/Component/ComponentAttack2.h>
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentObjectController.h>

namespace Game01
{
	class ComponentAttack: public Component
	{
	public:
		BP_COMPONENT_DECL( ComponentAttack, u8"攻撃コンポーネント" )

		void Init() override;

		void Update() override;

		void GUI() override;
	bool next_attack_ = false;
	};


}	 // namespace Game01
