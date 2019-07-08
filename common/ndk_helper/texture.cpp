//
// Created by Admin on 06.07.2019.
//

#include <cstdio>
#include <GLES2/gl2.h>
#include "texture.h"
#include "JNIHelper.h"

namespace ndk_helper {

    GLuint texture::loadBMP(const char *imagepath) {

        std::vector<uint8_t> data;
        bool b = JNIHelper::GetInstance()->ReadFile(imagepath, &data);
        if (!b) {
            LOGI("Can not open a file:%s", imagepath);
            return 0;
        }

        unsigned int dataPos;     // Смещение данных в файле (позиция данных)
        unsigned int width, height;
        unsigned int imageSize;   // Размер изображения = Ширина * Высота * 3
        // RGB-данные, полученные из файла

        if (data[0] != 'B' || data[1] != 'M') {
            printf("Некорректный BMP-файлn");
            return 0;
        }

        // Читаем необходимые данные
        dataPos = *(unsigned int *) &(data[0x0A]); // Смещение данных изображения в файле
        imageSize = *(unsigned int *) &(data[0x22]); // Размер изображения в байтах
        width = *(unsigned int *) &(data[0x12]); // Ширина
        height = *(unsigned int *) &(data[0x16]); // Высота

        // Некоторые BMP-файлы имеют нулевые поля imageSize и dataPos, поэтому исправим их
        if (imageSize == 0)
            imageSize = width * height * 3; // Ширину * Высоту * 3, где 3 - 3 компоненты цвета (RGB)

        // Создаем буфер

        static unsigned char header_length = 54;

        auto image_data = new unsigned char[data.size() - header_length];

        for (int i = 0; i < imageSize / 3; i++) {
            image_data[i*3] = data[i*3 + 2 + header_length];
            image_data[i*3 + 1] = data[i*3 + 1 + header_length];
            image_data[i*3 + 2] = data[i*3 + header_length];
        }

        // Создадим одну текстуру OpenGL
        GLuint textureID;
        glGenTextures(1, &textureID);

        // Сделаем созданную текстуру текущий, таким образом все следующие функции будут работать именно с этой текстурой
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // Передадим изображение OpenGL
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);

        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);

        delete[] image_data;
        return textureID;
    }
}