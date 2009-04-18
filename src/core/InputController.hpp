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
		_COUNT_ACTION // do not use
	};
}

class InputController
{
public:

	static InputController& GetInstance();

	/**
	 * Transformer un évènement en action
	 * @param event: évènement à tester
	 * @param action: action à récupérer
	 * @return true si l'évènement correspond à une action
	 */
	bool GetAction(const sf::Event& event, input::Action& action);

	/**
	 * @param[in] action: évènement à tester
	 */
	bool HasInput(input::Action action) const;

	/**
	 * Obtenir la touche clavier associée à une action
	 * @param action: action demandée
	 * @return code de la touche associée
	 */
	sf::Key::Code GetKeyboardBinding(input::Action action) const;

	/**
	 * Obtenir le bouton du joystick associé à une action
	 * @param action: action demandée
	 * @return id du bouton associé (-1 si action non gérée par le joystick)
	 */
	unsigned int GetJoystickBinding(input::Action action) const;

	/**
	 * Associer une touche du clavier à une action
	 * @param action: action à lier
	 * @param key: code de la touche à associer
	 */
	void BindKey(input::Action action, sf::Key::Code key);

	/**
	 * Associer un bouton du joystick à une action
	 * @param action: action à lier
	 * @param button: id du bouton à associer
	 */
	void BindJoystickButton(input::Action action, unsigned int button);

private:
	InputController();
	InputController(const InputController&);
	~InputController();

	sf::Key::Code keyboard_binds_[input::_COUNT_ACTION];

	typedef std::map<input::Action, unsigned int> JoystickMap;
	mutable JoystickMap joystick_binds_;
};


#endif // INPUTCONTROLLER_HPP
