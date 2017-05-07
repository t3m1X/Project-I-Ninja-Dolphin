#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "SDL/include/SDL.h"



ModuleInput::ModuleInput() : Module()
{}

// Destructor
ModuleInput::~ModuleInput()
{}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0) {
		LOG("SDL_JOYSTICK could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (SDL_InitSubSystem(SDL_INIT_HAPTIC) < 0) {
		LOG("SDL_HAPTIC could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	for (int i = 0; i < MAX_CONTROLLERS; ++i)
		controllers[i] = nullptr;

	int NumJoysticks = SDL_NumJoysticks();
	int connected_controllers = 0;

	for (int i = 0; i < NumJoysticks; ++i) {
		if (SDL_IsGameController(i)) {
			controllers[connected_controllers++] = new Controller(SDL_GameControllerOpen(i), SDL_HapticOpen(i), i);
			if (connected_controllers == MAX_CONTROLLERS)
				break;
		}
	}

	for (int i = 0; i < MAX_KEYS; ++i)
		keyboard[i] = KEY_IDLE;

	return ret;
}

// Called every draw update
update_status ModuleInput::Update()
{
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i) {
		if (keys[i] == 1) {
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else {
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for (int pad = 0; pad < MAX_CONTROLLERS; ++pad) {
		if (controllers[pad] == nullptr)
			continue;

		for (int i = 0; i < CONTROLLER_BUTTONS; ++i) {
			if (controllers[pad]->buttons[i] == KEY_DOWN)
				controllers[pad]->buttons[i] = KEY_REPEAT;
			if (controllers[pad]->buttons[i] == KEY_UP)
				controllers[pad]->buttons[i] = KEY_IDLE;
		}
	}

	if (keyboard[SDL_SCANCODE_ESCAPE])
		return update_status::UPDATE_STOP;

	SDL_Event event;
	while (SDL_PollEvent(&event) != 0) {
		switch (event.type) {
		case SDL_CONTROLLERDEVICEADDED:
			if (SDL_IsGameController(event.cdevice.which)) {
				int index = event.cdevice.which;
				for (int i = 0; i < MAX_CONTROLLERS; ++i) {
					if (controllers[i] == nullptr)
						controllers[i] = new Controller(SDL_GameControllerOpen(index),SDL_HapticOpen(index), index);
				}
			}
			break;
		case SDL_CONTROLLERDEVICEREMOVED:
			for (int i = 0; i < MAX_CONTROLLERS; ++i) {
				if (controllers[i]->index == event.cdevice.which) {
					SDL_HapticClose(controllers[i]->haptic);
					SDL_GameControllerClose(controllers[i]->controller);
					delete[] controllers[i];
					controllers[i] = nullptr;
				}
			}
			break;

		case SDL_CONTROLLERBUTTONDOWN:
			for (int i = 0; i < MAX_CONTROLLERS; ++i) {
				if (controllers[i] == nullptr)
					continue;
				if (controllers[i]->index == event.cdevice.which) {
					controllers[i]->buttons[event.cbutton.button] = KEY_DOWN;
					break;
				}
			}
			break;

		case SDL_CONTROLLERBUTTONUP:
			for (int i = 0; i < MAX_CONTROLLERS; ++i) {
				if (controllers[i] == nullptr)
					continue;
				if (controllers[i]->index == event.cdevice.which) {
					controllers[i]->buttons[event.cbutton.button] = KEY_UP;
					break;
				}
			}
			break;

		case SDL_CONTROLLERAXISMOTION:
			for (int i = 0; i < MAX_CONTROLLERS; ++i) {
				if (controllers[i] == nullptr)
					continue;
				if (controllers[i]->index == event.cdevice.which) {
					if (event.caxis.axis == SDL_CONTROLLER_AXIS_INVALID)
						break;
					else {
						int value;
						if (event.caxis.value > 0)
							value = event.caxis.value / 3276.7f;
						else if (event.caxis.value < 0)
							value = event.caxis.value / 3276.8f;
						else
							value = event.caxis.value;
						controllers[i]->axes[event.caxis.axis] = value / 10.0f;
					}
					break;
				}
			}
			break;
		case SDL_QUIT:
			return update_status::UPDATE_STOP;
		}
	}

	return update_status::UPDATE_CONTINUE;
}

KEY_STATE ModuleInput::GetControllerButton(int player, SDL_GameControllerButton button){
	if (player > 2) {
		LOG("GetControllerButton: Wrong player\n")
		return KEY_IDLE;
	}
	if (controllers[player - 1] != nullptr) 
		return controllers[player - 1]->buttons[button];
}

float ModuleInput::GetControllerAxis(int player, SDL_GameControllerAxis axis) {
	if (player > 2) {
		LOG("GetControllerButton: Wrong player\n")
			return 0;
	}
	if (controllers[player - 1] != nullptr)
		return controllers[player - 1]->axes[axis];
}

bool ModuleInput::HasController(int player) 	{
	if (player > 2)
		return false;
	else
		return controllers[player - 1] != nullptr;
}


void ModuleInput::ShakeController(int player, int time, float strength){
	if (player > 2)
		return;
	if (controllers[player - 1] != nullptr) {
		if (SDL_HapticRumbleInit(controllers[player - 1]->haptic) == -1)
			LOG("Cannot init rumble for controller number %d", player);
		SDL_HapticRumblePlay(controllers[player - 1]->haptic, strength, time);
	}

}

// Called before quitting
bool ModuleInput::CleanUp()
{
	for (int i = 0; i < MAX_CONTROLLERS; ++i) {
		if (controllers[i] != nullptr) {
			SDL_HapticRumbleStop(controllers[i]->haptic);
			SDL_HapticClose(controllers[i]->haptic);
			SDL_GameControllerClose(controllers[i]->controller);
			delete[] controllers[i];
			controllers[i] = nullptr;
		}
	}
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC);
	return true;
}