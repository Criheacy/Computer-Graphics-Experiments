#include "UIComponents.h"

namespace UI
{
	Canvas::Canvas()
	{
		componentList = std::vector<Component*>();

		project = glm::mat3x3(
			2.0f / SCREEN_WIDTH, 0.0f, 0.0f,
			0.0f, -2.0f / SCREEN_HEIGHT, 0.0f,
			-1.0f, 1.0f, 1.0f);
	}

	void Canvas::AddComponent(Component* component)
	{
		componentList.push_back(component);
	}

	void Canvas::RemoveComponent(Component* component)
	{
		auto it = std::find(componentList.begin(), componentList.end(), component);
		if (it != componentList.end())
			componentList.erase(it);
	}

	Component* Canvas::GetComponentByID(unsigned int id)
	{
		for (auto component = componentList.begin(); component != componentList.end(); component++)
		{
			if ((*component)->GetID() == id)
				return *component;
		}
		return NULL;
	}

	bool Canvas::HandleMouseDown(glm::vec2 src)
	{
		bool result = false;
		for (auto component = componentList.begin(); component != componentList.end(); component++)
		{
			result = result || (*component)->HandleMouseDown(src);
		}
		return result;
	}

	bool Canvas::HandleMouseUp(glm::vec2 src)
	{
		bool result = false;
		for (auto component = componentList.begin(); component != componentList.end(); component++)
		{
			result = result || (*component)->HandleMouseUp(src);
		}
		return result;
	}

	bool Canvas::HandleMotion(glm::vec2 src)
	{
		bool result = false;
		for (auto component = componentList.begin(); component != componentList.end(); component++)
		{
			result = result || (*component)->HandleMotion(src);
		}
		return result;
	}

	glm::vec2 Canvas::ProjectToScreen(glm::vec2 src)
	{
		return project * glm::vec3(src.x, src.y, 1.0f);
	}

	void Canvas::Render()
	{
		for (auto component = componentList.begin(); component != componentList.end(); component++)
		{
			(*component)->Render();
		}
	}

	Canvas& Canvas::Instance()
	{
		static Canvas instance;
		return instance;
	}

	Component::Component(unsigned int id)
	{
		this->id = id;
	}

	unsigned int Component::GetID()
	{
		return id;
	}

	bool Component::InComponent(glm::vec2 src)
	{
		return false;
	}

	bool Component::HandleMotion(glm::vec2 src)
	{
		return false;
	}

	bool Component::HandleMouseDown(glm::vec2 src)
	{
		return false;
	}

	bool Component::HandleMouseUp(glm::vec2 src)
	{
		return false;
	}

	void Component::Render()
	{
		// Nothing to render
	}

	RadioButton::RadioButton(unsigned int id) : Component(id)
	{
		radioButtonList = std::vector<Button>();
	}

	Button& RadioButton::operator[](int i)
	{
		return radioButtonList[i];
	}

	void RadioButton::AddButton(Button button)
	{
		radioButtonList.push_back(button);
	}

	int RadioButton::GetButtonCount()
	{
		return radioButtonList.size();
	}

	unsigned int RadioButton::GetSelectedID()
	{
		if (selectedButton != NULL)
			return selectedButton->GetID();
		else return 0;
	}

	bool RadioButton::InComponent(glm::vec2 src)
	{
		for (auto button = radioButtonList.begin(); button != radioButtonList.end(); button++)
		{
			if ((*button).InComponent(src))
				return true;
		}
		return false;
	}

	bool RadioButton::HandleMotion(glm::vec2 src)
	{
		for (auto button = radioButtonList.begin(); button != radioButtonList.end(); button++)
		{
			if ((*button).HandleMotion(src))
				return true;
		}
		return false;
	}

	bool RadioButton::HandleMouseUp(glm::vec2 src)
	{
		for (auto button = radioButtonList.begin(); button != radioButtonList.end(); button++)
		{
			if ((*button).HandleMouseUp(src))
			{
				Select(&(*button));
				return true;
			}
		}
		return false;
	}

	bool RadioButton::HandleMouseDown(glm::vec2 src)
	{
		for (auto button = radioButtonList.begin(); button != radioButtonList.end(); button++)
		{
			if ((*button).HandleMouseDown(src))
			{
				return true;
			}
		}
		return false;
	}

	void RadioButton::Render()
	{
		for (auto button = radioButtonList.begin(); button != radioButtonList.end(); button++)
		{
			(*button).Render();
		}
	}

	void RadioButton::Select(int i)
	{
		DeselectAll();

		(*this)[i].Select();
		selectedButton = &((*this)[i]);
	}

	void RadioButton::Select(Button* button)
	{
		DeselectAll();

		button->Select();
		selectedButton = button;
	}

	void RadioButton::DeselectAll()
	{
		if (selectedButton != NULL)
		{
			selectedButton->Deselect();
			selectedButton = NULL;
		}
	}

	Button::Button(unsigned int id, glm::vec2 leftTop, glm::vec2 rightBottom) : Component(id)
	{
		this->leftTop = leftTop;
		this->rightBottom = rightBottom;

		textOffset = glm::vec2(0.0f, 5.0f);

		isSelected = false;
		isHovering = false;
		isPressing = false;

		selectingColor = glm::vec3(0.65f, 0.65f, 0.65f);
		hoveringColor = glm::vec3(0.85f, 0.85f, 0.85f);
		pressingColor = glm::vec3(0.58f, 0.58f, 0.58f);
		idleColor = glm::vec3(0.9f, 0.9f, 0.9f);
	}

	Button::Button(unsigned int id, float left, float top, float right, float bottom) : Component(id)
	{
		leftTop = glm::vec2(left, top);
		rightBottom = glm::vec2(right, bottom);

		textOffset = glm::vec2(0.0f, 5.0f);

		isSelected = false;
		isHovering = false;
		isPressing = false;

		selectingColor = glm::vec3(0.65f, 0.65f, 0.65f);
		hoveringColor = glm::vec3(0.85f, 0.85f, 0.85f);
		pressingColor = glm::vec3(0.58f, 0.58f, 0.58f);
		idleColor = glm::vec3(0.9f, 0.9f, 0.9f);
	}

	void Button::Resize(glm::vec2 leftTop, glm::vec2 rightBottom)
	{
		this->leftTop = leftTop;
		this->rightBottom = rightBottom;
	}

	void Button::SetText(std::string text)
	{
		this->text = text;
	}

	bool Button::InComponent(glm::vec2 src)
	{
		if (src.x >= leftTop.x && src.x <= rightBottom.x
			&& src.y >= leftTop.y && src.y <= rightBottom.y)
			return true;
		return false;
	}

	bool Button::HandleMotion(glm::vec2 src)
	{
		if (InComponent(src))
		{
			isHovering = true;
			return true;
		}
		else
		{
			isHovering = false;
			return false;
		}
	}

	bool Button::HandleMouseDown(glm::vec2 src)
	{
		if (InComponent(src))
		{
			isPressing = true;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Button::HandleMouseUp(glm::vec2 src)
	{
		if (isPressing)
		{
			isPressing = false;
			if (InComponent(src))
			{
				Select();
				return true;
			}
			return false;
		}
		else
		{
			return false;
		}
	}

	void Button::SetOnSelectListener(void (*OnSelected)(void))
	{
		this->OnSelected = OnSelected;
	}

	void Button::Select()
	{
		isSelected = true;
		OnSelected();
	}

	void Button::Deselect()
	{
		isSelected = false;
	}

	void Button::Render()
	{
		glLoadIdentity();

		if (isPressing)
			glColor3f(pressingColor.r, pressingColor.g, pressingColor.b);
		else if (isSelected)
			glColor3f(selectingColor.r, selectingColor.g, selectingColor.b);
		else if (isHovering)
			glColor3f(hoveringColor.r, hoveringColor.g, hoveringColor.b);
		else
			glColor3f(idleColor.r, idleColor.g, idleColor.b);

		glm::vec2 pLeftTop = Canvas::Instance().ProjectToScreen(leftTop);
		glm::vec2 pRightBottom = Canvas::Instance().ProjectToScreen(rightBottom);

		glBegin(GL_POLYGON);
		glVertex2f(pLeftTop.x, pLeftTop.y);
		glVertex2f(pLeftTop.x, pRightBottom.y);
		glVertex2f(pRightBottom.x, pRightBottom.y);
		glVertex2f(pRightBottom.x, pLeftTop.y);
		glEnd();

		glColor3f(0.3f, 0.3f, 0.3f);
		glLineWidth(1.0f);

		glBegin(GL_LINE_LOOP);
		glVertex2f(pLeftTop.x, pLeftTop.y);
		glVertex2f(pLeftTop.x, pRightBottom.y);
		glVertex2f(pRightBottom.x, pRightBottom.y);
		glVertex2f(pRightBottom.x, pLeftTop.y);
		glEnd();

		// A simple and rough way to render text
		// Just use temporily
		int lens = text.length();
		glm::vec2 textPosition = glm::vec2((leftTop.x + rightBottom.x) / 2, (leftTop.y + rightBottom.y) / 2)
			- glm::vec2(FONT_WIDTH * lens / 2, 0) + textOffset;
		glm::vec2 pTextPosition;
		for (int i = 0; i < lens; i++)
		{
			pTextPosition = Canvas::Instance().ProjectToScreen(textPosition);
			glRasterPos2f(pTextPosition.x, pTextPosition.y);
			glutBitmapCharacter(FONT_TYPE, text[i]);
			textPosition += glm::vec2(FONT_WIDTH, 0);
		}
	}
}