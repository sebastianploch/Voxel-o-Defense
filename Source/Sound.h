#pragma once


class Sound
{
public:
	// Play sound just once
	static void Fire(const wchar_t* name);

	// Play sound instance
	static void Play(const wchar_t* name,
					 bool loop = false);

	// Resume paused sound instance
	static void Resume(const wchar_t* name);

	// Pause sound instance
	static void Pause(const wchar_t* name);

	// Stop sound instance
	static void Stop(const wchar_t* name);

	// Return SoundInstance of the provided key (name)
	static DirectX::SoundEffectInstance* GetSoundInstance(const wchar_t* name);

	// Check if specified sound is being fired already
	static bool IsFiring(const wchar_t* name);

	// Create sound instance from earlier loaded sound
	static void CreateInstance(const wchar_t* instanceName,
							   const wchar_t* soundName,
							   bool play = false,
							   bool loop = false);

	// Remove sound instance from the stored pool
	static void RemoveInstance(const wchar_t* name);

	// Initialise sounds with sounds contained within "Sounds.json"
	static void InitialiseSounds(DirectX::AudioEngine* audio);

	static inline float GetVolume() { return s_volume; }
	static inline void SetVolume(float volume) { s_volume = volume; }
	static inline void SetInstanceVolume(const wchar_t* name, float volume) { s_instances.at(name)->SetVolume(volume); }

	static inline float GetPitch() { return s_pitch; }
	static inline void SetPitch(float pitch) { s_pitch = pitch; }
	static inline void SetInstancePitch(const wchar_t* name, float pitch) { s_instances.at(name)->SetPitch(pitch); }

	static inline float GetPan() { return s_pan; }
	static inline void SetPan(float pan) { s_pan = pan; }
	static inline void SetInstancePan(const wchar_t* name, float pan) { s_instances.at(name)->SetPan(pan); }

private:
	static void ValidateName(const wchar_t* name);
	static void ValidateInstanceName(const wchar_t* name);

private:
	static float s_pan;
	static float s_pitch;
	static float s_volume;

	static std::map<std::wstring, std::unique_ptr<DirectX::SoundEffect>>		 s_sounds;
	static std::map<std::wstring, std::unique_ptr<DirectX::SoundEffectInstance>> s_instances;
};