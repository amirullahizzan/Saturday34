#pragma once
#include <System/Object.h>
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentObjectController.h>
#include <Game/Game01/Component/ComponentAttack3.h>

namespace Game01
{
	class ComponentAttack2: public Component
	{
	public:

		BP_COMPONENT_DECL( ComponentAttack2, u8"攻撃2コンポーネント" )

		void Init() override;

		void Update() override;

		void GUI();

		bool next_attack_ = false;

	};
}	 // namespace Game01
