#pragma once
#include <System/Object.h>
#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentObjectController.h>

namespace Game01
{
	class ComponentAttack3: public Component
	{
	public:
		BP_COMPONENT_DECL( ComponentAttack3, u8"攻撃3コンポーネント" )

		void Init() override;

		void Update() override;

		void GUI();
	};
}	 // namespace Game01
