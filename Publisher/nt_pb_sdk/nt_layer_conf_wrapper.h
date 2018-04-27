/*
* Author:YIENIJAN
* Copyright (C) 2015-2017 YIENIJAN. All rights reserved.
*
*/

// 这个类目的是方便各位操作叠加层，做一个简单的封装, 如果您有好的方式，请使用自己的方式去处理
// 后续增加更多功能时，可能会调整


#pragma once

#include <assert.h>
#include "../SmartPublisherSDK/nt_smart_publisher_sdk.h"

// 字节对齐
inline NT_UINT32 NT_ByteAlign(NT_UINT32 d, NT_UINT32 a) { return ((d)+(a - 1)) & ~(a - 1); }

namespace nt_pb_sdk
{
	class layer_conf_wrapper_base
	{
	public:
		virtual ~layer_conf_wrapper_base();

		virtual  NT_PB_LayerBaseConfig const* getBase() const = 0;

		virtual  NT_PB_LayerBaseConfig* getBase() = 0;
	};

	template <typename T, NT_INT32 LayerType>
	class layer_conf_wrapper : public layer_conf_wrapper_base
	{
	public:
		layer_conf_wrapper()
		{
			memset(&conf_, 0, sizeof(conf_));

			conf_.base_.type_ = LayerType;
			conf_.base_.offset_ = offsetof(T, base_);
			conf_.base_.cb_size_ = sizeof(conf_);

			conf_.base_.enable_ = 1;
		}

		layer_conf_wrapper(NT_INT32 index, bool is_enable,
			NT_INT32 region_x, NT_INT32 region_y,
			NT_INT32 region_w, NT_INT32 region_h)
		{
			memset(&conf_, 0, sizeof(conf_));

			conf_.base_.type_ = LayerType;
			conf_.base_.offset_ = offsetof(T, base_);
			conf_.base_.cb_size_ = sizeof(conf_);

			conf_.base_.enable_ = is_enable ? 1 : 0;

			assert(index >= 0);
			conf_.base_.index_ = index;

			conf_.base_.region_.x_ = region_x;
			conf_.base_.region_.y_ = region_y;

			assert(region_w > 0);
			conf_.base_.region_.width_ = region_w;

			assert(region_h > 0);
			conf_.base_.region_.height_ = region_h;
		}

		virtual ~layer_conf_wrapper();

		virtual  NT_PB_LayerBaseConfig const* getBase() const;

		virtual  NT_PB_LayerBaseConfig* getBase();

		T conf_;
	};

	template <typename T, NT_INT32 LayerType>
	layer_conf_wrapper<T, LayerType>::~layer_conf_wrapper()
	{

	}

	template <typename T, NT_INT32 LayerType>
	NT_PB_LayerBaseConfig const* layer_conf_wrapper<T, LayerType>::getBase() const
	{
		return &conf_.base_;
	}

	template <typename T, NT_INT32 LayerType>
	NT_PB_LayerBaseConfig* layer_conf_wrapper<T, LayerType>::getBase()
	{
		return &conf_.base_;
	}

	typedef layer_conf_wrapper<NT_PB_ScreenLayerConfig, NT_PB_E_LAYER_TYPE_SCREEN> ScreenLayerConfigWrapper;

	typedef layer_conf_wrapper<NT_PB_CameraLayerConfig, NT_PB_E_LAYER_TYPE_CAMERA> CameraLayerConfigWrapper;

	typedef layer_conf_wrapper<NT_PB_CameraLayerConfigV2, NT_PB_E_LAYER_TYPE_CAMERA> CameraLayerConfigWrapperV2;

	typedef layer_conf_wrapper<NT_PB_RGBARectangleLayerConfig, NT_PB_E_LAYER_TYPE_RGBA_RECTANGLE> RGBARectangleLayerConfigWrapper;

	typedef layer_conf_wrapper<NT_PB_ImageLayerConfig, NT_PB_E_LAYER_TYPE_IMAGE> ImageLayerConfigWrapper;

	typedef layer_conf_wrapper<NT_PB_WindowLayerConfig, NT_PB_E_LAYER_TYPE_WINDOW> WindowLayerConfigWrapper;
}
