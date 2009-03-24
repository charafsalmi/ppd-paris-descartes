#ifndef INPUTCONTROLLER_HPP
#define INPUTCONTROLLER_HPP

#include <SFML/Graphics.hpp>

namespace input
{
	enum Action
	{
		MOVE_UP,
		MOVE_DOWN,
		MOVE_LEFT,
		MOVE_RIGHT,
		USE_ITEM_1,
		USE_ITEM_2,
		USE_ITEM_3,
		SHOW_MINIMAP,
		SHOW_INVENTORY,
		TAKE_SCREENSHOT,
		PANEL_UP,
		PANEL_DOWN,
		PAUSE,
		EXIT_APP,
		_COUNT_ACTION, // do not use
		NONE
	};
}

class InputController
{
public:

	static InputController& GetInstance();

	/**
	 * Dépiler les évènements
	 * @param[out] action: évènement à récupérer
	 * @return true si la pile d'évènements est vide, sinon false
	 */
	bool GetAction(const sf::Event& event, input::Action& action);

	/**
	 * @param[in] action: évènement à tester
	 */
	bool HasInput(input::Action action) const;

	/**
	 * Récupérer le boutondu joystick associée à une action
	 */
	unsigned int GetJoystickBinding(input::Action action) const;

	/**
	 * Récupérer la touche clavier associée à une action
	 */
	sf::Key::Code GetKeyboardBinding(input::Action action) const;

	/**
	 * Associer un bouton du joystick à une action
	 */
	void BindJoystickButton(input::Action action, unsigned int button);

	/**
	 * Associer une touche du clavier à une action
	 */
	void BindKey(input::Action, sf::Key::Code key);

private:
	InputController();
	InputController(const InputController&);
	~InputController();

	sf::Key::Code keyboard_binds_[input::_COUNT_ACTION];

	typedef std::map<input::Action, unsigned int> JoyBindMap;
	mutable JoyBindMap joystick_binds_;
};


#endif // INPUTCONTROLLER_HPP
