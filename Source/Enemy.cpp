#pragma once
#include "pch.h"
#include "Mesh.h"
#include "Model.h"
#include "Enemy.h"
#include <iostream>
#include <vector>
#include <unordered_map>

// Ignore 'unscoped enum' warning
#pragma warning(disable : 26812)

std::vector<Mesh> eZombie::zombieMeshes = {};
std::vector<Mesh> eSkeleton::skeletonMeshes = {};
std::vector<Mesh> eSpider::spiderMeshes = {};