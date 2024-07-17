#include "BaseUpdater.h"

/*

#include "stdafx.h"

stdafx.h 란 자주 쓰이는 헤더들 여러 개를 한꺼번에 미리 컴파일 해놓아서 
나중에 다시 컴파일하지 않고 그냥 다시 쓰겠다는 헤더이다.

이렇게 미리 컴파일된(Precompiled) 파일이 .pch 파일이다.

프로젝트의 크기가 커지고 헤더들의 크기가 커지거나 숫자가 많아지면 
유용하게 써야할 필요가 있다.

프로젝트 속성에서 Precompiled Header의 사용을 Enable 하게 해줘야한다.

보통 Wizard로 프로젝트를 생성하는 경우 기본으로 stdafx.h가 Include 되어 있는데, 
여기서 stdio.h를 Include 하면 중복이 되거나 헤더가 엉키는 현상이 발생해서 
Unexpected File End 가 발생하게 된다.

stdafx.h 를 사용할 때는 모든 cpp 파일이 이를 Include 해야 하며 
가능한 깔끔하게 구조를 잡고 가는게 좋다.

*/

BaseUpdater::BaseUpdater() : m_bDeath(false), m_bUpdate(true)
{
}


BaseUpdater::~BaseUpdater()
{
}
// WinMain