#include "CubeMesh.h"

CubeMesh::CubeMesh()
{
	// Bottom Plane 
	AddVertex({ -0.5f,-0.5f,0.f }, { 1.f,0.f,0.f }, { 1.0f,0.f });
	AddVertex({ 0.5f,-0.5f,0.f }, { 0.f,1.f,0.f }, { 0.f,0.f });
	AddVertex({ 0.5f,0.5f,0.f }, { 0.f,0.f,1.f }, { 0.f,1.f });
	AddVertex({ -0.5f,0.5f,0.f }, { 1.f,1.f,1.f }, { 1.f,1.f });


	AddIndex(0);
	AddIndex(1);
	AddIndex(2);
	AddIndex(2);
	AddIndex(3);
	AddIndex(0);

	// Top Plane
	AddVertex({ -0.5f, -0.5f, -0.5f }, { 1.f, 0.f, 0.f }, { 1.0f,0.f });
	AddVertex({ 0.5f,-0.5f,-0.5f }, { 0.f,1.f,0.f }, { 0.f,0.f });
	AddVertex({ 0.5f,0.5f,-0.5f }, { 0.f,0.f,1.f }, { 0.f,1.f });
	AddVertex({ -0.5f,0.5f,-0.5f }, { 1.f,1.f,1.f }, { 1.f,1.f });

	AddIndex(4);
	AddIndex(5);
	AddIndex(6);
	AddIndex(6);
	AddIndex(7);
	AddIndex(4);
}
