#include "src/WinMain.h"
#include "src/System/Vector3.h"
#include "src/System/Transform.h"

Transform::Transform()
{
	// ’l‚Ì‰Šú‰»
	pos.set(0.0f, 0.0f, 0.0f);
	rot.set(0.0f, 0.0f, 0.0f);
	scale.set(0.0f, 0.0f, 0.0f);
}

Transform::~Transform()
{
}
