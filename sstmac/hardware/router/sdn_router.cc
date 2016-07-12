#include <cassert>

#include "sstmac/hardware/router/sdn_router.h"
#include "sprockit/sim_parameters.h"

using namespace sstmac;
using namespace sstmac::sw;
using namespace sstmac::hw;


// SpktRegister("sdn | SDN", router, sdn_router);

bool
sdn_router::Packet_Metadata::operator<(const Packet_Metadata & right) const {
    return (data < right.data);
}

sdn_router::sdn_router()
    : rtable_max_size(0)
{}

sdn_router::~sdn_router(){}

void
sdn_router::route(sstmac::hw::packet* pkt_)
{
    // Cast to expected packet type
    // Now you can you use whatever metadata you want to route
    assert(pkt_ != nullptr);
    sdn_packet* pkt = dynamic_cast<sdn_packet*>(pkt_);


    //TODO your routing logic

    // check if packet routing algorithm is SDN?

    // match with routing table
    Packet_Metadata * metadata = get_packet_metadata(pkt);
    if (!metadata){
        return;
    }

    // pkt -> outport = rtable[*metadata];
    return;
}

void
sdn_router::init_factory_params(sprockit::sim_parameters *params)
{
    //always call up to parent first
    router::init_factory_params(params);

    // maximum size of routing table; default is "infinite"
    rtable_max_size = params -> get_optional_int_param("rtable_max_size", (1 << ((sizeof(int) << 3) - 1) - 1));
}

sdn_router::Packet_Metadata *
sdn_router::get_packet_metadata(sdn_packet * pkt) const {
    if (!pkt){
        return nullptr;
    }

    Packet_Metadata * metadata = new Packet_Metadata;
    // fill in metadata values here, or get them directly from packet
    return metadata;
}

int
sdn_router::add_new_route(sdn_packet * pkt, const int outport){
    Packet_Metadata * metadata = get_packet_metadata(pkt);
    if (!metadata){
        return -1;
    }

    const int rc = add_new_route(*metadata, outport);
    delete metadata; // depends on where meta data comes from
    return rc;
}

int
sdn_router::add_new_route(sdn_router::Packet_Metadata & metadata, const int outport){
    // prevent routing table from getting too big (currently no recent usage replacement policy)
    if (rtable.size() >= rtable_max_size){
        return -1;
    }

    // replace old output port
    return (rtable[metadata] = outport);
}

int
sdn_router::default_route(sdn_router::Packet_Metadata & metadata) const {
    return -1;
}
