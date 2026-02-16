#include <gtest/gtest.h>
#include <memory>
#include <rainman/core/Exception.h>

using ExceptionPtr = std::unique_ptr<CRainmanException, ExceptionDeleter>;

TEST(ExceptionDeleterTest, UniquePtr_DestroysOnScopeExit)
{
	auto ex = ExceptionPtr(new CRainmanException(__FILE__, __LINE__, "test exception"));
	EXPECT_NE(ex.get(), nullptr);
	EXPECT_STREQ(ex->getMessage(), "test exception");
	// ex goes out of scope — ExceptionDeleter::operator() calls destroy()
}

TEST(ExceptionDeleterTest, ReleasePreventsDestruction)
{
	auto ex = ExceptionPtr(new CRainmanException(__FILE__, __LINE__, "released"));
	CRainmanException *pRaw = ex.release();
	EXPECT_EQ(ex.get(), nullptr);
	EXPECT_STREQ(pRaw->getMessage(), "released");
	// Must manually clean up since we released ownership
	pRaw->destroy();
}

TEST(ExceptionDeleterTest, ResetDestroysOldAndTakesNew)
{
	auto ex = ExceptionPtr(new CRainmanException(__FILE__, __LINE__, "old"));
	EXPECT_STREQ(ex->getMessage(), "old");

	ex.reset(new CRainmanException(__FILE__, __LINE__, "new"));
	EXPECT_STREQ(ex->getMessage(), "new");
	// "old" was destroyed by reset(); "new" destroyed when ex goes out of scope
}

TEST(ExceptionDeleterTest, ResetToNullDestroysException)
{
	auto ex = ExceptionPtr(new CRainmanException(__FILE__, __LINE__, "will be nulled"));
	EXPECT_NE(ex.get(), nullptr);

	ex.reset(nullptr);
	EXPECT_EQ(ex.get(), nullptr);
}

TEST(ExceptionDeleterTest, MoveConstructorTransfersOwnership)
{
	auto ex1 = ExceptionPtr(new CRainmanException(__FILE__, __LINE__, "moved"));
	CRainmanException *pRaw = ex1.get();

	ExceptionPtr ex2(std::move(ex1));
	EXPECT_EQ(ex1.get(), nullptr);
	EXPECT_EQ(ex2.get(), pRaw);
	EXPECT_STREQ(ex2->getMessage(), "moved");
}

TEST(ExceptionDeleterTest, MoveAssignmentTransfersOwnership)
{
	auto ex1 = ExceptionPtr(new CRainmanException(__FILE__, __LINE__, "source"));
	auto ex2 = ExceptionPtr(new CRainmanException(__FILE__, __LINE__, "target"));
	CRainmanException *pSource = ex1.get();

	ex2 = std::move(ex1);
	// ex1 is now empty; ex2 holds the source; old "target" was destroyed
	EXPECT_EQ(ex1.get(), nullptr);
	EXPECT_EQ(ex2.get(), pSource);
	EXPECT_STREQ(ex2->getMessage(), "source");
}

TEST(ExceptionDeleterTest, DefaultConstructedIsNull)
{
	ExceptionPtr ex;
	EXPECT_EQ(ex.get(), nullptr);
}
