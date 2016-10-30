#pragma once

class LuaGuiObject
{
public:
	LuaGuiObject();

	RefCountedPtr<LuaVector2> position;
	RefCountedPtr<LuaVector2> size;

	// Methods

	RefCountedPtr<LuaVector2> GetPosition() const;
	RefCountedPtr<LuaVector2> GetSize() const;

	void SetPosition(const RefCountedPtr<LuaVector2> &pos);
	void SetSize(const RefCountedPtr<LuaVector2> &size);

private:
	virtual void DoPaintTraverse() = 0;
	virtual void DoCreateMove() = 0;
};