#include "ComponentAttack3.h"

namespace Game01
{
	void ComponentAttack3::Init()
	{
		Super::Init();

		if( auto control =
				GetOwner()->GetComponent<ComponentObjectController>() )
		{
			control->SetEnable( false );
		}

		if( auto model = GetOwner()->GetComponent<ComponentModel>() )
		{
			model->PlayAnimation( "attack3" );
		}
	}

	void ComponentAttack3::Update()
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
		}
	}

	void ComponentAttack3::GUI()
	{
		Super::GUI();
		ImGui::Begin( GetOwner()->GetName().data() );
		{
			ImGui::Separator();
			if( ImGui::TreeNode( "Attack3" ) )
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
