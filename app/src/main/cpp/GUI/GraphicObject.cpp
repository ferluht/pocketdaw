#include "GraphicObject.h"

GraphicObject::GraphicObject() :
GraphicObject("default.vsh", "default.fsh") {}

GraphicObject::GraphicObject(const char *texture) :
        GraphicObject(texture, "default.vsh", "default.fsh") {}

GraphicObject::GraphicObject(
        const char *vshader,
        const char *fshader) :
        GraphicObject(nullptr, vshader, fshader) {}

GraphicObject::GraphicObject(const char *texture, const char *vshader, const char *fshader)
{
    this->parent = nullptr;
    this->texture = nullptr;
    this->vshader = vshader;
    this->fshader = fshader;
    this->texture_name = texture;
}
GraphicObject::~GraphicObject() { unload(); }

void GraphicObject::init_() {
    shader = GraphicEngine::CreateShaderProgram(vshader, fshader);
    if (texture_name != nullptr) texture = ndk_helper::texture::loadBMP(texture_name);
    init();
    draw();

    for (auto const &gr : Graphics) {
        gr->init_();
    }
}

void GraphicObject::draw_(){

    draw();

    for (auto const &gr : Graphics) {
        gr->draw_();
    }
}

void GraphicObject::grender_(float dTime) {

    GraphicEngine& eng = GraphicEngine::GetGraphicEngine();

    ndk_helper::Mat4 mat_vp = eng.mat_projection_ * eng.mat_view_;

    glUseProgram(shader.program_);

    glUniformMatrix4fv(shader.param_view_, 1, GL_FALSE, mat_vp.Ptr());

    GLfloat rot[] = {cos(angle), -sin(angle), sin(angle), cos(angle)};

    glUniformMatrix2fv(shader.param_texture_angle_, 1, GL_FALSE, rot);

    if (texture) glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(vao_);

    grender(dTime);

    glBindVertexArray(0);

    for (auto const &gr : Graphics) {
        gr->grender_(dTime);
    }
}

void GraphicObject::attach(GraphicObject *go) {
    go->attachTo(this);
    Graphics.push_back(go);
    draw_();
}

void GraphicObject::detach(GraphicObject *go) {
    Graphics.remove(go);
    go->unload();
    draw_();
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