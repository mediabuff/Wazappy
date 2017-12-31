// The Wazappy project implements a WASAPI-based sound engine for Windows UWP and desktop apps.
// https://github.com/RobJellinghaus/Wazappy
// Licensed under the MIT License.
// This file based on WindowsAudioSession sample from https://github.com/Microsoft/Windows-universal-samples

#include "pch.h"
#include "WASAPISession.h"

using namespace Windows::System::Threading;
using namespace Wazappy;

NodeId WASAPISession::s_nextNodeId{};
std::mutex WASAPISession::s_mutex{};
std::map<NodeId, ComPtr<WASAPIDevice>> WASAPISession::s_deviceMap{};

NodeId WASAPISession::RegisterDevice(const ComPtr<WASAPIDevice>& device)
{
	std::lock_guard<std::mutex> guard(s_mutex);
	NodeId next = ++s_nextNodeId;
	s_deviceMap.emplace(next, device);
	return next;
}

void WASAPISession::UnregisterDevice(NodeId nodeId)
{
	std::lock_guard<std::mutex> guard(s_mutex);
	Contract::Requires(s_deviceMap.find(nodeId) != s_deviceMap.end(), L"Device with given ID must exist");
	s_deviceMap.erase(nodeId);
}

const ComPtr<WASAPIDevice>& WASAPISession::GetDevice(NodeId nodeId)
{
	std::lock_guard<std::mutex> guard(s_mutex);
	const auto& found = s_deviceMap.find(nodeId);
	Contract::Requires(found != s_deviceMap.end(), L"Device with given ID must exist");
	return found->second;
}
