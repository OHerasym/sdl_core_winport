/*
 *
 * Copyright (c) 2013, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include "transport_manager/tcp/tcp_socket_connection.h"

#include "utils/logger.h"
#include "utils/threads/thread.h"
#include "transport_manager/tcp/tcp_device.h"
#include "transport_manager/transport_adapter/transport_adapter_controller.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

TcpSocketConnection::TcpSocketConnection(const DeviceUID& device_uid,
                                         const ApplicationHandle& app_handle,
                                         TransportAdapterController* controller)
    : ThreadedSocketConnection(device_uid, app_handle, controller) {}

TcpSocketConnection::~TcpSocketConnection() {}

bool TcpSocketConnection::Establish(ConnectError** error) {
  return true;
}

TcpServerOiginatedSocketConnection::TcpServerOiginatedSocketConnection(
    const DeviceUID& device_uid,
    const ApplicationHandle& app_handle,
    TransportAdapterController* controller)
    : ThreadedSocketConnection(device_uid, app_handle, controller) {}

TcpServerOiginatedSocketConnection::~TcpServerOiginatedSocketConnection() {}

bool TcpServerOiginatedSocketConnection::Establish(ConnectError** error) {
  LOGGER_AUTO_TRACE(logger_);
  DCHECK(error);
  LOGGER_DEBUG(logger_, "error " << error);
  DeviceSptr device = controller()->FindDevice(device_handle());
  if (!device.valid()) {
    LOGGER_ERROR(logger_, "Device " << device_handle() << " not found");
    *error = new ConnectError();
    return false;
  }
  TcpDevice* tcp_device = static_cast<TcpDevice*>(device.get());

  const int port = tcp_device->GetApplicationPort(application_handle());
  if (-1 == port) {
    LOGGER_ERROR(logger_,
                 "Application port for " << application_handle()
                                         << " not found");
    *error = new ConnectError();
    return false;
  }

  const std::string address = tcp_device->Address().ToString();
  LOGGER_DEBUG(logger_, "Connecting to " << address << ":" << port);
  utils::TcpSocketConnection connection;
  if (!connection.Connect(tcp_device->Address(), port)) {
    LOGGER_ERROR(logger_,
                 "Failed to connect to the server " << address << ":" << port
                                                    << " for application "
                                                    << application_handle());
    *error = new ConnectError();
    return false;
  }
  // Transfer ownership on the connection
  SetSocket(connection);
  return true;
}

}  // namespace transport_adapter
}  // namespace transport_manager
