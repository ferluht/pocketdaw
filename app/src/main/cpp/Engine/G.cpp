#include "G.h"

GObject::GObject()
{
    this->parent = nullptr;
    this->texture = 0;
    this->vshader = nullptr;
    this->fshader = nullptr;
    this->texture_name = nullptr;
    this->visible = true;
    this->saveRatio = false;
    this->focusObject = nullptr;
    this->initialized = false;
}

GObject::~GObject() { GUnload(); }

void GObject::GInit_() {

    for (auto const &gr : Graphics) {
        gr->GInit_();
    }

    if (vshader && fshader) shader = Shader::CreateShaderProgram(vshader, fshader);
    if (texture_name) texture = ndk_helper::texture::loadBMP(texture_name);

    glGenBuffers(1, &vbo_);
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &ibo_);

    GInit();

    initialized = true;
}

void GObject::GDraw_(){

    if (!initialized) GInit_();

    if (parent){
        globalPosition.x = parent->globalPosition.x + x * parent->globalPosition.width;
        globalPosition.y = parent->globalPosition.y + y * parent->globalPosition.height;
        globalPosition.z = parent->globalPosition.z + 1;
        if (width) {
            if (height) {
                globalPosition.height = height * parent->globalPosition.height;
                globalPosition.width = width * parent->globalPosition.width;
            } else {
                globalPosition.width = width * parent->globalPosition.width;
                globalPosition.height = globalPosition.width / ratio;
            }
        } else {
            if (height) {
                globalPosition.height = height * parent->globalPosition.height;
                globalPosition.width = globalPosition.height * ratio;
            } else {
                globalPosition.height = parent->globalPosition.height;
                globalPosition.width = parent->globalPosition.width;
            }
        }
        globalPosition.ratio = globalPosition.width / globalPosition.height;
    } else {
        globalPosition.x = x * GEngine::screen_width;
        globalPosition.y = y * GEngine::screen_height;
        globalPosition.z = 0;
        globalPosition.width = width * GEngine::screen_width;
        globalPosition.height = height * GEngine::screen_height;
        globalPosition.ratio = globalPosition.width/globalPosition.height;
    }

    GDraw();

    changed = false;

    for (auto const &gr : Graphics) {
        gr->GDraw_();
    }
}

void GObject::GRender_(float dTime) {

    if (focusObject && !focusObject->initialized) focusObject->GInit_();

    if (visible) {

        if (changed) {
            GDraw_();
        }

        GEngine &eng = GEngine::getGEngine();

        ndk_helper::Mat4 mat_vp = eng.mat_projection_ * eng.mat_view_;

        glUseProgram(shader.program_);

        glUniformMatrix4fv(shader.param_view_, 1, GL_FALSE, mat_vp.Ptr());

        GLfloat rot[] = {cos(angle), -sin(angle), sin(angle), cos(angle)};

        glUniformMatrix2fv(shader.param_texture_angle_, 1, GL_FALSE, rot);

        glUniform1f(shader.param_z_, globalPosition.z);

        glUniform4fv(shader.param_color_, 1, color);

        if (texture) glBindTexture(GL_TEXTURE_2D, texture);

        GRender(dTime);

        for (auto const &gr : Graphics) {
            gr->GRender_(dTime);
        }
    }

    if (focusObject) {
        focusObject->GRender_(dTime);
    }
}

void GObject::GAttach(GObject *go) {
    go->GAttachTo(this);
//    go->GSetVisible(this->visible);
    Graphics.push_front(go);
    //draw_();
}

void GObject::GDetach(GObject *go) {
    Graphics.remove(go);
    go->GUnload();
    //draw_();
}

void GObject::GUnload() {
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

float GEngine::screen_width;
float GEngine::screen_height;
float GEngine::screen_ratio;

////-------------------------------------------------------------------------
//// Ctor
////-------------------------------------------------------------------------
//GEngine::GEngine(Master * master_)
//        : initialized_resources_(false),
//          has_focus_(false),
//          app_(nullptr) {
//    gl_context_ = ndk_helper::GLContext::GetInstance();
//    this->master = master_;
//}
//
///**
// * Unload resources
// */
void GEngine::UnloadResources() { }


/**
 * Initialize an EGL context for the current display.
 */
int GEngine::InitDisplay(android_app *app) {
    gl_context_ = ndk_helper::GLContext::GetInstance();
    if (!initialized_resources_) {
        gl_context_->Init(app_->window);
        initialized_resources_ = true;
    } else if (app->window != gl_context_->GetANativeWindow()) {
        // Re-initialize ANativeWindow.
        // On some devices, ANativeWindow is re-created when the app is resumed
        assert(gl_context_->GetANativeWindow());
        UnloadResources();
        gl_context_->Invalidate();
        app_ = app;
        gl_context_->Init(app->window);
        initialized_resources_ = true;
    } else {
        // initialize OpenGL ES and EGL
        if (EGL_SUCCESS == gl_context_->Resume(app_->window)) {
            UnloadResources();
        } else {
            assert(0);
        }
    }

    SetupView();

    focusStack.front()->GInit_();

    ShowUI();

    // Initialize GL state.
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);
    glViewport(0, 0, gl_context_->GetScreenWidth(), gl_context_->GetScreenHeight());

    return 0;
}

void GEngine::SetupView()
{
    int32_t viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    mat_projection_ = ndk_helper::Mat4::Ortho2D(0, 0, viewport[2], -viewport[3]);

    screen_width = viewport[2];
    screen_height = viewport[3];
    screen_ratio = screen_width/screen_height;

    const float CAM_X = 0.f;
    const float CAM_Y = 0.f;
    const float CAM_Z = 700.f;

    mat_view_ = ndk_helper::Mat4::LookAt(ndk_helper::Vec3(CAM_X, CAM_Y, CAM_Z),
                                         ndk_helper::Vec3(0.f, 0.f, 0.f),
                                         ndk_helper::Vec3(0.f, 1.f, 0.f));
}

/**
 * Just the current frame in the display.
 */
void GEngine::DrawFrame() {
    float fps;
    if (monitor_.Update(fps)) {
        UpdateFPS(fps);
    }

    // Just fill the screen with a color.
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    focusStack.front()->GRender_((float)monitor_.GetCurrentTime());

    // Swap
    if (EGL_SUCCESS != gl_context_->Swap()) {
        UnloadResources();
    }
}

/**
 * Tear down the EGL context currently associated with the display.
 */
void GEngine::TermDisplay() { gl_context_->Suspend(); }

void GEngine::TrimMemory() {
    LOGI("Trimming memory");
    gl_context_->Invalidate();
}

//-------------------------------------------------------------------------
// Misc
//-------------------------------------------------------------------------
void GEngine::SetState(android_app *state) {
    app_ = state;
    tap_detector_.SetConfiguration(app_->config);
    doubletap_detector_.SetConfiguration(app_->config);
    drag_detector_.SetConfiguration(app_->config);
    pinch_detector_.SetConfiguration(app_->config);
}

bool GEngine::IsReady() {
    return has_focus_;
}

void GEngine::ShowUI() {
    JNIEnv *jni;
    app_->activity->vm->AttachCurrentThread(&jni, nullptr);

    // Default class retrieval
    jclass clazz = jni->GetObjectClass(app_->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "showUI", "()V");
    jni->CallVoidMethod(app_->activity->clazz, methodID);

    app_->activity->vm->DetachCurrentThread();
}

void GEngine::UpdateFPS(float fFPS) {
    JNIEnv *jni;
    app_->activity->vm->AttachCurrentThread(&jni, nullptr);

    // Default class retrieval
    jclass clazz = jni->GetObjectClass(app_->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "updateFPS", "(F)V");
    jni->CallVoidMethod(app_->activity->clazz, methodID, fFPS);

    app_->activity->vm->DetachCurrentThread();
}

Shader::Shader() {
    program_ = 0;
    param_view_ = 0;
    param_texture_angle_ = 0;
}

std::map<std::string, Shader> Shader::shaders;

Shader Shader::CreateShaderProgram(const char *vsh, const char *fsh) {
    GLuint program;
    GLuint vert_shader;
    GLuint frag_shader;

    Shader sh;

    std::string svsh = vsh;
    std::string sfsh = fsh;

    auto program_name = svsh + sfsh;

    auto loaded = shaders.find(program_name);
    if (loaded != shaders.end())
        return loaded->second;

    // Create shader program
    program = glCreateProgram();
    LOGI("Created Shader %d", program);

    // Create and compile vertex shader
    if (!ndk_helper::shader::CompileShader(&vert_shader, GL_VERTEX_SHADER,
                                           vsh)) {
        LOGI("Failed to compile vertex shader");
        glDeleteProgram(program);
        return sh;
    }

    // Create and compile fragment shader
    if (!ndk_helper::shader::CompileShader(&frag_shader, GL_FRAGMENT_SHADER,
                                           fsh)) {
        LOGI("Failed to compile fragment shader");
        glDeleteProgram(program);
        return sh;
    }

    // Attach vertex shader to program
    glAttachShader(program, vert_shader);

    // Attach fragment shader to program
    glAttachShader(program, frag_shader);


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

        return sh;
    }

    sh.program_ = program;
    sh.param_view_ = glGetUniformLocation(program, "uPMatrix");
    sh.param_texture_angle_ = glGetUniformLocation(program, "angle");
    sh.param_z_ = glGetUniformLocation(program, "z");
    sh.param_color_ = glGetUniformLocation(program, "color");

    // Release vertex and fragment shaders
    if (vert_shader) glDeleteShader(vert_shader);
    if (frag_shader) glDeleteShader(frag_shader);

    shaders.insert({program_name, sh});

    return sh;
}