#pragma once


namespace Engine::Core::Input
{
	struct MouseInputResource
	{
        float deltaX = 0.0f;
        float deltaY = 0.0f;

        float screenX = 0.0f;
        float screenY = 0.0f;

        float ndcX = 0.0f;
        float ndcY = 0.0f;
	};


    class MouseInputBridge
    {
    public:

    };
}