#include "sys.h"




struct InputData
{
	// Keyboard
	InputState keyboard[KEYBOARD_SIZE];
	// Mouse
	InputState mouseButton[MB_SIZE];
	int32_t x = 0;
	int32_t y = 0;
	// Grab
};

static InputData data;

static void keyDown(SDL_KeyboardEvent& e);
static void keyUp(SDL_KeyboardEvent& e);
static void mbDown(SDL_MouseButtonEvent& e);
static void mbUp(SDL_MouseButtonEvent& e);
static void mbMotion(SDL_MouseMotionEvent& e);

void input_init()
{
	// Keyboard
	for (uint32_t i = 0; i < KEYBOARD_SIZE; i++)
	{
		data.keyboard[i] = InputState::IS_RELEASE;
	}
	// MouseButton
	for (uint32_t i = 0; i < MB_SIZE; i++)
	{
		data.mouseButton[i] = InputState::IS_RELEASE;
	}
	// MouseMotion
}

void input_event(SDL_Event& e)
{
	switch (e.type)
	{
	case SDL_KEYDOWN:
		keyDown(e.key);
		break;
	case SDL_KEYUP:
		keyUp(e.key);
		break;
	case SDL_MOUSEBUTTONDOWN:
		mbDown(e.button);
		break;
	case SDL_MOUSEBUTTONUP:
		mbUp(e.button);
		break;
	case SDL_MOUSEMOTION:
		mbMotion(e.motion);
		break;
	}
}

void input_update()
{
	// Keyboard
	for (uint32_t i = 0; i < KEYBOARD_SIZE; i++)
	{
		if (data.keyboard[i] == InputState::IS_DOWN)
		{
			data.keyboard[i] = InputState::IS_PRESS;
		}

		if (data.keyboard[i] == InputState::IS_UP)
		{
			data.keyboard[i] = InputState::IS_RELEASE;
		}
	}
	// Mouse Button
	for (uint32_t i = 0; i < MB_SIZE; i++)
	{
		if (data.mouseButton[i] == InputState::IS_DOWN)
		{
			data.mouseButton[i] = InputState::IS_PRESS;
		}
		if (data.mouseButton[i] == InputState::IS_UP)
		{
			data.mouseButton[i] = InputState::IS_RELEASE;
		}
	}
	// Mouse Motion
	if (input_isGrab())
	{
		data.x = 0;
		data.y = 0;
	}
}

bool input_isKeyRelease(const Keyboard& key)
{
	return data.keyboard[key] == InputState::IS_RELEASE;
}

bool input_isKeyDown(const Keyboard& key)
{
	return data.keyboard[key] == InputState::IS_DOWN;
}

bool input_isKeyPress(const Keyboard& key)
{
	return data.keyboard[key] == InputState::IS_PRESS;
}

bool input_isKeyUp(const Keyboard& key)
{
	return data.keyboard[key] == InputState::IS_UP;
}

bool input_isMouseButtonRelease(const MouseButtons& mb)
{
	return data.mouseButton[mb] == InputState::IS_RELEASE;
}

bool input_isMouseButtonDown(const MouseButtons& mb)
{
	return data.mouseButton[mb] == InputState::IS_DOWN;
}

bool input_isMouseButtonPress(const MouseButtons& mb)
{
	return data.mouseButton[mb] == InputState::IS_PRESS;
}

bool input_isMouseButtonUp(const MouseButtons& mb)
{
	return data.mouseButton[mb] == InputState::IS_UP;
}

void input_getPosition(int32_t& x, int32_t& y)
{
	x = data.x;
	y = data.y;
}

bool input_isGrab()
{
	return (SDL_GetRelativeMouseMode()) ? true : false;
}

void input_setGrab(bool grab)
{
	SDL_SetRelativeMouseMode((grab) ? SDL_TRUE : SDL_FALSE);
}

void input_toggleGrab()
{
	input_setGrab(!input_isGrab());
}

static void keyDown(SDL_KeyboardEvent& e)
{
	if (data.keyboard[e.keysym.scancode] == IS_RELEASE)
	{
		data.keyboard[e.keysym.scancode] = IS_DOWN;
	}
}

static void keyUp(SDL_KeyboardEvent& e)
{
	if (data.keyboard[e.keysym.scancode] == IS_PRESS)
	{
		data.keyboard[e.keysym.scancode] = IS_UP;
	}
}

static void mbDown(SDL_MouseButtonEvent& e)
{
	uint32_t button = e.button - 1;
	if (data.mouseButton[button] == IS_RELEASE)
	{
		data.mouseButton[button] = IS_DOWN;
	}
}

static void mbUp(SDL_MouseButtonEvent& e)
{
	uint32_t button = e.button - 1;
	if (data.mouseButton[button] == IS_PRESS)
	{
		data.mouseButton[button] = IS_UP;
	}
}

static void mbMotion(SDL_MouseMotionEvent& e)
{
	if (input_isGrab())
	{
		data.x = e.xrel;
		data.y = e.yrel;
	}
	else
	{
		data.x = e.x;
		data.y = e.y;
	}
}
