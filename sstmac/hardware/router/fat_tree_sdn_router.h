#ifndef FAT_TREE_SDN_ROUTER_H
#define FAT_TREE_SDN_ROUTER_H

#include <map>

#include <sstmac/hardware/router/router.h>
#include "sstmac/hardware/packet_flow/sdn_packet.h"


/**
 * @brief The fat_tree_sdn_router class
 * Your custom router that will make routing decisions based on tables
 * and will operate on sdn_packet types
 */
class fat_tree_sdn_router : public sstmac::hw::router
{
public:
    // packet metadata to match against
    struct Packet_Metadata {
        int * data;
        sstmac::sw::app_id app_id;
        sstmac::sw::flow_id flow_id;
        sstmac::node_id toaddr;
        sstmac::node_id fromaddr;

        bool operator<(const Packet_Metadata & right) const;
    };

private:
    // packet metadata -> (out port, direction)
    std::map <Packet_Metadata, int> rtable;
    std::size_t rtable_max_size;

public:
    fat_tree_sdn_router();
    ~fat_tree_sdn_router();

    virtual void
    route(sstmac::hw::packet *pkt_);

    virtual void
    init_factory_params(sprockit::sim_parameters *params);

    virtual void
    productive_paths_to_switch(
        sstmac::switch_id dst,
        sstmac::hw::routing_info::path_set& paths){}

private:
    Packet_Metadata *
    get_packet_metadata(sdn_packet * pkt) const;                     // get key value for comparing with routing table

public:
    int
    add_new_route(sdn_packet * pkt, const int outport);              // add new route to table (old outports are replaced)

    int
    add_new_route(Packet_Metadata & metadata, const int outport);    // add new route to table (old outports are replaced)

    int
    default_route(Packet_Metadata & metadata) const;                 // route not found, so run some normal routing algorithm

};

#endif // FAT_TREE_SDN_ROUTER_H
