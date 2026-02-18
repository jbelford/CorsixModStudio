#include <gtest/gtest.h>
#include <memory>
#include <rainman/core/Exception.h>

// Tests for CRainmanException with standard unique_ptr (no custom deleter needed).

using ExceptionPtr = std::unique_ptr<CRainmanException>;

TEST(ExceptionDeleterTest, UniquePtr_DestroysOnScopeExit)
{
	auto ex = std::make_unique<CRainmanException>(__FILE__, __LINE__, "test exception");
	EXPECT_NE(ex.get(), nullptr);
	EXPECT_STREQ(ex->getMessage(), "test exception");
	// ex goes out of scope — standard delete
}

TEST(ExceptionDeleterTest, ReleasePreventsDestruction)
{
	auto ex = std::make_unique<CRainmanException>(__FILE__, __LINE__, "released");
	CRainmanException *pRaw = ex.release();
	EXPECT_EQ(ex.get(), nullptr);
	EXPECT_STREQ(pRaw->getMessage(), "released");
	delete pRaw; // clean up
}

TEST(ExceptionDeleterTest, ResetDestroysOldAndTakesNew)
{
	auto ex = std::make_unique<CRainmanException>(__FILE__, __LINE__, "old");
	EXPECT_STREQ(ex->getMessage(), "old");

	ex.reset(new CRainmanException(__FILE__, __LINE__, "new"));
	EXPECT_STREQ(ex->getMessage(), "new");
}

TEST(ExceptionDeleterTest, ResetToNullDestroysException)
{
	auto ex = std::make_unique<CRainmanException>(__FILE__, __LINE__, "will be nulled");
	EXPECT_NE(ex.get(), nullptr);

	ex.reset(nullptr);
	EXPECT_EQ(ex.get(), nullptr);
}

TEST(ExceptionDeleterTest, MoveConstructorTransfersOwnership)
{
	auto ex1 = std::make_unique<CRainmanException>(__FILE__, __LINE__, "moved");
	CRainmanException *pRaw = ex1.get();

	ExceptionPtr ex2(std::move(ex1));
	EXPECT_EQ(ex1.get(), nullptr);
	EXPECT_EQ(ex2.get(), pRaw);
	EXPECT_STREQ(ex2->getMessage(), "moved");
}

TEST(ExceptionDeleterTest, MoveAssignmentTransfersOwnership)
{
	auto ex1 = std::make_unique<CRainmanException>(__FILE__, __LINE__, "source");
	auto ex2 = std::make_unique<CRainmanException>(__FILE__, __LINE__, "target");
	CRainmanException *pSource = ex1.get();

	ex2 = std::move(ex1);
	EXPECT_EQ(ex1.get(), nullptr);
	EXPECT_EQ(ex2.get(), pSource);
	EXPECT_STREQ(ex2->getMessage(), "source");
}

TEST(ExceptionDeleterTest, DefaultConstructedIsNull)
{
	ExceptionPtr ex;
	EXPECT_EQ(ex.get(), nullptr);
}

TEST(ExceptionDeleterTest, StackBasedExceptionWorks)
{
	CRainmanException ex(__FILE__, __LINE__, "stack-based");
	EXPECT_STREQ(ex.getMessage(), "stack-based");
	EXPECT_EQ(ex.getPrecursor(), nullptr);
}

TEST(ExceptionDeleterTest, CopyConstructorDeepCopies)
{
	CRainmanException inner("inner.cpp", 1, "inner");
	CRainmanException outer(inner, "outer.cpp", 2, "outer");
	CRainmanException copy(outer);

	EXPECT_STREQ(copy.getMessage(), "outer");
	ASSERT_NE(copy.getPrecursor(), nullptr);
	EXPECT_STREQ(copy.getPrecursor()->getMessage(), "inner");
	// Precursor is a deep copy, not the same pointer
	EXPECT_NE(copy.getPrecursor(), outer.getPrecursor());
}
