/*
 * Copyright 2011-2019 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "entry_p.h"

#if ENTRY_CONFIG_USE_NATIVE && BX_PLATFORM_ANDROID

#include <bgfx/platform.h>

#include <bx/thread.h>
#include <bx/file.h>

#include <android/input.h>
#include <android/log.h>
#include <android/looper.h>
#include <android/window.h>
#include <android_native_app_glue.h>
#include <android/native_window.h>
#include <vecmath.h>
#include <gestureDetector.h>
#include <G.h>
#include <M.h>
#include <A.h>

extern "C"
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <android_native_app_glue.c>
#pragma GCC diagnostic pop
} // extern "C"

namespace entry
{
	///
	inline void androidSetWindow(::ANativeWindow* _window)
	{
		bgfx::PlatformData pd;
		pd.ndt          = NULL;
		pd.nwh          = _window;
		pd.context      = NULL;
		pd.backBuffer   = NULL;
		pd.backBufferDS = NULL;
		bgfx::setPlatformData(pd);
	}

	struct GamepadRemap
	{
		uint16_t  m_keyCode;
		Key::Enum m_key;
	};

	static GamepadRemap s_gamepadRemap[] =
	{
		{ AKEYCODE_DPAD_UP,       Key::GamepadUp        },
		{ AKEYCODE_DPAD_DOWN,     Key::GamepadDown      },
		{ AKEYCODE_DPAD_LEFT,     Key::GamepadLeft      },
		{ AKEYCODE_DPAD_RIGHT,    Key::GamepadRight     },
		{ AKEYCODE_BUTTON_START,  Key::GamepadStart     },
		{ AKEYCODE_BACK,          Key::GamepadBack      },
		{ AKEYCODE_BUTTON_THUMBL, Key::GamepadThumbL    },
		{ AKEYCODE_BUTTON_THUMBR, Key::GamepadThumbR    },
		{ AKEYCODE_BUTTON_L1,     Key::GamepadShoulderL },
		{ AKEYCODE_BUTTON_R1,     Key::GamepadShoulderR },
		{ AKEYCODE_GUIDE,         Key::GamepadGuide     },
		{ AKEYCODE_BUTTON_A,      Key::GamepadA         },
		{ AKEYCODE_BUTTON_B,      Key::GamepadB         },
		{ AKEYCODE_BUTTON_X,      Key::GamepadX         },
		{ AKEYCODE_BUTTON_Y,      Key::GamepadY         },
	};

	struct GamepadAxisRemap
	{
		int32_t m_event;
		GamepadAxis::Enum m_axis;
		bool m_convert;
	};

	static GamepadAxisRemap s_translateAxis[] =
	{
		{ AMOTION_EVENT_AXIS_X,        GamepadAxis::LeftX,  false },
		{ AMOTION_EVENT_AXIS_Y,        GamepadAxis::LeftY,  false },
		{ AMOTION_EVENT_AXIS_LTRIGGER, GamepadAxis::LeftZ,  false },
		{ AMOTION_EVENT_AXIS_Z,        GamepadAxis::RightX, true  },
		{ AMOTION_EVENT_AXIS_RZ,       GamepadAxis::RightY, false },
		{ AMOTION_EVENT_AXIS_RTRIGGER, GamepadAxis::RightZ, false },
	};

	struct MainThreadEntry
	{
	    int height;
	    int width;
		int m_argc;
		const char* const* m_argv;

		static int32_t threadFunc(bx::Thread* _thread, void* _userData);
	};

	class FileReaderAndroid : public bx::FileReaderI
	{
	public:
		FileReaderAndroid(AAssetManager* _assetManager, AAsset* _file)
			: m_assetManager(_assetManager)
			, m_file(_file)
			, m_open(false)
		{
		}

		virtual ~FileReaderAndroid()
		{
			close();
		}

		virtual bool open(const bx::FilePath& _filePath, bx::Error* _err) override
		{
			BX_CHECK(NULL != _err, "Reader/Writer interface calling functions must handle errors.");

			if (NULL != m_file)
			{
				BX_ERROR_SET(_err, BX_ERROR_READERWRITER_ALREADY_OPEN, "FileReader: File is already open.");
				return false;
			}

			m_file = AAssetManager_open(m_assetManager, _filePath.getCPtr(), AASSET_MODE_RANDOM);
			if (NULL == m_file)
			{
				BX_ERROR_SET(_err, BX_ERROR_READERWRITER_OPEN, "FileReader: Failed to open file.");
				return false;
			}

			m_open = true;
			return true;
		}

		virtual void close() override
		{
			if (m_open
			&&  NULL != m_file)
			{
				AAsset_close(m_file);
				m_file = NULL;
			}
		}

		virtual int64_t seek(int64_t _offset, bx::Whence::Enum _whence) override
		{
			BX_CHECK(NULL != m_file, "Reader/Writer file is not open.");
			return AAsset_seek64(m_file, _offset, _whence);

		}

		virtual int32_t read(void* _data, int32_t _size, bx::Error* _err) override
		{
			BX_CHECK(NULL != m_file, "Reader/Writer file is not open.");
			BX_CHECK(NULL != _err, "Reader/Writer interface calling functions must handle errors.");

			int32_t size = (int32_t)AAsset_read(m_file, _data, _size);
			if (size != _size)
			{
				if (0 == AAsset_getRemainingLength(m_file) )
				{
					BX_ERROR_SET(_err, BX_ERROR_READERWRITER_EOF, "FileReader: EOF.");
				}

				return size >= 0 ? size : 0;
			}

			return size;
		}

	private:
		AAssetManager* m_assetManager;
		AAsset* m_file;
		bool m_open;
	};

	struct Context
	{

		gestures::TapDetector tap_detector_;
		gestures::DragDetector drag_detector_;
		gestures::DoubletapDetector doubletap_detector_;
		gestures::PinchDetector pinch_detector_;

		Context()
			: m_window(NULL)
		{
			bx::memSet(m_value, 0, sizeof(m_value) );

			// Deadzone values from xinput.h
			m_deadzone[GamepadAxis::LeftX ] =
			m_deadzone[GamepadAxis::LeftY ] = 7849;
			m_deadzone[GamepadAxis::RightX] =
			m_deadzone[GamepadAxis::RightY] = 8689;
			m_deadzone[GamepadAxis::LeftZ ] =
			m_deadzone[GamepadAxis::RightZ] = 30;
		}

		void run(android_app* _app)
		{
			m_app = _app;
			m_app->userData = (void*) this;
			m_app->onAppCmd = onAppCmdCB;
			m_app->onInputEvent = onInputEventCB;

			ANativeActivity_setWindowFlags(m_app->activity, 0
				| AWINDOW_FLAG_FULLSCREEN
				| AWINDOW_FLAG_KEEP_SCREEN_ON
				, 0
				);

			const char* const argv[1] = { "android.so" };
			m_mte.m_argc = 1;
			m_mte.m_argv = argv;

			while (0 == m_app->destroyRequested)
			{
				int32_t num;
				android_poll_source* source;
				/*int32_t id =*/ ALooper_pollAll(-1, NULL, &num, (void**)&source);

				if (NULL != source)
				{
					source->process(m_app, source);
				}
			}

			m_thread.shutdown();
		}

		void onAppCmd(int32_t _cmd)
		{
			switch (_cmd)
			{
				case APP_CMD_INPUT_CHANGED:
					// Command from main thread: the AInputQueue has changed.  Upon processing
					// this command, android_app->inputQueue will be updated to the new queue
					// (or NULL).
					break;

				case APP_CMD_INIT_WINDOW:
					// Command from main thread: a new ANativeWindow is ready for use.  Upon
					// receiving this command, android_app->window will contain the new window
					// surface.
					if (m_window != m_app->window)
					{
						m_window = m_app->window;
						androidSetWindow(m_window);

						int32_t width  = ANativeWindow_getWidth(m_window);
						int32_t height = ANativeWindow_getHeight(m_window);

						m_mte.height = height;
						m_mte.width = width;

//						char width_char[10];
//                        char height_char[10];
//
//                        sprintf(width_char, "%d", width);
//                        sprintf(height_char, "%d", height);
//
//						m_mte.m_argc = 3;
//                        const char* const argv[3] = { "android.so", width_char, height_char};
//                        m_mte.m_argv = argv;

						DBG("ANativeWindow width %d, height %d", width, height);
						WindowHandle defaultWindow = { 0 };
						m_eventQueue.postSizeEvent(defaultWindow, width, height);

						if (!m_thread.isRunning() )
						{
							MEngine::getMEngine().attachApp(m_app);
							AEngine::getAEngine().attachApp(m_app);
							m_thread.init(MainThreadEntry::threadFunc, &m_mte);
						}
					}
					break;

				case APP_CMD_TERM_WINDOW:
					// Command from main thread: the existing ANativeWindow needs to be
					// terminated.  Upon receiving this command, android_app->window still
					// GContains the existing window; after calling android_app_exec_cmd
					// it will be set to NULL.
					break;

				case APP_CMD_WINDOW_RESIZED:
					// Command from main thread: the current ANativeWindow has been resized.
					// Please redraw with its new size.
					break;

				case APP_CMD_WINDOW_REDRAW_NEEDED:
					// Command from main thread: the system needs that the current ANativeWindow
					// be redrawn.  You should redraw the window before handing this to
					// android_app_exec_cmd() in order to avoid transient drawing glitches.
					break;

				case APP_CMD_CONTENT_RECT_CHANGED:
					// Command from main thread: the content area of the window has changed,
					// such as from the soft input window being shown or hidden.  You can
					// find the new content rect in android_app::contentRect.
					break;

				case APP_CMD_GAINED_FOCUS:
				{
					// Command from main thread: the app's activity window has gained
					// input focus.
					WindowHandle defaultWindow = { 0 };
					m_eventQueue.postSuspendEvent(defaultWindow, Suspend::WillResume);
					break;
				}

				case APP_CMD_LOST_FOCUS:
				{
					// Command from main thread: the app's activity window has lost
					// input focus.
					WindowHandle defaultWindow = { 0 };
					m_eventQueue.postSuspendEvent(defaultWindow, Suspend::WillSuspend);
					break;
				}

				case APP_CMD_CONFIG_CHANGED:
					// Command from main thread: the current device configuration has changed.
					break;

				case APP_CMD_LOW_MEMORY:
					// Command from main thread: the system is running low on memory.
					// Try to reduce your memory use.
					break;

				case APP_CMD_START:
					// Command from main thread: the app's activity has been started.
					break;

				case APP_CMD_RESUME:
				{
					// Command from main thread: the app's activity has been resumed.
					WindowHandle defaultWindow = { 0 };
					m_eventQueue.postSuspendEvent(defaultWindow, Suspend::DidResume);
					break;
				}

				case APP_CMD_SAVE_STATE:
					// Command from main thread: the app should generate a new saved state
					// for itself, to restore from later if needed.  If you have saved state,
					// allocate it with malloc and place it in android_app.savedState with
					// the size in android_app.savedStateSize.  The will be freed for you
					// later.
					break;

				case APP_CMD_PAUSE:
				{
					// Command from main thread: the app's activity has been paused.
					WindowHandle defaultWindow = { 0 };
					m_eventQueue.postSuspendEvent(defaultWindow, Suspend::DidSuspend);
					break;
				}

				case APP_CMD_STOP:
					// Command from main thread: the app's activity has been stopped.
					break;

				case APP_CMD_DESTROY:
					// Command from main thread: the app's activity is being destroyed,
					// and waiting for the app thread to clean up and exit before proceeding.
					m_eventQueue.postExitEvent();
					break;
			}
		}

		bool filter(GamepadAxis::Enum _axis, int32_t* _value)
		{
			const int32_t old = m_value[_axis];
			const int32_t deadzone = m_deadzone[_axis];
			int32_t value = *_value;
			value = value > deadzone || value < -deadzone ? value : 0;
			m_value[_axis] = value;
			*_value = value;
			return old != value;
		}

		int32_t onInputEvent(AInputEvent* event)
		{
			auto eng = &GUI::GEngine::getGEngine();
			if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
				gestures::GESTURE_STATE tapState = tap_detector_.Detect(event);
				gestures::GESTURE_STATE doubleTapState = doubletap_detector_.Detect(event);
				gestures::GESTURE_STATE dragState = drag_detector_.Detect(event);
				gestures::GESTURE_STATE pinchState = pinch_detector_.Detect(event);

                // Handle drag state
                if (tapState & gestures::GESTURE_STATE_ACTION) {
                    // Otherwise, start dragging
                    vecmath::Vec2 v;
                    tap_detector_.GetPointer(v);
                    eng->FindFocusObject(v);
                    auto new_focus = eng->focusStack.back()->GTapEnd(v);
//					eng->unfocus();
                    eng->focusOn(new_focus);
                    return 1;
                }

				// Handle drag state
				if (doubleTapState & gestures::GESTURE_STATE_ACTION) {
					// Otherwise, start dragging
					vecmath::Vec2 v;
					doubletap_detector_.GetPointer(v);
					eng->FindFocusObject(v);
					auto new_focus = eng->focusStack.back()->GDoubleTapEnd(v);
//					eng->unfocus();
					eng->focusOn(new_focus);
					return 1;
				}

				// Handle drag state
				if (dragState & gestures::GESTURE_STATE_START) {
					// Otherwise, start dragging
					vecmath::Vec2 v;
					drag_detector_.GetPointer(v);
					eng->FindFocusObject(v);
					auto new_focus = eng->focusStack.back()->GDragBegin(v);
					eng->focusOn(new_focus);
				} else if (dragState & gestures::GESTURE_STATE_MOVE) {
					vecmath::Vec2 v;
					drag_detector_.GetPointer(v);
					auto new_focus = eng->focusStack.back()->GDragHandler(v);
				} else if (dragState & gestures::GESTURE_STATE_END) {
					vecmath::Vec2 v;
					drag_detector_.GetPointer(v);
					auto new_focus = eng->focusStack.back()->GDragEnd(v);
//					eng->unfocus();
					eng->focusOn(new_focus);
				}

				return 1;
			}

			return 0;
		}

		static void onAppCmdCB(struct android_app* _app, int32_t _cmd)
		{
			Context* self = (Context*)_app->userData;
			self->onAppCmd(_cmd);
		}

		static int32_t onInputEventCB(struct android_app* _app, AInputEvent* _event)
		{
			Context* self = (Context*)_app->userData;
			return self->onInputEvent(_event);
		}

		MainThreadEntry m_mte;
		bx::Thread m_thread;

		EventQueue m_eventQueue;

		ANativeWindow* m_window;
		android_app* m_app;

		int32_t m_value[GamepadAxis::Count];
		int32_t m_deadzone[GamepadAxis::Count];
	};

	static Context s_ctx;

	const Event* poll()
	{
		return s_ctx.m_eventQueue.poll();
	}

	const Event* poll(WindowHandle _handle)
	{
		return s_ctx.m_eventQueue.poll(_handle);
	}

	void release(const Event* _event)
	{
		s_ctx.m_eventQueue.release(_event);
	}

	WindowHandle createWindow(int32_t _x, int32_t _y, uint32_t _width, uint32_t _height, uint32_t _flags, const char* _title)
	{
		BX_UNUSED(_x, _y, _width, _height, _flags, _title);
		WindowHandle handle = { UINT16_MAX };
		return handle;
	}

	void destroyWindow(WindowHandle _handle)
	{
		BX_UNUSED(_handle);
	}

	void setWindowPos(WindowHandle _handle, int32_t _x, int32_t _y)
	{
		BX_UNUSED(_handle, _x, _y);
	}

	void setWindowSize(WindowHandle _handle, uint32_t _width, uint32_t _height)
	{
		BX_UNUSED(_handle, _width, _height);
	}

	void setWindowTitle(WindowHandle _handle, const char* _title)
	{
		BX_UNUSED(_handle, _title);
	}

	void setWindowFlags(WindowHandle _handle, uint32_t _flags, bool _enabled)
	{
		BX_UNUSED(_handle, _flags, _enabled);
	}

	void toggleFullscreen(WindowHandle _handle)
	{
		BX_UNUSED(_handle);
	}

	void setMouseLock(WindowHandle _handle, bool _lock)
	{
		BX_UNUSED(_handle, _lock);
	}

	int32_t MainThreadEntry::threadFunc(bx::Thread* _thread, void* _userData)
	{
		BX_UNUSED(_thread);

		int32_t result = chdir("/sdcard/.pocketdaw");
		BX_CHECK(0 == result, "Failed to chdir to dir. android.permission.WRITE_EXTERNAL_STORAGE?", errno);

		MainThreadEntry* self = (MainThreadEntry*)_userData;

		result = main(self->height, self->width, self->m_argc, self->m_argv);
//		PostMessage(s_ctx.m_hwnd, WM_QUIT, 0, 0);
		return result;
	}

} // namespace entry

extern "C" void android_main(android_app* _app)
{
	using namespace entry;
	s_ctx.run(_app);
}

#endif // ENTRY_CONFIG_USE_NATIVE && BX_PLATFORM_ANDROID
