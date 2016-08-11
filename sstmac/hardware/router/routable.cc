#include <sstmac/hardware/router/routable.h>

#define enumcase(x) case x: return #x

namespace sstmac {
namespace hw {

geometry_routable::geometry_routable(node_id toaddr, node_id fromaddr)
  : toaddr_(toaddr), fromaddr_(fromaddr)
{
}

void
geometry_routable::serialize_order(serializer& ser)
{
  ser & toaddr_;
  ser & fromaddr_;
  ser & path_;
}

namespace routing {

const char*
tostr(routing::algorithm_t algo)
{
  switch(algo) {
      enumcase(minimal);
      enumcase(valiant);
      enumcase(deflt);
      enumcase(ugal);
    default:
      spkt_throw_printf(sprockit::value_error,
                       "invalud routing algorithm enum %d",
                       algo);
  }
  return 0;
}

}

}
}
