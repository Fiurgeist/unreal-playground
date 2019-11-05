#pragma once

#include "Engine.h"
#include "EngineUtils.h"

class TestUtils
{
public:
	static UWorld* GetWorld()
	{
		if (GEngine)
		{
			if (FWorldContext* WorldContext = GEngine->GetWorldContextFromPIEInstance(0))
			{
				return WorldContext->World();
			}
		}
		return nullptr;
	}

	static void Exit()
	{
		if (UWorld* World = GetWorld())
		{
			if (APlayerController* TargetPC = UGameplayStatics::GetPlayerController(World, 0))
			{
				TargetPC->ConsoleCommand(TEXT("Exit"), true);
			}
		}
	}

private:
	TestUtils() {}
};
