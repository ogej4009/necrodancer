#include "BaseUpdater.h"

/*

#include "stdafx.h"

stdafx.h �� ���� ���̴� ����� ���� ���� �Ѳ����� �̸� ������ �س��Ƽ� 
���߿� �ٽ� ���������� �ʰ� �׳� �ٽ� ���ڴٴ� ����̴�.

�̷��� �̸� �����ϵ�(Precompiled) ������ .pch �����̴�.

������Ʈ�� ũ�Ⱑ Ŀ���� ������� ũ�Ⱑ Ŀ���ų� ���ڰ� �������� 
�����ϰ� ����� �ʿ䰡 �ִ�.

������Ʈ �Ӽ����� Precompiled Header�� ����� Enable �ϰ� ������Ѵ�.

���� Wizard�� ������Ʈ�� �����ϴ� ��� �⺻���� stdafx.h�� Include �Ǿ� �ִµ�, 
���⼭ stdio.h�� Include �ϸ� �ߺ��� �ǰų� ����� ��Ű�� ������ �߻��ؼ� 
Unexpected File End �� �߻��ϰ� �ȴ�.

stdafx.h �� ����� ���� ��� cpp ������ �̸� Include �ؾ� �ϸ� 
������ ����ϰ� ������ ��� ���°� ����.

*/

BaseUpdater::BaseUpdater() : m_bDeath(false), m_bUpdate(true)
{
}


BaseUpdater::~BaseUpdater()
{
}
// WinMain