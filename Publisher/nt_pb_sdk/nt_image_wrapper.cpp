/*
* Author:YIENIJAN
* Copyright (C) 2015-2017 YIENIJAN. All rights reserved.
*
*/

#include "stdafx.h"
#include "nt_image_wrapper.h"
#include <assert.h>

namespace nt_pb_sdk
{
	image_wrapper::image_wrapper(const NT_SmartPublisherImageSDKAPI& pb_image_api, NT_PB_Image* image)
		:image_(image)
	{
		pb_image_api_ = pb_image_api;
	}

	image_wrapper::image_wrapper(const NT_SmartPublisherImageSDKAPI& pb_image_api)
		: image_(nullptr)
	{
		pb_image_api_ = pb_image_api;
	}

	image_wrapper::image_wrapper(const image_wrapper& r)
	{
		pb_image_api_ = r.pb_image_api_;
		if (r.image_ != nullptr)
		{
			image_ = pb_image_api_.CloneImage(r.image_);
		}
		else
		{
			image_ = nullptr;
		}
	}


	image_wrapper& image_wrapper::operator=(const image_wrapper& r)
	{
		if (this == &r)
		{
			return *this;
		}

		if (image_ != nullptr)
		{
			pb_image_api_.FreeImage(&image_);
		}

		pb_image_api_ = r.pb_image_api_;

		if (r.image_ != nullptr)
		{
			image_ = pb_image_api_.CloneImage(r.image_);
		}

		return *this;
	}

	image_wrapper::~image_wrapper()
	{
		pb_image_api_.FreeImage(&image_);
	}

	void image_wrapper::Reset(NT_PB_Image* image)
	{
		if (image_ != nullptr)
		{
			pb_image_api_.FreeImage(&image_);
		}

		image_ = image;
	}

	NT_PB_Image* image_wrapper::Release()
	{
		auto old = image_;
		image_ = nullptr;
		return old;
	}

	NT_INT32 image_wrapper::Format() const
	{
		assert(image_ != nullptr);
		return image_->format_;
	}

	NT_INT32 image_wrapper::Width() const
	{
		assert(image_ != nullptr);
		return image_->width_;
	}

	NT_INT32 image_wrapper::Height() const
	{
		assert(image_ != nullptr);
		return image_->height_;
	}

	NT_UINT64 image_wrapper::Timestamp() const
	{
		assert(image_ != nullptr);
		return image_->timestamp_;
	}

	NT_UINT8* image_wrapper::Plane(NT_INT32 index)
	{
		assert(image_ != nullptr);
		assert(index >= 0 && index < NT_PB_IMAGE_MAX_PLANE_NUM);

		return image_->plane_[index];
	}

	const NT_UINT8* image_wrapper::Plane(NT_INT32 index) const
	{
		assert(image_ != nullptr);
		assert(index >= 0 && index < NT_PB_IMAGE_MAX_PLANE_NUM);

		return image_->plane_[index];
	}

	NT_INT32 image_wrapper::Stride(NT_INT32 index)
	{
		assert(image_ != nullptr);
		assert(index >= 0 && index < NT_PB_IMAGE_MAX_PLANE_NUM);

		return image_->stride_[index];
	}

	NT_INT32 image_wrapper::Stride(NT_INT32 index) const
	{
		assert(image_ != nullptr);
		assert(index >= 0 && index < NT_PB_IMAGE_MAX_PLANE_NUM);

		return image_->stride_[index];
	}

	NT_INT32 image_wrapper::PlaneSize(NT_INT32 index)
	{
		assert(image_ != nullptr);
		assert(index >= 0 && index < NT_PB_IMAGE_MAX_PLANE_NUM);

		return image_->plane_size_[index];
	}

	NT_INT32 image_wrapper::PlaneSize(NT_INT32 index) const
	{
		assert(image_ != nullptr);
		assert(index >= 0 && index < NT_PB_IMAGE_MAX_PLANE_NUM);

		return image_->plane_size_[index];
	}

	NT_PB_Image** image_wrapper::getPP()
	{
		assert(image_ == nullptr);
		return &image_;
	}

}

