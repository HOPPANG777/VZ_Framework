#pragma once

#include "ServiceLocator.h"
#include "../Systems/Input.h"
#include "../Systems/Time.h"
#include "../Systems/Graphics.h"
#include "../Systems/Camera.h"
#include "../Managers/SceneManager.h"
#include "../Systems/Sounds.h"
#include "Gui.h"
#include "../Systems/Keyboard.h"
#include "../Systems/Mouse.h"
#include "../Managers/CollisionManager.h"
#include "Gizmo.h"
#include "../Managers/TileSetManager.h"
#include "../Managers/Texture2DManager.h"
#include "HelpMe.h"

inline void RegisterServices()
{
    ServiceLocator::Clear();
    ServiceLocator::Register<Input>("Input", std::shared_ptr<Input>(Input::Get(), [](Input*){}));
    ServiceLocator::Register<Time>("Time", std::shared_ptr<Time>(Time::Get(), [](Time*){}));
    ServiceLocator::Register<Graphics>("Graphics", std::shared_ptr<Graphics>(Graphics::Get(), [](Graphics*){}));
    ServiceLocator::Register<Camera>("Camera", std::shared_ptr<Camera>(Camera::Get(), [](Camera*){}));
    ServiceLocator::Register<SceneManager>("SceneManager", std::shared_ptr<SceneManager>(SceneManager::Get(), [](SceneManager*){}));
    ServiceLocator::Register<Sounds>("Sounds", std::shared_ptr<Sounds>(Sounds::Get(), [](Sounds*){}));
    ServiceLocator::Register<Gui>("Gui", std::shared_ptr<Gui>(Gui::Get(), [](Gui*){}));
    ServiceLocator::Register<Keyboard>("Keyboard", std::shared_ptr<Keyboard>(Keyboard::Get(), [](Keyboard*){}));
    ServiceLocator::Register<Mouse>("Mouse", std::shared_ptr<Mouse>(Mouse::Get(), [](Mouse*){}));
    ServiceLocator::Register<CollisionManager>("CollisionManager", std::shared_ptr<CollisionManager>(CollisionManager::Get(), [](CollisionManager*){}));
    ServiceLocator::Register<Gizmo>("Gizmo", std::shared_ptr<Gizmo>(Gizmo::Get(), [](Gizmo*){}));
    ServiceLocator::Register<TileSetManager>("TileSetManager", std::shared_ptr<TileSetManager>(TileSetManager::Get(), [](TileSetManager*){}));
    ServiceLocator::Register<Texture2DManager>("Texture2DManager", std::shared_ptr<Texture2DManager>(Texture2DManager::Get(), [](Texture2DManager*){}));
    ServiceLocator::Register<HelpMe>("HelpMe", std::shared_ptr<HelpMe>(HelpMe::Get(), [](HelpMe*){}));
}

