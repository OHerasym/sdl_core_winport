/*
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

#include "hmi_message_handler/hmi_message_handler_impl.h"
#include "config_profile/profile.h"
#include "utils/logger.h"

namespace hmi_message_handler {

CREATE_LOGGERPTR_GLOBAL(logger_, "HMIMessageHandler")

HMIMessageHandlerImpl::HMIMessageHandlerImpl()
    : observer_(NULL)
    , messages_to_hmi_(
          "HMH ToHMI",
          this,
          threads::ThreadOptions(
              profile::Profile::instance()->thread_min_stack_size()))
    , messages_from_hmi_(
          "HMH FromHMI",
          this,
          threads::ThreadOptions(
              profile::Profile::instance()->thread_min_stack_size())) {}

HMIMessageHandlerImpl::~HMIMessageHandlerImpl() {
  LOGGER_AUTO_TRACE(logger_);
  messages_to_hmi_.Shutdown();
  messages_from_hmi_.Shutdown();
  set_message_observer(NULL);
}

void HMIMessageHandlerImpl::OnMessageReceived(MessageSharedPointer message) {
  LOGGER_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(observer_locker_);
  if (!observer_) {
    LOGGER_WARN(logger_, "No HMI message observer set!");
    return;
  }
  messages_from_hmi_.PostMessage(impl::MessageFromHmi(message));
}

void HMIMessageHandlerImpl::SendMessageToHMI(MessageSharedPointer message) {
  LOGGER_INFO(logger_, "HMIMessageHandlerImpl::~sendMessageToHMI()");
  messages_to_hmi_.PostMessage(impl::MessageToHmi(message));
}

void HMIMessageHandlerImpl::set_message_observer(HMIMessageObserver* observer) {
  LOGGER_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(observer_locker_);
  observer_ = observer;
}

void HMIMessageHandlerImpl::OnErrorSending(MessageSharedPointer message) {
  LOGGER_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(observer_locker_);
  if (!observer_) {
    LOGGER_WARN(logger_, "No HMI message observer set!");
    return;
  }
  observer_->OnErrorSending(message);
}

void HMIMessageHandlerImpl::AddHMIMessageAdapter(HMIMessageAdapter* adapter) {
  LOGGER_AUTO_TRACE(logger_);
  message_adapters_.insert(adapter);
}

void HMIMessageHandlerImpl::RemoveHMIMessageAdapter(
    HMIMessageAdapter* adapter) {
  LOGGER_AUTO_TRACE(logger_);
  if (adapter != NULL) {
    message_adapters_.erase(adapter);
  }
}

void HMIMessageHandlerImpl::Handle(const impl::MessageFromHmi message) {
  LOGGER_INFO(logger_, "Received message from hmi");
  sync_primitives::AutoLock lock(observer_locker_);
  if (!observer_) {
    LOGGER_ERROR(logger_, "Observer is not set for HMIMessageHandler");
    return;
  }

  observer_->OnMessageReceived(message);
  LOGGER_INFO(logger_, "Message from hmi given away.");
}
void HMIMessageHandlerImpl::Handle(const impl::MessageToHmi message) {
  for (std::set<HMIMessageAdapter*>::iterator it = message_adapters_.begin();
       it != message_adapters_.end();
       ++it) {
    (*it)->SendMessageToHMI(message);
  }
}

}  //  namespace hmi_message_handler
