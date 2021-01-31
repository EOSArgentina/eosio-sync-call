#include <eosio/eosio.hpp>
using namespace eosio;
using eosio::internal_use_do_not_use::require_auth2;

typedef std::vector<char> bytes;

struct [[eosio::table]] callback_data {
    uint64_t id;
    uint64_t caller;
    uint64_t handler;
    bytes    data;

    uint64_t primary_key()const { return id; }
};
typedef eosio::multi_index< "cdata"_n, callback_data > cdata_table;

using no_param_handler_t = void(*)(void);

template<typename T>
using handler_t = void(*)(const T&);

using caller_t = void(*)(void*, const bytes&);

void call_handler_np(void* h_ptr, const bytes&) {
    auto handler = reinterpret_cast<no_param_handler_t>(h_ptr);
    handler();
}

template <typename T>
void call_handler(void* h_ptr, const bytes& data) {
    auto obj = unpack<T>(data);
    auto handler = reinterpret_cast<handler_t<T>>(h_ptr);
    handler(obj);
}

void store_and_call(const action& act, void* caller_ptr, void* handler_ptr, const bytes& data) {
    auto self = current_receiver();
    cdata_table cdata(self, self.value );
    
    auto itr = cdata.emplace(self, [&](auto& obj) {
        obj.id = cdata.available_primary_key();
        obj.caller = (uint64_t)caller_ptr;
        obj.handler = (uint64_t)handler_ptr;
        obj.data = data;
    });

    eosio::check(!(act.account == self && act.name == "chandler"_n), "naaa");

    act.send();
    action{
        eosio::permission_level{self, "active"_n},
        self, "chandler"_n,
        std::make_tuple(uint64_t(itr->id))
    }.send();
}

template <typename T, typename Handler>
void call(const action& act, const T& ctx, Handler&& handler) {
    caller_t caller_ptr = &call_handler<T>;
    handler_t<T> handler_ptr = handler;

    store_and_call(act,
        reinterpret_cast<void *>(caller_ptr), 
        reinterpret_cast<void *>(handler_ptr), 
        pack(ctx)
    );
};

template <typename Handler>
void call(const action& act, Handler&& h) {
    caller_t caller_ptr = &call_handler_np;
    no_param_handler_t handler_ptr = h;

    store_and_call(act,
        reinterpret_cast<void *>(caller_ptr), 
        reinterpret_cast<void *>(handler_ptr), 
        {}
    );
};

struct [[eosio::contract(CONTRACT_NAME)]] contract_ex : contract {
    using contract::contract;

    ACTION chandler(uint64_t id) {
        require_auth(get_self());

        cdata_table cdata(get_self(), get_self().value);
        const auto& obj = cdata.get(id);
        
        auto caller_fnc = reinterpret_cast<caller_t>((void*)obj.caller);
        caller_fnc((void*)obj.handler, obj.data);

        cdata.erase(obj);
    }
};
