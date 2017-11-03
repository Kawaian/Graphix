/*
#include <wtypes.h>
#include "../templates/GameStateManager.h"
#include "../core/LogManager.h"
#include "../templates/GameState.h"

using namespace Spiky;


static glm::ivec2 GetScreenResolution()
{
	RECT screen;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &screen);
	return glm::ivec2(screen.right, screen.bottom);
}

GameStateManager::GameStateManager(const char* title, int x, int y, unsigned width, unsigned height, bool resizeable, bool fullScreen)
	:
		m_screenResolution(GetScreenResolution()),
		m_window(x, y, width, height, title, resizeable, fullScreen),
		m_running(false)
{}

void GameStateManager::SwitchState(GameState* state){
    if(!m_states.empty())
	{
        m_states.back()->DISPOSE();
        m_states.pop_back();
    }
    m_states.push_back(state);
    m_states.back()->INIT();
}

void GameStateManager::PushState(GameState* state){
    if(!m_states.empty())
	{
        m_states.back()->Pause();
    }
    m_states.push_back(state);
    m_states.back()->INIT();
}

void GameStateManager::PopState(){
    if(!m_states.empty())
	{
        m_states.back()->DISPOSE();
        m_states.pop_back();
    }

    if(!m_states.empty())
	{
        m_states.back()->Resume();
    }
}

void GameStateManager::ProcessInput()
{
	m_input.Update();
	m_running = !(m_input.CloseRequested() || m_input.KeyDown(SDL_SCANCODE_ESCAPE));

	

    m_states.back()->ProcessInput(m_input);
}

void GameStateManager::Update()
{
	m_window.Update();
	m_states.back()->Update();
}

void GameStateManager::Render()
{
	m_window.BindAsRenderTarget();
	m_window.SwapBuffers();
	m_window.Clear(0.0f, 0.0f, 0.0f, 0.0f);
    m_states.back()->Render();
}

void GameStateManager::DISPOSE()
{
    while(!m_states.empty()){
        m_states.back()->DISPOSE();
        m_states.pop_back();
    }
	LogManager::LogInfo("\tCoreEngine", "Disposed all states !");
}
*/