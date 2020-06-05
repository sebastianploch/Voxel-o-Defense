#include "pch.h"
#include "VoxelRay.h"
#include "WorldManipulation.h"
#include "Camera.h"
#include <vector>

using namespace DirectX::SimpleMath;

/*
    3D Voxel Traversal Algorithm: J. Amanatides and A. Woo, A Fast Voxel Traversal Algorithm for Ray Tracing, Eurographics, 1987.
    http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.42.3443&rep=rep1&type=pdf

    C++ Implementation by Francis Engelmann
    https://github.com/francisengelmann/fast_voxel_traversal
*/

Vector3Int VoxelRay::VoxelRaycast(Vector3 ray_start, Vector3 ray_end) {
	std::vector<Vector3Int> visited_voxels;

    double _bin_size = 1;

    // This id of the first/current voxel hit by the ray.
  // Using floor (round down) is actually very important,
  // the implicit int-casting will round up for negative numbers.
    Vector3Int current_voxel((int)std::floor(ray_start.x / _bin_size),
                             (int)std::floor(ray_start.y / _bin_size),
                             (int)std::floor(ray_start.z / _bin_size));

    // The id of the last voxel hit by the ray.
    // TODO: what happens if the end point is on a border?
    Vector3Int last_voxel((int)std::floor(ray_end.x / _bin_size),
                          (int)std::floor(ray_end.y / _bin_size),
                          (int)std::floor(ray_end.z / _bin_size));

    // Compute normalized ray direction.
    Vector3 ray = ray_end - ray_start;
    //ray.normalize();

    // In which direction the voxel ids are incremented.
    double stepX = (ray.x >= 0) ? 1 : -1; // correct
    double stepY = (ray.y >= 0) ? 1 : -1; // correct
    double stepZ = (ray.z >= 0) ? 1 : -1; // correct

    // Distance along the ray to the next voxel border from the current position (tMaxX, tMaxY, tMaxZ).
    double next_voxel_boundary_x = (current_voxel.x + stepX) * _bin_size; // correct
    double next_voxel_boundary_y = (current_voxel.y + stepY) * _bin_size; // correct
    double next_voxel_boundary_z = (current_voxel.z + stepZ) * _bin_size; // correct

    // tMaxX, tMaxY, tMaxZ -- distance until next intersection with voxel-border
    // the value of t at which the ray crosses the first vertical voxel boundary
    double tMaxX = (ray.x != 0) ? (next_voxel_boundary_x - ray_start.x) / ray.x : DBL_MAX; //
    double tMaxY = (ray.y != 0) ? (next_voxel_boundary_y - ray_start.y) / ray.y : DBL_MAX; //
    double tMaxZ = (ray.z != 0) ? (next_voxel_boundary_z - ray_start.z) / ray.z : DBL_MAX; //

    // tDeltaX, tDeltaY, tDeltaZ --
    // how far along the ray we must move for the horizontal component to equal the width of a voxel
    // the direction in which we traverse the grid
    // can only be FLT_MAX if we never go in that direction
    double tDeltaX = (ray.x != 0) ? _bin_size / ray.x * stepX : DBL_MAX;
    double tDeltaY = (ray.y != 0) ? _bin_size / ray.y * stepY : DBL_MAX;
    double tDeltaZ = (ray.z != 0) ? _bin_size / ray.z * stepZ : DBL_MAX;

    Vector3Int diff(0, 0, 0);
    bool neg_ray = false;
    if (current_voxel.x != last_voxel.x && ray.x < 0) { diff.x--; neg_ray = true; }
    if (current_voxel.y != last_voxel.y && ray.y < 0) { diff.y--; neg_ray = true; }
    if (current_voxel.z != last_voxel.z && ray.z < 0) { diff.z--; neg_ray = true; }
    visited_voxels.push_back(current_voxel);
    if (neg_ray) {
        current_voxel = current_voxel + diff;
        visited_voxels.push_back(current_voxel);
    }

    while (!(current_voxel.x == last_voxel.x && 
             current_voxel.y == last_voxel.y && 
             current_voxel.z == last_voxel.z)) {
        if (tMaxX < tMaxY) {
            if (tMaxX < tMaxZ) {
                current_voxel.x += (int)stepX;
                tMaxX += tDeltaX;
            } else {
                current_voxel.z += (int)stepZ;
                tMaxZ += tDeltaZ;
            }
        } else {
            if (tMaxY < tMaxZ) {
                current_voxel.y += (int)stepY;
                tMaxY += tDeltaY;
            } else {
                current_voxel.z += (int)stepZ;
                tMaxZ += tDeltaZ;
            }
        }
        visited_voxels.push_back(current_voxel);
    }

    for (Vector3Int& v : visited_voxels) {
        if (WorldManipulation::GetVoxel(v) != VOXEL_TYPE::AIR) {
            return v;   //Traverse list of visit voxels, return first voxel which isn't air
        }
    }

	return DirectX::SimpleMath::Vector3Int();   //If nothing is found, return 0,0,0 vector
}

DirectX::SimpleMath::Vector3Int VoxelRay::VoxelRaycastFromMousePos(Camera* activeCam, InputState* m_inputState, int winWidth, int winHeight) {

    //Get normalised mouse position (-1 to 1)
    auto mState = m_inputState->GetMouse().GetState();
    Vector2 normMousePos = Vector2(mState.x, mState.y) * 2 - Vector2(winWidth, winHeight);
    normMousePos.x /= winWidth;
    normMousePos.y /= winHeight;

    //Convert radians to vector
    Vector2 dir = Vector2(-sin(activeCam->GetYaw()), -cos(activeCam->GetYaw()));

    //Offset by number of blocks on screen
    Vector3 offset = Vector3(activeCam->GetWidth() * -(normMousePos.x / 2.0f),
                             0,
                             activeCam->GetHeight() * -(normMousePos.y * 2.66));


    //Get ray direction from centre of camera
    Vector3 end = activeCam->GetTarget() - activeCam->GetPosition();	//Get normalised direction
    Vector3 start = -end;
    end *= 300;
    start *= 300;
    end += activeCam->GetPosition();	//Reapply the camera position
    start += activeCam->GetPosition();


    //Calculate final offset
    Vector3 finalOffset = Vector3(abs(dir.y) > abs(dir.x) ? offset.x * dir.y : offset.z * dir.x,
                                  0,
                                  abs(dir.y) > abs(dir.x) ? offset.z * dir.y : -offset.x * dir.x);


    //Apply final offset to ray positions
    end += finalOffset;
    start += finalOffset;

    //Cast ray and set voxel
    return VoxelRaycast(start, end);
}
