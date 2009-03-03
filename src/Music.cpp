#ifdef DUMB_MUSIC

#include <cstring>

#include "Music.hpp"

#define DELTA          65536.0f / SAMPLING_RATE


Music::Music(const char* filename)
{
	dumb_register_stdfiles();
	Initialize(N_CHANNELS, SAMPLING_RATE);
	
	const char* ext = strrchr(filename, '.');
	module_ = NULL;
	
	if (strcmp(ext, ".mod") == 0)
	{
		module_ = dumb_load_mod_quick(filename);
	}
	else if (strcmp(ext, ".xm") == 0)
	{
		module_ = dumb_load_xm_quick(filename);
	}
	else if (strcmp(ext, ".s3m") == 0)
	{
		module_ = dumb_load_s3m_quick(filename);
	}
	else if (strcmp(ext, ".it") == 0)
	{
		module_ = dumb_load_it_quick(filename);
	}
	
	if (module_ == NULL)
	{
		printf("[Music] failed to load %s!\n", filename);
	}
	else
	{
		printf("[Music] loading %s\n", filename);
	}
}


Music::~Music()
{
	duh_end_sigrenderer(player_);
	unload_duh(module_);
	dumb_exit();
}


bool Music::OnStart()
{ 
	const int sig = 0;
	const long pos = 0;
	player_ = duh_start_sigrenderer(module_, sig, N_CHANNELS, pos);
	return true;
}


bool Music::OnGetData(Chunk& data)
{
	// FIXME: le /2 est magique [sans lui, skipping]
	duh_render(player_, 16, 0, 1.0f, DELTA, BUFFER_SIZE / 2, samples_);
	data.Samples = samples_;
	data.NbSamples = BUFFER_SIZE; // nombre de samples
	
	return true;
}

#endif /* DUMB_MUSIC */

