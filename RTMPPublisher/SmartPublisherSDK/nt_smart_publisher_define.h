/*
* Author:YIENIJAN
* Copyright (C) 2015-2017 YIENIJAN. All rights reserved.
*
*/


#ifndef NT_SMART_PUBLISHER_DEFINE_H_
#define NT_SMART_PUBLISHER_DEFINE_H_

#ifdef WIN32
#include <windows.h>
#endif

#include "nt_type_define.h"
#include "nt_base_code_define.h"

#ifdef __cplusplus
extern "C"{
#endif

#ifndef NT_HWND_
#define NT_HWND_

#ifdef _WIN32
typedef HWND NT_HWND;
#else
typedef void* NT_HWND;
#endif

#endif

#ifndef NT_HDC_
#define NT_HDC_

#ifdef _WIN32
typedef HDC NT_HDC;
#else
typedef void* NT_HDC;
#endif

#endif


#define NT_PB_CAMERA_UNIQUE_ID_UTF8_MAX_SIZE	(1024) //摄像头唯一ID最大值是1023字符, 再加一个0
#define NT_PB_MAX_FILE_PATH_LENGTH				(2048) // 最大文件路径限制,注意还需要包括个0

/*错误码*/
typedef enum _NT_PB_E_ERROR_CODE
{
	NT_ERC_PB_HWND_IS_NULL				= (NT_ERC_SMART_PUBLISHER_SDK | 0x1), // 窗口句柄是空
	NT_ERC_PB_HWND_INVALID				= (NT_ERC_SMART_PUBLISHER_SDK | 0x2), // 窗口句柄无效
	NT_ERC_PB_DIR_NOT_EXIST				= (NT_ERC_SMART_PUBLISHER_SDK | 0x3), // 目录不存在
	NT_ERC_PB_INVALID_SCREEN_RECTREGION = (NT_ERC_SMART_PUBLISHER_SDK | 0x4), // 无效的屏幕矩形区域
	NT_ERC_PB_URL_EXIST					= (NT_ERC_SMART_PUBLISHER_SDK | 0x5), // 推送URL已经存在
	NT_ERC_PB_NO_IMAGE					= (NT_ERC_SMART_PUBLISHER_SDK | 0x6), // 没有图像
	NT_ERC_PB_TOO_MANY_CAPTURE_IMAGE_REQUESTS = (NT_ERC_SMART_PUBLISHER_SDK | 0x7), // 太多的截图请求
} NT_PB_E_ERROR_CODE;


/*设置参数ID, 这个目前这么写，SmartPlayerSDK 已经划分范围*/
typedef enum _NT_PB_E_PARAM_ID
{
	NT_PB_PARAM_ID_BASE = NT_PARAM_ID_SMART_PUBLISHER_SDK,
	
} NT_PB_E_PARAM_ID;


/*事件ID*/
typedef enum _NT_PB_E_EVENT_ID
{
	NT_PB_E_EVENT_ID_BASE = NT_EVENT_ID_SMART_PUBLISHER_SDK,

	NT_PB_E_EVENT_ID_CONNECTING			= NT_PB_E_EVENT_ID_BASE | 0x2,	/*连接中, param5表示推送URL */
	NT_PB_E_EVENT_ID_CONNECTION_FAILED	= NT_PB_E_EVENT_ID_BASE | 0x3,	/*连接失败, param5表示推送URL*/
	NT_PB_E_EVENT_ID_CONNECTED			= NT_PB_E_EVENT_ID_BASE | 0x4,	/*已连接, param5表示推送URL*/
	NT_PB_E_EVENT_ID_DISCONNECTED		= NT_PB_E_EVENT_ID_BASE | 0x5,	/*断开连接, param5表示推送URL*/
	
	NT_PB_E_EVENT_ID_RECORDER_START_NEW_FILE    = NT_PB_E_EVENT_ID_BASE | 0x7,	/*录像写入新文件, param5表示录像文件名*/
	NT_PB_E_EVENT_ID_ONE_RECORDER_FILE_FINISHED = NT_PB_E_EVENT_ID_BASE | 0x8,	/*一个录像文件完成, param5表示录像文件名*/

	NT_PB_E_EVENT_ID_CAPTURE_WINDOW_INVALID = NT_PB_E_EVENT_ID_BASE | 0xd, /*捕获窗口时，如果窗口句柄无效则通知用户, param1为窗口句柄*/

} NT_PB_E_EVENT_ID;


/*定义Video源选项*/
typedef enum _NT_PB_E_VIDEO_OPTION
{
	NT_PB_E_VIDEO_OPTION_NO_VIDEO = 0x0,
	NT_PB_E_VIDEO_OPTION_SCREEN   = 0x1, // 采集屏幕
	NT_PB_E_VIDEO_OPTION_CAMERA	  = 0x2, // 摄像头采集
	NT_PB_E_VIDEO_OPTION_LAYER    = 0x3, // 视频合并，比如桌面叠加摄像头等
	NT_PB_E_VIDEO_OPTION_ENCODED_DATA = 0x4, // 已经编码的视频数据，目前支持H264
	NT_PB_E_VIDEO_OPTION_WINDOW   = 0x5, // 采集窗口
} NT_PB_E_VIDEO_OPTION;

/*定义Auido源选项*/
typedef enum _NT_PB_E_AUDIO_OPTION
{
	NT_PB_E_AUDIO_OPTION_NO_AUDIO					= 0x0,
	NT_PB_E_AUDIO_OPTION_CAPTURE_MIC				= 0x1, // 采集麦克风音频
	NT_PB_E_AUDIO_OPTION_CAPTURE_SPEAKER			= 0x2, // 采集扬声器
	NT_PB_E_AUDIO_OPTION_CAPTURE_MIC_SPEAKER_MIXER	= 0x3, // 麦克风扬声器混音
	NT_PB_E_AUDIO_OPTION_ENCODED_DATA				= 0x4, // 编码后的音频数据，目前支持AAC, speex宽带(wideband mode)

} NT_PB_E_AUDIO_OPTION;

// 屏幕区域选择消息ID
typedef enum _NT_PB_E_SCREEN_REGION_CHOOSE_MSG_ID
{
	NT_PB_E_SCREEN_REGION_CHOOSE_MSG_CANCEL = 1, // 取消选择， 用户按了ESC键, 或者用户双击但是没有选择任何区域
	NT_PB_E_SCREEN_REGION_CHOOSE_MSG_OK		= 2, // 选择成功
	NT_PB_E_SCREEN_REGION_CHOOSE_MSG_MOVE	= 3, // 用户移动区域, 这个只在移动模式下才会发送这个消息

} NT_PB_E_SCREEN_REGION_CHOOSE_MSG_ID;

// 屏幕区域选择模式
typedef enum _NT_PB_E_SCREEN_REGION_CHOOSE_MODE
{
	NT_PB_E_SCREEN_REGION_CHOOSE_MODE_CHOOSE = 1, // 当前是选择模式
	NT_PB_E_SCREEN_REGION_CHOOSE_MODE_MOVE   = 2, // 当前是移动模式
} NT_PB_E_SCREEN_REGION_CHOOSE_MODE;


typedef enum _NT_PB_E_IMAGE_FORMAT
{
	NT_PB_E_IMAGE_FORMAT_RGB32 = 1, // 32位的rgb格式, r, g, b各占8, 另外一个字节保留, 内存字节格式为: bb gg rr xx, 主要是和windows位图匹配, 在小端模式下，按DWORD类型操作，最高位是xx, 依次是rr, gg, bb
	NT_PB_E_IMAGE_FORMAT_ARGB  = 2, // 32位的argb格式，内存字节格式是: bb gg rr aa 这种类型，和windows位图匹配
	NT_PB_E_IMAGE_FORMAT_I420  = 3, // YUV420格式, 三个分量保存在三个面上
} NT_PB_E_IMAGE_FORMAT;


// 视频合成时，每一层的类型
typedef enum _NT_PB_E_LAYER_TYPE
{
	NT_PB_E_LAYER_TYPE_SCREEN				= 1, // 屏幕层
	NT_PB_E_LAYER_TYPE_CAMERA				= 2, // 摄像头层
	NT_PB_E_LAYER_TYPE_RGBA_RECTANGLE		= 3, // RGBA矩形
	NT_PB_E_LAYER_TYPE_IMAGE				= 4, // 图片层
	NT_PB_E_LAYER_TYPE_EXTERNAL_VIDEO_FRAME = 5, // 外部视频数据层
	NT_PB_E_LAYER_TYPE_WINDOW               = 6, // 窗口层
} NT_PB_E_LAYER_TYPE;


/*
推送状态
*/
typedef enum _NT_PB_E_PUBLISHER_STATUS
{
	NT_PB_E_PUBLISHER_STATUS_CONNECTING	= 1001, /*连接中*/
	NT_PB_E_PUBLISHER_STATUS_CONNECTION_FAILED, /*连接失败*/
	NT_PB_E_PUBLISHER_STATUS_CONNECTED, /*已连接*/
	NT_PB_E_PUBLISHER_STATUS_DISCONNECTED, /*断开连接*/
} NT_PB_E_PUBLISHER_STATUS;



// 如果三项都是0的话，将不能启动录像
typedef struct _NT_PB_RecorderFileNameRuler
{
	NT_UINT32	type_; // 这个值目前默认是0，将来扩展用
	NT_PCSTR	file_name_prefix_; // 设置一个录像文件名前缀, 例如:daniulive
	NT_INT32	append_date_; // 如果是1的话，将在文件名上加日期, 例如:daniulive-2017-01-17
	NT_INT32	append_time_; //  如果是1的话，将增加时间，例如:daniulive-2017-01-17-17-10-36
} NT_PB_RecorderFileNameRuler;

// 这个目前只获取摄像头分辨率列表
typedef struct _NT_PB_VideoCaptureCapability
{
	NT_INT32 width_;
	NT_INT32 height_;
	NT_INT32 max_frame_rate_;
} NT_PB_VideoCaptureCapability;


// 矩形局域
typedef struct _NT_PB_RectRegion
{
	NT_INT32 x_;
	NT_INT32 y_;
	NT_INT32 width_;
	NT_INT32 height_;
} NT_PB_RectRegion;


// 定义图像数据结构.
// 图像始终是自上而下的方式, 如果要自下而上的话， 后续考虑height_可以取负值，这正好和位图默认的方式相反
// 对plane_的操作，请使用SDK提供的接口
// 这个结构体的创建，释放，SDK都提供了相关接口
#define NT_PB_IMAGE_MAX_PLANE_NUM  (16)
typedef struct _NT_PB_Image
{
	NT_UINT32 cb_size_; // 这个结构体的大小
	NT_INT32  format_;  // 图像格式, 请参考NT_PB_E_IMAGE_FORMAT
	NT_INT32  width_;   // 图像宽
	NT_INT32  height_;  // 图像高

	NT_UINT64 timestamp_; // 时间戳, 一般是0,不使用, 以ms为单位的
	
	// 具体的图像数据, 一般只用第一个
	NT_UINT8* plane_[NT_PB_IMAGE_MAX_PLANE_NUM];
	
	// 每一个平面的每一行的字节数，对于argb和rgb32，为了保持和windows位图兼容，必须是width_*4
	NT_INT32  stride_[NT_PB_IMAGE_MAX_PLANE_NUM];

	// 每一个平面的数据大小, 这个可以根据stride_[i]*abs(height_)计算出来
	// 不同格式的计算方式可能不同
	NT_INT32  plane_size_[NT_PB_IMAGE_MAX_PLANE_NUM];

} NT_PB_Image;


// 合成层的基本类型
typedef struct _NT_PB_LayerBaseConfig
{
	NT_INT32		   type_;    // 这个是类型, 具体类型请看NT_PB_E_LAYER_TYPE
	NT_INT32		   index_;   // 层索引, 从0开始
	NT_INT32           enable_;  // 第0层会忽略这个参数， 1:表示启用， 0表示禁用，其他值将报错
	NT_PB_RectRegion   region_;  // 第0层会忽略x, y，宽高将是视频分辨率.
	NT_UINT32          cb_size_; // 这个填充真正类型的大小
	NT_INT32		   offset_;  // 这个是一个偏移量，这个偏移量+这个结构体的指针，就能算出真正的类型指针. 比如 NT_PB_LayerBaseConfig* p; 那么真正的类型地址就是ActualType* pType= (ActualType*)((char*)p - offset_)
} NT_PB_LayerBaseConfig;


// 屏幕层, 运行过程中, 只能修改clip_region_.x_ 和 clip_region_.x_，其他值一律不能修改
// base_.type_ 是 NT_PB_E_LAYER_TYPE_SCREEN
typedef struct _NT_PB_ScreenLayerConfig
{
	NT_PB_LayerBaseConfig	base_;
	NT_PB_RectRegion		clip_region_; // 如果是全屏幕的话，请全部填充0
	NT_PVOID				reserve_; // 保留字段
} NT_PB_ScreenLayerConfig;


// 摄像头层, 运行过程中，任何值都不能修改
// base_.type_ 是 NT_PB_E_LAYER_TYPE_CAMERA
typedef struct _NT_PB_CameraLayerConfig
{
	NT_PB_LayerBaseConfig	base_;
	NT_CHAR					device_unique_id_utf8_[NT_PB_CAMERA_UNIQUE_ID_UTF8_MAX_SIZE];
} NT_PB_CameraLayerConfig;


// 摄像头层, 对NT_PB_CameraLayerConfig的扩展
// base_.type_ 是 NT_PB_E_LAYER_TYPE_CAMERA
typedef struct _NT_PB_CameraLayerConfigV2
{
	NT_PB_LayerBaseConfig	base_;
	NT_CHAR					device_unique_id_utf8_[NT_PB_CAMERA_UNIQUE_ID_UTF8_MAX_SIZE];
	NT_INT32                is_flip_horizontal_; // 是否水平反转 1:水平反转, 0: 不反转
	NT_INT32                is_flip_vertical_; // 是否垂直反转 1:垂直反转, 0:不反转
	NT_INT32                rotate_degress_; // 旋转角度 顺时针旋转 有效值为 0度, 90度, 180度, 270度
} NT_PB_CameraLayerConfigV2;


// RGB矩形层, 运行过程中，可以修改red_, green_, blue_, 和 alpha_， 其他不能修改
// base_.type_ 是 NT_PB_E_LAYER_TYPE_RGBA_RECTANGLE
typedef struct _NT_PB_RGBARectangleLayerConfig
{
	NT_PB_LayerBaseConfig	base_;
	NT_UINT8 red_;		// 红色分量
	NT_UINT8 green_;	// 绿色分量
	NT_UINT8 blue_;     // 蓝色分量
	NT_UINT8 alpha_;	// Alpha通道, 0是完全透明, 255是完全不透明, 注意：如果是第0层的话，这个值将被忽略
} NT_PB_RGBARectangleLayerConfig;


// 图像层， 运行过程中，任何值都不可以修改
// base_.type_ 是 NT_PB_E_LAYER_TYPE_IMAGE
// 目前只支持PNG
typedef struct _NT_PB_ImageLayerConfig
{
	NT_PB_LayerBaseConfig	base_;
	NT_CHAR	  file_name_utf8_[NT_PB_MAX_FILE_PATH_LENGTH]; // 完整的文件名, 
	NT_INT32  is_setting_background_;  // 是否设置了背景, 1：表示设置, 0:表示没有设置. 其他错误. 如果是第0层，不设置背景的话，将会使用白色作为默认背景, 其他层最好不要设置背景，不然不透明了
	NT_UINT8  bk_red_; // 红色分量 
	NT_UINT8  bk_green_; // 绿色分量
	NT_UINT8  bk_blue_; // 蓝色分量
	NT_INT32  reserve_; // 保留，填充0就可以
} NT_PB_ImageLayerConfig;



// 外部视频数据层
// base_.type_ 是 NT_PB_E_LAYER_TYPE_EXTERNAL_VIDEO_FRAME
// 这个层支持外部传入yuv或者rgb数据给sdk
typedef struct _NT_PB_ExternalVideoFrameLayerConfig
{
	NT_PB_LayerBaseConfig	base_;

} NT_PB_ExternalVideoFrameLayerConfig;


// 窗口层， 运行过程中，可以修改窗口句柄
// base_.type_ 是NT_PB_E_LAYER_TYPE_WINDOW
typedef struct _NT_PB_WindowLayerConfig
{
	NT_PB_LayerBaseConfig base_;
	NT_HWND  window_;   // 窗口句柄
	NT_INT32 reserve1_;  // 保留, 填充0就可以
	NT_INT32 reserve2_;  // 保留, 填充0就可以
} NT_PB_WindowLayerConfig;


// 这个主要是用来给sdk设置一组码率用的
// 每一项要填写的参数
typedef struct _NT_PB_VideoEncoderBitrateGroupItem
{
	NT_UINT32 video_area_; // 视频面积, 视频的宽*高
	NT_INT32  kbitrate_;  // 视频码率, 单位kbps
	NT_INT32  max_kbitrate_; // 视频最大码率, 单位kbps

} NT_PB_VideoEncoderBitrateGroupItem;


/*
屏幕区域选择工具CallBack
msg_id: 请参考NT_PB_E_SCREEN_REGION_CHOOSE_MSG_ID
msg_data: msg_id == NT_PB_E_SCREEN_REGION_CHOOSE_MSG_CANCEL, msg_data 是NULL
          msg_id == NT_PB_E_SCREEN_REGION_CHOOSE_MSG_OK, msg_data 是NT_PB_RectRegion*
		  msg_id == NT_PB_E_SCREEN_REGION_CHOOSE_MSG_MOVE, msg_data是NT_PB_RectRegion*
*/
typedef NT_VOID(NT_CALLBACK *NT_PB_SDKScreenRegionChooseCallBack)(NT_HANDLE tool_handle, NT_PVOID userData,
	NT_UINT32 msg_id, NT_PVOID msg_data);


/*
推送SDK, 截屏回调
result: 如果截屏成功的话，result是NT_ERC_OK,其他错误
*/
typedef NT_VOID(NT_CALLBACK *NT_PB_SDKCaptureImageCallBack)(NT_HANDLE handle, NT_PVOID userData, 
	NT_UINT32 result, NT_PCSTR file_name);


/*
当发送rtmp视频包时，会回调, 注意：请不要在回调函数里面做耗时的事情，这样会卡住发包线程
handle: 推送句柄
userData: 用户自定义数据
url:推送URL，主要用在推送多个url时，进行区分
is_changed:时间戳是否改变, 1:表示改变, 0:表示未改变
timestamp:时间戳(单位是毫秒)
reserve1:保留参数
reserve2:保留参数
*/
typedef NT_VOID(NT_CALLBACK *NT_PB_SDKVideoPacketTimestampCallBack)(NT_HANDLE handle, NT_PVOID userData,
	NT_PCSTR url, NT_INT32 is_changed, NT_UINT64 timestamp,
	NT_UINT64 reserve1, NT_PVOID reserve2);


/*
推送状态回调
handle: 推送句柄
user_data: 用户数据
url: 推送URL
status: 推送状态请参考 NT_PB_E_PUBLISHER_STATUS
*/
typedef NT_VOID(NT_CALLBACK *NT_PB_SDKPublisherStatusCallBack)(NT_HANDLE handle, NT_PVOID user_data,
	NT_PCSTR url, NT_UINT32 status);



/*
*推送事件回调
event_id: 事件ID，请参考NT_PB_E_EVENT_ID
param1 到 param7, 值的意义和具体事件ID相关, 注意如果具体事件ID没有说明param1-param7的含义，那说明这个事件不带参数
*/
typedef NT_VOID(NT_CALLBACK* NT_PB_SDKEventCallBack)(NT_HANDLE handle, NT_PVOID user_data,
	NT_UINT32 event_id,
	NT_INT64  param1,
	NT_INT64  param2,
	NT_UINT64 param3,
	NT_UINT64 param4,
	NT_PCSTR  param5,
	NT_PCSTR  param6,
	NT_PVOID  param7
	);


#ifdef __cplusplus
}
#endif

#endif