// Created by amoylel on 14/02/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_IPC_4710140E_4382_4A3F_BBC2_B62BD5C5852A_HPP__
#define AMO_IPC_4710140E_4382_4A3F_BBC2_B62BD5C5852A_HPP__

#include <amo/ipc/class_transfer.hpp>
#include <amo/ipc/ipc_emitter.hpp>
#include <amo/ipc/transfer_manager.hpp>
#include <amo/ipc/ipc_transfer.hpp>
#include <amo/ipc/ipc_emitter.hpp>
#include <amo/ipc/ipc_impl.hpp>


namespace amo {
    namespace ipc {
        typedef amo::ipc_impl<ipc_server> server;
        typedef amo::ipc_impl<ipc_client> client;
    }
}



#endif //AMO_IPC_4710140E_4382_4A3F_BBC2_B62BD5C5852A_HPP__