/*
* Author:YIENIJAN
* Copyright (C) 2015-2017 YIENIJAN. All rights reserved.
*
*/

#pragma once

#include <assert.h>
#include "../SmartPublisherSDK/nt_smart_publisher_sdk.h"

namespace nt_pb_sdk
{
	class image_wrapper
	{
	public:
		image_wrapper(const NT_SmartPublisherImageSDKAPI& pb_image_api, NT_PB_Image* image);
		explicit image_wrapper(const NT_SmartPublisherImageSDKAPI& pb_image_api);
		explicit image_wrapper(const image_wrapper& r);
		image_wrapper& operator=(const image_wrapper& r);
		~image_wrapper();

	public:
		void Reset(NT_PB_Image* image);
		NT_PB_Image* Release();

	public:
		const NT_PB_Image* Get() const { return image_; }
		NT_PB_Image* Get() { return image_; }

		NT_PB_Image** getPP();

	public:
		NT_INT32  Format() const;
		NT_INT32  Width() const;
		NT_INT32  Height() const;
		NT_UINT64 Timestamp() const;

		NT_UINT8* Plane(NT_INT32 index);
		const NT_UINT8* Plane(NT_INT32 index) const;

		NT_INT32  Stride(NT_INT32 index);
		NT_INT32  Stride(NT_INT32 index) const;

		NT_INT32  PlaneSize(NT_INT32 index);
		NT_INT32  PlaneSize(NT_INT32 index) const;

	private:
		// 这里拷贝这个结构体
		NT_SmartPublisherImageSDKAPI pb_image_api_;
		NT_PB_Image* image_;
	};
}

