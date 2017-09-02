#include <stdio.h>  /* printf */
#include <dlfcn.h>  /* dlopen, dlsym, dlerror, dlclose */

#include <sys/stat.h> /* stat */

#include "hot_main.h"

static const char *hot_main_path = "hot_main.dylib";

typedef int error_t;

typedef struct
{
	void *hot_main;
	init_t *init;
	main_loop_t *main_loop;
} LoadHotMainResult;

static time_t get_hot_main_last_write_time()
{
	time_t last_write_time = 0;

	struct stat file_stat;
	if (stat(hot_main_path, &file_stat))
	{
		printf("Unable to read file stats for %s.\n", hot_main_path);
		return 0;
	}
	else
	{
		return file_stat.st_mtimespec.tv_sec;
	}
}

static int hot_main_changed()
{
	static time_t last_write_time = 0;
	if (!last_write_time)
	{
		last_write_time = get_hot_main_last_write_time();
	}
	time_t new_write_time = get_hot_main_last_write_time();
	if (new_write_time > last_write_time)
	{
		last_write_time = new_write_time;
		return 1;
	}
	else
	{
		return 0;
	}
}

static error_t load_hot_main(LoadHotMainResult *result)
{
	result->hot_main = dlopen(hot_main_path, RTLD_LAZY | RTLD_GLOBAL);
	if (!result->hot_main)
	{
		printf("Unable to open %s:\n%s\n", hot_main_path, dlerror());
		return 1;
	}
	result->init = dlsym(result->hot_main, "init");
	if (!result->init)
	{
		printf("Unable to link init:\n%s\n", dlerror());
		return 1;
	}
	result->main_loop = dlsym(result->hot_main, "main_loop");
	if (!result->main_loop)
	{
		printf("Unable to link main_loop:\n%s\n", dlerror());
		return 1;
	}
	return 0;
}

static error_t unload_hot_main(LoadHotMainResult *result)
{
	if (dlclose(result->hot_main))
	{
		printf("unable to close %s:\n%s\n", hot_main_path, dlerror());
		return 1;
	}
	*result = (LoadHotMainResult){0, 0, 0};
	return 0;
}

error_t main()
{
	// Load `init` and `main_loop` from `hot_main.dylib`
	LoadHotMainResult hot_main = {0, 0, 0};
	if (load_hot_main(&hot_main))
	{
		// Exit on failure
		return 1;
	}
	get_hot_main_last_write_time(); // initialize the lase write time

	// Call `init` to allocate and initialize the persistent state
	void *state = 0;
	hot_main.init(&state);

	int hot_swap = 0;
	while (hot_main.main_loop(state, &hot_swap))
	{
		if (hot_swap)
		{
			printf("Attempting hot swap.\n");
			if (hot_main_changed())
			{
				printf("Attempting to load changed module.\n");
				if (unload_hot_main(&hot_main))
				{
					printf("Failed to unload old module.\n");
					return 1;
				}
				else
				{
					if (load_hot_main(&hot_main))
					{
						printf("Failed to load new module.\n");
						return 1;
					}
					else
					{
						printf("Hot swap complete.\n");
					}
				}
			}
			else
			{
				printf("No changes to swap.\n");
			}
		}
	}

	return 0;
}