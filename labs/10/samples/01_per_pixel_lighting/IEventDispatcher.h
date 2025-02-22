#pragma once

template <class ListenerType>
class IEventDispatcher
{
public:
	virtual void AddEventListener(ListenerType * pListener) = 0;
	virtual void RemoveEventListener(ListenerType * pListener) = 0;
protected:
	IEventDispatcher(void){}
	virtual ~IEventDispatcher(void){}
};

template <class Base, class ListenerType>
class CEventDispatcherImpl : public Base
{
public:
	// ��������� ��������� �� ��������� ������� � ��������� ����������
	virtual void AddEventListener(ListenerType * pListener)
	{
		m_listeners.insert(pListener);
	}

	// ������� ��������� �� ��������� ������� �� ��������� ����������
	virtual void RemoveEventListener(ListenerType * pListener)
	{
		Listeners::const_iterator it = m_listeners.find(pListener);
		if (it != m_listeners.end())
		{
			m_listeners.erase(it);
		}
	}

	// ��������� ���� ���������� �������, ���������� � �������
	// �������� � �������������� ������� event
	template <class EventType>
	void DispatchEvent(EventType const& event)const
	{
		// ������� ����� ��������� ����������, �.�. � �������� ��������
		// ����������� � ������� ���������� ����� ������� ��������� �� ���������,
		// ��� ������� ����������� ���������, ������������ � for_each
		Listeners listeners(m_listeners);

		// �������� () ������� event ����� ������� ������� ���, �������
		// ���������� � ���������� �������.
		// ���������� ��������� () ����� �������� ������ �� ���������� 
		// �� ��������� listeners
		std::for_each(listeners.begin(), listeners.end(), event);
	}
private:
	typedef std::set<ListenerType *> Listeners;
	Listeners m_listeners;
};
