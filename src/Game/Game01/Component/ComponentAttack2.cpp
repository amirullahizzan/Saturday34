﻿#include "ComponentAttack2.h"

namespace Game01
{
	void ComponentAttack2::Init()
	{
		Super::Init();

		if( auto control =
				GetOwner()->GetComponent<ComponentObjectController>() )
		{
			control->SetEnable( false );
		}

		if( auto model = GetOwner()->GetComponent<ComponentModel>() )
		{
			model->PlayAnimation( "attack2" );
		}
	}

	void ComponentAttack2::Update()
	{
		Super::Update();

		if( auto model = GetOwner()->GetComponent<ComponentModel>() )
		{
			if( !model->IsPlaying() )
			{
				if( auto control =
						GetOwner()->GetComponent<ComponentObjectController>(
						) )
				{
					control->SetEnable( true );
				}

				GetOwner()->RemoveComponent( shared_from_this() );
			}

			auto time = model->GetAnimationTime();

			if( time <= 0.70f )
			{
				// Play中
				if( Input::IsKeyDown( KEY_INPUT_B ) )
				{
					next_attack_ = true;
				}
			}
			if( time >= 0.60f )
			{
				if( next_attack_ )
				{
					GetOwner()->RemoveComponent( shared_from_this() );
					GetOwner()->AddComponent<ComponentAttack3>();
				}
			}
		}
	}

	void ComponentAttack2::GUI()
	{
		Super::GUI();
		ImGui::Begin( GetOwner()->GetName().data() );
		{
			ImGui::Separator();
			if( ImGui::TreeNode( "Attack2" ) )
			{
				if( ImGui::Button( UNIQUE_TEXT( u8"削除" ) ) )
				{
					GetOwner()->RemoveComponent( shared_from_this() );
				}
				ImGui::TreePop();
			}
		}
		ImGui::End();
	}
}	 // namespace Game01
