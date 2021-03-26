/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/

//           |                    name                                       |    Aspect Flags   |
//           |   RHI::Format        |      VK_FORMAT                         |Color|Depth|Stencil|
#define RHIVK_EXPAND_FOR_FORMATS(_Func) \
        _Func(R32G32B32A32_FLOAT,      VK_FORMAT_R32G32B32A32_SFLOAT,           1,    0,    0) \
        _Func(R32G32B32A32_UINT,       VK_FORMAT_R32G32B32A32_UINT,             1,    0,    0) \
        _Func(R32G32B32A32_SINT,       VK_FORMAT_R32G32B32A32_SINT,             1,    0,    0) \
        _Func(R32G32B32_FLOAT,         VK_FORMAT_R32G32B32_SFLOAT,              1,    0,    0) \
        _Func(R32G32B32_UINT,          VK_FORMAT_R32G32B32_UINT,                1,    0,    0) \
        _Func(R32G32B32_SINT,          VK_FORMAT_R32G32B32_SINT,                1,    0,    0) \
        _Func(R16G16B16A16_FLOAT,      VK_FORMAT_R16G16B16A16_SFLOAT,           1,    0,    0) \
        _Func(R16G16B16A16_UNORM,      VK_FORMAT_R16G16B16A16_UNORM,            1,    0,    0) \
        _Func(R16G16B16A16_UINT,       VK_FORMAT_R16G16B16A16_UINT,             1,    0,    0) \
        _Func(R16G16B16A16_SNORM,      VK_FORMAT_R16G16B16A16_SNORM,            1,    0,    0) \
        _Func(R16G16B16A16_SINT,       VK_FORMAT_R16G16B16A16_SINT,             1,    0,    0) \
        _Func(R32G32_FLOAT,            VK_FORMAT_R32G32_SFLOAT,                 1,    0,    0) \
        _Func(R32G32_UINT,             VK_FORMAT_R32G32_UINT,                   1,    0,    0) \
        _Func(R32G32_SINT,             VK_FORMAT_R32G32_SINT,                   1,    0,    0) \
        _Func(D32_FLOAT_S8X24_UINT,    VK_FORMAT_D32_SFLOAT_S8_UINT,            0,    1,    1) \
        _Func(R10G10B10A2_UNORM,       VK_FORMAT_A2R10G10B10_UNORM_PACK32,      1,    0,    0) \
        _Func(R10G10B10A2_UINT,        VK_FORMAT_A2R10G10B10_UINT_PACK32,       1,    0,    0) \
        _Func(R11G11B10_FLOAT,         VK_FORMAT_B10G11R11_UFLOAT_PACK32,       1,    0,    0) \
        _Func(R8G8B8A8_UNORM,          VK_FORMAT_R8G8B8A8_UNORM,                1,    0,    0) \
        _Func(R10G10B10_XR_BIAS_A2_UNORM, VK_FORMAT_A2B10G10R10_UNORM_PACK32,   1,    0,    0) \
        _Func(R8G8B8A8_UNORM_SRGB,     VK_FORMAT_R8G8B8A8_SRGB,                 1,    0,    0) \
        _Func(R8G8B8A8_UINT,           VK_FORMAT_R8G8B8A8_UINT,                 1,    0,    0) \
        _Func(R8G8B8A8_SNORM,          VK_FORMAT_R8G8B8A8_SNORM,                1,    0,    0) \
        _Func(R8G8B8A8_SINT,           VK_FORMAT_R8G8B8A8_SINT,                 1,    0,    0) \
        _Func(A8B8G8R8_UNORM,          VK_FORMAT_A8B8G8R8_UNORM_PACK32,         1,    0,    0) \
        _Func(A8B8G8R8_SNORM,          VK_FORMAT_A8B8G8R8_SNORM_PACK32,         1,    0,    0) \
        _Func(A8B8G8R8_UNORM_SRGB,     VK_FORMAT_A8B8G8R8_SRGB_PACK32,          1,    0,    0) \
        _Func(R16G16_FLOAT,            VK_FORMAT_R16G16_SFLOAT,                 1,    0,    0) \
        _Func(R16G16_UNORM,            VK_FORMAT_R16G16_UNORM,                  1,    0,    0) \
        _Func(R16G16_UINT,             VK_FORMAT_R16G16_UINT,                   1,    0,    0) \
        _Func(R16G16_SNORM,            VK_FORMAT_R16G16_SNORM,                  1,    0,    0) \
        _Func(R16G16_SINT,             VK_FORMAT_R16G16_SINT,                   1,    0,    0) \
        _Func(D32_FLOAT,               VK_FORMAT_D32_SFLOAT,                    0,    1,    0) \
        _Func(R32_FLOAT,               VK_FORMAT_R32_SFLOAT,                    1,    0,    0) \
        _Func(R32_UINT,                VK_FORMAT_R32_UINT,                      1,    0,    0) \
        _Func(R32_SINT,                VK_FORMAT_R32_SINT,                      1,    0,    0) \
        _Func(D24_UNORM_S8_UINT,       VK_FORMAT_D24_UNORM_S8_UINT,             0,    1,    1) \
        _Func(R8G8_UNORM,              VK_FORMAT_R8G8_UNORM,                    1,    0,    0) \
        _Func(R8G8_UINT,               VK_FORMAT_R8G8_UINT,                     1,    0,    0) \
        _Func(R8G8_SNORM,              VK_FORMAT_R8G8_SNORM,                    1,    0,    0) \
        _Func(R8G8_SINT,               VK_FORMAT_R8G8_SINT,                     1,    0,    0) \
        _Func(R16_FLOAT,               VK_FORMAT_R16_SFLOAT,                    1,    0,    0) \
        _Func(D16_UNORM,               VK_FORMAT_D16_UNORM,                     0,    1,    0) \
        _Func(R16_UNORM,               VK_FORMAT_R16_UNORM,                     1,    0,    0) \
        _Func(R16_UINT,                VK_FORMAT_R16_UINT,                      1,    0,    0) \
        _Func(R16_SNORM,               VK_FORMAT_R16_SNORM,                     1,    0,    0) \
        _Func(R16_SINT,                VK_FORMAT_R16_SINT,                      1,    0,    0) \
        _Func(R8_UNORM,                VK_FORMAT_R8_UNORM,                      1,    0,    0) \
        _Func(R8_UINT,                 VK_FORMAT_R8_UINT,                       1,    0,    0) \
        _Func(R8_SNORM,                VK_FORMAT_R8_SNORM,                      1,    0,    0) \
        _Func(R8_SINT,                 VK_FORMAT_R8_SINT,                       1,    0,    0) \
        _Func(R9G9B9E5_SHAREDEXP,      VK_FORMAT_E5B9G9R9_UFLOAT_PACK32,        1,    0,    0) \
        _Func(R8G8_B8G8_UNORM,         VK_FORMAT_G8B8G8R8_422_UNORM,            1,    0,    0) \
        _Func(G8R8_G8B8_UNORM,         VK_FORMAT_B8G8R8G8_422_UNORM,            1,    0,    0) \
        _Func(BC1_UNORM,               VK_FORMAT_BC1_RGBA_UNORM_BLOCK,          1,    0,    0) \
        _Func(BC1_UNORM_SRGB,          VK_FORMAT_BC1_RGBA_SRGB_BLOCK,           1,    0,    0) \
        _Func(BC2_UNORM,               VK_FORMAT_BC2_UNORM_BLOCK,               1,    0,    0) \
        _Func(BC2_UNORM_SRGB,          VK_FORMAT_BC2_SRGB_BLOCK,                1,    0,    0) \
        _Func(BC3_UNORM,               VK_FORMAT_BC3_UNORM_BLOCK,               1,    0,    0) \
        _Func(BC3_UNORM_SRGB,          VK_FORMAT_BC3_SRGB_BLOCK,                1,    0,    0) \
        _Func(BC4_UNORM,               VK_FORMAT_BC4_UNORM_BLOCK,               1,    0,    0) \
        _Func(BC4_SNORM,               VK_FORMAT_BC4_SNORM_BLOCK,               1,    0,    0) \
        _Func(BC5_UNORM,               VK_FORMAT_BC5_UNORM_BLOCK,               1,    0,    0) \
        _Func(BC5_SNORM,               VK_FORMAT_BC5_SNORM_BLOCK,               1,    0,    0) \
        _Func(R5G6B5_UNORM,            VK_FORMAT_R5G6B5_UNORM_PACK16,           1,    0,    0) \
        _Func(B5G6R5_UNORM,            VK_FORMAT_B5G6R5_UNORM_PACK16,           1,    0,    0) \
        _Func(B5G5R5A1_UNORM,          VK_FORMAT_A1R5G5B5_UNORM_PACK16,         1,    0,    0) \
        _Func(B8G8R8A8_UNORM,          VK_FORMAT_B8G8R8A8_UNORM,                1,    0,    0) \
        _Func(B8G8R8A8_SNORM,          VK_FORMAT_B8G8R8A8_SNORM,                1,    0,    0) \
        _Func(B8G8R8A8_UNORM_SRGB,     VK_FORMAT_B8G8R8A8_SRGB,                 1,    0,    0) \
        _Func(BC6H_UF16,               VK_FORMAT_BC6H_UFLOAT_BLOCK,             1,    0,    0) \
        _Func(BC6H_SF16,               VK_FORMAT_BC6H_SFLOAT_BLOCK,             1,    0,    0) \
        _Func(BC7_UNORM,               VK_FORMAT_BC7_UNORM_BLOCK,               1,    0,    0) \
        _Func(BC7_UNORM_SRGB,          VK_FORMAT_BC7_SRGB_BLOCK,                1,    0,    0) \
        _Func(NV12,                    VK_FORMAT_G8_B8R8_2PLANE_420_UNORM,      1,    0,    0) \
        _Func(P010,                    VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16, 1, 0, 0) \
        _Func(P016,                    VK_FORMAT_G16_B16R16_2PLANE_420_UNORM,   1,    0,    0) \
        _Func(B4G4R4A4_UNORM,          VK_FORMAT_B4G4R4A4_UNORM_PACK16,         1,    0,    0) \
        _Func(R4G4B4A4_UNORM,          VK_FORMAT_R4G4B4A4_UNORM_PACK16,         1,    0,    0) \
        _Func(D16_UNORM_S8_UINT,       VK_FORMAT_D16_UNORM_S8_UINT,             0,    1,    1) \
        _Func(EAC_R11_UNORM,           VK_FORMAT_EAC_R11_UNORM_BLOCK,           1,    0,    0) \
        _Func(EAC_R11_SNORM,           VK_FORMAT_EAC_R11_SNORM_BLOCK,           1,    0,    0) \
        _Func(EAC_RG11_UNORM,          VK_FORMAT_EAC_R11G11_UNORM_BLOCK,        1,    0,    0) \
        _Func(EAC_RG11_SNORM,          VK_FORMAT_EAC_R11G11_SNORM_BLOCK,        1,    0,    0) \
        _Func(ETC2_UNORM,              VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK,       1,    0,    0) \
        _Func(ETC2_UNORM_SRGB,         VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK,        1,    0,    0) \
        _Func(ETC2A1_UNORM,            VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK,     1,    0,    0) \
        _Func(ETC2A1_UNORM_SRGB,       VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK,      1,    0,    0) \
        _Func(ETC2A_UNORM,             VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK,     1,    0,    0) \
        _Func(ETC2A_UNORM_SRGB,        VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK,      1,    0,    0) \
        _Func(PVRTC2_UNORM,            VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG,   1,    0,    0) \
        _Func(PVRTC2_UNORM_SRGB,       VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG,    1,    0,    0) \
        _Func(PVRTC4_UNORM,            VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG,   1,    0,    0) \
        _Func(PVRTC4_UNORM_SRGB,       VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG,    1,    0,    0) \
        _Func(ASTC_4x4_UNORM,          VK_FORMAT_ASTC_4x4_UNORM_BLOCK,          1,    0,    0) \
        _Func(ASTC_4x4_UNORM_SRGB,     VK_FORMAT_ASTC_4x4_SRGB_BLOCK,           1,    0,    0) \
        _Func(ASTC_5x4_UNORM,          VK_FORMAT_ASTC_5x4_UNORM_BLOCK,          1,    0,    0) \
        _Func(ASTC_5x4_UNORM_SRGB,     VK_FORMAT_ASTC_5x4_SRGB_BLOCK,           1,    0,    0) \
        _Func(ASTC_5x5_UNORM,          VK_FORMAT_ASTC_5x5_UNORM_BLOCK,          1,    0,    0) \
        _Func(ASTC_5x5_UNORM_SRGB,     VK_FORMAT_ASTC_5x5_SRGB_BLOCK,           1,    0,    0) \
        _Func(ASTC_6x5_UNORM,          VK_FORMAT_ASTC_6x5_UNORM_BLOCK,          1,    0,    0) \
        _Func(ASTC_6x5_UNORM_SRGB,     VK_FORMAT_ASTC_6x5_SRGB_BLOCK,           1,    0,    0) \
        _Func(ASTC_6x6_UNORM,          VK_FORMAT_ASTC_6x6_UNORM_BLOCK,          1,    0,    0) \
        _Func(ASTC_6x6_UNORM_SRGB,     VK_FORMAT_ASTC_6x6_SRGB_BLOCK,           1,    0,    0) \
        _Func(ASTC_8x5_UNORM,          VK_FORMAT_ASTC_8x5_UNORM_BLOCK,          1,    0,    0) \
        _Func(ASTC_8x5_UNORM_SRGB,     VK_FORMAT_ASTC_8x5_SRGB_BLOCK,           1,    0,    0) \
        _Func(ASTC_8x6_UNORM,          VK_FORMAT_ASTC_8x6_UNORM_BLOCK,          1,    0,    0) \
        _Func(ASTC_8x6_UNORM_SRGB,     VK_FORMAT_ASTC_8x6_SRGB_BLOCK,           1,    0,    0) \
        _Func(ASTC_8x8_UNORM,          VK_FORMAT_ASTC_8x8_UNORM_BLOCK,          1,    0,    0) \
        _Func(ASTC_8x8_UNORM_SRGB,     VK_FORMAT_ASTC_8x8_SRGB_BLOCK,           1,    0,    0) \
        _Func(ASTC_10x5_UNORM,         VK_FORMAT_ASTC_10x5_UNORM_BLOCK,         1,    0,    0) \
        _Func(ASTC_10x5_UNORM_SRGB,    VK_FORMAT_ASTC_10x5_SRGB_BLOCK,          1,    0,    0) \
        _Func(ASTC_10x6_UNORM,         VK_FORMAT_ASTC_10x6_UNORM_BLOCK,         1,    0,    0) \
        _Func(ASTC_10x6_UNORM_SRGB,    VK_FORMAT_ASTC_10x6_SRGB_BLOCK,          1,    0,    0) \
        _Func(ASTC_10x8_UNORM,         VK_FORMAT_ASTC_10x8_UNORM_BLOCK,         1,    0,    0) \
        _Func(ASTC_10x8_UNORM_SRGB,    VK_FORMAT_ASTC_10x8_SRGB_BLOCK,          1,    0,    0) \
        _Func(ASTC_10x10_UNORM,        VK_FORMAT_ASTC_10x10_UNORM_BLOCK,        1,    0,    0) \
        _Func(ASTC_10x10_UNORM_SRGB,   VK_FORMAT_ASTC_10x10_SRGB_BLOCK,         1,    0,    0) \
        _Func(ASTC_12x10_UNORM,        VK_FORMAT_ASTC_12x10_UNORM_BLOCK,        1,    0,    0) \
        _Func(ASTC_12x10_UNORM_SRGB,   VK_FORMAT_ASTC_12x10_SRGB_BLOCK,         1,    0,    0) \
        _Func(ASTC_12x12_UNORM,        VK_FORMAT_ASTC_12x12_UNORM_BLOCK,        1,    0,    0) \
        _Func(ASTC_12x12_UNORM_SRGB,   VK_FORMAT_ASTC_12x12_SRGB_BLOCK,         1,    0,    0)
