#pragma once
#include "../render/GameStateManager.h"
#include "alpha.h"
#include "alpha/Renderer.h"

namespace gpx
{
	class Application : public GameState{
	public:
		void Init(CoreEngine* parent) override;
		void Update(float delta) override;
		void Render() override;

		static Application* State(){
			return &s_state;
		}

	private:
		static Application s_state;

		Window m_window;
		ForwardRenderer renderer;
		Mesh m_box;
		PhongLight m_light;
		Material m_material;

		Camera m_camera;
		std::vector<RenderObject> m_objects;
		InputHandler m_input;
	};
}