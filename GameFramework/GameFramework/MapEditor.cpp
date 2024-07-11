//#include "stdafx.h"
#include "MapEditor.h"
#include <GamePath.h>
#include <GameInput.h>
#include <GameDic.h>
#include <GameWin.h>
#include "GameWall.h"
#include "GameDoor.h"
#include "Torch.h"
#include "GreenSlime.h"
#include "BlueSlime.h"
#include "Bat.h"
#include "Skeleton.h"
#include "Minotaur.h"
#include "Monkey.h"
#include "Dragon.h"
#include "Stair.h"
#include "Text.h"
#include "GameRender.h"
#include "GameMap.h"
#include "GameBackWall.h"
#include "GameState.h"
#include "Weapon.h"
#include "ARMOR.h"
#include "Helmet.h"
#include "ShopKeeper.h"
#include "StairLock.h"
#include "ShopFloor.h"
#include "ShopItem.h"
#include "Zombie.h"
#include "CoralHead.h"
#include "Drum.h"
#include "Horn.h"
#include "KeyTar.h"
#include "MonString.h"
#include "Chest.h"

MapEditor::MapEditor(std::wstring _SaveFile, int _TileNameIndex /*= 0*/) : m_GameDir(), SaveFileName(_SaveFile), m_WaterTileMake(false)
{
	m_WallName[0] = L"wall_shop_crypt.bmp";
	m_WallName[1] = L"zone1_wall_dirt_cracked.bmp";
	m_WallName[2] = L"wall_dirt_crypt.bmp";
	m_WallName[3] = L"wall_stone_crypt.bmp";
	m_WallName[4] = L"zone1_wall_stone_cracked.bmp";
	m_WallName[5] = L"wall_dirt_crypt.bmp";
	m_WallName[6] = L"TEMP_wall_unbreakable.bmp";
	m_WallName[7] = L"boss_wall.bmp";

	m_TileName[0] = L"floor_dirt1.bmp";
	m_TileName[1] = L"boss_floor_A.bmp";
	m_TileNameIndex = _TileNameIndex;
	Pos({ m_TilePadding / 2 , m_TilePadding / 2 });
	TilemapInit();
	//InitUI();
	m_GameDir.MoveParent();
	m_GameDir.Move(L"BIN");
	State()->SetEditor(this);

}


void MapEditor::Update()
{
	if (GameInput::Press(L"SAVEBTN"))
	{
		std::wstring SavePath = m_GameDir.CFullPath() + L"\\" + SaveFileName.c_str();
		GameFile SaveFile = GameFile(SavePath.c_str(), L"wb");

		//바닥 저장
		{
			SaveFile.Write(static_cast<int>(TileRenderer->m_AllTile.size()));
			auto StartIter = TileRenderer->m_AllTile.begin();
			auto EndIter = TileRenderer->m_AllTile.end();
			CVector::MapIndex tempindex;
			int xPos = 0;
			int yPos = 0;

			for (; StartIter != EndIter; ++StartIter)
			{
				tempindex.Index = (*StartIter).first;
				xPos = tempindex.X;
				yPos = tempindex.Y;
				SaveFile.Write(xPos);
				SaveFile.Write(yPos);
			}

		}
		//저장해야 할 것 
		//1. 벽의 갯수
		//2. 벽의 위치
		//3. 단일 비트맵 벽 저장하기
		{
			SaveFile.Write(static_cast<int>(m_ListMapActor.size()));
			auto IterStart = m_ListMapActor.begin();
			auto IterEnd = m_ListMapActor.end();
			std::wstring tempString;
			CVector::MapIndex savePosIndex;
			int tempType = 0;
			for (; IterStart != IterEnd; ++IterStart)
			{
				switch ((*IterStart)->ActorType())
				{
				case EActorTypeIndex::Wall: {
					//다운캐스팅
					GameWall* NewWall = static_cast<GameWall*>(*IterStart);
					tempType = static_cast<int>(NewWall->m_ActorTypeIndex);
					SaveFile.Write(tempType);
					tempString = NewWall->m_Wall->GetImageName()->c_str();
					SaveFile.Write(tempString);
					savePosIndex = NewWall->m_Wall->SubPosIndeX();
					SaveFile.Write(savePosIndex);
					SaveFile.Write(NewWall->m_Wall->Index());
					break;
				}

				case EActorTypeIndex::BackWall:
				{
					GameBackWall* NewBackWall = static_cast<GameBackWall*>(*IterStart);
					tempType = static_cast<int>(NewBackWall->m_ActorTypeIndex);
					SaveFile.Write(tempType);
					tempString = NewBackWall->m_BackWall->GetImageName()->c_str();
					SaveFile.Write(tempString);
					savePosIndex = NewBackWall->m_BackWall->SubPosIndeX();
					SaveFile.Write(savePosIndex);
					SaveFile.Write(NewBackWall->m_BackWall->Index());
					break;
				}
				case EActorTypeIndex::Door:
				{
					GameDoor* NewDoor = static_cast<GameDoor*>(*IterStart);
					tempType = static_cast<int>(NewDoor->m_ActorTypeIndex);
					SaveFile.Write(tempType);
					SaveFile.Write(NewDoor->m_bIsSide);
					savePosIndex = NewDoor->m_Door->SubPosIndeX();
					SaveFile.Write(savePosIndex);
					break;
				}
				case EActorTypeIndex::ShopFloor:
				{
					ShopFloor* NewStair = static_cast<ShopFloor*>(*IterStart);
					tempType = static_cast<int>(NewStair->m_ActorTypeIndex);
					SaveFile.Write(tempType);
					savePosIndex = NewStair->m_ShopFloor->SubPosIndeX();
					SaveFile.Write(savePosIndex);
					break;
				}
				case EActorTypeIndex::Stair:
				{
					Stair* NewStair = static_cast<Stair*>(*IterStart);
					tempType = static_cast<int>(NewStair->m_ActorTypeIndex);
					SaveFile.Write(tempType);
					savePosIndex = NewStair->m_Stair->SubPosIndeX();
					SaveFile.Write(savePosIndex);
					break;
				}
				case EActorTypeIndex::StairLock:
				{
					StairLock* NewStair = static_cast<StairLock*>(*IterStart);
					tempType = static_cast<int>(NewStair->m_ActorTypeIndex);
					SaveFile.Write(tempType);
					savePosIndex = NewStair->m_Stair->SubPosIndeX();
					SaveFile.Write(savePosIndex);
					break;
				}
				case EActorTypeIndex::ShopItem:
				{
					ShopItem* NewStair = static_cast<ShopItem*>(*IterStart);
					tempType = static_cast<int>(NewStair->m_ActorTypeIndex);
					SaveFile.Write(tempType);
					savePosIndex = CalcIndex(NewStair->m_InitPos);
					SaveFile.Write(savePosIndex);
					break;
				}
				case EActorTypeIndex::Weapon:
				{
					Weapon* NewWeapon = static_cast<Weapon*>(*IterStart);
					tempType = static_cast<int>(NewWeapon->m_ActorTypeIndex);
					SaveFile.Write(tempType);
					savePosIndex = NewWeapon->m_Weapon->SubPosIndeX();
					SaveFile.Write(savePosIndex);
					tempType = (int)(NewWeapon->m_WeaponType);
					SaveFile.Write(tempType);
					break;
				}
				case EActorTypeIndex::Armor:
				{
					Armor* NewArmor = static_cast<Armor*>(*IterStart);
					tempType = static_cast<int>(NewArmor->m_ActorTypeIndex);
					SaveFile.Write(tempType);
					savePosIndex = NewArmor->m_Armor->SubPosIndeX();
					SaveFile.Write(savePosIndex);
					break;
				}
				case EActorTypeIndex::Helmet:
				{
					Helmet* NewHelmet = static_cast<Helmet*>(*IterStart);
					tempType = static_cast<int>(NewHelmet->m_ActorTypeIndex);
					SaveFile.Write(tempType);
					savePosIndex = NewHelmet->m_Helmet->SubPosIndeX();
					SaveFile.Write(savePosIndex);
					break;
				}
				case EActorTypeIndex::Walltorch:
				{
					Torch* NewTorch = static_cast<Torch*>(*IterStart);
					tempType = static_cast<int>(NewTorch->m_ActorTypeIndex);
					SaveFile.Write(tempType);
					savePosIndex = NewTorch->m_Torch->SubPosIndeX();
					SaveFile.Write(savePosIndex);
					break;
				}
				case EActorTypeIndex::ShopKeeper:
				{
					ShopKeeper* NewKeeper = static_cast<ShopKeeper*>(*IterStart);
					tempType = static_cast<int>(NewKeeper->m_ActorTypeIndex);
					SaveFile.Write(tempType);
					savePosIndex = NewKeeper->m_Keeper->SubPosIndeX();
					SaveFile.Write(savePosIndex);
					break;
				}
				case EActorTypeIndex::Chest:
				{
					Chest* NewChest = static_cast<Chest*>(*IterStart);
					tempType = static_cast<int>(NewChest->m_ActorTypeIndex);
					SaveFile.Write(tempType);
					int tempInt = NewChest->InWeapon();
					SaveFile.Write(tempInt);
					savePosIndex = NewChest->m_Chest->SubPosIndeX();
					SaveFile.Write(savePosIndex);
					break;
				}
				case EActorTypeIndex::Text:
				{
					Text* NewText = static_cast<Text*>(*IterStart);
					tempType = static_cast<int>(NewText->m_ActorTypeIndex);
					SaveFile.Write(tempType);
					tempString = NewText->GetText();
					SaveFile.Write(tempString);
					savePosIndex = NewText->m_TextPos;
					SaveFile.Write(savePosIndex);

					break;
				}
				default:
					break;
				}
			}

		}
		{
			//몬스터 저장하기
			SaveFile.Write(static_cast<int>(m_ListMonster.size()));
			auto IterStart = m_ListMonster.begin();
			auto IterEnd = m_ListMonster.end();
			CVector::MapIndex savePosIndex = { 0,0 };

			int tempIndex = 0;
			for (; IterStart != IterEnd; ++IterStart)
			{
				tempIndex = static_cast<int>((*IterStart)->m_ActorTypeIndex);
				auto renderIter = (*IterStart)->m_AllRenderList.begin();
				savePosIndex = (*IterStart)->MonsterPosIndex();

				SaveFile.Write(tempIndex);
				SaveFile.Write(savePosIndex);
			}
		}

		//물바닥 저장
		{
			if (WaterTileRenderer != nullptr)
			{
				SaveFile.Write(static_cast<int>(WaterTileRenderer->m_AllTile.size()));
				auto StartIter = WaterTileRenderer->m_AllTile.begin();
				auto EndIter = WaterTileRenderer->m_AllTile.end();
				CVector::MapIndex tempindex;
				int xPos = 0;
				int yPos = 0;

				for (; StartIter != EndIter; ++StartIter)
				{
					tempindex.Index = (*StartIter).first;
					xPos = tempindex.X;
					yPos = tempindex.Y;
					SaveFile.Write(xPos);
					SaveFile.Write(yPos);
				}
			}
			

		}
	}


	if (GameInput::Down(L"CHANGEOBJECTBTN"))
	{
		if (static_cast<int>(m_EObjectdata) >= static_cast<int>(ObjectData::Max) - 1)
		{
			m_EObjectdata = ObjectData::tile;
			swprintf_s(buff, L"Createtile\n");
			OutputDebugStringW(buff);
		}
		else
		{
			m_EObjectdata = static_cast<ObjectData>(static_cast<int>(m_EObjectdata) + 1);
			CheckObjectType();
		}

	}


	if (true == GameInput::Down(L"RevCHANGEOBJECTBTN"))
	{
		if (static_cast<int>(m_EObjectdata) < 0)
		{
			m_EObjectdata = ObjectData::Max;
			swprintf_s(buff, L"Max dont create \n");
			OutputDebugStringW(buff);
		}
		else
		{
			m_EObjectdata = static_cast<ObjectData>(static_cast<int>(m_EObjectdata) - 1);
			CheckObjectType();
		}

	}

	m_MousePos = GameWin::MainWindow()->MousePos();
	//X,Y좌표 얻기
	m_CalcMousePos = CVector({ std::round((m_MousePos.X + State()->CamPos().X) / m_TileSize)
						   , std::round((m_MousePos.Y + State()->CamPos().Y) / m_TileSize) });
	//얻은 좌표에 타일 크기 곱하기
	m_CalcMousePos *= m_TileSize;


	if (true == GameInput::Press(L"LMOUSEBTN"))
	{
		TileRenderer->AddTile(GameWin::MainWindow()->MousePos(), 0/*스프라이트 인덱스*/);;
	}


	if (true == GameInput::Down(L"RMOUSEBTN"))
	{
		CVector::MapIndex calcIndex = { static_cast<int>(m_CalcMousePos.X / m_TileSize), static_cast<int>(m_CalcMousePos.Y / m_TileSize) };
		std::map<__int64, GameActor*>::iterator findMonster = State()->m_MonsterMap.find(calcIndex.Index);
		std::map<__int64, GameActor*>::iterator endMonster = State()->m_MonsterMap.end();

		std::map<__int64, GameActor*>::iterator findItem = State()->m_InterObjMap.find(calcIndex.Index);
		std::map<__int64, GameActor*>::iterator endItem = State()->m_InterObjMap.end();

		std::map<__int64, GameActor*>::iterator findWall = State()->m_WallActorMap.find(calcIndex.Index);
		std::map<__int64, GameActor*>::iterator endWall = State()->m_WallActorMap.end();

		switch (m_EObjectdata)
		{
		case ObjectData::tile:
		{
			TileRenderer->AddTile(GameWin::MainWindow()->MousePos(), 0/*스프라이트 인덱스*/);
			break;
		}
		case ObjectData::wall:
		{
			if (findWall == endWall)
			{
				GameWall* NewWall = new GameWall(m_CalcMousePos, m_WallName[0]);
				GameBackWall* NewBackWall = new GameBackWall(m_CalcMousePos, m_WallName[0]);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewWall));
				m_ListMapActor.push_back(static_cast<GameActor*>(NewBackWall));
			}
			break;

		}
		case ObjectData::door:
		{
			if (findItem == endItem)
			{
				GameDoor* NewDoor = new GameDoor(m_CalcMousePos, false);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewDoor));
			}
			break;
		}
		case ObjectData::doorSide:
		{
			if (findItem == endItem)
			{
				GameDoor* NewDoor = new GameDoor(m_CalcMousePos, true);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewDoor));
			}
			break;
		}
		case ObjectData::stair:
		{
			if (findItem == endItem)
			{
				Stair* NewStair = new Stair(m_CalcMousePos);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewStair));
			}
			break;
		}
		case ObjectData::stairlock:
		{
			if (findItem == endItem)
			{
				StairLock* NewStair = new StairLock(m_CalcMousePos);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewStair));
			}
			break;
		}

		case ObjectData::shopFloor:
		{
			if (findItem == endItem)
			{
				ShopFloor* NewStair = new ShopFloor(m_CalcMousePos);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewStair));
			}
			break;
		}
		case ObjectData::walltorch:
		{
			if (findItem == endItem)
			{
				Torch* NewTorch = new Torch(m_CalcMousePos);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewTorch));
			}
			break;
		}
		case ObjectData::waterTile:
		{
			if (!m_WaterTileMake)
			{
				m_WaterTileMake = true;
				m_WaterTileMap = new GameMap(EActorTypeIndex::WaterTile);
				WaterTileRenderer = m_TileMap->TileRen();
				WaterTileRenderer->Image(L"TEMP_floor_water.bmp");
				WaterTileRenderer->m_AllTile.begin();
				State()->m_WaterTileRender = WaterTileRenderer;
			}

			WaterTileRenderer->AddTile(GameWin::MainWindow()->MousePos(), 0);

			break;
		}
		case ObjectData::dirtWallCrack:
		{
			if (findWall == endWall)
			{
				GameWall* NewWall = new GameWall(m_CalcMousePos, m_WallName[1]);
				GameBackWall* NewBackWall = new GameBackWall(m_CalcMousePos, m_WallName[1]);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewWall));
				m_ListMapActor.push_back(static_cast<GameActor*>(NewBackWall));
			}
			break;
		}
		case ObjectData::dirtWall:
		{
			if (findWall == endWall)
			{
				GameWall* NewWall = new GameWall(m_CalcMousePos, m_WallName[2]);
				GameBackWall* NewBackWall = new GameBackWall(m_CalcMousePos, m_WallName[2]);
				NewWall->ChangeIndex(0);
				NewBackWall->ChangeIndex(17);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewWall));
				m_ListMapActor.push_back(static_cast<GameActor*>(NewBackWall));
			}
			break;
		}
		case ObjectData::cryptWall:
		{
			if (findWall == endWall)
			{
				GameWall* NewWall = new GameWall(m_CalcMousePos, m_WallName[5]);
				GameBackWall* NewBackWall = new GameBackWall(m_CalcMousePos, m_WallName[5]);
				int random = CVector::RANDOM.RandomMinMax(2, 15);
				NewWall->ChangeIndex(random);
				NewBackWall->ChangeIndex(random + 17); // 17+index
				m_ListMapActor.push_back(static_cast<GameActor*>(NewWall));
				m_ListMapActor.push_back(static_cast<GameActor*>(NewBackWall));
			}
			break;
		}

		case ObjectData::dirtWallNestItem:
		{
			if (findWall == endWall)
			{
				GameWall* NewWall = new GameWall(m_CalcMousePos, m_WallName[2]);
				GameBackWall* NewBackWall = new GameBackWall(m_CalcMousePos, m_WallName[2]);
				NewWall->ChangeIndex(1);
				NewBackWall->ChangeIndex(3);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewWall));
				m_ListMapActor.push_back(static_cast<GameActor*>(NewBackWall));
			}
			break;
		}
		case ObjectData::dirtWallNestDia:
		{
			if (findWall == endWall)
			{
				GameWall* NewWall = new GameWall(m_CalcMousePos, m_WallName[2]);
				GameBackWall* NewBackWall = new GameBackWall(m_CalcMousePos, m_WallName[2]);
				NewWall->ChangeIndex(2);
				NewBackWall->ChangeIndex(3);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewWall));
				m_ListMapActor.push_back(static_cast<GameActor*>(NewBackWall));
			}
			break;
		}
		case ObjectData::stoneWall:
		{
			if (findWall == endWall)
			{
				GameWall* NewWall = new GameWall(m_CalcMousePos, m_WallName[3]);
				GameBackWall* NewBackWall = new GameBackWall(m_CalcMousePos, m_WallName[3]);
				NewWall->ChangeIndex(0);
				NewBackWall->ChangeIndex(1);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewWall));
				m_ListMapActor.push_back(static_cast<GameActor*>(NewBackWall));
			}
			break;
		}

		case ObjectData::stoneWallCrack:
		{
			if (findWall == endWall)
			{
				GameWall* NewWall = new GameWall(m_CalcMousePos, m_WallName[4]);
				GameBackWall* NewBackWall = new GameBackWall(m_CalcMousePos, m_WallName[4]);
				NewWall->ChangeIndex(0);
				NewBackWall->ChangeIndex(1);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewWall));
				m_ListMapActor.push_back(static_cast<GameActor*>(NewBackWall));
			}
			break;
		}

		case ObjectData::bossWall:
		{
			if (findWall == endWall)
			{
				GameWall* NewWall = new GameWall(m_CalcMousePos, m_WallName[6]);
				GameBackWall* NewBackWall = new GameBackWall(m_CalcMousePos, m_WallName[6]);
				NewWall->ChangeIndex(0);
				NewBackWall->ChangeIndex(1);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewWall));
				m_ListMapActor.push_back(static_cast<GameActor*>(NewBackWall));
			}
			break;
		}

		case ObjectData::unbreakable:
		{
			if (findWall == endWall)
			{
				GameWall* NewWall = new GameWall(m_CalcMousePos, m_WallName[7]);
				GameBackWall* NewBackWall = new GameBackWall(m_CalcMousePos, m_WallName[7]);
				int random = CVector::RANDOM.RandomMinMax(0, 4);
				NewWall->ChangeIndex(random);
				NewBackWall->ChangeIndex(random + 5);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewWall));
				m_ListMapActor.push_back(static_cast<GameActor*>(NewBackWall));
			}
			break;
		}

		case ObjectData::chest:
		{
			if (findItem == endItem)
			{
				Chest* NewWeapon = new Chest(m_CalcMousePos, 1);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewWeapon));
			}
			break;
		}

		case ObjectData::chest2:
		{
			if (findItem == endItem)
			{
				Chest* NewWeapon = new Chest(m_CalcMousePos, 2);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewWeapon));
			}
			break;
		}

		case ObjectData::weapon:
		{
			if (findItem == endItem)
			{
				Weapon* NewWeapon = new Weapon(m_CalcMousePos, Weapon::WeaponType::BroadSword);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewWeapon));
			}
			break;
		}


		case ObjectData::armor:
		{
			if (findItem == endItem)
			{
				Armor* NewWeapon = new Armor(m_CalcMousePos);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewWeapon));
			}
			break;
		}

		case ObjectData::helmet:
		{
			if (findItem == endItem)
			{
				Helmet* NewWeapon = new Helmet(m_CalcMousePos);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewWeapon));
			}
			break;
		}


		case ObjectData::shopKeeper:
		{
			if (findItem == endItem)
			{
				ShopKeeper* NewKeeper = new ShopKeeper(m_CalcMousePos);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewKeeper));
			}
			break;
		}

		case ObjectData::shopItem:
		{
			if (findItem == endItem)
			{
				ShopItem* NewKeeper = new ShopItem(m_CalcMousePos);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewKeeper));
			}
			break;
		}

		case ObjectData::greenSlime:
		{
			if (findMonster == endMonster)
			{
				GreenSlime* NewMonster = new GreenSlime(m_CalcMousePos);
				m_ListMonster.push_back(static_cast<Monster*>(NewMonster));
			}
			break;
		}
		case ObjectData::blueSlime:
		{
			if (findMonster == endMonster)
			{
				BlueSlime* NewMonster = new BlueSlime(m_CalcMousePos);
				m_ListMonster.push_back(static_cast<Monster*>(NewMonster));
			}
			break;
		}
		case ObjectData::bat:
		{
			if (findMonster == endMonster)
			{
				Bat* NewMonster = new Bat(m_CalcMousePos);
				m_ListMonster.push_back(static_cast<Monster*>(NewMonster));
			}
			break;
		}
		case ObjectData::skeleton:
		{
			if (findMonster == endMonster)
			{
				Skeleton* NewMonster = new Skeleton(m_CalcMousePos);
				m_ListMonster.push_back(static_cast<Monster*>(NewMonster));
			}
			break;
		}

		case ObjectData::coralhead:
		{
			if (findMonster == endMonster)
			{
				CoralHead* NewMonster = new CoralHead(m_CalcMousePos);
				m_ListMonster.push_back(static_cast<Monster*>(NewMonster));
			}
			break;
		}

		case ObjectData::coraldrum:
		{
			if (findMonster == endMonster)
			{
				Drum* NewMonster = new Drum(m_CalcMousePos);
				m_ListMonster.push_back(static_cast<Monster*>(NewMonster));
			}
			break;
		}

		case ObjectData::coralhorn:
		{
			if (findMonster == endMonster)
			{
				Horn* NewMonster = new Horn(m_CalcMousePos);
				m_ListMonster.push_back(static_cast<Monster*>(NewMonster));
			}
			break;
		}

		case ObjectData::coratkeytar:
		{
			if (findMonster == endMonster)
			{
				Keytar* NewMonster = new Keytar(m_CalcMousePos);
				m_ListMonster.push_back(static_cast<Monster*>(NewMonster));
			}
			break;
		}

		case ObjectData::coralstring:
		{
			if (findMonster == endMonster)
			{
				MonString* NewMonster = new MonString(m_CalcMousePos);
				m_ListMonster.push_back(static_cast<Monster*>(NewMonster));
			}
			break;
		}

		case ObjectData::zombie:
		{
			if (findMonster == endMonster)
			{
				Zombie* NewMonster = new Zombie(m_CalcMousePos);
				m_ListMonster.push_back(static_cast<Monster*>(NewMonster));
			}
			break;
		}
		case ObjectData::minotaur:
		{
			if (findMonster == endMonster)
			{
				Minotaur* NewMonster = new Minotaur(m_CalcMousePos);
				m_ListMonster.push_back(static_cast<Monster*>(NewMonster));
			}
			break;
		}
		case ObjectData::monkey:
		{
			if (findMonster == endMonster)
			{
				Monkey* NewMonster = new Monkey(m_CalcMousePos);
				m_ListMonster.push_back(static_cast<Monster*>(NewMonster));
			}
			break;
		}
		case ObjectData::dragon:
		{
			if (findMonster == endMonster)
			{
				Dragon* NewMonster = new Dragon(m_CalcMousePos);
				m_ListMonster.push_back(static_cast<Monster*>(NewMonster));
			}
			break;
		}
		case ObjectData::text_HelloWorld:
		{
			std::wstring newText = L"Zone1";
			m_CalcMousePos.X += 4;
			Text* NewText = new Text(m_CalcMousePos, { 8.0f, 10.0f }, newText);
			m_ListMapActor.push_back(static_cast<GameActor*>(NewText));
			break;
		}

		default:
			ASSERTMSG;
			break;
		}
	}
}


MapEditor::~MapEditor()
{
}



void MapEditor::TilemapInit()
{
	int tempindex = 0;
	//각 스테이지를 이름으로 저장하고 불러올땐 이런식으로 불러오도록 하면 될듯
	m_GameDir.MoveParent();
	m_GameDir.Move(L"BIN");
	std::wstring SavePath = m_GameDir.CFullPath();
	SavePath.append(L"\\");
	SavePath.append(SaveFileName);

	if (false == GamePath::IsExist(SavePath.c_str()))
	{
		m_TileMap = new GameMap;
		TileRenderer = m_TileMap->TileRen();
		TileRenderer->Image(L"DebugTile.bmp");
		State()->m_TileRender = TileRenderer;
	}
	else
	{

		int tempintData;
		bool bIsSide = false;
		tempindex = 0;
		CVector tempVector;
		std::wstring tempName;
		int objectCount = 0;
		CVector::MapIndex loadPosIndex = { 0,0 };

		//바닥 로딩해오기
		m_TileMap = new GameMap;
		TileRenderer = m_TileMap->TileRen();
		TileRenderer->Image(m_TileName[m_TileNameIndex].c_str());
		TileRenderer->m_AllTile.begin();
		State()->m_TileRender = TileRenderer;

		GameFile LoadData = GameFile(SavePath.c_str(), L"rb");
		LoadData.Read(objectCount);

		int xPos = 0;
		int yPos = 0;
		for (int i = 0; i < objectCount; ++i)
		{
			LoadData.Read(xPos);
			LoadData.Read(yPos);
			TileRenderer->AddTile(xPos, yPos);
		}

		LoadData.Read(objectCount);
		//맵 액터
		for (int i = 0; i < objectCount; ++i)
		{
			LoadData.Read(tempintData);
			EActorTypeIndex tempType = static_cast<EActorTypeIndex>(tempintData);

			switch (tempType)
			{
			case EActorTypeIndex::Wall:
			{
				LoadData.Read(tempName);
				LoadData.Read(loadPosIndex);
				LoadData.Read(tempindex);

				tempVector = { loadPosIndex.X * m_TileSize, loadPosIndex.Y * m_TileSize };

				if (tempName == m_WallName[5])
				{
					if (tempindex > 1)
					{
						tempindex = CVector::RANDOM.RandomMinMax(2, 15);
					}
				}

				GameWall* NewWall = new GameWall(tempVector, tempName);
				NewWall->m_Wall->Index(tempindex);

				m_ListMapActor.push_back(static_cast<GameActor*>(NewWall));
				break;
			}
			case EActorTypeIndex::BackWall:
			{
				LoadData.Read(tempName);
				LoadData.Read(loadPosIndex);
				LoadData.Read(tempindex);

				tempVector = { loadPosIndex.X * m_TileSize, loadPosIndex.Y * m_TileSize };

				if (tempName == m_WallName[5])
				{
					if (tempindex > 17)
					{
						tempindex = CVector::RANDOM.RandomMinMax(18, 30);
					}
				}

				GameBackWall* NewBackWall = new GameBackWall(tempVector, tempName);
				NewBackWall->m_BackWall->Index(tempindex);

				m_ListMapActor.push_back(static_cast<GameActor*>(NewBackWall));
				break;
			}
			case EActorTypeIndex::Door:
			{
				LoadData.Read(bIsSide);
				LoadData.Read(loadPosIndex);

				tempVector = { loadPosIndex.X * m_TileSize, loadPosIndex.Y * m_TileSize };
				GameDoor* NewDoor = new GameDoor(tempVector, bIsSide);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewDoor));
				break;
			}
			case EActorTypeIndex::Stair:
			{
				LoadData.Read(loadPosIndex);

				tempVector = { loadPosIndex.X * m_TileSize, loadPosIndex.Y * m_TileSize };
				Stair* NewStair = new Stair(tempVector);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewStair));
				break;
			}

			case EActorTypeIndex::ShopFloor:
			{
				LoadData.Read(loadPosIndex);

				tempVector = { loadPosIndex.X * m_TileSize, loadPosIndex.Y * m_TileSize };
				ShopFloor* NewStair = new ShopFloor(tempVector);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewStair));
				break;
			}

			case EActorTypeIndex::ShopItem:
			{
				LoadData.Read(loadPosIndex);

				tempVector = { loadPosIndex.X * m_TileSize, loadPosIndex.Y * m_TileSize };
				ShopItem* NewStair = new ShopItem(tempVector);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewStair));
				break;
			}

			case EActorTypeIndex::StairLock:
			{
				LoadData.Read(loadPosIndex);

				tempVector = { loadPosIndex.X * m_TileSize, loadPosIndex.Y * m_TileSize };
				StairLock* NewStair = new StairLock(tempVector);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewStair));
				break;
			}

			case EActorTypeIndex::Weapon:
			{
				LoadData.Read(loadPosIndex);
				LoadData.Read(tempintData);

				tempVector = { loadPosIndex.X * m_TileSize, loadPosIndex.Y * m_TileSize };
				Weapon* NewWeapon = new Weapon(tempVector, (Weapon::WeaponType)tempintData);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewWeapon));
				break;
			}
			case EActorTypeIndex::Armor:
			{
				LoadData.Read(loadPosIndex);

				tempVector = { loadPosIndex.X * m_TileSize, loadPosIndex.Y * m_TileSize };
				Armor* NewArmor = new Armor(tempVector);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewArmor));
				break;
			}
			case EActorTypeIndex::Helmet:
			{
				LoadData.Read(loadPosIndex);

				tempVector = { loadPosIndex.X * m_TileSize, loadPosIndex.Y * m_TileSize };
				Helmet* NewWeapon = new Helmet(tempVector);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewWeapon));
				break;
			}
			case EActorTypeIndex::Walltorch:
			{
				LoadData.Read(loadPosIndex);

				tempVector = { loadPosIndex.X * m_TileSize, loadPosIndex.Y * m_TileSize };
				Torch* NewTorch = new Torch(tempVector);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewTorch));
				break;
			}
			case EActorTypeIndex::Chest:
			{
				LoadData.Read(tempintData);
				LoadData.Read(loadPosIndex);
				
				tempVector = { loadPosIndex.X * m_TileSize, loadPosIndex.Y * m_TileSize };
				Chest* NewChest = new Chest(tempVector,tempintData);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewChest));
				break;
			}

			case EActorTypeIndex::ShopKeeper:
			{
				LoadData.Read(loadPosIndex);

				tempVector = { loadPosIndex.X * m_TileSize, loadPosIndex.Y * m_TileSize };
				ShopKeeper* NewKeeper = new ShopKeeper(tempVector);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewKeeper));
				break;
			}
			case EActorTypeIndex::Text:
			{
				LoadData.Read(tempName);
				LoadData.Read(loadPosIndex);

				tempVector = { loadPosIndex.X * m_TileSize + 4, loadPosIndex.Y * m_TileSize };
				Text* NewText = new Text(tempVector, { 8.0f, 10.0f }, tempName);
				m_ListMapActor.push_back(static_cast<GameActor*>(NewText));
				break;
			}
			default:
				ASSERTMSG;
				break;
			}
		}

		//몬스터 로딩하기
		LoadData.Read(objectCount);
		for (int i = 0; i < objectCount; ++i)
		{
			LoadData.Read(tempindex);
			LoadData.Read(loadPosIndex);

			tempVector = { loadPosIndex.X * m_TileSize, loadPosIndex.Y * m_TileSize };

			Monster* tempPtr;


			switch (static_cast<EActorTypeIndex>(tempindex))
			{
			case EActorTypeIndex::M_greenSlime:
			{
				GreenSlime* NewMonster = new GreenSlime(tempVector);
				tempPtr = static_cast<Monster*>(NewMonster);
			}
			break;
			case EActorTypeIndex::M_blueSlime:
			{
				BlueSlime* NewMonster = new BlueSlime(tempVector);
				tempPtr = static_cast<Monster*>(NewMonster);
			}
			break;
			case EActorTypeIndex::M_CoralHead:
			{
				CoralHead* NewMonster = new CoralHead(tempVector);
				tempPtr = static_cast<Monster*>(NewMonster);
			}
			break;
			case EActorTypeIndex::M_CoralDrum:
			{
				Drum* NewMonster = new Drum(tempVector);
				tempPtr = static_cast<Monster*>(NewMonster);
			}
			break;
			case EActorTypeIndex::M_CoralHorn:
			{
				Horn* NewMonster = new Horn(tempVector);
				tempPtr = static_cast<Monster*>(NewMonster);
			}
			break;
			case EActorTypeIndex::M_CoralKeytar:
			{
				Keytar* NewMonster = new Keytar(tempVector);
				tempPtr = static_cast<Monster*>(NewMonster);
			}
			break;
			case EActorTypeIndex::M_CoralString:
			{
				MonString* NewMonster = new MonString(tempVector);
				tempPtr = static_cast<Monster*>(NewMonster);
			}
			break;
			case EActorTypeIndex::M_bat:
			{
				Bat* NewMonster = new Bat(tempVector);
				tempPtr = static_cast<Monster*>(NewMonster);
			}
			break;
			case EActorTypeIndex::M_skeleton:
			{
				Skeleton* NewMonster = new Skeleton(tempVector);
				tempPtr = static_cast<Monster*>(NewMonster);
			}
			break;
			case EActorTypeIndex::M_zombie:
			{
				Zombie* NewMonster = new Zombie(tempVector);
				tempPtr = static_cast<Monster*>(NewMonster);
			}
			break;
			case EActorTypeIndex::M_minotaur:
			{
				Minotaur * NewMonster = new Minotaur(tempVector);
				tempPtr = static_cast<Monster*>(NewMonster);
			}
			break;
			case EActorTypeIndex::M_monkey:
			{
				Monkey * NewMonster = new Monkey(tempVector);
				tempPtr = static_cast<Monster*>(NewMonster);
			}
			break;
			case EActorTypeIndex::M_dragon:
			{
				Dragon * NewMonster = new Dragon(tempVector);
				tempPtr = static_cast<Monster*>(NewMonster);
			}
			break;
			default:
				ASSERTMSG;
				break;
			}

			m_ListMonster.push_back(tempPtr);
		}


		//물바닥 로딩해오기
		
		LoadData.Read(objectCount);

		xPos = 0;
		yPos = 0;
		for (int i = 0; i < objectCount; ++i)
		{
			if (i == 0)
			{
				m_WaterTileMap = new GameMap(EActorTypeIndex::WaterTile);
				WaterTileRenderer = m_WaterTileMap->TileRen();
				WaterTileRenderer->Image(L"TEMP_floor_water.bmp");
				WaterTileRenderer->m_AllTile.begin();
				State()->m_WaterTileRender = WaterTileRenderer;

			}


			LoadData.Read(xPos);
			LoadData.Read(yPos);
			WaterTileRenderer->AddTile(xPos, yPos);
		}



	}
}


void MapEditor::CheckObjectType()
{
	switch (m_EObjectdata)
	{
	case ObjectData::tile:
		swprintf_s(buff, L"Creattile\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::wall:
		swprintf_s(buff, L"Createwall\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::door:
		swprintf_s(buff, L"CreateFrontdoor\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::doorSide:
		swprintf_s(buff, L"CreateSidedoor\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::shopFloor:
		swprintf_s(buff, L"CreateshopFloor\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::stair:
		swprintf_s(buff, L"Createstair\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::waterTile:
		swprintf_s(buff, L"waterTile\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::stairlock:
		swprintf_s(buff, L"Createstairlock\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::walltorch:
		swprintf_s(buff, L"CreateTorch\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::cryptWall:
		swprintf_s(buff, L"cryptWall\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::dirtWall:
		swprintf_s(buff, L"dirtWall\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::dirtWallCrack:
		swprintf_s(buff, L"dirtWallCrack\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::dirtWallNestItem:
		swprintf_s(buff, L"DirtWallNestItem\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::dirtWallNestDia:
		swprintf_s(buff, L"DirtWallNestDia\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::shopItem:
		swprintf_s(buff, L"shopItem\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::stoneWall:
		swprintf_s(buff, L"stoneWall\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::stoneWallCrack:
		swprintf_s(buff, L"stoneWallCrack\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::bossWall:
		swprintf_s(buff, L"Unbreak\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::unbreakable:
		swprintf_s(buff, L"BossWall\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::weapon:
		swprintf_s(buff, L"weapon\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::chest:
		swprintf_s(buff, L"chest\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::chest2:
		swprintf_s(buff, L"chest2\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::armor:
		swprintf_s(buff, L"armor\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::helmet:
		swprintf_s(buff, L"helmet\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::coralhead:
		swprintf_s(buff, L"coralhead\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::coraldrum:
		swprintf_s(buff, L"coraldrum\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::coralhorn:
		swprintf_s(buff, L"coralhorn\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::coralstring:
		swprintf_s(buff, L"coralstring\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::coratkeytar:
		swprintf_s(buff, L"coratkeytar\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::shopKeeper:
		swprintf_s(buff, L"shopKeeper\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::greenSlime:
		swprintf_s(buff, L"greenSlime\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::blueSlime:
		swprintf_s(buff, L"blueSlime\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::bat:
		swprintf_s(buff, L"bat\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::skeleton:
		swprintf_s(buff, L"skeleton\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::zombie:
		swprintf_s(buff, L"zombie\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::minotaur:
		swprintf_s(buff, L"minotaur\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::monkey:
		swprintf_s(buff, L"monkey\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::dragon:
		swprintf_s(buff, L"dragon\n");
		OutputDebugStringW(buff);
		break;
	case ObjectData::text_HelloWorld:
		swprintf_s(buff, L"text_HelloWorld\n");
		OutputDebugStringW(buff);
		break;
	default:
		ASSERTMSG;
		break;
	}
}