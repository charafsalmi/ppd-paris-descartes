#include "InputController.hpp"
#include "Game.hpp"

#define JOY_ID			 0
#define JOY_DEADZONE	50.f


using namespace input;


InputController& InputController::GetInstance()
{
    static InputController self;
    return self;
}


InputController::InputController()
{
	// clavier par défaut
	keyboard_binds_[MOVE_UP] = sf::Key::Up;
	keyboard_binds_[MOVE_DOWN] = sf::Key::Down;
	keyboard_binds_[MOVE_LEFT] = sf::Key::Left;
	keyboard_binds_[MOVE_RIGHT] = sf::Key::Right;
	keyboard_binds_[USE_ITEM_1] = sf::Key::Space;
	keyboard_binds_[USE_ITEM_2] = sf::Key::A;
	keyboard_binds_[USE_ITEM_3] = sf::Key::Z;
	keyboard_binds_[SHOW_MINIMAP] = sf::Key::C;
	keyboard_binds_[SHOW_INVENTORY] = sf::Key::I;
	keyboard_binds_[TAKE_SCREENSHOT] = sf::Key::F1;
	keyboard_binds_[PAUSE] = sf::Key::P;
	keyboard_binds_[PANEL_UP] = sf::Key::PageUp;
	keyboard_binds_[PANEL_DOWN] = sf::Key::PageDown;
	keyboard_binds_[EXIT_APP] = sf::Key::Escape;
	// joystick par défaut
	joystick_binds_[SHOW_INVENTORY] = 0;
	joystick_binds_[SHOW_MINIMAP] = 1;
	joystick_binds_[USE_ITEM_1] = 6;
	joystick_binds_[USE_ITEM_2] = 2;
	joystick_binds_[USE_ITEM_3] = 3;
	joystick_binds_[PAUSE] = 4;
}


InputController::~InputController()
{
}


bool InputController::GetAction(const sf::Event& event, Action& action)
{
	action = NONE;

	// CLOSE
	if (event.Type == sf::Event::Closed)
	{
		action = EXIT_APP;
		return true;
	}
	// KEY PRESSED
	if (event.Type == sf::Event::KeyPressed)
	{
		for (int i = 0; i < _COUNT_ACTION; ++i)
		{
			if (event.Key.Code == keyboard_binds_[i])
			{
				action = (Action) i;
				return true;
			}
		}
		return false;
	}
	// JOYBUTTON PRESSED
	else if (event.Type == sf::Event::JoyButtonPressed)
	{
		for (JoyBindMap::const_iterator it = joystick_binds_.begin();
			it != joystick_binds_.end(); ++it)
		{
			if (it->second == event.JoyButton.Button)
			{
				action = it->first;
				return true;
			}
		}
		return false;
	}
	// JOYAXIS MOVED
	else if (event.Type == sf::Event::JoyMoved)
	{
		if (event.JoyMove.Axis == sf::Joy::AxisX)
		{
			if (event.JoyMove.Position > JOY_DEADZONE)
			{
				action = MOVE_RIGHT;
				return true;
			}
			else if (event.JoyMove.Position < -JOY_DEADZONE)
			{
				action = MOVE_LEFT;
				return true;
			}
			return false;
		}
		else if (event.JoyMove.Axis == sf::Joy::AxisY)
		{
			if (event.JoyMove.Position > JOY_DEADZONE)
			{
				action = MOVE_DOWN;
				return true;
			}
			else if (event.JoyMove.Position < -JOY_DEADZONE)
			{
				action = MOVE_UP;
				return true;
			}
			return false;
		}
		return false;
	}
	return false;
}



bool InputController::HasInput(Action action) const
{
	const sf::Input& input = Game::GetInstance().GetApp().GetInput();
	if (input.IsKeyDown(keyboard_binds_[action]))
	{
		return true;
	}
	if (action == MOVE_UP)
	{
		return input.GetJoystickAxis(JOY_ID, sf::Joy::AxisY) < -JOY_DEADZONE;
	}
	if (action == MOVE_DOWN)
	{
		return input.GetJoystickAxis(JOY_ID, sf::Joy::AxisY) > JOY_DEADZONE;
	}
	if (action == MOVE_LEFT)
	{
		return input.GetJoystickAxis(JOY_ID, sf::Joy::AxisX) < -JOY_DEADZONE;
	}
	if (action == MOVE_RIGHT)
	{
		return input.GetJoystickAxis(JOY_ID, sf::Joy::AxisX) > JOY_DEADZONE;
	}
	if (input.IsJoystickButtonDown(JOY_ID, joystick_binds_[action]))
	{
		return true;
	}
	return false;
}


unsigned int InputController::GetJoystickBinding(Action action) const
{
	//TODO
}


sf::Key::Code InputController::GetKeyboardBinding(Action action) const
{
	//TODO
}


void InputController::BindJoystickButton(Action action, unsigned int button)
{
	//TODO
}


void InputController::BindKey(Action, sf::Key::Code key)
{
	//TODO
}



