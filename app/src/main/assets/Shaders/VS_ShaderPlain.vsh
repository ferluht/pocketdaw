#version 320 es

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out vec2 TexCoord;

uniform mat4 uPMatrix;

void main()
{
    gl_Position = uPMatrix * vec4(position, 1.0f);
    TexCoord = texCoord;
}


//
// Copyright (C) 2015 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//  ShaderPlain.vsh
//
//attribute highp vec3    myVertex;
//varying lowp    vec4    colorDiffuse;
//
//uniform highp mat4      uPMatrix;
//
//uniform lowp vec3       vMaterialAmbient;
//
//void main(void)
//{
//    highp vec4 p = vec4(myVertex,1);
//    gl_Position = uPMatrix * p;
//
//    colorDiffuse = vec4( vMaterialAmbient, 1 );
//}