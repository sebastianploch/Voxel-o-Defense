#include "pch.h"

using namespace DirectX;

float Sound::s_pan = 0.0f;
float Sound::s_pitch = 0.0f;
float Sound::s_volume = 1.0f;
std::map<std::wstring, std::unique_ptr<DirectX::SoundEffect>> Sound::s_sounds;
std::map<std::wstring, std::unique_ptr<DirectX::SoundEffectInstance>> Sound::s_instances;


void Sound::Fire(const wchar_t* name)
{
	ValidateName(name);

	s_sounds.at(name)->Play(s_volume, s_pitch, s_pan);
}

void Sound::Play(const wchar_t* name,
				 bool loop)
{
	ValidateInstanceName(name);

	s_instances.at(name)->Play(loop);
}

void Sound::Resume(const wchar_t* name)
{
	ValidateInstanceName(name);

	s_instances.at(name)->Resume();
}

void Sound::Pause(const wchar_t* name)
{
	ValidateInstanceName(name);

	s_instances.at(name)->Pause();
}

void Sound::Stop(const wchar_t* name)
{
	ValidateInstanceName(name);

	s_instances.at(name)->Stop();
}

DirectX::SoundEffectInstance* Sound::GetSoundInstance(const wchar_t* name)
{
	ValidateInstanceName(name);

	return s_instances.at(name).get();
}

bool Sound::IsFiring(const wchar_t* name)
{
	ValidateName(name);

	return s_sounds.at(name)->IsInUse();
}

void Sound::CreateInstance(const wchar_t* instanceName,
						   const wchar_t* soundName,
						   bool play,
						   bool loop)
{
	ValidateName(soundName);

	s_instances.emplace(instanceName, s_sounds.at(soundName)->CreateInstance());

	if (play)
		s_instances.at(instanceName)->Play(loop);
}

void Sound::RemoveInstance(const wchar_t* name)
{
	ValidateInstanceName(name);

	if (s_instances.at(name)->GetState() == SoundState::PLAYING)
		s_instances.at(name)->Stop();

	s_instances.erase(name);
}

void Sound::InitialiseSounds(DirectX::AudioEngine* audio)
{
	SoundsCfg config;
	JSONLoader::LoadFile("Resources/Config/Sounds.json", config, "sounds");

	for (auto i = config.amount - 1; i >= 0.0f; --i)
	{
		// Extract path
		std::wstring path = TO_WSTRING(config.paths[i]);

		// Extract name from the path
		std::wstring name = path.substr(path.rfind('/') + 1);
		name.erase(name.rfind('.'), 4);

		s_sounds.emplace(name, std::make_unique<SoundEffect>(SoundEffect(audio, path.c_str())));
	}
}

void Sound::ValidateName(const wchar_t* name)
{
	/* If it failed here, that means you are trying to use non-existent name for the sound
	   Verify that the sound name is correct (same as JSON) */
	assert(s_sounds.find(name) != s_sounds.end());
	UNREFERENCED_PARAMETER(name);
}

void Sound::ValidateInstanceName(const wchar_t* name)
{
	/* If it failed here, that means you are trying to use non-existent name for the sound instance
	   Verify that the sound instance name is correct) */
	assert(s_instances.find(name) != s_instances.end());
	UNREFERENCED_PARAMETER(name);
}