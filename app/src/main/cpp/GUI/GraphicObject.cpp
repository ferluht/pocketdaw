#include "GraphicObject.h"
#include "GraphicEngine.h"

GraphicObject::GraphicObject(float x_, float y_, float z_, float height_, float width_, float angle_,
                             const char * texture, const char * vshader, const char * fshader, bool saveRatio_):
                             BBox(x_, y_, z_, height_, width_, angle_)
{
    this->parent = nullptr;
    this->texture = 0;
    this->vshader = vshader;
    this->fshader = fshader;
    this->texture_name = texture;
    this->visible = true;
    this->saveRatio = saveRatio_;
    this->focusObject = nullptr;
    this->initialized = false;
}

GraphicObject::~GraphicObject() { unload(); }

void GraphicObject::init_() {
    shader = Shader::CreateShaderProgram(vshader, fshader);
    if (texture_name != nullptr) texture = ndk_helper::texture::loadBMP(texture_name);

    init();

    glGenBuffers(1, &vbo_);
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &ibo_);

    for (auto const &gr : Graphics) {
        gr->init_();
    }

    initialized = true;
}

void GraphicObject::draw_(){

    if (parent){
        globalPosition.x = parent->globalPosition.x + x * parent->globalPosition.width;
        globalPosition.y = parent->globalPosition.y + y * parent->globalPosition.height;
        globalPosition.z = parent->globalPosition.z + 1;
        globalPosition.height = height * parent->globalPosition.height;
        globalPosition.width = width * parent->globalPosition.width;
        if (saveRatio) {
            globalPosition.ratio = ratio;
        } else {
            globalPosition.ratio = globalPosition.width / globalPosition.height;
        }
    } else {
        globalPosition.x = x * GraphicEngine::screen_width;
        globalPosition.y = y * GraphicEngine::screen_height;
        globalPosition.z = 0;
        globalPosition.width = width * GraphicEngine::screen_width;
        globalPosition.height = height * GraphicEngine::screen_height;
        globalPosition.ratio = globalPosition.width/globalPosition.height;
    }

    draw();

    changed = false;

    for (auto const &gr : Graphics) {
        gr->draw_();
    }
}

void GraphicObject::grender_(float dTime) {

    if (!initialized) init_();
    if (focusObject && !focusObject->initialized) focusObject->init_();

    if (visible) {

        if (changed) {
            draw_();
        }

        GraphicEngine &eng = GraphicEngine::GetGraphicEngine();

        ndk_helper::Mat4 mat_vp = eng.mat_projection_ * eng.mat_view_;

        glUseProgram(shader.program_);

        glUniformMatrix4fv(shader.param_view_, 1, GL_FALSE, mat_vp.Ptr());

        GLfloat rot[] = {cos(angle), -sin(angle), sin(angle), cos(angle)};

        glUniformMatrix2fv(shader.param_texture_angle_, 1, GL_FALSE, rot);

        glUniform1f(shader.param_z_, globalPosition.z);

        if (texture) glBindTexture(GL_TEXTURE_2D, texture);

        grender(dTime);

        for (auto const &gr : Graphics) {
            gr->grender_(dTime);
        }
    }

    if (focusObject) {
        focusObject->grender_(dTime);
    }
}

void GraphicObject::setVisible(bool visible_) {
    visible = visible_;
    changed = true;
    for (auto const &gr : Graphics) {
        gr->setVisible(visible_);
    }
}

void GraphicObject::attach(GraphicObject *go) {
    go->attachTo(this);
    Graphics.push_back(go);
    //draw_();
}

void GraphicObject::detach(GraphicObject *go) {
    Graphics.remove(go);
    go->unload();
    //draw_();
}

void GraphicObject::unload() {
    if (vbo_) {
        glDeleteBuffers(1, &vbo_);
        vbo_ = 0;
    }

    if (ibo_) {
        glDeleteBuffers(1, &ibo_);
        ibo_ = 0;
    }

    if (shader.program_) {
        glDeleteProgram(shader.program_);
        shader.program_ = 0;
    }
}