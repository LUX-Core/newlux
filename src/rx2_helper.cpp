// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2019 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include <validation.h>
#include <uint256.h>
#include <chainparams.h>

uint256 GetRandomXSeed(const uint32_t& nHeight)
{  
    static uint256 current_key_block;
    const Consensus::Params& consensusParams = Params().GetConsensus();
    uint32_t SeedStartingHeight = consensusParams.RX2SeedHeight;
    uint32_t SeedInterval = consensusParams.RX2SeedInterval;
    uint32_t SwitchKey = SeedStartingHeight % SeedInterval;
    if (current_key_block == uint256()) {
        LOCK(cs_main);
        current_key_block = ::ChainActive().Genesis()->GetBlockHash();
    }
    if (nHeight < SeedStartingHeight) {
        return current_key_block;
    }
    uint32_t remainer = nHeight % SeedInterval;

    uint32_t first_check = nHeight - remainer;
    uint32_t second_check = nHeight - SeedInterval - remainer;

    if (nHeight > nHeight - remainer + SwitchKey) {
        if (nHeight > first_check) {
            LOCK(cs_main);
            current_key_block = ::ChainActive()[first_check-SeedStartingHeight]->GetBlockHash();
        }
    } else {
        if (nHeight > second_check) {
            LOCK(cs_main);
            current_key_block = ::ChainActive()[second_check-SeedStartingHeight]->GetBlockHash();
        }
        
    }
    return current_key_block;
}