#include "GameAPI.h"

#include<list> 

enum SignType
{
	ModName,
	Toggle,
	FloatValue,
	IntValue
};

class Sign 
{
	public:
		wString text;
		wString uniqueName;
		CoordinateInCentimeters position;
		bool display = false;

		HANDLE hintTextHandle;

		Sign(wString text = L"Text not set", wString uniqueName = L"UniqueNameNotSet") {
			this->text = text;
			this->uniqueName = uniqueName;

			ScopedSharedMemoryHandle thisSignHandle = GetSharedMemoryPointer(this->uniqueName, true, false);
			thisSignHandle.Pointer = this;

			if (GetSharedMemoryPointer(L"ModList", false, true).Valid)
			{
				ScopedSharedMemoryHandle modList = GetSharedMemoryPointer(L"ModList", false, true);
				((void (*)(wString))modList.Pointer)(this->uniqueName);
			}
		}

	public:float hitboxSize = 20;
	public:bool CheckCollision(CoordinateInCentimeters handLeft, CoordinateInCentimeters handRight)
	{
		return (handLeft.X < (position.X + hitboxSize) && handLeft.X > (position.X - hitboxSize)
				&& handLeft.Y < (position.Y + hitboxSize) && handLeft.Y >(position.Y - hitboxSize)
				&& handLeft.Z < (position.Z + hitboxSize) && handLeft.Z >(position.Z - hitboxSize)
				|| handRight.X < (position.X + hitboxSize) && handRight.X >(position.X - hitboxSize)
				&& handRight.Y < (position.Y + hitboxSize) && handRight.Y >(position.Y - hitboxSize)
				&& handRight.Z < (position.Z + hitboxSize) && handRight.Z >(position.Z - hitboxSize));
	}
};

class MainSign : public Sign
{
	public:
		bool value = false;
		MainSign(wString text, wString uniqueName, bool value = false) : Sign(text, uniqueName)
		{
			this->value = value;
		}
		bool GetValue() { return value; }
		bool SetValue(bool value) { this->value = value; }
};

class ToggleSign: public Sign
{
	public:
		bool value = false;
		ToggleSign(wString text, wString uniqueName, bool value = false) : Sign(text, uniqueName)
		{
			this->value = value;
		}
		bool GetValue() { return value; }
		bool SetValue(bool value) { this->value = value; }
};

class FloatSign : public Sign
{
	public:
		float value;
		FloatSign(wString text, wString uniqueName, float value = 0) : Sign(text, uniqueName)
		{
			this->value = value;
		}
		float GetValue() { return value; }
		float SetValue(float value) { this->value = value; }
};

class IntSign : public Sign
{
	public:
		int value;
		IntSign(wString text, wString uniqueName, int value = 0) : Sign(text, uniqueName)
		{
			this->value = value;
		}
		int GetValue() { return value; }
		int SetValue(int value) { this->value = value; }
};

class ModMenuGroup
{
	public: 
		Sign main;
		std::list<Sign> options;
		int optionAmountCap = 10;
		ModMenuGroup(Sign modName)
		{
			this->main = modName;
			ScopedSharedMemoryHandle modMenus = GetSharedMemoryPointer(L"ModMenus", false, true);
			((void (*)(ModMenuGroup *))modMenus.Pointer)(this);
		}
		void AddOption(Sign option)
		{
			if(options.size() < optionAmountCap)
				options.push_back(option);
		}
};