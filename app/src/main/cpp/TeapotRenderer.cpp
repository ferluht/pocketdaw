/*
 * Copyright 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//--------------------------------------------------------------------------------
// TeapotRenderer.cpp
// Render a teapot
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// Include files
//--------------------------------------------------------------------------------
#include "TeapotRenderer.h"

//--------------------------------------------------------------------------------
// Teapot model data
//--------------------------------------------------------------------------------
#include "teapot.inl"

//#include <SOIL.h>

//--------------------------------------------------------------------------------
// Ctor
//--------------------------------------------------------------------------------
TeapotRenderer::TeapotRenderer() {}

//--------------------------------------------------------------------------------
// Dtor
//--------------------------------------------------------------------------------
TeapotRenderer::~TeapotRenderer() { Unload(); }

void TeapotRenderer::Init() {
    // Settings
    glFrontFace(GL_CCW);

    // Load shader
    LoadShaders(&shader_param_, "Shaders/VS_ShaderPlain.vsh",
              "Shaders/ShaderPlain.fsh");


    // Массив 3 векторов, которые являются вершинами треугольника
    GLfloat g_vertex_buffer_data[] = {
            // Позиции          // Цвета             // Текстурные координаты
            1.5f,  1.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Верхний правый
            1.5f, -1.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Нижний правый
            -1.5f, -1.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Нижний левый
            -1.5f,  1.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Верхний левый
    };

    GLuint indices[] = {  // Помните, что мы начинаем с 0!
            0, 1, 2,   // Первый треугольник
            3, 2, 3    // Второй треугольник
    };

    glGenBuffers(1, &vbo_);
    glGenVertexArrays(1, &vao_square_);
    glGenBuffers(1, &ibo_);



    glBindVertexArray(vao_square_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT,GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);


    texture = loadBMP_custom("Textures/container.bmp");


    UpdateViewport();
    mat_model_ = ndk_helper::Mat4::Translation(0, 0, 0);
}

GLuint TeapotRenderer::loadBMP_custom(const char *imagepath)
{

    // Open your file
    AAsset* file = AAssetManager_open(app->activity->assetManager, imagepath, AASSET_MODE_BUFFER);
// Get the file length

// Do whatever you want with the content of the file

// Free the memoery you allocated earlie

    // Данные, прочитанные из заголовка BMP-файла
    unsigned char header[54]; // Каждый BMP-файл начинается с заголовка, длиной в 54 байта
    unsigned int dataPos;     // Смещение данных в файле (позиция данных)
    unsigned int width, height;
    unsigned int imageSize;   // Размер изображения = Ширина * Высота * 3
// RGB-данные, полученные из файла

    if (!file) {
        printf("Изображение не может быть открытоn");
        return 0;
    }

    size_t fileLength = AAsset_getLength(file);

    if (  AAsset_read(file, header, 54) != 54 ) { // Если мы прочитали меньше 54 байт, значит возникла проблема
        printf("Некорректный BMP-файлn");
        return 0;
    }

    if ( header[0]!='B' || header[1]!='M' ){
        printf("Некорректный BMP-файлn");
        return 0;
    }

    // Читаем необходимые данные
    dataPos    = *(int*)&(header[0x0A]); // Смещение данных изображения в файле
    imageSize  = *(int*)&(header[0x22]); // Размер изображения в байтах
    width      = *(int*)&(header[0x12]); // Ширина
    height     = *(int*)&(header[0x16]); // Высота

    // Некоторые BMP-файлы имеют нулевые поля imageSize и dataPos, поэтому исправим их
    if (imageSize==0)    imageSize=width*height*3; // Ширину * Высоту * 3, где 3 - 3 компоненты цвета (RGB)
    if (dataPos==0)      dataPos=54; // В таком случае, данные будут следовать сразу за заголовком

    // Создаем буфер
    unsigned char * data = new unsigned char [imageSize];

// Считываем данные из файла в буфер
    AAsset_read(file, data, fileLength);

// Закрываем файл, так как больше он нам не нужен
    AAsset_close(file);

    // Создадим одну текстуру OpenGL
    GLuint textureID;
    glGenTextures(1, &textureID);

// Сделаем созданную текстуру текущий, таким образом все следующие функции будут работать именно с этой текстурой
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

// Передадим изображение OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    delete [] data;

    return textureID;
}

void TeapotRenderer::UpdateViewport() {
      // Init Projection matrices
      int32_t viewport[4];
      glGetIntegerv(GL_VIEWPORT, viewport);

      mat_projection_ = ndk_helper::Mat4::Ortho2D(-viewport[2]/100, -viewport[3]/100, viewport[2]/100, viewport[3]/100);

//      const float CAM_NEAR = 5.f;
//      const float CAM_FAR = 10000.f;
//      if (viewport[2] < viewport[3]) {
//            float aspect =
//                static_cast<float>(viewport[2]) / static_cast<float>(viewport[3]);
//            mat_projection_ =
//                ndk_helper::Mat4::Perspective(aspect, 1.0f, CAM_NEAR, CAM_FAR);
//      } else {
//            float aspect =
//                static_cast<float>(viewport[3]) / static_cast<float>(viewport[2]);
//            mat_projection_ =
//                ndk_helper::Mat4::Perspective(1.0f, aspect, CAM_NEAR, CAM_FAR);
//      }
}

void TeapotRenderer::Unload() {
      if (vbo_) {
            glDeleteBuffers(1, &vbo_);
            vbo_ = 0;
      }

      if (ibo_) {
            glDeleteBuffers(1, &ibo_);
            ibo_ = 0;
      }

      if (shader_param_.program_) {
            glDeleteProgram(shader_param_.program_);
            shader_param_.program_ = 0;
      }
}

void TeapotRenderer::Update(float fTime) {
      const float CAM_X = 0.f;
      const float CAM_Y = 0.f;
      const float CAM_Z = 100.f;

      mat_view_ = ndk_helper::Mat4::LookAt(ndk_helper::Vec3(CAM_X, CAM_Y, CAM_Z),
                                           ndk_helper::Vec3(0.f, 0.f, 0.f),
                                           ndk_helper::Vec3(0.f, 1.f, 0.f));

      if (camera_) {
            camera_->Update();
            mat_view_ = camera_->GetTransformMatrix() * mat_view_;// *
//                        camera_->GetRotationMatrix() * mat_model_;
      } else {
            mat_view_ = mat_view_ * mat_model_;
      }
}

void TeapotRenderer::Render(std::atomic<float> * wave, int r) {
      //
      // Feed Projection and Model View matrices to the shaders
      ndk_helper::Mat4 mat_vp = mat_projection_ * mat_view_;


    glUseProgram(shader_param_.program_);

//      TEAPOT_MATERIALS material = {
//          {1.0f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 10.f}, {0.1f, 0.1f, 0.1f}, };
//
//      // Update uniforms
//      glUniform4f(shader_param_.material_diffuse_, material.diffuse_color[0],
//                  material.diffuse_color[1], material.diffuse_color[2], 1.f);
//
//      glUniform4f(shader_param_.material_specular_, material.specular_color[0],
//                  material.specular_color[1], material.specular_color[2],
//                  material.specular_color[3]);
      //
      // using glUniform3fv here was troublesome
      //
//      glUniform3f(shader_param_.material_ambient_, material.ambient_color[0],
//                  material.ambient_color[1], material.ambient_color[2]);

      glUniformMatrix4fv(shader_param_.matrix_projection_, 1, GL_FALSE,
                         mat_vp.Ptr());
//      glUniformMatrix4fv(shader_param_.matrix_view_, 1, GL_FALSE, mat_view_.Ptr());
//      glUniform3f(shader_param_.light0_, 0.f, 0.f, -600.f);
//
//      glDrawElements(GL_TRIANGLES, num_indices_, GL_UNSIGNED_SHORT,
//                     BUFFER_OFFSET(0));
//
//      glBindBuffer(GL_ARRAY_BUFFER, 0);
//      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

//
//      static GLfloat g_vertex_buffer_data[6000];
//
//      for (int i = 0; i < 2000 - 1; i++) {
//            g_vertex_buffer_data[r*3] = wave[r]*15;
//            g_vertex_buffer_data[r*3+1] = (float)i/100.f - 10;
//            g_vertex_buffer_data[r*3+2] = 0;
//            r++;
//            if(r>=2000) r = 0;
//      };
//
//      // Generate 1 buffer, put the resulting identifier in vertexbuffer
//      glGenBuffers(1, &vertexbuffer);
//      // The following commands will talk about our 'vertexbuffer' buffer
//      glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//      // Give our vertices to OpenGL.
//      glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);
//
//
//      // 1st attribute buffer : vertices
//      glEnableVertexAttribArray(0);
//      glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//      glVertexAttribPointer(
//            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//            3,                  // size
//            GL_FLOAT,           // type
//            GL_FALSE,           // normalized?
//            0,                  // stride
//            (void*)0            // array buffer offset
//      );
//      // Draw the triangle !
//      //glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
////      for(int i = 0; i < 2000 - 1; i++) {
//          glDrawArrays(GL_LINE_STRIP, 100, 1900);
////      }

//    glDisableVertexAttribArray(0);


    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(vao_square_);


    glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, 0);

      //glDrawArrays(GL_LINES, 0, 6000 - 3);

//      g_vertex_buffer_data[6] = 5;

//      for(int i = 0; i < 3; i++) {
//          glDrawArrays(GL_LINES, i, 2);
//      }
    glBindVertexArray(0);
}

bool TeapotRenderer::LoadShaders(SHADER_PARAMS* params, const char* strVsh,
                                 const char* strFsh) {
      GLuint program;
      GLuint vert_shader, frag_shader;

      // Create shader program
      program = glCreateProgram();
      LOGI("Created Shader %d", program);

      // Create and compile vertex shader
      if (!ndk_helper::shader::CompileShader(&vert_shader, GL_VERTEX_SHADER,
                                             strVsh)) {
            LOGI("Failed to compile vertex shader");
            glDeleteProgram(program);
            return false;
      }

      // Create and compile fragment shader
      if (!ndk_helper::shader::CompileShader(&frag_shader, GL_FRAGMENT_SHADER,
                                             strFsh)) {
            LOGI("Failed to compile fragment shader");
            glDeleteProgram(program);
            return false;
      }

      // Attach vertex shader to program
      glAttachShader(program, vert_shader);

      // Attach fragment shader to program
      glAttachShader(program, frag_shader);


      // Bind attribute locations
      // this needs to be done prior to linking
//      glBindAttribLocation(program, ATTRIB_VERTEX, "position");
//      glBindAttribLocation(program, ATTRIB_NORMAL, "myNormal");
//      glBindAttribLocation(program, ATTRIB_UV, "myUV");

      // Link program
      if (!ndk_helper::shader::LinkProgram(program)) {
            LOGI("Failed to link program: %d", program);

            if (vert_shader) {
                 glDeleteShader(vert_shader);
                vert_shader = 0;
            }
            if (frag_shader) {
                glDeleteShader(frag_shader);
                frag_shader = 0;
            }
            if (program) {
                glDeleteProgram(program);
            }

            return false;
      }

      // Get uniform locations
      params->matrix_projection_ = glGetUniformLocation(program, "uPMatrix");
//      params->matrix_view_ = glGetUniformLocation(program, "uMVMatrix");

//      params->light0_ = glGetUniformLocation(program, "vLight0");
//      params->material_diffuse_ = glGetUniformLocation(program, "vMaterialDiffuse");
//      params->material_ambient_ = glGetUniformLocation(program, "vMaterialAmbient");
//      params->material_specular_ =
//          glGetUniformLocation(program, "vMaterialSpecular");

      // Release vertex and fragment shaders
      if (vert_shader) glDeleteShader(vert_shader);
      if (frag_shader) glDeleteShader(frag_shader);

      params->program_ = program;
      return true;
}

bool TeapotRenderer::Bind(ndk_helper::TapCamera* camera) {
      camera_ = camera;
      return true;
}
