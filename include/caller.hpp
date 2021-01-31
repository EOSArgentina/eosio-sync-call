#include <eosio/eosio.hpp>
using namespace eosio;

#define CONTRACT_NAME "caller"
#include "contract_ex.hpp"

CONTRACT caller : public contract_ex {
   public:
      using contract_ex::contract_ex;

      ACTION test1();
      ACTION test2();
      ACTION test3();

      ACTION callme() {
         print("soy callme");
      }
};