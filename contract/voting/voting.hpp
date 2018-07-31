
#include <eosiolib/eosio.hpp>
#include <eosiolib/types.hpp>
#include <eosiolib/currency.hpp>
#include <eosiolib/public_key.hpp>
#include <eosiolib/asset.hpp>
#include <string>
#include <utility>
#include <vector>
#include <string>
#include <eosiolib/eosio.hpp>
#include <eosiolib/time.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/contract.hpp>
#include <eosiolib/crypto.h>
#include <algorithm>
#include <cmath>

//@abi table voters i64
struct voter_info {
      account_name                owner = 0;
      std::vector<account_name>   candidates;
      std::vector<uint16_t>   amount_votes;
      uint64_t primary_key()const { return owner; }

      EOSLIB_SERIALIZE( voter_info, (owner)(candidates)(amount_votes) )
};
typedef eosio::multi_index< N(voters), voter_info>  voters_table;

//@abi table candidates i64
struct candidate{
      account_name          owner;
      uint64_t                total_votes = 0;
      eosio::public_key     candidate_key;
      std::string           url;
      uint64_t primary_key()const { return owner; }
      EOSLIB_SERIALIZE( candidate, (owner)(total_votes)(candidate_key)(url) )
};
typedef eosio::multi_index< N(candidates), candidate>  candidates_table;

class voting : public eosio::contract {
  public:
      voting( account_name _self);
      //@abi action
      void regcandidate( const account_name account, const eosio::public_key& account_key, const std::string& url);
      void vote( const account_name voter, const account_name candidate);

  private:
      void update_votes( const account_name voter, const account_name candidate);

      voters_table _voters;
      candidates_table _candidates;

};

EOSIO_ABI( voting, (regcandidate)(vote) )
