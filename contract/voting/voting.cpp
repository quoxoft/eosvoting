
#include "voting.hpp"
#include <eosiolib/transaction.hpp>
#include <eosiolib/currency.hpp>

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

voting::voting( account_name _self )
:eosio::contract(_self),
_voters(_self,_self),
_candidates(_self,_self){}

void voting::regcandidate( const account_name account, const eosio::public_key& account_key, const std::string& url){
      eosio_assert( url.size() < 512, "url too long" );
      eosio_assert( account_key != eosio::public_key(), "public key should not be the default value" );
      require_auth( account );

      auto acc = _candidates.find( account );

      if ( acc != _candidates.end() ) {
         _candidates.modify( acc, account, [&]( auto& info ){
               info.candidate_key = account_key;
               info.url          = url;
            });
      } else {
         _candidates.emplace( account, [&]( auto& info ){
               info.owner         = account;
               info.total_votes   = 0;
               info.candidate_key  = account_key;
               info.url           = url;
         });
      }
}


void voting::vote( const account_name voter,account_name  candidate){
      require_auth( voter );

        // update voting power
    auto to_candidate = _candidates.find(candidate);
    if( to_candidate != _candidates.end() ) {
            _candidates.modify( to_candidate, 0, [&]( auto& p ) {
               p.total_votes ++;
               if ( p.total_votes < 0 ) {
                  p.total_votes = 0;
               }
            });
    } else {
        eosio_assert( false, "candidate is not registered" );
    }


    // update voter infor 
    auto from_voter = _voters.find(voter);
    if( from_voter == _voters.end() ) {
        from_voter = _voters.emplace( voter, [&]( auto& v ) {
            v.owner  = voter;
            v.candidates.push_back(candidate);
            v.amount_votes.push_back(1);
        });
    } else {
         _voters.modify( from_voter, 0, [&]( auto& v ) {
            auto it = std::find(v.candidates.begin(), v.candidates.end(), candidate);
            if (it == v.candidates.end())
            {
                    v.candidates.push_back(candidate);
                    v.amount_votes.push_back(1);
            } else
            {
                auto index = std::distance( v.candidates.begin(), it);
                v.amount_votes[index]++;
            }       
        });
    }

}
