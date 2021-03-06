// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "ReportedSchema.h"
#include "DMJsonConstants.h"

#define BUFFER_SIZE 20480

namespace Microsoft { namespace Azure { namespace DeviceManagement { namespace Common {

    enum DeploymentStatus
    {
        eSucceeded,
        eNotStarted,
        ePending,
        eFailed,
        eUnknown
    };

    enum ConnectionStatus
    {
        eOffline,
        eOnline
    };

    class IMdmServer
    {
    public:
        virtual std::string RunSyncML(const std::string& sid, const std::string& syncML) = 0;
    };

    class IRawHandlerHost
    {
    public:
        virtual void Report(const std::string& id, DeploymentStatus deploymentStatus, const Json::Value& value) = 0;
    };

    struct OperationModel
    {
        bool present;
        Json::Value value;

        OperationModel() :
            present(false)
        {}
    };

    template<class T>
    struct OperationModelT
    {
        bool present;
        T value;

        OperationModelT() :
            present(false)
        {}
    };

    enum InvokeContext
    {
        eDesiredState,
        eDirectMethod
    };

    struct InvokeResult
    {
        bool present;
        int code;
        std::string payload;

        InvokeResult() :
            present(false),
            code(0)
        {}

        // ToDo: change bool present to InvokeContext context
        InvokeResult(bool present_) :
            present(present_),
            code(0)
        {}

        InvokeResult(bool present_, int code_, const std::string& payload_) :
            present(present_),
            code(code_),
            payload(payload_)
        {}

        InvokeResult(InvokeContext context) :
            present(context == eDirectMethod),
            code(0)
        {}

        InvokeResult(InvokeContext context, int code_, const std::string& payload_) :
            present(context == eDirectMethod),
            code(code_),
            payload(payload_)
        {}
    };

    // IHandler
    class IHandler
    {
    public:
        virtual std::string GetHandlerType() const = 0;
    };

    // IRawHandler
    class IRawHandler : public IHandler
    {
    public:
        virtual std::string GetId() const = 0;
        virtual bool IsConfigured() const = 0;

        // Start/Stop
        virtual void Start(
            const Json::Value& config,
            bool& active) = 0;

        virtual void Stop() = 0;

        virtual void OnConnectionStatusChanged(
            ConnectionStatus status) = 0;

        // Deployment Status...
        virtual void SetDeploymentStatus(DeploymentStatus deploymentStatus) = 0;
        virtual DeploymentStatus GetDeploymentStatus() const = 0;

        // Processing desired state...
        virtual InvokeResult Invoke(
            const Json::Value& parameters) noexcept = 0;

        // Deployment Status Json
        virtual Json::Value GetDeploymentStatusJson() const = 0;

        // Reported Schema
        virtual ReportedSchema GetReportedSchema() const = 0;

        // MdmServer
        virtual void SetMdmServer(std::shared_ptr<IMdmServer> iMdmServer) = 0;

        // DMClient
        virtual void SetHandlerHost(std::shared_ptr<IRawHandlerHost> iPluginHost) = 0;

        // Virtual destructor
        virtual ~IRawHandler() {};
    };
}}}}
