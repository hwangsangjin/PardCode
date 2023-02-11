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
    if (::GetKeyboardState(m_key_states))
    {
        for (unsigned int i = 0; i < 256; i++)
        {
            // Key is down
            if (m_key_states[i] & 0x80)
            {
                std::map<App*, App*>::iterator iter = m_map_listeners.begin();

                while (iter != m_map_listeners.end())
                {
                    iter->second->OnKeyDown(i);
                    ++iter;
                }
            }
            else // Key is up
            {
                // 키의 현재 상태가 이전 상태와 같지 않은 경우
                if (m_key_states[i] != m_old_key_states[i])
                {
                    std::map<App*, App*>::iterator iter = m_map_listeners.begin();

                    while (iter != m_map_listeners.end())
                    {
                        iter->second->OnKeyUp(i);
                        ++iter;
                    }
                }
            }
        }

        // 현재 키 상태를 이전 키 상태 버퍼에 저장
        ::memcpy(m_old_key_states, m_key_states, sizeof(unsigned char) * 256);
    }
}

void Input::AddListener(App* listener)
{
    m_map_listeners.insert(std::make_pair<App*, App*>(std::forward<App*>(listener), std::forward<App*>(listener)));
}

void Input::RemoveListener(App* listener)
{
    std::map<App*, App*>::iterator iter = m_map_listeners.find(listener);

    if (iter != m_map_listeners.end())
    {
        m_map_listeners.erase(iter);
    }
}
