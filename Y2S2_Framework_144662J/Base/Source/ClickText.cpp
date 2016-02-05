#include "ClickText.h"

ClickText::ClickText()
{
	name = "";
	size = 1;
	min.Set(0, 0, 0);
	max.Set(0, 0, 0);
	mouseover = false;
}

ClickText::ClickText(std::string name, float size, float x, float y)
{
	this->name = name;
	this->size = size;
	min.Set(x, y, 0);
	max.Set(x + name.size()*(size/2.2), y + size/1.3, 0);
	mouseover = false;
}

ClickText::~ClickText()
{
}

void ClickText::CheckMouseOver(float x, float y)
{
	if (x > min.x && x < max.x && y > min.y && y < max.y)
		mouseover = true;
	else
		mouseover = false;
}

std::string ClickText::GetName()
{
	return name;
}

float ClickText::GetSize()
{
	return size;
}

float ClickText::GetX()
{
	return min.x;
}

float ClickText::GetY()
{
	return min.y;
}

bool ClickText::GetMouseover()
{
	return mouseover;
}