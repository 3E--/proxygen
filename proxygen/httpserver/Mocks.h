/*
 *  Copyright (c) 2014, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#pragma once

#include <gmock/gmock.h>
#include <proxygen/httpserver/RequestHandler.h>
#include <proxygen/httpserver/ResponseHandler.h>

namespace proxygen {

/**
 * Mocks to help with testing
 */

class MockResponseHandler : public ResponseHandler {
 public:
  explicit MockResponseHandler(RequestHandler* h): ResponseHandler(h) {
  }

  GMOCK_METHOD1_(, noexcept,, sendHeaders, void(HTTPMessage&));
  GMOCK_METHOD1_(, noexcept,, sendChunkHeader, void(size_t));
  GMOCK_METHOD1_(, noexcept,, sendBody, void(std::shared_ptr<folly::IOBuf>));
  GMOCK_METHOD0_(, noexcept,, sendChunkTerminator, void());
  GMOCK_METHOD0_(, noexcept,, sendEOM, void());
  GMOCK_METHOD0_(, noexcept,, sendAbort, void());
  GMOCK_METHOD0_(, noexcept,, refreshTimeout, void());
  GMOCK_METHOD0_(, noexcept,, pauseIngress, void());
  GMOCK_METHOD0_(, noexcept,, resumeIngress, void());
  const folly::TransportInfo& getSetupTransportInfo() const noexcept {
    return transportInfo;
  }
  MOCK_CONST_METHOD1(getCurrentTransportInfo, void(folly::TransportInfo*));

  void sendBody(std::unique_ptr<folly::IOBuf> body) noexcept override {
    if (body) {
      sendBody(std::shared_ptr<folly::IOBuf>(body.release()));
    } else {
      sendBody(std::shared_ptr<folly::IOBuf>(nullptr));
    }
  }

  folly::TransportInfo transportInfo;
};

class MockRequestHandler : public RequestHandler {
 public:
  GMOCK_METHOD1_(, noexcept,, setResponseHandler, void(ResponseHandler*));
  GMOCK_METHOD1_(, noexcept,, onRequest, void(std::shared_ptr<HTTPMessage>));
  GMOCK_METHOD1_(, noexcept,, onBody, void(std::shared_ptr<folly::IOBuf>));
  GMOCK_METHOD1_(, noexcept,, onUpgrade, void(UpgradeProtocol));
  GMOCK_METHOD0_(, noexcept,, onEOM, void());
  GMOCK_METHOD0_(, noexcept,, requestComplete, void());
  GMOCK_METHOD1_(, noexcept,, onError, void(ProxygenError));
  GMOCK_METHOD0_(, noexcept,, onEgressPaused, void());
  GMOCK_METHOD0_(, noexcept,, onEgressResumed, void());

  void onRequest(std::unique_ptr<HTTPMessage> headers) noexcept override {
    onRequest(std::shared_ptr<HTTPMessage>(headers.release()));
  }

  void onBody(std::unique_ptr<folly::IOBuf> body) noexcept override {
    if (body) {
      onBody(std::shared_ptr<folly::IOBuf>(body.release()));
    } else {
      onBody(std::shared_ptr<folly::IOBuf>(nullptr));
    }
  }
};

}
