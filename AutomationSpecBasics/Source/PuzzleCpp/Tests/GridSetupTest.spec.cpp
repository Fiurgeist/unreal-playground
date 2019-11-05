#include "GridSetupTest.h"

#include "Engine.h"
#include "EngineUtils.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"

#include "PuzzleCppBlock.h"
#include "PuzzleCppBlockGrid.h"
#include "TestUtils.h"

static const int32 TestFlags =
	EAutomationTestFlags::EditorContext |
	EAutomationTestFlags::ClientContext |
	EAutomationTestFlags::ProductFilter;

// Everything between the macros is shared between tests.
BEGIN_DEFINE_SPEC(GridSetupTest, "PuzzleCpp", TestFlags)
UWorld* World;
APuzzleCppBlockGrid* Grid;
END_DEFINE_SPEC(GridSetupTest)

void GridSetupTest::Define()
{
	Describe("PuzzleGridSetup", [this]()
	{
		BeforeEach([this]()
		{
			AutomationOpenMap(TEXT("/Game/PuzzleCPP/Maps/PuzzleExampleMap"));

			World = TestUtils::GetWorld();
			TestNotNull(TEXT("The World was not created properly."), World);
			
			Grid = (APuzzleCppBlockGrid*)UGameplayStatics::GetActorOfClass(World, APuzzleCppBlockGrid::StaticClass());
			TestNotNull(TEXT("The Grid was not created properly."), Grid);
		});

		It("should have the expected amount of blocks", [this]()
		{
			int32 BlockCount = 0;
			for (TActorIterator<AActor> It(World, APuzzleCppBlock::StaticClass()); It; ++It)
			{
				APuzzleCppBlock* Block = (APuzzleCppBlock*)*It;
				TestFalse(TEXT("Block should not be active."), Block->bIsActive);
				TestEqual(TEXT("Block has the wrong material."), Block->GetBlockMesh()->GetMaterial(0), (UMaterialInterface*)Block->BlueMaterial);
				TestEqual(TEXT("Block is not assinged to the correct grid."), Block->OwningGrid, Grid);

				BlockCount++;
			}

			TestEqual(TEXT("Wrong amount of blocks in the grid."), BlockCount, 9);
		});

		It("should display the correct initial Score", [this]()
		{
			TestEqual(TEXT("Wrong initial score."), Grid->Score, 0);
			TestEqual(TEXT("Wrong score text is displayed."), Grid->GetScoreText()->Text.ToString(), TEXT("Score: 0"));
		});

		AfterEach([this]()
		{
			TestUtils::Exit();
		});
	});
}
