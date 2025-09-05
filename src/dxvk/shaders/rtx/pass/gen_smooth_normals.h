/*
* Copyright (c) 2021-2023, NVIDIA CORPORATION. All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/
#pragma once

// This function can be executed on the CPU or GPU!!
#ifdef __cplusplus
namespace dxvk {
typedef Vector3 float3;
typedef Vector4 float4;
#define WriteBuffer(T) T*
#define ReadBuffer(T) const T*
#define ConstBuffer(T) const T&
#define InterlockedAddFloat(B,V) (B) += (V)
#else
#define WriteBuffer(T) RWStructuredBuffer<T>
#define ReadBuffer(T) StructuredBuffer<T>
#define ConstBuffer(T) ConstantBuffer<T>
#define inline
#endif

inline float3 getElement(uint idx, uint offset, uint stride, ReadBuffer(float) buff)
{
  const uint baseOffset = (offset + idx * stride) / 4;
  float3 position = float3(buff[baseOffset + 0],
                           buff[baseOffset + 1],
                           buff[baseOffset + 2]);
}

void generateSmoothNormals(const uint32_t faceID, ReadBuffer(uint16_t) indices, ReadBuffer(float) positions, WriteBuffer(float) normals, ConstBuffer(GenSmoothNormalsArgs) cb)
{
  uint i0 = indices[faceID * 3 + 0];
  uint i1 = indices[faceID * 3 + 1];
  uint i2 = indices[faceID * 3 + 2];

  float3 v0 = getElement(i0,cb.srcPositionOffset,cb.srcPositionStride,positions);
  float3 v1 = getElement(i1,cb.srcPositionOffset,cb.srcPositionStride,positions);
  float3 v2 = getElement(i2,cb.srcPositionOffset,cb.srcPositionStride,positions);

  float3 edge1 = v1 - v0;
  float3 edge2 = v2 - v0;

  float3 faceNormal = normalize(cross(edge1, edge2));

  //vertex 0 normal
  const uint baseDstNormalOffset0 = (cb.dstNormalOffset + i0 * cb.dstNormalStride) / 4;
  InterlockedAddFloat(normals[baseDstNormalOffset0 + 0], faceNormal.x);
  InterlockedAddFloat(normals[baseDstNormalOffset0 + 1], faceNormal.y);
  InterlockedAddFloat(normals[baseDstNormalOffset0 + 2], faceNormal.z);
  //vertex 1 normal
  const uint baseDstNormalOffset1 = (cb.dstNormalOffset + i1 * cb.dstNormalStride) / 4;
  InterlockedAddFloat(normals[baseDstNormalOffset1 + 0], faceNormal.x);
  InterlockedAddFloat(normals[baseDstNormalOffset1 + 1], faceNormal.y);
  InterlockedAddFloat(normals[baseDstNormalOffset1 + 2], faceNormal.z);
  //vertex 2 normal
  const uint baseDstNormalOffset2 = (cb.dstNormalOffset + i2 * cb.dstNormalStride) / 4;
  InterlockedAddFloat(normals[baseDstNormalOffset2 + 0], faceNormal.x);
  InterlockedAddFloat(normals[baseDstNormalOffset2 + 1], faceNormal.y);
  InterlockedAddFloat(normals[baseDstNormalOffset2 + 2], faceNormal.z);
}

#ifdef __cplusplus
#undef WriteBuffer
#undef ReadBuffer
#undef ConstBuffer
#undef InterlockedAddFloat
} //dxvk
#endif