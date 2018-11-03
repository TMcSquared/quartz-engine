#include "engine/graphics/opengl/opengl.hpp"
#include "engine/SDL/SDLWindow.hpp"

#include "engine/math/Vector2.hpp"

using namespace phx::sdl;
using namespace phx;

SDLWindow::SDLWindow(const char* title, int width, int height, phx::gfx::GLVersion version, phx::gfx::GLProfile profile)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, version.major);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, version.minor);

	uint32_t SDLProfile = SDL_GL_CONTEXT_PROFILE_CORE;
	switch (profile)
	{
	case phx::gfx::GLProfile::CORE:
		SDLProfile = SDL_GL_CONTEXT_PROFILE_CORE;
		break;
	case phx::gfx::GLProfile::COMPATABILITY:
		SDLProfile = SDL_GL_CONTEXT_PROFILE_COMPATIBILITY;
		break;
	case phx::gfx::GLProfile::ES:
		SDLProfile = SDL_GL_CONTEXT_PROFILE_ES;
		break;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDLProfile);

	m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	if (m_window == nullptr)
	{
		SDL_Quit();
		ERROR("Couldn't create window, need OpenGL >= " + std::to_string(version.major) + "." + std::to_string(version.minor));
		exit(EXIT_FAILURE);
	}

	m_context = SDL_GL_CreateContext(m_window);

	if (glewInit() != GLEW_OK)
	{
		ERROR("Uh Oh! There was a booboo, and we can't fix it :(. Tell the pros that an OpenGL context could not be created. Sorry for the inconvenience!");
		exit(EXIT_FAILURE);
	}

	SDL_ShowWindow(m_window);

	GLCheck(glEnable(GL_DEPTH_TEST));

	m_running = true;
}

void SDLWindow::pollEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event) > 0)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_running = false;
			break;
		case SDL_MOUSEMOTION:
			for (auto& e : m_mouseMoveEvents)
			{
				e.callback(event.motion.x, event.motion.y);
			}
			break;
		case SDL_KEYDOWN:
			for (auto& e : m_keyEvents)
			{
				if (e.eventType == static_cast<int>(gfx::EventType::PRESSED))
				{
					if (event.key.keysym.scancode == static_cast<SDL_Scancode>(e.key))
					{
						std::cout << "Key Pressed!" << std::endl;
						e.callback();
					}
				}
			}
			break;
		case SDL_KEYUP:
			for (auto& e : m_keyEvents)
			{
				if (e.eventType == static_cast<int>(gfx::EventType::RELEASED))
				{
					if (event.key.keysym.scancode == static_cast<SDL_Scancode>(e.key))
					{
						e.callback();
					}
				}
			}
			break;
		}
	}
}

void SDLWindow::swapBuffers()
{
	SDL_GL_SwapWindow(m_window);
}

bool SDLWindow::isRunning()
{
	return m_running;
}

void SDLWindow::close()
{
	m_running = false;
}

void SDLWindow::setTitle(const char* title)
{
	SDL_SetWindowTitle(m_window, title);
}

void SDLWindow::getSize(int& width, int& height)
{
	SDL_GetWindowSize(m_window, &width, &height);
}

void SDLWindow::setFullscreen(bool enabled)
{
	SDL_SetWindowFullscreen(m_window, enabled ? SDL_WINDOW_FULLSCREEN : 0);
}

void SDLWindow::setResizable(bool enabled)
{
	SDL_SetWindowResizable(m_window, enabled ? SDL_TRUE : SDL_FALSE);
}

void SDLWindow::setCursorState(phx::gfx::CursorState cursorState)
{
	bool on = cursorState == phx::gfx::CursorState::NORMAL;
	SDL_ShowCursor(on);
}

void SDLWindow::setVSync(bool enabled)
{
	SDL_GL_SetSwapInterval(enabled ? 1 : 0);
}

void SDLWindow::addKeyCallback(int eventType, int key, std::function<void()> callback)
{
	m_keyEvents.push_back({ eventType, key, callback });
}

void SDLWindow::addMouseMoveCallback(std::function<void(double, double)> callback)
{
	m_mouseMoveEvents.push_back({ callback });
}

bool SDLWindow::isKeyDown(int key)
{
	return SDL_GetKeyboardState(NULL)[key];
}

TVector2<int> SDLWindow::getMousePosition()
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	return { x, y };
}

void SDLWindow::setMousePosition(TVector2<int> newPos)
{
	SDL_WarpMouseInWindow(m_window, newPos.x, newPos.y);
}