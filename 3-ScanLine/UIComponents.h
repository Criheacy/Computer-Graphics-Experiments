#pragma once

#include "Settings.h"

namespace UI
{
	class Canvas
	{
	public:
		static Canvas& Instance();

		Canvas(Canvas const&) = delete;
		void operator=(Canvas const&) = delete;

		void AddComponent(class Component* component);
		void RemoveComponent(class Component* component);
		class Component* GetComponentByID(unsigned int id);

		bool HandleMouseDown(glm::vec2 src);
		bool HandleMouseUp(glm::vec2 src);
		bool HandleMotion(glm::vec2 src);

		glm::vec2 ProjectToScreen(glm::vec2 src);
		void Render();

	protected:
		Canvas();
		std::vector<class Component*> componentList;

		glm::mat3x3 project;
	};

	class Component
	{
	public:
		Component(unsigned int id);

		unsigned int GetID();

		virtual bool InComponent(glm::vec2 src);
		virtual bool HandleMotion(glm::vec2 src);
		virtual bool HandleMouseDown(glm::vec2 src);
		virtual bool HandleMouseUp(glm::vec2 src);
		virtual void Render();
		
	protected:
		unsigned int id;
	};

	class RadioButton : public Component
	{
	public:
		RadioButton(unsigned int id);

		class Button& operator[](int i);
		void AddButton(class Button button);
		int GetButtonCount();
		unsigned int GetSelectedID();

		bool InComponent(glm::vec2 src) override;
		bool HandleMotion(glm::vec2 src) override;
		bool HandleMouseDown(glm::vec2 src) override;
		bool HandleMouseUp(glm::vec2 src) override;
		void Render() override;

		void Select(int i);
		void Select(class Button* button);
		void DeselectAll();

	private:
		std::vector<class Button> radioButtonList;
		Button* selectedButton;
	};

	class Button : public Component
	{
	public:
		Button(unsigned int id, glm::vec2 leftTop, glm::vec2 rightBottom);
		Button(unsigned int id, float left, float top, float right, float bottom);
		void Resize(glm::vec2 leftTop, glm::vec2 rightBottom);
		void SetText(std::string text);

		bool InComponent(glm::vec2 src) override;
		bool HandleMotion(glm::vec2 src) override;
		bool HandleMouseDown(glm::vec2 src) override;
		bool HandleMouseUp(glm::vec2 src) override;
		void Render() override;

		// Interactive interface
		void SetOnSelectListener(void (*OnSelected)(void));
		void Select();
		void Deselect();

	private:
		std::string text;
		glm::vec2 textOffset;

		void (*OnSelected)(void);

		bool isSelected;
		bool isHovering;
		bool isPressing;

		glm::vec2 leftTop;
		glm::vec2 rightBottom;

		glm::vec3 selectingColor;
		glm::vec3 hoveringColor;
		glm::vec3 pressingColor;
		glm::vec3 idleColor;
	};
}