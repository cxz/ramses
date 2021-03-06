//  -------------------------------------------------------------------------
//  Copyright (C) 2017 BMW Car IT GmbH
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#include "SceneViewer.h"

#include "ramses-client.h"
#include "ramses-utils.h"

#include "Utils/Argument.h"
#include "Utils/LogMacros.h"
#include "Utils/RamsesLogger.h"

#include "ramses-renderer-api/RamsesRenderer.h"
#include "ramses-renderer-api/DisplayConfig.h"
#include "ramses-framework-api/RamsesFramework.h"
#include "DisplayManager/DisplayManager.h"

#include "PlatformAbstraction/PlatformThread.h"
#include "RendererLib/RendererConfigUtils.h"
#include "ramses-hmi-utils.h"

namespace ramses_internal
{
    SceneViewer::SceneViewer(int argc, char* argv[])
        : m_parser(argc, argv)
        , m_helpArgument(m_parser, "help", "help", false, "Print this help")
        , m_scenePathAndFileArgument(m_parser, "s", "scene", String(), "Scene path+file")
        , m_optionalResFileArgument(m_parser, "r", "res", String(), "Resource file")
    {
        GetRamsesLogger().initialize(m_parser, String(), String(), false);

        const bool   helpRequested    = m_helpArgument;
        const String scenePathAndFile = m_scenePathAndFileArgument;
        const String optionalResFile  = m_optionalResFileArgument;

        if (helpRequested)
        {
            printUsage();
        }
        else
        {
            if (scenePathAndFile != "")
            {
                const String& sceneFile = scenePathAndFile + ".ramses";
                String        resFile   = optionalResFile;
                if (resFile.empty())
                {
                    resFile = scenePathAndFile + ".ramres";
                }
                loadAndRenderScene(argc, argv, sceneFile, resFile);
            }
            else
            {
                LOG_ERROR(CONTEXT_CLIENT,
                          "A scene file including path has to be specified by option "
                              << m_scenePathAndFileArgument.getHelpString());
            }
        }
    }

    void SceneViewer::printUsage() const
    {
        String argumentHelpString = m_helpArgument.getHelpString() + m_scenePathAndFileArgument.getHelpString() + m_optionalResFileArgument.getHelpString();

        const String& programName = m_parser.getProgramName();
        LOG_INFO(CONTEXT_CLIENT,
                "\nUsage: " << programName << " [options] -s <sceneFileName>\n"
                "Loads and views a RAMSES scene from the files <sceneFileName>.ramses / <sceneFileName>.ramres\n"
                "Arguments:\n" << argumentHelpString);

        ramses_internal::RendererConfigUtils::PrintCommandLineOptions();

    }

    void SceneViewer::loadAndRenderScene(int argc, char* argv[], const String& sceneFile, const String& resFile)
    {
        ramses::RamsesFrameworkConfig frameworkConfig(argc, argv);
        frameworkConfig.setRequestedRamsesShellType(ramses::ERamsesShellType_Console);
        ramses::RamsesFramework framework(frameworkConfig);

        const ramses::RendererConfig rendererConfig(argc, argv);
        ramses::RamsesRenderer renderer(framework, rendererConfig);
        renderer.startThread();
        ramses_display_manager::DisplayManager displayManager(renderer, framework, false);

        ramses::DisplayConfig displayConfig(argc, argv);
        const ramses::displayId_t displayId = displayManager.createDisplay(displayConfig);

        ramses::RamsesClient client("client-scene-reader", framework);
        framework.connect();

        auto loadedScene = loadSceneWithResources(client, sceneFile, resFile);
        if (loadedScene == nullptr)
        {
            LOG_ERROR(CONTEXT_CLIENT, "Loading scene failed!");
            framework.disconnect();
            return;
        }

        loadedScene->publish();
        loadedScene->flush();
        validateContent(client, *loadedScene);

        ramses::RamsesHMIUtils::DumpUnrequiredSceneObjects(*loadedScene);

        displayManager.showSceneOnDisplay(loadedScene->getSceneId(), displayId);

        while (displayManager.isRunning())
        {
            displayManager.dispatchAndFlush();
            ramses_internal::PlatformThread::Sleep(30u);
        }
    }

    ramses::Scene* SceneViewer::loadSceneWithResources(ramses::RamsesClient& client, const String& sceneFile, const String& resFile)
    {
        LOG_INFO(CONTEXT_CLIENT, "Load files: scene:" << sceneFile << ", resources:" << resFile);
        ramses::ResourceFileDescriptionSet resourceFileInformation;
        resourceFileInformation.add(ramses::ResourceFileDescription(resFile.c_str()));
        return client.loadSceneFromFile(sceneFile.c_str(), resourceFileInformation);
    }

    void SceneViewer::validateContent(const ramses::RamsesClient& client, const ramses::Scene& scene) const
    {
        ramses::status_t validateStatus = scene.validate();
        if (validateStatus != ramses::StatusOK)
        {
            LOG_ERROR(CONTEXT_CLIENT, "Scene validate failed: " << client.getStatusMessage(validateStatus));
        }

        LOG_INFO(CONTEXT_CLIENT, "Scene validation report: " << scene.getValidationReport(ramses::EValidationSeverity_Info));

        validateStatus = client.validate();
        if (validateStatus != ramses::StatusOK)
        {
            LOG_ERROR(CONTEXT_CLIENT, "Client validate failed: " << client.getStatusMessage(validateStatus));
        }

        LOG_INFO(CONTEXT_CLIENT, "Client validation report: " << client.getValidationReport(ramses::EValidationSeverity_Info));
    }
}
