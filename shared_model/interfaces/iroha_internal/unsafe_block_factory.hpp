/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_UNSAFE_BLOCK_FACTORY_HPP
#define IROHA_UNSAFE_BLOCK_FACTORY_HPP

#include "interfaces/iroha_internal/block_variant.hpp"

namespace shared_model {
  namespace interface {
    /**
     * UnsafeBlockFactory creates block without any validation
     */
    class UnsafeBlockFactory {
     public:
       /**
        * Create block without any validation
        * @param height - block height
        * @param prev_hash - hash of the previous block
        * @param created_time - time the block is created
        * @param txs - list of transactions. If it empty, EmptyBlock is creted
        * @return BlockVariant with block or empty block
        */
      virtual BlockVariant unsafeCreateBlock(
          types::HeightType height,
          const types::HashType &prev_hash,
          types::TimestampType created_time,
          const types::TransactionsCollectionType &txs) = 0;

      virtual ~UnsafeBlockFactory() = default;
    };
  }  // namespace interface
}  // namespace shared_model

#endif  // IROHA_UNSAFE_BLOCK_FACTORY_HPP
