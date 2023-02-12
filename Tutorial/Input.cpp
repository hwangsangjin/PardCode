#include "Input.h"
#include "App.h"
#include <Windows.h>

Input* Input::GetInstance()
{
    static Input instance;
    return &instance;
}

void Input::Update()
{
    POINT current_mouse_position = {};
    ::GetCursorPos(&current_mouse_position);

    if (m_first_time)
    {
        m_old_mouse_position = Point(current_mouse_position.x, current_mouse_position.y);
        m_first_time = false;
    }

    if (current_mouse_position.x != m_old_mouse_position.GetX() || current_mouse_position.y != m_old_mouse_position.GetY())
    {
        // Mouse move event
        std::unordered_set<App*>::iterator iter = m_set_listeners.begin();

        while (iter != m_set_listeners.end())
        {
            (*iter)->OnMouseMove(Point(current_mouse_position.x - m_old_mouse_position.GetX(), current_mouse_position.y - m_old_mouse_position.GetY()));
            ++iter;
        }
    }
    m_old_mouse_position = Point(current_mouse_position.x, current_mouse_position.y);

    if (::GetKeyboardState(m_key_states))
    {
        for (unsigned int i = 0; i < 256; i++)
        {
            // Key is down
            if (m_key_states[i] & 0x80)
            {
                std::unordered_set<App*>::iterator iter = m_set_listeners.begin();

                while (iter != m_set_listeners.end())
                {
                    if (i == VK_LBUTTON)
                    {
                        if (m_key_states[i] != m_old_key_states[i])
                        {
                            (*iter)->OnLeftButtonDown(Point(current_mouse_position.x, current_mouse_position.y));
                        }
                    }
                    else if (i == VK_RBUTTON)
                    {
                        if (m_key_states[i] != m_old_key_states[i])
                        {
                            (*iter)->OnRightButtonDown(Point(current_mouse_position.x, current_mouse_position.y));
                        }
                    }
                    else
                    {
                        (*iter)->OnKeyDown(i);
                    }
                    ++iter;
                }
            }
            else // Key is up
            {
                // Ű�� ���� ���°� ���� ���¿� ���� ���� ���
                if (m_key_states[i] != m_old_key_states[i])
                {
                    std::unordered_set<App*>::iterator iter = m_set_listeners.begin();

                    while (iter != m_set_listeners.end())
                    {
                        if (i == VK_LBUTTON)
                        {
                            (*iter)->OnLeftButtonUp(Point(current_mouse_position.x, current_mouse_position.y));
                        }
                        else if (i == VK_RBUTTON)
                        {
                            (*iter)->OnRightButtonUp(Point(current_mouse_position.x, current_mouse_position.y));
                        }
                        else
                        {
                            (*iter)->OnKeyUp(i);
                        }
                        ++iter;
                    }
                }
            }
        }

        // ���� Ű ���¸� ���� Ű ���� ���ۿ� ����
        ::memcpy(m_old_key_states, m_key_states, sizeof(unsigned char) * 256);
    }
}

void Input::AddListener(App* listener)
{
    m_set_listeners.insert(listener);
}

void Input::RemoveListener(App* listener)
{
    m_set_listeners.erase(listener);
}
