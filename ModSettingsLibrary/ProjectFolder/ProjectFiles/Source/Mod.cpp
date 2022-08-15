#include "GameAPI.h"
#include "ModInterface.cpp"

#include<iostream> 

#include<list> 

/************************************************************
	Config Variables (Set these to whatever you need. They are automatically read by the game.)
*************************************************************/

UniqueID ThisModUniqueIDs[] = { 0 }; // All the UniqueIDs this mod manages. Functions like Event_BlockPlaced are only called for blocks of IDs mentioned here. 

float TickRate = 4;							 // Set how many times per second Event_Tick() is called. 0 means the Event_Tick() function is never called.

/*************************************************************
//	Functions (Run automatically by the game, you can put any code you want into them)
*************************************************************/

// Run every time a block is placed
void Event_BlockPlaced(CoordinateInBlocks At, UniqueID CustomBlockID, bool Moved)
{

}

// Run every time a block is destroyed
void Event_BlockDestroyed(CoordinateInBlocks At, UniqueID CustomBlockID, bool Moved)
{

}


/*
*	Run every time a block is hit by a tool
*	All the exact possible ToolName are: T_Stick, T_Arrow, T_Pickaxe_Stone, T_Axe_Stone, T_Shovel_Stone, T_Pickaxe_Copper, T_Axe_Copper, T_Shovel_Copper, T_Sledgehammer_Copper, T_Pickaxe_Iron, T_Axe_Iron, T_Shovel_Iron, T_Sledgehammer_Iron
*/
void Event_BlockHitByTool(CoordinateInBlocks At, UniqueID CustomBlockID, wString ToolName, CoordinateInCentimeters ExactHitLocation, bool ToolHeldByHandLeft)
{

	if (ToolName == L"T_Stick") {

	}
	else if (ToolName == L"T_Pickaxe_Stone") {

	}
	else if (ToolName == L"T_Arrow") {

	}
	else {

	}
}

float AxisDistance(float pos1, float pos2)
{
	return abs(pos1 - pos2);
}

CoordinateInCentimeters CentimeterAxisDistances(CoordinateInCentimeters coord1, CoordinateInCentimeters coord2)
{
	return CoordinateInCentimeters(AxisDistance(coord1.X, coord2.X), AxisDistance(coord1.Y, coord2.Y), AxisDistance(coord1.Z, coord2.Z));
}

bool HandsToHead(CoordinateInCentimeters handLeft, CoordinateInCentimeters handRight, CoordinateInCentimeters headPos)
{
	float hitboxSize = 20;
	
	return (handLeft.X < (headPos.X + hitboxSize) && handLeft.X > (headPos.X - hitboxSize)
		&& handLeft.Y < (headPos.Y + hitboxSize) && handLeft.Y >(headPos.Y - hitboxSize)
		&& handLeft.Z < (headPos.Z + hitboxSize) && handLeft.Z >(headPos.Z - hitboxSize)
		&& handRight.X < (headPos.X + hitboxSize) && handRight.X >(headPos.X - hitboxSize)
		&& handRight.Y < (headPos.Y + hitboxSize) && handRight.Y >(headPos.Y - hitboxSize)
		&& handRight.Z < (headPos.Z + hitboxSize) && handRight.Z >(headPos.Z - hitboxSize));


}

bool menuOpen = false;
bool menuOpenStart = false;

bool modSignHandRemoved = true;

//Test Create Menu
Sign closeMenu = ToggleSign(L"Close Menu");

std::list<ModMenuGroup> modMenus;
std::list<ModMenuGroup>::iterator modMenusIt;

void CloseMenu()
{
	menuOpen = false;
	DestroyHintText(closeMenu.hintTextHandle);
	
	for (modMenusIt = modMenus.begin(); modMenusIt != modMenus.end(); modMenusIt++)
	{
		DestroyHintText(modMenusIt->main.hintTextHandle);
	}
}

// Run X times per second, as specified in the TickRate variable at the top
void Event_Tick()
{
	CoordinateInCentimeters handLeftPos = GetHandLocation(true);
	CoordinateInCentimeters handRightPos = GetHandLocation(false);

	CoordinateInCentimeters headPos = GetPlayerLocationHead();
	DirectionVectorInCentimeters headDir = GetPlayerViewDirection();

	
	if (!menuOpen)
	{
		if (HandsToHead(handLeftPos, handRightPos, headPos))
		{
			PlayHapticFeedbackOnHand(true, 0.1f, 20, 0.25f);
			menuOpen = true;
			closeMenu.display = true;
			closeMenu.position = headPos + CoordinateInCentimeters(0, 0, 20) + (headDir * 35);
			closeMenu.hintTextHandle = SpawnHintTextAdvanced(closeMenu.position, closeMenu.text, -1, 0.5f);
			for (modMenusIt = modMenus.begin(); modMenusIt != modMenus.end(); modMenusIt++)
			{
				modMenusIt->main.display = true;
				modMenusIt->main.position = headPos + CoordinateInCentimeters(modMenus.size() * modMenusIt->main.hitboxSize, 0, 0) + (headDir * 35);
				modMenusIt->main.hintTextHandle = SpawnHintTextAdvanced(modMenusIt->main.position, modMenusIt->main.text, -1, 0.5f);
			}
		}
	}
	if (menuOpen)
	{
		
		if (closeMenu.CheckCollision(handLeftPos, handRightPos))
		{
			CloseMenu();
		}
	}
}



void SetModMenus(ModMenuGroup mod)
{
	Log(mod.main.text + L" SetModMenus Attempt: (Mod Menus)");
	modMenus.push_back(mod);
	Log(mod.main.text + L" was set : (Mod Menus)");
}

// Run once when the world is loaded
void Event_OnLoad(bool CreatedNewWorld)
{
	Log(L"ModSettings Load start (Mod Menus)");
	ScopedSharedMemoryHandle setModMenusHandle = GetSharedMemoryPointer(L"ModMenus", true, false);
	Log(L"ModSettings Mod Menus Shared Memory Handle Created (Mod Menus)");
	setModMenusHandle.Pointer = SetModMenus;
	Log(L"ModSettings Mod Menus Pointer Set (Mod Menus)");

	ModMenuGroup mainMenu = ModMenuGroup(ToggleSign(L"Close Menu"));
}

// Run once when the world is exited
void Event_OnExit()
{

}

/*******************************************************

	Advanced functions

*******************************************************/


// Run every time any block is placed by the player
void Event_AnyBlockPlaced(CoordinateInBlocks At, BlockInfo Type, bool Moved)
{

}

// Run every time any block is destroyed by the player
void Event_AnyBlockDestroyed(CoordinateInBlocks At, BlockInfo Type, bool Moved)
{

}

/*
*	Run every time any block is hit by a tool
*	All the exact possible ToolName are: T_Stick, T_Arrow, T_Pickaxe_Stone, T_Axe_Stone, T_Shovel_Stone, T_Pickaxe_Copper, T_Axe_Copper, T_Shovel_Copper, T_Sledgehammer_Copper, T_Pickaxe_Iron, T_Axe_Iron, T_Shovel_Iron, T_Sledgehammer_Iron
*/
void Event_AnyBlockHitByTool(CoordinateInBlocks At, BlockInfo Type, wString ToolName, CoordinateInCentimeters ExactHitLocation, bool ToolHeldByHandLeft)
{

}


/*******************************************************

	For all the available game functions you can call, look at the GameAPI.h file

*******************************************************/