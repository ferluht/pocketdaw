#version 320 es

in mediump vec2 TexCoord;

out mediump vec4 color;

uniform mediump mat2 angle;
uniform sampler2D ourTexture;

void main()
{
    mediump vec2 new_coord = angle*(TexCoord - vec2(0.5, 0.5));
    color = texture(ourTexture, new_coord + vec2(0.5, 0.5));
    if ((new_coord[0]*new_coord[0] + new_coord[1]*new_coord[1]) > 0.25) color.a = 0.0;
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
//  ShaderPlain.fsh
//

//void main()
//{
//    gl_FragColor = vec4( 0, 0, 0, 1 );
//}