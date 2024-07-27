#pragma once
#include <System/Object.h>

namespace Game01
{
	class Player: public Object
	{
	public:
		BP_OBJECT_DECL( Player, "[Game01] Player" );

		bool Init() override;

		//! @brief 更新
		void Update() override;
	};

}	 // namespace Game01
