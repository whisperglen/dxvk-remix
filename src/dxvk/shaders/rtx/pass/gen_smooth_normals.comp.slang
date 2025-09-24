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

#include "gen_smooth_normals_indices.h"
#include "gen_smooth_normals.h"

layout(binding = GEN_SMOOTH_NORMALS_BINDING_INDEX_INPUT) 
StructuredBuffer<uint16_t> indices;

layout(binding = GEN_SMOOTH_NORMALS_BINDING_POSITION_INPUT) 
StructuredBuffer<float> positions;

layout(binding = GEN_SMOOTH_NORMALS_BINDING_NORMAL_OUTPUT)
RWStructuredBuffer<float> normals;

layout(binding = GEN_SMOOTH_NORMALS_BINDING_POSREMAP_INPUT) 
StructuredBuffer<uint16_t> posRemap;

layout(push_constant)
ConstantBuffer<GenSmoothNormalsArgs> cb;


[shader("compute")]
[numthreads(128, 1, 1)]
void main(uint faceID : SV_DispatchThreadID)
{
  if (faceID >= cb.primCount)
  {
    return;
  }

  generateSmoothNormals(faceID, indices, positions, normals, posRemap, cb);
}