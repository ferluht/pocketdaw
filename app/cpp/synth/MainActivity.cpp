#include <bx/uint32_t.h>
#include <common.h>
#include <bgfx_utils.h>
#include "logo.h"
#include <GUI/vectordisplay/vectordisplay.h>
#include <imgui/imgui.h>
#include <common/nanovg/nanovg_bgfx.h>
#include <common/nanovg/nanovg.h>
#include <synth/Synth.h>

namespace
{

    int32_t createFont(NVGcontext* _ctx, const char* _name, const char* _filePath)
    {
        uint32_t size;
        void* data = load(_filePath, &size);
        if (NULL == data)
        {
            return -1;
        }

        return nvgCreateFontMem(_ctx, _name, (uint8_t*)data, size, 0);
    }


    class MainActivity : public entry::AppI
    {
    public:
        MainActivity(const char* _name, const char* _description, const char* _url)
                : entry::AppI(_name, _description, _url)
        {
        }

        void init(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height) override
        {
            Args args(_argc, _argv);

            m_width  = _width;
            m_height = _height;
            m_debug  = BGFX_DEBUG_TEXT;
            m_reset  = BGFX_RESET_VSYNC;

            bgfx::Init init;
            init.type     = args.m_type;
            init.vendorId = args.m_pciId;
            init.resolution.width  = m_width;
            init.resolution.height = m_height;
            init.resolution.reset  = m_reset;
            bgfx::init(init);

            // Enable debug text.
            bgfx::setDebug(m_debug);

            // Set view 0 clear state.
            bgfx::setViewClear(0
                    , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
                    , 0x303030ff
                    , 1.0f
                    , 0
            );

            const bgfx::RendererType::Enum renderer = bgfx::getRendererType();
            float texelHalf = bgfx::RendererType::Direct3D9 == renderer ? 0.5f : 0.0f;
            bool originBottomLeft = bgfx::RendererType::OpenGL == renderer
                                    || bgfx::RendererType::OpenGLES == renderer;
            m_vd.init(originBottomLeft, texelHalf);
            m_vd.setup(uint16_t(m_width), uint16_t(m_height) );

            imguiCreate();

            m_nvg = nvgCreate(1, 0);

            createFont(m_nvg, "sans", "font/droidsans.ttf");

            S = new Synth();
            S->shape->lPlace({0, 0});
            S->shape->lSetHeight(1);
            S->shape->lSetWidth(1);

            eng = new Engine(S);

            GUI::GEngine::getGEngine().setDisplay(m_nvg, m_width, m_height);

            eng->audio->start();
        }

        virtual int shutdown() override
        {
            imguiDestroy();

            m_vd.teardown();

            nvgDelete(m_nvg);

            // Shutdown bgfx.
            bgfx::shutdown();

            return 0;
        }

        bool update() override
        {


            if (!entry::processEvents(m_width, m_height, m_debug, m_reset, &m_mouseState) )
            {
//                imguiBeginFrame(m_mouseState.m_mx
//                        ,  m_mouseState.m_my
//                        , (m_mouseState.m_buttons[entry::MouseButton::Left  ] ? IMGUI_MBUT_LEFT   : 0)
//                          | (m_mouseState.m_buttons[entry::MouseButton::Right ] ? IMGUI_MBUT_RIGHT  : 0)
//                          | (m_mouseState.m_buttons[entry::MouseButton::Middle] ? IMGUI_MBUT_MIDDLE : 0)
//                        ,  m_mouseState.m_mz
//                        , uint16_t(m_width)
//                        , uint16_t(m_height)
//                );
//
//                showExampleDialog(this);
//
//                imguiEndFrame();

                // Set view 0 default viewport.
//                bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height) );
//
//                // This dummy draw call is here to make sure that view 0 is cleared
//                // if no other draw calls are submitted to view 0.
//                bgfx::touch(0);
//
//                // Use debug font to print information about this example.
//                bgfx::dbgTextClear();
////                bgfx::dbgTextImage(
////                        bx::max<uint16_t>(uint16_t(m_width /2/20 ), 20)-20
////                        , bx::max<uint16_t>(uint16_t(m_height/2/26),  60)-6
////                        , 40
////                        , 12
////                        , s_logo
////                        , 160
////                );
//
//                bgfx::dbgTextPrintf(10, 20, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");

                eng->graphic->Render();

                return true;
            }

            return false;
        }

        entry::MouseState m_mouseState;

        VectorDisplay m_vd;

        NVGcontext* m_nvg;

        Engine * eng;

        Synth * S;

        uint32_t m_width;
        uint32_t m_height;
        uint32_t m_debug;
        uint32_t m_reset;
    };

} // namespace

ENTRY_IMPLEMENT_MAIN(
        MainActivity
, "00-helloworld"
, "Initialization and debug text."
, "https://bkaradzic.github.io/bgfx/examples.html#helloworld"
);