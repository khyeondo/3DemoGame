#pragma once

class GameObject
{
public:
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
protected:
	GameObject() {}
	virtual ~GameObject() {}
};