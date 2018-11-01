
#include "GameObject3D.h"
#include "Renderer3D.h"

void GameObject3D::Init()
{
}

void GameObject3D::Update()
{
}

void GameObject3D::Render()
{
	Renderer3D::Instance()->Rendering(this);
}
