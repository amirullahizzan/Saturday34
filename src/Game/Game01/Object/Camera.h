#pragma once
#include <System/Object.h>

namespace Game01
{
	//! @brief Camera
	class Camera: public Object
	{
	public:
		BP_OBJECT_DECL( Camera, "[Game01] Camera" );

		bool Init() override;
	};

}	 // namespace Game01
