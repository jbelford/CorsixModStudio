/*
    This file is part of Corsix's Mod Studio.

    Corsix's Mod Studio is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Corsix's Mod Studio is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Corsix's Mod Studio; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <gtest/gtest.h>
#include "services/ModuleService.h"

TEST(ModuleServiceTest, DefaultConstructorHasNoModule)
{
	ModuleService svc;
	EXPECT_FALSE(svc.HasModule());
	EXPECT_EQ(svc.GetModule(), nullptr);
}

TEST(ModuleServiceTest, SetAndGetModule)
{
	CModuleFile mod;
	ModuleService svc(&mod);
	EXPECT_TRUE(svc.HasModule());
	EXPECT_EQ(svc.GetModule(), &mod);
}

TEST(ModuleServiceTest, SetModuleLater)
{
	CModuleFile mod;
	ModuleService svc;
	svc.SetModule(&mod);
	EXPECT_TRUE(svc.HasModule());
}

TEST(ModuleServiceTest, GetNameReturnsEmptyForFreshModule)
{
	CModuleFile mod;
	ModuleService svc(&mod);
	// A fresh CModuleFile has null name pointers — wxString conversion should return empty
	wxString name = svc.GetName();
	EXPECT_TRUE(name.IsEmpty());
}

TEST(ModuleServiceTest, GetModFolderReturnsEmptyForFreshModule)
{
	CModuleFile mod;
	ModuleService svc(&mod);
	EXPECT_TRUE(svc.GetModFolder().IsEmpty());
}

TEST(ModuleServiceTest, GetDescriptionReturnsEmptyForFreshModule)
{
	CModuleFile mod;
	ModuleService svc(&mod);
	EXPECT_TRUE(svc.GetDescription().IsEmpty());
}

// Note: LoadModuleFile and LoadSgaAsMod with invalid paths cause access violations
// in the underlying CModuleFile code (pre-existing bug). These cannot be tested
// with invalid input until that is fixed.
