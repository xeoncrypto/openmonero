//
// Created by mwo on 14/12/16.
//

#ifndef RESTBED_XMR_CURRENTBLOCKCHAINSTATUS_H
#define RESTBED_XMR_CURRENTBLOCKCHAINSTATUS_H

#define MYSQLPP_SSQLS_NO_STATICS 1

#include "MicroCore.h"
#include "ssqlses.h"

#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>


namespace xmreg {

using namespace std;

class TxSearch;
class XmrAccount;
class MySqlAccounts;


static mutex searching_threads_map_mtx;
static mutex getting_mempool_txs;

/*
* This is a thread class
*/
struct CurrentBlockchainStatus
{
    static string blockchain_path;

    static atomic<uint64_t> current_height;

    static string deamon_url;

    static bool testnet;

    static std::thread m_thread;

    static bool is_running;

    static uint64_t refresh_block_status_every_seconds;


    static string   import_payment_address;
    static string   import_payment_viewkey;
    static uint64_t import_fee;
    static uint64_t spendable_age;

    static account_public_address address;
    static secret_key             viewkey;

    // vector of mempool transactions that all threads
    // can refer to
    //           <recieved_time, transaction>
    static vector<pair<uint64_t, transaction>> mempool_txs;

    // map that will keep track of search threads. In the
    // map, key is address to which a running thread belongs to.
    // make it static to guarantee only one such map exist.
    static map<string, shared_ptr<TxSearch>> searching_threads;

    // since this class monitors current status
    // of the blockchain, its seems logical to
    // make object for accessing the blockchain here
    static xmreg::MicroCore mcore;
    static cryptonote::Blockchain *core_storage;

    static
    void start_monitor_blockchain_thread();

    static
    uint64_t
    get_current_blockchain_height();

    static void
    set_blockchain_path(const string &path);

    static void
    set_testnet(bool is_testnet);

    static bool
    init_monero_blockchain();

    static bool
    is_tx_unlocked(uint64_t tx_blk_height);

    static bool
    get_block(uint64_t height, block &blk);

    static bool
    get_block_txs(const block &blk, list <transaction> &blk_txs);

    static bool
    tx_exist(const crypto::hash& tx_hash);

    static bool
    tx_exist(const string& tx_hash_str);

    static bool
    get_tx_with_output(uint64_t output_idx, uint64_t amount,
                       transaction& tx, uint64_t& output_idx_in_tx);

    static bool
    get_output_keys(const uint64_t& amount,
                    const vector<uint64_t>& absolute_offsets,
                    vector<cryptonote::output_data_t>& outputs);

    static bool
    get_amount_specific_indices(const crypto::hash& tx_hash,
                                vector<uint64_t>& out_indices);

    static bool
    get_random_outputs(const vector<uint64_t>& amounts,
                       const uint64_t& outs_count,
                       vector<COMMAND_RPC_GET_RANDOM_OUTPUTS_FOR_AMOUNTS::outs_for_amount>& found_outputs);

    static bool
    commit_tx(const string& tx_blob);

    static bool
    read_mempool();

    static vector<pair<uint64_t, transaction>>
    get_mempool_txs();

    static bool
    search_if_payment_made(
            const string& payment_id_str,
            const uint64_t& desired_amount,
            string& tx_hash_with_payment);


    static string
    get_payment_id_as_string(const transaction& tx);

    static output_data_t
    get_output_key(uint64_t amount,
                   uint64_t global_amount_index);

    // definitions of these function are at the end of this file
    // due to forward declaraions of TxSearch
    static bool
    start_tx_search_thread(XmrAccount acc);

    static bool
    ping_search_thread(const string& address);

    static bool
    get_xmr_address_viewkey(const string& address_str,
                            account_public_address& address,
                            secret_key& viewkey);
    static bool
    find_txs_in_mempool(const string& address_str,
                        json& transactions);

    static bool
    set_new_searched_blk_no(const string& address,
                            uint64_t new_value);

    static void
    clean_search_thread_map();

    /*
     * The frontend requires rct field to work
     * the filed consisitct of rct_pk, mask, and amount.
     */
    static tuple<string, string, string>
    construct_output_rct_field(
            const uint64_t global_amount_index,
            const uint64_t out_amount);

};


}
#endif //RESTBED_XMR_CURRENTBLOCKCHAINSTATUS_H
