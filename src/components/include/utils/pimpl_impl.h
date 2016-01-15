/*
 * Copyright (c) 2016, Ford Motor Company
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
#ifndef SRC_COMPONENTS_INCLUDE_UTILS_PIMPL_IMPL_H_
#define SRC_COMPONENTS_INCLUDE_UTILS_PIMPL_IMPL_H_

#include <algorithm>
#include "utils/pimpl.h"

template <typename Impl>
utils::Pimpl<Impl>::Pimpl() {
  impl_ = new Impl;
}

template <typename Impl>
utils::Pimpl<Impl>::Pimpl(utils::Pimpl<Impl>& rhs) {
  Swap(rhs);
}

template <typename Impl>
utils::Pimpl<Impl>::~Pimpl() {
  delete impl_;
}

template <typename Impl>
utils::Pimpl<Impl>& utils::Pimpl<Impl>::operator=(utils::Pimpl<Impl>& rhs) {
  Swap(rhs);
  return *this;
}

template <typename Impl>
Impl* utils::Pimpl<Impl>::operator->() const {
  return impl_;
}

template <typename Impl>
Impl& utils::Pimpl<Impl>::operator&() const {
  return *impl_;
}

template <typename Impl>
void utils::Pimpl<Impl>::Swap(utils::Pimpl<Impl>& rhs) {
  std::swap(this->impl_, rhs.impl_);
}

#endif  // SRC_COMPONENTS_INCLUDE_UTILS_PIMPL_IMPL_H_
