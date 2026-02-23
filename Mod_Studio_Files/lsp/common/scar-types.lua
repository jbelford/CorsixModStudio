---@meta scar-types

-- Shared SCAR (Scripting At Relic) type definitions.
-- These opaque handle types are used by both Dawn of War and Company of Heroes.
-- Auto-generated type stubs — do not edit manually.

---Squad group handle (collection of military squads)
---@class SGroupID
local SGroupID = {}

---Entity group handle (collection of buildings/objects)
---@class EGroupID
local EGroupID = {}

---Individual squad handle
---@class SquadID
local SquadID = {}

---Individual entity handle
---@class EntityID
local EntityID = {}

---Player handle
---@class PlayerID
local PlayerID = {}

---World-editor marker handle
---@class MarkerID
local MarkerID = {}

---3D world coordinate
---@class Position
---@field x number
---@field y number
---@field z number
local Position = {}

---NIS actor reference
---@class ActorTable
---@field blueprint string?
---@field icon string?
---@field squadID any
---@field squadGameID any
---@field [string] any
local ActorTable = {}

---Ability handle
---@class AbilityID
local AbilityID = {}

---Upgrade handle
---@class UpgradeID
local UpgradeID = {}

---Critical hit type handle
---@class CriticalID
local CriticalID = {}

---Property bag group handle
---@class PropertyBagGroup
local PropertyBagGroup = {}

---Localized string handle
---@class LocString
local LocString = {}

---Blueprint handle
---@class Blueprint
local Blueprint = {}
