/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_ON_DEMAND_CACHE_HPP
#define IROHA_ON_DEMAND_CACHE_HPP

#include <boost/circular_buffer.hpp>
#include <queue>

#include "ordering/impl/og_cache/og_cache.hpp"

namespace iroha {
  namespace ordering {
    namespace cache {

      class OnDemandCache : public OgCache {
       public:
        void addToBack(const BatchesListType &batches) override;

        BatchesListType clearFrontAndGet() override;

        void up() override;

        void remove(const BatchesListType &batches) override;

        virtual const BatchesListType &front() const override;

        virtual const BatchesListType &back() const override;

       private:
        using BatchesQueueType =
            std::queue<BatchesListType,
                       boost::circular_buffer<BatchesListType>>;
        BatchesQueueType queue_{
            boost::circular_buffer<BatchesListType>(3, BatchesListType{})};
      };

    }  // namespace cache
  }    // namespace ordering
}  // namespace iroha

#endif  // IROHA_ON_DEMAND_CACHE_HPP