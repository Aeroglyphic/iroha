/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_CONSENSUS_CACHE_H
#define IROHA_CONSENSUS_CACHE_H

#include <boost/optional.hpp>

/**
 * Stores pointer to some data came from the consensus
 */
template <typename DataType,
          template <typename> class WrapperType = boost::optional>
class ConsensusCache {
 public:
  using DataPointer = std::shared_ptr<DataType>;
  using WrappedData = WrapperType<DataPointer>;

  /**
   * Insert data to the cache
   * @param data to be inserted
   */
  virtual void insert(DataPointer data) = 0;

  /**
   * Get data from the cache
   * @return wrapped data
   */
  virtual WrappedData get() const = 0;

  /**
   * Delete data inside the cache
   */
  virtual void release() = 0;

  virtual ~ConsensusCache() = default;
};

#endif  // IROHA_CONSENSUS_CACHE_H
