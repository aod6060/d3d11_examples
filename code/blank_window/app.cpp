#include "sys.h"


struct AppData
{
	std::string caption;
	uint32_t width;
	uint32_t height;
	IApp* app = nullptr;
	bool running = true;
	SDL_Window* window = nullptr;
};

static AppData data;

void app_init(
	std::string caption, 
	uint32_t width, 
	uint32_t height, 
	IApp* app)
{
	data.caption = caption;
	data.width = width;
	data.height = height;
	data.app = app;

	SDL_Init(SDL_INIT_EVERYTHING);

	data.window = SDL_CreateWindow(
		data.caption.c_str(),
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		data.width,
		data.height,
		SDL_WINDOW_SHOWN
	);

	input_init();

	if (data.app)
	{
		data.app->init();
	}
}

void app_update()
{
	SDL_Event e;

	uint32_t pre = SDL_GetTicks();
	uint32_t curr = 0;
	float delta = 0.0f;

	while (data.running)
	{
		curr = SDL_GetTicks();
		delta = (curr - pre) / 1000.0f;
		pre = curr;

		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				data.running = false;
			}

			input_event(e);
		}

		if (data.app)
		{
			data.app->update(delta);
			data.app->render();
		}

		input_update();
	}
}

void app_release()
{
	if (data.app)
	{
		data.app->release();
	}
	SDL_DestroyWindow(data.window);
	SDL_Quit();
}

std::string app_getCaption()
{
	return data.caption;
}

uint32_t app_getWidth()
{
	return data.width;
}

uint32_t app_getHeight()
{
	return data.height;
}

void app_exit()
{
	data.running = false;
}

SDL_Window* app_getWindow()
{
	return data.window;
}
