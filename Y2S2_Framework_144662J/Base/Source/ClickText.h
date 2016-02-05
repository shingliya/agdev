#pragma once

#include <string>
#include "Vector3.h"

class ClickText
{
public:
	ClickText();
	ClickText(std::string name, float size, float x, float y);
	~ClickText();
	
	std::string GetName();
	float GetSize();
	float GetX();
	float GetY();
	bool GetMouseover();
	void CheckMouseOver(float x, float y);
	
private:
	std::string name;
	float size;
	Vector3 min, max;

	bool mouseover;
};