#ifndef SDN_ROUTER_H
#define SDN_ROUTER_H

#include <map>

#include <sstmac/hardware/router/router.h>
#include "sstmac/hardware/packet_flow/sdn_packet.h"


/**
 * @brief The sdn_router class
 * Your custom router that will make routing decisions based on tables
 * and will operate on sdn_packet types
 */
class sdn_router : public sstmac::hw::router
{
public:
    // packet metadata to match against
    struct Packet_Metadata {
        int * data;
        bool operator<(const Packet_Metadata & right) const;
    };

private:
    // packet metadata -> (out port, direction)
    std::map <Packet_Metadata, int> rtable;
    std::size_t rtable_max_size;

public:
    sdn_router();
    ~sdn_router();

    virtual void
    route(sstmac::hw::packet *pkt_);

    virtual void
    init_factory_params(sprockit::sim_parameters *params);

private:
    Packet_Metadata *
    get_packet_metadata(sdn_packet * pkt) const;       // get key value for comparing with routing table

public:
    int
    add_new_route(sdn_packet * pkt, const int outport);              // add new route to table (old outports are replaced)

    int
    add_new_route(Packet_Metadata & metadata, const int outport);    // add new route to table (old outports are replaced)

    int
    default_route(Packet_Metadata & metadata) const;                 // route not found, so run some normal routing algorithm

};

#endif // SDN_ROUTER_H
