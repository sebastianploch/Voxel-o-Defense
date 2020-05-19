#pragma once
// C++ Perlin Noise Implementation
// Referenced C# Implementation by Keijiro Takahashi https://github.com/keijiro/PerlinNoise
// http://mrl.nyu.edu/~perlin/noise/


class PerlinNoise {
private:
	static int seed;
	static int perm[];

public:
	static void RandomiseSeed();

	//1D
	static float Noise(float x);		
	//2D
	static float Noise(float x, float y); 
	static float Noise(DirectX::SimpleMath::Vector2 coord);
	//3D
	static float Noise(float x, float y, float z); 
	static float Noise(DirectX::SimpleMath::Vector3 coord);

private:
	static float Fade(float t);
	static float Lerp(float t, float a, float b);
	static float Grad(int hash, float x);
	static float Grad(int hash, float x, float y);
	static float Grad(int hash, float x, float y, float z);
};