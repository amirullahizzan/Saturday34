﻿//---------------------------------------------------------------------------
//! @file   PhysicsLayer.h
//! @brief  物理シミュレーション レイヤー管理
//---------------------------------------------------------------------------
#pragma once

namespace physics
{

	// オブジェクトが属することのできるレイヤーで、他のどのオブジェクトと衝突することができるかを決定します。
	// 一般的には、少なくとも動的オブジェクトのレイヤーと静止オブジェクトのレイヤーが必要ですが、
	// 必要であればもっと多くのレイヤーを持つことができます。
	// 例えば、ハイディテールコリジョン用のレイヤーを持つこともできます
	// （これは物理シミュレーションでは使用されませんが、コリジョンテストを行う場合にのみ使用されます）。
	enum ObjectLayers : u16
	{
		NON_MOVING = 0,	   //!< 静的。動かない固定
		MOVING	   = 1,	   //!< 動的
		DEBRIS	   = 2,	   //!< 破片エフェクトなどは NON_MOVING のみ衝突
		SENSOR	   = 3,	   //!< センサーは MOVING オブジェクトにのみ衝突
		//--------
		MAX_COUNT = 4,	  //!< 定義最大数
	};

}	 // namespace physics
