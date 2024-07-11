#include "ClientUpdater.h"
#include <GameMath.h>
#include <GameInput.h>
#include <GameTime.h>
#include <GameTexMgr.h>
#include "GameState.h"
#include "Title.h"
#include "Lobby.h"
#include "Level1.h"
#include "Level2.h"
#include "BossRoom.h"
#include "MapEditorScene.h"
#include <GameDic.h>
#include <GameSound.h>
#include "Player.h"


ClientUpdater::ClientUpdater()
{
}


ClientUpdater::~ClientUpdater()
{
}

void ClientUpdater::GameInit()
{
	ResLoad();

	//Init() = Back HDC 만드는 과정
	GameTexMgr::Inst().InitTexMgr();
	//밑에가 가능하긴 한데 하면 바로 릭 남는다.
	//꼭 CreateState로 만들어야 관리를 받게 된다.
	//GameState* test = new Title();
	GameState::CreateState<Title>(L"TITLE");
	GameState::CreateState<Lobby>(L"LOBBY");
	GameState::CreateState<MapEditorScene>(L"MapEditor");
	GameState::CreateState<Level1>(L"LEVEL1");
	GameState::CreateState<Level2>(L"LEVEL2");
	GameState::CreateState<BossRoom>(L"BOSSROOM");
	GameState::ChangeState(L"TITLE");

	GameInput::CreateKey(L"MOVEL", L'A');
	GameInput::CreateKey(L"MOVER", L'D');
	GameInput::CreateKey(L"MOVET", L'W');
	GameInput::CreateKey(L"MOVEB", L'S');

	GameInput::CreateKey(L"PETOnOff", L'Y');

	GameInput::CreateKey(L"IMAGEINDEXPREV", L'Q');
	GameInput::CreateKey(L"IMAGEINDEXNEXT", L'E');

	GameInput::CreateKey(L"LMOUSEBTN", VK_LBUTTON);
	GameInput::CreateKey(L"RMOUSEBTN", VK_RBUTTON);
	GameInput::CreateKey(L"SAVEBTN", VK_F1);
	GameInput::CreateKey(L"CHANGEOBJECTBTN", VK_F2);
	GameInput::CreateKey(L"RevCHANGEOBJECTBTN", VK_F3);

	GameInput::CreateKey(L"PlayerHealthSub", VK_F4);
	GameInput::CreateKey(L"DebugTurn", VK_F8);

	GameInput::CreateKey(L"FIRE", VK_SPACE);
	GameInput::CreateKey(L"ChangeScene", VK_SPACE);

	GameInput::CreateKey(L"MOVELT", L'A', L'W');
	GameInput::CreateKey(L"MOVERT", L'D', L'W');
	GameInput::CreateKey(L"MOVELB", L'A', L'S');
	GameInput::CreateKey(L"MOVERB", L'D', L'S');

	//Rect.Pos = CVector(100.0f, 100.0f);
	//Rect.Size = CVector(100.0f, 100.0f);
}

void ClientUpdater::GameUpdate()
{
	GameState::Progress();
}

void ClientUpdater::ResLoad()
{
	GameDic NewDir;
	NewDir.MoveParent();
	if (!NewDir.IsFile(L"res"))
	{
		ASSERTMSG;
	}
	
	NewDir.Move(L"res");
	NewDir.Move(L"texture");
	NewDir.Move(L"_player");

	std::vector<GameFile> AllTex = NewDir.DirToAllFile();

	for (size_t i = 0; i < AllTex.size(); i++)
	{
		GameTexMgr::Inst().LoadTex(AllTex[i]);
	}

	NewDir.MoveParent();
	NewDir.Move(L"level_use");
	AllTex = NewDir.DirToAllFile();

	for (size_t i = 0; i < AllTex.size(); i++)
	{
		GameTexMgr::Inst().LoadTex(AllTex[i]);
	}

	NewDir.MoveParent();
	NewDir.Move(L"enemy");
	AllTex = NewDir.DirToAllFile();

	for (size_t i = 0; i < AllTex.size(); i++)
	{
		GameTexMgr::Inst().LoadTex(AllTex[i]);
	}

	NewDir.MoveParent();
	NewDir.Move(L"item_choose");
	AllTex = NewDir.DirToAllFile();

	for (size_t i = 0; i < AllTex.size(); i++)
	{
		GameTexMgr::Inst().LoadTex(AllTex[i]);
	}

	NewDir.MoveParent();
	NewDir.Move(L"text_use");
	AllTex = NewDir.DirToAllFile();

	for (size_t i = 0; i < AllTex.size(); i++)
	{
		GameTexMgr::Inst().LoadTex(AllTex[i]);
	}

	NewDir.MoveParent();
	NewDir.Move(L"gui_use");
	AllTex = NewDir.DirToAllFile();

	for (size_t i = 0; i < AllTex.size(); i++)
	{
		GameTexMgr::Inst().LoadTex(AllTex[i]);
	}

	NewDir.MoveParent();
	NewDir.Move(L"swipes_use");
	AllTex = NewDir.DirToAllFile();

	for (size_t i = 0; i < AllTex.size(); i++)
	{
		GameTexMgr::Inst().LoadTex(AllTex[i]);
	}
	// C:\Users\이의진\OneDrive\바탕 화면\ProjRepo\Dancer\GameFramework\res\sound
	NewDir.MoveParent();
	NewDir.MoveParent();
	NewDir.Move(L"sound");

	std::vector<GameFile> AllFile = NewDir.DirToAllFile();

	for (size_t i = 0; i < AllFile.size(); i++)
	{
		GameSound::SoundLoad(AllFile[i]);
	}


	GameImage* Gold1 = GameTexMgr::Inst().CreateImage(L"resource_coin1.bmp", 1, 2);
	GameImage* Gold2 = GameTexMgr::Inst().CreateImage(L"resource_coin2.bmp", 1, 2);
	GameImage* Gold3 = GameTexMgr::Inst().CreateImage(L"resource_coin3.bmp", 1, 2);
	GameImage* Gold4 = GameTexMgr::Inst().CreateImage(L"resource_coin4.bmp", 1, 2);
	GameImage* Gold5 = GameTexMgr::Inst().CreateImage(L"resource_coin5.bmp", 1, 2);
	GameImage* Gold6 = GameTexMgr::Inst().CreateImage(L"resource_coin6.bmp", 1, 2);
	GameImage* Gold7 = GameTexMgr::Inst().CreateImage(L"resource_coin7.bmp", 1, 2);
	GameImage* Gold8 = GameTexMgr::Inst().CreateImage(L"resource_coin8.bmp", 1, 2);
	GameImage* Gold9 = GameTexMgr::Inst().CreateImage(L"resource_coin9.bmp", 1, 2);
	GameImage* Gold10 = GameTexMgr::Inst().CreateImage(L"resource_coin10.bmp", 1, 2);

	GameImage* BackGround = GameTexMgr::Inst().CreateImage(L"TEMP_overlay_black.bmp", 1, 1);
	GameImage* EnemySwipe = GameTexMgr::Inst().CreateImage(L"swipe_enemy.bmp", 5, 1);
	GameImage* AttackRed = GameTexMgr::Inst().CreateImage(L"Attack_red.bmp", 1, 1);
	GameImage* AttackRedSh = GameTexMgr::Inst().CreateImage(L"Attack_red_shadow.bmp", 1, 1);
	GameImage* ShopFloor = GameTexMgr::Inst().CreateImage(L"shop_floor.bmp", 3, 1);

	GameImage* BSwordDown = GameTexMgr::Inst().CreateImage(L"swipe_broadsword_Down.bmp", 1, 3);
	GameImage* BSwordLeft = GameTexMgr::Inst().CreateImage(L"swipe_broadsword_Left.bmp", 3, 1);
	GameImage* BSwordRight = GameTexMgr::Inst().CreateImage(L"swipe_broadsword_Right.bmp", 3, 1);
	GameImage* BSwordUp = GameTexMgr::Inst().CreateImage(L"swipe_broadsword_Up.bmp", 1, 3);

	GameImage* DaggerDown = GameTexMgr::Inst().CreateImage(L"swipe_dagger_Down.bmp", 1, 3);
	GameImage* DaggerLeft = GameTexMgr::Inst().CreateImage(L"swipe_dagger_Left.bmp", 3, 1);
	GameImage* DaggerRight = GameTexMgr::Inst().CreateImage(L"swipe_dagger_Right.bmp", 3, 1);
	GameImage* DaggerUp = GameTexMgr::Inst().CreateImage(L"swipe_dagger_Up.bmp", 1, 3);

	GameImage* LSwordDown = GameTexMgr::Inst().CreateImage(L"swipe_longsword_Down.bmp", 1, 4);
	GameImage* LSwordLeft = GameTexMgr::Inst().CreateImage(L"swipe_longsword_Left.bmp", 4, 1);
	GameImage* LSwordRight = GameTexMgr::Inst().CreateImage(L"swipe_longsword_Right.bmp", 4, 1);
	GameImage* LSwordUp = GameTexMgr::Inst().CreateImage(L"swipe_longsword_Up.bmp", 1, 4);

	GameImage* RapierDown = GameTexMgr::Inst().CreateImage(L"swipe_rapier_Down.bmp", 1, 4);
	GameImage* RapierLeft = GameTexMgr::Inst().CreateImage(L"swipe_rapier_Left.bmp", 4, 1);
	GameImage* RapierRight = GameTexMgr::Inst().CreateImage(L"swipe_rapier_Right.bmp", 4, 1);
	GameImage* RapierUp = GameTexMgr::Inst().CreateImage(L"swipe_rapier_Up.bmp", 1, 4);

	GameImage* zombie = GameTexMgr::Inst().CreateImage(L"zombie.bmp", 24, 2);
	GameImage* addChest = GameTexMgr::Inst().CreateImage(L"addchest_black.bmp", 1, 2);

	GameImage* drum1 = GameTexMgr::Inst().CreateImage(L"coralriff_drums.bmp", 4, 2);
	GameImage* drum2 = GameTexMgr::Inst().CreateImage(L"coralriff_drums2.bmp",4, 2);
	GameImage* Coral = GameTexMgr::Inst().CreateImage(L"coralriff_head.bmp", 4, 2);
	GameImage* horn1 = GameTexMgr::Inst().CreateImage(L"coralriff_horns.bmp", 4, 2);
	GameImage* horn2 = GameTexMgr::Inst().CreateImage(L"coralriff_horns2.bmp", 4, 2);
	GameImage* keytar = GameTexMgr::Inst().CreateImage(L"coralriff_keytar.bmp", 4, 2);
	GameImage* keytar2 = GameTexMgr::Inst().CreateImage(L"coralriff_keytar2.bmp", 4, 2);
	GameImage* string = GameTexMgr::Inst().CreateImage(L"coralriff_strings.bmp", 4, 2);
	GameImage* string2 = GameTexMgr::Inst().CreateImage(L"coralriff_strings2.bmp", 4, 2);

	GameImage* smallHeartImg = GameTexMgr::Inst().CreateImage(L"smallHeart.bmp", 2, 1);
	GameImage* Dirt = GameTexMgr::Inst().CreateImage(L"jump_dirt.bmp", 5, 1);
	GameImage* shopKeeper = GameTexMgr::Inst().CreateImage(L"shopkeeper.bmp", 8, 2);
	GameImage* TitleShadowImage = GameTexMgr::Inst().CreateImage(L"TILESH.bmp", 1, 1);
	GameImage* WallShadowImage = GameTexMgr::Inst().CreateImage(L"WallShadow.bmp", 1, 2);
	GameImage* playerImage = GameTexMgr::Inst().CreateImage(L"player1_heads.bmp", 4, 2);
	GameImage* tileImage = GameTexMgr::Inst().CreateImage(L"floor_dirt1.bmp", 3, 2);
	GameImage* armorImage = GameTexMgr::Inst().CreateImage(L"player1_armor_body_xmas.bmp", 4, 5);
	GameImage* wallImage = GameTexMgr::Inst().CreateImage(L"TEMP_wall_unbreakable.bmp", 1, 2);
	GameImage* stonewallImage = GameTexMgr::Inst().CreateImage(L"wall_stone_crypt.bmp", 1, 2);
	GameImage* stonewallcrackImage = GameTexMgr::Inst().CreateImage(L"zone1_wall_stone_cracked.bmp", 1, 2);
	GameImage* cryptwallImage = GameTexMgr::Inst().CreateImage(L"wall_dirt_crypt.bmp", 16, 2);
	GameImage* shopWallImage = GameTexMgr::Inst().CreateImage(L"wall_shop_crypt.bmp", 1, 2);
	GameImage* doorFrontImage = GameTexMgr::Inst().CreateImage(L"door_front.bmp", 1, 1);
	GameImage* doorSideImage = GameTexMgr::Inst().CreateImage(L"door_side.bmp", 1, 1);
	GameImage* torchImage = GameTexMgr::Inst().CreateImage(L"wall_torch.bmp", 4, 1);
	GameImage* dirtWallCrackImage = GameTexMgr::Inst().CreateImage(L"zone1_wall_dirt_cracked.bmp", 1, 2);
	GameImage* dirtWall_Three_Image = GameTexMgr::Inst().CreateImage(L"wall_dirt_crypt_diamond4.bmp", 3, 2);
	GameImage* greenSlimeImage = GameTexMgr::Inst().CreateImage(L"slime_green.bmp", 4, 2);
	GameImage* blueSlimeImage = GameTexMgr::Inst().CreateImage(L"slime_blue.bmp", 8, 2);
	GameImage* batImage = GameTexMgr::Inst().CreateImage(L"bat.bmp", 4, 2);
	GameImage* skeletonImage = GameTexMgr::Inst().CreateImage(L"skeleton.bmp", 9, 2);
	GameImage* minotaurImage = GameTexMgr::Inst().CreateImage(L"minotaur.bmp", 9, 2);
	GameImage* monkeyImage = GameTexMgr::Inst().CreateImage(L"monkey.bmp", 6, 2);
	GameImage* dragonImage = GameTexMgr::Inst().CreateImage(L"dragon_green.bmp", 2, 2);
	GameImage* stairImage = GameTexMgr::Inst().CreateImage(L"stairs.bmp", 1, 1);
	GameImage* stairLockImage = GameTexMgr::Inst().CreateImage(L"stairs_locked_miniboss.bmp", 1, 1);

	GameImage* bossFloor = GameTexMgr::Inst().CreateImage(L"boss_floor_A.bmp", 4, 2);
	GameImage* waterFloor = GameTexMgr::Inst().CreateImage(L"TEMP_floor_water.bmp", 3, 1);

	GameImage* shovelImage = GameTexMgr::Inst().CreateImage(L"shovel_basic.bmp", 1, 2);
	GameImage* daggerlImage = GameTexMgr::Inst().CreateImage(L"weapon_dagger.bmp", 1, 2);
	GameImage* LongSwordImage = GameTexMgr::Inst().CreateImage(L"weapon_longsword.bmp", 1, 2);
	GameImage* rapierImage = GameTexMgr::Inst().CreateImage(L"weapon_rapier.bmp", 1, 2);
	GameImage* BroadImage = GameTexMgr::Inst().CreateImage(L"weapon_broadsword.bmp", 1, 2);
	GameImage* headblastlImage = GameTexMgr::Inst().CreateImage(L"head_blast_helm.bmp", 1, 2);
	GameImage* spikeLeaderImage = GameTexMgr::Inst().CreateImage(L"armor_spiked_leather.bmp", 1, 2);
	GameImage* textImage = GameTexMgr::Inst().CreateImage(L"necrosans_12_black.bmp", 5, 9);
	GameImage* debugTileImage = GameTexMgr::Inst().CreateImage(L"DebugTile.bmp", 2, 1);
	GameImage* fixsTileImage = GameTexMgr::Inst().CreateImage(L"floor_dirt_fix.bmp", 3, 2);
	GameImage* diamondImage = GameTexMgr::Inst().CreateImage(L"diamond.bmp", 1, 1);
	GameImage* heartImage = GameTexMgr::Inst().CreateImage(L"heart_ani.bmp", 2, 1);
	GameImage* heartemptyImage = GameTexMgr::Inst().CreateImage(L"heart_empty.bmp", 1, 1);
	GameImage* hearthalfImage = GameTexMgr::Inst().CreateImage(L"heart_half.bmp", 1, 1);
	GameImage* hudCoinImage = GameTexMgr::Inst().CreateImage(L"hud_coins.bmp", 1, 1);
	GameImage* hudActionSlotImage = GameTexMgr::Inst().CreateImage(L"hud_slot_action1.bmp", 1, 1);
	GameImage* hudreloadSlotImage = GameTexMgr::Inst().CreateImage(L"hud_slot_reload.bmp", 1, 1);
	GameImage* scrollImage = GameTexMgr::Inst().CreateImage(L"scroll_black.bmp", 1, 2);
	GameImage* hudSlotImage = GameTexMgr::Inst().CreateImage(L"hud_slot_1.bmp", 1, 1);
	GameImage* BombImage = GameTexMgr::Inst().CreateImage(L"bomb.bmp", 5, 2);
	GameImage* beatHeartImage = GameTexMgr::Inst().CreateImage(L"TEMP_beat_heart.bmp", 2, 1);
	GameImage* beatMarkerImage = GameTexMgr::Inst().CreateImage(L"TEMP_beat_marker.bmp", 1, 1);
	GameImage* beatMarkerSHImage = GameTexMgr::Inst().CreateImage(L"TEMP_beat_markerShadow.bmp", 1, 1);
	GameImage* bossWallImage = GameTexMgr::Inst().CreateImage(L"boss_wall.bmp", 5, 2);
	GameImage* alphatestImage = GameTexMgr::Inst().CreateImage(L"wall_stone_alpha_test2.bmp", 1, 2);
	GameImage* titleImage = GameTexMgr::Inst().CreateImage(L"mainmenu.bmp", 1, 1);
	GameImage* continueImage = GameTexMgr::Inst().CreateImage(L"continue.bmp", 1, 1);
	//이렇게 잘라놓은 이미지는 나중에 Actor가 가지고 있는 Renderer의 Image() 함수로 찾아올 수 있음

}

