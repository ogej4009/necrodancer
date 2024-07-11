#pragma once
//Ű�� ����, ���� ���� üũ Ŭ����
//�߻�Ŭ���� = �������̽� �뵵��
class BaseUpdater
{
private:
	bool m_bUpdate;
	bool m_bDeath;

public:
	bool IsOff()
	{
		return false == m_bUpdate;
	}

	bool IsOn()
	{
		return true == m_bUpdate;
	}

	void On()
	{
		m_bUpdate = true;
	}

	void Off()
	{
		m_bUpdate = false;
	}

	void Death()
	{
		m_bDeath = true;
	}

	virtual bool IsUpdate()
	{
		return (false == m_bDeath) && (true == m_bUpdate);
	}

	virtual bool IsDeath()
	{
		return m_bDeath;
	}

public:
	BaseUpdater();
	virtual ~BaseUpdater() = 0;

};

