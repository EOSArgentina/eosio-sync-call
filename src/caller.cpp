#include <caller.hpp>

struct mycontext {
   action   myact;
   uint64_t mynum;
};

ACTION caller::test1() {

   auto req = action{
      eosio::permission_level{get_self(), "active"_n},
      "dummycaller2"_n, "test"_n,
      std::make_tuple(uint64_t(1001))
   };

   //simulate lambda capture by using the sent action as context
   call(req, req, [](const auto& req){
      eosio::print("aca volvi1");
      call(req, req, [](const auto& req){
         eosio::print("aca volvi2");
         call(req, req, [](const auto& req){
            eosio::print("aca volvi3");
         });
      });
   });

}

ACTION caller::test2() {

   auto req = action{
      eosio::permission_level{get_self(), "active"_n},
      "dummycaller2"_n, "test"_n,
      std::make_tuple(uint64_t(999))
   };

   //no context
   call(req, [](){
      eosio::print("aca volvi1");
   });
}

ACTION caller::test3() {

   auto req = action{
      eosio::permission_level{get_self(), "active"_n},
      "dummycaller2"_n, "test"_n,
      std::make_tuple(uint64_t(999))
   };

   //custom context
   auto ctx = mycontext{req, 555};
   call(req, ctx, [](const auto& ctx){
      eosio::print("aca volvi => ", ctx.mynum);
   });
}
