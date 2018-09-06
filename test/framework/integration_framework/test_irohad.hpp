/**
 * Copyright Soramitsu Co., Ltd. 2018 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef IROHA_TESTIROHAD_HPP
#define IROHA_TESTIROHAD_HPP

#include "common/types.hpp"
#include "cryptography/keypair.hpp"
#include "main/application.hpp"

namespace integration_framework {
  /**
   * Class for integration testing of Irohad.
   */
  class TestIrohad : public Irohad {
   public:
    TestIrohad(const std::string &block_store_dir,
               const std::string &pg_conn,
               size_t torii_port,
               size_t internal_port,
               size_t max_proposal_size,
               std::chrono::milliseconds proposal_delay,
               std::chrono::milliseconds vote_delay,
               const shared_model::crypto::Keypair &keypair,
               bool is_mst_supported)
        : Irohad(block_store_dir,
                 pg_conn,
                 torii_port,
                 internal_port,
                 max_proposal_size,
                 proposal_delay,
                 vote_delay,
                 keypair,
                 is_mst_supported) {}

    auto &getCommandService() {
      return command_service;
    }

    auto &getQueryService() {
      return query_service;
    }

    auto &getPeerCommunicationService() {
      return pcs;
    }

    auto &getCryptoSigner() {
      return crypto_signer_;
    }

    auto getStatusBus() {
      return status_bus_;
    }

    void run() override {
      using iroha::operator|;
      internal_server = std::make_unique<ServerRunner>(
          "127.0.0.1:" + std::to_string(internal_port_));
      internal_server->append(ordering_init.service)
          .append(yac_init.consensus_network)
          .append(loader_init.service)
          .run()
          .match([](iroha::expected::Value<int>) {},
                 [](iroha::expected::Error<std::string> e) {
                   BOOST_ASSERT_MSG(false, e.error.c_str());
                 });
      log_->info("===> iroha initialized");
      pcs->on_commit()
          .start_with(
              storage->createBlockQuery() |
              [](const auto &block_query) {
                return block_query->getTopBlock().match(
                    [](iroha::expected::Value<
                        std::shared_ptr<shared_model::interface::Block>> &block)
                        -> iroha::synchronizer::SynchronizationEvent {
                      return {rxcpp::observable<>::just(block.value),
                              iroha::synchronizer::SynchronizationOutcomeType::
                                  kCommit};
                    },
                    [](iroha::expected::Error<std::string> &error)
                        -> iroha::synchronizer::SynchronizationEvent {
                      throw std::runtime_error("Failed to get the top block: "
                                               + error.error);
                    });
              })
          .subscribe(ordering_init.notifier.get_subscriber());
    }

    void terminate() {
      if (internal_server) {
        internal_server->shutdown();
      }
    }
  };
}  // namespace integration_framework

#endif  // IROHA_TESTIROHAD_HPP
